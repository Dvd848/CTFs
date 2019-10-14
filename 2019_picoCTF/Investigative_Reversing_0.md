# Investigative Reversing 0
Forensics, 300 points

## Description:
> We have recovered a binary and an image. See what you can make of it. There should be a flag somewhere.


## Solution: 

We receive a binary file and an image. Let's inspect the binary with Ghidra:

```c
void main(void)

{
  long lVar1;
  FILE *__flag_stream;
  FILE *__img_stream;
  size_t sVar2;
  long in_FS_OFFSET;
  int i;
  int j;
  char flag [26];
  
  lVar1 = *(long *)(in_FS_OFFSET + 0x28);
  __flag_stream = fopen("flag.txt","r");
  __img_stream = fopen("mystery.png","a");
  if (__flag_stream == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  if (__img_stream == (FILE *)0x0) {
    puts("mystery.png is missing, please run this on the server");
  }
  sVar2 = fread(flag,26,1,__flag_stream);
  if ((int)sVar2 < 1) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  puts("at insert");
  fputc((int)flag[0],__img_stream);
  fputc((int)flag[1],__img_stream);
  fputc((int)flag[2],__img_stream);
  fputc((int)flag[3],__img_stream);
  fputc((int)flag[4],__img_stream);
  fputc((int)flag[5],__img_stream);
  i = 6;
  while (i < 0xf) {
    fputc((int)(char)(flag[i] + '\x05'),__img_stream);
    i = i + 1;
  }
  fputc((int)(char)(flag[15] + -3),__img_stream);
  j = 16;
  while (j < 26) {
    fputc((int)flag[j],__img_stream);
    j = j + 1;
  }
  fclose(__img_stream);
  fclose(__flag_stream);
  if (lVar1 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}
```

We can see that the program opens the flag file, and places an encoded version of it at the end of the image file:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_0# xxd -g 1 mystery.png | tail
0001e7f0: 82 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21  . .. .. .. d.2.!
0001e800: 08 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82  .. .. .. .B.!#..
0001e810: 20 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08   .. .. .. d.2.!.
0001e820: 82 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20  . .. .. .B.!#..
0001e830: 08 82 20 08 82 20 08 82 20 64 1f 32 12 21 08 82  .. .. .. d.2.!..
0001e840: 20 08 82 20 08 82 20 08 42 f6 21 23 11 82 20 08   .. .. .B.!#.. .
0001e850: 82 20 08 82 20 08 82 20 64 17 ff ef ff fd 7f 5e  . .. .. d......^
0001e860: ed 5a 9d 38 d0 1f 56 00 00 00 00 49 45 4e 44 ae  .Z.8..V....IEND.
0001e870: 42 60 82 70 69 63 6f 43 54 4b 80 6b 35 7a 73 69  B`.picoCTK.k5zsi
0001e880: 64 36 71 5f 66 62 36 39 66 36 63 32 7d           d6q_fb69f6c2}
```

We can easily reverse the logic with the following script:
```python
import os
import mmap

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDONLY)
    return mmap.mmap(fd, size, access=access)

with memory_map("mystery.png") as b:
    flag = b[-26:]
    for i in range(6):
        print(chr(flag[i]), end='')
    for i in range(6, 15):
        print(chr(flag[i] - 5), end='')
    print(chr(flag[15] + 3), end='')
    for i in range(16, 26):
        print(chr(flag[i]), end='')
    print ("")
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Investigative_Reversing_0# python3 solve.py
picoCTF{f0und_1t_fb69f6c2}
```