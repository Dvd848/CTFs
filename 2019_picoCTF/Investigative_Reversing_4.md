# Investigative Reversing 4
Forensics, 400 points

## Description:
> We have recovered a binary and 5 images: image01, image02, image03, image04, image05. See what you can make of it. There should be a flag somewhere.


## Solution: 

This is the follow-up for [Investigative Reversing 3](Investigative_Reversing_3.md).

Again, let's check the binary with Ghidra:

```c
undefined8 main(void)
{
  size_t sVar1;
  undefined4 local_4c;
  undefined local_48 [52];
  int local_14;
  FILE *flag_file;
  
  flag = local_48;
  local_4c = 0;
  flag_index = &local_4c;
  flag_file = fopen("flag.txt","r");
  if (flag_file == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  sVar1 = fread(flag,0x32,1,flag_file);
  local_14 = (int)sVar1;
  if (local_14 < 1) {
    puts("Invalid Flag");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  fclose(flag_file);
  encodeAll();
  return 0;
}

void encodeAll(void)
{
  ulong local_48;
  undefined8 local_40;
  undefined4 local_38;
  ulong local_28;
  undefined8 local_20;
  undefined4 local_18;
  char local_9;
  
  local_28 = 7160496016605934665;   // "Item01_c"
  local_20 = 482871488112;          // "p.bmp\x00\x00\x00"
  local_18 = 0;
  local_48 = 7074646148709184585;   // "Item01.b"
  local_40 = 28781;                 // "mp\x00\x00"
  local_38 = 0;
  local_9 = '5';
  while ('0' < local_9) {
    local_48._0_6_ = CONCAT15(local_9,(undefined5)local_48);
    local_48 = local_48 & 0xffff000000000000 | (ulong)(uint6)local_48;
    local_28._0_6_ = CONCAT15(local_9,(undefined5)local_28);
    local_28 = local_28 & 0xffff000000000000 | (ulong)(uint6)local_28;
    encodeDataInFile((char *)&local_48,(char *)&local_28);
    local_9 = local_9 + -1;
  }
  return;
}

void encodeDataInFile(char *src_file,char *dst_file)
{
  size_t sVar1;
  ulong uVar2;
  byte b;
  char encoded_b;
  int limit;
  FILE *dst_fd;
  FILE *src_fd;
  int k;
  int j;
  int i;
  int local_c;
  
  src_fd = fopen(src_file,"r");
  dst_fd = fopen(dst_file,"a");
  if (src_fd != (FILE *)0x0) {
    sVar1 = fread(&b,1,1,src_fd);
    local_c = (int)sVar1;
    limit = 2019;
    i = 0;
    while (i < limit) {
      fputc((int)(char)b,dst_fd);
      sVar1 = fread(&b,1,1,src_fd);
      local_c = (int)sVar1;
      i = i + 1;
    }
    j = 0;
    while (j < 50) {
      if (j % 5 == 0) {
        k = 0;
        while (k < 8) {
          uVar2 = codedChar(k,*(byte *)(*flag_index + flag),b);
          encoded_b = (char)uVar2;
          fputc((int)encoded_b,dst_fd);
          fread(&b,1,1,src_fd);
          k = k + 1;
        }
        *flag_index = *flag_index + 1;
      }
      else {
        fputc((int)(char)b,dst_fd);
        fread(&b,1,1,src_fd);
      }
      j = j + 1;
    }
    while (local_c == 1) {
      fputc((int)(char)b,dst_fd);
      sVar1 = fread(&b,1,1,src_fd);
      local_c = (int)sVar1;
    }
    fclose(dst_fd);
    fclose(src_fd);
    return;
  }
  puts("No output found, please run this on the server");
                    /* WARNING: Subroutine does not return */
  exit(0);
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

In this case the data is encoded in 5 files: `Item05_cp.bmp` to `Item01_cp.bmp`. In each case, the program jumps to offset `2019` and LSB-encodes a byte from the flag in 8 bytes from the original file, then copies another 4 bytes from the original file. Every file contains 10 bytes from the flag.

For example:

```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_4# xxd -g 1 -s $((2019 - 32)) -l $((10*8 + 48 + 64)) Item05_cp.bmp
000007c3: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
000007d3: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
000007e3: e8 e8 e8 e8 e9 e9 e9 e8 e8 e8 e8 e8 e9 e8 e8 e9  ................
000007f3: e8 e9 e9 e8 e8 e8 e8 e8 e9 e9 e8 e8 e8 e9 e9 4e  ...............N
00000803: 4f 4f 4f 4f e9 e9 e9 e9 e8 e9 e9 e8 e8 e8 e8 e8  OOOO............
00000813: e9 e9 e8 e8 e8 e8 e9 e8 e8 e8 e8 e8 e8 e8 e9 e8  ................
00000823: e9 e8 e9 e8 e8 e8 e8 e8 e8 e9 e9 e8 e8 e8 e9 e8  ................
00000833: e8 e8 e8 e8 e9 e9 e8 e9 e9 e9 e9 e8 e8 e8 e8 e8  ................
00000843: e8 e9 e9 e9 e8 e8 e9 e8 e8 e8 e8 e8 e9 e8 e8 e8  ................
00000853: e9 e9 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
00000863: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
00000873: e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8 e8  ................
```

Therefore, to extract the flag, we use the following script:

```python
from pwn import *

bin_str = ""
for i in range(5, 0, -1):
    with open("Item0{}_cp.bmp".format(i), "rb") as b:
        b.seek(2019)
        
        for j in range(50):
            if ((j % 5) == 0):
                for k in range(8):
                    bin_str += str(ord(b.read(1)) & 1)
            else:
                b.read(1)

char_str = unbits(bin_str, endian = 'little')
print char_str
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_4# python solve.py
picoCTF{N1c3_R3ver51ng_5k1115_0000000000023ef6902}
```
