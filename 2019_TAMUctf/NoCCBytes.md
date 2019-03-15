# KeyGenMe
Reversing, 439 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:
```console
root@kali:/media/sf_CTFs/tamu/NoCCBytes# ./noccbytes

Welcome. Please Enter a password to continue:
test

Wrong Password
```

It's always a good idea to get some insights about the program internals using `ltrace`:

```console
root@kali:/media/sf_CTFs/tamu/NoCCBytes# ltrace ./noccbytes
_ZNSt8ios_base4InitC1Ev(0x55d8c259604a, 0xffff, 0x7ffde7278288, 128)                                              = 0
__cxa_atexit(0x7f515221d0b0, 0x55d8c259604a, 0x55d8c2596008, 6)                                                   = 0
setvbuf(0x7f5152175760, 0, 2, 0)                                                                                  = 0
puts("\nWelcome. Please Enter a passwor"...
Welcome. Please Enter a password to continue:
)                                                                      = 48
fgets(test
"test\n", 25, 0x7f5152174a00)                                                                               = 0x7ffde7278100
strcpy(0x7ffde72780a0, "WattoSays\n")                                                                             = 0x7ffde72780a0
strcmp("test\n", "WattoSays\n")                                                                                   = 29
puts("\nWrong Password"
Wrong Password
)                                                                                          = 16
+++ exited (status 0) +++
```

We see the following call:
```
strcmp("test\n", "WattoSays\n")     
```

What if we try `WattoSays` as the password?

```console
root@kali:/media/sf_CTFs/tamu/_NoCCBytes# echo WattoSays | nc rev.tamuctf.com 8188

Welcome. Please Enter a password to continue:
gigem{Y0urBreakpo1nt5Won7Work0nMeOnlyMon3y}
```

Look at that, solved without breaking a sweat...