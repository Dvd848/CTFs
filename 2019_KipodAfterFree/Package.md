# Package
Reversing, 15 points

## Description
> A package has arrived, come take it!

An executable file was attached.

## Solution

Let's start by running the executable:

```console
E:\CTFs\kaf\Package>Package.exe
Hello, you have a package in the mail!
Please enter your details, inorder to take the package
Enter your name : test_name
Enter your password: test_pass
THIS IS NOT YOUR PACKAGE!!!
```

Looks like we need to find the correct username/password.

Let's inspect the binary:
```console
user@CTF_PC:/mnt/e/CTFs/kaf/Package$ file Package.exe
Package.exe: PE32 executable (console) Intel 80386, for MS Windows, UPX compressed
```

This binary is compressed with [UPX](https://en.wikipedia.org/wiki/UPX), we should extract it before trying to analyze it:
```console
user@CTF_PC:/mnt/e/CTFs/kaf/Package$ upx -d Package.exe -oUnpackedPackage.exe
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2018
UPX 3.95        Markus Oberhumer, Laszlo Molnar & John Reiser   Aug 26th 2018

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
     43539 <-     27155   62.37%    win32/pe     UnpackedPackage.exe

Unpacked 1 file.
user@CTF_PC:/mnt/e/CTFs/kaf/Package$ file UnpackedPackage.exe
UnpackedPackage.exe: PE32 executable (console) Intel 80386, for MS Windows
```

Now we can open the file in Ghidra and view the decompiled output:

```c
int __cdecl _main(int _Argc,char **_Argv,char **_Env)

{
  size_t input_length;
  char *_Str1;
  int iVar1;
  char password [21];
  char name [62];
  
  ___main();
  _puts("Hello, you have a package in the mail!");
  _puts("Please enter your details, inorder to take the package");
  _printf("Enter your name : ");
  _scanf("%s",name);
  input_length = _strlen(name);
  if (50 < input_length) {
    _printf("You reached the max length!");
                    /* WARNING: Subroutine does not return */
    _exit(0);
  }
  _printf("Enter your password: ");
  _scanf("%s",password);
  input_length = _strlen(password);
  password[input_length] = '\0';
  input_length = _strlen(password);
  if (50 < input_length) {
    _printf("You reached the max length!");
                    /* WARNING: Subroutine does not return */
    _exit(0);
  }
  _Str1 = _hashed((int)"n9ain9ain9ai_n9aik9l");
  iVar1 = _strncmp(_Str1,password,20);
  if (iVar1 == 0) {
    _printf("Enjoy your package!");
                    /* WARNING: Subroutine does not return */
    _exit(0);
  }
  _printf("THIS IS NOT YOUR PACKAGE!!!");
  return 0;
}
```

The main function is pretty straightforward - the password must be equal to the result of `_hashed("n9ain9ain9ai_n9aik9l")`.

Ghidra provides the full decompiled implementation of `_hashed`:

```c
char * __cdecl _hashed(int param_1)

{
  char cVar1;
  char local_24 [28];
  int local_8;
  
  local_8 = 0;
  while (local_8 < 0x14) {
    cVar1 = *(char *)(param_1 + local_8);
    if ((cVar1 < 'a') || ('z' < cVar1)) {
      if ((cVar1 < '0') || ('9' < cVar1)) {
        local_24[local_8] = cVar1;
      }
      else {
        local_24[local_8] = cVar1 + -5;
      }
    }
    else {
      local_24[local_8] = cVar1 + '\x02';
    }
    local_8 = local_8 + 1;
  }
  return local_24;
}
```

We can take this code almost as-is and use it (instead of returning what is a local array, just print it).
The result of calling `_hashed("n9ain9ain9ai_n9aik9l")` is `p4ckp4ckp4ck_p4ckm4n`. Let's verify this against the executable:

```console
E:\CTFs\kaf\Package>Package.exe
Hello, you have a package in the mail!
Please enter your details, inorder to take the package
Enter your name : name
Enter your password: p4ckp4ckp4ck_p4ckm4n
Enjoy your package!
```

Therefore, the flag is `KAF{p4ckp4ckp4ck_p4ckm4n}`.