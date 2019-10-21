# investigation_encoded_1
Forensics, 450 points

## Description:
> We have recovered a binary and 1 file: image01. See what you can make of it. 
> NOTE: The flag is not in the normal picoCTF{XXX} format.

## Solution: 

Let's check the files we got:

```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# file mystery
mystery: ELF 64-bit LSB pie executable x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=29b4dba83a1a5a26d76e122ad48d63cff886b075, not stripped
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# file output
output: Non-ISO extended-ASCII text, with no line terminators
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# xxd -g 1 output
00000000: 8e 8e ba 3b b8 ea 23 a8 eb b8 bb 8a e3 a8 eb b8  ...;..#.........
00000010: eb a2 ee e2 a2 a8 ea 80                          ........
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# ./mystery
./flag.txt not found
```

We have a binary output file, and a program which requires `flag.txt`. Let's give it a fake flag and see what happens:
```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# mkdir tmp
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# cp ./mystery tmp
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# cd tmp
root@kali:/media/sf_CTFs/pico/investigation_encoded_1/tmp# echo test>flag.txt
root@kali:/media/sf_CTFs/pico/investigation_encoded_1/tmp# ./mystery
Error, I don't know why I crashed
```

Neither do we. Let's take a look at the Ghidra decompiler output.

The main function is:

```c
undefined8 main(void)
{
  long lVar1;
  size_t sVar2;
  undefined4 local_18;
  int local_14;
  FILE *flag_file;
  
  flag_file = fopen("flag.txt","r");
  if (flag_file == (FILE *)0x0) {
    fwrite("./flag.txt not found\n",1,0x15,stderr);
                    // WARNING: Subroutine does not return
    exit(1);
  }
  flag_size = 0;
  fseek(flag_file,0,2);
  lVar1 = ftell(flag_file);
  flag_size = (int)lVar1;
  fseek(flag_file,0,0);
  if (0xfffe < flag_size) {
    fwrite("Error, file bigger that 65535\n",1,0x1e,stderr);
                    // WARNING: Subroutine does not return
    exit(1);
  }
  flag = malloc((long)flag_size);
  sVar2 = fread(flag,1,(long)flag_size,flag_file);
  local_14 = (int)sVar2;
  if (local_14 < 1) {
                    // WARNING: Subroutine does not return
    exit(0);
  }
  local_18 = 0;
  flag_index = &local_18;
  output = fopen("output","w");
  buffChar = 0;
  remain = 7;
  fclose(flag_file);
  encode();
  fclose(output);
  fwrite("I\'m Done, check ./output\n",1,0x19,stderr);
  return 0;
}
```

It reads the flag, sets some global (`flag_index`, `output`, `buffChar`, `remain`) and calls `encode()`:

```c
void encode(void)
{
  byte current_char;
  int end;
  undefined8 is_valid;
  ulong uVar1;
  int current_index;
  char current_lower;
  
  while( true ) {
    if (flag_size <= *flag_index) {
      while (remain != 7) {
        save(0);
      }
      return;
    }
    current_char = *(byte *)(*flag_index + flag);
    is_valid = isValid(current_char);
    if ((char)is_valid != '\x01') break;
    uVar1 = lower(current_char);
    current_lower = (char)uVar1;
    if (current_lower == ' ') {
      current_lower = '{';
    }
    current_index = *(int *)(matrix + (long)((int)current_lower + -'a') * 8 + 4);
    end = current_index + *(int *)(matrix + (long)((int)current_lower + -'a') * 8);
    while (current_index < end) {
      uVar1 = getValue(current_index);
      save((byte)uVar1);
      current_index = current_index + 1;
    }
    *flag_index = *flag_index + 1;
  }
  fwrite("Error, I don\'t know why I crashed\n",1,0x22,stderr);
                    // WARNING: Subroutine does not return
  exit(1);
}
```

