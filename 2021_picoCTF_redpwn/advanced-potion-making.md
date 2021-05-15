# advanced-potion-making
Forensics, 100 points

## Description

> Ron just found his own copy of advanced potion making, but its been corrupted by some kind of spell. Help him recover it! 

A binary file was attached.

## Solution

Let's check the file type:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/advanced-potion-making]
└─$ file advanced-potion-making
advanced-potion-making: data
```

`file` doesn't recognize it. Let's peek inside:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/advanced-potion-making]
└─$ xxd -g 1 ./advanced-potion-making| head
00000000: 89 50 42 11 0d 0a 1a 0a 00 12 13 14 49 48 44 52  .PB.........IHDR
00000010: 00 00 09 90 00 00 04 d8 08 02 00 00 00 04 2d e7  ..............-.
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 00 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 00 00 76 39 49 44 41 54 78 5e ec fd 61  R$...v9IDATx^..a
00000060: 72 e3 4c 94 a6 59 ce 16 6a fe 76 cd fe 57 d7 dd  r.L..Y..j.v..W..
00000070: 5b 18 45 e9 4b 8a 7a 28 d1 9d 20 48 07 a9 63 76  [.E.K.z(.. H..cv
00000080: ac 2d 2b 3e bf af 5f 07 18 01 82 d7 b2 f3 ff f3  .-+>.._.........
00000090: ff fc 7f ff 7f 00 00 00 00 00 00 00 4b 18 58 02  ............K.X.
```

This looks like a PNG header due to the existence of `IHDR`, but doesn't start with the expected magic bytes. Let's fix it based on the spec and see what we get:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/advanced-potion-making]
└─$ xxd -g 1 advanced-potion-making-test.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 09 90 00 00 04 d8 08 02 00 00 00 04 2d e7  ..............-.
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 00 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 00 00 76 39 49 44 41 54 78 5e ec fd 61  R$...v9IDATx^..a
00000060: 72 e3 4c 94 a6 59 ce 16 6a fe 76 cd fe 57 d7 dd  r.L..Y..j.v..W..
00000070: 5b 18 45 e9 4b 8a 7a 28 d1 9d 20 48 07 a9 63 76  [.E.K.z(.. H..cv
00000080: ac 2d 2b 3e bf af 5f 07 18 01 82 d7 b2 f3 ff f3  .-+>.._.........
00000090: ff fc 7f ff 7f 00 00 00 00 00 00 00 4b 18 58 02  ............K.X.
```

Is there anything else wrong with this PNG file?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/advanced-potion-making]
└─$ pngcheck advanced-potion-making-test.png
OK: advanced-potion-making-test.png (2448x1240, 24-bit RGB, non-interlaced, 99.7%).
```

Looks Ok, but when we open the image we see it's completely red.

Using a tool such as `stegsolve` or [this website](https://incoherency.co.uk/image-steganography/#unhide) we can uncover a secret message hiding in the image: `picoCTF{w1z4rdry}`.