# investigation_encoded_2
Forensics, 500 points

## Description:
> We have recovered a binary and 1 file: image01. See what you can make of it. 
> NOTE: The flag is not in the normal picoCTF{XXX} format.

## Solution: 

This is the follow-up challenge for [investigation_encoded_1](investigation_encoded_1.md). The challenges are very similar and therefore we'll only be listing the differences.

We start by running the new binary:

```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_2# ./mystery
Error: file ./flag.txt not found
root@kali:/media/sf_CTFs/pico/investigation_encoded_2# echo abcde>flag.txt
root@kali:/media/sf_CTFs/pico/investigation_encoded_2# ./mystery
Segmentation fault
```

Let's just open it with Ghidra.

The main function is:

```c
undefined8 main(void)
{
  long lVar1;
  size_t sVar2;
  undefined4 local_18;
  int local_14;
  FILE *local_10;
  
  badChars = '\0';
  local_10 = fopen("flag.txt","r");
  if (local_10 == (FILE *)0x0) {
    fwrite("Error: file ./flag.txt not found\n",1,0x21,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  flag_size = 0;
  fseek(local_10,0,2);
  lVar1 = ftell(local_10);
  flag_size = (int)lVar1;
  fseek(local_10,0,0);
  login();
  if (0xfffe < flag_size) {
    fwrite("Error, file bigger than 65535\n",1,0x1e,stderr);
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  flag = malloc((long)flag_size);
  sVar2 = fread(flag,1,(long)flag_size,local_10);
  local_14 = (int)sVar2;
  if (local_14 < 1) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  local_18 = 0;
  flag_index = &local_18;
  output = fopen("output","w");
  buffChar = 0;
  remain = 7;
  fclose(local_10);
  encode();
  fclose(output);
  if (badChars == '\x01') {
    fwrite("Invalid Characters in flag.txt\n./output is corrupted\n",1,0x35,stderr);
  }
  else {
    fwrite("I\'m Done, check file ./output\n",1,0x1e,stderr);
  }
  return 0;
}
```

We proceed to `encode()`:

```c
void encode(void)
{
  byte bVar1;
  ulong uVar2;
  int iVar3;
  int local_10;
  char current_char;
  
  while (*flag_index < flag_size) {
    uVar2 = lower(*(byte *)(*flag_index + flag));
    current_char = (char)uVar2;
    if (current_char == ' ') {
      current_char = -0x7b;
    }
    else {
      if (('/' < current_char) && (current_char < ':')) {
        current_char = current_char + 'K';
      }
    }
    current_char = current_char + -0x61;
    if ((current_char < '\0') || ('$' < current_char)) {
      badChars = 1;
    }
    if (current_char != '$') {
      iVar3 = ((int)current_char + 0x12) % 0x24;
      bVar1 = (byte)(iVar3 >> 0x1f);
      current_char = ((byte)iVar3 ^ bVar1) - bVar1;
    }
    local_10 = *(int *)(indexTable + (long)(int)current_char * 4);
    iVar3 = *(int *)(indexTable + (long)((int)current_char + 1) * 4);
    while (local_10 < iVar3) {
      uVar2 = getValue(local_10);
      save((byte)uVar2);
      local_10 = local_10 + 1;
    }
    *flag_index = *flag_index + 1;
  }
  while (remain != 7) {
    save(0);
  }
  return;
}
```

And to complete the picture, `getValue` and `save`:
```c
ulong getValue(int param_1)
{
  byte bVar1;
  int iVar2;
  
  iVar2 = param_1;
  if (param_1 < 0) {
    iVar2 = param_1 + 7;
  }
  bVar1 = (byte)(param_1 >> 0x37);
  return (ulong)((int)(uint)(byte)secret[iVar2 >> 3] >>
                 (7 - (((char)param_1 + (bVar1 >> 5) & 7) - (bVar1 >> 5)) & 0x1f) & 1);
}

void save(byte param_1)
{
  buffChar = buffChar | param_1;
  if (remain == 0) {
    remain = 7;
    fputc((int)(char)buffChar,output);
    buffChar = '\0';
  }
  else {
    buffChar = buffChar * '\x02';
    remain = remain + -1;
  }
  return;
}
```