`encode` iterates the flag character by character, and (assuming it's valid) converts it to lowercase, and then reads two DWORDs from `matrix`. The first value looks like a staring index, and the second value looks like a length. Then, for `length` iterations, it reads a value from `getValue()` and calls `save()`.

After all the characters in the flag were processed, the function calls `save(0)` for a number of times and returns.

Let's dive in, starting with `isValid`:
```c
undefined8 isValid(char param_1)

{
  undefined8 uVar1;
  
  if ((param_1 < 'a') || ('z' < param_1)) {
    if ((param_1 < 'A') || ('Z' < param_1)) {
      if (param_1 == ' ') {
        uVar1 = 1;
      }
      else {
        uVar1 = 0;
      }
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}
```

We can see that valid characters are uppercase and lowercase letters, and space. So now we know why our experiment didn't work before: We entered a newline which was rejected. We can repeat our experiment, omitting the newline:

```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_1/tmp# echo -n test>flag.txt
root@kali:/media/sf_CTFs/pico/investigation_encoded_1/tmp# ./mystery
I'm Done, check ./output
root@kali:/media/sf_CTFs/pico/investigation_encoded_1/tmp# xxd -g 1 output
00000000: e2 2a 38                                         .*8
```

This doesn't look like a simple substitution cipher: The output is shorter than the input, and there is no obvious repetition of the `t`. We continue with the decompiler:

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
```

This function performs some bit-manipulation on the input (which is `current_index`) using a `secret` global variable and returns a `0` or `1` (notice the `& 1` at the end of the return statement).

The value returned by this function is then passed on to `save`:

```c
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

This function looks like it buffers the input bits in the `buffChar` global, and writes them to the output file once there are enough bits to form a byte. Now we understand the following code in `encode`:
```c
    if (flag_size <= *flag_index) {
      while (remain != 7) {
        save(0);
      }
      return;
    }
```

When the end of the flag is reached, this code is responsible to pad the output with zeroes and make sure that the output is actually flushed to the output file.

To complete the picture, let's inspect `matrix` and `secret` (this time using radare2 since it allowes dumping `C` arrays):

```
[0x000007c0]> bf obj.secret
[0x000007c0]> pc @ obj.secret
#define _BUFFER_SIZE 37
const uint8_t buffer[_BUFFER_SIZE] = {
  0xb8, 0xea, 0x8e, 0xba, 0x3a, 0x88, 0xae, 0x8e, 0xe8, 0xaa,
  0x28, 0xbb, 0xb8, 0xeb, 0x8b, 0xa8, 0xee, 0x3a, 0x3b, 0xb8,
  0xbb, 0xa3, 0xba, 0xe2, 0xe8, 0xa8, 0xe2, 0xb8, 0xab, 0x8b,
  0xb8, 0xea, 0xe3, 0xae, 0xe3, 0xba, 0x80
};

[0x000007c0]> bf obj.matrix 
[0x000007c0]> pcw @ obj.matrix 
#define _BUFFER_SIZE 54
const uint32_t buffer[_BUFFER_SIZE] = {
  0x00000008U, 0x00000000U, 0x0000000cU, 0x00000008U, 0x0000000eU,
  0x00000014U, 0x0000000aU, 0x00000022U, 0x00000004U, 0x0000002cU,
  0x0000000cU, 0x00000030U, 0x0000000cU, 0x0000003cU, 0x0000000aU,
  0x00000048U, 0x00000006U, 0x00000052U, 0x00000010U, 0x00000058U,
  0x0000000cU, 0x00000068U, 0x0000000cU, 0x00000074U, 0x0000000aU,
  0x00000080U, 0x00000008U, 0x0000008aU, 0x0000000eU, 0x00000092U,
  0x0000000eU, 0x000000a0U, 0x00000010U, 0x000000aeU, 0x0000000aU,
  0x000000beU, 0x00000008U, 0x000000c8U, 0x00000006U, 0x000000d0U,
  0x0000000aU, 0x000000d6U, 0x0000000cU, 0x000000e0U, 0x0000000cU,
  0x000000ecU, 0x0000000eU, 0x000000f8U, 0x00000010U, 0x00000106U,
  0x0000000eU, 0x00000116U, 0x00000004U, 0x00000124U
};

```

We'll run with an example of encoding `b` to understand the algorithm:

```c
    current_char = *(byte *)(*flag_index + flag);   // Let's assume it's 'b'
    is_valid = isValid(current_char);               // It's valid
    if ((char)is_valid != '\x01') break;
    uVar1 = lower(current_char);                    // Returns 'b'
    current_lower = (char)uVar1;                    // Assigns 'b'
    if (current_lower == ' ') {                     // Skipped
      current_lower = '{';
    }

    current_index = *(int *)(matrix + (long)((int)current_lower + -'a') * 8 + 4);
    // current_lower + -'a' = 'b' - 'a' = 1
    // 1 * 8 + 4 = 12
    // current_index = matrix[12] = 0x00000008 (treating matrix as an int array, although it's really a char array with casts)

    end = current_index + *(int *)(matrix + (long)((int)current_lower + -'a') * 8);
    // end = current_index + matrix[8] = current_index + 0x0000000c = 0x00000008 + 0x0000000c = 20

    while (current_index < end) { // Now we'll run from 8 to 19 and call getValue(), encoding 12 bits
      uVar1 = getValue(current_index);
      save((byte)uVar1);
      current_index = current_index + 1;
    }
```


As we can see, `matrix` is composed of 27 pairs of DWORDs - one for each lowercase letter in the ABC + space. The first DWORD in each pair is actually the amount of bits the character will be encoded into (and the second is an offset into `secret` used for the encoding). From a quick look at `matrix`, we can see that each letter is encoded to a bitstream of a different length, explaining why the output we saw was shorter than the input.

Now we can copy the needed logic from the decompilation and create a `C` program which will act as a dictionary:

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

// Definitions from Ghidra
typedef unsigned char    byte;
typedef unsigned int     uint;
typedef unsigned long    ulong;

// r2: pc 216 @ obj.matrix
const uint8_t matrix[] = {
  0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00,
  0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00,
  0x14, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x22, 0x00,
  0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x2c, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x0c, 0x00,
  0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x48, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x52, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00,
  0x0c, 0x00, 0x00, 0x00, 0x68, 0x00, 0x00, 0x00, 0x0c, 0x00,
  0x00, 0x00, 0x74, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0x80, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x8a, 0x00,
  0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x92, 0x00, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x10, 0x00,
  0x00, 0x00, 0xae, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
  0xbe, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0xc8, 0x00,
  0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xd0, 0x00, 0x00, 0x00,
  0x0a, 0x00, 0x00, 0x00, 0xd6, 0x00, 0x00, 0x00, 0x0c, 0x00,
  0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,
  0xec, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0xf8, 0x00,
  0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x06, 0x01, 0x00, 0x00,
  0x0e, 0x00, 0x00, 0x00, 0x16, 0x01, 0x00, 0x00, 0x04, 0x00,
  0x00, 0x00, 0x24, 0x01, 0x00, 0x00
};

// r2: pc 37 @ obj.secret
const uint8_t secret[] = {
  0xb8, 0xea, 0x8e, 0xba, 0x3a, 0x88, 0xae, 0x8e, 0xe8, 0xaa,
  0x28, 0xbb, 0xb8, 0xeb, 0x8b, 0xa8, 0xee, 0x3a, 0x3b, 0xb8,
  0xbb, 0xa3, 0xba, 0xe2, 0xe8, 0xa8, 0xe2, 0xb8, 0xab, 0x8b,
  0xb8, 0xea, 0xe3, 0xae, 0xe3, 0xba, 0x80
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
    int end;
    ulong uVar1;
    int current_index;
    
    printf("%c: ", c);
    
    if (c == ' ')
    {
        c = '{';
    }

    current_index = *(int *)(matrix + (long)((int)c + -'a') * 8 + 4);
    end = current_index + *(int *)(matrix + (long)((int)c + -'a') * 8);
    while (current_index < end) {
        uVar1 = getValue(current_index);
        printf("%d", uVar1);
        current_index = current_index + 1;
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    char c;
    for (c = 'a'; c <= 'z'; c++)
    {
        encode(c);
    }
    encode(' ');
    return 0;
}

```

Let's run it:
```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# gcc dict.c -o dict && ./dict
dict.c: In function ‘getValue’:
dict.c:55:26: warning: right shift count >= width of type [-Wshift-count-overflow]
   bVar1 = (byte)(param_1 >> 0x37);
                          ^~
a: 10111000
b: 111010101000
c: 11101011101000
d: 1110101000
e: 1000
f: 101011101000
g: 111011101000
h: 1010101000
i: 101000
j: 1011101110111000
k: 111010111000
l: 101110101000
m: 1110111000
n: 11101000
o: 11101110111000
p: 10111011101000
q: 1110111010111000
r: 1011101000
s: 10101000
t: 111000
u: 1010111000
v: 101010111000
w: 101110111000
x: 11101010111000
y: 1110101110111000
z: 11101110101000
 : 0000
```

The remaining effort is reduced to a standard task of decoding a [Prefix Code](https://en.wikipedia.org/wiki/Prefix_code), for example via the following script:

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
    log.success("Output: {}".format(res))
```

Output:
```console
root@kali:/media/sf_CTFs/pico/investigation_encoded_1# python decode.py
[+] Starting local process './dict': pid 4540
[+] Receiving all data: Done (404B)
[*] Process './dict' stopped with exit code 0 (pid 4540)
[*] Binary data:
    100011101000111010111010001110111011100011101010001000111010100011101011101110001011101110001010111000111010100011101011101110001110101110100010111011101110001010100010101010001110101010000000
[+] Output: encodedywudycjshb
```

The flag: `picoCTF{encodedywudycjshb}`.