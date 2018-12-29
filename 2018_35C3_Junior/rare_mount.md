# rare mount
Forensics

## Description:
> Little or big, we do not care!

A binary file was attached.

## Solution:

Let's inspect the binary:

```console
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# file ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin
ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin: data
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# binwalk ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JFFS2 filesystem, big endian

```

So this might be a JFFS2 filesystem.

Searching for a way to mount the filesystem, I found [this link](http://wiki.emacinc.com/wiki/Mounting_JFFS2_Images_on_a_Linux_PC).

Copied their script and tried it out:
```console
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# ./mount.sh ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin /mnt/m 256
17152+0 records in
17152+0 records out
8781824 bytes (8.8 MB, 8.4 MiB) copied, 1.28533 s, 6.8 MB/s
mount: /mnt/m: can't read superblock on /dev/mtdblock0.
```

Continued searching and found [this](https://www.digitalworldz.co.uk/threads/looking-inside-jffs2-images-on-a-linux-box.47718/):

> A linux utility called "jffs2dump" which is part of the "mtd"
package. If you don't have the mtd utilities in your Linux
system, do a search in google for "mtd". Download, build,
and install.
> 
> You will use jffs2dump to convert the big endian image to
an image that is of little endian. If you don't, then you
will not be able to "mount" the image later on.
> 
> To convert the original JFFS2 image (which is B.E), type the
following into your shell. Note the "$" should not be typed,
it is only used to indicate the shell prompt.
> 
> $ jffs2dump -r -e /tmp/converted.img -b /tmp/original.img
> 
> Change the above command as necessary to reflect where your
own images are stored.
> 
> Note you will see several screens of errors/warnings - just
ignore these.

Let's try:
```console
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# jffs2dump -r -e rare-fs-le.bin -b ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# ./mount.sh rare-fs-le.bin /mnt/m 256
17152+0 records in
17152+0 records out
8781824 bytes (8.8 MB, 8.4 MiB) copied, 1.28779 s, 6.8 MB/s
Successfully mounted rare-fs-le.bin on /mnt/m
```

It worked!

Now, let's take a look at the contents:
```console
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# ls /mnt/m
flag  RickRoll_D-oHg5SJYRHA0.mkv
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# cat /mnt/m/flag
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
35C3_big_or_little_1_dont_give_a_shizzle
root@kali:/media/sf_CTFs/35c3ctf/rare_mount# umount /mnt/m
```

The flag: 35C3_big_or_little_1_dont_give_a_shizzle