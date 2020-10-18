# Challenge 2

In the previous challenge, we found a RAR file hidden inside a JPEG file, time to try and extract it:

```console
root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# unrar x 2F668.rar

UNRAR 5.50 freeware      Copyright (c) 1993-2017 Alexander Roshal

Enter password (will not be echoed) for 2F668.rar:

Program aborted
```

Looks like it's password-protected. Can we crack the password using `John the Ripper`?

```console
root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# rar2john 2F668.rar
2F668.rar:$rar5$16$579d4072e134154ad96dc406a4c79a2a$15$b3e6f0509aad72672c4704a6ecde25f2$8$79fbf0734b305be8
root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# rar2john 2F668.rar > rar.john

root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# ~/utils/john/run/john rar.john --wordlist=/root/utils/dictionaries/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (RAR5 [PBKDF2-SHA256 256/256 AVX2 8x])
Cost 1 (iteration count) is 32768 for all loaded hashes
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
Password!        (2F668.rar)
1g 0:00:02:49 DONE (2020-09-17 21:16) 0.005889g/s 267.2p/s 267.2c/s 267.2C/s SEXYLOVE..LAURITA
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

Looks like the password is `Password!`. Let's use it to extract the archive:

```console
root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# unrar x -pPassword! 2F668.rar

UNRAR 5.50 freeware      Copyright (c) 1993-2017 Alexander Roshal


Extracting from 2F668.rar

Extracting  Payload.png                                               OK
All OK
root@kali:/media/sf_CTFs/hoshen/1/_Sukkah.jpg.extracted# file Payload.png
Payload.png: PNG image data, 2100 x 1575, 8-bit colormap, non-interlaced
```

We got a PNG file.