# tunn3l_v1s10n
Category: Forensics, 40 points

## Description

> We found this file. Recover the flag.

A file was attached.

## Solution

Let's check the file type:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ file tunn3l_v1s10n
tunn3l_v1s10n: data
```

It's just marked as a data file. How does it start?

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ xxd -g 1 tunn3l_v1s10n | head
00000000: 42 4d 8e 26 2c 00 00 00 00 00 ba d0 00 00 ba d0  BM.&,...........
00000010: 00 00 6e 04 00 00 32 01 00 00 01 00 18 00 00 00  ..n...2.........
00000020: 00 00 58 26 2c 00 25 16 00 00 25 16 00 00 00 00  ..X&,.%...%.....
00000030: 00 00 00 00 00 00 23 1a 17 27 1e 1b 29 20 1d 2a  ......#..'..) .*
00000040: 21 1e 26 1d 1a 31 28 25 35 2c 29 33 2a 27 38 2f  !.&..1(%5,)3*'8/
00000050: 2c 2f 26 23 33 2a 26 2d 24 20 3b 32 2e 32 29 25  ,/&#3*&-$ ;2.2)%
00000060: 30 27 23 33 2a 26 38 2c 28 36 2b 27 39 2d 2b 2f  0'#3*&8,(6+'9-+/
00000070: 26 23 1d 12 0e 23 17 11 29 16 0e 55 3d 31 97 76  &#...#..)..U=1.v
00000080: 66 8b 66 52 99 6d 56 9e 70 58 9e 6f 54 9c 6f 54  f.fR.mV.pX.oT.oT
00000090: ab 7e 63 ba 8c 6d bd 8a 69 c8 97 71 c1 93 71 c1  .~c..m..i..q..q.
```

It actually starts with `BM`, so this might be a `BMP` file. And indeed, if we change the extension to `BMP` and open it, we get an image together with some text saying "notaflag{sorry}".

The size of the image is `1134x306`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ exiftool tunn3l_v1s10n.bmp
ExifTool Version Number         : 12.16
File Name                       : tunn3l_v1s10n.bmp
Directory                       : .
File Size                       : 2.8 MiB
File Modification Date/Time     : 2021:04:06 20:24:34+03:00
File Access Date/Time           : 2021:04:06 20:29:28+03:00
File Inode Change Date/Time     : 2021:04:06 20:29:26+03:00
File Permissions                : rwxrwxrwx
File Type                       : BMP
File Type Extension             : bmp
MIME Type                       : image/bmp
BMP Version                     : Unknown (53434)
Image Width                     : 1134
Image Height                    : 306
Planes                          : 1
Bit Depth                       : 24
Compression                     : None
Image Length                    : 2893400
Pixels Per Meter X              : 5669
Pixels Per Meter Y              : 5669
Num Colors                      : Use BitDepth
Num Important Colors            : All
Red Mask                        : 0x27171a23
Green Mask                      : 0x20291b1e
Blue Mask                       : 0x1e212a1d
Alpha Mask                      : 0x311a1d26
Color Space                     : Unknown (,5%()
Rendering Intent                : Unknown (826103054)
Image Size                      : 1134x306
Megapixels                      : 0.347
```

But the file itself is larger than it should be:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ stat tunn3l_v1s10n.bmp | grep Size
  Size: 2893454         Blocks: 5656       IO Block: 4096   regular file
```

Let's parse the BMP header using `kaitai`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ ksv tunn3l_v1s10n.bmp ~/utils/forensics/kaitai_struct_formats/image/bmp.ksy
```

Note that when trying to run this command, we first get a warning: `attempted to read 53430 bytes, got only 53416`, and only then the following is displayed:

```
[-] [root]                               00000000: 42 4d 8e 26 2c 00 00 00 00 00 ba d0 00 00 ba d0 | BM.&,...........
  [-] file_hdr                           00000010: 00 00 6e 04 00 00 32 01 00 00 01 00 18 00 00 00 | ..n...2.........
    [.] file_type = 42 4d                00000020: 00 00 58 26 2c 00 25 16 00 00 25 16 00 00 00 00 | ..X&,.%...%.....
    [.] len_file = 2893454               00000030: 00 00 00 00 00 00 23 1a 17 27 1e 1b 29 20 1d 2a | ......#..'..) .*
    [.] reserved1 = 0                    00000040: 21 1e 26 1d 1a 31 28 25 35 2c 29 33 2a 27 38 2f | !.&..1(%5,)3*'8/
    [.] reserved2 = 0                    00000050: 2c 2f 26 23 33 2a 26 2d 24 20 3b 32 2e 32 29 25 | ,/&#3*&-$ ;2.2)%
    [.] ofs_bitmap = 53434               00000060: 30 27 23 33 2a 26 38 2c 28 36 2b 27 39 2d 2b 2f | 0'#3*&8,(6+'9-+/
  [-] dib_info                           00000070: 26 23 1d 12 0e 23 17 11 29 16 0e 55 3d 31 97 76 | &#...#..)..U=1.v
    [.] len_header = 53434               00000080: 66 8b 66 52 99 6d 56 9e 70 58 9e 6f 54 9c 6f 54 | f.fR.mV.pX.oT.oT
    [?] color_mask_alpha                 00000090: ab 7e 63 ba 8c 6d bd 8a 69 c8 97 71 c1 93 71 c1 | .~c..m..i..q..q.
    [?] color_mask_blue                  000000a0: 97 74 c1 94 73 c0 93 72 c0 8f 6f bd 8e 6e ba 8d | .t..s..r..o..n..
    [?] color_mask_green                 000000b0: 6b b7 8d 6a b0 85 64 a0 74 55 a3 77 5a 98 6f 56 | k..j..d.tU.wZ.oV
    [?] color_mask_red                   000000c0: 76 52 3a 71 52 3d 6c 4f 40 6d 52 44 6e 53 49 77 | vR:qR=lO@mRDnSIw
    [?] is_color_mask_here               000000d0: 5e 54 53 39 33 70 58 52 76 61 59 73 5f 54 7e 6b | ^TS93pXRvaYs_T~k
    [?] is_color_mask_given              000000e0: 5e 86 74 63 7e 6a 59 76 62 50 76 5e 4c 7a 62 50 | ^.tc~jYvbPv^LzbP
    [?] color_mask_given                 000000f0: 87 6d 5d 83 69 59 8d 73 63 9b 81 71 9e 84 74 98 | .m].iY.sc..q..t.
