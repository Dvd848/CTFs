# Buenos Aires - Conference
Category: Misc.

## Description

> You arrive at the location through the coordinates that you got from the assassin, a luxurious yacht. A fat, bald man lies on a puma couch. He sips on a dry martini, smokes the biggest cigar you've ever seen and when he smiles, a golden tooth is revealed. You can’t help but smile back at him, although you think the place seems shady. "Welcome to my yacht, Johnson, finally you show us your face. Have you killed the AGENT now? Good! You’re here to collect your reward I presume? I’ll have my guy finalize the transaction but before you leave I need a small favour from you." It seems that he is mistaking you for the assassin but you don’t mind.
> 
> Challenge: Hide and seek (misc)
> 
> The man hands you a pendrive which you reluctantly connect to your laptop. He says he got it from a partner, and the partner claims that he hid valuable information in that PNG there. The problem is, it looks empty. See if you can find anything.

A PNG file was attached.

## Solution

Let's check the PNG validity with `pngcheck`:

```console
┌──(user@kali)-[/media/sf_CTFs/google/8_Croatia_-_Yacht]
└─$ pngcheck download/hideandseek.png
download/hideandseek.png  illegal (unless recently approved) unknown, public chunk eDIH
ERROR: download/hideandseek.png
```

This PNG has an unknown chunk ID: `eDIH`, read backwards as `HIDe`.

We'll use [Kaitai](https://kaitai.io/) to extract whatever's there. First, we generate a Python script to access PNG files:

```console
┌──(user@kali)-[/media/sf_CTFs/google/8_Croatia_-_Yacht]
└─$ ksc ~/utils/forensics/kaitai_struct_formats/image/png.ksy -t python
```

Then we use it in the following script:

```python
import png

data = bytearray()

p = png.Png.from_file("download/hideandseek.png")
for chunk in p.chunks:
    if chunk.type == "eDIH":
        data += chunk.body

print(data.decode())
```

Output: 

```console
┌──(user@kali)-[/media/sf_CTFs/google/8_Croatia_-_Yacht]
└─$ python3 solve.py
Q1RGe0RpZFlvdUtub3dQTkdpc1Byb25vdW5jZWRQSU5HP30=
```

Looks like base64:

```console
┌──(user@kali)-[/media/sf_CTFs/google/8_Croatia_-_Yacht]
└─$ python3 solve.py | base64 -d
CTF{DidYouKnowPNGisPronouncedPING?}
```