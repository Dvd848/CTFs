# Can you see it?

 * Category: Steganography
 * 100 Points
 * Solved by the JCTF Team

## Description

> This image isn't just a picture - it's a puzzle waiting to be unraveled.
> The hidden message isn't painted on the surface, but woven into the very fabric of the image.
> Can you pierce the veil and extract the secret data concealed within?

An image was attached.

## Solution

We can run `zsteg` on the image and see that it suggests a few options:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Can_you_see_it]
└─$ zsteg Can_You_See_It/chal.png
imagedata           .. file: VAX-order 68K Blit (standalone) executable
b1,b,lsb,xy         .. file: AIX core file
b1,a,lsb,xy         .. file: PNG image data, 256 x 20, 8-bit grayscale, non-interlaced
b1,rgba,lsb,xy      .. text: "#3\"\"3#\"\"22\"#22\"\""
b1,rgba,msb,xy      .. text: "LLDD;;33333333333333"
b1,abgr,lsb,xy      .. text: ["3" repeated 14 times]
b1,abgr,msb,xy      .. text: "23\"\"32\"\"##\"2##\"\""
b2,r,lsb,xy         .. text: ["U" repeated 32 times]
b2,r,msb,xy         .. text: ["U" repeated 8 times]
b2,g,lsb,xy         .. text: ["U" repeated 8 times]
b2,g,msb,xy         .. text: ["U" repeated 8 times]
b2,b,msb,xy         .. text: "RUUUUUUUU"
b2,a,msb,xy         .. text: "wUwUUUUUUU"
b2,rgba,lsb,xy      .. text: ".///....//./...././....//./.....srsrrrrrrrrrrrrrrrrrrrrrrrrrrrrrssrsrsrrsrrsrsrrsrrrrsrrrsrrrsrsrrsrrrrrrrrrrrrrrrrrrrrrrrrsrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrs./....../..................................../...///.././..//.//ssrr"
b2,rgba,msb,xy      .. text: ["N" repeated 29 times]
b4,r,lsb,xy         .. text: "wwwwwwwwwwwwwwDDDDDDDDDDDDDDDDUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUffffffffffffffffDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"
b4,r,msb,xy         .. text: ["\"" repeated 16 times]
b4,g,lsb,xy         .. text: "wwwwwwwwwwwwwwffffffffffffffffwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwffffffffffffffffffffffffffffffffwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"
b4,g,msb,xy         .. text: ["f" repeated 16 times]
b4,a,msb,xy         .. text: ["w" repeated 14 times]
```

`zsteg` can even attempt to extract the file(s). We'll extract the PNG:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Can_you_see_it]
└─$ zsteg Can_You_See_It/chal.png -E b1,a,lsb,xy > extracted.png

┌──(user@kali)-[/media/sf_CTFs/bsides/Can_you_see_it]
└─$ file extracted.png
extracted.png: PNG image data, 256 x 20, 8-bit grayscale, non-interlaced
```

The image contained the flag: `BSidesTLV2024{wh0_kn3w_1ts_1n?}`