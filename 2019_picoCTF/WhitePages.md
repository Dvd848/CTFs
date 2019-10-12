# WhitePages
Forensics, 250 points

## Description:
> I stopped using YellowPages and moved onto WhitePages... but the page they gave me is all blank!

The following file was attached (yes, it's composed of whitespaces):

```
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
```

## Solution: 

If we inspect the file using a HEX editor, we can see that there are two types of whitespaces:

```console
root@kali:/media/sf_CTFs/pico/WhitePages# xxd -g 1 whitepages.txt  | head
00000000: e2 80 83 e2 80 83 e2 80 83 e2 80 83 20 e2 80 83  ............ ...
00000010: 20 e2 80 83 e2 80 83 e2 80 83 e2 80 83 e2 80 83   ...............
00000020: 20 e2 80 83 e2 80 83 20 e2 80 83 e2 80 83 e2 80   ...... ........
00000030: 83 e2 80 83 20 e2 80 83 e2 80 83 20 e2 80 83 20  .... ...... ...
00000040: 20 20 e2 80 83 e2 80 83 e2 80 83 e2 80 83 e2 80    ..............
00000050: 83 20 20 e2 80 83 20 e2 80 83 e2 80 83 20 e2 80  .  ... ...... ..
00000060: 83 20 20 e2 80 83 e2 80 83 e2 80 83 20 20 e2 80  .  .........  ..
00000070: 83 20 20 e2 80 83 20 20 20 20 e2 80 83 20 e2 80  .  ...    ... ..
00000080: 83 e2 80 83 e2 80 83 e2 80 83 20 20 e2 80 83 20  ..........  ...
00000090: e2 80 83 20 e2 80 83 20 e2 80 83 e2 80 83 e2 80  ... ... ........
```

We have the standard space (`0x20`), and the Unicode `EM SPACE` (`U+2003` / `0xE2 0x80 0x83`).

Since we have only two options, let's try to treat them as binary.

```python
from pwn import *

with open("whitepages.txt", "rb") as bin_file:
    data = bytearray(bin_file.read()) 
    data = data.replace(b'\xe2\x80\x83', b'0')
    data = data.replace(b'\x20', b'1')
    data = data.decode("ascii")
    print unbits(data)
```

Output:
```console
root@kali:/media/sf_CTFs/pico/WhitePages# python solve.py

        picoCTF

        SEE PUBLIC RECORDS & BACKGROUND REPORT
        5000 Forbes Ave, Pittsburgh, PA 15213
        picoCTF{not_all_spaces_are_created_equal_f006c045f6b402ce4bc749dc7a262380}
```