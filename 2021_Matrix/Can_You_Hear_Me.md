# Can You Hear Me?
Category: Forensics, 50 Points

## Description

> I think I heard something. I'm pretty sure it was around 3:00 pm...

A binary file was attached.

## Solution

Let's check the binary file:

```console
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# file Can_You_Hear_Me
Can_You_Hear_Me: data
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# binwalk Can_You_Hear_Me

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
```

Noting much to see here. Let's take a look at the binary:

```console
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# xxd -g 1 Can_You_Hear_Me | head
00000000: f3 ff c4 44 00 00 03 00 00 48 00 00 17 00 91 05  ...D.....H......
00000010: 03 f6 04 b8 63 60 32 13 87 e6 8a a1 48 06 7e 99  ....c`2.....H.~.
00000020: 5e fd 43 af c5 10 b1 62 ca e3 20 6b 00 00 83 00  ^.C....b.. k....
00000030: 82 8b 2e 0e 2e 1f 70 fe 3e b8 13 7c de 07 fe 53  ......p.>..|...S
00000040: 40 90 0c 10 81 44 ff 8f ff ff a3 fc 00 b1 0f 3e  @....D.........>
00000050: 0a c1 f0 c1 0e fa 61 05 70 a0 b3 22 65 8c 4e 5f  ......a.p.."e.N_
00000060: f3 ff c4 44 11 53 79 e8 01 f0 10 4f 63 00 e7 a5  ...D.Sy....Oc...
00000070: 32 a3 d3 c9 4e 82 0e 73 9a c5 cb b8 63 f4 aa 08  2...N..s....c...
00000080: f8 61 9b 16 2f 1d 81 9c 8f 73 cb 02 90 ad 34 45  .a../....s....4E
00000090: 15 10 58 a8 59 58 84 58 de 68 a4 c9 a0 93 ac 1a  ..X.YX.X.h......
```

Nothing jumps to the eye, but if we take a closer look we can see a pattern starting with `f3 ff c4 44`. Is this the prefix for some kind of record?

We'll align the binary differently and inspect it again:

```console
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# xxd -g 1 -c 96 Can_You_Hear_Me | head
00000000: f3 ff c4 44 00 00 03 00 00 48 00 00 17 00 91 05 03 f6 04 b8 63 60 32 13 87 e6 8a a1 48 06 7e 99 5e fd 43 af c5 10 b1 62 ca e3 20 6b 00 00 83 00 82 8b 2e 0e 2e 1f 70 fe 3e b8 13 7c de 07 fe 53 40 90 0c 10 81 44 ff 8f ff ff a3 fc 00 b1 0f 3e 0a c1 f0 c1 0e fa 61 05 70 a0 b3 22 65 8c 4e 5f  ...D.....H..........c`2.....H.~.^.C....b.. k..........p.>..|...S@....D.........>......a.p.."e.N_
00000060: f3 ff c4 44 11 53 79 e8 01 f0 10 4f 63 00 e7 a5 32 a3 d3 c9 4e 82 0e 73 9a c5 cb b8 63 f4 aa 08 f8 61 9b 16 2f 1d 81 9c 8f 73 cb 02 90 ad 34 45 15 10 58 a8 59 58 84 58 de 68 a4 c9 a0 93 ac 1a 3a 93 eb 41 7a a5 a2 97 a2 a2 5c 6c db 64 1b 74 b6 5a 17 9a ce cd bd 9b df 75 d0 6b 56 65 98 8c  ...D.Sy....Oc...2...N..s....c....a../....s....4E..X.YX.X.h......:..Az.....\l.d.t.Z.......u.kVe..
000000c0: f3 ff c4 44 21 5e 2a 23 01 2c 98 9d 34 00 66 22 4c f4 6d d3 e9 5f ad 55 d5 f6 d0 41 a6 42 90 5a f5 43 ae 6a a9 fb ad 99 75 7d 99 ed 46 ba cd 74 2f d0 7a 9f 04 12 38 cd 04 12 01 62 6b a0 81 b1 99 01 f1 8a 89 89 31 61 c2 8b 87 81 71 42 02 9e 30 08 34 a9 0d cc 14 01 60 00 c1 28 00 51 80 c1  ...D!^*#.,..4.f"L.m.._.U...A.B.Z.C.j....u}..F..t/.z...8....bk.........1a....qB..0.4.....`..(.Q..
00000120: f3 ff c4 44 1c 2c a2 99 01 60 98 9d 20 00 1d 44 13 04 64 20 0a 40 8f 30 33 01 90 14 82 b2 10 c0 f8 41 0a 63 27 6b 89 88 5a 16 1c 04 69 cd a0 1b 24 bb 5b 5e 28 cd 77 21 ff fa 7a fa 6c fa 6a a5 7d ad a6 e8 d3 9a e9 7c 90 e0 ab b8 7f 3b ff ff b3 ff 55 f2 cf 65 4e 32 30 98 89 39 9f 13 e0 07  ...D.,...`.. ..D..d .@.03........A.c'k..Z...i...$.[^(.w!..z.l.j.}......|.....;....U..eN20..9....
00000180: f3 ff c4 44 13 0c 96 f8 01 70 30 db d1 00 83 79 85 80 82 85 0c 0d 7c ee ca d4 89 05 1c 50 1c 24 82 62 76 0a 63 b8 28 4d 26 aa a1 62 ac a8 af 9a a5 3c 4f d7 78 a0 0f 82 70 aa cb fd 1d 50 ff d3 01 fd c3 00 e1 ee 20 63 ff f9 ff ff ff ff 4a ff df 96 69 e3 21 d0 1a 73 b1 1f 7d 2a 45 85 75 8e  ...D.....p0....y......|......P.$.bv.c.(M&..b.....<O.x...p....P........ c......J...i.!..s..}*E.u.
000001e0: f3 ff c4 44 15 0f 92 a9 00 98 44 de 9c 94 6e 04 4e f3 20 30 80 69 0b 25 f1 f7 c8 d7 30 c3 21 e4 d9 60 b2 bc e9 5a 43 24 d3 ea c4 fe 47 4e 87 4f 3c ef f5 ff 1b 96 e7 7c ff ff 46 a6 09 40 74 0e 59 66 b7 08 08 21 0e a9 04 71 e3 d9 8a 40 ff 77 ff ff 91 fe ef 3d eb 3f 11 37 eb 6d 4a b8 9b c1  ...D......D...n.N. 0.i.%....0.!..`...ZC$....GN.O<......|..F..@t.Yf...!...q...@.w.....=.?.7.mJ...
00000240: f3 ff c4 44 13 0b a2 d9 00 b4 06 c6 59 94 f3 d6 32 dd 82 08 db 56 ba e5 7c 89 03 0e 76 db 10 fe f1 58 89 20 34 fc 27 fa 34 23 f1 cf 2b a9 ff b7 01 be f0 bf e5 ff b2 3f 18 bf 97 18 9c 80 30 60 86 37 76 4e 34 94 61 cd d2 3b 1a 61 a2 bd c2 c2 6a 72 38 eb c3 2f 2f 23 5a 48 a0 42 0c d9 88 f3  ...D........Y...2....V..|...v....X. 4.'.4#..+..........?......0`.7vN4.a..;.a....jr8..//#ZH.B....
000002a0: f3 ff c4 44 13 0e 4e 59 00 bc 0a 8e 03 95 19 06 3d 79 d4 ed 81 71 75 d4 bd 35 99 44 0f cc a9 86 53 a8 a7 15 ff db 55 ca 9f 77 0b f2 2a f5 63 36 fa 39 b3 19 0b 09 7b 14 a4 9c 03 0c cc 87 70 54 1d 28 95 10 12 62 6b cf fb ff 0b ff ff 55 da fe d5 05 7b ba 45 e0 3e 81 1b ef b3 b4 03 62 e5 49  ...D..NY........=y...qu..5.D....S.....U..w..*.c6.9....{.......pT.(...bk......U....{.E.>......b.I
00000300: f3 ff c4 44 11 13 5e d9 00 c0 0a 7e 9c 94 c3 ce 55 22 f2 56 6e ff 93 44 bc 49 f7 b1 ab 29 ac 45 ff 73 b5 2a e5 30 e8 67 a7 ff e5 d3 35 41 88 42 06 94 1f 1c 49 8b 87 02 0a 0b ff 8d ff ff ff ff 6a a5 90 f9 81 67 64 db e4 0c 20 f0 48 a2 6c a8 07 8b 07 44 08 68 c5 90 52 4c d8 0f 04 2d ee 4f  ...D..^....~....U".Vn..D.I...).E.s.*.0.g....5A.B....I...........j....gd... .H.l....D.h..RL...-.O
00000360: f3 ff c4 44 11 1e 6e 29 00 bc 84 94 82 94 06 67 7d 58 45 bf 57 13 5f f9 ff ec ff 7f 4c f9 0c 71 14 c6 71 18 83 e7 88 ae 87 84 83 14 7a f6 ff 3f ff ff 45 d9 ef ff 04 f7 07 11 6b 7f f6 3a 05 af 3a da 27 c9 4d 8d 4a 94 ad 86 54 03 51 2c 3e 91 33 53 34 58 36 00 f2 18 13 27 1e 2a 92 6e fd 49  ...D..n).......g}XE.W._.....L..q..q.........z..?..E.......k..:..:.'.M.J...T.Q,>.3S4X6....'.*.n.I
```

Indeed, this looks like each record is of length 96. Now what?

Well, searching for the `f3 ff c4 44` prefix as a known file prefix in Google does not yield any result. That makes sense, since if it would, `binwalk` would have probably found it. We have two clues to help us progress. The first one is a very common string seen in the binary:

```console
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# strings Can_You_Hear_Me | sort | uniq -c | sort -n | tail
      4 vL5+
      4 wKLs
      4 WqT\A
      4 )xPe
      4 y&!p
      4 y&(x
      6 00UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
      7 00UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUALEM.3
     13 UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUU
     41 ALEM.3
```

That `ALEM.3` repeats at least 41 times (even more, sometimes identified by `strings` as part of a larger string). In this case too, we can't find it in Google, but it must have some significance.

The second one is the challenge title:

> Can you hear it?

This implies some kind of sound file format.

Now, if we think of `ALEM` in the context of sound files, it sounds very much like [LAME](https://lame.sourceforge.io/), the famous MP3 encoder. But the file signature doesn't match MP3! According to [this list](https://en.wikipedia.org/wiki/List_of_file_signatures), MP3 signatures are:

| Hex Signature | Offset | Filename Extension | Description | 
| ------------- | ------ | ------------------ | ----------- |
| (-) `FF FB` <br /> (-) `FF F3` <br /> (-) `FF F2` | 0 | mp3 |  	MPEG-1 Layer 3 file without an ID3 tag or with an ID3v1 tag (which's appended at the end of the file) |
| (-) `49 44 33` | 0 | mp3 | MP3 file with an ID3v2 container  |

We don't have that, we have `F3 FF`. It is pretty close to `FF F3` though. And we did see `ALEM` earlier, which is close to `LAME`. So what if we do the most trivial thing we can think of, and swap every two bytes?

```python
from pathlib import Path
import os

FILE = "Can_You_Hear_Me"

with open(FILE, "rb") as f, open(f"{Path(FILE).stem}_out.mp3", "wb") as o:
    file_size = os.path.getsize(FILE)
    for _ in range(file_size // 2):
        b1 = f.read(1)
        b2 = f.read(1)
        o.write(b2)
        o.write(b1)

    if (file_size % 2 == 1):
        o.write(f.read(1))
```

Output:

```console
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# python3 solve.py
root@kali:/media/sf_CTFs/matrix/Can_You_Hear_Me# file Can_You_Hear_Me_out.mp3
Can_You_Hear_Me_out.mp3: MPEG ADTS, layer III, v2,  32 kbps, 24 kHz, Monaural
```

We got a legal MP3 file! It spells out the flag, which is `MCL{I_KNEW_THAT_I_HEARD_SOMETHING}`