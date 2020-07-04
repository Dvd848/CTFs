# MoreWithLess
* Category: Forensics
* 200 Points
* Solved by the JCTF Team

## Description
> You need some EFF in your life.

A 8GB file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# file MoreWithLess.E01
MoreWithLess.E01: EWF/Expert Witness/EnCase image file format
```

Same as [Check yourself so you won't wreck yourself](Check_yourself_so_you_wont_wreck_yourself.md). Let's try the same thing again:

```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# mkdir rawimage
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# ewfmount MoreWithLess.E01 ./rawimage/
ewfmount 20140807

root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# fdisk -l rawimage/ewf1
Disk rawimage/ewf1: 29.4 GiB, 31534870528 bytes, 61591544 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: dos
Disk identifier: 0x73736572

Device          Boot      Start        End    Sectors   Size Id Type
rawimage/ewf1p1      1920221984 3736432267 1816210284   866G 72 unknown
rawimage/ewf1p2      1936028192 3889681299 1953653108 931.6G 6c unknown
rawimage/ewf1p3               0          0          0     0B  0 Empty
rawimage/ewf1p4        27722122   27722568        447 223.5K  0 Empty

Partition table entries are not in disk order.
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# mkdir mountpoint
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# mount ./rawimage/ewf1 ./mountpoint -o ro,loop,show_sys_files,streams_interace=windows,offset=$((1936028192*512))
mount: /media/sf_CTFs/bsidestlv/MoreWithLess/mountpoint: wrong fs type, bad option, bad superblock on /dev/loop0, missing codepage or helper program, or other error.
```

Trying the different start offsets didn't help, the same error always appeared.

We can try to use `fls` to view all the files:

```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# fls -rp rawimage/ewf1 | head
r/r 4-128-1:    $AttrDef
r/r 8-128-2:    $BadClus
r/r 8-128-1:    $BadClus:$Bad
r/r 6-128-4:    $Bitmap
r/r 6-128-5:    $Bitmap:$SRAT
r/r 7-128-1:    $Boot
d/d 11-144-4:   $Extend
d/d 29-144-6:   $Extend/$Deleted
-/r * 124435-128-4:     $Extend/$Deleted/001F00000001E6137BC9339E
r/r 25-144-5:   $Extend/$ObjId:$O
```

If we find an interesting file, we can `cat` it with `icat`. An example for a not-so-interesting file:
```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# fls -rp rawimage/ewf1 | grep -i  Users/bsidestlv2020/Desktop
d/d 89991-144-6:        Users/bsidestlv2020/Desktop
r/r 90426-128-1:        Users/bsidestlv2020/Desktop/desktop.ini
r/r 91140-128-4:        Users/bsidestlv2020/Desktop/Microsoft Edge.lnk
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# icat rawimage/ewf1 90426-128-1

 [ . S h e l l C l a s s I n f o ]
 L o c a l i z e d R e s o u r c e N a m e = @ % S y s t e m R o o t % \ s y s t e m 3 2 \ s h e l l 3 2 . d l l , - 2 1 7 6 9
 I c o n R e s o u r c e = % S y s t e m R o o t % \ s y s t e m 3 2 \ i m a g e r e s . d l l , - 1 8 3
```

However it's hard to search that way. It's much easier to use `tsk_recover` to try and dump the contents of the image to the disk and search there:

```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# mkdir recovered
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess# tsk_recover -e ./rawimage/ewf1 ./recovered/
```

We can now grep for the flag:
```console
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess/recovered# grep -ri "BSidesTLV2020{"
Binary file pagefile.sys matches
^C
root@kali:/media/sf_CTFs/bsidestlv/MoreWithLess/recovered# strings pagefile.sys | grep -i "BSidesTLV2020{"
BSIDESTLV2020{
BSIDESTLV2020{HideYourNeed
VBSIDESTLV2020{HideYou
BSIDESTLV2020{HideYourNeedleInTheHayStack}
^C
```