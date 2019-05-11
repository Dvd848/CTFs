# Patches' Punches
Reversing, 50 points

## Description

> That moment when you go for a body slam and you realize you jump too far. Adjust your aim, and you'll crush this challenge!

A binary file was attached.

## Solution

Let's run the file:

```console
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# ./patches
Woah there! you jumped over the flag.
```

Not much there. If we jumped over the flag though, it might be available via `strings`:
```console
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# strings patches
...
Hurray the flag is %s
Woah there! you jumped over the flag.
;*2$"
zyq|Xu3Px~_{Uo}TmfUq2E3piVtJ2nf!}
...
```

The flag seems encrypted, but there is hope - There's a string that hints that it is supposed to print the flag.

Let's open the binary with Ghidra. The decompilation of main shows:

```c
/* WARNING: Removing unreachable block (ram,0x00010546) */
/* WARNING: Removing unreachable block (ram,0x00010580) */
/* WARNING: Removing unreachable block (ram,0x0001054f) */
/* WARNING: Removing unreachable block (ram,0x00010586) */

undefined4 main(undefined1 param_1)

{
  int iVar1;
  
  iVar1 = __x86.get_pc_thunk.ax();
  printf((char *)(iVar1 + 0x134));
  return 0;
}
```

There's not much to work with here. However, the disassembly is much longer than what we are seeing (pasted from `radare2` since it supports text mode):

```
[0x0000051d]> VV @ main (nodes 7 edges 8 zoom 100%) BB-NORM mouse:canvas-y mov-speed:5
                                         .-----------------------------------------------.
                                         |  0x51d                                        |
                                         | (fcn) main 169                                |
                                         |   main (int argc, char **argv, char **envp);  |
                                         | ; var unsigned int local_10h @ ebp-0x10       |
                                         | ; var signed int local_ch @ ebp-0xc           |
                                         | ; var int local_8h @ ebp-0x8                  |
                                         | ; arg int arg_4h @ esp+0x4                    |
                                         | lea ecx, [arg_4h]                             |
                                         | and esp, 0xfffffff0                           |
                                         | push dword [ecx - 4]                          |
                                         | push ebp                                      |
                                         | mov ebp, esp                                  |
                                         | push ebx                                      |
                                         | push ecx                                      |
                                         | sub esp, 0x10                                 |
                                         | call sym.__x86.get_pc_thunk.ax;[ga]           |
                                         | add eax, 0x1aa4                               |
                                         | mov dword [local_10h], 1                      |
                                         | cmp dword [local_10h], 0                      |
                                         | jne 0x5a3;[gb]                                |
                                         `-----------------------------------------------'
                                                 f t
                                                 | |
                                                 | '-------------------------.
                           .---------------------'                           |
                           |                                                 |
                       .-------------------------------.                 .-----------------------------------.
                       |  0x546                        |                 |  0x5a3                            |
                       | mov dword [local_ch], 0       |                 | ; CODE XREF from main (0x544)     |
                       | jmp 0x580;[gc]                |                 | sub esp, 0xc                      |
                       `-------------------------------'                 | lea edx, [eax - 0x1970]           |
                           v                                             | ; const char *format              |
                           |                                             | push edx                          |
                           |                                             | mov ebx, eax                      |
                           |                                             | ; int printf(const char *format)  |
                           |                                             | call sym.imp.printf;[ge]          |
                           |                                             | add esp, 0x10                     |
                           |                                             `-----------------------------------'
                           |                                                 v
                           |                                                 |
                           |                                                 |
                           |                                                 '-------------.
