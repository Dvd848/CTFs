# Floppy
Misc.

## Description:
> Using the credentials from the letter, you logged in to the Foobanizer9000-PC. It has a floppy drive...why? There is an .ico file on the disk, but it doesn't smell right..

A binary file was attached.

## Solution:

Let's inspect the attached file:
```console
root@kali:/media/sf_CTFs/google/floppy# file foo.ico
foo.ico: MS Windows icon resource - 1 icon, 32x32, 16 colors
```

It looks like a harmless icon file... or does it?
```console
root@kali:/media/sf_CTFs/google/floppy# binwalk foo.ico

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
765           0x2FD           Zip archive data, at least v2.0 to extract, compressed size: 123, uncompressed size: 136, name: driver.txt
956           0x3BC           Zip archive data, at least v2.0 to extract, compressed size: 214, uncompressed size: 225, name: www.com
1392          0x570           End of Zip archive, footer length: 22
```

Extract with `binwalk` and inspect:
```console
root@kali:/media/sf_CTFs/google/floppy# binwalk foo.ico -e

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
765           0x2FD           Zip archive data, at least v2.0 to extract, compressed size: 123, uncompressed size: 136, name: driver.txt
956           0x3BC           Zip archive data, at least v2.0 to extract, compressed size: 214, uncompressed size: 225, name: www.com
1392          0x570           End of Zip archive, footer length: 22

root@kali:/media/sf_CTFs/google/floppy# cd _foo.ico.extracted/
root@kali:/media/sf_CTFs/google/floppy/_foo.ico.extracted# cat
2FD.zip     driver.txt  www.com
root@kali:/media/sf_CTFs/google/floppy/_foo.ico.extracted# cat driver.txt
This is the driver for the Aluminum-Key Hardware password storage device.
     CTF{qeY80sU6Ktko8BJW}

In case of emergency, run www.com
```

The flag: CTF{qeY80sU6Ktko8BJW}