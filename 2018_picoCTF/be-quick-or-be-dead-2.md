# be-quick-or-be-dead-2
Reversing, 275 points

## Description:
> As you enjoy this [music](https://www.youtube.com/watch?v=CTt1vk9nM9c) even more, another executable **be-quick-or-be-dead-2** shows up. Can you run this fast enough too?

Attached was a binary file.

## Solution:

Let's run the file:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# ./be-quick-or-be-dead-2
Be Quick Or Be Dead 2
=====================

Calculating key...
You need a faster machine. Bye bye.
```

Time to analyze the program. We'll use Radare2.

```console
r2 be-quick-or-be-dead-2
```


Analyze the binary with `aa` and list the functions with `afl`:
```
[0x004005a0]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x004005a0]> afl
0x004004e8    3 26           sym._init
0x00400520    1 6            sym.imp.putchar
0x00400530    1 6            sym.imp.puts
0x00400540    1 6            sym.imp.printf
0x00400550    1 6            sym.imp.alarm
0x00400560    1 6            sym.imp.__libc_start_main
0x00400570    1 6            sym.imp.__sysv_signal
0x00400580    1 6            sym.imp.exit
0x00400590    1 6            fcn.00400590
0x004005a0    1 41           entry0
0x004005d0    4 50   -> 41   sym.deregister_tm_clones
0x00400610    4 58   -> 55   sym.register_tm_clones
0x00400650    3 28           sym.__do_global_dtors_aux
0x00400670    4 38   -> 35   entry1.init
0x00400696    6 112          sym.decrypt_flag
0x00400706    4 69           sym.fib
0x0040074b    1 16           sym.calculate_key
0x0040075b    1 31           sym.alarm_handler
0x0040077a    3 84           sym.set_timer
0x004007ce    1 43           sym.get_key
0x004007f9    1 40           sym.print_flag
0x00400821    4 62           sym.header
0x0040085f    1 62           sym.main
0x004008a0    4 101          sym.__libc_csu_init
0x00400910    1 2            sym.__libc_csu_fini
0x00400914    1 9            sym._fini
```


The `main` function is similar to be-quick-or-be-dead-1.
However, this time, if we disable the timer, we hang and the program does not return in a reasonable time.


Like last time, the `get_key` function calls the `calculate_key` function, which holds the important logic:

```assembly
[0x004007ce]> s sym.calculate_key
[0x0040074b]> pdf
/ (fcn) sym.calculate_key 16
|   sym.calculate_key ();
|           ; CALL XREF from sym.get_key (0x4007e1)
|           0x0040074b      55             push rbp
|           0x0040074c      4889e5         mov rbp, rsp
|           0x0040074f      bf2b040000     mov edi, 0x42b              ; 1067
|           0x00400754      e8adffffff     call sym.fib
|           0x00400759      5d             pop rbp
\           0x0040075a      c3             ret
[0x0040074b]>
```

This function calls `fib` with 1067 as a parameter.

Believe it or not, `fib` calculates the nth member of the Fibonacci series:
```
              .-------------------------------------------------.
              | [0x400706]                                      |
              | (fcn) sym.fib 69                                |
              |   sym.fib (int arg1);                           |
              | ; var int local_24h @ rbp-0x24                  |
              | ; var int local_14h @ rbp-0x14                  |
              | ; arg int arg1 @ rdi                            |
              | ; CALL XREFS from sym.fib (0x400728, 0x400737)  |
              | ; CALL XREF from sym.calculate_key (0x400754)   |
              | push rbp                                        |
              | mov rbp, rsp                                    |
              | push rbx                                        |
              | ; '('                                           |
              | sub rsp, 0x28                                   |
              | ; arg1                                          |
              | mov dword [local_24h], edi                      |
              | cmp dword [local_24h], 1                        |
              | ja 0x400720;[ga]                                |
              `-------------------------------------------------'
                      f t
                      | |
                      | '--------------------.
     .----------------'                      |
     |                                       |
 .-----------------------------------.   .-----------------------------------.
 |  0x400718 [gd]                    |   |  0x400720 [ga]                    |
 | mov eax, dword [local_24h]        |   | mov eax, dword [local_24h]        |
 | mov dword [local_14h], eax        |   | sub eax, 1                        |
 | jmp 0x400741;[gc]                 |   | mov edi, eax                      |
 `-----------------------------------'   | call sym.fib;[gb]                 |
     v                                   | mov ebx, eax                      |
     |                                   | mov eax, dword [local_24h]        |
     |                                   | sub eax, 2                        |
     |                                   | mov edi, eax                      |
     |                                   | call sym.fib;[gb]                 |
     |                                   | add eax, ebx                      |
     |                                   | mov dword [local_14h], eax        |
     |                                   `-----------------------------------'
     |                                       v
     |                                       |
     '--------------------.                  |
                          | .----------------'
                          | |
                          | |
                    .-------------------------------------.
                    |  0x400741 [gc]                      |
                    | ; CODE XREF from sym.fib (0x40071e) |
                    | mov eax, dword [local_14h]          |
                    | ; '('                               |
                    | add rsp, 0x28                       |
                    | pop rbx                             |
                    | pop rbp                             |
                    | ret                                 |
                    `-------------------------------------'   
