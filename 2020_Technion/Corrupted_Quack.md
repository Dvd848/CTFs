# Corrupted Quack
Category: Forensics

## Description
> DuckyDebugDuck's sister sent him a picture from her vacation to Canada, but it got corrupted, can you fix it?

A `*.png` file was attached.

## Solution

Let's check the file with some basic tools:

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# pngcheck corrupted_quack.png
corrupted_quack.png  this is neither a PNG or JNG image nor a MNG stream
ERROR: corrupted_quack.png
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# file corrupted_quack.png
corrupted_quack.png: data
```

Neither `file` nor `pngcheck` can recognize this file as a PNG file. This means that something very basic in the file signature is corrupted.

According to the [specs](https://en.wikipedia.org/wiki/Portable_Network_Graphics), a PNG file should start with the magic number `89 50 4e 47 0d 0a 1a 0a`. What does our file start with?

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# xxd -g 1 corrupted_quack.png | head -n 1
00000000: 51 55 41 43 4b 4b 4b 4b 00 00 00 0d 44 55 43 4b  QUACKKKK....DUCK
```

That doesn't seem right. Let's fix the first 8 bytes according to the known signature:

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack#  printf '\x89\x50\x4e\x47\x0d\x0a\x1a\x0a' | dd conv=notrunc of=corrupted_quack.png  bs=1 seek=0
8+0 records in
8+0 records out
8 bytes copied, 0.00238445 s, 3.4 kB/s
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# xxd -g 1 corrupted_quack.png | head -n 1
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 44 55 43 4b  .PNG........DUCK
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# pngcheck corrupted_quack.png
corrupted_quack.png  first chunk must be IHDR
ERROR: corrupted_quack.png
```

This looks a bit better, but still not exactly right: `pngcheck` tells us that the first chunk must be of type `IHDR` (and not `DUCK`). Let's fix that:

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# printf 'IHDR' | dd conv=notrunc of=corrupted_quack.png  bs=1 seek=12
4+0 records in
4+0 records out
4 bytes copied, 0.00224021 s, 1.8 kB/s
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# xxd -g 1 corrupted_quack.png | head -n 1
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# pngcheck corrupted_quack.png
corrupted_quack.png  illegal (unless recently approved) unknown, public chunk DUCK
ERROR: corrupted_quack.png
```

We're past the `IHDR` error, now `pngcheck` is complaining about another unknown chunk type also called `DUCK`. Technically most viewers will already be able to display the image, but let's search for it and fix it:

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# grep --only-matching --byte-offset --binary --text DUCK corrupted_quack.png
987435:DUCK
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# xxd -g 1 -skip 987435 corrupted_quack.png
000f112b: 44 55 43 4b ae 42 60 82                          DUCK.B`.
```

This is the last chunk, which according to the specs should be called `IEND`. Let's fix it:

```console
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# printf 'IEND' | dd conv=notrunc of=corrupted_quack.png  bs=1 seek=987435
4+0 records in
4+0 records out
4 bytes copied, 0.00186267 s, 2.1 kB/s
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# xxd -g 1 corrupted_quack.png | tail -n 2
000f1120: f8 81 db 9c 24 4c 90 00 00 00 00 49 45 4e 44 ae  ....$L.....IEND.
000f1130: 42 60 82                                         B`.
root@kali:/media/sf_CTFs/technion/Corrupted_Quack# pngcheck corrupted_quack.png
OK: corrupted_quack.png (1280x1024, 32-bit RGB+alpha, non-interlaced, 81.2%).
```

Finally, we have a good PNG. The flag is in the image: `cstechnion{h3x4quackc1mal}`.

