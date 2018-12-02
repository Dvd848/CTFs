# Security by Obscurity
Misc.

## Description:
> Reading the contents of the screenshot you find that some guy named "John" created the firmware for the OffHub router and stored it on an iDropDrive cloud share. You fetch it and find "John" packed the firmware with an unknown key. Can you recover the package key?

A binary file was attached.

## Solution:

The attached file is a zip file:
```console
root@kali:/media/sf_CTFs/google/obscurety/new# file 2cdc6654fb2f8158cd976d8ffac28218b15d052b5c2853232e4c1bafcb632383
2cdc6654fb2f8158cd976d8ffac28218b15d052b5c2853232e4c1bafcb632383: Zip archive data, at least v2.0 to extract
```

Unzipping it reveals another zip file:
```console
root@kali:/media/sf_CTFs/google/obscurety/new# unzip 2cdc6654fb2f8158cd976d8ffac28218b15d052b5c2853232e4c1bafcb632383
Archive:  2cdc6654fb2f8158cd976d8ffac28218b15d052b5c2853232e4c1bafcb632383
 extracting: password.x.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.a.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p
root@kali:/media/sf_CTFs/google/obscurety/new# file password.x.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.a.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p
password.x.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p.p.o.n.m.l.k.j.i.h.g.f.e.d.c.b.a.a.b.c.d.e.f.g.h.i.j.k.l.m.n.o.p: Zip archive data, at least v2.0 to extract
```

Inside, we find another zip file, and later on we find archives of different formats.

The following script attempts to extract the files:
```python
#!/usr/bin/python3

import os
import shutil
import zipfile
import lzma
import bz2
import gzip
import time

OUTPUT_DIR = "output"

def extract_file(src_name, dest_name):
    print ("Extracting {}".format(src_name))
    algos = [zipfile.ZipFile, lzma.open, bz2.BZ2File, gzip.GzipFile]
    for algo in algos:
        try:
            with algo(os.path.join(OUTPUT_DIR, src_name)) as f:
                try:
                    c = f.read(f.infolist()[0]) # Zip file has different interface
                except:
                    c = f.read()
                with open(os.path.join(OUTPUT_DIR, dest_name), "wb") as o:
                    o.write(c)
            return True
        except Exception as e:
            pass
    return False


counter = 0

shutil.rmtree(OUTPUT_DIR, ignore_errors=True)
time.sleep(1)
os.mkdir(OUTPUT_DIR)
shutil.copy("2cdc6654fb2f8158cd976d8ffac28218b15d052b5c2853232e4c1bafcb632383", os.path.join(OUTPUT_DIR, str(counter)))

while extract_file(str(counter), str(counter + 1)):
    counter += 1
```

It extracts 65 files. Let's inspect that last extracted file:
```console
root@kali:/media/sf_CTFs/google/obscurety/new/output# file 65
65: Zip archive data, at least v1.0 to extract
root@kali:/media/sf_CTFs/google/obscurety/new/output# unzip 65
Archive:  65
[65] password.txt password: 
```

The zip file is password protected.

Let's use [John the Ripper](https://github.com/magnumripper/JohnTheRipper) to crack the password:

```console
root@kali:/media/sf_CTFs/google/obscurety/new# ~/utils/john/run/zip2john output/65 > zip.hashes
ver 1.0 efh 5455 efh 7875 65/password.txt PKZIP Encr: 2b chk, TS_chk, cmplen=44, decmplen=32, crc=4341BA5D
root@kali:/media/sf_CTFs/google/obscurety/new# cat zip.hashes
65/password.txt:$pkzip2$1*2*2*0*2c*20*4341ba5d*0*46*0*2c*4341*6eab*327fdc864aefd7fd5a7462f9355e15e84fc09d4e7d3a5ecd1318f77f7f6f2c86b62edc0a6d7eb87cba92a613*$/pkzip2$:password.txt:65::output/65
root@kali:/media/sf_CTFs/google/obscurety/new# ~/utils/john/run/john zip.hashes
Using default input encoding: UTF-8
Loaded 1 password hash (PKZIP [32/64])
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
asdf             (65/password.txt)
1g 0:00:00:00 DONE 2/3 (2018-11-26 22:27) 7.692g/s 101915p/s 101915c/s 101915C/s lacrosse..franklin
Use the "--show" option to display all of the cracked passwords reliably
Session completed
root@kali:/media/sf_CTFs/google/obscurety/new# ~/utils/john/run/john zip.hashes --show
65/password.txt:asdf:password.txt:65::output/65

1 password hash cracked, 0 left
```

The password is "asdf"!

Finally, we can unzip the file:
```console
root@kali:/media/sf_CTFs/google/obscurety/new# unzip -P asdf output/65
Archive:  output/65
 extracting: password.txt
root@kali:/media/sf_CTFs/google/obscurety/new# cat password.txt
CTF{CompressionIsNotEncryption}
```

The flag: CTF{CompressionIsNotEncryption}

## Appendix A: John the Ripper
The default `john` bundled with Kali (currently) provides the following error when attempting to crack the zip file's password:
```console
root@kali:/media/sf_CTFs/google/obscurety/new# john zip.hashes
Using default input encoding: UTF-8
No password hashes loaded (see FAQ)
```

This can be fixed by using the installing some extra packages and compiling from the sources:
```
apt-get install build-essential libssl-dev git
apt-get install yasm libgmp-dev libpcap-dev pkg-config libbz2-dev
git clone git://github.com/magnumripper/JohnTheRipper -b bleeding-jumbo john
cd john/src
./configure
make -s clean && make -sj4
../run/john --test=0
```