```

So, we just need to find the 1067th Fibonacci number, and we're done.

Here's the code (or at least, version 1 of it):

```python
from ctypes import *
import numpy as np

np.seterr(all="ignore")

class Memoize(object):
    def __init__(self, func):
        self.func = func
        self.cache = {}
    def __call__(self, *args):
        if args in self.cache:
            return self.cache[args]
        ret = self.func(*args)
        self.cache[args] = ret
        return ret

@Memoize
def fib(n):
    if n == 0:
        return np.uint32(0)
    elif n == 1:
        return np.uint32(1)
    return fib(n-2) + fib(n-1)

requested_number = 1067

print(fib(requested_number))
```

Note that we use numpy's uint32 in order to simulate the overflows that occur when adding 32-bit integers (as in the original assembly, where the result is returned in eax: `mov eax, dword [local_14h]`).

Running the above code gives us the following error:
```
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2# python solve.py
Traceback (most recent call last):
  File "solve.py", line 29, in <module>
    print(fib(requested_number))
  File "solve.py", line 13, in __call__
    ret = self.func(*args)
  File "solve.py", line 23, in fib
    return fib(n-2) + fib(n-1)
[...]
RuntimeError: maximum recursion depth exceeded
```

We fix that by calculating the series from bottom to top, i.e.:
```python
requested_number = 1067
for i in range(requested_number):
    fib(i)
    
print(fib(requested_number))
```

This addition gives us the answer in less than a second:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2# python solve.py
781077913
```

Since patching with Radare2 in write mode is giving me incorrect results for an unknown reason, we'll patch with dd.

First, we need to find the exact place to patch. `get_key` makes most sense:
```assembly
[0x004005a0]> s sym.get_key
[0x004007ce]> pdf
/ (fcn) sym.get_key 43
|   sym.get_key ();
|           ; CALL XREF from sym.main (0x400887)
|           0x004007ce      55             push rbp
|           0x004007cf      4889e5         mov rbp, rsp
|           0x004007d2      bfb8094000     mov edi, str.Calculating_key... ; 0x4009b8 ; "Calculating key..."
|           0x004007d7      e854fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x004007dc      b800000000     mov eax, 0
|           0x004007e1      e865ffffff     call sym.calculate_key
|           0x004007e6      8905d4082000   mov dword [obj.key], eax    ; obj.__TMC_END ; [0x6010c0:4]=0
|           0x004007ec      bfcb094000     mov edi, str.Done_calculating_key ; 0x4009cb ; "Done calculating key"
|           0x004007f1      e83afdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x004007f6      90             nop
|           0x004007f7      5d             pop rbp
\           0x004007f8      c3             ret
```

Instead of calling `sym.calculate_key`, we'll hardcode the result.

We'd like to replace the `call sym.calculate_key` command with `mov eax, 781077913`.

