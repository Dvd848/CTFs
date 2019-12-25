# Dinary
Misc., 15 points

## Description
> My kipod is gone :(

An image file was provided.

## Solution

We receive an image file composed of black and white pixels.

Running `zsteg` on the file, we get:

```console
root@kali:/media/sf_CTFs/kaf/Dinary# zsteg dinary.png
b1,r,lsb,xy         .. text: "Hello World!, Yesterday my kipod went free and i can't find him! KAF{wh3r3_1s_my_k1p0d?}\nMaybe YOU can help me find my kipod? (his name is \"Fibon\")\nHope you'll find him, he should go on a tour really soon.\n1nFr>F3jpYQu@.KNMJPGUC*D$DL5Ky(IV:&{z6B#a7+jX0(u&?"
```

This is also achievable by iterating the pixels and interpreting them as binary:

```python
from PIL import Image

WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

res = ""

img = Image.open("dinary.png")
m = img.load()
w, h = img.size

i = 0
for x in range(w):
    for y in range(h):
        i += 1
        if m[y,x] == WHITE:
            res += "1"
        elif m[y,x] == BLACK:
            res += "0"
        else:
            raise Exception("Unexpected value {}".format(m[x,y]))
        if len(res) == 8:
            print("".join(chr(int(res,2))), end='', flush=True)
            res = ""
```

The flag: `KAF{wh3r3_1s_my_k1p0d?}`