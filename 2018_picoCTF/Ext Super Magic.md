# Ext Super Magic
Forensics, 250 points

## Description:
> We salvaged a ruined Ext SuperMagic II-class mech recently and pulled the filesystem out of the black box. It looks a bit corrupted, but maybe there's something interesting in there.

Attached was a binary file.

### Hints:

* Are there any [tools](https://en.wikipedia.org/wiki/Fsck) for diagnosing corrupted filesystems? What do they say if you run them on this one?
* How does a linux machine know what [type](https://www.garykessler.net/library/file_sigs.html) of file a [file](https://linux.die.net/man/1/file) is?
* You might find this [doc](http://www.nongnu.org/ext2-doc/ext2.html) helpful.
* Be careful with [endianness](https://en.wikipedia.org/wiki/Endianness) when making edits.
* Once you've fixed the corruption, you can use /sbin/[debugfs](https://linux.die.net/man/8/debugfs) to pull the flag file out.





## Solution:

The first thing to do is to run `file` on the binary and see if it can identify what we have:
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# file ext-super-magic.img
ext-super-magic.img: data
```

Not very helpful.

How about `foremost`?
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# foremost ext-super-magic.img
Processing: ext-super-magic.img
|*|
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# ls output/jpg/
00001026.jpg  00002234.jpg  00002522.jpg  00002734.jpg  00003038.jpg  00003392.jpg  00003792.jpg  00004230.jpg  00004580.jpg  00008630.jpg
00001944.jpg  00002252.jpg  00002594.jpg  00002778.jpg  00003060.jpg  00003410.jpg  00003880.jpg  00004438.jpg  00005360.jpg  00008670.jpg
00002068.jpg  00002376.jpg  00002678.jpg  00002810.jpg  00003084.jpg  00003678.jpg  00004044.jpg  00004478.jpg  00005730.jpg  00008938.jpg
00002222.jpg  00002502.jpg  00002692.jpg  00002916.jpg  00003276.jpg  00003702.jpg  00004104.jpg  00004536.jpg  00008196.jpg  00009054.jpg
```

Foremost is able to extract a few JPEGs, but they are all corrupted.

After trying out a few things, I hit a wall and checked the hints.

The first hint directs us to run `Fsck`:
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# fsck ext-super-magic.img
fsck from util-linux 2.31.1
Usage: fsck.ext4 [-panyrcdfktvDFV] [-b superblock] [-B blocksize]
                [-l|-L bad_blocks_file] [-C fd] [-j external_journal]
                [-E extended-options] [-z undo_file] device
```

The third hint sent us to the ext2 specs, so I called `fsck.ext2`:
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# fsck.ext2 ./ext-super-magic.img
e2fsck 1.44.1 (24-Mar-2018)
ext2fs_open2: Bad magic number in super-block
fsck.ext2: Superblock invalid, trying backup blocks...
fsck.ext2: Bad magic number in super-block while trying to open ./ext-super-magic.img

The superblock could not be read or does not describe a valid ext2/ext3/ext4
filesystem.  If the device is valid and it really contains an ext2/ext3/ext4
filesystem (and not swap or ufs or something else), then the superblock
is corrupt, and you might try running e2fsck with an alternate superblock:
    e2fsck -b 8193 <device>
 or
    e2fsck -b 32768 <device>
```

According to the tool, the "super block" is corrupted. The tool suggest trying some alternative superblocks but they don't work:
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# fsck.ext2 -b 8193 ./ext-super-magic.img
e2fsck 1.44.1 (24-Mar-2018)
fsck.ext2: Attempt to read block from filesystem resulted in short read while trying to open ./ext-super-magic.img
Could this be a zero-length partition?
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# fsck.ext2 -b 32768 ./ext-super-magic.img
e2fsck 1.44.1 (24-Mar-2018)
fsck.ext2: Attempt to read block from filesystem resulted in short read while trying to open ./ext-super-magic.img
Could this be a zero-length partition?
```

What's a superblock?
According to the [specs](http://www.nongnu.org/ext2-doc/ext2.html#SUPERBLOCK):
> The superblock is always located at byte offset 1024 from the beginning of the file, block device or partition formatted with Ext2 and later variants (Ext3, Ext4). 

Let's try to visualize the EXT2 filesystem with [kaitai](http://formats.kaitai.io/):
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# ksv ext-super-magic.img ~/utils/kaitai_struct_formats/filesystem/ext2.ksy
```

When trying to browse the structure with kaitai, an error is displayed:
```
Unexpected fixed contents: got 00 00, was waiting for 53 ef
```

Searching for this constant in the [specs](http://www.nongnu.org/ext2-doc/ext2.html#S-MAGIC), we find the following:
> 3.1.16. s_magic
> 
> 16bit value identifying the file system as Ext2. The value is currently fixed to EXT2_SUPER_MAGIC of value 0xEF53. 

s_magic appears in table 3-3 (SuperBlock Structure):
```
Offset (bytes)	Size (bytes)	Description
56	        2	        s_magic 
```

So at offset 1024 + 56, we should find the value of 0xef53, but instead we have 0x0000. Let's patch the file with `dd`:

```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# xxd -s $((1024+56)) -l 16 ext-super-magic.img
00000438: 0000 0100 0100 0000 74dc ad5b 0000 0000  ........t..[....
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# cp ext-super-magic.img ext-super-magic.old
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# printf '\x53\xef' | dd conv=notrunc of=ext-super-magic.img bs=1 seek=$((1024+56))
2+0 records in
2+0 records out
2 bytes copied, 0.00398822 s, 0.5 kB/s
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# xxd -s $((1024+56)) -l 16 ext-super-magic.img
00000438: 53ef 0100 0100 0000 74dc ad5b 0000 0000  S.......t..[....
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# file ext-super-magic.img
ext-super-magic.img: Linux rev 1.0 ext2 filesystem data, UUID=aa043d2f-4828-4073-8dfe-f13acc388fdc (large files)
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2#
```

Now we can use `debugfs` to extract the file:
```console
root@kali:/media/sf_CTFs/pico/Ext_Super_Magic/v2# debugfs ext-super-magic.img
debugfs 1.44.1 (24-Mar-2018)
debugfs:  ls
 2  (12) .    2  (1012) ..    26  (24) filler-348.jpg
 84  (24) filler-303.jpg    103  (24) filler-18.jpg
 107  (24) filler-434.jpg    169  (20) filler-6.jpg    182  (16) flag.jpg
 183  (24) filler-47.jpg    187  (24) filler-326.jpg
 188  (24) filler-290.jpg    191  (24) filler-118.jpg
 194  (24) filler-246.jpg    198  (24) filler-112.jpg
 201  (24) filler-140.jpg    212  (24) filler-41.jpg
 214  (24) filler-232.jpg    229  (24) filler-90.jpg
[...]
debugfs:  dump flag.jpg flag.jpg
debugfs:  q
```

The JPEG file is not corrupted anymore:
![flag](images/ext-super-magic.jpg)

The flag: picoCTF{a7DB29eCf7dB9960f0A19Fdde9d00Af0}