Since this is the exact same idea as before, we'll only point out the minor deltas:

* The `matrix` array was replaced with `indexTable`
* The program accepts an additional set of valid characters, and performs some kind of manipulation on them before using them as array indices. 
  * If we consider all characters which don't cause `badChars` to be set, we get lowercase and uppercase letters, digits, space and '`{`', '`|`', '`}`', '`~`'. However, the last four characters get the exact same encoding as the digits `0`, `1`, `2`, `3` and therefore should be considered invalid.

Like last time, we create our `dict.c` program based on the Ghidra decompilation, with the buffers extracted using `radare2`:

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

// Definitions from Ghidra
typedef unsigned char    byte;
typedef unsigned int     uint;
typedef unsigned long    ulong;

//[0x00000ae0]> bf obj.secret
//[0x00000ae0]> pc @ obj.secret
const uint8_t secret[] = {
  0x8b, 0xaa, 0x2e, 0xee, 0xe8, 0xbb, 0xae, 0x8e, 0xbb, 0xae,
  0x3a, 0xee, 0x8e, 0xee, 0xa8, 0xee, 0xae, 0xe3, 0xaa, 0xe3,
  0xae, 0xbb, 0x8b, 0xae, 0xb8, 0xea, 0xae, 0x2e, 0xba, 0x2e,
  0xae, 0x8a, 0xee, 0xa3, 0xab, 0xa3, 0xbb, 0xbb, 0x8b, 0xbb,
  0xb8, 0xae, 0xee, 0x2a, 0xee, 0x2e, 0x2a, 0xb8, 0xaa, 0x8e,
  0xaa, 0x3b, 0xaa, 0x3b, 0xba, 0x8e, 0xa8, 0xeb, 0xa3, 0xa8,
  0xaa, 0x28, 0xbb, 0xb8, 0xae, 0x2a, 0xe2, 0xee, 0x3a, 0xb8,
  0x00
};

//[0x00000ae0]> bf obj.indexTable
//[0x00000ae0]> pc @ obj.indexTable
const uint8_t indexTable[] = {
  0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x12, 0x00,
  0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00,
  0x52, 0x00, 0x00, 0x00, 0x64, 0x00, 0x00, 0x00, 0x78, 0x00,
  0x00, 0x00, 0x8e, 0x00, 0x00, 0x00, 0x9e, 0x00, 0x00, 0x00,
  0xb4, 0x00, 0x00, 0x00, 0xc8, 0x00, 0x00, 0x00, 0xda, 0x00,
  0x00, 0x00, 0xea, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00,
  0x0e, 0x01, 0x00, 0x00, 0x1e, 0x01, 0x00, 0x00, 0x34, 0x01,
  0x00, 0x00, 0x48, 0x01, 0x00, 0x00, 0x5a, 0x01, 0x00, 0x00,
  0x6a, 0x01, 0x00, 0x00, 0x72, 0x01, 0x00, 0x00, 0x80, 0x01,
  0x00, 0x00, 0x8c, 0x01, 0x00, 0x00, 0x9a, 0x01, 0x00, 0x00,
  0xaa, 0x01, 0x00, 0x00, 0xbc, 0x01, 0x00, 0x00, 0xc8, 0x01,
  0x00, 0x00, 0xd6, 0x01, 0x00, 0x00, 0xe0, 0x01, 0x00, 0x00,
  0xea, 0x01, 0x00, 0x00, 0xf0, 0x01, 0x00, 0x00, 0x00, 0x02,
  0x00, 0x00, 0x0a, 0x02, 0x00, 0x00, 0x16, 0x02, 0x00, 0x00,
  0x22, 0x02, 0x00, 0x00, 0x30, 0x02, 0x00, 0x00, 0x34, 0x02,
  0x00, 0x00
};

ulong getValue(int param_1)
{
    byte bVar1;
    int iVar2;

    iVar2 = param_1;
    if (param_1 < 0) {
        iVar2 = param_1 + 7;
    }
    bVar1 = (byte)(param_1 >> 0x37);
    return (ulong)((int)(uint)(byte)secret[iVar2 >> 3] >>
            (7 - (((char)param_1 + (bVar1 >> 5) & 7) - (bVar1 >> 5)) & 0x1f) & 1);
}

