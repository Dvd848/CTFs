# Where no man has GOne before!
* Category: Reverse Engineering
* 300 Points
* Solved by the JCTF Team

## Description
> In this simple challenge, you need to break the protection in order to extract the flag
>
> http://revengme.challenges.bsidestlv.com/revengme

The link contained a binary file.

## Solution

Let's start by running the binary:

```console
root@kali:/media/sf_CTFs/bsidestlv/Where_no_man_has_GOne_before# ./revengme
Enter your password:test
Don't Worry, Relax, Chill and Try harder
```

We need to enter a password. Time to view the disassembly.

Since this is a golang binary, we'll use the [golang_renamer.py](https://github.com/ghidraninja/ghidra_scripts) script to restores function names from the stripped Go binary.

After the dust from running the script settles, we can take a look at the `main.main` implementation. It's long and complicated, but a single function call stands out:

```c
  if ((lVar9 == lVar7) &&
     (runtime_memequal_4023F0
                ((char)pplVar6,(char)puVar5,(char)local_118,uVar3,in_R8B,in_R9B,local_118,uVar8,
                 lVar7,(char)uVar8), uVar2 = extraout_DL_05, (char)uVar8 != '\0')) {
    local_f0 = 0x1e;
    main_ObfStr_488AF0(pplVar6,puVar5,extraout_DL_05,uVar3,in_R8B,in_R9B,&local_18c,0x1e,0x1e,uVar8,
                       lVar9);
    runtime_convTstring_408B60
              ((char)pplVar6,(char)puVar5,extraout_DL_06,(char)uVar8,in_R8B,in_R9B,uVar8,lVar9,
               local_f0);
    local_f8 = &DAT_0049a600;
    fmt_Fprintln_4822C0(pplVar6,(undefined *)puVar5,extraout_DL_07,0x60,in_R8B,in_R9B,
                        &PTR_DAT_004d3a60,DAT_0055b7f0,&local_f8,1,1,lVar10,local_1d8,
                        in_stack_fffffffffffffe30);
    return;
  }
```

A single `memequal` call in a program that requests a password? That's worth a breakpoint.

```console
gdb-peda$ b *0x00488e99
Breakpoint 2 at 0x488e99
gdb-peda$ r
Starting program: /media/sf_CTFs/bsidestlv/Where_no_man_has_GOne_before/revengme 
[New LWP 1766]
[New LWP 1767]
Enter your password:aaaaaaaaaa
RAX: 0xc00001c0f0 ("BSidesTLV{revenge is best served cold}")
RBX: 0x26 ('&')
RCX: 0xa ('\n')
RDX: 0x26 ('&')
RSI: 0xc00001c0c0 ("BSidesTLV{revenge is best served cold}")
RDI: 0xc00001c0f0 ("BSidesTLV{revenge is best served cold}")
RBP: 0xc00006af88 --> 0xc00006af90 --> 0x4299fc (mov    eax,DWORD PTR [rip+0x14d62e]        # 0x577030)
RSP: 0xc00006ad88 --> 0xc00006ae22 --> 0xe6fed9cfcec3f9e8 
RIP: 0x488e99 (cmp    QWORD PTR [rsp+0x20],rcx)
R8 : 0x1 
R9 : 0x0 
R10: 0xc00001c0f0 ("BSidesTLV{revenge is best served cold}")
R11: 0x0 
R12: 0xffffffffffffffff 
R13: 0x6 
R14: 0x5 
R15: 0xaa
EFLAGS: 0x212 (carry parity ADJUST zero sign trap INTERRUPT direction overflow)
[-------------------------------------code-------------------------------------]
   0x488e8a:	call   0x488af0
   0x488e8f:	mov    rax,QWORD PTR [rsp+0x18]
   0x488e94:	mov    rcx,QWORD PTR [rsp+0x40]
=> 0x488e99:	cmp    QWORD PTR [rsp+0x20],rcx
   0x488e9e:	je     0x488f52
   0x488ea4:	lea    rax,[rsp+0xc0]
   0x488eac:	mov    QWORD PTR [rsp],rax
   0x488eb0:	mov    QWORD PTR [rsp+0x8],0x28
[------------------------------------stack-------------------------------------]
0000| 0xc00006ad88 --> 0xc00006ae22 --> 0xe6fed9cfcec3f9e8 
0008| 0xc00006ad90 --> 0x26 ('&')
0016| 0xc00006ad98 --> 0x26 ('&')
0024| 0xc00006ada0 --> 0xc00001c0f0 ("BSidesTLV{revenge is best served cold}")
0032| 0xc00006ada8 --> 0x26 ('&')
0040| 0xc00006adb0 --> 0x0 
0048| 0xc00006adb8 --> 0x0 
0056| 0xc00006adc0 --> 0x0 
[------------------------------------------------------------------------------]
Legend: code, data, rodata, value

Thread 1 "revengme" hit Breakpoint 2, 0x0000000000488e99 in ?? ()
gdb-peda$ 
```

We hit the breakpoint and the flag is all over the place.

```console
root@kali:/media/sf_CTFs/bsidestlv/Where_no_man_has_GOne_before# ./revengme
Enter your password:BSidesTLV{revenge is best served cold}
You Cracked it, A Hero is born
```