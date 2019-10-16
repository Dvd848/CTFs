# c0rrupt
Forensics, 250 points

## Description:
> We found this file. Recover the flag.


## Solution: 

Let's see what we can tell about the file:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# file mystery
mystery: data
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 mystery | head
00000000: 89 65 4e 34 0d 0a b0 aa 00 00 00 0d 43 22 44 52  .eN4........C"DR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 aa 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 aa aa ff a5 ab 44 45 54 78 5e ec bd 3f  R$......DETx^..?
00000060: 8e 64 cd 71 bd 2d 8b 20 20 80 90 41 83 02 08 d0  .d.q.-.  ..A....
00000070: f9 ed 40 a0 f3 6e 40 7b 90 23 8f 1e d7 20 8b 3e  ..@..n@{.#... .>
00000080: b7 c1 0d 70 03 74 b5 03 ae 41 6b f8 be a8 fb dc  ...p.t...Ak.....
00000090: 3e 7d 2a 22 33 6f de 5b 55 dd 3d 3d f9 20 91 88  >}*"3o.[U.==. ..
```

`file` won't recognize it, but inspecting the header we can see strings which are common in PNG files. According to the [PNG specs], the first 8 bytes of the file are constant, so let's go ahead and fix that:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# cp mystery fixed.png
root@kali:/media/sf_CTFs/pico/c0rrupt# printf '\x89\x50\x4E\x47\x0D\x0A\x1A\x0A' | dd of=fixed.png bs=1 seek=0 count=8 conv=notrunc
8+0 records in
8+0 records out
8 bytes copied, 0.00550841 s, 1.5 kB/s
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 fixed.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 43 22 44 52  .PNG........C"DR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 aa 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 aa aa ff a5 ab 44 45 54 78 5e ec bd 3f  R$......DETx^..?
00000060: 8e 64 cd 71 bd 2d 8b 20 20 80 90 41 83 02 08 d0  .d.q.-.  ..A....
00000070: f9 ed 40 a0 f3 6e 40 7b 90 23 8f 1e d7 20 8b 3e  ..@..n@{.#... .>
00000080: b7 c1 0d 70 03 74 b5 03 ae 41 6b f8 be a8 fb dc  ...p.t...Ak.....
00000090: 3e 7d 2a 22 33 6f de 5b 55 dd 3d 3d f9 20 91 88  >}*"3o.[U.==. ..
```

After the header come a series of chunks. Each chunk starts with 4 bytes for the length of the chunk, 4 bytes for the type, then the chunk content itself (with the length declared earlier) and 4 bytes of a checksum. The first chunk is `IHDR` and has the length of `0xD`, so let's fix that as well.

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# printf '\x00\x00\x00\x0D\x49\x48\x44\x52' | dd of=fixed.png bs=1 seek=8 count=8 conv=notrunc
8+0 records in
8+0 records out
8 bytes copied, 0.00210317 s, 3.8 kB/s
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 fixed.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 aa 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 aa aa ff a5 ab 44 45 54 78 5e ec bd 3f  R$......DETx^..?
00000060: 8e 64 cd 71 bd 2d 8b 20 20 80 90 41 83 02 08 d0  .d.q.-.  ..A....
00000070: f9 ed 40 a0 f3 6e 40 7b 90 23 8f 1e d7 20 8b 3e  ..@..n@{.#... .>
00000080: b7 c1 0d 70 03 74 b5 03 ae 41 6b f8 be a8 fb dc  ...p.t...Ak.....
00000090: 3e 7d 2a 22 33 6f de 5b 55 dd 3d 3d f9 20 91 88  >}*"3o.[U.==. ..
```

Now the file is identified as a PNG file:
```console
root@kali:/media/sf_CTFs/pico/c0rrupt# file fixed.png
fixed.png: PNG image data, 1642 x 1095, 8-bit/color RGB, non-interlaced
```

However, `pngcheck` complains about errors:
```console
root@kali:/media/sf_CTFs/pico/c0rrupt# pngcheck -v -f fixed.png
File: fixed.png (202940 bytes)
  chunk IHDR at offset 0x0000c, length 13
    1642 x 1095 image, 24-bit RGB, non-interlaced
  chunk sRGB at offset 0x00025, length 1
    rendering intent = perceptual
  chunk gAMA at offset 0x00032, length 4: 0.45455
  chunk pHYs at offset 0x00042, length 9: 2852132389x5669 pixels/meter
  CRC error in chunk pHYs (computed 38d82c82, expected 495224f0)
:  invalid chunk length (too large)
ERRORS DETECTED in fixed.png
```

Let's take a look at the `pHYs` chunk:
```
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 -s 0x3e -l $((4+4+9+4)) fixed.png
0000003e: 00 00 00 09 70 48 59 73 aa 00 16 25 00 00 16 25  ....pHYs...%...%
0000004e: 01 49 52 24 f0                                   .IR$.
```

The header declared 9 bytes, then come 4 bytes of the type (`pHYs`), then nine bytes of the payload and 4 bytes of the checksum. `pngcheck` says that the expected checksum as stated in the file (`0x495224f0`) doesn't match the computed checksum. Therefore, either the checksum is corrupted, or the data is. 

The definition of `pHYs` is:
```
   Pixels per unit, X axis: 4 bytes (unsigned integer)
   Pixels per unit, Y axis: 4 bytes (unsigned integer)
   Unit specifier:          1 byte