```

We can see that `len_header` has the value of `53434`, or `ba d0` in hex. This doesn't make sense, according to the [spec](https://medium.com/sysf/bits-to-bitmaps-a-simple-walkthrough-of-bmp-image-format-765dc6857393) the header size should be 40.  `ofs_bitmap` has the same value of `53434`, sending us far far away to look for the image data. The correct value should probably be `0x36`, which is right after the `file_hdr` (14 bytes) and `dib_info` (40 bytes).

We'll fix the header:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ xxd -g 1 tunn3l_v1s10n.bmp | head -1
00000000: 42 4d 8e 26 2c 00 00 00 00 00 36 00 00 00 28 00  BM.&,.....6...(.
```

Attempting to view the image again, it looks a bit better (and `ksv` doesn't complain anymore) but still no flag. 

```
[-] [root]                              00000000: 42 4d 8e 26 2c 00 00 00 00 00 36 00 00 00 28 00 | BM.&,.....6...(.
  [-] file_hdr                          00000010: 00 00 6e 04 00 00 32 01 00 00 01 00 18 00 00 00 | ..n...2.........
    [.] file_type = 42 4d               00000020: 00 00 58 26 2c 00 25 16 00 00 25 16 00 00 00 00 | ..X&,.%...%.....
    [.] len_file = 2893454              00000030: 00 00 00 00 00 00 23 1a 17 27 1e 1b 29 20 1d 2a | ......#..'..) .*
    [.] reserved1 = 0                   00000040: 21 1e 26 1d 1a 31 28 25 35 2c 29 33 2a 27 38 2f | !.&..1(%5,)3*'8/
    [.] reserved2 = 0                   00000050: 2c 2f 26 23 33 2a 26 2d 24 20 3b 32 2e 32 29 25 | ,/&#3*&-$ ;2.2)%
    [.] ofs_bitmap = 54                 00000060: 30 27 23 33 2a 26 38 2c 28 36 2b 27 39 2d 2b 2f | 0'#3*&8,(6+'9-+/
  [-] dib_info                          00000070: 26 23 1d 12 0e 23 17 11 29 16 0e 55 3d 31 97 76 | &#...#..)..U=1.v
    [.] len_header = 40                 00000080: 66 8b 66 52 99 6d 56 9e 70 58 9e 6f 54 9c 6f 54 | f.fR.mV.pX.oT.oT
    [-] header                          00000090: ab 7e 63 ba 8c 6d bd 8a 69 c8 97 71 c1 93 71 c1 | .~c..m..i..q..q.
      [.] image_width = 1134            000000a0: 97 74 c1 94 73 c0 93 72 c0 8f 6f bd 8e 6e ba 8d | .t..s..r..o..n..
      [.] image_height_raw = 306        000000b0: 6b b7 8d 6a b0 85 64 a0 74 55 a3 77 5a 98 6f 56 | k..j..d.tU.wZ.oV
      [.] num_planes = 1                000000c0: 76 52 3a 71 52 3d 6c 4f 40 6d 52 44 6e 53 49 77 | vR:qR=lO@mRDnSIw
      [.] bits_per_pixel = 24           000000d0: 5e 54 53 39 33 70 58 52 76 61 59 73 5f 54 7e 6b | ^TS93pXRvaYs_T~k
      [+] bitmap_info_ext               000000e0: 5e 86 74 63 7e 6a 59 76 62 50 76 5e 4c 7a 62 50 | ^.tc~jYvbPv^LzbP
      [?] extends_bitmap_v5             000000f0: 87 6d 5d 83 69 59 8d 73 63 9b 81 71 9e 84 74 98 | .m].iY.sc..q..t.
      [?] uses_fixed_palette            00000100: 7e 6e 9b 81 71 8d 73 63 73 5a 4a 70 57 47 5a 41 | ~n..q.scsZJpWGZA
      [?] image_height                  00000110: 31 4f 36 26 4e 37 27 4f 38 28 4f 38 28 51 3a 2a | 1O6&N7'O8(O8(Q:*
      [?] len_header                    00000120: 50 39 29 4f 38 29 4b 35 29 50 3a 2f 4b 35 2a 3f | P9)O8)K5)P:/K5*?
      [?] bottom_up                     00000130: 29 1e 42 2e 23 4b 37 2c 45 31 26 3f 2b 20 43 2f | ).B.#K7,E1&?+ C/
      [?] is_core_header                00000140: 24 43 2f 24 40 2a 1f 48 32 27 4b 32 28 47 2e 24 | $C/$@*.H2'K2(G.$
      [?] extends_bitmap_info           00000150: 40 27 1d 45 2c 22 4c 34 28 4c 34 28 4b 33 27 4a | @'.E,"L4(L4(K3'J
      [?] is_color_mask_here            00000160: 32 26 4c 32 24 4e 34 26 50 35 27 52 37 29 53 36 | 2&L2$N4&P5'R7)S6
      [?] extends_os2_2x_bitmap         00000170: 28 55 38 2a 4b 30 22 5d 42 34 63 49 39 49 2f 1f | (U8*K0"]B4cI9I/.
      [?] extends_bitmap_v4             00000180: 44 2b 1b 4d 34 24 4d 36 27 4a 33 24 46 2c 20 48 | D+.M4$M6'J3$F, H
```


Back to the file size. The file size is `2893454` bytes, and without the `54` bytes for the header(s), we get `2893400` bytes of body. Given that `bits_per_byte = 24`, this should should account for around `2893400 / 3 =~ 964466` pixels (since we have to add some scanline padding as explained in the spec). However, we only have `1134 * 306 = 347004` pixels according to the dimensions. We can try to adjust the dimensions so that all the data is accounted for. Adjusting the width seems to distort the image, so we should probably stick to adjusting the height. `964466 / 1134` gives us `~850` so let's try that as the height:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/tunn3l_v1s10n]
└─$ xxd -g 1 tunn3l_v1s10n.bmp | head -2
00000000: 42 4d 8e 26 2c 00 00 00 00 00 36 00 00 00 28 00  BM.&,.....6...(.
00000010: 00 00 6e 04 00 00 52 03 00 00 01 00 18 00 00 00  ..n...R.........
```

Trying to open the image now, we get the flag: `picoCTF{qu1t3_a_v13w_2020}`