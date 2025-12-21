# CorruptedZipFile

Category: Forensics

## Description

> Inspired by the XZ attack, can you find a way to reuse one of the substitution ciphers to find your flag? (see https://gynvael.coldwind.pl/?lang=en&id=782). 

A file, `corrupted.zip`,  was attached.

## Solution

The "corrupted" file has a `*.zip` extension, but it isn't recognized as a zip archive by `file`:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ file corrupted.zip
corrupted.zip: data
```

This is expected based on the description, which refers to the `XZ` attack where one of the 
steps was to encrypt a file with a simple substitution cipher.

As the blog post very nicely breaks down (see the [original post](https://gynvael.coldwind.pl/?lang=en&id=782) for a more thorough explanation):

> [...] Step 5 [of the attack] revisits the `tr` command, which in this case is used as a very simple substitution cipher [...]  
> `tr "\5-\51\204-\377\52-\115\132-\203\0-\4\116-\131" "\0-\377"`  
> This basically means that byte of value 5 will be substitute with byte of value 0, byte of value 6 will be substituted with byte of value 1, and so on. In each case there are 6 ranges which map to the whole 0 - 255 (that's 377 octal) range.

So, we'll need to break the cipher, mapping back each byte of the encrypted file to its original value.

This isn't entirely trivial for a ZIP file, since ZIP files are parsed from the bottom up, as visualized so nicely by [angea](https://github.com/corkami/pics/tree/master)'s poster:

![](https://github.com/corkami/pics/blob/master/binary/ZIP101.png?raw=true)

To located the first element to parse, the "End of Central Directory", we need to scan the binary, looking for `50 4B 05 06`. But, in our case, what do we look for?

Well, while it's not mandatory by the spec, ZIP files usually start with a similar signature, `50 4B 03 04`, which is a Local File Header. So, in the absence of any better option, we'll hope that this is the case here too, and start building a mapping between byte values.

Our zip file starts with the following bytes:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ xxd -g 1 corrupted.zip| head
00000000: f6 c2 f2 f3 05 ef ef ef ef ef 31 df 82 fe ef ef  ..........1.....
00000010: ef ef ef ef ef ef ef ef ef ef 03 ef 17 ef a5 a5  ................
00000020: a6 cf cc df cc a6 fb fa 04 ef f2 91 5f ed d1 ca  ............_...
00000030: 94 49 d1 e0 e3 06 ef f0 f3 11 8d f2 ef f3 f9 ca  .I..............
00000040: f0 ef f6 c2 f2 f3 0f ef 04 ef 03 ef 70 db 9f fe  ............p...
00000050: 0a d2 32 1e b7 dd 10 ef ce e8 10 ef 17 ef 17 ef  ..2.............
00000060: a5 a5 a6 cf cc df cc a6 e5 a7 d8 ab d0 e0 e5 a7  ................
00000070: d8 ab d0 e0 e5 a7 d8 ab a5 db d9 d2 fb fa 04 ef  ................
00000080: f2 d0 94 49 d1 ed 94 49 d1 e0 e3 06 ef f0 f3 11  ...I...I........
00000090: 8d f2 ef f3 f9 ca f0 ef d3 27 c9 a4 a2 1b d5 7e  .........'.....~
```

We'll assume that: 

```
f6 -> 50
c2 -> 4b
f2 -> 03
f3 -> 04
```

Given that we're dealing with substitution ranges, the fact that `f2, f3` are consecutive (just
like `03, 04`) is a bit encouraging. 

Let's start translating with the following script template:

