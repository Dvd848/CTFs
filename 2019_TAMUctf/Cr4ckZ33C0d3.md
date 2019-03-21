# Cr4ckZ33C0d3
Reversing, 466 points

## Description:

A binary was attached, as well as a server address.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/Cr4ckZ33C0d3# ./prodkey

Please Enter a product key to continue:
test
Key was too short 5.
```

So we'll have to provide some product key in order to get the flag.

Let's check out the decompiled C source using ghidra:
```c
undefined8 main(void)
{
  ulong uVar1;
  FILE *__stream;
  long in_FS_OFFSET;
  char user_key [32];
  char local_78 [104];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  puts("\nPlease Enter a product key to continue: ");
  fgets(user_key,30,stdin);
  uVar1 = verify_key(user_key);
  if ((char)uVar1 != 0) {
    __stream = fopen("flag.txt","r");
    if (__stream == (FILE *)0x0) {
      puts("Too bad the flag is only on the remote server!");
    }
    else {
      fgets(local_78,100,__stream);
      printf("%s",local_78);
    }
  }
  if (local_10 == *(long *)(in_FS_OFFSET + 0x28)) {
    return 0;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}

ulong verify_key(char *user_key)
{
  uint uVar1;
  size_t key_len;
  size_t sVar2;
  undefined8 uVar3;
  ulong uVar4;
  
  key_len = strlen(user_key);
  if (28 < key_len) {
    uVar3 = check_01((long)user_key);
    if ((((((char)uVar3 == 0) || (uVar3 = check_02((long)user_key), (char)uVar3 == 0)) ||
         (uVar3 = check_03((long)user_key), (char)uVar3 == 0)) ||
        (((uVar4 = check_04((long)user_key), (char)uVar4 == 0 ||
          (uVar4 = check_05((long)user_key), (char)uVar4 == 0)) ||
         ((uVar4 = check_06((long)user_key), (char)uVar4 == 0 ||
          ((uVar4 = check_07((long)user_key), (char)uVar4 == 0 ||
           (uVar4 = check_08((long)user_key), (char)uVar4 == 0)))))))) ||
       ((uVar4 = check_09((long)user_key), (char)uVar4 == 0 ||
        (((((uVar4 = check_0A((long)user_key), (char)uVar4 == 0 ||
            (uVar3 = check_0B((long)user_key), (char)uVar3 == 0)) ||
           (uVar4 = check_0C((long)user_key), (char)uVar4 == 0)) ||
          ((uVar4 = check_0D((long)user_key), (char)uVar4 == 0 ||
           (uVar4 = check_0E((long)user_key), (char)uVar4 == 0)))) ||
         (uVar4 = check_0F(user_key), (char)uVar4 == 0)))))) {
      uVar1 = 0;
    }
    else {
      uVar1 = 1;
    }
    return (ulong)uVar1;
  }
  sVar2 = strlen(user_key);
  printf("Key was too short %d.\n",sVar2);
  return 0;
}
```

So we see that the key needs to be 29 characters long, and has to pass multiple checks in order to be considered valid.

An example check:
```c
undefined8 check_01(long param_1)
{
  undefined8 uVar1;
  
  if ((((*(char *)(param_1 + 5) == '-') && (*(char *)(param_1 + 0xb) == '-')) &&
      (*(char *)(param_1 + 0x11) == '-')) && (*(char *)(param_1 + 0x17) == '-')) {
    uVar1 = 1;
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
```

This checks that there are dashes in the correct places, dividing the key into five segments of 5 characters each.

The following script uses Z3 to satisfy all the constrains and send the key to the server. Each constraint contains the matching decompiled source code as a comment:

```python
from z3 import *
from pwn import *

def get_key():
    KEY_LEN = 29

    key = [BitVec("{}".format(i), 32) for i in range(KEY_LEN)]
    solver = Solver()


    # all values are printable characters excluding space (33 - 126)
    for i in range(KEY_LEN):
        solver.add(key[i] >= ord('!'))
        solver.add(key[i] <= ord('~'))

    # Check_01

        #     ((((*(char *)(param_1 + 5) == '-') && (*(char *)(param_1 + 0xb) == '-')) &&
        #      (*(char *)(param_1 + 0x11) == '-')) && (*(char *)(param_1 + 0x17) == '-'))
    solver.add(key[5] == ord("-"))
    solver.add(key[0xb] == ord("-"))
    solver.add(key[0x11] == ord("-"))
    solver.add(key[0x17] == ord("-"))

    #Check_02

        #  (((((((int)*(char *)(param_1 + 1) - 0x30U < 10) && ((int)*(char *)(param_1 + 4) - 0x30U < 10))
        #     && ((int)*(char *)(param_1 + 6) - 0x30U < 10)) &&
        #    (((int)*(char *)(param_1 + 9) - 0x30U < 10 && ((int)*(char *)(param_1 + 0xf) - 0x30U < 10))))
        #   && (((int)*(char *)(param_1 + 0x12) - 0x30U < 10 &&
        #       (((int)*(char *)(param_1 + 0x16) - 0x30U < 10 &&
        #        ((int)*(char *)(param_1 + 0x1b) - 0x30U < 10)))))) &&
        #  ((int)*(char *)(param_1 + 0x1c) - 0x30U < 10))

    for i in [1, 4, 6, 9, 0xf, 0x12, 0x16, 0x1b, 0x1c]:
        solver.add(key[i] < ord('0') + 10)
        solver.add(key[i] >= ord('0'))

    #Check_03

        #  ((((int)*(char *)(param_1 + 4) + -0x30 == ((int)*(char *)(param_1 + 1) + -0x30) * 2 + 1) &&
        #   (7 < (int)*(char *)(param_1 + 4) + -0x30)) &&
        #  ((int)*(char *)(param_1 + 9) ==
        #   ((int)*(char *)(param_1 + 4) - ((int)*(char *)(param_1 + 1) + -0x30)) + 2))

    solver.add(key[4] - ord('0') == (key[1] - ord('0')) * 2 + 1)
    solver.add(7 < key[4] - ord('0'))
    solver.add(key[9] == key[4] - (key[1] - ord('0')) + 2)

    #Check_04

        # (ulong)(((int)*(char *)(param_1 + 0x1b) + (int)*(char *)(param_1 + 0x1c)) % 0xd == 8)

    solver.add((key[0x1b] + key[0x1c]) % 0xd == 8)

    #Check_05

        # (ulong)(((int)*(char *)(param_1 + 0x1b) + (int)*(char *)(param_1 + 0x16)) % 0x16 == 0x12)

    solver.add((key[0x1b] + key[0x16]) % 0x16 == 0x12)

    #Check_06

        # (ulong)(((int)*(char *)(param_1 + 0x12) + (int)*(char *)(param_1 + 0x16)) % 0xb == 5)

    solver.add((key[0x12] + key[0x16]) % 0xb == 5)

    #Check_07

        #  (ulong)(((int)*(char *)(param_1 + 0x1c) + (int)*(char *)(param_1 + 0x16) +
        #      (int)*(char *)(param_1 + 0x12)) % 0x1a == 4)

    solver.add((key[0x1c] + key[0x16] + key[0x12]) % 0x1a == 4)


    #Check_08

        #  (ulong)(((int)*(char *)(param_1 + 1) +
        #      (int)*(char *)(param_1 + 6) * (int)*(char *)(param_1 + 4)) % 0x29 == 5)

    solver.add((key[1] + key[6] * key[4]) % 0x29 == 5)

    #Check_09

        #   uint uVar1;
        #   int iVar2;
        
        #   iVar2 = (int)*(char *)(param_1 + 0xf) - (int)*(char *)(param_1 + 0x1c);
        #   uVar1 = (uint)(iVar2 >> 0x1f) >> 0x1e;
        #   return (ulong)((iVar2 + uVar1 & 3) - uVar1 == 1)

    #uVar1_09 = BitVec("uVar1_09", 32)
    #iVar2_09 = BitVec("iVar2_09", 32)

    iVar2_09 = key[0xf] - key[0x1c]
    uVar1_09 = (iVar2_09 >> 0x1f) >> 0x1e
    solver.add((iVar2_09 + uVar1_09 & 3) - uVar1_09 == 1)

    #Check_10

        #   uint uVar1;
        #   int iVar2;
        
        #   iVar2 = (int)*(char *)(param_1 + 4) + (int)*(char *)(param_1 + 0x16);
        #   uVar1 = (uint)(iVar2 >> 0x1f) >> 0x1e;
        #   return (ulong)((iVar2 + uVar1 & 3) - uVar1 == 3)

    
    iVar2_10 = key[4] + key[0x16]
    uVar1_10 = (iVar2_10 >> 0x1f) >> 0x1e
    solver.add((iVar2_10 + uVar1_10 & 3) - uVar1_10 == 3)

    #Check_11

        # ((*(char *)(param_1 + 0x14) == 'B') && (*(char *)(param_1 + 0x15) == 'B'))

    solver.add(key[0x14] == ord('B'))
    solver.add(key[0x15] == ord('B'))

    #Check_12

        #  (ulong)(((int)*(char *)(param_1 + 6) +
        #          (int)*(char *)(param_1 + 9) * (int)*(char *)(param_1 + 0xf)) % 10 == 1)

    solver.add((key[6] + key[9] * key[0xf]) % 10 == 1)

    #Check_13

        #   int iVar1;
        #   uint uVar2;
        
        #   iVar1 = (int)*(char *)(param_1 + 0x1b) +
        #           (int)*(char *)(param_1 + 4) + (int)*(char *)(param_1 + 0xf) + -0x12;
        #   uVar2 = (uint)(iVar1 >> 0x1f) >> 0x1c;
        #   return (ulong)((iVar1 + uVar2 & 0xf) - uVar2 == 8)

    iVar1_13 = key[0x1b] + key[4] + key[0xf] - 0x12
    uVar2_13 = (iVar1_13 >> 0x1f) >> 0x1c
    solver.add(iVar1_13 + uVar2_13 & 0xf - uVar2_13 == 8)

    #Check_14

        #   uint uVar1;
        #   int iVar2;
        
        #   iVar2 = (int)*(char *)(param_1 + 0x1c) - (int)*(char *)(param_1 + 9);
        #   uVar1 = (uint)(iVar2 >> 0x1f) >> 0x1f;
        #   return (ulong)((iVar2 + uVar1 & 1) - uVar1 == 1);

    iVar2_14 = key[0x1c] - key[9]
    uVar1_14 = (iVar2_14 >> 0x1f) >> 0x1f
    solver.add((iVar2_14 + uVar1_14 & 1) - uVar1_14 == 1)


    #Check_15

        # (ulong)(*param_1 == 'M')

    solver.add(key[0] == ord('M'))


    if solver.check() == sat:
        model = solver.model()
        #print(model)

        res = ""
        for i in range(KEY_LEN):
            res += chr(model[key[i]].as_long())
        return res

if args["REMOTE"]:
    p = remote("rev.tamuctf.com", 8189)
else:
    p = process("./prodkey")

key = get_key()
log.info("Using key: {}".format(key))

p.sendlineafter("Please Enter a product key to continue:", key)
res =p.recvall()
print (res.decode("ascii"))
```

The output:
```console
root@kali:/media/sf_CTFs/tamu/Cr4ckZ33C0d3# python3 exploit.py REMOTE
[+] Opening connection to rev.tamuctf.com on port 8189: Done
[*] Using key: M4!!9-8!!7@-!!!9!-6!BB2-!!!88
[+] Recieving all data: Done (23B)
[*] Closed connection to rev.tamuctf.com port 8189

gigem{z3_b3st_thr33}
```