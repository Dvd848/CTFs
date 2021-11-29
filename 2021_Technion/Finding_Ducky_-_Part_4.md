# Finding Ducky - Part 4
Category: Cryptography, 100 points

## Description

> Okay so we found a cipher, but what is it? and what twitter has to do with it ?


## Solution

In the [previous challenge](Finding_Ducky_-_Part_3.md) we got the following text:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_3]
└─$ cat cipher.txt
Look at my twitter, the flag is cstechnion{the_new_logo_is_great_right}
84 366 446 496 402 66 111 340 280 243 131 157 429 263 181 273 143 407 470 356 125 176 333 467 343 161 188 115 306 179 419 466 253 156 278 316 72 317 485 348 320
```

If we check Ducky's twitter, we see that a mysterious tweet:

```
Aekv5OXXuaSa8KK09ZlLorK;JfKSygNa7DvP34WzRmcy0xuA;nkChdfWjy81HzgDuI/ewZ9LaqWMKFaVBVoZhBi6DuZFxih6QH.tstHN0Eb:joz/tIntJXyogXp2/cvr9MSoYL8nv9lRkFp5aOV1GWo1j3LZoai45sl:nzrqDT32vi;1t23cB2;CX0QS.ebHv;kw.B.T5Zw0qbro.2GT6DeazvBt1ZUuQ5V7LJeCVbjrPadeE;:hxuj3;JQ92i9ZYCq1Xfr4QvKAGdckL1;nhNnXcUkV3mERXjYT4TR7H,rUldD/iqe6xUFbs9A4.c06l,E2Zoon7hFtWf1s:KYaeh1cbW7wi2uOcStW.wEeqJi1c7tK2xC57n87EQNrk4XQ/CHg4m,:n7nB8UdK3Z:pDu32WTIkm8Iv6FFhFjeCJ7,qdi1AcqC,Hb0KNjTb48tX/Y7L,E8C6d13.;pywLn.8V1RKzIobWy9:acRx.yMIw6fjKFMpdrP
```

This might look like a hash but none of [hashcat's example hashes](https://hashcat.net/wiki/doku.php?id=example_hashes) seem to match, and what should we do with the list of numbers anyway?

What if we use them as indices to the text?

```python
>>> text = "Aekv5OXXuaSa8KK09ZlLorK;JfKSygNa7DvP34WzRmcy0xuA;nkChdfWjy81HzgDuI/ewZ9LaqWMKFaVBVoZhBi6DuZFxih6QH.tstHN0Eb:joz/tIntJXyogXp2/cvr9MSoYL8nv9lRkFp5aOV1GWo1j3LZoai45sl:nzrqDT32vi;1t23cB2;CX0QS.ebHv;kw.B.T5Zw0qbro.2GT6DeazvBt1ZUuQ5V7LJeCVbjrPadeE;:hxuj3;JQ92i9ZYCq1Xfr4QvKAGdckL1;nhNnXcUkV3mERXjYT4TR7H,rUldD/iqe6xUFbs9A4.c06l,E2Zoon7hFtWf1s:KYaeh1cbW7wi2uOcStW.wEeqJi1c7tK2xC57n87EQNrk4XQ/CHg4m,:n7nB8UdK3Z:pDu32WTIkm8Iv6FFhFjeCJ7,qdi1AcqC,Hb0KNjTb48tX/Y7L,E8C6d13.;pywLn.8V1RKzIobWy9:acRx.yMIw6fjKFMpdrP"
>>> for x in "84 366 446 496 402 66 111 340 280 243 131 157 429 263 181 273 143 407 470 356 125 176 333 467 343 161 188 115 306 179 419 466 253 156 278 316 72 317 485 348 320".split():
...     print(text[int(x)], end="")
...
http://echoai421521.ctf.cs.technion.ac.il>>>
```

We visit the URL to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_4]
└─$ curl http://echoai421521.ctf.cs.technion.ac.il
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Echo AI</title>
</head>
<body style="background-color:#040720;">
    <div align="center">
    <h1 style="color:#e5e5e5;">cstechnion{an_ai_duck,_now_i've_seen_everything}
</h1>
    <video width="960" height="600" controls autoplay>
        <source src="ducky.mp4" type="video/mp4">
    </video>
</div>
</body>
</html>
```