# LoadSomeBits
Forensics, 500 points

## Description:
> Can you find the flag encoded inside this image?

![logo](images/pico2018-special-logo.bmp)

## Solution:

Couldn't find the flag by using any existing Steganography tool (e.g. zsteg, StegSolve).

I ended up writing this script which skips the BMP header and then creates a payload byte from the LSB of each 8 consecutive image bytes:
```python
import os
import mmap

BMP_HEADER_SIZE = 54
BITS_PER_BYTE = 8

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDWR)
    return mmap.mmap(fd, size, access=access)

with memory_map("pico2018-special-logo.bmp") as b:
    for i in range(BMP_HEADER_SIZE,
                   len(b) - BMP_HEADER_SIZE - BITS_PER_BYTE,
                   BITS_PER_BYTE):
        chunk = b[i:i+BITS_PER_BYTE]
        new_byte = 0
        for x, byte in enumerate(chunk):
            new_byte |= (byte & 0x01) << (BITS_PER_BYTE - x - 1)
        c = chr(new_byte)
        if new_byte == 0:
            break
        print (c, end='')
```

The output:
```
$ python3 solve.py
picoCTF{st0r3d_iN_tH3_l345t_s1gn1f1c4nT_b1t5_882756901}
```

The flag: picoCTF{st0r3d_iN_tH3_l345t_s1gn1f1c4nT_b1t5_882756901}