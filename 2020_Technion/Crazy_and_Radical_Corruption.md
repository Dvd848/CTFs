# Crazy and Radical Corruption
Category: Forensics

## Description
> Corruption can be critical, but sometimes it can be detected and fixed (unlike Ducky's super secret hacker attacks 🦆).

A zip file was attached.

## Solution

From the challenge title we can already guess that we will be dealing with CRC mismatches. However, it's not very clear what we were supposed to do in this challenge since it was possible to extract the flag without problems.

We get a ZIP file which is corrupted:

```console
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# unzip -t crazy_and_radical_corruption.zip
Archive:  crazy_and_radical_corruption.zip
    testing: crazy_and_radical_corruption.png   bad CRC b424e46f  (should be 00000000)
At least one error was detected in crazy_and_radical_corruption.zip.
```

Even though the CRC is incorrect, modern tools can successfully extract the contents without a problem. However, for the sake of the writeup, we'll run a command to try and fix the mismatch:

```console
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# zip -F crazy_and_radical_corruption.zip --out new.zip
Fix archive (-F) - assume mostly intact archive
Zip entry offsets do not need adjusting
 copying: crazy_and_radical_corruption.png
        zip warning: Local Entry CRC does not match CD: crazy_and_radical_corruption.png
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# unzip -t new
Archive:  new.zip
    testing: crazy_and_radical_corruption.png   OK
No errors detected in compressed data of new.zip.
```

After unzipping the archive, we get a PNG file which is again corrupted:

```console
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# pngcheck crazy_and_radical_corruption.png
crazy_and_radical_corruption.png  CRC error in chunk IDAT (computed 8c4c671d, expected 00000000)
ERROR: crazy_and_radical_corruption.png
```

However, again - the flag is visible enough and can be extracted without further actions. For the sake of the writeup, we'll fix it too.

[PCRT](https://github.com/sherlly/PCRT) was unable to fix the PNG but [PNGCSum](http://schaik.com/png/pngcsum.html) did it without a problem:

```console
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# ~/utils/pngcsum/pngcsum crazy_and_radical_corruption.png out.png
IHDR ( 13 ) - csum = 139de5a2
sRGB (  1 ) - csum = aece1ce9
gAMA (  4 ) - csum = 0bfc6105
pHYs (  9 ) - csum = c76fa864
IDAT (65445 ) - csum = 00000000 -> 8c4c671d
IDAT (45774 ) - csum = 8cffddca
IEND (  0 ) - csum = ae426082
root@kali:/media/sf_CTFs/technion/Crazy_and_Radical_Corruption# pngcheck out.png
OK: out.png (2460x1259, 32-bit RGB+alpha, non-interlaced, 99.1%).
```

The flag: `cstechnion{fixiNg_errors_1n_Error_De7ec7ion}`