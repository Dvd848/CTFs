# B1ll_Gat35
Reverse Engineering, 400 points

## Description:
> Can you reverse this Windows Binary?

## Solution:

This is a Windows reversing challenge. Let's start by running the executable:

```console
E:\CTFs\pico\B1ll_Gat35>win-exec-1.exe
Input a number between 1 and 5 digits: 1
Initializing...
Enter the correct key to get the access codes: test
Incorrect key. Try again.
```

We need to provide the correct key in order to get the flag.

Let's take a look at the main function's decompilation using Ghidra:

```c
void main(void)
{
  int iVar1;
  FILE *pFVar2;
  uint uVar3;
  int user_input;
  int num_digits;
  char is_valid;
  char local_6c [100];
  uint local_8;
  
  local_8 = DAT_0047b174 ^ (uint)&stack0xfffffffc;
  _print(s_Input_a_number_between_1_and_5_d_0047b06c);
  _scanf(&DAT_0047b094,&user_input);
  num_digits = 1;
  while (9 < user_input) {
    num_digits = num_digits + 1;
    user_input = user_input / 10;
  }
                    /* user_input is now least significat digit of input */
  if (num_digits < 6) {
    _print(s_Initializing..._0047b0b4);
    initialize_key(user_input,num_digits);
    do {
      iVar1 = thunk_FUN_00415840();
    } while (iVar1 != 10);
    _print(s_Enter_the_correct_key_to_get_the_0047b0c8);
    pFVar2 = (FILE *)___acrt_iob_func(0);
    thunk_FUN_004157db(local_6c,100,pFVar2);
    uVar3 = validate_key(local_6c);
    is_valid = (char)uVar3;
    if (is_valid == '\0') {
      _print(s_Incorrect_key._Try_again._0047b0f8);
    }
    else {
      _print(s_Correct_input._Printing_flag:_0047b114);
      print_flag();
    }
  }
  else {
    _print(s_Number_too_big._Try_again._0047b098);
  }
  thunk_FUN_004084bf();
  return;
}
```

We can see the calculation of the number of digits for the first request ("Input a number between 1 and 5 digits"), then some initialization using the number of digits and the least significant digit. Later, `validate_key` is called, and an appropriate string is printed according to the result.

Since the next-level functions are a bit complicated to statically reverse, it's convenient to use a debugger side-by-side. `x64dbg` is a great choice.

However, before starting to debug, it makes life easier to disable ASLR. This can be done using a tool called `CFF Explorer`. We open the executable in the tool, select `NT Headers -> Optional Headers -> DllCharacteristics -> Click Here -> DLL can move` and remove the checkbox. We can then save the executable and debug it without having to worry about ASLR.

Stepping through the code, we can see an interesting string in `eax` after calling `initialize_key` (a.k.a. `win-exec-1-noaslr.402117`):

```
00408040 | 55                       | push ebp                                           |
00408041 | 8BEC                     | mov ebp,esp                                        |
00408043 | 83EC 74                  | sub esp,74                                         |
00408046 | A1 74B14700              | mov eax,dword ptr ds:[47B174]                      | eax:"The key is: 425"
0040804B | 33C5                     | xor eax,ebp                                        |
0040804D | 8945 FC                  | mov dword ptr ss:[ebp-4],eax                       |
00408050 | 68 6CB04700              | push win-exec-1-noaslr.47B06C                      | 47B06C:"Input a number between 1 and 5 digits: "
00408055 | E8 0B94FFFF              | call win-exec-1-noaslr.401465                      |
0040805A | 83C4 04                  | add esp,4                                          |
0040805D | 8D45 8C                  | lea eax,dword ptr ss:[ebp-74]                      |
00408060 | 50                       | push eax                                           | eax:"The key is: 425"
00408061 | 68 94B04700              | push win-exec-1-noaslr.47B094                      | 47B094:"%d"
00408066 | E8 4698FFFF              | call win-exec-1-noaslr.4018B1                      |
0040806B | 83C4 08                  | add esp,8                                          |
0040806E | C745 90 01000000         | mov dword ptr ss:[ebp-70],1                        |
00408075 | 837D 8C 09               | cmp dword ptr ss:[ebp-74],9                        | 9:'\t'
00408079 | 7E 19                    | jle win-exec-1-noaslr.408094                       |
0040807B | 8B4D 90                  | mov ecx,dword ptr ss:[ebp-70]                      |
0040807E | 83C1 01                  | add ecx,1                                          |
00408081 | 894D 90                  | mov dword ptr ss:[ebp-70],ecx                      |
00408084 | 8B45 8C                  | mov eax,dword ptr ss:[ebp-74]                      |
00408087 | 99                       | cdq                                                |
00408088 | B9 0A000000              | mov ecx,A                                          | A:'\n'
0040808D | F7F9                     | idiv ecx                                           |
0040808F | 8945 8C                  | mov dword ptr ss:[ebp-74],eax                      |
00408092 | EB E1                    | jmp win-exec-1-noaslr.408075                       |
00408094 | 837D 90 05               | cmp dword ptr ss:[ebp-70],5                        |
00408098 | 7E 16                    | jle win-exec-1-noaslr.4080B0                       |
0040809A | 68 98B04700              | push win-exec-1-noaslr.47B098                      | 47B098:"Number too big. Try again.\n"
0040809F | E8 C193FFFF              | call win-exec-1-noaslr.401465                      |
004080A4 | 83C4 04                  | add esp,4                                          |
004080A7 | 33C0                     | xor eax,eax                                        | eax:"The key is: 425"
004080A9 | E9 8B000000              | jmp win-exec-1-noaslr.408139                       |
004080AE | EB 1D                    | jmp win-exec-1-noaslr.4080CD                       |
004080B0 | 68 B4B04700              | push win-exec-1-noaslr.47B0B4                      | 47B0B4:"Initializing...\n"
004080B5 | E8 AB93FFFF              | call win-exec-1-noaslr.401465                      |
004080BA | 83C4 04                  | add esp,4                                          |
004080BD | 8B55 90                  | mov edx,dword ptr ss:[ebp-70]                      |
004080C0 | 52                       | push edx                                           |
004080C1 | 8B45 8C                  | mov eax,dword ptr ss:[ebp-74]                      |
004080C4 | 50                       | push eax                                           | eax:"The key is: 425"
004080C5 | E8 4DA0FFFF              | call win-exec-1-noaslr.402117                      |
004080CA | 83C4 08                  | add esp,8                                          | <<----------------------------- EIP
004080CD | E8 93B1FFFF              | call win-exec-1-noaslr.403265                      |
```

