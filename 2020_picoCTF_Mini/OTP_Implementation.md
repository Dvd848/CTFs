# OTP Implementation
Category: Reverse Engineering

## Description

> Yay reversing!

A binary file and the following text file were attached:

```
1fcb81cd1f6f1e12b429092e3647153b6c212772554ca004145b82367e1e6b7870827dc249a319601776f727434e6b6227d1
```


## Solution

Let's run the attached binary:
```console
root@kali:/media/sf_CTFs/pico/OTP_Implementation# ./otp
USAGE: ./otp [KEY]
root@kali:/media/sf_CTFs/pico/OTP_Implementation# ./otp test
Invalid key!
```

So we need to enter a key of some sort. What if we try the one provided in the text file?

```console
root@kali:/media/sf_CTFs/pico/OTP_Implementation# ./otp 1fcb81cd1f6f1e12b429092e3647153b6c212772554ca004145b82367e1e6b7870827dc249a319601776f727434e6b6227d1
Invalid key!
```

Still no luck. Let's open the file in Ghidra:

```c
undefined8 main(int argc,undefined8 *argv)
{
  byte bVar1;
  int iVar2;
  undefined8 status;
  ulong jumbled_char;
  long in_FS_OFFSET;
  int i;
  int j;
  char user_input [100];
  undefined local_84;
  char jumbled_input [104];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  if (argc < 2) {
    printf("USAGE: %s [KEY]\n",*argv);
    status = 1;
  }
  else {
    strncpy(user_input,(char *)argv[1],100);
    local_84 = 0;
    i = 0;
    while( true ) {
      status = valid_char(user_input[i]);
      if ((int)status == 0) break;
      if (i == 0) {
        jumbled_char = jumble(user_input[0]);
        bVar1 = (byte)((char)jumbled_char >> 7) >> 4;
        jumbled_input[0] = ((char)jumbled_char + bVar1 & 0xf) - bVar1;
      }
      else {
        jumbled_char = jumble(user_input[i]);
        iVar2 = (int)(char)jumbled_char + (int)jumbled_input[i + -1];
        bVar1 = (byte)(iVar2 >> 0x37);
        jumbled_input[i] = ((char)iVar2 + (bVar1 >> 4) & 0xf) - (bVar1 >> 4);
      }
      i = i + 1;
    }
    j = 0;
    while (j < i) {
      jumbled_input[j] = jumbled_input[j] + 'a';
      j = j + 1;
    }
    if (i == 100) {
      iVar2 = strncmp(jumbled_input,
                      "mlaebfkoibhoijfidblechbggcgldicegjbkcmolhdjihgmmieabohpdhjnciacbjjcnpcfaopigkpdfnoaknjlnlaohboimombk"
                      ,100);
      if (iVar2 == 0) {
        puts("You got the key, congrats! Now xor it with the flag!");
        status = 0;
        goto LAB_5555555549ea;
      }
    }
    puts("Invalid key!");
    status = 1;
  }
LAB_5555555549ea:
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return status;
}
```

We can see that we need to input a key of length 100, and that it is jumbled character by character, and eventually compared to a known result. Once we find the input that will be jumbled to match the predefined result, we should XOR it with the text file contents to get the flag.

The program does not accept all possible characters as input, it validates input using the following method:

```c
undefined8 valid_char(char param_1)

{
  undefined8 uVar1;
  
  if ((param_1 < '0') || ('9' < param_1)) {
    if ((param_1 < 'a') || ('f' < param_1)) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
  }
  else {
    uVar1 = 1;
  }
  return uVar1;
}
```

So valid characters are `[0-9a-f]`, which means that we're looking at a hex stream. This makes sense given the instructions to XOR it with another hex stream.

Finally we have the `jumble` method:

```c
ulong jumble(char param_1)

{
  byte bVar1;
  byte local_c;
  
  local_c = param_1;
  if ('`' < param_1) {
    local_c = param_1 + '\t';
  }
  bVar1 = (byte)((char)local_c >> 7) >> 4;
  local_c = ((local_c + bVar1 & 0xf) - bVar1) * '\x02';
  if ('\x0f' < (char)local_c) {
    local_c = local_c + 1;
  }
  return (ulong)local_c;
}
```

It contains bit shifts and transformations and anything you'd expect from a jumble function. We won't go into analyzing it since we don't need to understand it for our solution. Instead, we'll just use a `gdb` script to brute force the correct answer.

We can see that `jumbled_input` is populated character by character. The first character is populated by:
```c
        jumbled_char = jumble(user_input[0]);
        bVar1 = (byte)((char)jumbled_char >> 7) >> 4;
        jumbled_input[0] = ((char)jumbled_char + bVar1 & 0xf) - bVar1;
```

We can see that its value depends solely on the value of the first character in the input. Therefore, we can try all valid input characters until we hit the one that gives us a correct output, i.e. until we get `'m'` as the result.

Subsequent characters are populated by:
```c
        jumbled_char = jumble(user_input[i]);
        iVar2 = (int)(char)jumbled_char + (int)jumbled_input[i + -1];
        bVar1 = (byte)(iVar2 >> 0x37);
        jumbled_input[i] = ((char)iVar2 + (bVar1 >> 4) & 0xf) - (bVar1 >> 4);
