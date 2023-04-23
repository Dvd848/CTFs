
# Water Turned to Blood

## Description

> > “Thus says the Lord, ‘By this you shall know that I am the Lord: behold, I will strike the water that is in the Nile with the staff that is in my hand, and it will be turned to blood. The fish that are in the Nile will die, and the Nile will become foul, and the Egyptians will find difficulty in drinking water from the Nile’” (Exodus 7:17-18).

> Once the waters turned to blood, our talented Israeli photographer decided to capture it, twice. Note the exclusive differences you see, or do not.

Two PNG files were available for download.

## Solution

Visually, both images display the same picture, but we can see that they are not binary identical:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Water_turned_to blood]
└─$ diff 1.png 2.png
Binary files 1.png and 2.png differ
```

The difference itself is located at the very end of the files:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Water_turned_to blood]
└─$ diff <(xxd -g 1 1.png) <(xxd -g 1 2.png)
86871,86873c86871,86873
< 00153560: 92 24 49 92 24 49 92 24 e9 ba f7 ff 01 57 e7 7d  .$I.$I.$.....W.}
< 00153570: 3b 77 9a 1f 77 00 00 00 00 49 45 4e 44 ae 42 60  ;w..w....IEND.B`
< 00153580: 82                                               .
---
> 00153560: 92 24 49 92 24 49 92 24 e9 ba f7 ff 01 15 b4 30  .$I.$I.$.......0
> 00153570: 78 3f e1 56 28 4c 4f 56 45 16 0d 0f 09 eb 16 3a  x?.V(LOVE......:
> 00153580: ff
```

We can see that the last `20` bytes are different. It's also pretty noticeable that
the sequence of four consecutive `0x00` bytes in `1.png` became `LOVE` in `2.png`.
This might mean that the last 20 bytes were "encrypted" with a simple XOR cipher, and that the key at that location was `LOVE`. Let's try to XOR both buffers together to recover the full key:

```python
>>> import pwn
>>> with open("1.png", "rb") as f1, open("2.png", "rb") as f2:
...     data1 = f1.read()
...     data2 = f2.read()
...     print(pwn.xor(data1[-20:], data2[-20:]).decode())
BSMCH{I_LOVE_HAMETZ}
```