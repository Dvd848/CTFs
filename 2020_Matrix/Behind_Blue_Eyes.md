# Behind Blue Eyes
Category: Warmup, 20 Points

## Description

> Not everything meets the eye

A zip file was attached.

## Solution

Let's unzip the archive:

```console
root@kali:/media/sf_CTFs/matrix/BehindBlueEyes# unzip BehindBlueEyes.zip
Archive:  BehindBlueEyes.zip
  inflating: 0.jpeg
  inflating: 1.jpeg
  inflating: 10.jpeg
  inflating: 11.jpeg
  inflating: 2.jpeg
  inflating: 3.jpeg
  inflating: 4.jpeg
  inflating: 5.jpeg
  inflating: 6.jpeg
  inflating: 7.jpeg
  inflating: 8.jpeg
  inflating: 9.jpeg
```

We get 12 images, all containing what seems to be the same blue eye.

Studying the metadata, we see that there's some difference between different images:
```console
root@kali:/media/sf_CTFs/matrix/BehindBlueEyes# diff <(exiftool 0.jpeg) <(exiftool 1.jpeg)
2c2
< File Name                       : 0.jpeg
---
> File Name                       : 1.jpeg
16c16
< Make                            : F
---
> Make                            : L
```

The `Make` seems suspicious, let's read it for all files:
```console
root@kali:/media/sf_CTFs/matrix/BehindBlueEyes# for i in {0..11}; do exiftool $i.jpeg | grep Make | awk '{ printf $3 }'; done && echo
FLAG_THE_WHO
```