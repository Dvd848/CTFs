#  Secret Bits
Category: Steganography, 50 points

## Description

> Some bits are more secretive then others.... 

An image was attached.

## Solution

Just run `zsteg` on the image:

```console
┌──(user@kali)-[/media/sf_CTFs/ducky_debug_duck/Secret_Bits]
└─$ zsteg secret_bits.png
imagedata           .. text: "%'(:7+@;"
b1,bgr,lsb,xy       .. text: "flag{least_secretive_bits}\n"
b2,r,lsb,xy         .. text: ["U" repeated 12 times]
b2,g,lsb,xy         .. text: "+ 0G{Wmo"
b2,b,msb,xy         .. text: ["U" repeated 8 times]
b4,b,lsb,xy         .. text: ["D" repeated 24 times]
b4,b,msb,xy         .. text: ["\"" repeated 24 times]
```