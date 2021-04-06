# Trivial Flag Transfer Protocol
Category: Forensics, 90 points

## Description

> Figure out how they moved the flag.

A network capture was attached.

## Solution

Let's check the protocol statistics of the capture:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ tshark -qz io,phs -r tftp.pcapng

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:152413 bytes:47086514
  ip                                     frames:152399 bytes:47085800
    udp                                  frames:152399 bytes:47085800
      tftp                               frames:152395 bytes:47084936
        data                             frames:76192 bytes:42512764
      ssdp                               frames:4 bytes:864
  arp                                    frames:14 bytes:714
===================================================================
```

Looks like most of the traffic is [TFTP](https://en.wikipedia.org/wiki/Trivial_File_Transfer_Protocol) (Trivial File Transfer Protocol). Let's extract any files sent in the session:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ tshark -r tftp.pcapng --export-objects "tftp,exported_objects" -2 > /dev/null

┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ file exported_objects/*
exported_objects/instructions.txt: ASCII text
exported_objects/picture1.bmp:     PC bitmap, Windows 3.x format, 605 x 454 x 24, image size 824464, resolution 5669 x 5669 px/m, cbSize 824518, bits offset 54
exported_objects/picture2.bmp:     data
exported_objects/picture3.bmp:     data
exported_objects/plan:             ASCII text
exported_objects/program.deb:      Debian binary package (format 2.0), with control.tar.gz, data compression xz
```

We got two text files, three images, and a program.

The text files contain the following contents:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ cat exported_objects/instructions.txt
GSGCQBRFAGRAPELCGBHEGENSSVPFBJRZHFGQVFTHVFRBHESYNTGENAFSRE.SVTHERBHGNJNLGBUVQRGURSYNTNAQVJVYYPURPXONPXSBEGURCYNA
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ cat exported_objects/plan
VHFRQGURCEBTENZNAQUVQVGJVGU-QHRQVYVTRAPR.PURPXBHGGURCUBGBF
```

This looks like a code, hopefully a simple substitution cipher. Using [this online service](https://www.boxentriq.com/code-breaking/cryptogram) we can crack it:

```
tftp doesnt encrypt our traffic so we must disguise our flag transfer figure out a way to hide the flag and i will check back for the plan

i used the program and hid it with due diligence check out the photos
```

Let's check the program:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ dpkg-deb -I exported_objects/program.deb
 new Debian package, version 2.0.
 size 138310 bytes: control archive=1250 bytes.
     826 bytes,    18 lines      control
    1184 bytes,    17 lines      md5sums
 Package: steghide
 Source: steghide (0.5.1-9.1)
 Version: 0.5.1-9.1+b1
 Architecture: amd64
 Maintainer: Ola Lundqvist <opal@debian.org>
 Installed-Size: 426
 Depends: libc6 (>= 2.2.5), libgcc1 (>= 1:4.1.1), libjpeg62-turbo (>= 1:1.3.1), libmcrypt4, libmhash2, libstdc++6 (>= 4.9), zlib1g (>= 1:1.1.4)
 Section: misc
 Priority: optional
 Description: A steganography hiding tool
  Steghide is steganography program which hides bits of a data file
  in some of the least significant bits of another file in such a way
  that the existence of the data file is not visible and cannot be proven.
  .
  Steghide is designed to be portable and configurable and features hiding
  data in bmp, wav and au files, blowfish encryption, MD5 hashing of
  passphrases to blowfish keys, and pseudo-random distribution of hidden bits
  in the container data.
```

This is `steghide`, a stenography program used to hide files within other files. We can use it to extract the hidden files as well, as long as we have the correct password. But what is it?

Let's take another look at the second message. We decoded it as:

```
i used the program and hid it with due diligence check out the photos
```

But if we keep the original formatting, we get:

```
VHFRQGURCEBTENZNAQUVQVGJVGU-QHRQVYVTRAPR.PURPXBHGGURCUBGBF
->
IUSEDTHEPROGRAMANDHIDITWITH-DUEDILIGENCE.CHECKOUTTHEPHOTOS
```

Is `DUEDILIGENCE` the actual password?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ steghide extract -sf exported_objects/picture3.bmp -p "DUEDILIGENCE"
wrote extracted data to "flag.txt".

┌──(user@kali)-[/media/sf_CTFs/pico/Trivial_Flag_Transfer_Protocol]
└─$ cat flag.txt
picoCTF{h1dd3n_1n_pLa1n_51GHT_18375919}
```

The flag: `picoCTF{h1dd3n_1n_pLa1n_51GHT_18375919}`