```python
def byte_to_ascii(byte_value):
    if 32 <= byte_value <= 126:
        return chr(byte_value).replace("<","&gt;").replace(">", "&lt;")
    else:
        return '.'

def write_html_hex_dump(output_file_html, original_bytes, translated_bytes, start_offset):
    with open(output_file_html, 'w') as html_file:
        html_file.write("<html><head><style>.r{ color:red }</style></head><body><pre>\n")
        for i in range(start_offset, len(original_bytes), 16):
            original_chunk = original_bytes[i:i + 16]
            translated_chunk = translated_bytes[i:i + 16]

            html_file.write(f"{i:08X} ")
            
            for j in range(len(original_chunk)):
                original_byte = original_chunk[j]
                translated_byte = translated_chunk[j]
                classname = " class='r'" if original_byte != translated_byte else ''
                html_file.write(f"<span{classname}>{translated_byte:02X} </span>")
                
            html_file.write("  ")
            
            for j in range(len(original_chunk)):
                html_file.write(byte_to_ascii(translated_chunk[j]))
            
            html_file.write("\n")
        html_file.write("</pre></body></html>\n")

def translate_binary_file_with_hex_dump(input_file, output_file, output_file_html, byte_mapping): 
    
    with open(input_file, 'rb') as infile, open(output_file, 'wb') as outfile:
        original_bytes = infile.read()
        translated_bytes = bytearray()
        for byte in original_bytes:   
            byte_value = byte_mapping.get(byte, byte)
            translated_bytes.append(byte_value if byte_value is not None else byte)
            
        outfile.write(translated_bytes)
    
    start = 0
    write_html_hex_dump(output_file_html, original_bytes, translated_bytes, start)

byte_mapping = {
    0xc2: 0x4b,
    0xf2: 0x03,
    0xf6: 0x50,
    0xf3: 0x04,
}

if (len(set(byte_mapping.items())) != 256):
    print("Warning: Not all bytes are mapped!")

translate_binary_file_with_hex_dump("corrupted.zip", "output.zip", "output_dump.html", byte_mapping)
```

Apart from translating the binary, it creates a simple hex dump HTML file where translated 
bytes are marked in red.

Let's take a look at the file after translating the bytes we mapped:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ xxd -g 1 output.zip| head
00000000: 50 4b 03 04 05 ef ef ef ef ef 31 df 82 fe ef ef  PK........1.....
00000010: ef ef ef ef ef ef ef ef ef ef 03 ef 17 ef a5 a5  ................
00000020: a6 cf cc df cc a6 fb fa 04 ef 03 91 5f ed d1 ca  ............_...
00000030: 94 49 d1 e0 e3 06 ef f0 04 11 8d 03 ef 04 f9 ca  .I..............
00000040: f0 ef 50 4b 03 04 0f ef 04 ef 03 ef 70 db 9f fe  ..PK........p...
00000050: 0a d2 32 1e b7 dd 10 ef ce e8 10 ef 17 ef 17 ef  ..2.............
00000060: a5 a5 a6 cf cc df cc a6 e5 a7 d8 ab d0 e0 e5 a7  ................
00000070: d8 ab d0 e0 e5 a7 d8 ab a5 db d9 d2 fb fa 04 ef  ................
00000080: 03 d0 94 49 d1 ed 94 49 d1 e0 e3 06 ef f0 04 11  ...I...I........
00000090: 8d 03 ef 04 f9 ca f0 ef d3 27 c9 a4 a2 1b d5 7e  .........'.....~
```

There's another `PK` there, which is also good. Let's take a look at the end of the file:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ xxd -g 1 output.zip| tail
0045aa10: d9 d2 fb fa 00 ef 03 91 5f ed d1 e0 e3 06 ef f0  ........_.......
0045aa20: 04 11 8d 03 ef 04 f9 ca f0 ef 50 4b f0 f1 19 03  ..........PK....
0045aa30: 0f ef 04 ef 03 ef 31 df 82 fe 38 f7 c3 83 cc ea  ......1...8.....
0045aa40: 0b ef 07 eb 0b ef 17 ef 13 ef ef ef ef ef ef ef  ................
0045aa50: ef ef 41 ec 90 23 ac ef a5 a5 a6 cf cc df cc a6  ..A..#..........
0045aa60: e5 a7 d8 ab d0 e1 e5 a7 d8 ab d0 e1 e5 a7 d8 ab  ................
0045aa70: a5 db d9 d2 fb fa 00 ef 03 91 5f ed d1 e0 e3 06  .........._.....
0045aa80: ef f0 04 11 8d 03 ef 04 f9 ca f0 ef 50 4b 00 01  ............PK..
0045aa90: ef ef ef ef 00 ef 00 ef 77 f0 ef ef 57 49 bc ef  ........w...WI..
0045aaa0: ef ef
```