.----------------------------.                                                             |
|                          | |                                                             |
|                    .-----------------------------------.                                 |
|                    |  0x580                            |                                 |
|                    | ; CODE XREF from main (0x54d)     |                                 |
|                    | cmp dword [local_ch], 0x1e        |                                 |
|                    | jle 0x54f;[gd]                    |                                 |
|                    `-----------------------------------'                                 |
|                          t f                                                             |
|                          | |                                                             |
|    .---------------------' |                                                             |
|    |                       '-------------------------.                                   |
|    |                                                 |                                   |
|.--------------------------------------------.    .-----------------------------------.   |
||  0x54f                                     |    |  0x586                            |   |
|| ; CODE XREF from main (0x584)              |    | sub esp, 8                        |   |
|| lea ecx, [eax + 0xc8]                      |    | lea edx, [eax + 0xc8]             |   |
|| mov edx, dword [local_ch]                  |    | push edx                          |   |
|| add edx, ecx                               |    | lea edx, [eax - 0x1988]           |   |
|| movzx edx, byte [edx]                      |    | ; const char *format              |   |
|| mov ecx, edx                               |    | push edx                          |   |
|| mov edx, dword [local_ch]                  |    | mov ebx, eax                      |   |
|| ; [0x48:4]=288                             |    | ; int printf(const char *format)  |   |
|| ; 'H'                                      |    | call sym.imp.printf;[ge]          |   |
|| mov edx, dword [eax + edx*4 + 0x48]        |    | add esp, 0x10                     |   |
|| sub ecx, edx                               |    | jmp 0x5b7;[gf]                    |   |
|| mov edx, ecx                               |    `-----------------------------------'   |
|| mov ebx, edx                               |        v                                   |
|| lea ecx, [eax + 0xc8]                      |        |                                   |
|| mov edx, dword [local_ch]                  |        |                                   |
|| add edx, ecx                               |        |                                   |
|| mov byte [edx], bl                         |        |                                   |
|| add dword [local_ch], 1                    |        |                                   |
|`--------------------------------------------'        |                                   |
|    v                                                 |                                   |
|    |                                                 |                                   |
`----'                                                 |                                   |
                                                       '--------------.                    |
                                                                      | .------------------'
                                                                      | |
                                                                .-------------------------------.
                                                                |  0x5b7                        |
                                                                | ; CODE XREF from main (0x5a1) |
                                                                | mov eax, 0                    |
                                                                | lea esp, [local_8h]           |
                                                                | pop ecx                       |
                                                                | pop ebx                       |
                                                                | pop ebp                       |
                                                                | lea esp, [ecx - 4]            |
                                                                | ret                           |
                                                                `-------------------------------'

```

It turns out that by default, Ghidra removes unreachable code from the decompilation window. It even warned us with a comment above `main()`.

This behavior can be modified by entering Edit -> Tool Options -> Decompiler -> Analysis and unchecking "Eliminate unreachable code".

Now we get the following code:
```c
undefined4 main(undefined1 param_1)

{
  int iVar1;
  int local_14;
  
  iVar1 = __x86.get_pc_thunk.ax();
  if (true) {
    printf((char *)(iVar1 + 0x134));
  }
  else {
    local_14 = 0;
    while (local_14 < 0x1f) {
      *(char *)(local_14 + iVar1 + 0x1b6c) =
           *(char *)(local_14 + iVar1 + 0x1b6c) -
           (char)*(undefined4 *)(iVar1 + 0x1aec + local_14 * 4);
      local_14 = local_14 + 1;
    }
    printf((char *)(iVar1 + 0x11c),iVar1 + 0x1b6c);
  }
  return 0;
}
```

We obviously want to change the "if (true)" to "if (false)". 

In assembly, we have:
```assembly
0x00000539      c745f0010000.  mov dword [local_10h], 1
0x00000540      837df000       cmp dword [local_10h], 0
```

We'd like to change the immediate value being stored in the local variable to the value being compared right after it.

We can try to do this with Ghidra:
Right click on the line we want to patch -> Select "Patch Instruction" -> Change the "1" to "0". Now we can export with "File -> Export Program -> Binary".

Let's run the new program:
```console
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# ./patches2.bin
Segmentation fault
```

Turns out Ghidra has a [bug](https://github.com/NationalSecurityAgency/ghidra/issues/19) related to patching - it messes up the ELF headers. The bug report offers a workaround but let's try `r2` instead:

```
[0x0000051d]> oo+
[0x0000051d]> s 0x00000539
[0x00000539]> "wa mov dword [var_10h], 0"
Cannot assemble 'mov dword [var_10h], 0' at line 3
```

Really?

How about IDA? Mark the line and then select Edit -> Patch program -> Assemble -> Enter new assembly.
Here, we get a warning window: "`Warning: Invalid operand size(s)`".

We'll just use `dd` to patch the byte:
```console
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# xxd -g 1 -s $((0x539)) -l 7 patches
00000539: c7 45 f0 01 00 00 00                             .E.....
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# cp patches patches.old
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# echo '00' | xxd -p -r | dd conv=notrunc of=patches bs=1 seek=$((0x53c))
1+0 records in
1+0 records out
1 byte copied, 0.0014464 s, 0.7 kB/s
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# xxd -g 1 -s $((0x539)) -l 7 patches
00000539: c7 45 f0 00 00 00 00                             .E.....
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# radiff2 patches patches.old
0x0000053c 00 => 01 0x0000053c
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# rasm2 -d $(xxd -p -s $((0x539)) -l 7 patches)
mov dword [ebp - 0x10], 0
```

And it turns out we could have used this as well:
```
[0x000003e0]> s 0x539
[0x00000539]> oo+
[0x00000539]> "wa mov dword [var_10h], 0"
Cannot assemble 'mov dword [var_10h], 0' at line 3
[0x00000539]> "wa mov dword [ebp - 0x10], 0"
Written 7 byte(s) (mov dword [ebp - 0x10], 0) = wx c745f000000000
```

Now, when we run the program, we get:
```
root@kali:/media/sf_CTFs/sunshine/Patches_Punches# ./patches
Hurray the flag is sun{To0HotToHanDleTo0C0ldToH0ld!}
```