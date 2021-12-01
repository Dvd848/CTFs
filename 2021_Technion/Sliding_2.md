# Sliding 2
Category: Reversing & Binary Exploitation, 500 points

## Description

> This challenge is so easy my cousin can solve it in a few seconds!

A binary file was attached.

## Solution

Let's run the binary file:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Sliding_2]
└─$ ./sliding_2
Enter key: test
Invalid Key
```

So we need to provide a key. Let's open the executable with Ghidra and check the decompilation output.

This is the main function:

```c
undefined8 main(void)
{
  uint int_input;
  size_t input_len;
  undefined8 ret;
  long in_FS_OFFSET;
  int i;
  char user_input [312];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  printf("Enter key: ");
  __isoc99_scanf("%300s",user_input);
  input_len = strnlen(user_input,300);
  i = 0;
  do {
    if ((int)input_len <= i) {
      check_input();
      ret = 0;
LAB_5555555559a7:
      if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
        __stack_chk_fail();
      }
      return ret;
    }
    int_input = char_to_int_05(user_input[i]);
    if ((int)int_input < 0) {
      printf("Invalid Key");
      ret = 1;
      goto LAB_5555555559a7;
    }
    handle_input(int_input);
    i = i + 1;
  } while( true );
}
```

We can see that it reads an input of up to 300 characters. For each character, it calls `char_to_int_05`:

```c
int char_to_int_05(char param_1)
{
  int iVar1;
  
  iVar1 = param_1 + -0x30;
  if ((iVar1 < 0) || (5 < iVar1)) {
    iVar1 = -1;
  }
  return iVar1;
}
```

This function reveals that the expected input is composed of digits 0-5. It simply converts the ASCII digits to integers. After the conversion, each integer is handled using some complex logic:

```c
void handle_input(uint param_1)
{
  undefined uVar1;
  long lVar2;
  uint uVar3;
  long in_FS_OFFSET;
  int local_4c;
  int local_48;
  undefined *local_40;
  undefined *local_38;
  undefined *local_30;
  undefined *local_28;
  undefined *local_20;
  
  lVar2 = *(long *)(in_FS_OFFSET + 0x28);
  uVar3 = param_1 * 2 & 2;
  if ((int)param_1 < 2) {
    local_38 = &DAT_555555558010;
    local_30 = &DAT_555555558030;
    local_28 = &DAT_555555558020;
    local_20 = &DAT_555555558040;
    if ((param_1 & 1) == 0) {
      local_40 = &DAT_555555558050;
    }
    else {
      local_40 = &DAT_555555558060;
    }
  }
  else {
    if ((int)param_1 < 4) {
      local_38 = &DAT_555555558010;
      local_30 = &DAT_555555558050;
      local_28 = &DAT_555555558020;
      local_20 = &DAT_555555558060;
      if ((param_1 & 1) == 0) {
        local_40 = &DAT_555555558040;
      }
      else {
        local_40 = &DAT_555555558030;
      }
    }
    else {
      local_38 = &DAT_555555558050;
      local_30 = &DAT_555555558030;
      local_28 = &DAT_555555558060;
      local_20 = &DAT_555555558040;
      if ((param_1 & 1) == 0) {
        local_40 = &DAT_555555558020;
      }
      else {
        local_40 = &DAT_555555558010;
      }
    }
  }
  for (local_4c = 0; local_4c < 3; local_4c = local_4c + 1) {
    if ((int)param_1 < 2) {
      uVar1 = local_38[(long)local_4c + (long)(int)uVar3 * 3];
      local_38[(long)local_4c + (long)(int)uVar3 * 3] =
           local_30[(long)local_4c + (long)(int)uVar3 * 3];
      local_30[(long)local_4c + (long)(int)uVar3 * 3] =
           local_28[(long)local_4c + (long)(int)uVar3 * 3];
      local_28[(long)local_4c + (long)(int)uVar3 * 3] =
           local_20[(long)local_4c + (long)(int)uVar3 * 3];
      local_20[(long)local_4c + (long)(int)uVar3 * 3] = uVar1;
    }
    else {
      if ((int)param_1 < 4) {
        uVar1 = local_38[(long)(int)uVar3 + (long)local_4c * 3];
        local_38[(long)(int)uVar3 + (long)local_4c * 3] =
             local_30[(long)(int)uVar3 + (long)local_4c * 3];
        local_30[(long)(int)uVar3 + (long)local_4c * 3] =
             local_28[(long)(int)(2 - uVar3) + (long)(2 - local_4c) * 3];
        local_28[(long)(int)(2 - uVar3) + (long)(2 - local_4c) * 3] =
             local_20[(long)(int)uVar3 + (long)local_4c * 3];
        local_20[(long)(int)uVar3 + (long)local_4c * 3] = uVar1;
      }
      else {
        uVar1 = local_38[(long)local_4c + (long)(int)uVar3 * 3];
        local_38[(long)local_4c + (long)(int)uVar3 * 3] =
             local_30[(long)(int)(2 - uVar3) + (long)local_4c * 3];
        local_30[(long)(int)(2 - uVar3) + (long)local_4c * 3] =
             local_28[(long)(2 - local_4c) + (long)(int)(2 - uVar3) * 3];
        local_28[(long)(2 - local_4c) + (long)(int)(2 - uVar3) * 3] =
             local_20[(long)(int)uVar3 + (long)(2 - local_4c) * 3];
        local_20[(long)(int)uVar3 + (long)(2 - local_4c) * 3] = uVar1;
      }
    }
  }
  for (local_48 = 0; local_48 < 2; local_48 = local_48 + 1) {
    if ((param_1 & 1) == 0) {
      uVar1 = local_40[local_48];
      local_40[local_48] = local_40[(long)(2 - local_48) * 3];
      local_40[(long)(2 - local_48) * 3] = local_40[(long)(2 - local_48) + 6];
      local_40[(long)(2 - local_48) + 6] = local_40[(long)local_48 * 3 + 2];
      local_40[(long)local_48 * 3 + 2] = uVar1;
    }
    else {
      uVar1 = local_40[local_48];
      local_40[local_48] = local_40[(long)local_48 * 3 + 2];
      local_40[(long)local_48 * 3 + 2] = local_40[(long)(2 - local_48) + 6];
      local_40[(long)(2 - local_48) + 6] = local_40[(long)(2 - local_48) * 3];
      local_40[(long)(2 - local_48) * 3] = uVar1;
    }
  }
  if (lVar2 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}
```

Finally, the computed output is verified:

```c
void check_input(void)

{
  int i;
  int j;
  int k;
  
  i = 0;
  do {
    if (5 < i) {
      printf("Congratulation, The flag is %s","cstechnion{################}");
      return;
    }
    for (j = 0; j < 3; j = j + 1) {
      for (k = 0; k < 3; k = k + 1) {
        if (*(&PTR_DAT_555555558080)[i] != (&PTR_DAT_555555558080)[i][(long)k + (long)j * 3]) {
          printf("Invalid Key");
          return;
        }
      }
    }
    i = i + 1;
  } while( true );
}
```

So what does it do?

We can start by trying to understand what `check_input` actually checks. We can see a triple loop - the external loop runs `i` from `0` to `5`. What about `k` and `j`?

```python
>>> [k + j *3 for j in range(3) for k in range(3)]
[0, 1, 2, 3, 4, 5, 6, 7, 8]
```

So that's just a fancy way to iterate nine array indices. Essentially, the loop is testing if every index of the `PTR_DAT_555555558080[i]` 2D array is equal to `PTR_DAT_555555558080[i][0]`.

What is `PTR_DAT_555555558080`?

```assembly
                             PTR_DAT_555555558080                            XREF[4]:     check_input:555555555a11(*), 
                                                                                          check_input:555555555a18(R), 
                                                                                          check_input:555555555a41(*), 
                                                                                          check_input:555555555a48(R)  
    555555558080 50 80 55        addr       DAT_555555558050                                 = 03h
                 55 55 55 
                 00 00
    555555558088 60 80 55        addr       DAT_555555558060                                 = 03h
                 55 55 55 
                 00 00
    555555558090 40 80 55        addr       DAT_555555558040                                 = 02h
                 55 55 55 
                 00 00
    555555558098 30 80 55        addr       DAT_555555558030                                 = 04h
                 55 55 55 
                 00 00
    5555555580a0 10 80 55        addr       DAT_555555558010                                 = 06h
                 55 55 55 
                 00 00
    5555555580a8 20 80 55        addr       DAT_555555558020                                 = 04h
                 55 55 55 
                 00 00
```

It looks like an array of pointers, each pointer to some other array:

```assembly
                             DAT_555555558010                                XREF[7]:     handle_input:555555555243(*), 
                                                                                          handle_input:55555555524a(*), 
                                                                                          handle_input:55555555529f(*), 
                                                                                          handle_input:5555555552a6(*), 
                                                                                          handle_input:555555555325(*), 
                                                                                          handle_input:55555555532c(*), 
                                                                                          5555555580a0(*)  
    555555558010 06              ??         06h
    555555558011 02              ??         02h
    555555558012 02              ??         02h
    555555558013 04              ??         04h
    555555558014 03              ??         03h
    555555558015 02              ??         02h
    555555558016 05              ??         05h
    555555558017 04              ??         04h
    555555558018 04              ??         04h
    555555558019 00              ??         00h
    55555555801a 00              ??         00h
    55555555801b 00              ??         00h
    55555555801c 00              ??         00h
    55555555801d 00              ??         00h
    55555555801e 00              ??         00h
    55555555801f 00              ??         00h
                             DAT_555555558020                                XREF[7]:     handle_input:555555555259(*), 
                                                                                          handle_input:555555555260(*), 
                                                                                          handle_input:5555555552b5(*), 
                                                                                          handle_input:5555555552bc(*), 
                                                                                          handle_input:555555555332(*), 
                                                                                          handle_input:555555555339(*), 
                                                                                          5555555580a8(*)  
    555555558020 04              ??         04h
    555555558021 06              ??         06h
    555555558022 01              ??         01h
    555555558023 04              ??         04h
    555555558024 04              ??         04h
    555555558025 03              ??         03h
    555555558026 03              ??         03h
    555555558027 03              ??         03h
    555555558028 03              ??         03h
    555555558029 00              ??         00h
    55555555802a 00              ??         00h
    55555555802b 00              ??         00h
    55555555802c 00              ??         00h
    55555555802d 00              ??         00h
    55555555802e 00              ??         00h
    55555555802f 00              ??         00h
                             DAT_555555558030                                XREF[7]:     handle_input:55555555524e(*), 
                                                                                          handle_input:555555555255(*), 
                                                                                          handle_input:5555555552d5(*), 
                                                                                          handle_input:5555555552dc(*), 
                                                                                          handle_input:5555555552fa(*), 
                                                                                          handle_input:555555555301(*), 
                                                                                          555555558098(*)  
    555555558030 04              ??         04h
    555555558031 04              ??         04h
    555555558032 01              ??         01h
    555555558033 03              ??         03h
    555555558034 05              ??         05h
    555555558035 05              ??         05h
    555555558036 05              ??         05h
    555555558037 06              ??         06h
    555555558038 06              ??         06h
    555555558039 00              ??         00h
    55555555803a 00              ??         00h
    55555555803b 00              ??         00h
    55555555803c 00              ??         00h
    55555555803d 00              ??         00h
    55555555803e 00              ??         00h
    55555555803f 00              ??         00h
                             DAT_555555558040                                XREF[7]:     handle_input:555555555264(*), 
                                                                                          handle_input:55555555526b(*), 
                                                                                          handle_input:5555555552e2(*), 
                                                                                          handle_input:5555555552e9(*), 
                                                                                          handle_input:555555555310(*), 
                                                                                          handle_input:555555555317(*), 
                                                                                          555555558090(*)  
    555555558040 02              ??         02h
    555555558041 02              ??         02h
    555555558042 05              ??         05h
    555555558043 01              ??         01h
    555555558044 02              ??         02h
    555555558045 01              ??         01h
    555555558046 06              ??         06h
    555555558047 01              ??         01h
    555555558048 01              ??         01h
    555555558049 00              ??         00h
    55555555804a 00              ??         00h
    55555555804b 00              ??         00h
    55555555804c 00              ??         00h
    55555555804d 00              ??         00h
    55555555804e 00              ??         00h
    55555555804f 00              ??         00h
                             DAT_555555558050                                XREF[7]:     handle_input:555555555289(*), 
                                                                                          handle_input:555555555290(*), 
                                                                                          handle_input:5555555552aa(*), 
                                                                                          handle_input:5555555552b1(*), 
                                                                                          handle_input:5555555552ef(*), 
                                                                                          handle_input:5555555552f6(*), 
                                                                                          555555558080(*)  
    555555558050 03              ??         03h
    555555558051 05              ??         05h
    555555558052 02              ??         02h
    555555558053 06              ??         06h
    555555558054 01              ??         01h
    555555558055 06              ??         06h
    555555558056 04              ??         04h
    555555558057 01              ??         01h
    555555558058 06              ??         06h
    555555558059 00              ??         00h
    55555555805a 00              ??         00h
    55555555805b 00              ??         00h
    55555555805c 00              ??         00h
    55555555805d 00              ??         00h
    55555555805e 00              ??         00h
    55555555805f 00              ??         00h
                             DAT_555555558060                                XREF[15]:    handle_input:555555555279(*), 
                                                                                          handle_input:555555555280(*), 
                                                                                          handle_input:5555555552c0(*), 
                                                                                          handle_input:5555555552c7(*), 
                                                                                          handle_input:555555555305(*), 
                                                                                          handle_input:55555555530c(*), 
                                                                                          handle_input:55555555571c(R), 
                                                                                          handle_input:55555555573e(R), 
                                                                                          handle_input:55555555574a(W), 
                                                                                          handle_input:55555555577d(W), 
                                                                                          handle_input:5555555557df(R), 
                                                                                          handle_input:55555555580c(W), 
                                                                                          handle_input:555555555863(R), 
                                                                                          handle_input:555555555891(W), 
                                                                                          555555558088(*)  
    555555558060 03              ??         03h
    555555558061 02              ??         02h
                             DAT_555555558062                                XREF[4]:     handle_input:5555555557a3(R), 
                                                                                          handle_input:5555555557cf(W), 
                                                                                          handle_input:555555555800(R), 
                                                                                          handle_input:55555555583c(W)  
    555555558062 01              ??         01h
    555555558063 05              ??         05h
    555555558064 06              ??         06h
    555555558065 03              ??         03h
                             DAT_555555558066                                XREF[4]:     handle_input:555555555765(R), 
                                                                                          handle_input:5555555557ad(W), 
                                                                                          handle_input:55555555581d(R), 
                                                                                          handle_input:55555555586d(W)  
    555555558066 02              ??         02h
    555555558067 05              ??         05h
    555555558068 05              ??         05h
    555555558069 00              ??         00h
    55555555806a 00              ??         00h
    55555555806b 00              ??         00h
    55555555806c 00              ??         00h
    55555555806d 00              ??         00h
    55555555806e 00              ??         00h
    55555555806f 00              ??         00h

```

In these arrays, we find numbers between `1` and `6` (padded with zeroes for alignment?).

If we take a look back at `handle_input`, we'll see that given a byte of user input (a number between `0` and `5`), the program swaps and shifts some elements of the arrays around. To get a better idea of what's happening, we can try to "flatten" the logic by removing the pointer level and use the following script to see what actually happens for each input:

```python
import sys

param_1 = int(sys.argv[1])
uVar3 = param_1 * 2 & 2

if (param_1 < 2):
    local_38 = "DAT_00104010"
    local_30 = "DAT_00104030"
    local_28 = "DAT_00104020"
    local_20 = "DAT_00104040"
    if ((param_1 & 1) == 0):
        local_40 = "DAT_00104050"
    else:
        local_40 = "DAT_00104060"

else:
    if (param_1 < 4):
        local_38 = "DAT_00104010"
        local_30 = "DAT_00104050"
        local_28 = "DAT_00104020"
        local_20 = "DAT_00104060"
        if ((param_1 & 1) == 0):
            local_40 = "DAT_00104040"
      
        else:
            local_40 = "DAT_00104030"
    
    else:
        local_38 = "DAT_00104050"
        local_30 = "DAT_00104030"
        local_28 = "DAT_00104060"
        local_20 = "DAT_00104040"
        if ((param_1 & 1) == 0):
            local_40 = "DAT_00104020"
      
        else:
            local_40 = "DAT_00104010"



for local_4c in range(0, 3):
    if (param_1 < 2): # 0, 1
        print(f"uVar1 = {local_38}[{local_4c + uVar3 * 3}];")
        print(f"{local_38}[{local_4c + uVar3 * 3}] = {local_30}[{local_4c + uVar3 * 3}];")
        print(f"{local_30}[{local_4c + uVar3 * 3}] = {local_28}[{local_4c + uVar3 * 3}];")
        print(f"{local_28}[{local_4c + uVar3 * 3}] = {local_20}[{local_4c + uVar3 * 3}];")
        print(f"{local_20}[{local_4c + uVar3 * 3}] = uVar1;")
        
      
    else:
        if (param_1 < 4): # 2, 3
            print(f"uVar1 = {local_38}[{uVar3 + local_4c * 3}];")
            print(f"{local_38}[{uVar3 + local_4c * 3}] = {local_30}[{uVar3 + local_4c * 3}];")
            print(f"{local_30}[{uVar3 + local_4c * 3}] = {local_28}[{(2 - uVar3) + (2 - local_4c) * 3}];")
            print(f"{local_28}[{(2 - uVar3) + (2 - local_4c) * 3}] = {local_20}[{uVar3 + local_4c * 3}];")
            print(f"{local_20}[{uVar3 + local_4c * 3}] = uVar1;")
         
        else: # 4, 5
            print(f"uVar1 = {local_38}[{local_4c + uVar3 * 3}];")
            print(f"{local_38}[{local_4c + uVar3 * 3}] = {local_30}[{(2 - uVar3) + local_4c * 3}];")
            print(f"{local_30}[{(2 - uVar3) + local_4c * 3}] = {local_28}[{(2 - local_4c) + (2 - uVar3) * 3}];")
            print(f"{local_28}[{(2 - local_4c) + (2 - uVar3) * 3}] = {local_20}[{uVar3 + (2 - local_4c) * 3}];")
            print(f"{local_20}[{uVar3 + (2 - local_4c) * 3}] = uVar1;")
    print("\n")
         
print("\n\n")
   
for local_48 in range(2):
    if ((param_1 & 1) == 0): # 0, 2, 4
        print(f"uVar1 = {local_40}[{local_48}];")
        print(f"{local_40}[{local_48}] = {local_40}[{(2 - local_48) * 3}];")
        print(f"{local_40}[{(2 - local_48) * 3}] = {local_40}[{(2 - local_48) + 6}];")
        print(f"{local_40}[{(2 - local_48) + 6}] = {local_40}[{local_48 * 3 + 2}];")
        print(f"{local_40}[{local_48 * 3 + 2}] = uVar1;")
  
    else: # 1, 3, 5
        print(f"uVar1 = {local_40}[{local_48}];")
        print(f"{local_40}[{local_48}] = {local_40}[{local_48 * 3 + 2}];")
        print(f"{local_40}[{local_48 * 3 + 2}] = {local_40}[{(2 - local_48) + 6}];")
        print(f"{local_40}[{(2 - local_48) + 6}] = {local_40}[{(2 - local_48) * 3}];")
        print(f"{local_40}[{(2 - local_48) * 3}] = uVar1;")
    print("\n")
```

For example, if we input `0`, this is what will actually happen:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Sliding_2]
└─$ python3 ./handle_input.py 0
uVar1 = DAT_00104010[0];
DAT_00104010[0] = DAT_00104030[0];
DAT_00104030[0] = DAT_00104020[0];
DAT_00104020[0] = DAT_00104040[0];
DAT_00104040[0] = uVar1;

uVar1 = DAT_00104010[1];
DAT_00104010[1] = DAT_00104030[1];
DAT_00104030[1] = DAT_00104020[1];
DAT_00104020[1] = DAT_00104040[1];
DAT_00104040[1] = uVar1;

uVar1 = DAT_00104010[2];
DAT_00104010[2] = DAT_00104030[2];
DAT_00104030[2] = DAT_00104020[2];
DAT_00104020[2] = DAT_00104040[2];
DAT_00104040[2] = uVar1;


uVar1 = DAT_00104050[0];
DAT_00104050[0] = DAT_00104050[6];
DAT_00104050[6] = DAT_00104050[8];
DAT_00104050[8] = DAT_00104050[2];
DAT_00104050[2] = uVar1;

uVar1 = DAT_00104050[1];
DAT_00104050[1] = DAT_00104050[3];
DAT_00104050[3] = DAT_00104050[7];
DAT_00104050[7] = DAT_00104050[5];
DAT_00104050[5] = uVar1;
```

There is some circular movement between elements from arrays `DAT_00104010`, `DAT_00104030`, `DAT_00104020` and `DAT_00104040`, together with some in-place scrambling of `DAT_00104050`. Different inputs produce a similar pattern, with the indices and the arrays themselves changing roles.

Putting together everything that we've discovered, this all looks very much like a Rubik's Cube!

 * The six arrays are the six faces of the cube
 * Each face contains nine slots with nine colors
 * This cube supports six different operations 
   * Each operation shifts three colors from face to face and changed the order of the colors within another face
 * The puzzle is solved when each of the faces contains just one color

To solve the puzzle, we can use a Rubik's Cube solver such as [kociemba](https://github.com/muodov/kociemba), but we first have to create some mapping between the colors, faces and operations of both programs.

Let's name the different faces based on the LSB of the matching array. So `DAT_00104010` becomes `10`, and we also have `20`, `30`, `40`, `50` and `60`.

From the output of `handle_input` we can learn about the relationship between all of the faces. For example, from the output we saw earlier, we know that `10` is next to `30` from one side, and `40` from the other side, since colors move from `10` to `30` and from `40` to `10`. Using the same logic, we know that `20` is connected to `40` and `30`, and we have our four candidates for the left, right, front and back faces. This leaves `50` and `60` as candidates for the top and bottom faces. By inspecting the relationship between the faces from additional operations, we arrive to the following layout:

```
   50
40 10 30 20
   60
```

The next step is to identify the mapping between the input operations (0-5) and the [standard notations](https://ruwix.com/the-rubiks-cube/notation/). Again, from following the logic for each operation based on `handle_input`, we see that:

```
0 -> U
1 -> D'
2 -> L
3 -> R'
4 -> B
5 -> F'
```

Note that a single letter by itself refers to a **clockwise** face rotation in 90 degrees (quarter turn), while a letter followed by an apostrophe means to turn that face **counterclockwise** 90 degrees. Since `kociemba` doesn't limit itself to just 6 operations but instead uses all 12, we must translate the output of `kociemba` to the input for the challenge by performing three consecutive turns when a move we need is missing. For example, if `kociemba` instructs us to perform `D`, we can arrive to the same state by performing `D'` three times instead.

The following script performs solves the cube for our specific state:

```python
import kociemba
import itertools

# kociemba accepts the input in the order of {U, R, F, D, L, B}

faces = [
            [0x03, 0x05, 0x02,   0x06, 0x01, 0x06,   0x04, 0x01, 0x06], # 5 | 50 | U
            [0x04, 0x04, 0x01,   0x03, 0x05, 0x05,   0x05, 0x06, 0x06], # 3 | 30 | R
            [0x06, 0x02, 0x02,   0x04, 0x03, 0x02,   0x05, 0x04, 0x04], # 1 | 10 | F
            [0x03, 0x02, 0x01,   0x05, 0x06, 0x03,   0x02, 0x05, 0x05], # 6 | 60 | D
            [0x02, 0x02, 0x05,   0x01, 0x02, 0x01,   0x06, 0x01, 0x01], # 4 | 40 | L
            [0x04, 0x06, 0x01,   0x04, 0x04, 0x03,   0x03, 0x03, 0x03], # 2 | 20 | B
        ]
          
color_id_to_encoding = {
    1: 'U',
    2: 'L',
    3: 'F',
    4: 'B',
    5: 'R',
    6: 'D'
}

"""
Move mapping
0 -> U
1 -> D'
2 -> L
3 -> R'
4 -> B
5 -> F'
"""
moves = {
    "R" : "333", 
    "R'": "3",
    "U" : "0", 
    "U'": "000", 
    "D" : "111", 
    "D'": "1", 
    "L" : "2", 
    "L'": "222", 
    "B" : "4", 
    "B'": "444", 
    "F" : "555", 
    "F'": "5"
}

cube_repr = ""

for c in itertools.chain.from_iterable(faces):
    cube_repr += color_id_to_encoding[c]
    
try:
    s = kociemba.solve(cube_repr)
    print(cube_repr)
    print(s)
    for x in s.split(" "):
        if len(x) == 2 and x[1] == "2":
            print(moves[x[0]], end="")
            print(moves[x[0]], end="")
        else:
            print(moves[x], end="")
    print("\n")
except ValueError:
    pass

```


Output:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Sliding_2]
└─$ python3 solve.py
FRLDUDBUDBBUFRRRDDDLLBFLRBBFLURDFLRRLLRULUDUUBDUBBFFFF
R U' R U D2 R F B' D' R' U' B2 R2 U' L2 D' F2 U2 L2 F2
333000333011111133355544413000443333330002215555550022555555
```

Let's enter it as the solution to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Sliding_2]
└─$ echo 333000333011111133355544413000443333330002215555550022555555 | nc ctf.cs.technion.ac.il 4312
Enter key: Congratulation, The flag is cstechnion{at_least_you_now_know_how_to_solve_a_rubik's_cube}
```