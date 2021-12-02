# gogo
Category: Reverse Engineering, 110 points

## Description
> Hmmm this is a weird file...

A binary file was attached.

## Solution

Let's run the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/gogo]
└─$ ./enter_password
Enter Password: test
```

So we need to find the password.

Let's check the disassembly with Ghidra. We can see that there's a function called `checkPassword`:

```c
void __regparm1 main.checkPassword(undefined4 param_1,int param_2,uint param_3,undefined param_4)

{
  uint *puVar1;
  uint i;
  undefined4 uVar2;
  int correct_chars;
  int *in_GS_OFFSET;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  byte local_20 [28];
  undefined4 uStack4;
  
  puVar1 = (uint *)(*(int *)(*in_GS_OFFSET + -4) + 8);
  if (register0x00000010 < (undefined *)*puVar1 ||
      (undefined *)register0x00000010 == (undefined *)*puVar1) {
    uStack4 = 0x80d4b72;
    uVar2 = runtime.morestack_noctxt(param_1);
    main.checkPassword(uVar2,param_2,param_3,param_4);
    return;
  }
  if ((int)param_3 < 0x20) {
    os.Exit(param_1,0);
  }
  FUN_08090b18(0);
  local_40 = 0x38313638;
  local_3c = 0x31663633;
  local_38 = 0x64336533;
  local_34 = 0x64373236;
  local_30 = 0x37336166;
  local_2c = 0x62646235;
  local_28 = 0x39383338;
  local_24 = 0x65343132;
  FUN_08090fe0();
  i = 0;
  correct_chars = 0;
  while( true ) {
    if (0x1f < (int)i) {
      if (correct_chars == 0x20) {
        return;
      }
      return;
    }
    if ((param_3 <= i) || (0x1f < i)) break;
    if ((*(byte *)(param_2 + i) ^ *(byte *)((int)&local_40 + i)) == local_20[i]) {
      correct_chars = correct_chars + 1;
    }
    i = i + 1;
  }
  runtime.panicindex();
  do {
    invalidInstructionException();
  } while( true );
}
```

In this function, we see a loop performing a XOR between two characters and checking them against some expected output.

In assembly, it looks like this:

```assembly
   0x080d4b18 <+152>:   movzx  ebp,BYTE PTR [ecx+eax*1]
   0x080d4b1c <+156>:   cmp    eax,0x20
   0x080d4b1f <+159>:   jae    0x80d4b66 <main.checkPassword+230>
   0x080d4b21 <+161>:   movzx  esi,BYTE PTR [esp+eax*1+0x4]
   0x080d4b26 <+166>:   xor    ebp,esi
   0x080d4b28 <+168>:   movzx  esi,BYTE PTR [esp+eax*1+0x24]
   0x080d4b2d <+173>:   xchg   ebp,eax
   0x080d4b2e <+174>:   xchg   esi,ebx
   0x080d4b30 <+176>:   cmp    al,bl
```

So let's set a breakpoint at `0x080d4b28` and check the different arrays participating in the XOR calculation.

We know from the disassembly that the expected number of correct characters is `0x20`, so we'll enter 32 "a"s.

We can find our input at `$ecx`:
```
gef>  hexdump byte $ecx 32
0x18414260     61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61    aaaaaaaaaaaaaaaa
0x18414270     61 61 61 61 61 61 61 61 61 61 61 61 61 61 61 61    aaaaaaaaaaaaaaaa
```

The XORed values are at `$esp+0x4`:
```
gef>  hexdump byte $esp+0x4 32
0x18439f28     38 36 31 38 33 36 66 31 33 65 33 64 36 32 37 64    861836f13e3d627d
0x18439f38     66 61 33 37 35 62 64 62 38 33 38 39 32 31 34 65    fa375bdb8389214e
```

And the expected values are at `$esp+0x24`:

```
gef>  hexdump byte $esp+0x24 32
0x18439f48     4a 53 47 5d 41 45 03 54 5d 02 5a 0a 53 57 45 0d    JSG]AE.T].Z.SWE.
0x18439f58     05 00 5d 55 54 10 01 0e 41 55 57 4b 45 50 46 01    ..]UT...AUWKEPF.
```

So let's take these two arrays and XOR them to get the correct input:

```python
>>> from pwn import *
>>> xor(unhex("3836313833366631336533643632376466613337356264623833383932313465"), unhex("4a53475d414503545d025a0a5357450d05005d555410010e4155574b45504601"))
b'reverseengineericanbarelyforward'
```

Let's try that as the password:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/gogo]
└─$ ./enter_password
Enter Password: reverseengineericanbarelyforward
=========================================
This challenge is interrupted by psociety
What is the unhashed key?
```

Now we need to provide some unhashed key. The value at `$esp+0x4` looked like a hash, and if we Google it we find that it's the MD5 of `goldfish`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/gogo]
└─$ echo -n "goldfish" | md5sum
861836f13e3d627dfa375bdb8389214e  -
```

So let's enter that:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/gogo]
└─$ { echo "reverseengineericanbarelyforward"; echo "goldfish";} | nc mercury.picoctf.net 48728
Enter Password: =========================================
This challenge is interrupted by psociety
What is the unhashed key?
Flag is:  picoCTF{p1kap1ka_p1c0b187f1db}
```

The flag: `picoCTF{p1kap1ka_p1c0b187f1db}`