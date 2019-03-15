# I heard you like files
Misc, 306 points

## Description:

> Bender B. Rodriguez was caught with a flash drive with only a single file on it. We think it may contain valuable information. His area of research is PDF files, so it's strange that this file is a PNG.

A PNG file was attached.

## Solution:

We start with inspecting the PNG file.

```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# file art.png
art.png: PNG image data, 1920 x 1080, 8-bit/color RGBA, non-interlaced
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# pngcheck art.png
art.png  additional data after IEND chunk
ERROR: art.png
```

This is indeed a PNG file (we can even view the image), but `pngcheck` claims that there is additional data after the IEND chunk.

Let's try to extract it with `binwalk`:

```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# binwalk --dd=".*" art.png

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 1920 x 1080, 8-bit/color RGBA, non-interlaced
3408641       0x340301        PDF document, version: "1.5"
3408712       0x340348        Zlib compressed data, default compression
3412206       0x3410EE        Zlib compressed data, default compression
3419203       0x342C43        Zlib compressed data, default compression
3419994       0x342F5A        Zlib compressed data, default compression
3428883       0x345213        Zlib compressed data, default compression
3430685       0x34591D        Zip archive data, at least v2.0 to extract, compressed size: 217, uncompressed size: 573, name: _rels/.rels
3430943       0x345A1F        Zip archive data, at least v2.0 to extract, compressed size: 288, uncompressed size: 511, name: docProps/app.xml
3431277       0x345B6D        Zip archive data, at least v2.0 to extract, compressed size: 356, uncompressed size: 731, name: docProps/core.xml
3431680       0x345D00        Zip archive data, at least v2.0 to extract, compressed size: 222, uncompressed size: 663, name: word/_rels/document.xml.rels
3431960       0x345E18        Zip archive data, at least v2.0 to extract, compressed size: 165, uncompressed size: 208, name: word/settings.xml
3432172       0x345EEC        Zip archive data, at least v2.0 to extract, compressed size: 297, uncompressed size: 918, name: word/fontTable.xml
3432517       0x346045        Zip archive data, at least v2.0 to extract, compressed size: 83172, uncompressed size: 84725, name: word/media/image1.png
3515768       0x35A578        Zip archive data, at least v2.0 to extract, compressed size: 1138, uncompressed size: 4099, name: word/document.xml
3516953       0x35AA19        Zip archive data, at least v2.0 to extract, compressed size: 605, uncompressed size: 2192, name: word/styles.xml
3517603       0x35ACA3        Zip archive data, at least v2.0 to extract, compressed size: 352, uncompressed size: 1443, name: [Content_Types].xml
3518004       0x35AE34        Zip archive data, at least v1.0 to extract, compressed size: 20, uncompressed size: 20, name: not_the_flag.txt
3518847       0x35B17F        End of Zip archive, footer length: 22

root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# cd _art.png.extracted/
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted# file *
0:           PNG image data, 1920 x 1080, 8-bit/color RGBA, non-interlaced
340301:      PDF document, version 1.5
340348:      ASCII text, with very long lines
340348.zlib: Zip archive data, 3\213\356.p\200.\212\356z\333]\201\336M\377~-\276DJ\262\2229(\0160g&\222(\212\342\343#\305\0147w\371\357\307\177.\303e\270\015~\271\314\316\335\326\350.q\305\377\377\370\375\343\257\177\272\374\033g\034\377\376\370\327\307\347\317\217i\276-\227\350\307\313\317\177\\376\374\345.
3410EE:      TrueType Font data, 12 tables, 1st "cmap", 15 names, Microsoft, language 0x409, Copyright 2015 Google Inc. All Rights Reserved.Noto SansRegular2.000;GOOG;NotoSans-RegularNoto
3410EE.zlib: Zip archive data
342C43:      ASCII text
342C43.zlib: Zip archive data
342F5A:      TrueType Font data, 12 tables, 1st "cmap", 26 names, Macintosh, Copyright (c) 2003 by Bitstream, Inc. All Rights Reserved.
342F5A.zlib: Zip archive data, `r1!`
345213:      ASCII text
345213.zlib: Zip archive data
34591D:      Microsoft Word 2007+
35B17F:      Zip archive data (empty)
```

We have a PDF file, some text files, some zip files, some fonts and a Word file.

The text files are the easiest to look at, but nothing interesting is found there.

Let's peek into the zip files:

