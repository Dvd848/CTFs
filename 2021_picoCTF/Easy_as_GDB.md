# Easy as GDB
Category: Reverse Engineering, 160 points

## Description
> The flag has got to be checked somewhere...

A binary file was attached.

## Solution

Let's run the file:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Easy_as_GDB]
└─$ ./brute
input the flag: test
checking solution...
Incorrect.
```

We need to enter a flag candidate, and the program will tell us if it's the correct flag.

Obviously this means that the program needs to somehow compare our flag to the correct one.

Let's check the decompiled output using Ghidra. The main function implementation:

```c
undefined4 main(undefined1 param_1)
{
  char *user_input;
  size_t encoded_flag_len;
  int iVar1;
  
  user_input = (char *)calloc(0x200,1);
  printf("input the flag: ");
  fgets(user_input,0x200,stdin);
  encoded_flag_len = strnlen(&g_encoded_flag,0x200);
  user_input = FUN_0001082b(user_input,encoded_flag_len);
  FUN_000107c2((int)user_input,encoded_flag_len,1);
  iVar1 = check_flag(user_input,encoded_flag_len);
  if (iVar1 == 1) {
    puts("Correct!");
  }
  else {
    puts("Incorrect.");
  }
  return 0;
}
```

We can see that it calls `check_flag` to check the user input against a precalculated encoded flag:

```c
undefined4 check_flag(char *user_input,uint encoded_flag_len)

{
  char *__dest;
  char *__dest_00;
  uint i;
  
  __dest = (char *)calloc(encoded_flag_len + 1,1);
  strncpy(__dest,user_input,encoded_flag_len);
  FUN_000107c2((int)__dest,encoded_flag_len,-1);
  __dest_00 = (char *)calloc(encoded_flag_len + 1,1);
  strncpy(__dest_00,&g_encoded_flag,encoded_flag_len);
  FUN_000107c2((int)__dest_00,encoded_flag_len,-1);
  puts("checking solution...");
  i = 0;
  while( true ) {
    if (encoded_flag_len <= i) {
      return 1;
    }
    if (__dest[i] != __dest_00[i]) break;
    i = i + 1;
  }
  return 0xffffffff;
}
```

`g_encoded_flag` is:
```assembly
                             g_encoded_flag                                  XREF[4]:     check_flag:00010931(*), 
                                                                                          check_flag:00010937(*), 
                                                                                          main:00010a19(*), 
                                                                                          main:00010a1f(*)  
        00012008 7a              ??         7Ah    z
        00012009 2e              ??         2Eh    .
        0001200a 6e              ??         6Eh    n
        0001200b 68              ??         68h    h
        0001200c 1d              ??         1Dh
        0001200d 65              ??         65h    e
        0001200e 16              ??         16h
        0001200f 7c              ??         7Ch    |
        00012010 6d              ??         6Dh    m
        00012011 43              ??         43h    C
        00012012 6f              ??         6Fh    o
        00012013 36              ??         36h    6
        00012014 66              ??         66h    f
        00012015 62              ??         62h    b
        00012016 14              ??         14h
        00012017 47              ??         47h    G
        00012018 43              ??         43h    C
        00012019 37              ??         37h    7
        0001201a 40              ??         40h    @
        0001201b 34              ??         34h    4
        0001201c 58              ??         58h    X
        0001201d 01              ??         01h
        0001201e 58              ??         58h    X
        0001201f 65              ??         65h    e
        00012020 62              ??         62h    b
        00012021 6f              ??         6Fh    o
        00012022 53              ??         53h    S
        00012023 30              ??         30h    0
        00012024 67              ??         67h    g
        00012025 17              ??         17h

```

The program obviously somehow encodes our flag in order to compare it to `g_encoded_flag`. One way to solve the challenge would be to reverse the encoding logic, and find an input that would produce the encoded flag. However, as the challenge title and description state, in this case it would be much easier to try and brute force the flag character by character using a GDB script. 

In `check_flag`, we have the following loop that checks the characters one after the other:

```c
  while( true ) {
    if (encoded_flag_len <= i) {
      return 1;
    }
    if (__dest[i] != __dest_00[i]) break;
    i = i + 1;
  }
  return 0xffffffff;
