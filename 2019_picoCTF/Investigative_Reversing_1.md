# Investigative Reversing 1
Forensics, 350 points

## Description:
> We have recovered a binary and a few images: image, image2, image3. See what you can make of it. There should be a flag somewhere.


## Solution: 

This is the follow-up for [Investigative Reversing 0](Investigative_Reversing_0.md).

Again, let's check the binary with Ghidra:
```c
void main(void)

{
  long lVar1;
  FILE *flag_stream;
  FILE *m1_stream;
  FILE *m2_stream;
  FILE *m3_stream;
  long in_FS_OFFSET;
  char local_6b;
  int i;
  int j;
  int k;
  char flag [6];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  flag_stream = fopen("flag.txt","r");
  m1_stream = fopen("mystery.png","a");
  m2_stream = fopen("mystery2.png","a");
  m3_stream = fopen("mystery3.png","a");
  if (flag_stream == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  if (m1_stream == (FILE *)0x0) {
    puts("mystery.png is missing, please run this on the server");
  }
  fread(flag,26,1,flag_stream);
  fputc((int)flag[1],m3_stream);
  fputc((int)(char)(flag[0] + '\x15'),m2_stream);
  fputc((int)flag[2],m3_stream);
  local_6b = flag[3];
  fputc((int)flag[5],m3_stream);
  fputc((int)flag[4],m1_stream);
  i = 6;
  while (i < 10) {
    local_6b = local_6b + '\x01';
    fputc((int)flag[i],m1_stream);
    i = i + 1;
  }
  fputc((int)local_6b,m2_stream);
  j = 10;
  while (j < 0xf) {
    fputc((int)flag[j],m3_stream);
    j = j + 1;
  }
  k = 0xf;
  while (k < 0x1a) {
    fputc((int)flag[k],m1_stream);
    k = k + 1;
  }
  fclose(m1_stream);
  fclose(flag_stream);
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}
```

The encoded flag is scattered among three files:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_1# xxd -g 1 mystery.png | tail
0001e7f0: 82 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21  . .. .. .. d.2.!
0001e800: 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82  .. .. .. .B.!#..
0001e810: 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08   .. .. .. d.2.!.
0001e820: 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20  . .. .. .B.!#..
0001e830: 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08 82  .. .. .. d.2.!..
0001e840: 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20 08   .. .. .B.!#.. .
0001e850: 82 20 08 82 20 08 82 20 64 17 ff ef ff fd 7f 5e  . .. .. d......^
0001e860: ed 5a 9d 38 d0 1f 56 00 00 00 00 49 45 4e 44 ae  .Z.8..V....IEND.
0001e870: 42 60 82 43 46 7b 41 6e 31 5f 38 35 35 36 31 31  B`.CF{An1_855611
0001e880: 64 33 7d                                         d3}
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_1# xxd -g 1 mystery2.png | tail
0001e7e0: 21 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11  !.. .. .. .B.!#.
0001e7f0: 82 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21  . .. .. .. d.2.!
0001e800: 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82  .. .. .. .B.!#..
0001e810: 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08   .. .. .. d.2.!.
0001e820: 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20  . .. .. .B.!#..
0001e830: 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08 82  .. .. .. d.2.!..
0001e840: 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20 08   .. .. .B.!#.. .
0001e850: 82 20 08 82 20 08 82 20 64 17 ff ef ff fd 7f 5e  . .. .. d......^
0001e860: ed 5a 9d 38 d0 1f 56 00 00 00 00 49 45 4e 44 ae  .Z.8..V....IEND.
0001e870: 42 60 82 85 73                                   B`..s
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_1# xxd -g 1 mystery3.png | tail
0001e7e0: 21 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11  !.. .. .. .B.!#.
0001e7f0: 82 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21  . .. .. .. d.2.!
0001e800: 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82  .. .. .. .B.!#..
0001e810: 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08   .. .. .. d.2.!.
0001e820: 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20  . .. .. .B.!#..
0001e830: 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08 82  .. .. .. d.2.!..
0001e840: 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20 08   .. .. .B.!#.. .
0001e850: 82 20 08 82 20 08 82 20 64 17 ff ef ff fd 7f 5e  . .. .. d......^
0001e860: ed 5a 9d 38 d0 1f 56 00 00 00 00 49 45 4e 44 ae  .Z.8..V....IEND.
0001e870: 42 60 82 69 63 54 30 74 68 61 5f                 B`.icT0tha_
```

We can reassemble the flag using the following script:

```python
import os
import mmap

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDONLY)
    return mmap.mmap(fd, size, access=access)

class Mystery(object):
    PNG_CRC_LEN = 4
    def __init__(self, file_name):
        self.map = memory_map(file_name)
        self.buffer = self.map[self.map.find(b"IEND") + len(b"IEND") + self.PNG_CRC_LEN:]
        self.offset = 0

    def read_byte(self):
        b = self.buffer[self.offset]
        self.offset += 1
        return b

    def __del__(self):
        self.map.close()

FLAG_LEN = 26

flag = [0] * FLAG_LEN
m1_stream = Mystery("mystery.png")
m2_stream = Mystery("mystery2.png")
m3_stream = Mystery("mystery3.png")

flag[1] = m3_stream.read_byte()
flag[0] = m2_stream.read_byte() - 0x15
flag[2] = m3_stream.read_byte()
flag[5] = m3_stream.read_byte()
flag[4] = m1_stream.read_byte()
for i in range(6, 10):
    flag[i] = m1_stream.read_byte()
flag[3] = m2_stream.read_byte() - (10 - 6)
for i in range(10, 15):
    flag[i] = m3_stream.read_byte()
for i in range(15, 26):
    flag[i] = m1_stream.read_byte()

print ("".join(chr(x) for x in flag))
```

The script uses a memory map to map the contents of each file, so that it can search for the legitimate end of the PNG file (which is the location of `IEND` plus 4 more bytes for the `IEND` chunk CRC) and start reading the flag from there.

Output:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_1# python3 solve.py
picoCTF{An0tha_1_855611d3}
```