```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted# for file in `ls *.zlib`; do 7z l $file; done

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 110157 bytes (108 KiB)

Listing archive: 340348.zlib

--
Path = 340348.zlib
Type = zip
Offset = 21973
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 106663 bytes (105 KiB)

Listing archive: 3410EE.zlib

--
Path = 3410EE.zlib
Type = zip
Offset = 18479
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 99666 bytes (98 KiB)

Listing archive: 342C43.zlib

--
Path = 342C43.zlib
Type = zip
Offset = 11482
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 98875 bytes (97 KiB)

Listing archive: 342F5A.zlib

--
Path = 342F5A.zlib
Type = zip
Offset = 10691
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 89986 bytes (88 KiB)

Listing archive: 345213.zlib

--
Path = 345213.zlib
Type = zip
Offset = 1802
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files
```

They all look like word documents (a word document is actually just a zip file) but have different physical sizes. The content of the archive, though, seems to be similar to the other file identified as a Word file:

```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted# 7z l 34591D

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 88184 bytes (87 KiB)

Listing archive: 34591D

--
Path = 34591D
Type = zip
Physical Size = 88184

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
2018-09-20 21:00:00 .....          573          217  _rels/.rels
2018-09-20 21:00:00 .....          511          288  docProps/app.xml
2018-09-20 21:00:00 .....          731          356  docProps/core.xml
2018-09-20 21:00:00 .....          663          222  word/_rels/document.xml.rels
2018-09-20 21:00:00 .....          208          165  word/settings.xml
2018-09-20 21:00:00 .....          918          297  word/fontTable.xml
2018-09-20 23:11:40 .....        84725        83172  word/media/image1.png
2018-09-20 21:00:00 .....         4099         1138  word/document.xml
2018-09-20 21:00:00 .....         2192          605  word/styles.xml
2018-09-20 21:00:00 .....         1443          352  [Content_Types].xml
2018-09-20 23:05:40 .....           20           20  not_the_flag.txt
------------------- ----- ------------ ------------  ------------------------
2018-09-20 23:11:40              96083        86832  11 files
```

`binwalk` sometimes makes mistakes when extracting files, it might be reasonable to assume for now that the multiple zip files should in fact just have been the Word file.

Since the word file contains a file names "not_the_flag.txt", that looks like a good place to look.

```
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted# 7z e -o_34591D 34591D

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 88184 bytes (87 KiB)

Extracting archive: 34591D
--
Path = 34591D
Type = zip
Physical Size = 88184

Everything is Ok

Files: 11
Size:       96083
Compressed: 88184
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted# cd _34591D/
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted/_34591D# cat not_the_flag.txt
Sorry, no flag here
```

So indeed, no flag there.
We also have another PNG hiding in the file, let's inspect it:

```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted/_34591D# file image1.png
image1.png: PNG image data, 300 x 168, 8-bit colormap, non-interlaced
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted/_34591D# pngcheck image1.png
image1.png  additional data after IEND chunk
ERROR: image1.png
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/_art.png.extracted/_34591D# binwalk --dd=".*" image1.png

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 300 x 168, 8-bit colormap, non-interlaced
410           0x19A           Zlib compressed data, default compression
5253          0x1485          PDF document, version: "1.5"
5324          0x14CC          Zlib compressed data, default compression
5686          0x1636          JPEG image data, JFIF standard 1.01
75471         0x126CF         Zlib compressed data, default compression
83009         0x14441         Zlib compressed data, default compression
```

Now we have a PDF document and a JPEG file. The ZLIB files won't open correctly. The description mentions PDFs as the culprit's expertise.

The PDF file opens correctly, but Didier Stevens has a [tool](https://blog.didierstevens.com/2009/05/14/malformed-pdf-documents/) that can inspect PDF files and find malformed elements - these parts are printed with a "todo" prefix.


```console
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# mkdir pdfs
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files# cd pdfs
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/pdfs# cp _art.png.extracted/340301 .
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/pdfs# cp _art.png.extracted/_34591D/_image1.png.extracted/1485 .
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/pdfs# file *
1485:   PDF document, version 1.5
340301: PDF document, version 1.5
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/pdfs# python ~/utils/pdftools/pdf-parser.py -v 1485 | grep "todo"
todo 10: 3 'ZmxhZ3tQMGxZdEByX0QwX3kwdV9HM3RfSXRfTjB3P30K'
```

This actually looks like base64:

```
root@kali:/media/sf_CTFs/tamu/I_heard_you_like_files/pdfs# python ~/utils/pdftools/pdf-parser.py -v 1485 | grep "todo" | awk '{ printf $4 }' | tr -d "'" | base64 -d
flag{P0lYt@r_D0_y0u_G3t_It_N0w?}
```