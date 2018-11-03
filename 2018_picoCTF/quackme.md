# quackme
Reversing, 200 points

## Description:
> Can you deal with the Duck Web? Get us the flag from this program. 

A binary was attached.

## Solution:

Let's run the supplied binary:
```console
root@kali:/media/sf_CTFs/pico/quackme# ./main
You have now entered the Duck Web, and you're in for a honkin' good time.
Can you figure out my trick?
test
That's all folks.
```

Not to much to work with. The next step is launching a disassembler:
```assembly
root@kali:/media/sf_CTFs/pico/quackme# r2 main
 -- Beer in mind.
[0x080484e0]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x080484e0]> afl
0x08048400    3 35           sym._init
0x08048440    1 6            sym.imp.getline
0x08048450    1 6            sym.imp.__stack_chk_fail
0x08048460    1 6            sym.imp.malloc
0x08048470    1 6            sym.imp.puts
0x08048480    1 6            sym.imp.exit
0x08048490    1 6            sym.imp.strlen
0x080484a0    1 6            sym.imp.__libc_start_main
0x080484b0    1 6            sym.imp.setvbuf
0x080484c0    1 6            sym.imp.memset
0x080484d0    1 6            fcn.080484d0
0x080484e0    1 33           entry0
0x08048510    1 4            sym.__x86.get_pc_thunk.bx
0x08048520    4 43           sym.deregister_tm_clones
0x08048550    4 53           sym.register_tm_clones
0x08048590    3 30           sym.__do_global_dtors_aux
0x080485b0    4 43   -> 40   entry1.init
0x080485db    6 103          sym.read_input
0x08048642   10 211          sym.do_magic
0x08048715    1 87           sym.main
0x08048770    4 93           sym.__libc_csu_init
0x080487d0    1 2            sym.__libc_csu_fini
0x080487d4    1 20           sym._fini
[0x080484e0]> s sym.main
[0x08048715]> pdf
            ;-- main:
/ (fcn) sym.main 87
|   sym.main (int argc, char **argv, char **envp);
|           ; var int local_4h @ ebp-0x4
|           ; arg int arg_4h @ esp+0x4
|           ; DATA XREF from entry0 (0x80484f7)
|           0x08048715      8d4c2404       lea ecx, [arg_4h]           ; 4
|           0x08048719      83e4f0         and esp, 0xfffffff0
|           0x0804871c      ff71fc         push dword [ecx - 4]
|           0x0804871f      55             push ebp
|           0x08048720      89e5           mov ebp, esp
|           0x08048722      51             push ecx
|           0x08048723      83ec04         sub esp, 4
|           0x08048726      a144a00408     mov eax, dword [obj.stdout__GLIBC_2.0] ; [0x804a044:4]=0
|           0x0804872b      6a00           push 0
|           0x0804872d      6a02           push 2                      ; 2
|           0x0804872f      6a00           push 0
|           0x08048731      50             push eax
|           0x08048732      e879fdffff     call sym.imp.setvbuf        ; int setvbuf(FILE*stream, char *buf, int mode, size_t size)
|           0x08048737      83c410         add esp, 0x10
|           0x0804873a      83ec0c         sub esp, 0xc
|           0x0804873d      68f0870408     push str.You_have_now_entered_the_Duck_Web__and_you_re_in_for_a_honkin__good_time.__Can_you_figure_out_my_trick ; 0x80487f0 ; "You have now entered the Duck Web, and you're in for a honkin' good time.\nCan you figure out my trick?"
|           0x08048742      e829fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x08048747      83c410         add esp, 0x10
|           0x0804874a      e8f3feffff     call sym.do_magic
|           0x0804874f      83ec0c         sub esp, 0xc
|           0x08048752      68bb880408     push str.That_s_all_folks.  ; 0x80488bb ; "That's all folks."
|           0x08048757      e814fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x0804875c      83c410         add esp, 0x10
|           0x0804875f      b800000000     mov eax, 0
|           0x08048764      8b4dfc         mov ecx, dword [local_4h]
|           0x08048767      c9             leave
|           0x08048768      8d61fc         lea esp, [ecx - 4]
\           0x0804876b      c3             ret
[0x08048715]>
```

The magic seems to happen in `sym.do_magic`, let's inspect it:
```
[0x08048715]> s sym.do_magic
[0x08048642]> VV
```

