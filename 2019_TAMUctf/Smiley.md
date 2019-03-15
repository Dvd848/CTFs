# :)
Crypto, 456 points

## Description:

> Look at what I found! XUBdTFdScw5XCVRGTglJXEpMSFpOQE5AVVxJBRpLT10aYBpIVwlbCVZATl1WTBpaTkBOQFVcSQdH

## Solution:

This looks like base64, let's try to decode it:

```console
root@kali:/media/sf_CTFs/tamu/Smiley# echo XUBdTFdScw5XCVRGTglJXEpMSFpOQE5AVVxJBRpLT10aYBpIVwlbCVZATl1WTBpaTkBOQFVcSQdH | base64 -d
]@]LWRsW       TFN     I\JLHZN@N@U\IKO]`HW [       V@N]VLZN@N@U\IG
root@kali:/mediYBpIVwlbCVZATl1WTBpaTkBOQFVcSQdH | base64 -d | xxd -g 1OQE5AVVxJBRpLT10aY
00000000: 5d 40 5d 4c 57 52 73 0e 57 09 54 46 4e 09 49 5c  ]@]LWRs.W.TFN.I\
00000010: 4a 4c 48 5a 4e 40 4e 40 55 5c 49 05 1a 4b 4f 5d  JLHZN@N@U\I..KO]
00000020: 1a 60 1a 48 57 09 5b 09 56 40 4e 5d 56 4c 1a 5a  .`.HW.[.V@N]VL.Z
00000030: 4e 40 4e 40 55 5c 49 07 47                       N@N@U\I.G
```

The first three letters give us a reason to be optimistic: We know that all flags start with `gigem`, and having the first and third bytes identical matches this pattern.

At first I thought that this might be some kind of rotation cipher, but the diff between `g` and 0x5d is different than the diff between `i` and 0x40.

The next attempt was a XOR cipher. One of the XOR properties is that if we assume that `a^b = c`, then it is also true that `a^c = b`. So if there is some secret key `k` which was used to XOR the original plaintext, and we know that `'g' ^ key = 0x5d`, then we can recover the key by performing `0x5d ^ 'g'` which is 0x3a.

```python
>>> "".join(chr(ord(x) ^ 0x3a) for x in base64.b64decode("XUBdTFdScw5XCVRGTglJXEpMSFpOQE5AVVxJBRpLT10aYBpIVwlbCVZATl1WTBpaTkBOQFVcSQdH"))
'gzgvmhI4m3n|t3sfpvr`tztzofs? qug Z rm3a3lztglv `tztzofs=}'
```

That's not quite right, although we did get the the `g`'s and the `m` correct, as well as the final `}`.

Maybe there are two alternating XOR keys?

```python
import base64

s = "XUBdTFdScw5XCVRGTglJXEpMSFpOQE5AVVxJBRpLT10aYBpIVwlbCVZATl1WTBpaTkBOQFVcSQdH"
decoded = base64.b64decode(s)

xor1 = ord(decoded[0]) ^ ord('g')
xor2 = ord(decoded[1]) ^ ord('i')

xors = [xor1, xor2]

res = ""
for i in range(len(decoded)):
    res += chr(ord(decoded[i]) ^ xors[i % 2])

print (res)
```

Output:
```
root@kali:/media/sf_CTFs/tamu/Smiley# python solve.py
gigem{I'm not superstitious, but I am a little stitious.}
```