void encode(char c)
{
    byte bVar1;
    ulong uVar2;
    int iVar3;
    int local_10;
    char current_char = c;

    if (current_char == ' ') {
        current_char = -0x7b;
    }
    else {
        if (('/' < current_char) && (current_char < ':')) {
            current_char = current_char + 'K';
        }
    }

    current_char = current_char + -0x61;
    if ((current_char < '\0') || ('$' < current_char)) {
        return;
    }

    printf("%c: ", c);

    if (current_char != '$') {
        iVar3 = ((int)current_char + 0x12) % 0x24;
        bVar1 = (byte)(iVar3 >> 0x1f);
        current_char = ((byte)iVar3 ^ bVar1) - bVar1;
    }

    local_10 = *(int *)(indexTable + (long)(int)current_char * 4);
    iVar3 = *(int *)(indexTable + (long)((int)current_char + 1) * 4);

    while (local_10 < iVar3) {
        uVar2 = getValue(local_10);
        printf("%d", uVar2);
        local_10 = local_10 + 1;
    }
    printf("\n");
    
    return;
}

int main(int argc, char* argv[])
{
    char c;

    for (c = 'a'; c <= 'z'; c++)
    {
        encode(c);
    }

    for (c = '0'; c <= '9'; c++)
    {
        encode(c);
    }

    encode(' ');

    return 0;
}
```

Output:
```
root@kali:/media/sf_CTFs/pico/investigation_encoded_2# cc dict.c -o dict && ./dict
dict.c: In function ‘getValue’:
dict.c:51:28: warning: right shift count >= width of type [-Wshift-count-overflow]
     bVar1 = (byte)(param_1 >> 0x37);
                            ^~
a: 101011101110111000
b: 1010101110111000
c: 10111000
d: 10101010111000
e: 101010101000
f: 11101010101000
g: 1110111010101000
h: 111011101110101000
i: 111010101000
j: 11101011101000
k: 1110101000
l: 1010101000
m: 101000
n: 1011101110111000
o: 1010111000
p: 101010111000
q: 101110111000
r: 11101010111000
s: 1000
t: 10111010101000
u: 1011101110111011101000
v: 10111011101011101000
w: 1110101110111010111000
x: 111010111011101000
y: 11101110111010101000
z: 1110111010101110111000
0: 1110101010111000
1: 1110101110101110111000
2: 10111010111010111000
3: 111010101010111000
4: 1011101011101000
5: 101110101011101000
6: 101011101110101000
7: 1110101011101000
8: 1110111011101110111000
9: 10111011101110111000
 : 0000
```

The Python script to decode the message is also similar to last time:

```python
from pwn import *

p = process("./dict")
dict_output = p.recvall().rstrip()

encoding_dict = {}
for line in dict_output.split("\n"):
    line = line.rstrip()
    char, encoding = line.split(": ")
    encoding_dict[encoding] = char

with open("output", "rb") as f:
    data = f.read()
    bin_data = bits_str(data)
    log.info("Binary data:\n{}".format(bin_data))
    res = ""
    while bin_data:
        for k in encoding_dict:
            if bin_data.startswith(k):
                res += encoding_dict[k]
                bin_data = bin_data[len(k):]
                break
        else:
            if (bin_data.rstrip("0") != ""):
                log.error("Can't find decoded value for {}".format(bin_data))
                raise Exception()
            break
    log.success("Output: {}".format(res))
```

Output:
```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_2# python decode.py
[+] Starting local process './dict': pid 2992
[+] Receiving all data: Done (712B)
[*] Process './dict' stopped with exit code 0 (pid 2992)
[*] Binary data:
    101110101010001110101110101110111000101000111010101010111000111010101010001110101110101110111000111010101000111010101010111000101110101011101000111010101011100011101010101110001110101010111000111010101011100011101010101110001110101010111000111010101011100011101010101110001110101010111000111010101011100011101010101110001011101011101000101110101110100011101010101000101110111011101110001110101010111000111010101110100010111010111010111000111010101110100000
[+] Output: t1m3f1i350000000000044f90727
```

The flag: `picoCTF{t1m3f1i350000000000044f90727}`