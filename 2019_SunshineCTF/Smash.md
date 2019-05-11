# Smash
Reversing, 150 points

## Description

> We discovered this old CD from the 90s in our attic. It looks like it used to register you to a WWE betting community back in the day, but we seem to have lost the access code. Can you get us in?

A binary file was attached.

## Solution

Let's run the binary:

```console
root@kali:/media/sf_CTFs/sunshine/Smash# ./WrestleOfMania
WRESTLE-O-MANIA! We bring your wrestling bets to the internet.
All rights reserved, 1991.
Beginning your installation.....

Please enter your access code: test
ERROR: Access code invalid.
```

Looks like we'll have to provide a valid access code in order to proceed.

Let's take a look at the Ghidra decompilation output (function names were included, all other labels and types are user-defined):

```c
undefined4 main(undefined1 param_1)

{
  char *pcVar1;
  uint uVar2;
  undefined4 uVar3;
  char user_key [31];
  int local_14;
  
  puts("WRESTLE-O-MANIA! We bring your wrestling bets to the internet.");
  puts("All rights reserved, 1991.");
  printf("Beginning your installation");
  fflush(stdout);
  local_14 = 0;
  while (local_14 < 5) {
    putchar('.');
    fflush(stdout);
    local_14 = local_14 + 1;
  }
  printf("\n\nPlease enter your access code: ");
  pcVar1 = fgets(user_key,31,stdin);
  if (pcVar1 == (char *)0x0) {
    puts("ERROR: Access code not recognized.");
    uVar3 = 0xffffffff;
  }
  else {
    uVar2 = checkAccessCode(user_key);
    if (uVar2 == 1) {
      puts("Thank you for registering!");
    }
    else {
      puts("ERROR: Access code invalid.");
    }
    uVar3 = 0;
  }
  return uVar3;
}
```

The important logic resides in `checkAccessCode()`, let's inspect it:
```c
uint checkAccessCode(char *user_key)

{
  int iVar1;
  int *p_out_arr;
  int in_arr [30];
  int verify_res;
  
  __x86.get_pc_thunk.ax();
  in_arr[0] = 5;
  in_arr[1] = 3;
  in_arr[2] = 6;
  in_arr[3] = 5;
  in_arr[4] = 2;
  in_arr[5] = 5;
  in_arr[6] = 3;
  in_arr[7] = 3;
  in_arr[8] = 3;
  in_arr[9] = 5;
  in_arr[10] = 2;
  in_arr[11] = 4;
  in_arr[12] = 6;
  in_arr[13] = 5;
  in_arr[14] = 5;
  in_arr[15] = 2;
  in_arr[16] = 2;
  in_arr[17] = 5;
  in_arr[18] = 2;
  in_arr[19] = 6;
  in_arr[20] = 5;
  in_arr[21] = 1;
  in_arr[22] = 3;
  in_arr[23] = 4;
  in_arr[24] = 5;
  in_arr[25] = 3;
  in_arr[26] = 4;
  in_arr[27] = 6;
  in_arr[28] = 6;
  in_arr[29] = 5;
  process(user_key,in_arr,&p_out_arr);
  prepare(user_key);
  verify(user_key);
  format(user_key,&p_out_arr);
  iVar1 = checkResult(p_out_arr);
  return (uint)(iVar1 == 1);
}
```

This function defines an array, then calls a series of functions.

Let's inspect them one by one:

```c
void process(char *user_key,int *in_arr,int **pp_out_arr)

{
  int *piVar1;
  int i;
  
  __x86.get_pc_thunk.ax();
  piVar1 = (int *)malloc(120);
  *pp_out_arr = piVar1;
  i = 0;
  while (i < 30) {
    (*pp_out_arr)[i] = 0;
    i = i + 1;
  }
  i = 0;
  while (i < 30) {
    while (in_arr[i] != 0) {
      (*pp_out_arr)[i] = (*pp_out_arr)[i] + 1;
      in_arr[i] = in_arr[i] + -1;
    }
    i = i + 1;
  }
  return;
}
```

This function allocates `out_arr` (an array of 30 integers) and copies `in_arr` to `out_arr`.

```c
void prepare(char *user_key)

{
  FILE *__s;
  int i;
  
  __s = fopen("/dev/null","w");
  i = 0;
  while( true ) {
    if (29 < i) {
      return;
    }
    if ((int)user_key[i] - 'n'U < 16) break;
    fwrite("there",1,5,__s);
    i = i + 1;
  }
                    /* WARNING: Could not recover jumptable at 0x000108f1. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)((int)&_GLOBAL_OFFSET_TABLE_ + *(int *)(&UNK_00010d94 + ((int)user_key[i] - 'n'U) * 4)))
            ();
  return;
}
```