```

In our case, we have:
```
Pixels per unit, X axis: 4 bytes (unsigned integer) <-- aa 00 16 25
Pixels per unit, Y axis: 4 bytes (unsigned integer) <-- 00 00 16 25
Unit specifier:          1 byte                     <-- 01
```

Since the pixels per unit differ in just one byte, and the 0xaa for the X axis makes the value very large, it makes sense to place a zero instead. Let's see if that fixes the checksum:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# printf '\x00' | dd of=fixed.png bs=1 seek=70 count=1 conv=notrunc
1+0 records in
1+0 records out
1 byte copied, 0.00376626 s, 0.3 kB/s
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 fixed.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 00 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 aa aa ff a5 ab 44 45 54 78 5e ec bd 3f  R$......DETx^..?
00000060: 8e 64 cd 71 bd 2d 8b 20 20 80 90 41 83 02 08 d0  .d.q.-.  ..A....
00000070: f9 ed 40 a0 f3 6e 40 7b 90 23 8f 1e d7 20 8b 3e  ..@..n@{.#... .>
00000080: b7 c1 0d 70 03 74 b5 03 ae 41 6b f8 be a8 fb dc  ...p.t...Ak.....
00000090: 3e 7d 2a 22 33 6f de 5b 55 dd 3d 3d f9 20 91 88  >}*"3o.[U.==. ..
root@kali:/media/sf_CTFs/pico/c0rrupt# pngcheck -v -f fixed.png
File: fixed.png (202940 bytes)
  chunk IHDR at offset 0x0000c, length 13
    1642 x 1095 image, 24-bit RGB, non-interlaced
  chunk sRGB at offset 0x00025, length 1
    rendering intent = perceptual
  chunk gAMA at offset 0x00032, length 4: 0.45455
  chunk pHYs at offset 0x00042, length 9: 5669x5669 pixels/meter (144 dpi)
:  invalid chunk length (too large)
ERRORS DETECTED in fixed.png
```

That fixed the problem, we remain with a "invalid chunk length (too large)" message.

Let's take a look at what starts after the `pHYs` chunk ends:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 -s 0x53 -l 8 fixed.png
00000053: aa aa ff a5 ab 44 45 54                          .....DET
```

We have a chunk of size 0xaaaaffa5 which is very large, and a type of `\xabDET` which doesn't exist. The closest chunk type is IDAT, let's try to fix that first:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# printf 'IDAT' | dd of=fixed.png bs=1 seek=87 count=4 conv=notrunc
4+0 records in
4+0 records out
4 bytes copied, 0.00224062 s, 1.8 kB/s
```

Now let's take a look at the size. `IDAT` chunks must be consecutive:

```
IHDR    No      Must be first
PLTE    No      Before IDAT
IDAT    Yes     Multiple IDATs must be consecutive
IEND    No      Must be last
```

So we can search for the next IDAT chunk (if it exists) and calculate the difference.

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# binwalk -R "IDAT" fixed.png

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
87            0x57            Raw signature (IDAT)
65544         0x10008         Raw signature (IDAT)
131080        0x20008         Raw signature (IDAT)
196616        0x30008         Raw signature (IDAT)
```

The next `IDAT` chunk is at offset `0x10004`. We count the length of the first `IDAT` chunk starting from `0x5B`, and need to add another extra 4 bytes for the checksum. Therefore, we get the length of `0x10004 - 0x5B - 0x4 = 0xFFA5` which is good since the original value is `0xAAAAFFA5`. So, we just need to override `0xAAAA` with zeroes again.

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# printf '\x00\x00' | dd of=fixed.png bs=1 seek=83 count=2 conv=notrunc
2+0 records in
2+0 records out
2 bytes copied, 0.00197452 s, 1.0 kB/s
root@kali:/media/sf_CTFs/pico/c0rrupt# xxd -g 1 fixed.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 00 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 00 00 ff a5 49 44 41 54 78 5e ec bd 3f  R$.....IDATx^..?
00000060: 8e 64 cd 71 bd 2d 8b 20 20 80 90 41 83 02 08 d0  .d.q.-.  ..A....
00000070: f9 ed 40 a0 f3 6e 40 7b 90 23 8f 1e d7 20 8b 3e  ..@..n@{.#... .>
00000080: b7 c1 0d 70 03 74 b5 03 ae 41 6b f8 be a8 fb dc  ...p.t...Ak.....
00000090: 3e 7d 2a 22 33 6f de 5b 55 dd 3d 3d f9 20 91 88  >}*"3o.[U.==. ..
```

`pngcheck` doesn't complain anymore:

```console
root@kali:/media/sf_CTFs/pico/c0rrupt# pngcheck -v -f fixed.png
File: fixed.png (202940 bytes)
  chunk IHDR at offset 0x0000c, length 13
    1642 x 1095 image, 24-bit RGB, non-interlaced
  chunk sRGB at offset 0x00025, length 1
    rendering intent = perceptual
  chunk gAMA at offset 0x00032, length 4: 0.45455
  chunk pHYs at offset 0x00042, length 9: 5669x5669 pixels/meter (144 dpi)
  chunk IDAT at offset 0x00057, length 65445
    zlib: deflated, 32K window, fast compression
  chunk IDAT at offset 0x10008, length 65524
  chunk IDAT at offset 0x20008, length 65524
  chunk IDAT at offset 0x30008, length 6304
  chunk IEND at offset 0x318b4, length 0
No errors detected in fixed.png (9 chunks, 96.3% compression).
```

Viewing the image, we get the flag: `picoCTF{c0rrupt10n_1847995}`.