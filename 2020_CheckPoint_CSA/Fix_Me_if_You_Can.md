# Fix Me if You Can
Category: Misc., 50 Points

## Description

> Fix me if you can and you will be half way to find the flag.
> 
> Good Luck!

A `*.doc` was attached.

## Solution

The attached file has a `*.doc` extension, but `file` doesn't recognize it as a Word document:

```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# file Fix_me.doc
Fix_me.doc: data
```

Microsoft Word fails to open it as well. 

As the old saying goes, we can do this the easy way, or we can do this the hard way.

## The Hard Way

In this path, we'll attempt to fix the file. We'll need to work with the file specs.

A good place to start is to try and understand why `file` doesn't recognize it as a Word file.

`file` works with magic file signature, so we should start by trying to identify the signature `file` looks for in `doc` files.

From a quick search we find that:
> Microsoft Word documents of version 97-2003 use the OLE Compound File (OLECF). These files therefore have the OLECF file signature. ([Source](https://forensicswiki.xyz/wiki/index.php?title=Word_Document_(DOC)))

We continue by searching for the OLECF file signature ([Source](https://forensicswiki.xyz/wiki/index.php?title=OLE_Compound_File)):

> The OLECF has the following file signature (as a hexadecimal byte sequence):
> 
> `d0 cf 11 e0 a1 b1 1a e1`

Let's compare to what we have:

```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# xxd -g 1 -l 8 Fix_me.doc
00000000: d0 cf 11 e0 a1 b1 1a 1e                          ........
```

Similar, but not identical. The last byte is `0x1e`, but it should be `0xe1`. 

Let's fix it:

```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# cp Fix_me.doc Fixed_me.doc
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# printf '\xe1' | dd conv=notrunc of=Fixed_me.doc bs=1 seek=7
1+0 records in
1+0 records out
1 byte copied, 0.00159627 s, 0.6 kB/s
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# xxd -g 1 -l 8 Fixed_me.doc
00000000: d0 cf 11 e0 a1 b1 1a e1                          ........
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# file Fixed_me.doc
Fixed_me.doc: Composite Document File V2 Document, Little Endian, Os: Windows, Version 10.0, Code page: 1252, Title: Word Template, Author: Kobi Eisenkraft, Template: Normal.dotm, Last Saved By: Kobi Eisenkraft, Revision Number: 6, Name of Creating Application: Microsoft Office Word, Total Editing Time: 30:00, Last Printed: Sat Jan 30 01:17:00 2010, Create Time/Date: Sun Apr 12 23:28:00 2020, Last Saved Time/Date: Wed Apr 29 21:31:00 2020, Number of Pages: 1, Number of Words: 256, Number of Characters: 1460, Security: 0
```

In our case, this was enough to fix the document, and we can now open it with Word. It contains some text and an image.

## The Easy Way

In this path, we'll just attempt to extract any data we can from the file without really fixing it.

Using `strings`, we can extract some text from the file:
```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# strings Fix_me.doc
bjbjj
Hello,
R U looking for the flag? (-:
You probably succeeded to fix me, now you should find me.
I hope U R in the right direction, keep going
Good Luck!
Maybe
she
will
help
```

Using `foremost`, we can extract any embedded files:
```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# foremost Fix_me.doc
Processing: Fix_me.doc
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# cat output/audit.txt
Foremost version 1.5.7 by Jesse Kornblum, Kris Kendall, and Nick Mikus
Audit File

Foremost started at Fri May  8 13:44:10 2020
Invocation: foremost Fix_me.doc
Output directory: /media/sf_CTFs/checkpoint/Fix_Me_if_You_Can/output
Configuration file: /etc/foremost.conf
------------------------------------------------------------------
File: Fix_me.doc
Start: Fri May  8 13:44:10 2020
Length: 87 KB (89088 bytes)

Num      Name (bs=512)         Size      File Offset     Comment

0:      00000125.zip          18 KB           64242
1:      00000067.png          19 KB           34401       (105 x 160)
2:      00000106.png          156 B           54519       (15 x 15)
Finish: Fri May  8 13:44:10 2020

3 FILES EXTRACTED

zip:= 1
png:= 2
------------------------------------------------------------------

Foremost finished at Fri May  8 13:44:10 2020
```

We were able to extract the same information as in the hard way in a fraction of the time.

## Analyzing the Image

Let's take a look at the image:

![](images/00000067.png)

Who is this? [Google Images Reverse Search](https://images.google.com) tells us that this is [Velvalee Dickinson](https://en.wikipedia.org/wiki/Velvalee_Dickinson):

> Velvalee Dickinson (October 12, 1893 – ca 1980), was convicted of espionage against the United States on behalf of Japan during World War II. Known as the "Doll Woman", she used her business in New York City to send information on the United States Navy to contacts in Argentina via steganographic messages.

This is a very thick hint that we should be looking for Steganography. Let's run `zsteg` on the image:

```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# zsteg output/png/00000067.png
imagedata           .. text: "+++444***"
b1,rgb,lsb,xy       .. text: "48:UTFOQmUxTjBNemxoVGpCZklXNWZaREJqWHpGelgwNHhZek45"
b2,rgb,msb,xy       .. text: "jUUUUUUUUUUU"
b2,bgr,msb,xy       .. text: "jUUUUUUUUUUU"
b4,r,msb,xy         .. text: "{wwwwwww"
b4,g,msb,xy         .. text: "{wwwwwww"
b4,b,msb,xy         .. file: MPEG ADTS, layer I, v2, 112 kbps, 24 kHz, JntStereo
```

The second string looks like Base64:
```console
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# echo "UTFOQmUxTjBNemxoVGpCZklXNWZaREJqWHpGelgwNHhZek45" | base64 -d && echo
Q1NBe1N0MzlhTjBfIW5fZDBjXzFzX04xYzN9
root@kali:/media/sf_CTFs/checkpoint/Fix_Me_if_You_Can# echo "UTFOQmUxTjBNemxoVGpCZklXNWZaREJqWHpGelgwNHhZek45" | base64 -d | base64 -d && echo
CSA{St39aN0_!n_d0c_1s_N1c3}
```