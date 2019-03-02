# Journey to the Center of the File
Grab Bag, 100 points

## Description:

> W(e( (h(a(v(e( (t(o( (g(o( (d(e(e(p(e(r)))))))))))))))))))

A file was attached.

## Solution:

Let's inspect the file:

```console
# file flag
flag: bzip2 compressed data, block size = 400k
```

Extract it and inspect the output:

```console
root@kali:/media/sf_CTFs/mitre/Center_of_File# 7z x flag

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 4681715 bytes (4572 KiB)

Extracting archive: flag
--
Path = flag
Type = bzip2

Everything is Ok

Size:       4655443
Compressed: 4681715
root@kali:/media/sf_CTFs/mitre/Center_of_File# ls
flag  flag~
root@kali:/media/sf_CTFs/mitre/Center_of_File# file flag~
flag~: Zip archive data, at least v2.0 to extract
```

Extract that:
```console
root@kali:/media/sf_CTFs/mitre/Center_of_File# 7z x flag~

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 4655443 bytes (4547 KiB)

Extracting archive: flag~
--
Path = flag~
Type = zip
Physical Size = 4655443


Would you like to replace the existing file:
  Path:     ./flag
  Size:     4681715 bytes (4572 KiB)
  Modified: 2019-01-22 23:13:30
with the file from archive:
  Path:     flag
  Size:     4654627 bytes (4546 KiB)
  Modified: 2018-11-29 12:43:16
? (Y)es / (N)o / (A)lways / (S)kip all / A(u)to rename all / (Q)uit? u

Everything is Ok

Size:       4654627
Compressed: 4655443
root@kali:/media/sf_CTFs/mitre/Center_of_File# ls
flag  flag~  flag_1
root@kali:/media/sf_CTFs/mitre/Center_of_File# file flag_1
flag_1: bzip2 compressed data, block size = 400k
```

We have a pattern here. Let's write a Python script to recursively extract any file we encounter:
```python
import os
import time
import shutil
import subprocess

WORKING_DIR = "out"
filename = "flag"

if os.path.exists(WORKING_DIR):
    shutil.rmtree(WORKING_DIR)
    time.sleep(1)

os.mkdir(WORKING_DIR)
shutil.copy(filename, WORKING_DIR)
os.chdir(WORKING_DIR)

while True:
    print ("Processing '{}'".format(filename))
    new_filename = "{}_{}".format(filename, int(time.time()))
    os.rename(filename, new_filename)
    filename = new_filename
    file_info = subprocess.check_output(["file", filename], encoding='ASCII')
    remove_original = False
    if "zip" in file_info.lower():
        subprocess.call(["7z", "x", filename], stderr=subprocess.DEVNULL, stdout=subprocess.DEVNULL)
        remove_original = True
    elif "ASCII text" in file_info:
        with open (filename) as f:
            contents = f.read()
        if "MCA{" in contents:
            print (contents)
            break
        else:
            with open("{}_new".format(filename), "wb", 0) as out:
                subprocess.call(["base64", "-d", filename], stdout=out)
            #os.system("base64 -d {f} > {f}.new".format(f = filename))
            remove_original = True
    else:
        print ("Unhandled format:")
        print (file_info)
        break

    if remove_original:
        os.remove(filename)
        filename = subprocess.check_output(["ls"], encoding='ASCII').rstrip()
```

Running it produces:
```console
[...]
Processing 'flag_1551555318_new'
Processing 'flag'
Processing 'flag_1551555318~'
Processing 'flag_1551555318~_1551555318_new'
Processing 'flag_1551555318~_1551555318_new_1551555318~'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~_1551555318_new'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~_1551555318_new_1551555318~'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~_1551555318_new_1551555318~_1551555318_new'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~_1551555318_new_1551555318~_1551555318_new_1551555318~'
Processing 'flag_1551555318~_1551555318_new_1551555318~_1551555318~_1551555318~_1551555318_new_1551555318~_1551555318_new_1551555318~_1551555318_new'
MCA{Wh0_Needz_File_Extensions?}
```

