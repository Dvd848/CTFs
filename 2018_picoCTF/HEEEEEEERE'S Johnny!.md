# HEEEEEEERE'S Johnny!
Cryptography, 100 points

## Description:
> Okay, so we found some important looking files on a linux computer. Maybe they can be used to get a password to the process. Connect with nc 2018shell3.picoctf.com 38860. Files can be found here: _passwd_ _shadow_. 


## Solution:

We can use [John the Ripper](https://www.openwall.com/john/) to crack the password.

```console
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# cat passwd 
root:x:0:0:root:/root:/bin/bash
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# cat shadow 
root:$6$IGI9prWh$ZHToiAnzeD1Swp.zQzJ/Gv.iViy39EmjVsg3nsZlfejvrAjhmp5jY.1N6aRbjFJVQX8hHmTh7Oly3NzogaH8c1:17770:0:99999:7:::
```

First we use the `unshadow` utility to combine the `passwd` and `shadow` files and output them in the format of the traditional Unix password file:

```console
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# unshadow passwd shadow > input.txt
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# cat input.txt 
root:$6$IGI9prWh$ZHToiAnzeD1Swp.zQzJ/Gv.iViy39EmjVsg3nsZlfejvrAjhmp5jY.1N6aRbjFJVQX8hHmTh7Oly3NzogaH8c1:0:0:root:/root:/bin/bash
```

Then we run `john` to try and crack the password:

```console
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# john input.txt 
Warning: detected hash type "sha512crypt", but the string is also recognized as "crypt"
Use the "--format=crypt" option to force loading these as that type instead
Using default input encoding: UTF-8
Loaded 1 password hash (sha512crypt, crypt(3) $6$ [SHA512 128/128 AVX 2x])
Press 'q' or Ctrl-C to abort, almost any other key for status
password1        (root)
1g 0:00:00:01 DONE 2/3 (2018-10-08 23:22) 0.6666g/s 587.4p/s 587.4c/s 587.4C/s 123456..green
Use the "--show" option to display all of the cracked passwords reliably
Session completed
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# john input.txt --show
root:password1:0:0:root:/root:/bin/bash

1 password hash cracked, 0 left

```

The password is **password1**.
Now we can connect to the remote host and obtain the flag:

```console
root@kali:/media/sf_CTFs/pico/HEEEEEEERES_Johnny# nc 2018shell3.picoctf.com 38860
Username: root
Password: password1
picoCTF{J0hn_1$_R1pp3d_4e5aa29e}
```

The flag: picoCTF{J0hn_1$_R1pp3d_4e5aa29e}