We can use `rasm2` to encode this instruction:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# rasm2 'mov eax, 781077913'
b8994d8e2e
```

Luckily, the length of both instructions is identical, so we can patch in-place.

Therefore, at virtual address `0x004007e1`, we'd like to replace `e865ffffff` with `b8994d8e2e`. How do we find the offset in the binary itself?

We will need to check the section table of the executable:
```
[0x004005a0]> iS
[Sections]
Nm Paddr       Size Vaddr      Memsz Perms Name
00 0x00000000     0 0x00000000     0 ----
01 0x00000238    28 0x00400238    28 -r-- .interp
02 0x00000254    32 0x00400254    32 -r-- .note.ABI_tag
03 0x00000274    36 0x00400274    36 -r-- .note.gnu.build_id
04 0x00000298    28 0x00400298    28 -r-- .gnu.hash
05 0x000002b8   216 0x004002b8   216 -r-- .dynsym
06 0x00000390   101 0x00400390   101 -r-- .dynstr
07 0x000003f6    18 0x004003f6    18 -r-- .gnu.version
08 0x00000408    32 0x00400408    32 -r-- .gnu.version_r
09 0x00000428    24 0x00400428    24 -r-- .rela.dyn
10 0x00000440   168 0x00400440   168 -r-- .rela.plt
11 0x000004e8    26 0x004004e8    26 -r-x .init
12 0x00000510   128 0x00400510   128 -r-x .plt
13 0x00000590     8 0x00400590     8 -r-x .plt.got
14 0x000005a0   882 0x004005a0   882 -r-x .text
15 0x00000914     9 0x00400914     9 -r-x .fini
16 0x00000920   232 0x00400920   232 -r-- .rodata
17 0x00000a08   116 0x00400a08   116 -r-- .eh_frame_hdr
18 0x00000a80   500 0x00400a80   500 -r-- .eh_frame
19 0x00000e10     8 0x00600e10     8 -rw- .init_array
20 0x00000e18     8 0x00600e18     8 -rw- .fini_array
21 0x00000e20     8 0x00600e20     8 -rw- .jcr
22 0x00000e28   464 0x00600e28   464 -rw- .dynamic
23 0x00000ff8     8 0x00600ff8     8 -rw- .got
24 0x00001000    80 0x00601000    80 -rw- .got.plt
25 0x00001060    90 0x00601060    90 -rw- .data
26 0x000010ba     0 0x006010bc    12 -rw- .bss
27 0x000010ba    53 0x00000000    53 ---- .comment
28 0x00001b9c   268 0x00000000   268 ---- .shstrtab
29 0x000010f0  1992 0x00000000  1992 ---- .symtab
30 0x000018b8   740 0x00000000   740 ---- .strtab
```

Our virtual address appears in the following section:
```
Nm Paddr       Size Vaddr      Memsz Perms Name
14 0x000005a0   882 0x004005a0   882 -r-x .text
```

So we calculate:
```
0x004007e1 - 0x004005a0 + 0x000005a0 = 0x7e1
```

Let's verify we have the correct offset in the binary:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# xxd -g 1 -s $((0x7e1)) -l 5 be-quick-or-be-dead-2
000007e1: e8 65 ff ff ff                                   .e...
```

Now, we patch:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# cp be-quick-or-be-dead-2 be-quick-or-be-dead-2_patched
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# rasm2 'mov eax, 781077913' | xxd -p -r | dd conv=notrunc of=be-quick-or-be-dead-2_patched bs=1 seek=$((0x7e1))
5+0 records in
5+0 records out
5 bytes copied, 0.00431698 s, 1.2 kB/s
```

The result:
```
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-2/v2# ./be-quick-or-be-dead-2_patched
Be Quick Or Be Dead 2
=====================

Calculating key...
Done calculating key
Printing flag:
picoCTF{the_fibonacci_sequence_can_be_done_fast_ec58967b}
```

The flag: picoCTF{the_fibonacci_sequence_can_be_done_fast_ec58967b}