```

We can see that the value of the `i`'th character depends on the `i`'th input and the `(i - 1)`'th jumbled character. By induction, we already have identified everything up to `jumbled_input[i - 1]`, so again we can identify `jumbled_input[i]` by brute forcing `user_input[i]`.

The location chosen to inspect the value of `jumbled_input[i]` for our current input of `user_input[i]` is:
```c
   while (j < i) {
      jumbled_input[j] = jumbled_input[j] + 'a';
      j = j + 1;
    }
    if (i == 100) { // <------------------------- Right here
      iVar2 = strncmp(jumbled_input,
                      "mlaebfkoibhoijfidblechbggcgldicegjbkcmolhdjihgmmieabohpdhjnciacbjjcnpcfaopigkpdfnoaknjlnlaohboimombk"
                      ,100);
```

This is after the loop that adds `'a'` to each member of the `jumbled_input` array, bringing it to its final value.

In assembly, this maps to:

```assembly
|     :||   0x00000984      88540590       mov byte [rbp + rax - 0x70], dl ; Store jumbled character to jumbled_input[i] (rax is i)
|     :||   0x00000988      83851cffffff.  add dword [var_e4h], 1
|     :||   ; CODE XREF from main @ 0x968
|     :|`-> 0x0000098f      8b851cffffff   mov eax, dword [var_e4h]
|     :|    0x00000995      3b8518ffffff   cmp eax, dword [n]
|     `===< 0x0000099b      7ccd           jl 0x96a
|      |    0x0000099d      83bd18ffffff.  cmp dword [n], 0x64 ; <------- A breakpoint right here
|      |,=< 0x000009a4      7520           jne 0x9c6
|      ||   0x000009a6      8b8518ffffff   mov eax, dword [n]
```

We'll implement a breakpoint in GDB's Python extension, and call it `Checkpoint`:

```python
class Checkpoint(gdb.Breakpoint):
    def __init__(self, queue, *args):
        super().__init__(*args)
        self.silent = True
        self.queue = queue

    def stop(self):
        index = self.queue.get(timeout = 10)
        a = gdb.parse_and_eval("((char*)($rbp - 0x70))[{}]".format(index))
        self.queue.put(chr(a))
        print(f"\t{a}")
        
        return False
```

Basically, when the breakpoint is hit, it evaluates `((char*)($rbp - 0x70))[i]` and returns the result (by pushing it to a queue).

We can set the breakpoint to our requested location by calling `bp = Checkpoint(queue, '*0x55555555499d')`.

The full script is as follows:

```python
import gdb
from queue import Queue

ALPHABET = '1234567890abcdef'
EXPECTED = 'mlaebfkoibhoijfidblechbggcgldicegjbkcmolhdjihgmmieabohpdhjnciacbjjcnpcfaopigkpdfnoaknjlnlaohboimombk'

class Checkpoint(gdb.Breakpoint):
    def __init__(self, queue, *args):
        super().__init__(*args)
        self.silent = True
        self.queue = queue

    def stop(self):
        index = self.queue.get(timeout = 10)
        a = gdb.parse_and_eval("((char*)($rbp - 0x70))[{}]".format(index))
        self.queue.put(chr(a))
        print(f"\t{a}")
        
        return False

gdb.execute("set disable-randomization on")
gdb.execute("delete")
queue = Queue()
bp = Checkpoint(queue, '*0x55555555499d')

with open("flag.txt") as f:
    flag = f.read()

key = ""

for i in range(len(key), len(EXPECTED)):
    for c in ALPHABET:
        queue.put(i)
        gdb.execute("run {}".format(key + c))
        result = queue.get(timeout = 10)
        if result == EXPECTED[i]:
            key += c
            print(key)
            break

bytes_key = bytes.fromhex(key)
bytes_flag = bytes.fromhex(flag)

xor = bytes(a ^ b for a, b in zip(bytes_flag, bytes_key)).decode("ascii")

print("\n")

print(f"flag: {flag}")
print(f"key : {key}")
print(f"xor : {xor}")
```

What this does is first initialize a queue which will be used to communicate with the breakpoint (since once created, the breakpoint code is called in an async form and the "main" loop can't wait for it to finish its work without some form of synchronization).

Then, the breakpoint is created. Finally, we re-run the program for each index and for each character in the alphabet, until we identify that the current run has resulted in the correct character in the current index.

Finally, we XOR the correct key with the contents of the text file to recover the flag:

```console
root@kali:/media/sf_CTFs/pico/OTP_Implementation# gdb ./otp
gdb-peda$ source solve_gdb.py

# [...]

flag: 1fcb81cd1f6f1e12b429092e3647153b6c212772554ca004145b82367e1e6b7870827dc249a319601776f727434e6b6227d1
key : 6fa2e2a25c3b5869d75c7a5a062a4a51194c451e663fff306668ec42212a341f40cd199d78c72a21481596117a7c5e5217ac
xor : picoCTF{cust0m_jumbl3s_4r3nt_4_g0Od_1d3A_ca692500}
```