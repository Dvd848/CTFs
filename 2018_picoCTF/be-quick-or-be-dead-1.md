# be-quick-or-be-dead-1
Reversing, 200 points

## Description:
> You find [this](https://www.youtube.com/watch?v=CTt1vk9nM9c) when searching for some music, which leads you to **be-quick-or-be-dead-1**. Can you run it fast enough?

Attached was a binary file.

## Solution:

Let's run the file:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-1/v2# ./be-quick-or-be-dead-1 Be Quick Or Be Dead 1
=====================

Calculating key...
You need a faster machine. Bye bye.
```

Time to analyze the program. We'll use Radare2.

```console
r2 be-quick-or-be-dead-1
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
0x00400706    3 29           sym.calculate_key
0x00400723    1 31           sym.alarm_handler
0x00400742    3 84           sym.set_timer
0x00400796    1 43           sym.get_key
0x004007c1    1 40           sym.print_flag
0x004007e9    4 62           sym.header
0x00400827    1 62           sym.main
0x00400870    4 101          sym.__libc_csu_init
0x004008e0    1 2            sym.__libc_csu_fini
0x004008e4    1 9            sym._fini
```


Let's take a look at the `main` function:
```assembly
[0x004005a0]> VV @ sym.main
.---------------------------------------------------.
|  0x400827                                         |
| ;-- main:                                         |
| (fcn) sym.main 62                                 |
|   sym.main (int argc, char **argv, char **envp);  |
| ; var int local_10h @ rbp-0x10                    |
| ; var int local_4h @ rbp-0x4                      |
| ; arg int argc @ rdi                              |
| ; arg char **argv @ rsi                           |
| ; DATA XREF from entry0 (0x4005bd)                |
| push rbp                                          |
| mov rbp, rsp                                      |
| sub rsp, 0x10                                     |
| ; argc                                            |
| mov dword [local_4h], edi                         |
| ; argv                                            |
| mov qword [local_10h], rsi                        |
| mov eax, 0                                        |
| call sym.header;[ga]                              |
| mov eax, 0                                        |
| call sym.set_timer;[gb]                           |
| mov eax, 0                                        |
| call sym.get_key;[gc]                             |
| mov eax, 0                                        |
| call sym.print_flag;[gd]                          |
| mov eax, 0                                        |
| leave                                             |
| ret                                               |
`---------------------------------------------------'
```

It sets a timer, gets the key and prints the flag.

What is this timer?

```assembly
[0x00400742]> VV @ sym.set_timer
                                                                        .-------------------------------------------------.
                                                                        |  0x400742                                       |
                                                                        | (fcn) sym.set_timer 84                          |
                                                                        |   sym.set_timer ();                             |
                                                                        | ; var int local_ch @ rbp-0xc                    |
                                                                        | ; var int local_8h @ rbp-0x8                    |
                                                                        | ; CALL XREF from sym.main (0x400845)            |
                                                                        | push rbp                                        |
                                                                        | mov rbp, rsp                                    |
                                                                        | sub rsp, 0x10                                   |
                                                                        | mov dword [local_ch], 1                         |
                                                                        | ; 0x400723                                      |
                                                                        | mov esi, sym.alarm_handler                      |
                                                                        | ; 14                                            |
                                                                        | mov edi, 0xe                                    |
                                                                        | call sym.imp.__sysv_signal;[ga]                 |
                                                                        | mov qword [local_8h], rax                       |
                                                                        | cmp qword [local_8h], 0xffffffffffffffff        |
                                                                        | jne 0x400789;[gb]                               |
                                                                        `-------------------------------------------------'
                                                                                f t
                                                                                | |
                                                                                | '-----------------------------------------.
    .---------------------------------------------------------------------------'                                           |
    |                                                                                                                       |
.-------------------------------------------------------------------------------------------------------------------.   .---------------------------------.
|  0x40076b                                                                                                         |   |  0x400789                       |
| ; ';'                                                                                                             |   | mov eax, dword [local_ch]       |
| ; 59                                                                                                              |   | mov edi, eax                    |
| mov esi, 0x3b                                                                                                     |   | call sym.imp.alarm;[ge]         |
| ; 0x400928                                                                                                        |   | nop                             |
| ; "\n\nSomething went terribly wrong. \nPlease contact the admins with \"be-quick-or-be-dead-1.c:%d\".\n"         |   | leave                           |
| mov edi, str.Something_went_terribly_wrong.___Please_contact_the_admins_with__be_quick_or_be_dead_1.c:_d_.        |   | ret                             |
| mov eax, 0                                                                                                        |   `---------------------------------'
| ; int printf(const char *format)                                                                                  |
| call sym.imp.printf;[gc]                                                                                          |
| mov edi, 0                                                                                                        |
| ; void exit(int status)                                                                                           |
| call sym.imp.exit;[gd]                                                                                            |
`-------------------------------------------------------------------------------------------------------------------'
```

After 1 second (`mov dword [local_ch], 1 `), the alarm handler will be triggered and terminate the program.

So, the first thing that jumps to mind, even before analyzing the rest of the program, is to just nop-out the `set_timer` call.

We move to "write mode" with `oo+` and patch with `wa`:
```
[0x00400742]> s sym.main
[0x00400827]> pdf
            ;-- main:
/ (fcn) sym.main 62
|   sym.main (int argc, char **argv, char **envp);
|           ; var int local_10h @ rbp-0x10
|           ; var int local_4h @ rbp-0x4
|           ; arg int argc @ rdi
|           ; arg char **argv @ rsi
|           ; DATA XREF from entry0 (0x4005bd)
|           0x00400827      55             push rbp
|           0x00400828      4889e5         mov rbp, rsp
|           0x0040082b      4883ec10       sub rsp, 0x10
|           0x0040082f      897dfc         mov dword [local_4h], edi   ; argc
|           0x00400832      488975f0       mov qword [local_10h], rsi  ; argv
|           0x00400836      b800000000     mov eax, 0
|           0x0040083b      e8a9ffffff     call sym.header
|           0x00400840      b800000000     mov eax, 0
|           0x00400845      e8f8feffff     call sym.set_timer
|           0x0040084a      b800000000     mov eax, 0
|           0x0040084f      e842ffffff     call sym.get_key
|           0x00400854      b800000000     mov eax, 0
|           0x00400859      e863ffffff     call sym.print_flag
|           0x0040085e      b800000000     mov eax, 0
|           0x00400863      c9             leave
\           0x00400864      c3             ret
[0x00400827]> oo+
[0x00400827]> s 0x00400845
[0x00400845]> "wa nop;nop;nop;nop;nop"
Written 5 byte(s) (nop;nop;nop;nop;nop) = wx 9090909090
```

Now if we run the program, we get:
```console
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-1/v2# ./be-quick-or-be-dead-1
Be Quick Or Be Dead 1
=====================

Calculating key...
Done calculating key
Printing flag:
picoCTF{why_bother_doing_unnecessary_computation_d0c6aace}
```

However, it still takes some time. 
Let's inspect the `get_key` function:
```
root@kali:/media/sf_CTFs/pico/be-quick-or-be-dead-1/v2# r2 be-quick-or-be-dead-1
 -- Use rarun2 to launch your programs with a predefined environment.
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
0x00400706    3 29           sym.calculate_key
0x00400723    1 31           sym.alarm_handler
0x00400742    3 84           sym.set_timer
0x00400796    1 43           sym.get_key
0x004007c1    1 40           sym.print_flag
0x004007e9    4 62           sym.header
0x00400827    1 62           sym.main
0x00400870    4 101          sym.__libc_csu_init
0x004008e0    1 2            sym.__libc_csu_fini
0x004008e4    1 9            sym._fini
[0x004005a0]> s sym.get_key
[0x00400796]> pdf
/ (fcn) sym.get_key 43
|   sym.get_key ();
|           ; CALL XREF from sym.main (0x40084f)
|           0x00400796      55             push rbp
|           0x00400797      4889e5         mov rbp, rsp
|           0x0040079a      bf88094000     mov edi, str.Calculating_key... ; 0x400988 ; "Calculating key..."
|           0x0040079f      e88cfdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x004007a4      b800000000     mov eax, 0
|           0x004007a9      e858ffffff     call sym.calculate_key
|           0x004007ae      89050c092000   mov dword [obj.key], eax    ; obj.__TMC_END ; [0x6010c0:4]=0
|           0x004007b4      bf9b094000     mov edi, str.Done_calculating_key ; 0x40099b ; "Done calculating key"
|           0x004007b9      e872fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x004007be      90             nop
|           0x004007bf      5d             pop rbp
\           0x004007c0      c3             ret
[0x00400796]> s sym.calculate_key
[0x00400706]> pdf
/ (fcn) sym.calculate_key 29
|   sym.calculate_key ();
|           ; var int local_4h @ rbp-0x4
|           ; CALL XREF from sym.get_key (0x4007a9)
|           0x00400706      55             push rbp
|           0x00400707      4889e5         mov rbp, rsp
|           0x0040070a      c745fc1191fe.  mov dword [local_4h], 0x72fe9111
|       .-> 0x00400711      8345fc01       add dword [local_4h], 1
|       :   0x00400715      817dfc2222fd.  cmp dword [local_4h], 0xe5fd2222 ; [0xe5fd2222:4]=-1
|       `=< 0x0040071c      75f3           jne 0x400711
|           0x0040071e      8b45fc         mov eax, dword [local_4h]
|           0x00400721      5d             pop rbp
\           0x00400722      c3             ret
[0x00400706]> oo+
[0x00400706]> s 0x0040070a
[0x0040070a]> wa mov dword [rbp-0x4], 0xe5fd2221
Written 7 byte(s) (mov dword [rbp-0x4], 0xe5fd2221) = wx c745fc2122fde5
[0x0040070a]> q
```

`get_key` calls `calculate_key`, which simply loops from 0x72fe9111 to 0xe5fd2222 and returns 0xe5fd2222 as the key.
So, in order to save time, we modify it to loop from 0xe5fd2221 to 0xe5fd2222.
Note that in order for the `wa` command to work, we must substitute `local_4h` with `rbp-0x4` (we can see the translation at the beginning of the function).

Now, when we run the program, we immediately get the flag.

---

Note: For some reason, I had a lot of trouble patching with Radare2, since in many cases moving to "write mode" caused the session to get corrupted. I had to use `pd 1` to show the current disassembly and make sure the no corruption has occurred, and if it has, reload again with `oo` and `oo+`. Perhaps I am missing something about usage, but I couldn't find any hint online. 
```
[0x004005a0]> s sym.calculate_key 
[0x00400706]> oo+
[0x00400706]> pd 1
/ (fcn) sym.calculate_key 29
|   sym.calculate_key ();
|           ; var int local_4h @ rbp-0x4
|           ; CALL XREF from sym.get_key (0x4007a9)
|           0x00400706      55             push rbp
[0x00400706]> s 0x0040070a
[0x0040070a]> pd 1
|           0x0040070a      ff             invalid
[0x0040070a]> oo
[0x0040070a]> pd 1
|           0x0040070a      c745fc2122fd.  mov dword [local_4h], 0xe5fd2221
[0x0040070a]> oo+ 
[0x0040070a]> pd 1
|           0x0040070a      c745fc2122fd.  mov dword [local_4h], 0xe5fd2221
```
---

The flag is: picoCTF{why_bother_doing_unnecessary_computation_d0c6aace}