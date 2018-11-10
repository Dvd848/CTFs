# be-quick-or-be-dead-3
Reversing, 350 points

## Description:
> As the [song](https://www.youtube.com/watch?v=CTt1vk9nM9c) draws closer to the end, another executable **be-quick-or-be-dead-3** suddenly pops up. This one requires even faster machines. Can you run it fast enough too? 


Attached was a binary file.

## Solution:

Let's run the file:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# ./be-quick-or-be-dead-3
Be Quick Or Be Dead 3
=====================

Calculating key...
You need a faster machine. Bye bye.
```

Time to analyze the program. We'll use Radare2.

```console
r2 be-quick-or-be-dead-3
```


Analyze the binary with `aa` and list the functions with `afl`:
```
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# r2 be-quick-or-be-dead-3
 -- When you sold that exploit, what they really bought, was your silence.
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
0x00400706    4 140          sym.calc
0x00400792    1 16           sym.calculate_key
0x004007a2    1 31           sym.alarm_handler
0x004007c1    3 84           sym.set_timer
0x00400815    1 43           sym.get_key
0x00400840    1 40           sym.print_flag
0x00400868    4 62           sym.header
0x004008a6    1 62           sym.main
0x004008f0    4 101          sym.__libc_csu_init
0x00400960    1 2            sym.__libc_csu_fini
0x00400964    1 9            sym._fini
[0x004005a0]>
```


The `main` function is similar to the previous two challenges.
Just like last time, if we disable the timer, we hang and the program does not return in a reasonable time.


Like last time, the `get_key` function calls the `calculate_key` function, which holds the important logic:

```assembly
[0x004008a6]> s sym.calculate_key
[0x00400792]> pdf
/ (fcn) sym.calculate_key 16
|   sym.calculate_key ();
|           ; CALL XREF from sym.get_key (0x400828)
|           0x00400792      55             push rbp
|           0x00400793      4889e5         mov rbp, rsp
|           0x00400796      bf65990100     mov edi, 0x19965
|           0x0040079b      e866ffffff     call sym.calc
|           0x004007a0      5d             pop rbp
\           0x004007a1      c3             ret
```

This function calls `calc` with 0x19965 as a parameter.

Here's the logic for `calc`:
```
[0x00400706]> VV @ sym.calc
 .------------------------------------------------------------------------------------------------------------------.
 | [0x400706]                                                                                                       |
 | (fcn) sym.calc 140                                                                                               |
 |   sym.calc (int arg1);                                                                                           |
 | ; var int local_24h @ rbp-0x24                                                                                   |
 | ; var int local_14h @ rbp-0x14                                                                                   |
 | ; arg int arg1 @ rdi                                                                                             |
 | ; XREFS: CALL 0x00400733  CALL 0x00400742  CALL 0x00400751  CALL 0x00400761  CALL 0x00400776  CALL 0x0040079b    |
 | push rbp                                                                                                         |
 | mov rbp, rsp                                                                                                     |
 | push r12                                                                                                         |
 | push rbx                                                                                                         |
 | sub rsp, 0x20                                                                                                    |
 | ; arg1                                                                                                           |
 | mov dword [local_24h], edi                                                                                       |
 | cmp dword [local_24h], 4                                                                                         |
 | ja 0x40072b;[ga]                                                                                                 |
 `------------------------------------------------------------------------------------------------------------------'
         f t
         | |
         | '------------------------------------------------------.
         '--------------.                                         |
                        |                                         |
                    .------------------------------------.    .-----------------------------------.
                    |  0x40071a [gd]                     |    |  0x40072b [ga]                    |
                    | mov eax, dword [local_24h]         |    | mov eax, dword [local_24h]        |
                    | imul eax, dword [local_24h]        |    | sub eax, 1                        |
                    | ; 'E#'                             |    | mov edi, eax                      |
                    | add eax, 0x2345                    |    | call sym.calc;[gb]                |
                    | mov dword [local_14h], eax         |    | mov ebx, eax                      |
                    | jmp 0x400786;[gc]                  |    | mov eax, dword [local_24h]        |
                    `------------------------------------'    | sub eax, 2                        |
                        v                                     | mov edi, eax                      |
                        |                                     | call sym.calc;[gb]                |
                        |                                     | sub ebx, eax                      |
                        |                                     | mov eax, dword [local_24h]        |
                        |                                     | sub eax, 3                        |
                        |                                     | mov edi, eax                      |
                        |                                     | call sym.calc;[gb]                |
                        |                                     | mov r12d, eax                     |
                        |                                     | mov eax, dword [local_24h]        |
                        |                                     | sub eax, 4                        |
                        |                                     | mov edi, eax                      |
                        |                                     | call sym.calc;[gb]                |
                        |                                     | sub r12d, eax                     |
                        |                                     | mov eax, r12d                     |
                        |                                     | add ebx, eax                      |
                        |                                     | mov eax, dword [local_24h]        |
                        |                                     | sub eax, 5                        |
                        |                                     | mov edi, eax                      |
                        |                                     | call sym.calc;[gb]                |
                        |                                     | imul eax, eax, 0x1234             |
                        |                                     | add eax, ebx                      |
                        |                                     | mov dword [local_14h], eax        |
                        |                                     `-----------------------------------'
                        |                                         v
                        |                                         |
                        '--------------------.                    |
                                             | .------------------'
                                             | |
                                       .--------------------------------------.
                                       |  0x400786 [gc]                       |
                                       | ; CODE XREF from sym.calc (0x400729) |
                                       | mov eax, dword [local_14h]           |
                                       | add rsp, 0x20                        |
                                       | pop rbx                              |
                                       | pop r12                              |
                                       | pop rbp                              |
                                       | ret                                  |
                                       `--------------------------------------'     
```

So, we just need to predict the logic for `calc(0x19965)`, and we're done.

Calculating this in Python, using dynamic programming:

```python
import numpy as np
import functools
import sys
import ctypes

np.seterr(all="ignore")

def memoize(func):
    cache = func.cache = {}

    @functools.wraps(func)
    def memoized_func(*args, **kwargs):
        key = str(args) + str(kwargs)
        if key not in cache:
            cache[key] = func(*args, **kwargs)
        return cache[key]

    return memoized_func

@memoize
def calc(x):
    assert (type(x) == np.int32)
    if x > 4:
        ebx = calc(x - np.int32(1))
        ebx -= calc(x - np.int32(2))
        r12d = calc(x - np.int32(3))
        r12d -= calc(x - np.int32(4))
        ebx += r12d
        eax = calc(x - np.int32(5))
        eax *= np.int32(0x1234)
        return eax + ebx
    else:
        return (x * x) + np.int32(0x2345)
        
for i in range(0x19965):
    calc(np.int32(i))

res = calc(np.int32(0x19965))
print res
print str(ctypes.c_uint(res))

```

Note that we use numpy's uint32 in order to simulate the overflows that occur when adding 32-bit integers (as in the original assembly, where the result is returned in eax: `mov eax, dword [local_14h]`).

Running the above code gives us the following output (takes a few seconds):
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3# python solve.py
-1641887346
c_uint(2653079950L)
```

Since patching with Radare2 in write mode is giving me incorrect results for an unknown reason, we'll patch with dd.

First, we need to find the exact place to patch. `get_key` makes most sense:
```assembly
[0x004005a0]> s sym.get_key
[0x00400815]> pdf
/ (fcn) sym.get_key 43
|   sym.get_key ();
|           ; CALL XREF from sym.main (0x4008ce)
|           0x00400815      55             push rbp
|           0x00400816      4889e5         mov rbp, rsp
|           0x00400819      bf080a4000     mov edi, str.Calculating_key... ; 0x400a08 ; "Calculating key..."
|           0x0040081e      e80dfdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x00400823      b800000000     mov eax, 0
|           0x00400828      e865ffffff     call sym.calculate_key
|           0x0040082d      89057d082000   mov dword [obj.key], eax    ; obj.__TMC_END ; [0x6010b0:4]=0
|           0x00400833      bf1b0a4000     mov edi, str.Done_calculating_key ; 0x400a1b ; "Done calculating key"
|           0x00400838      e8f3fcffff     call sym.imp.puts           ; int puts(const char *s)
|           0x0040083d      90             nop
|           0x0040083e      5d             pop rbp
\           0x0040083f      c3             ret
```

Instead of calling `sym.calculate_key`, we'll hardcode the result.

We'd like to replace the `call sym.calculate_key` command with `mov eax, 2653079950`.

We can use `rasm2` to encode this instruction:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# rasm2 'mov eax, 2653079950'
b88ec9229e
```

Luckily, the length of both instructions is identical, so we can patch in-place.

Therefore, at virtual address `0x00400828`, we'd like to replace `e865ffffff` with `b88ec9229e`. How do we find the offset in the binary itself?

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
14 0x000005a0   962 0x004005a0   962 -r-x .text
15 0x00000964     9 0x00400964     9 -r-x .fini
16 0x00000970   232 0x00400970   232 -r-- .rodata
17 0x00000a58   116 0x00400a58   116 -r-- .eh_frame_hdr
18 0x00000ad0   508 0x00400ad0   508 -r-- .eh_frame
19 0x00000e10     8 0x00600e10     8 -rw- .init_array
20 0x00000e18     8 0x00600e18     8 -rw- .fini_array
21 0x00000e20     8 0x00600e20     8 -rw- .jcr
22 0x00000e28   464 0x00600e28   464 -rw- .dynamic
23 0x00000ff8     8 0x00600ff8     8 -rw- .got
24 0x00001000    80 0x00601000    80 -rw- .got.plt
25 0x00001060    74 0x00601060    74 -rw- .data
26 0x000010aa     0 0x006010ac    12 -rw- .bss
27 0x000010aa    53 0x00000000    53 ---- .comment
28 0x00001b8d   268 0x00000000   268 ---- .shstrtab
29 0x000010e0  1992 0x00000000  1992 ---- .symtab
30 0x000018a8   741 0x00000000   741 ---- .strtab
```

Our virtual address appears in the following section:
```
Nm Paddr       Size Vaddr      Memsz Perms Name
14 0x000005a0   962 0x004005a0   962 -r-x .text
```

So we calculate:
```
0x00400828 - 0x004005a0 + 0x000005a0 = 0x828
```

Let's verify we have the correct offset in the binary:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2#  xxd -g 1 -s $((0x828)) -l 5 be-quick-or-be-dead-3
00000828: e8 65 ff ff ff                                   .e...
```

Now, we patch:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# cp be-quick-or-be-dead-3 be-quick-or-be-dead-3_patched
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# rasm2 'mov eax, 2653079950' | xxd -p -r | dd conv=notrunc of=be-quick-or-be-dead-3_patched  bs=1 seek=$((0x828))
5+0 records in
5+0 records out
5 bytes copied, 0.00637541 s, 0.8 kB/s
```

The result:
```
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-3/v2# ./be-quick-or-be-dead-3_patched
Be Quick Or Be Dead 3
=====================

Calculating key...
Done calculating key
Printing flag:
picoCTF{dynamic_pr0gramming_ftw_b5c45645}
```

The flag: picoCTF{dynamic_pr0gramming_ftw_b5c45645}