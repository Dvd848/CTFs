# SmellyOnion
Misc., 10 points

## Description
> I cry when I cut onions.

An archive file was provided.

## Solution

Let's extract the provided archive file:
```console
root@kali:/media/sf_CTFs/kaf/SmellyOnion# file smelly-onion.rar
smelly-onion.rar: RAR archive data, v5
root@kali:/media/sf_CTFs/kaf/SmellyOnion# mkdir temp
root@kali:/media/sf_CTFs/kaf/SmellyOnion# cp smelly-onion.rar temp
root@kali:/media/sf_CTFs/kaf/SmellyOnion# cd temp
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# 7z x smelly-onion.rar

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 3475 bytes (4 KiB)

Extracting archive: smelly-onion.rar
--
Path = smelly-onion.rar
Type = Rar5
Physical Size = 3475
Solid = -
Blocks = 1
Encrypted = -
Multivolume = -
Volumes = 1

Everything is Ok

Size:       3405
Compressed: 3475
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# ls
onion  smelly-onion.rar
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# file onion
onion: Zip archive data, at least v2.0 to extract
```

We get a zip file. Let's try to extract it as well:
```console
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# 7z x onion

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,1 CPU Intel(R) Core(TM) i5-4330M CPU @ 2.80GHz (306C3),ASM,AES-NI)

Scanning the drive for archives:
1 file, 3405 bytes (4 KiB)

Extracting archive: onion
--
Path = onion
Type = zip
Physical Size = 3405
Comment = 4b

Everything is Ok

Size:       3348
Compressed: 3405
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# ls
80  onion  smelly-onion.rar
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# file 80
80: Zip archive data, at least v2.0 to extract
```

We get another zip file. If we extract it - you guessed correctly, we get another one and so on.

The trivial way to proceed is to extract all layers of this onion and inspect the files. However, at the end of the road we find an empty file, so clearly we need to search elsewhere.

How about the zip file comments?

```console
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# 7z l onion | grep -i comment
Comment = 4b
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# python -c "print chr(0x4b)"
K
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# 7z l 80 | grep -i comment
Comment = 41
root@kali:/media/sf_CTFs/kaf/SmellyOnion/temp# python -c "print chr(0x41)"
A
```

We can see that the first two comments, interpreted as a hex value representing an ASCII character, are `K` and `A`: The flag format!

Using the following script, we can extract all of the comments from the files:

```python
from zipfile import ZipFile
from StringIO import StringIO

input_zip = "smelly-onion/onion"
flag = ""
while True:
    zip=ZipFile(input_zip)
    zip_files = zip.namelist()
    assert(len(zip_files) < 2)
    if len(zip_files) == 0:
        break
    internal_file_name = zip_files[0]
    internal_file_contents = zip.read(internal_file_name)
    flag += chr(int(zip.comment, 16))
    input_zip = zipdata = StringIO(internal_file_contents)

print flag
```

Output:
```console
root@kali:/media/sf_CTFs/kaf/SmellyOnion# python solve.py
KAF{21P_PH1L32_R_AW3S0M3_D0NT_Y0U_TH1NK}
```