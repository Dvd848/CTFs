# Radix's Terminal
Reversing, 400 points

## Description:
> Can you find the password to Radix's login?

A binary file was attached.

## Solution:

Let's run the program and inspect the output:

```console
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# ./radix
Please provide a password!
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# ./radix password
Incorrect Password!
```

We need to provide a password.

`ltrace` can give us some insights about the logic executed by the program:

```
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# ltrace ./radix password
__libc_start_main(0x8048737, 2, 0xffcd60b4, 0x80487d0 <unfinished ...>
setvbuf(0xf7f7bd80, 0, 2, 0)                                                                                      = 0
strlen("password")                                                                                                = 8
strncmp("cGFzc3dvcmQ=\302", "cGljb0NURntiQXNFXzY0X2VOQ29EaU5n"..., 12)                                            = -1
puts("Incorrect Password!"Incorrect Password!
)                                                                                       = 20
+++ exited (status 255) +++
```

So it compares `cGFzc3dvcmQ=` with `cGljb0NURntiQXNFXzY0X2VOQ29EaU5n...`. `cGFzc3dvcmQ=` is simply our argument, decoded as base64:

```console
# echo "cGFzc3dvcmQ=" | base64 -d && echo
password
```

We need to find the string that our input is being compared to. Running `strings` on the binary provides too many result, but if we just search for string of length 30+, we get:

```console
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# strings radix | grep -x '.\{30,\}'
cGljb0NURntiQXNFXzY0X2VOQ29EaU5nX2lTX0VBc1lfNzU3NDAyNTF9
Congrats, now where's my flag?
ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/
GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.10) 5.4.0 20160609
__do_global_dtors_aux_fini_array_entry
__frame_dummy_init_array_entry
```

The first result is what we're looking for. Let's decode it:

```console
root@kali:/media/sf_CTFs/pico/Radixs_Terminal# echo "cGljb0NURntiQXNFXzY0X2VOQ29EaU5nX2lTX0VBc1lfNzU3NDAyNTF9" | base64 -d && echo
picoCTF{bAsE_64_eNCoDiNg_iS_EAsY_75740251}
```

The flag: picoCTF{bAsE_64_eNCoDiNg_iS_EAsY_75740251}