The first block of logic:
```assembly
.----------------------------------------.
| [0x8048642]                            |
| (fcn) sym.do_magic 211                 |
|   sym.do_magic ();                     |
| ; var int local_1dh @ ebp-0x1d         |
| ; var int local_1ch @ ebp-0x1c         |
| ; var int local_18h @ ebp-0x18         |
| ; var int local_14h @ ebp-0x14         |
| ; var int local_10h @ ebp-0x10         |
| ; var int local_ch @ ebp-0xc           |
| ; CALL XREF from sym.main (0x804874a)  |
| push ebp                               |
| mov ebp, esp                           |
| ; '('                                  |
| sub esp, 0x28                          |
| call sym.read_input;[ga]               |
| mov dword [local_14h], eax             |
| sub esp, 0xc                           |
| push dword [local_14h]                 |
| ; size_t strlen(const char *s)         |
| call sym.imp.strlen;[gb]               |
| add esp, 0x10                          |
| mov dword [local_10h], eax             |
| mov eax, dword [local_10h]             |
| add eax, 1                             |
| sub esp, 0xc                           |
| push eax                               |
| ;  void *malloc(size_t size)           |
| call sym.imp.malloc;[gc]               |
| add esp, 0x10                          |
| mov dword [local_ch], eax              |
| cmp dword [local_ch], 0                |
| jne 0x8048696;[gd]                     |
`----------------------------------------'
```

We'll rename local variables as we go by entering command mode with '`:`', e.g.:
```
Press <enter> to return to Visual mode.
:> afvn input local_14h
:> afvn input_len local_10h
:> afvn new_buffer local_ch
```

In this block, we just read the input, calculate it's length and allocate a new buffer.
Assuming the allocation was successful, we jump to the [gd] block:
```assembly
.-------------------------------------------.
|  0x8048696 [gd]                           |
| mov eax, dword [input_len]                |
| add eax, 1                                |
| sub esp, 4                                |
| push eax                                  |
| push 0                                    |
| push dword [new_buffer]                   |
| ; void *memset(void *s, int c, size_t n)  |
| call sym.imp.memset;[gi]                  |
| add esp, 0x10                             |
| mov dword [local_1ch], 0                  |
| mov dword [local_18h], 0                  |
| jmp 0x804870b;[gj]                        |
`-------------------------------------------'
```

Renaming:
```
Press <enter> to return to Visual mode. 
:> afvn i local_18h
:> afvn counter local_1ch
:>
```

Here we memset the buffer and initialize some helper variables.

What comes next is a loop:

```

                                                                                                               | .-------------------------------------------------.
                                                                                                               | |                                                 |
                                                                                                         .-----------------------------------.                     |
                                                                                                         |  0x804870b [gj]                   |                     |
                                                                                                         | mov eax, dword [i]                |                     |
                                                                                                         | cmp eax, dword [input_len]        |                     |
                                                                                                         | jl 0x80486bd;[gl]                 |                     |
                                                                                                         `-----------------------------------'                     |
                                                                                                               t f                                                 |
                                                                                                               | |                                                 |
                     .-----------------------------------------------------------------------------------------' |                                                 |
                     |                                                                                           '-----------------------------------------------. |
                     |                                                                                                                                           | |
                 .---------------------------------------------------------------------------------------------------------------------------------------.       | |
                 |  0x80486bd [gl]                                                                                                                       |       | |
                 | mov eax, dword [i]                                                                                                                    |       | |
                 | add eax, obj.sekrutBuffer                                                                                                             |       | |
                 | movzx ecx, byte [eax]                                                                                                                 |       | |
                 | mov edx, dword [i]                                                                                                                    |       | |
                 | mov eax, dword [input]                                                                                                                |       | |
                 | add eax, edx                                                                                                                          |       | |
                 | movzx eax, byte [eax]                                                                                                                 |       | |
                 | xor eax, ecx                                                                                                                          |       | |
                 | mov byte [local_1dh], al                                                                                                              |       | |
                 | ; [0x804a038:4]=0x80487f0 str.You_have_now_entered_the_Duck_Web__and_you_re_in_for_a_honkin__good_time.__Can_you_figure_out_my_trick  |       | |
                 | mov edx, dword obj.greetingMessage                                                                                                    |       | |
                 | mov eax, dword [i]                                                                                                                    |       | |
                 | add eax, edx                                                                                                                          |       | |
                 | movzx eax, byte [eax]                                                                                                                 |       | |
                 | cmp al, byte [local_1dh]                                                                                                              |       | |
                 | jne 0x80486ef;[gk]                                                                                                                    |       | |
                 `---------------------------------------------------------------------------------------------------------------------------------------'       | |
                         f t                                                                                                                                     | | 
                         | |                                                                                                                                     | |
                         | '-----------------------------------------------------------------------------------------------------------------------------.       | |
                         '----------------------------------------------.                                                                                |       | |
                                                                        |                                                                                |       | |
                                                                    .--------------------------------.                                                   |       | |
                                                                    |  0x80486eb [gm]                |                                                   |       | |
                                                                    | add dword [counter], 1         |                                                   |       | |
                                                                    `--------------------------------'                                                   |       | |
                                                                        v                                                                                |       | |
                                                                        |                                                                                |       | |
                                                                        '----------------------------------.                                             |       | |
                                                                                                           | .-------------------------------------------'       | |
                                                                                                           | |                                                   | |
                                                                                                     .-----------------------------------.                       | |
                                                                                                     |  0x80486ef [gk]                   |                       | |
                                                                                                     | cmp dword [counter], 0x19         |                       | |
                                                                                                     | jne 0x8048707;[gn]                |                       | |
                                                                                                     `-----------------------------------'                       | |
                                                                                                             f t                                                 | |
                                                                                                             | |                                                 | |
                                                                                                             | '---------------.                                 | |
                                                                                         .-------------------'                 |                                 | |
                                                                                         |                                     |                                 | |
                                                                                     .--------------------------------.    .--------------------------------.    | |
                                                                                     |  0x80486f5 [gp]                |    |  0x8048707 [gn]                |    | |
                                                                                     | sub esp, 0xc                   |    | add dword [i], 1               |    | |
                                                                                     | ; 0x80488ab                    |    `--------------------------------'    | |
                                                                                     | ; "You are winner!"            |        v                                 | |
                                                                                     | push str.You_are_winner        |        |                                 | |
                                                                                     | ; int puts(const char *s)      |        |                                 | |
                                                                                     | call sym.imp.puts;[gf]         |        |                                 | |
                                                                                     | add esp, 0x10                  |        |                                 | |
                                                                                     | jmp 0x8048713;[go]             |        |                                 | |
                                                                                     `--------------------------------'        |                                 | |
                                                                                         v                                     |                                 | |
                                                                                         |                                     |                                 | |
                                                                                         '------------------------------------.|                                 | |
                                                                                                                              |`-----------------------------------'
                                                                                                                              | .--------------------------------'
                                                                                                                              | |
                                                                                                                              | |
                                                                                                                        .--------------------.
                                                                                                                        |  0x8048713 [go]    |
                                                                                                                        | leave              |
                                                                                                                        | ret                |
                                                                                                                        `--------------------' 
```

