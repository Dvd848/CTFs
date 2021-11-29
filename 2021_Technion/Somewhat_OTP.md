# Somewhat OTP
Category: Cryptography, 250 points

## Description

> Oh no! EEvil department managed to get an hold on our computers and encrypted all of our files! We know from last time that their cryptography skills are not the best, see if you can reverse the encryption, you might even get a flag...

A binary file was attached.

## Solution

Let's check the file:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ file somewhat_otp.png.evil
somewhat_otp.png.evil: data
```

It isn't recognized, we should check the contents:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ xxd -g 1 somewhat_otp.png.evil | head
00000000: 89 d9 c7 ce 84 83 93 83 89 89 89 84 c0 c1 cd db  ................
00000010: 00 00 07 80 00 00 04 38 08 06 00 00 00 e8 d3 c1  .......8........
00000020: 43 43 43 43 45 21 08 04 07 43 bc 43 bc 43 bc e3  CCCCE!...C.C.C..
00000030: bd 1a 2e bd bd bd b4 cd f5 e4 ce bd bd 93 9e bd  ................
00000040: 00 2e 23 01 78 a5 3f 76 00 00 00 07 74 49 4d 45  ..#.x.?v....tIME
00000050: 07 e2 0c 0a 0a 2a 30 98 af a2 e9 07 07 07 1e 73  .....*0........s
00000060: 45 1d 31 06 2a 28 28 20 2b 31 45 06 37 20 24 31  E.1.*(( +1E.7 $1
00000070: 65 01 45 12 0c 11 0d 45 22 2c 28 35 32 e4 6b 72  e.E....E",(52.kr
00000080: 00 00 20 00 49 44 41 54 78 da ec dd cd 6f 94 d7  .. .IDATx....o..
00000090: dd 2d 2c 42 e0 6b 1a eb 1b bb 1f e6 eb f9 49 ca  .-,B.k........I.
```

We can clearly see `tIME` and `IDAT` which are known PNG chunks names, but the PNG header is corrupted. 

We know that a PNG file should start with the following sequence:

```
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
```

This is what we have instead:

```
89 d9 c7 ce 84 83 93 83 89 89 89 84 c0 c1 cd db
```

Here they are one on top of the other:

```
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52
89 d9 c7 ce 84 83 93 83 89 89 89 84 c0 c1 cd db
```

Notice how the `00 00 00` sequence gets translated to `89 89 89`, and how `0A` gets translated to `83` twice. That's feels like a substitution cipher or a XOR encryption. It's easy to check the latter:

```python
>>> a = "89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52"
>>> b = "89 d9 c7 ce 84 83 93 83 89 89 89 84 c0 c1 cd db"
>>> for (x, y) in zip(a.split(), b.split()):
...     print(hex(int(x, 16) ^ int(y, 16)), end=" ")
...
0x0 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89 0x89
```

So most of the line was encrypted using `0x89`, except for the first byte.

For other bytes, we can't really know for sure what the expected value is. But, if we scroll down a bit, we can see an interesting pattern:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ xxd -g 1 --skip 256 --length 256 somewhat_otp.png.evil
00000100: 00 00 00 00 00 00 f8 dc ab b7 04 00 00 00 00 00  ................
00000110: 00 00 39 08 c0 00 00 00 00 00 00 00 49 08 c0 00  ..9.........I...
00000120: 00 00 00 00 00 00 49 08 c0 00 00 00 00 00 00 00  ......I.........
00000130: 49 41 89 49 49 49 49 49 49 49 00 41 89 49 49 49  IA.IIIIIII.A.III
00000140: 00 00 00 00 49 08 c0 00 00 00 00 00 00 00 49 08  ....I.........I.
00000150: c0 c0 c0 c0 c0 c0 c0 c0 89 c8 00 c0 c0 c0 c0 c0  ................
00000160: 00 00 49 08 c0 00 00 00 00 00 00 00 49 08 c0 00  ..I.........I...
00000170: 00 00 00 00 00 00 49 08 c0 00 00 00 00 00 00 00  ......I.........
00000180: 49 41 89 49 49 49 49 49 49 49 00 41 89 49 49 49  IA.IIIIIII.A.III
00000190: 00 00 00 00 49 08 c0 00 00 00 00 00 00 00 49 08  ....I.........I.
000001a0: c0 c0 c0 c0 c0 c0 c0 c0 89 c8 00 c0 c0 c0 c0 c0  ................
000001b0: 00 00 49 08 c0 00 00 00 00 00 00 00 49 08 c0 00  ..I.........I...
000001c0: 00 00 00 00 00 00 49 08 c0 00 00 00 00 00 00 00  ......I.........
000001d0: 49 41 89 49 49 49 49 49 49 49 00 41 89 49 49 49  IA.IIIIIII.A.III
000001e0: 00 00 00 00 49 08 c0 00 00 00 00 00 00 00 49 08  ....I.........I.
000001f0: c0 c0 c0 c0 c0 c0 c0 c0 89 c8 00 c0 c0 c0 c0 c0  ................
```

There are many zeroes there, but every now and then one of the lines has a different common value (e.g. `0x49` or `0xc0`). To turn those lines back into (mostly) zeroes, we'll have to XOR them with the common value from the same line. For example, if we take `c0 c0 c0 c0 c0 c0 c0 c0 89 c8 00 c0 c0 c0 c0 c0` and XOR it with `0xc0`, we get `00 00 00 00 00 00 00 00 49 08 c0 00 00 00 00 00`. 

Adding these two observations together, we can come up with a theory where the first byte of each line (16 bytes) is the key used to encrypt the rest of the line. So, to decrypt the file, we can try:

```python
with open("somewhat_otp.png.evil", "rb") as f, open("output.png", "wb") as o:
    while chunk := f.read(16):
        o.write(bytes([chunk[0]] + [chunk[0] ^ x for x in chunk[1:]]))
```

This gives us:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ python3 solve.py

┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ pngcheck output.png
OK: output.png (1920x1080, 32-bit RGB+alpha, non-interlaced, 99.0%).
```

The result is a slightly rotated text with the flag, we can rotate it back and use OCR to avoid typing it:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ convert -rotate "8" output.png rotated.png

┌──(user@kali)-[/media/sf_CTFs/technion/Somewhat_OTP]
└─$ tesseract rotated.png -
cstechnion{atleast_it's_not_vigenere}
```