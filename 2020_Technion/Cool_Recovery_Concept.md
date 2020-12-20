# Cool Recovery Concept
Category: Forensics

## Description
> I received this image from my partner in Intro. to Computer Networks and it seems to be corrupted, can you fix it?

A PNG file was attached.

## Solution

From the challenge title we can already guess that we will be dealing with CRC mismatches. 

The PNG we've downloaded is corrupted, as confirmed by `pngcheck`:

```console
root@kali:/media/sf_CTFs/technion/Cool_Recovery_Concept# pngcheck cool_recovery_concept.png
cool_recovery_concept.png  invalid IHDR image dimensions (0x0)
ERROR: cool_recovery_concept.png
```

Looks like the image width and height are set to 0, which is invalid.

So, we'll try to search for the correct width and height values, checking ourselves against the IHDR CRC. When we find good values, we'll fix the image and write the result to a new file.

In order to abstract away the implementation details of a PNG file and concentrate on the high-level logic, we'll use a "toy" PNG module which supports more or less the bare minimum for this challenge.

First, the solution:

```python
from png import Png

with open("cool_recovery_concept.png", "rb") as f:
    image = Png(f)
    assert(image.ihdr.is_crc_valid() == False)
    print("Searching for width, height which will provide CRC value {}"
            .format(hex(image.ihdr.crc)))
    for h in range(2000):
        for w in range(2000):
            image.ihdr.body.height = h
            image.ihdr.body.width = w
            if image.ihdr.is_crc_valid():
                file_name = f"result_{w}x{h}.png"
                print(f"Found a match, writing to {file_name}")
                with open(file_name, "wb") as o:
                    o.write(image.to_bytes())
```

Output:
```console
root@kali:/media/sf_CTFs/technion/Cool_Recovery_Concept# python3 solve.py
Searching for width, height which will provide CRC value 0x831721d9
Found a match, writing to result_1328x995.png
```

The fix in the binary:
```console
root@kali:/media/sf_CTFs/technion/Cool_Recovery_Concept# xxd -g 1 cool_recovery_concept.png | head -2
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 00 00 00 00 00 00 08 06 00 00 00 83 17 21  ...............!
root@kali:/media/sf_CTFs/technion/Cool_Recovery_Concept# xxd -g 1 result_1328x995.png | head -2
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 05 30 00 00 03 e3 08 06 00 00 00 83 17 21  ...0...........!
```

The flag was in the image: `cstechnion{f1x1ng_3rror5_us1ng_3rror_d3t3ction}`.

The PNG module is brought here for completeness as well, however it is a half-baked toy implementation and should not be used for anything important.

```python
import struct, io, logging, zlib
from collections import namedtuple


class StructBase(object):
    def __init__(self):
        self._total_length = 0

    def read(self, iostream, format):
        length = struct.calcsize(format)
        self._total_length += length
        res = struct.unpack(format,  iostream.read(length))
        if len(res) != 1:
            raise ValueError("read() expects exactly one format entity")
        return res[0]

    def read_child(self, child):
        self._total_length += child.get_length()
        return child

    def get_length(self):
        return self._total_length

    def verify_length(self, expected_length):
        if self._total_length != expected_length:
            raise RuntimeError(f"Error: Expected {expected_length} bytes, got {self._total_length}")



class ChunkBodyIHDR(StructBase):
    TYPE = b'IHDR'

    _fields = {
        "width":                '>I',
        "height":               '>I',
        "bit_depth":            '>B',
        "color_type":           '>B',
        "compression_method":   '>B',
        "filter_method":        '>B',
        "interlace_method":     '>B',
    }

    def __init__(self, iostream, length):
        super().__init__()
        for name, format in self._fields.items():
            setattr(self, name, self.read(iostream, format))
        self.verify_length(length)

    def to_bytes(self):
        res = bytearray()
        for name, format in self._fields.items():
            res.extend(struct.pack(format, getattr(self, name)))
        return res

class ChunkBodyIEND(StructBase):
    TYPE = b'IEND'
    def __init__(self, iostream, length):
        super().__init__()
        self.verify_length(length)

    def to_bytes(self):
        return b''

class ChunkBodyGeneric(StructBase):
    def __init__(self, iostream, length):
        super().__init__()
        self.bytes = self.read(iostream, f">{length}s")
        self.verify_length(length)

    def to_bytes(self):
        return bytes(self.bytes)

class Chunk(StructBase):
    type_factory = {
        ChunkBodyIHDR.TYPE: ChunkBodyIHDR,
        ChunkBodyIEND.TYPE: ChunkBodyIEND
    }

    _fields = {
        "length":                '>I',
        "type":                  '>4s',
        "body":                  '?',
        "crc":                   '>I',
    }
    
    def __init__(self, iostream):
        super().__init__()
        self.length = self.read(iostream, self._fields["length"])
        self.type   = self.read(iostream, self._fields["type"])
        self.body   = self.read_child(self.chunk_factory(self.type)(iostream, self.length))
        self.crc    = self.read(iostream, self._fields["crc"])

    @classmethod
    def chunk_factory(cls, type):
        if type in cls.type_factory:
            return cls.type_factory[type]
        logging.debug(f"Unimplemented chunk type: {type.decode('ASCII')}")
        return ChunkBodyGeneric

    def to_bytes(self):
        res = bytearray()
        res.extend(struct.pack(self._fields["length"], self.length))
        res.extend(struct.pack(self._fields["type"], self.type))
        res.extend(self.body.to_bytes())
        res.extend(struct.pack(self._fields["crc"], self.crc))
        return res

    def is_crc_valid(self):
        actual_crc = zlib.crc32(self.type + self.body.to_bytes()) & 0xffffffff
        return actual_crc == self.crc


class Png(object):
    PNG_MAGIC = b'\x89PNG\r\n\x1a\n'

    def __init__(self, byte_stream):
        self.io = byte_stream
        self.magic = byte_stream.read(len(self.PNG_MAGIC))
        if self.magic != self.PNG_MAGIC:
            raise Exception("Invalid signature")

        self.chunks = []
        self._read_chunks()
    
    def _read_chunks(self):

        while chunk := Chunk(self.io):
            self.chunks.append(chunk)
            if chunk.type == ChunkBodyIEND.TYPE:
                break

        self.ihdr = self.chunks[0]
        if self.ihdr.type != ChunkBodyIHDR.TYPE:
            raise Exception("First chunk must be IHDR")

        self.iend = self.chunks[-1]

    def to_bytes(self):
        res = bytearray()
        res.extend(self.magic)
        for chunk in self.chunks:
            res.extend(chunk.to_bytes())
        return res

if __name__ == "__main__":
    with open("cool_recovery_concept.png", "rb") as f:
        p = Png(f)
```