The string says `The key is: 425`. Let's try that in parallel:

```console
E:\CTFs\pico\B1ll_Gat35>win-exec-1.exe
Input a number between 1 and 5 digits: 1
Initializing...
Enter the correct key to get the access codes: 425
Incorrect key. Try again.
```

No-go. Let's continue stepping. We are requested to enter our key at `win-exec-1-noaslr.4036ED` (a.k.a. `___acrt_iob_func`). We enter a random string (e.g. `1234`) and continue.

A few steps later, we see another interesting string in `ecx`:

```
004080F0 | 50                       | push eax                                           |
004080F1 | 6A 64                    | push 64                                            |
004080F3 | 8D4D 98                  | lea ecx,dword ptr ss:[ebp-68]                      |
004080F6 | 51                       | push ecx                                           | ecx:"The key is: 4253360"
004080F7 | E8 61AAFFFF              | call win-exec-1-noaslr.402B5D                      |
004080FC | 83C4 0C                  | add esp,C                                          |
004080FF | 8D55 98                  | lea edx,dword ptr ss:[ebp-68]                      |
00408102 | 52                       | push edx                                           |
00408103 | E8 ADACFFFF              | call win-exec-1-noaslr.402DB5                      |
00408108 | 83C4 04                  | add esp,4                                          |<<----------------------------- EIP
0040810B | 8845 97                  | mov byte ptr ss:[ebp-69],al                        |
0040810E | 0FB645 97                | movzx eax,byte ptr ss:[ebp-69]                     |
00408112 | 85C0                     | test eax,eax                                       |
00408114 | 75 0F                    | jne win-exec-1-noaslr.408125                       |
00408116 | 68 F8B04700              | push win-exec-1-noaslr.47B0F8                      | 47B0F8:"Incorrect key. Try again.\n"
0040811B | E8 4593FFFF              | call win-exec-1-noaslr.401465                      |
00408120 | 83C4 04                  | add esp,4                                          |
00408123 | EB 12                    | jmp win-exec-1-noaslr.408137                       |
00408125 | 68 14B14700              | push win-exec-1-noaslr.47B114                      | 47B114:"Correct input. Printing flag: "
0040812A | E8 3693FFFF              | call win-exec-1-noaslr.401465                      |
0040812F | 83C4 04                  | add esp,4                                          |
00408132 | E8 CE9DFFFF              | call win-exec-1-noaslr.401F05                      |
```

It now says "`The key is: 4253360`". Let's try that in parallel:

```console
E:\CTFs\pico\B1ll_Gat35>win-exec-1.exe
Input a number between 1 and 5 digits: 1
Initializing...
Enter the correct key to get the access codes: 4253360
Incorrect key. Try again.
```

Didn't work. But what if we try the complete string?
```console
E:\CTFs\pico\B1ll_Gat35>win-exec-1.exe
Input a number between 1 and 5 digits: 1
Initializing...
Enter the correct key to get the access codes: The key is: 4253360
Correct input. Printing flag: PICOCTF{These are the access codes to the vault: 1063340}
```

That actually gives us the flag, without any need to reverse the deeper levels.