The loop runs for i=0..input_len (loop maintenance is located in [gj] and [gn]).

For each iteration, the main logic is executed in [gl]:
```assembly
 0x80486bd [gl] 
mov eax, dword [i]
add eax, obj.sekrutBuffer
movzx ecx, byte [eax]     ; ecx = sekrutBuffer[i]
mov edx, dword [i]
mov eax, dword [input]
add eax, edx
movzx eax, byte [eax]     ; eax = input[i]
xor eax, ecx              ; eax = eax ^ ecx
mov byte [local_1dh], al  local_1dh = eax
; [0x804a038:4]=0x80487f0 str.You_have_now_entered_the_Duck_Web__and_you_re_in_for_a_honkin__good_time.__Can_you_figure_out_my_trick
mov edx, dword obj.greetingMessage
mov eax, dword [i]
add eax, edx
movzx eax, byte [eax]     ; eax = obj.greetingMessage[i]
cmp al, byte [local_1dh]  ; compare obj.greetingMessage[i] and sekrutBuffer[i] ^ input[i]
jne 0x80486ef;[gk]
```

If the result is equal, [gm] will increment the counter, and if the counter hits 0x19, [gk] will end the loop with a success message.

Therefore, our task is to supply an input such that when XORed with the secret buffer, will result in the first 0x19 characters of the greeting message.

Let's inspect the secret buffer:
```
[0x08048858]> pcp  0x19 @ obj.sekrutBuffer
import struct
buf = struct.pack ("25B", *[
0x29,0x06,0x16,0x4f,0x2b,0x35,0x30,0x1e,0x51,0x1b,0x5b,
0x14,0x4b,0x08,0x5d,0x2b,0x5c,0x10,0x06,0x06,0x18,0x45,
0x51,0x00,0x5d])
```

Using this, we can create a python script to reveal the expected input:
```python
import struct

buf = struct.pack ("25B", *[
0x29,0x06,0x16,0x4f,0x2b,0x35,0x30,0x1e,0x51,0x1b,0x5b,
0x14,0x4b,0x08,0x5d,0x2b,0x5c,0x10,0x06,0x06,0x18,0x45,
0x51,0x00,0x5d])
greet = "You have now entered the Duck Web"

s = ""
for i, byte in enumerate(buf):
    s += chr(byte ^ ord(greet[i]))
print (s)
```

The result:

```console
root@kali:/media/sf_CTFs/pico/quackme# python3 exploit.py 
picoCTF{qu4ckm3_9bcb819e}
root@kali:/media/sf_CTFs/pico/quackme# python3 exploit.py | ./main
You have now entered the Duck Web, and you're in for a honkin' good time.
Can you figure out my trick?
You are winner!
That's all folks.
```

The flag: picoCTF{qu4ckm3_9bcb819e}