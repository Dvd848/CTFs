# Need For Speed
Reverse Engineering, 400 points

## Description:
> The name of the game is speed. Are you quick enough to solve this problem and keep it above 50 mph?


## Solution: 

This challenge is similar to last year's [be-quick-or-be-dead-1](/2018_picoCTF/be-quick-or-be-dead-1.md).

We run the program and hit an alarm:

```console
root@kali:/media/sf_CTFs/pico/Need_For_Speed# ./need-for-speed
Keep this thing over 50 mph!
============================

Creating key...
Not fast enough. BOOM!
```

Let's patch the program, just like last time:

```console
root@kali:/media/sf_CTFs/pico/Need_For_Speed# cp need-for-speed patched
root@kali:/media/sf_CTFs/pico/Need_For_Speed# r2 patched
 -- Add custom Have you setup your ~/.radare2rc today?
[0x000006b0]> aaa
[Cannot analyze at 0x000006a0g with sym. and entry0 (aa)
[x] Analyze all flags starting with sym. and entry0 (aa)
[Cannot analyze at 0x000006a0ac)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Check for objc references
[x] Check for vtables
[x] Type matching analysis for all functions (aaft)
[x] Propagate noreturn information
[x] Use -AA or aaaa to perform additional experimental analysis.
[0x000006b0]> afl
0x000006b0    1 42           entry0
0x000006e0    4 50   -> 40   sym.deregister_tm_clones
0x00000720    4 66   -> 57   sym.register_tm_clones
0x00000770    5 58   -> 51   entry.fini0
0x000007b0    1 10           entry.init0
0x00000a30    1 2            sym.__libc_csu_fini
0x000007ba    6 135          sym.decrypt_flag
0x00000650    1 6            sym.imp.puts
0x00000690    1 6            sym.imp.exit
0x00000932    4 66           sym.header
0x00000a34    1 9            sym._fini
0x000008d7    1 47           sym.get_key
0x00000841    3 29           sym.calculate_key
0x000009c0    4 101          sym.__libc_csu_init
0x00000974    1 62           main
0x00000680    1 6            sym.imp.__sysv_signal
0x00000660    1 6            sym.imp.printf
0x00000670    1 6            sym.imp.alarm
0x00000906    1 44           sym.print_flag
0x00000610    3 23           sym._init
0x00000640    1 6            sym.imp.putchar
0x00000000    6 459  -> 485  loc.imp._ITM_deregisterTMCloneTable
0x0000085e    1 33           sym.alarm_handler
0x0000087f    3 88           sym.set_timer
[0x000006b0]> s sym.get_key
[0x000008d7]> pdf
/ (fcn) sym.get_key 47
|   sym.get_key ();
|           ; CALL XREF from main @ 0x99c
|           0x000008d7      55             push rbp
|           0x000008d8      4889e5         mov rbp, rsp
|           0x000008db      488d3dfc0100.  lea rdi, str.Creating_key... ; 0xade ; "Creating key..." ; const char *s
|           0x000008e2      e869fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x000008e7      b800000000     mov eax, 0
|           0x000008ec      e850ffffff     call sym.calculate_key
|           0x000008f1      890565072000   mov dword [obj.key], eax    ; [0x20105c:4]=0
|           0x000008f7      488d3df00100.  lea rdi, str.Finished       ; 0xaee ; "Finished" ; const char *s
|           0x000008fe      e84dfdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x00000903      90             nop
|           0x00000904      5d             pop rbp
\           0x00000905      c3             ret
[0x000008d7]> s sym.calculate_key
sym.calculate_key   sym.calculate_key
[0x000008d7]> s sym.calculate_key
[0x00000841]> pdf
/ (fcn) sym.calculate_key 29
|   sym.calculate_key ();
|           ; var uint32_t var_4h @ rbp-0x4
|           ; CALL XREF from sym.get_key @ 0x8ec
|           0x00000841      55             push rbp
|           0x00000842      4889e5         mov rbp, rsp
|           0x00000845      c745fc1c07c2.  mov dword [var_4h], 0xd8c2071c
|           ; CODE XREF from sym.calculate_key @ 0x857
|       .-> 0x0000084c      836dfc01       sub dword [var_4h], 1
|       :   0x00000850      817dfc8e0361.  cmp dword [var_4h], 0xec61038e
|       `=< 0x00000857      75f3           jne 0x84c
|           0x00000859      8b45fc         mov eax, dword [var_4h]
|           0x0000085c      5d             pop rbp
\           0x0000085d      c3             ret
[0x00000841]> oo+
[0x00000841]> s 0x00000845
[0x00000845]> pd 1
|           0x00000845      c745fc1c07c2.  mov dword [var_4h], 0xd8c2071c
[0x00000845]> wa mov dword [rbp-0x4], 0xec61038f
Written 7 byte(s) (mov dword [rbp-0x4], 0xec61038f) = wx c745fc8f0361ec
[0x00000845]> q
```

Before the change, `calculate_key` would count from `0xd8c2071c` to `0xec61038e` and only then return. Since this is just a time-waster, we start counting from `0xec61038f` instead.

Now the program runs much faster, and doesn't hit the alarm:
```console
root@kali:/media/sf_CTFs/pico/Need_For_Speed# ./patched
Keep this thing over 50 mph!
============================

Creating key...
Finished
Printing flag:
PICOCTF{Good job keeping bus #236cb1c9 speeding along!}
```