This looks like a dummy function, it doesn't modify the input or do anything else which seems meaningful. Hopefully we can ignore it.

```c
int verify(char *user_key)

{
  int j;
  uint local_c;
  int i;
  
  __x86.get_pc_thunk.ax();
  i = 0;
  local_c = 5;
  j = 25;
  while (j != 0) {
    if (user_key[i] == '-') {
      local_c = local_c & 3;
    }
    i = i + 1;
    j = j + -1;
  }
  return (uint)(local_c != 92);
}
```

This function performs some kind of verification on the input, but the return value is ignored. Hopefully we can ignore it as well.

```c
void format(char *user_key,int **pp_out_arr)

{
  void *__ptr;
  int i;
  
  __ptr = malloc(120);
  i = 0;
  while (i < 30) {
    (*pp_out_arr)[i] = (int)user_key[i] << ((byte)(*pp_out_arr)[i] & 0x1f);
    i = i + 1;
  }
  free(__ptr);
  return;
}
```

This function formats `out_arr` using the user key - this seems important.

```c
undefined4 checkResult(int *p_out_arr)

{
  int j;
  int *piVar1;
  int *piVar2;
  int expected_arr [30];
  int i;
  
  j = 30;
  piVar1 = g_expected_values;
  piVar2 = expected_arr;
  while (j != 0) {
    j = j + -1;
    *piVar2 = *piVar1;
    piVar1 = piVar1 + 1;
    piVar2 = piVar2 + 1;
  }
  i = 0;
  while( true ) {
    if (29 < i) {
      return 1;
    }
    if (expected_arr[i] != p_out_arr[i]) break;
    i = i + 1;
  }
  return 0;
}
```

Finally, this function checks if the formatted `out_arr` is identical to a global `g_expected_values`:

```
                             g_expected_values                               XREF[2]:     checkResult:000109af(*), 
                                                                                          checkResult:000109be(R)  
        00010de0 60 0e 00 00        int[30]
                 a8 03 00 00 
                 80 1b 00 00 
           00010de0 [0]                    E60h,         3A8h,        1B80h,         F60h
           00010df0 [4]                    120h,         EA0h,         188h,         358h
           00010e00 [8]                    1A0h,         9A0h,         184h,         4E0h
           00010e10 [12]                   C40h,         C20h,         5A0h,         1C8h
           00010e20 [16]                   1D4h,         9C0h,         1CCh,         B40h
           00010e30 [20]                   AE0h,          62h,         360h,         340h
           00010e40 [24]                   5A0h,         180h,         6E0h,         B40h
           00010e50 [28]                  1540h,         FA0h

```

Now that we have the important logic, we can use z3 to find a key that complies with it (even though the logic doesn't seem to hard to reverse manually):

```python
from z3 import *

KEY_LEN = 30
key = [BitVec("{}".format(i), 32) for i in range(KEY_LEN)]
solver = Solver()

for i in range(KEY_LEN):
    solver.add(key[i] >= ord('!'))
    solver.add(key[i] <= ord('~'))

in_arr = [5, 3, 6, 5, 2, 5, 3, 3, 3, 5, 2, 4, 6, 5, 5, 2, 2, 5, 2, 6, 5, 1, 3, 4, 5, 3, 4, 6, 6, 5]

for i in range(len(in_arr)):
    in_arr[i] = key[i] << (in_arr[i] & 0x1f)

expected_arr = [0xE60,         0x3A8,         0x1B80,        0xF60,
                0x120,         0xEA0,         0x188,         0x358,
                0x1A0,         0x9A0,         0x184,         0x4E0,
                0xC40,         0xC20,         0x5A0,         0x1C8,
                0x1D4,         0x9C0,         0x1CC,         0xB40,
                0xAE0,         0x62,          0x360,         0x340,
                0x5A0,         0x180,         0x6E0,         0xB40,
                0x1540,        0xFA0]

for i in range(len(expected_arr)):
    solver.add(expected_arr[i] == in_arr[i])

if solver.check() == sat:
    model = solver.model()
    #print(model)

    res = ""
    for i in range(KEY_LEN):
        res += chr(model[key[i]].as_long())
    print res
```

The output:

```console
root@kali:/media/sf_CTFs/sunshine/Smash# python solve.py
sun{Hu1k4MaN1a-ruNs-W1l4-0n-U}
root@kali:/media/sf_CTFs/sunshine/Smash# python solve.py | ./WrestleOfMania
WRESTLE-O-MANIA! We bring your wrestling bets to the internet.
All rights reserved, 1991.
Beginning your installation.....

Please enter your access code: Thank you for registering!
```

The flag: sun{Hu1k4MaN1a-ruNs-W1l4-0n-U}