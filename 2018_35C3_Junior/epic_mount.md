# epic mount
Forensics

## Description:
> A little bit of stego. Not every header field looks like the other.

A binary file was attached.

## Solution:

Let's inspect the binary:

```console
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# file f9be7cb88a778615216f212d58f62ea3-epic-fs.bin
f9be7cb88a778615216f212d58f62ea3-epic-fs.bin: data
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# binwalk f9be7cb88a778615216f212d58f62ea3-epic-fs.bin

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JFFS2 filesystem, big endian
```

Same as [rare mount](rare_mount.md).

What worked there was using `jffs2dump`, let's try it here too:
```console
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# jffs2dump -r -e epic-fs-le.bin -b f9be7cb88a778615216f212d58f62ea3-epic-fs.bin
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# ../rare_mount/mount.sh epic-fs-le.bin  /mnt/m 256
17152+0 records in
17152+0 records out
8781824 bytes (8.8 MB, 8.4 MiB) copied, 1.21051 s, 7.3 MB/s
Successfully mounted epic-fs-le.bin on /mnt/m
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# ls /mnt/m
RickRoll_D-oHg5SJYRHA0.mkv
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# umount /mnt/m
```

Just a [Rickroll](https://www.youtube.com/watch?v=dQw4w9WgXcQ).

What's the difference between the two file systems - the one from "rare mount", and this one?

```console
root@kali:/media/sf_CTFs/35c3ctf/epic_mount# cmp -l --print-bytes f9be7cb88a778615216f212d58f62ea3-epic-fs.bin ../rare_mount/ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin
   3665  63 3      0 ^@
  16157  65 5      0 ^@
  28605 103 C      0 ^@
  45241  63 3      0 ^@
  86797 137 _      0 ^@
 153501 150 h      0 ^@
 170157 151 i      0 ^@
 262177 144 d      0 ^@
 474269 145 e      0 ^@
 528485 137 _      0 ^@
 574289 155 m      0 ^@
 582617 145 e      0 ^@
 615929 137 _      0 ^@
 628421 142 b      0 ^@
 707577 141 a      0 ^@
 765873 142 b      0 ^@
 836805 171 y      0 ^@
 928541 137 _      0 ^@
1057705 157 o      0 ^@
1149313 156 n      0 ^@
1336853 145 e      0 ^@
1341017 137 _      0 ^@
5378145 155 m      0 ^@
5415613 157 o      0 ^@
5677953 162 r      0 ^@
5727893 145 e      0 ^@
5902877 137 _      0 ^@
6027865 164 t      0 ^@
6144449 151 i      0 ^@
6244413 155 m      0 ^@
8653041 145 e      0 ^@
```

That actually worked! Although, I suppose the organizers wanted us to find the flag by parsing the header.

Anyway, here's a Python script that does the same thing:
```python
import os
import mmap

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDONLY)
    return mmap.mmap(fd, size, access=access)

with memory_map("f9be7cb88a778615216f212d58f62ea3-epic-fs.bin") as f1, memory_map("../rare_mount/ffbde7acedff79aa36f0f5518aad92d3-rare-fs.bin") as f2:
    res = ""
    for i in range(len(f1)):
        if f1[i] != f2[i]:
            if f2[i] != 0:
                break
            res += chr(f1[i])

    print (res)

```

The flag: 35C3_hide_me_baby_one_more_time