```

The comparison `(__dest[i] != __dest_00[i])` compares a character of our input (after being encoded) to the expected output. If we get all the characters right, it will return `1`. Otherwise, it will break once it sees an error and return -1.

In assembly, this looks like:

```assembly
                             LAB_56555978                                    XREF[1]:     565559a5(j)  
        56555978 8b 55 f0        MOV        EDX,dword ptr [EBP + local_14]
        5655597b 8b 45 ec        MOV        EAX,dword ptr [EBP + i]
        5655597e 01 d0           ADD        EAX,EDX
        56555980 0f b6 10        MOVZX      EDX,byte ptr [EAX]
        56555983 8b 4d f4        MOV        ECX,dword ptr [EBP + local_10]
        56555986 8b 45 ec        MOV        EAX,dword ptr [EBP + i]
        56555989 01 c8           ADD        EAX,ECX
        5655598b 0f b6 00        MOVZX      EAX,byte ptr [EAX]
        5655598e 38 c2           CMP        DL,AL
        56555990 74 09           JZ         LAB_5655599b
        56555992 c7 45 e8        MOV        dword ptr [EBP + local_1c],0xffffffff
                 ff ff ff ff
        56555999 eb 0c           JMP        LAB_565559a7
                             LAB_5655599b                                    XREF[1]:     56555990(j)  
        5655599b 83 45 ec 01     ADD        dword ptr [EBP + i],0x1
                             LAB_5655599f                                    XREF[1]:     56555976(j)  
        5655599f 8b 45 ec        MOV        EAX,dword ptr [EBP + i]
        565559a2 3b 45 0c        CMP        EAX,dword ptr [EBP + encoded_flag_len]
        565559a5 72 d1           JC         LAB_56555978
```

There are several ways to brute force the flag with GDB. We'll set a breakpoint at `0x5655598e` and check if `AL` is equal to `DL`. If so - the current character got encoded to the correct character, and we can move on to brute force the next character.

Here's our GDB script:

```python
import gdb
import string
from queue import Queue, Empty


MAX_FLAG_LEN = 0x200

class Checkpoint(gdb.Breakpoint):
    def __init__(self, queue, target_hitcount, *args):
        super().__init__(*args)
        self.silent = True
        self.queue = queue
        self.target_hitcount = target_hitcount
        self.hit = 0

    def stop(self):
        res = []
        self.hit += 1
        #print(f"\nhit {self.hit}/{self.target_hitcount}")
        if self.hit == self.target_hitcount:
            al = gdb.parse_and_eval("$al")
            dl = gdb.parse_and_eval("$dl")
            self.queue.put(al == dl)
        return False

class Solvepoint(gdb.Breakpoint):
    def __init__(self, *args):
        super().__init__(*args)
        self.silent = True
        self.hit = 0

    def stop(self):
        #gdb.execute("q")
        self.hit += 1
        return False


gdb.execute("set disable-randomization on")
gdb.execute("delete")
sp = Solvepoint("*0x56555a71")
queue = Queue()


flag = ""
ALPHABET = string.ascii_letters + string.digits + "{}_"

for i in range(len(flag), MAX_FLAG_LEN):
    for c in ALPHABET:
        bp = Checkpoint(queue, len(flag) + 1, '*0x5655598e')
        gdb.execute("run <<< {}{}".format(flag, c))
        try:
            result = queue.get(timeout = 1)
            bp.delete()
            if result:
                flag += c
                print("\n\n{}\n\n".format(flag))
                break
        except Empty:
            print("Error: Empty queue!")
            gdb.execute("q")

    if sp.hit > 0:
        print("Found flag: {}".format(flag))
        gdb.execute("q")
```

This script implements a `Checkpoint`, which is a custom subclass of a breakpoint that performs some logic only after the breakpoint has been hit a certain amount of times (similar to a conditional breakpoint). We need this since in order to check character #i, we will need to ignore all the times that our breakpoint was hit for characters (0..(i-1)).

When the breakpoint is hit for the requested amount of times, we read `AL` and `DL` to understand if our guess was correct.

We also create an additional breakpoint at the following location to tell us if we got the flag correctly:

```assembly
        56555a71 50              PUSH       encoded_flag_len=>s_Correct!_56555b76            = "Correct!"
        56555a72 e8 a9 fa        CALL       puts                                             int puts(char * __s)
                 ff ff

```

If this breakpoint is hit once, we know that we found the flag.

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Easy_as_GDB]
└─$ gdb -n -q -ex "set pagination off" -ex "source solve.py" ./brute
...
[Inferior 1 (process 4074) exited normally]
Breakpoint 1191 at 0x5655598e
input the flag: checking solution...
Incorrect.
[Inferior 1 (process 4075) exited normally]
Breakpoint 1192 at 0x5655598e
input the flag: checking solution...
Incorrect.
[Inferior 1 (process 4076) exited normally]
Breakpoint 1193 at 0x5655598e
input the flag: checking solution...
Incorrect.
[Inferior 1 (process 4077) exited normally]
Breakpoint 1194 at 0x5655598e
input the flag: checking solution...
Incorrect.
[Inferior 1 (process 4078) exited normally]
Breakpoint 1195 at 0x5655598e
input the flag: checking solution...
Incorrect.
[Inferior 1 (process 4079) exited normally]
Breakpoint 1196 at 0x5655598e
input the flag: checking solution...
Correct!
[Inferior 1 (process 4080) exited normally]


picoCTF{I_5D3_A11DA7_0db137a9}


Found flag: picoCTF{I_5D3_A11DA7_0db137a9}
```

The flag: `picoCTF{I_5D3_A11DA7_0db137a9}`