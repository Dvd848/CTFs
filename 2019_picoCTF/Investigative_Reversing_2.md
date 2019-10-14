# Investigative Reversing 2
Forensics, 350 points

## Description:
> We have recovered a binary and an image See what you can make of it. There should be a flag somewhere.


## Solution: 

This is the follow-up for [Investigative Reversing 1](Investigative_Reversing_1.md).

Again, let's check the binary with Ghidra:

```c
undefined8 main(void)
{
  long lVar1;
  size_t sVar2;
  ulong uVar3;
  long in_FS_OFFSET;
  byte b;
  char new_val;
  int bytes_read;
  int i;
  int j;
  int k;
  undefined4 local_6c;
  int limit;
  int flag_bytes_read;
  FILE *flag_file;
  FILE *original_file;
  FILE *encoded_file;
  char flag [50];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  local_6c = 0;
  flag_file = fopen("flag.txt","r");
  original_file = fopen("original.bmp","r");
  encoded_file = fopen("encoded.bmp","a");
  if (flag_file == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  if (original_file == (FILE *)0x0) {
    puts("original.bmp is missing, please run this on the server");
  }
  sVar2 = fread(&b,1,1,original_file);
  bytes_read = (int)sVar2;
  limit = 2000;
  i = 0;
  while (i < limit) {
    fputc((int)(char)b,encoded_file);
    sVar2 = fread(&b,1,1,original_file);
    bytes_read = (int)sVar2;
    i = i + 1;
  }
  sVar2 = fread(flag,50,1,flag_file);
  flag_bytes_read = (int)sVar2;
  if (flag_bytes_read < 1) {
    puts("flag is not 50 chars");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  j = 0;
  while (j < 50) {
    k = 0;
    while (k < 8) {
      uVar3 = codedChar(k,flag[j] - 5,b);
      new_val = (char)uVar3;
      fputc((int)new_val,encoded_file);
      fread(&b,1,1,original_file);
      k = k + 1;
    }
    j = j + 1;
  }
  while (bytes_read == 1) {
    fputc((int)(char)b,encoded_file);
    sVar2 = fread(&b,1,1,original_file);
    bytes_read = (int)sVar2;
  }
  fclose(encoded_file);
  fclose(original_file);
  fclose(flag_file);
  if (lVar1 == *(long *)(in_FS_OFFSET + 0x28)) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

ulong codedChar(int param_1,byte param_2,byte param_3)
{
  byte local_20;
  
  local_20 = param_2;
  if (param_1 != 0) {
    local_20 = (byte)((int)(char)param_2 >> ((byte)param_1 & 0x1f));
  }
  return (ulong)(param_3 & 0xfe | local_20 & 1);
}
```

This time the program jumps to offset 2000, and hides the flag using LSB encoding.

```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_2# xxd -g 1 -s $((2000 - 32)) -l $((50*8 + 64)) encoded.bmp
000007b0: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
000007c0: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
000007d0: e9 e9 e8 e9 e8 e9 e9 e8 e8 e8 e9 e8 e8 e9 e9 e8  ................
000007e0: e8 e9 e9 e9 e9 e8 e9 e8 e8 e9 e8 e9 e8 e9 e9 e8  ................
000007f0: e8 e9 e9 e9 e9 e9 e8 e8 e9 e9 e9 e9 e8 e8 e9 e8  ................
00000800: e9 e8 4e 4e 4e 4e 4f e8 e8 e9 e9 e8 e9 e9 e9 e8  ..NNNNO.........
00000810: e9 e8 e8 e9 e8 e9 e9 e8 e8 e9 e9 e9 e8 e9 e8 e8  ................
00000820: e9 e9 e8 e8 e9 e9 e9 e8 e9 e9 e9 e9 e8 e9 e9 e8  ................
00000830: e8 e9 e8 e9 e9 e8 e9 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000840: e9 e8 e8 e9 e8 e9 e9 e8 e8 e9 e9 e9 e8 e9 e8 e8  ................
00000850: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000860: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000870: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000880: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000890: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008a0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008b0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008c0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008d0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008e0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
000008f0: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000900: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000910: e9 e9 e8 e9 e8 e9 e8 e8 e9 e9 e8 e9 e8 e9 e8 e8  ................
00000920: e8 e8 e8 e8 e8 e9 e9 e8 e8 e9 e9 e9 e8 e9 e8 e8  ................
00000930: e9 e9 e8 e9 e8 e9 e8 e8 e8 e9 e8 e8 e9 e9 e8 e8  ................
00000940: e9 e8 e9 e9 e9 e8 e9 e8 e9 e8 e9 e9 e8 e9 e8 e8  ................
00000950: e9 e8 e8 e8 e9 e9 e8 e8 e8 e8 e8 e9 e9 e9 e9 e8  ................
00000960: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
00000970: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
```

We can see that up to offset 2000 (0x7d0) we have a constant value of `0xe8`. Then, for 50 * 8 bytes we have different values, and finally, at offset `0x960` we're back to `0xe8`.

Let's undo the logic with the following script:

```python
from pwn import *

with open("encoded.bmp", "rb") as b:
    b.seek(2000)
    bin_str = ""
    for j in range(50 * 8):
        bin_str += str(ord(b.read(1)) & 1)

char_str = unbits(bin_str, endian = 'little')
print "".join(map(lambda c: chr(ord(c) + 5), char_str))
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_2# python solve.py
picoCTF{n3xt_0n300000000000000000000000000e307b26}
```