More `PK`'s, great. Hopefully, the last one is the End of Central Directory, so we can add
two more bytes to our mapping based on the expected signature:

```
00 -> 05
01 -> 06
```

We can continue, working with the [ZIP header structure reference](https://en.wikipedia.org/wiki/ZIP_(file_format)#File_headers), 
and possibly in parallel with a legal zip file using a HEX viewer that is able to parse the file
structure (such as [pytai](https://github.com/Dvd848/pytai)) to recover byte after byte. It's easy
to see, based on the amount and location of `ef`s, that it would map to `00`. We won't go into
all the different bytes that can be recovered with high confidence, simply since it's really
to much to capture. However, the basic idea is as explained above, and at some point you 
should have enough bytes spread across the full range to take a deep breath and just fill in 
the blank ranges consecutively.

Eventually, after too long and too many guesses, we arrive to the following mapping:

```python
byte_mapping = {
    0x00:0x05, 0x01:0x06, 0x02:0x07, 0x03:0x08, 0x04:0x09, 0x05:0x0a, 0x06:0x0b, 0x07:0x0c, 0x08:0x0d, 0x09:0x0e, 0x0A:0x0f, 0x0B:0x10, 0x0C:0x11, 0x0D:0x12, 0x0E:0x13, 0x0F:0x14, 
    0x10:0x15, 0x11:0x16, 0x12:0x17, 0x13:0x18, 0x14:0x19, 0x15:0x1a, 0x16:0x1b, 0x17:0x1c, 0x18:0x1d, 0x19:0x1e, 0x1A:0x1f, 0x1B:0x20, 0x1C:0x21, 0x1D:0x22, 0x1E:0x23, 0x1F:0x24, 
    0x20:0x25, 0x21:0x26, 0x22:0x27, 0x23:0x28, 0x24:0x29, 0x25:0x84, 0x26:0x85, 0x27:0x86, 0x28:0x87, 0x29:0x88, 0x2A:0x89, 0x2B:0x8a, 0x2C:0x8b, 0x2D:0x8c, 0x2E:0x8d, 0x2F:0x8e, 
    0x30:0x8f, 0x31:0x90, 0x32:0x91, 0x33:0x92, 0x34:0x93, 0x35:0x94, 0x36:0x95, 0x37:0x96, 0x38:0x97, 0x39:0x98, 0x3A:0x99, 0x3B:0x9a, 0x3C:0x9b, 0x3D:0x9c, 0x3E:0x9d, 0x3F:0x9e, 
    0x40:0x9f, 0x41:0xa0, 0x42:0xa1, 0x43:0xa2, 0x44:0xa3, 0x45:0xa4, 0x46:0xa5, 0x47:0xa6, 0x48:0xa7, 0x49:0xa8, 0x4A:0xa9, 0x4B:0xaa, 0x4C:0xab, 0x4D:0xac, 0x4E:0xad, 0x4F:0xae, 
    0x50:0xaf, 0x51:0xb0, 0x52:0xb1, 0x53:0xb2, 0x54:0xb3, 0x55:0xb4, 0x56:0xb5, 0x57:0xb6, 0x58:0xb7, 0x59:0xb8, 0x5A:0xb9, 0x5B:0xba, 0x5C:0xbb, 0x5D:0xbc, 0x5E:0xbd, 0x5F:0xbe, 
    0x60:0xbf, 0x61:0xc0, 0x62:0xc1, 0x63:0xc2, 0x64:0xc3, 0x65:0xc4, 0x66:0xc5, 0x67:0xc6, 0x68:0xc7, 0x69:0xc8, 0x6A:0xc9, 0x6B:0xca, 0x6C:0xcb, 0x6D:0xcc, 0x6E:0xcd, 0x6F:0xce, 
    0x70:0xcf, 0x71:0xd0, 0x72:0xd1, 0x73:0xd2, 0x74:0xd3, 0x75:0xd4, 0x76:0xd5, 0x77:0xd6, 0x78:0xd7, 0x79:0xd8, 0x7A:0xd9, 0x7B:0xda, 0x7C:0xdb, 0x7D:0xdc, 0x7E:0xdd, 0x7F:0xde, 
    0x80:0xdf, 0x81:0xe0, 0x82:0xe1, 0x83:0xe2, 0x84:0xe3, 0x85:0xe4, 0x86:0xe5, 0x87:0xe6, 0x88:0xe7, 0x89:0xe8, 0x8A:0xe9, 0x8B:0xea, 0x8C:0xeb, 0x8D:0xec, 0x8E:0xed, 0x8F:0xee, 
    0x90:0xef, 0x91:0xf0, 0x92:0xf1, 0x93:0xf2, 0x94:0xf3, 0x95:0xf4, 0x96:0xf5, 0x97:0xf6, 0x98:0xf7, 0x99:0xf8, 0x9A:0xf9, 0x9B:0xfa, 0x9C:0xfb, 0x9D:0xfc, 0x9E:0xfd, 0x9F:0xfe, 
    0xA0:0xff, 0xA1:0x2a, 0xA2:0x2b, 0xA3:0x2c, 0xA4:0x2d, 0xA5:0x2e, 0xA6:0x2f, 0xA7:0x30, 0xA8:0x31, 0xA9:0x32, 0xAA:0x33, 0xAB:0x34, 0xAC:0x35, 0xAD:0x36, 0xAE:0x37, 0xAF:0x38, 
    0xB0:0x39, 0xB1:0x3a, 0xB2:0x3b, 0xB3:0x3c, 0xB4:0x3d, 0xB5:0x3e, 0xB6:0x3f, 0xB7:0x40, 0xB8:0x41, 0xB9:0x42, 0xBA:0x43, 0xBB:0x44, 0xBC:0x45, 0xBD:0x46, 0xBE:0x47, 0xBF:0x48, 
    0xC0:0x49, 0xC1:0x4a, 0xC2:0x4b, 0xC3:0x4c, 0xC4:0x4d, 0xC5:0x5a, 0xC6:0x5b, 0xC7:0x5c, 0xC8:0x5d, 0xC9:0x5e, 0xCA:0x5f, 0xCB:0x60, 0xCC:0x61, 0xCD:0x62, 0xCE:0x63, 0xCF:0x64, 
    0xD0:0x65, 0xD1:0x66, 0xD2:0x67, 0xD3:0x68, 0xD4:0x69, 0xD5:0x6a, 0xD6:0x6b, 0xD7:0x6c, 0xD8:0x6d, 0xD9:0x6e, 0xDA:0x6f, 0xDB:0x70, 0xDC:0x71, 0xDD:0x72, 0xDE:0x73, 0xDF:0x74, 
    0xE0:0x75, 0xE1:0x76, 0xE2:0x77, 0xE3:0x78, 0xE4:0x79, 0xE5:0x7a, 0xE6:0x7b, 0xE7:0x7c, 0xE8:0x7d, 0xE9:0x7e, 0xEA:0x7f, 0xEB:0x80, 0xEC:0x81, 0xED:0x82, 0xEE:0x83, 0xEF:0x00, 
    0xF0:0x01, 0xF1:0x02, 0xF2:0x03, 0xF3:0x04, 0xF4:0x4e, 0xF5:0x4f, 0xF6:0x50, 0xF7:0x51, 0xF8:0x52, 0xF9:0x53, 0xFA:0x54, 0xFB:0x55, 0xFC:0x56, 0xFD:0x57, 0xFE:0x58, 0xFF:0x59 
}
```

Translated to `tr` format, we get the following mapping:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ cat corrupted.zip | tr "\357-\363\0-\44\241-\304\364-\377\305-\356\45-\240" "\0-\377" > decrypted.zip

┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ file decrypted.zip
decrypted.zip: Zip archive data, at least v1.0 to extract, compression method=store
```

Let's peek into the zip file to see what we should expect:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ unzip -l decrypted.zip
Archive:  decrypted.zip
  Length      Date    Time    Name
---------  ---------- -----   ----
        0  2024-07-01 17:36   ../data/
  1408355  2024-07-30 17:06   ../data/z0m4euz0m4euz0m4.png
   983544  2024-07-01 17:36   ../data/z0m4ewz0m4ewz0m4.png
  1094699  2024-07-01 17:36   ../data/z0m4etz0m4etz0m4.png
  1081356  2024-07-01 17:36   ../data/z0m4evz0m4evz0m4.png
---------                     -------
  4567954                     5 files
```

Ok, let's extract:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ unzip -j decrypted.zip -d out
Archive:  decrypted.zip
[decrypted.zip] ../data/z0m4euz0m4euz0m4.png password:
```

Wait, what? A password? Seriously, after so many hours of horrible manual labor?  
Hopefully, `john` can crack it.

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ zip2john decrypted.zip > zip.hash
ver 1.0 decrypted.zip/../data/ is not encrypted, or stored with non-handled compression type
ver 2.0 efh 5455 efh 7875 decrypted.zip/../data/z0m4euz0m4euz0m4.png PKZIP Encr: TS_chk, cmplen=1405504, decmplen=1408355, crc=2391670F ts=70CF cs=70cf type=8
ver 2.0 efh 5455 efh 7875 decrypted.zip/../data/z0m4ewz0m4ewz0m4.png PKZIP Encr: TS_chk, cmplen=983381, decmplen=983544, crc=22561ACC ts=7490 cs=7490 type=8
ver 2.0 efh 5455 efh 7875 decrypted.zip/../data/z0m4etz0m4etz0m4.png PKZIP Encr: TS_chk, cmplen=1094630, decmplen=1094699, crc=C234CB35 ts=7490 cs=7490 type=8
ver 2.0 efh 5455 efh 7875 decrypted.zip/../data/z0m4evz0m4evz0m4.png PKZIP Encr: TS_chk, cmplen=1081185, decmplen=1081356, crc=E24C5197 ts=7490 cs=7490 type=8
NOTE: It is assumed that all files in each archive have the same password.
If that is not the case, the hash may be uncrackable. To avoid this, use
option -o to pick a file at a time.

┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ john zip.hash --wordlist=/usr/share/wordlists/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (PKZIP [32/64])
Will run 4 OpenMP threads
Press 'q' or Ctrl-C to abort, almost any other key for status
2smart4u         (decrypted.zip)
1g 0:00:00:00 DONE (2024-10-18 18:14) 25.00g/s 1638Kp/s 1638Kc/s 1638KC/s XIOMARA..sabrina7
Use the "--show" option to display all of the cracked passwords reliably
Session completed.
```

Finally, we can extract the archive:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ unzip -P 2smart4u -j decrypted.zip -d out
Archive:  decrypted.zip
  inflating: out/z0m4euz0m4euz0m4.png
  inflating: out/z0m4ewz0m4ewz0m4.png
  inflating: out/z0m4etz0m4etz0m4.png
  inflating: out/z0m4evz0m4evz0m4.png
```

We'll use an OCR to check them for the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/CorruptedZipFile]
└─$ ls -rtd out/* | xargs -I '{}' tesseract '{}' - 2>/dev/null
CTF{BytesAreAMatterOfInterpretation}
```

And there you go, one of the images indeed contains it.