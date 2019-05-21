# Castles
Forensics, 150 points

## Description

> The flag might be in another castle.

A binary file was attached.

## Hint 1:

> If you are almost there, someone may have used STEG to HIDE something.

## Hint 2:

> If you've found the message for peach, it is a guide for where to find the two halves of the key, not the key itself. Think of how the four letters relate to the castles.

## Solution

Let's inspect the attached file:
```console
root@kali:/media/sf_CTFs/sunshine/Castles# file Castles.001
Castles.001: DOS/MBR boot sector, code offset 0x3c+2, OEM-ID "MSDOS5.0", sectors/cluster 4, reserved sectors 4, root entries 512, Media descriptor 0xf8, sectors/FAT 238, sectors/track 63, heads 255, sectors 243712 (volumes > 32 MB), serial number 0xa4c3fb1f, unlabeled, FAT (16 bit)
```

It's a FAT image file. 

```console
root@kali:/media/sf_CTFs/sunshine/Castles# fsck.vfat Castles.001
fsck.fat 4.1 (2017-01-24)
Castles.001: 7 files, 395/60800 clusters
root@kali:/media/sf_CTFs/sunshine/Castles# fsck.vfat Castles.001 -l
fsck.fat 4.1 (2017-01-24)
Checking file /System Volume Information (SYSTEM~1)
Checking file /Castelo_da_Feira.jpg (CASTEL~1.JPG)
Checking file /Inveraray_Castle.jpg (INVERA~1.JPG)
Checking file /Castello_Di_Amorosa.jpg (CASTEL~2.JPG)
Checking file /Spis_castle.jpg (SPIS_C~1.JPG)
Checking file /System Volume Information/.
Checking file /System Volume Information/..
Checking file /System Volume Information/WPSettings.dat (WPSETT~1.DAT)
Checking file /System Volume Information/IndexerVolumeGuid (INDEXE~1)
Castles.001: 7 files, 395/60800 clusters
```

According to `fsck`, it contains 7 files, 4 out of them seem to be JPG files.

```console
root@kali:/media/sf_CTFs/sunshine/Castles# debugfs Castles.001
debugfs 1.44.1 (24-Mar-2018)
Checksum errors in superblock!  Retrying...
Castles.001: Bad magic number in super-block while opening filesystem
Castles.001 contains a vfat file system
debugfs:  ls
ls: Filesystem not open
```

`debugfs` is unable to list the files, but we can try `fls` instead:

```console
root@kali:/media/sf_CTFs/sunshine/Castles# fls -r Castles.001
d/d 5:  System Volume Information
+ r/r 519:      WPSettings.dat
+ r/r 522:      IndexerVolumeGuid
r/r * 8:        Castelo_da_Feira.jpg
r/r 11: Castelo_da_Feira.jpg
r/r * 14:       Inveraray_Castle.jpg
r/r 17: Inveraray_Castle.jpg
r/r * 20:       Castello_Di_Amorosa.jpg
r/r 23: Castello_Di_Amorosa.jpg
r/r * 26:       spis-castle.jpg
r/r 29: Spis_castle.jpg
r/r * 32:       AlmostThere.jpg
v/v 3891715:    $MBR
v/v 3891716:    $FAT1
v/v 3891717:    $FAT2
V/V 3891718:    $OrphanFiles
```

This time we see five distinct JPG files! The `*` marks deleted files, and there's a file which only exists as a deleted item: `AlmostThere.jpg`.

We can try to extract the files using `icat`, specifying the inode ID:
```console
icat Castles.001 32 > AlmostThere.jpg
icat Castles.001 8 > Castelo_da_Feira.jpg
icat Castles.001 14 > Inveraray_Castle.jpg
icat Castles.001 20 > Castello_Di_Amorosa.jpg
icat Castles.001 26 > spis-castle.jpg
```

Note: `foremost` can extract the files as well.

Until here it's pretty obvious, this is where things get a bit harder, and where the hints come in handy.

> If you are almost there, someone may have used STEG to HIDE something.

This obviously means that `steghide` was used to hide a message in `AlmostThere.jpg`, but we'll need a passphrase to extract the message.

> If you've found the message for peach, it is a guide for where to find the two halves of the key, not the key itself. Think of how the four letters relate to the castles.

What's the message for peach?

```console
root@kali:/media/sf_CTFs/sunshine/Castles# grep peach Castles.001
Binary file Castles.001 matches
root@kali:/media/sf_CTFs/sunshine/Castles# strings Castles.001 | grep peach
peachPeachPEACH Hey! Mario said something about a hidden key. Hesaid this: F2I and A1S, and that it was in two pieces.
```

Ok, we got the message. How do the four letters relate to the castles? F is like Castelo_da_Feira, I is like Inveraray_Castle, A is like Castello_Di_Amorosa and S is like spis-castle. So the first half of the key is in between Amorosa and spis, and the second half between Feira and Inveraray?

Turns out the if we extract Castelo_da_Feira and Castello_Di_Amorosa **including the file slack**, we get two interesting strings hidden in the slack:
```
root@kali:/media/sf_CTFs/sunshine/Castles# icat -s Castles.001 23 > Castello_Di_Amorosa_slack.jpg
root@kali:/media/sf_CTFs/sunshine/Castles# strings Castello_Di_Amorosa_slack.jpg | tail -n 1
AQ273RFGHUI91O
root@kali:/media/sf_CTFs/sunshine/Castles# strings Castelo_da_Feira_slack.jpg | tail -n 1
LO987YTFGY78IK
```

Combine these two parts and we get the passphrase for `steghide`:

```console
root@kali:/media/sf_CTFs/sunshine/Castles# steghide extract -sf AlmostThere.jpg -p AQ273RFGHUI91OLO987YTFGY78IK
wrote extracted data to "flag.txt".
root@kali:/media/sf_CTFs/sunshine/Castles# cat flag.txt
flag{7H4NK5_F0R_PL4Y1NG}
```

