# ∑BIT
Category: Misc., 100 points

## Description

> We hid an email in this challenge, find it to get the flag (and feel free to send your CV to this email 😉).
> 
> Note: the flag is cstechnion{sha1(<email>)}

A binary file was attached.

## Solution

Let's check the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ file ./sigma_ctf
./sigma_ctf: ASCII text, with very long lines

┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ head ./sigma_ctf
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
```

Looks like base64, let's decode:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ base64 -d sigma_ctf > phase1

┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ file phase1
phase1: gzip compressed data, last modified: Sun Mar 31 15:30:16 2019, from Unix, original size modulo 2^32 7680
```

Now it's GZip, let's extract:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ mv phase1 phase1.gz

┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ gunzip -v phase1.gz
phase1.gz:       65.7% -- replaced with phase1

┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ file phase1
phase1: POSIX tar archive

┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ tar -xvf phase1
bin.elf
```

Finally, we got a binary:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ file bin.elf
bin.elf: ELF 32-bit LSB executable, ARM, EABI5 version 1 (SYSV), dynamically linked, interpreter /lib/ld-linux.so.3, for GNU/Linux 2.6.26, BuildID[sha1]=52e7f23013b23e494a72b69698259ff891ec04a3, not stripped
```

However it's compiled for ARM, so might not be streightforward to run. Let's start by inspecting the decompilation with Ghidra:

```c
undefined4 main(void)

{
  size_t sVar1;
  undefined4 uVar2;
  
  __isoc99_scanf(&DAT_00008638,input);
  sVar1 = strlen(input);
  if (sVar1 == 20) {
    if ((input._0_4_ ^ 0x12345678) == xor_result) {
      if (input._4_4_ + input._0_4_ == -0x1e2f342c) {
        if (input._8_4_ - input._4_4_ == -0xd28eff9) {
          if (input._8_4_ - input._12_4_ == -0x528ef05) {
            if (input._16_4_ + input._12_4_ == -0x26273b65) {
              printf("Correct! The flag is: %s\n",input);
              uVar2 = 0;
            }
            else {
              uVar2 = 1;
            }
          }
          else {
            uVar2 = 1;
          }
        }
        else {
          uVar2 = 1;
        }
      }
      else {
        uVar2 = 1;
      }
    }
    else {
      uVar2 = 1;
    }
  }
  else {
    uVar2 = 1;
  }
  return uVar2;
}
```

Seems easy enough to reverse:

```c
#include <stdio.h>

int main()
{
    int a[6] = {0};
    a[0] = 0x12345678 ^ 0x7F533F0B;
    a[1] = -0x1e2f342c - a[0];
    a[2] = -0xd28eff9 + a[1];
    a[3] = a[2] + 0x528ef05;
    a[4] = -0x26273b65 - a[3];
    printf("%s", (char*)&a);

    return 0;
}
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ gcc solve.c -o solve && ./solve
sigmabithr@gmail.com
```

So the flag is:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/BIT]
└─$ echo -n "sigmabithr@gmail.com" | sha1sum | awk '{ printf "cstechnion{%s}", $1; }'
cstechnion{cc13535a16336db29c278b29a9c0ce7cf4b1e7c8}
```