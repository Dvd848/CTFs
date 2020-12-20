# Anonymous
Category: Steganography

## Description
> **Blast from the Past challenges series**
> 
> In 2017, Anonymous hacked lectures and tutorials to promote the 2nd faculty party, they even hid a message in one of the frames...

A PNG file was attached.

## Solution

Running `zsteg` on the file reveals the flag:

```console
root@kali:/media/sf_CTFs/technion/Anonymous# zsteg anon.png
b1,g,lsb,xy         .. file: dBase III DBT, version number 0, next free block index 3183084812
b1,g,msb,xy         .. file: dBase III DBT, version number 0, next free block index 3177029680
b1,b,msb,xy         .. file: dBase III DBT, version number 0, next free block index 4196096609
b1,rgb,lsb,xy       .. file: dBase III DBT, version number 0, next free block index 2532068640
b2,g,lsb,xy         .. file: dBase III DBT, version number 0, next free block index 3148544642
b2,g,msb,xy         .. file: dBase III DBT, version number 0, next free block index 3713359681
b2,b,lsb,xy         .. file: dBase III DBT, version number 0, next free block index 3837465664
b2,rgba,lsb,xy      .. text: "cstechnion{d0_n0t_b3c0m3_4_m4ch1n3_maN}"
b3,r,lsb,xy         .. file: MacBinary, more flags 0x7, Mon Feb  6 08:28:16 2040 INVALID date, modified Mon Feb  6 08:28:16 2040, type '    ' "I$\222H$\022A\004\020H$\020I$"
b3,g,msb,xy         .. file: dBase III DBT, version number 0, next free block index 2454414338
b3,b,msb,xy         .. file: StarOffice Gallery theme l\0216h, 0 objects
b3,rgb,msb,xy       .. file: 0420 Alliant virtual executable not stripped
b4,r,lsb,xy         .. file: raw G3 (Group 3) FAX, byte-padded
b4,g,lsb,xy         .. text: "2211#\"21\""
```

The flag: `cstechnion{d0_n0t_b3c0m3_4_m4ch1n3_maN}`