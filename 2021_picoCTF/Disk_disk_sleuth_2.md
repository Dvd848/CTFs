# Disk, disk, sleuth! II
Category: Forensics, 130 points

## Description
> All we know is the file with the flag is named `down-at-the-bottom.txt`...

A disk image was attached.

## Solution

If we simply try to `fsstat` the attached image in order to display the disk image details, we get an error:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth_2]
└─$ fsstat ./dds2-alpine.flag.img
Cannot determine file system type
```

That's because we need to provide it with the correct offset, which in this case isn't 0:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth_2]
└─$ mmls ./dds2-alpine.flag.img
DOS Partition Table
Offset Sector: 0
Units are in 512-byte sectors

      Slot      Start        End          Length       Description
000:  Meta      0000000000   0000000000   0000000001   Primary Table (#0)
001:  -------   0000000000   0000002047   0000002048   Unallocated
002:  000:000   0000002048   0000262143   0000260096   Linux (0x83)
```

As we can see, the image starts at offset `2048`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth_2]
└─$ fsstat ./dds2-alpine.flag.img -o 2048 | head -n 19
FILE SYSTEM INFORMATION
--------------------------------------------
File System Type: Ext3
Volume Name:
Volume ID: dc53a3bb0ae739a5164c89db56bbb12f

Last Written at: 2021-02-16 20:21:20 (IST)
Last Checked at: 2021-02-16 20:21:19 (IST)

Last Mounted at: 2021-02-16 20:21:19 (IST)
Unmounted properly
Last mounted on: /os/mnt

Source OS: Linux
Dynamic Structure
Compat Features: Journal, Ext Attributes, Resize Inode, Dir Index
InCompat Features: Filetype,
Read Only Compat Features: Sparse Super, Large File,
```

We use this offset for other `sleuthkit` commands as well, such as finding the requested file's inode:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth_2]
└─$ fls -r ./dds2-alpine.flag.img -o 2048 | grep down-at-the-bottom.txt
+ r/r 18291:    down-at-the-bottom.txt
```

And displaying its contents:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Disk_disk_sleuth_2]
└─$ icat ./dds2-alpine.flag.img -o 2048 18291
   _     _     _     _     _     _     _     _     _     _     _     _     _
  / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \
 ( p ) ( i ) ( c ) ( o ) ( C ) ( T ) ( F ) ( { ) ( f ) ( 0 ) ( r ) ( 3 ) ( n )
  \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/
   _     _     _     _     _     _     _     _     _     _     _     _     _
  / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \
 ( s ) ( 1 ) ( c ) ( 4 ) ( t ) ( 0 ) ( r ) ( _ ) ( n ) ( 0 ) ( v ) ( 1 ) ( c )
  \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/
   _     _     _     _     _     _     _     _     _     _     _
  / \   / \   / \   / \   / \   / \   / \   / \   / \   / \   / \
 ( 3 ) ( _ ) ( 0 ) ( b ) ( a ) ( 8 ) ( d ) ( 0 ) ( 2 ) ( d ) ( } )
  \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/   \_/
```

The flag: `picoCTF{f0r3ns1c4t0r_n0v1c3_0ba8d02d}`