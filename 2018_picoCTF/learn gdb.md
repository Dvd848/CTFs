# learn gdb
General Skills, 300 points

## Description:
> Using a debugging tool will be extremely useful on your missions. Can you run this program in gdb and find the flag?

Attached was a binary file.

## Solution:

Let's run the file:
```console
root@kali:/media/sf_CTFs/pico/learn_gdb# ./run
Decrypting the Flag into global variable 'flag_buf'
.....................................
Finished Reading Flag into global variable 'flag_buf'. Exiting.
```

Simple enough. Let's inspect it with a disassembler:
```assembly
root@kali:/media/sf_CTFs/pico/learn_gdb# r2 run
 -- In visual mode press 'c' to toggle the cursor mode. Use tab to navigate
[0x00400690]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x00400690]> afl
0x004005c0    3 26           sym._init
0x004005f0    1 6            sym.imp.putchar
0x00400600    1 6            sym.imp.puts
0x00400610    1 6            sym.imp.__stack_chk_fail
0x00400620    1 6            sym.imp.__libc_start_main
0x00400630    1 6            sym.imp.strtol
0x00400640    1 6            sym.imp.malloc
0x00400650    1 6            sym.imp.setvbuf
0x00400660    1 6            sym.imp.exit
0x00400670    1 6            sym.imp.usleep
0x00400680    1 6            fcn.00400680
0x00400690    1 41           entry0
0x004006c0    4 50   -> 41   sym.deregister_tm_clones
0x00400700    4 58   -> 55   sym.register_tm_clones
0x00400740    3 28           sym.__do_global_dtors_aux
0x00400760    4 38   -> 35   entry1.init
0x00400786   11 323          sym.decrypt_flag
0x004008c9    1 82           sym.main
0x00400920    4 101          sym.__libc_csu_init
0x00400990    1 2            sym.__libc_csu_fini
0x00400994    1 9            sym._fini
[0x00400690]> s sym.main
[0x004008c9]> pdf
            ;-- main:
/ (fcn) sym.main 82
|   sym.main (int argc, char **argv, char **envp);
|           ; var int local_10h @ rbp-0x10
|           ; var int local_4h @ rbp-0x4
|           ; arg int argc @ rdi
|           ; arg char **argv @ rsi
|           ; DATA XREF from entry0 (0x4006ad)
|           0x004008c9      55             push rbp
|           0x004008ca      4889e5         mov rbp, rsp
|           0x004008cd      4883ec10       sub rsp, 0x10
|           0x004008d1      897dfc         mov dword [local_4h], edi   ; argc
|           0x004008d4      488975f0       mov qword [local_10h], rsi  ; argv
|           0x004008d8      488b05f90a20.  mov rax, qword [obj.stdout__GLIBC_2.2.5] ; obj.__TMC_END ; [0x6013d8:8]=0
|           0x004008df      b900000000     mov ecx, 0
|           0x004008e4      ba02000000     mov edx, 2
|           0x004008e9      be00000000     mov esi, 0
|           0x004008ee      4889c7         mov rdi, rax
|           0x004008f1      e85afdffff     call sym.imp.setvbuf        ; int setvbuf(FILE*stream, char *buf, int mode, size_t size)
|           0x004008f6      bfd0094000     mov edi, str.Decrypting_the_Flag_into_global_variable__flag_buf ; 0x4009d0 ; "Decrypting the Flag into global variable 'flag_buf'"
|           0x004008fb      e800fdffff     call sym.imp.puts           ; int puts(const char *s)
|           0x00400900      b800000000     mov eax, 0
|           0x00400905      e87cfeffff     call sym.decrypt_flag
|           0x0040090a      bf080a4000     mov edi, str.Finished_Reading_Flag_into_global_variable__flag_buf_._Exiting. ; 0x400a08 ; "Finished Reading Flag into global variable 'flag_buf'. Exiting."
|           0x0040090f      e8ecfcffff     call sym.imp.puts           ; int puts(const char *s)
|           0x00400914      b800000000     mov eax, 0
|           0x00400919      c9             leave
\           0x0040091a      c3             ret
[0x004008c9]>
```

The program simply decrypts the flag, stores the result in the global variable called `flag_buf` (according to the print, verified by disassembly too) and exits.
Using GDB, we can easily break right after the decryption and inspect the flag.

It's pretty straightforward to perform this directly with GDB, but it seemed like a good chance to practice using the pwntools GDB class:
```python
from pwn import *

gdb_script = """
b decrypt_flag
c
fin
printf "Flag: %s", flag_buf
call printf("%s", flag_buf)
q
"""

p = gdb.debug("./run", gdb_script)
print p.recvall()
p.wait()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/learn_gdb# python solve.py 
[+] Starting local process '/usr/bin/gdbserver': pid 1356
[*] running in new terminal: /usr/bin/gdb -q  "./run" -x "/tmp/pwnPv3xeB.gdb"
[+] Receiving all data: Done (164B)
[*] Process '/usr/bin/gdbserver' stopped with exit code 0 (pid 1360)
Remote debugging from host 127.0.0.1
Decrypting the Flag into global variable 'flag_buf'
.....................................
picoCTF{gDb_iS_sUp3r_u53fuL_a6c61d82}
```

The script launches GDB which in turn launches the program. GDB also receives a script to run:
* Set breakpoint on the "decrypt_flag" function
* run
* When breakpoint is hit, run until end of function (finish decrypting)
* Print the flag to the GDB console
* This one is pretty cool: Call the program's "printf" function, which will result in printing the flag to STDOUT (as if the code itself called `printf`). This is what allows us to read the flag using `print p.recvall()`

The flag: picoCTF{gDb_iS_sUp3r_u53fuL_a6c61d82}