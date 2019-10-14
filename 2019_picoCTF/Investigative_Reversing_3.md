# Investigative Reversing 3
Forensics, 400 points

## Description:
> We have recovered a binary and an image See what you can make of it. There should be a flag somewhere.


## Solution: 


This is the follow-up for [Investigative Reversing 2](Investigative_Reversing_2.md).

Again, let's check the binary with Ghidra:

```c
undefined8 main(void)
{
  long lVar1;
  size_t sVar2;
  ulong uVar3;
  long in_FS_OFFSET;
  byte b;
  char local_7d;
  int bytes_read;
  int i;
  uint j;
  int k;
  undefined4 local_6c;
  int limit;
  int bytes_read_from_flag;
  FILE *flag_file;
  FILE *original_file;
  FILE *encoded_file;
  byte flag [50];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  local_6c = 0;
  flag_file = fopen("flag.txt","r");
  original_file = fopen("original.bmp","r");
  encoded_file = fopen("encoded.bmp","a");
  if (flag_file == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  if (original_file == (FILE *)0x0) {
    puts("No output found, please run this on the server");
  }
  sVar2 = fread(&b,1,1,original_file);
  bytes_read = (int)sVar2;
  limit = 0x2d3;
  i = 0;
  while (i < limit) {
    fputc((int)(char)b,encoded_file);
    sVar2 = fread(&b,1,1,original_file);
    bytes_read = (int)sVar2;
    i = i + 1;
  }
  sVar2 = fread(flag,50,1,flag_file);
  bytes_read_from_flag = (int)sVar2;
  if (bytes_read_from_flag < 1) {
    puts("Invalid Flag");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  j = 0;
  while ((int)j < 100) {
    if ((j & 1) == 0) {
      k = 0;
      while (k < 8) {
        uVar3 = codedChar(k,flag[(int)j / 2],b);
        local_7d = (char)uVar3;
        fputc((int)local_7d,encoded_file);
        fread(&b,1,1,original_file);
        k = k + 1;
      }
    }
    else {
      fputc((int)(char)b,encoded_file);
      fread(&b,1,1,original_file);
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

We see that the flag is encoded using LSB encoding, in a similar manner to the previous challenge. This time, the encoding starts from offset 0x2d3. 8 bits of payload are encoded in the LSB of 8 bytes of the image, and then one byte of the original image is placed as-is.


```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_3# xxd -g 1 -s $((0x2d3 - 32)) -l $((50*8 + 48 + 64)) encoded.bmp
000002b3: 60 e0 00 80 80 00 00 80 80 20 00 80 80 40 00 80  `........ ...@..
000002c3: 80 60 00 80 80 80 00 80 80 a0 00 80 80 c0 00 80  .`..............
000002d3: 80 e0 00 80 a1 01 01 80 a0 21 00 80 a1 40 01 81  .........!...@..
000002e3: a0 60 01 81 a0 80 00 81 a1 a0 00 81 a1 c1 01 80  .`..............
000002f3: a1 e1 00 80 c1 01 00 80 c0 20 01 80 c0 40 00 81  ......... ...@..
00000303: c0 61 00 81 c0 80 00 81 c1 a0 00 80 c1 c0 00 81  .a..............
00000313: c1 e0 01 81 e1 01 00 80 e0 20 01 80 e1 41 00 80  ......... ...A..
00000323: e0 60 01 81 e1 80 01 81 e0 a0 00 80 e0 c0 01 81  .`..............
00000333: e0 e0 00 c0 00 01 00 c1 01 21 00 c0 00 40 00 c1  .........!...@..
00000343: 00 61 01 c0 00 81 01 c0 00 a1 01 c0 00 c0 00 c1  .a..............
00000353: 00 e0 01 c1 21 00 00 c1 21 21 01 c1 20 41 00 c0  ....!...!!.. A..
00000363: 20 60 01 c1 20 80 01 c0 20 a1 00 c1 20 c1 01 c0   `.. ... ... ...
00000373: 20 e0 00 c1 40 00 00 c1 41 20 00 c1 41 41 01 c1   ...@...A ..AA..
00000383: 40 61 00 c0 40 80 00 c0 41 a1 01 c0 40 c0 01 c0  @a..@...A...@...
00000393: 40 e1 01 c1 60 00 00 c0 60 20 01 c1 60 40 00 c0  @...`...` ..`@..
000003a3: 61 60 00 c0 61 81 00 c0 60 a0 01 c1 60 c1 01 c0  a`..a...`...`...
000003b3: 60 e1 01 c0 80 01 01 c0 80 20 01 c0 81 41 00 c1  `........ ...A..
000003c3: 81 60 00 c1 81 81 01 c1 80 a1 00 c0 80 c0 00 c0  .`..............
000003d3: 81 e1 00 c0 a0 00 00 c0 a0 21 01 c0 a0 40 00 c0  .........!...@..
000003e3: a0 60 01 c1 a0 80 00 c0 a0 a0 00 c1 a1 c0 00 c0  .`..............
000003f3: a0 e0 00 c0 c1 01 00 c0 c0 20 00 c0 c0 41 01 c0  ......... ...A..
00000403: c0 60 00 c0 c0 80 01 c1 c0 a0 00 f0 fa fe 00 a5  .`..............
00000413: a1 a0 00 80 80 80 00 00 01 ff 00 00 ff 00 00 00  ................
00000423: fe ff 01 fe 00 00 00 fe 00 fe 01 ff fe 00 00 fe  ................
00000433: fe fe 00 e9 e9 e8 e8 e8 e8 e8 e8 e8 e9 e9 e8 e8  ................
00000443: e8 e9 e8 e8 e8 e9 e9 e8 e8 e8 e8 e8 e8 e9 e9 e9  ................
00000453: e8 e8 e8 e9 e8 e8 e8 e8 e9 e9 e8 e8 e8 e9 e8 e8  ................
00000463: e9 e9 e8 e8 e8 e9 e9 e9 e8 e9 e9 e8 e8 e8 e8 e8  ................
00000473: e8 e8 e9 e9 e8 e8 e8 e9 e8 e8 e8 e8 e9 e9 e8 e8  ................
00000483: e9 e8 e9 e8 e8 e9 e9 e8 e8 e9 e8 e9 e9 e9 e9 e9  ................
00000493: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
000004a3: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
```

We reverse the logic with the following script:

```python
from pwn import *

with open("encoded.bmp", "rb") as b:
    b.seek(0x2d3)
    bin_str = ""
    for j in range(100):
        if ((j & 1) == 0):
            for k in range(8):
                bin_str += str(ord(b.read(1)) & 1)
        else:
            b.read(1)

char_str = unbits(bin_str, endian = 'little')
print char_str
```

The result:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_3# python solve.py
picoCTF{4n0th3r_L5b_pr0bl3m_000000000000018a270ae}
```