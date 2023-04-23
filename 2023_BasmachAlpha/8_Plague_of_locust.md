
# Plague of Locust

## Description

> > “if you refuse to let My people go, behold, tomorrow I will bring locusts into your territory. And they shall cover the face of the earth, so that no one will be able to see the earth; and they shall eat the residue of what is left, which remains to you from the hail, and they shall eat every tree which grows up for you out of the field” (Exodus 10:4-5).
> 
> Prepare yourself! You are about to be attacked by a swarm of locusts, try to stay calm and focus on the picture.
> 
> Note: This is not a reverse challenge

An archive was attached.

## Solution

We extract the archive and get two files:

```console
┌──(user@kali)-[~/CTFs/basmach/Plague_of_locust]
└─$ ls -al locust
total 32992
drwxr-xr-x 2 user user     4096 Mar 26 17:43 .
drwxr-xr-x 3 user user     4096 Apr 12 15:10 ..
-rw-r--r-- 1 user user 33756326 Mar 20 13:07 script.exe
-rw-r--r-- 1 user user    16348 Mar 20 14:50 ארבה.png
```

We have a Windows executable and a PNG image. Let's run the executable:

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
- I  couldn't undestand what 'PNG' means
```

Amusingly, the screen gets filled with Locusts until the program is killed. Anyway, the program is complaining that it doesn't understand what "PNG" is. 
In our context, PNG is the format (and extension) of the attached image. The first few bytes of any PNG image contain the `89 50 4E 47` magic signature, 
which essentially contain the string `PNG`. So what happens if we edit the file and replace the magic signature with a different one?

```
89 50 4E 47 0D 0A 1A 0A 00 00 00 0D 49 48 44 52 | .PNG........IHDR
->
89 54 45 53 54 0A 1A 0A 00 00 00 0D 49 48 44 52 | .TEST.......IHDR
```

Run again:

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
- I  couldn't undestand what 'TEST' means
```

Good! We know how to interact with the program. To make things even easier, moving forward we'll just create a text file called `ארבה.png` and edit it instead.

So, we need to start by asking politely. Instead of `TEST`, we'll use `please`:

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
```

We made some progress, let's decode the next string using base64 and get:

```
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag! to proceed just repeat this message.
```

So, we repeat the message: `hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!`

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
```

The answer is obviously `4`.

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
4
- Beep boop beep beep... I can't find my site on google! what am I missing?
```

It must be related to `robots.txt`.

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
4
- Beep boop beep beep... I can't find my site on google! what am I missing?
robots.txt
- What is null in ASCII?
```

Everyone knows it's `0`.

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
4
- Beep boop beep beep... I can't find my site on google! what am I missing?
robots.txt
- What is null in ASCII?
0
- Oh i know! just repeat what i say...
- e99a18c428cb38d5f260853678922e03
- You don't understand it? maybe you can ask john
```

`john` seems like overkill when we have Google. This is the MD5 hash of `abc123`.

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
4
- Beep boop beep beep... I can't find my site on google! what am I missing?
robots.txt
- What is null in ASCII?
0
- Oh i know! just repeat what i say...
- e99a18c428cb38d5f260853678922e03
- You don't understand it? maybe you can ask john
abc123
- Finally just XOR !($+<;.,; with the letter I
```

XOR we shall, to get a `hamburger`.

This is what our text file looks like at this point:

```
please
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
4
robots.txt
0
abc123
hamburger
```

And the response we get from the program is:

```console
PS E:\CTFs\basmach\locust> .\script.exe
- If you wanna play a game ask politely!
please
- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu
hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!
- Let's start easy...
- What is 2 + 2 = ?
4
- Beep boop beep beep... I can't find my site on google! what am I missing?
robots.txt
- What is null in ASCII?
0
- Oh i know! just repeat what i say...
- e99a18c428cb38d5f260853678922e03
- You don't understand it? maybe you can ask john
abc123
- Finally just XOR !($+<;.,; with the letter I
hamburger
- so about the Flag, may I tell you a seceRt? oh kingS of egypT, you already have the flag
```

Another riddle? So they say that we already have the flag, and notice how the capital letters in the last sentence spell out `FIRST`. What if we take the first letter of each answer?

Indeed, the flag that was accepted was `BSMCH{ph4r0ah}`.

## Appendix: Reversing the Executable

As mentioned by the description, this isn't a reversing challenge. However, we'll see here how it was easier to solve the challenge with reversing.

If we try to open the executable in Ghidra, we see lots of strings related to Python. The executable is also called `script.exe`. It sounds like it's a Python script packed to an `exe`.

First, we must find a way to reverse the `Python -> exe` process. [This script](https://github.com/extremecoders-re/pyinstxtractor) helps us do that:

```console
PS E:\CTFs\basmach\locust> py -3.11 .\pyinstxtractor.py .\script.exe
[+] Processing .\script.exe
[+] Pyinstaller version: 2.1+
[+] Python version: 3.11
[+] Length of package: 33377958 bytes
[+] Found 121 files in CArchive
[+] Beginning extraction...please standby
[+] Possible entry point: pyiboot01_bootstrap.pyc
[+] Possible entry point: pyi_rth_inspect.pyc
[+] Possible entry point: pyi_rth_pkgres.pyc
[+] Possible entry point: pyi_rth_win32comgenpy.pyc
[+] Possible entry point: pyi_rth_pywintypes.pyc
[+] Possible entry point: pyi_rth_pythoncom.pyc
[+] Possible entry point: pyi_rth_pkgutil.pyc
[+] Possible entry point: pyi_rth_multiprocessing.pyc
[+] Possible entry point: pyi_rth_setuptools.pyc
[+] Possible entry point: script.pyc
[+] Found 748 files in PYZ archive
[+] Successfully extracted pyinstaller archive: .\script.exe

You can now use a python decompiler on the pyc files within the extracted directory
```

We have a `script.pyc` (compiled bytecode) file, but no sources. Solutions such as `uncompyle6` don't support Python 3.11 yet, but we can at least see the bytecode with the `dis` module:

<details>
  <summary>Expand</summary>

```python
>>> import script
>>> help(script.main)
Help on function main in module script:

main()

>>> import dis
>>> dis.dis(script.main)
 24           0 RESUME                   0

 25           2 NOP

 26           4 LOAD_CONST               1 (0)
              6 STORE_FAST               0 (counterLines)

 27           8 LOAD_GLOBAL              1 (NULL + print)
             20 LOAD_CONST               2 ('- If you wanna play a game ask politely!')
             22 PRECALL                  1
             26 CALL                     1
             36 POP_TOP

 28          38 LOAD_GLOBAL              3 (NULL + open)
             50 LOAD_GLOBAL              4 (CRICKET_PATH)
             62 LOAD_CONST               3 ('rb+')
             64 PRECALL                  2
             68 CALL                     2
             78 BEFORE_WITH
             80 STORE_FAST               1 (f)

 29          82 LOAD_FAST                1 (f)
             84 LOAD_METHOD              3 (readlines)
            106 PRECALL                  0
            110 CALL                     0
            120 STORE_FAST               2 (lines)

 30         122 LOAD_FAST                2 (lines)
            124 LOAD_CONST               1 (0)
            126 BINARY_SUBSCR
            136 LOAD_METHOD              4 (strip)
            158 PRECALL                  0
            162 CALL                     0
            172 LOAD_METHOD              5 (decode)
            194 LOAD_CONST               4 ('utf-8')
            196 LOAD_CONST               5 ('ignore')
            198 PRECALL                  2
            202 CALL                     2
            212 STORE_FAST               3 (input)

 31         214 LOAD_FAST                3 (input)
            216 LOAD_METHOD              6 (startswith)
            238 LOAD_CONST               6 ('please')
            240 PRECALL                  1
            244 CALL                     1
            254 EXTENDED_ARG             4
            256 POP_JUMP_FORWARD_IF_FALSE  1094 (to 2446)

 32         258 LOAD_GLOBAL              1 (NULL + print)
            270 LOAD_FAST                3 (input)
            272 PRECALL                  1
            276 CALL                     1
            286 POP_TOP

 33         288 LOAD_FAST                0 (counterLines)
            290 LOAD_CONST               7 (1)
            292 BINARY_OP               13 (+=)
            296 STORE_FAST               0 (counterLines)

 35         298 LOAD_GLOBAL              1 (NULL + print)
            310 LOAD_CONST               8 ('- aGV5ISBJJ20gZ29pbmcgdG8gYXNrIHlvdSBhIGZldyByaWRkbGVzIGFuZCBpZiB5b3UgY2FuIHNvbHZlIHRoZW0gYWxsIEkgd2lsbCBnaXZlIHlvdSB0aGUgZmxhZyEgdG8gcHJvY2VlZCBqdXN0IHJlcGVhdCB0aGlzIG1lc3NhZ2Uu')
            312 PRECALL                  1
            316 CALL                     1
            326 POP_TOP

 36         328 LOAD_FAST                2 (lines)
            330 LOAD_CONST               7 (1)
            332 BINARY_SUBSCR
            342 LOAD_METHOD              4 (strip)
            364 PRECALL                  0
            368 CALL                     0
            378 LOAD_METHOD              5 (decode)
            400 LOAD_CONST               4 ('utf-8')
            402 LOAD_CONST               5 ('ignore')
            404 PRECALL                  2
            408 CALL                     2
            418 STORE_FAST               3 (input)

 37         420 LOAD_FAST                3 (input)
            422 LOAD_METHOD              6 (startswith)
            444 LOAD_CONST               9 ("hey! I'm going to ask you a few riddles and if you can solve them all I will give you the flag!")
            446 PRECALL                  1
            450 CALL                     1
            460 EXTENDED_ARG             3
            462 POP_JUMP_FORWARD_IF_FALSE   925 (to 2314)

 38         464 LOAD_GLOBAL              1 (NULL + print)
            476 LOAD_FAST                3 (input)
            478 PRECALL                  1
            482 CALL                     1
            492 POP_TOP

 39         494 LOAD_FAST                0 (counterLines)
            496 LOAD_CONST               7 (1)
            498 BINARY_OP               13 (+=)
            502 STORE_FAST               0 (counterLines)

 41         504 LOAD_GLOBAL              1 (NULL + print)
            516 LOAD_CONST              10 ("- Let's start easy...")
            518 PRECALL                  1
            522 CALL                     1
            532 POP_TOP

 42         534 LOAD_GLOBAL              1 (NULL + print)
            546 LOAD_CONST              11 ('- What is 2 + 2 = ?')
            548 PRECALL                  1
            552 CALL                     1
            562 POP_TOP

 43         564 LOAD_FAST                2 (lines)
            566 LOAD_CONST              12 (2)
            568 BINARY_SUBSCR
            578 LOAD_METHOD              4 (strip)
            600 PRECALL                  0
            604 CALL                     0
            614 LOAD_METHOD              5 (decode)
            636 LOAD_CONST               4 ('utf-8')
            638 LOAD_CONST               5 ('ignore')
            640 PRECALL                  2
            644 CALL                     2
            654 STORE_FAST               3 (input)

 44         656 LOAD_CONST              13 ('4')
            658 LOAD_FAST                3 (input)
            660 COMPARE_OP               2 (==)
            666 EXTENDED_ARG             2
            668 POP_JUMP_FORWARD_IF_FALSE   756 (to 2182)

 45         670 LOAD_GLOBAL              1 (NULL + print)
            682 LOAD_FAST                3 (input)
            684 PRECALL                  1
            688 CALL                     1
            698 POP_TOP

 46         700 LOAD_FAST                0 (counterLines)
            702 LOAD_CONST               7 (1)
            704 BINARY_OP               13 (+=)
            708 STORE_FAST               0 (counterLines)

 48         710 LOAD_GLOBAL              1 (NULL + print)
            722 LOAD_CONST              14 ("- Beep boop beep beep... I can't find my site on google! what am I missing?")
            724 PRECALL                  1
            728 CALL                     1
            738 POP_TOP

 49         740 LOAD_FAST                2 (lines)
            742 LOAD_CONST              15 (3)
            744 BINARY_SUBSCR
            754 LOAD_METHOD              4 (strip)
            776 PRECALL                  0
            780 CALL                     0
            790 LOAD_METHOD              5 (decode)
            812 LOAD_CONST               4 ('utf-8')
            814 LOAD_CONST               5 ('ignore')
            816 PRECALL                  2
            820 CALL                     2
            830 STORE_FAST               3 (input)

 50         832 LOAD_FAST                3 (input)
            834 LOAD_METHOD              6 (startswith)
            856 LOAD_CONST              16 ('robots')
            858 PRECALL                  1
            862 CALL                     1
            872 EXTENDED_ARG             2
            874 POP_JUMP_FORWARD_IF_FALSE   587 (to 2050)

 51         876 LOAD_GLOBAL              1 (NULL + print)
            888 LOAD_FAST                3 (input)
            890 PRECALL                  1
            894 CALL                     1
            904 POP_TOP

 52         906 LOAD_FAST                0 (counterLines)
            908 LOAD_CONST               7 (1)
            910 BINARY_OP               13 (+=)
            914 STORE_FAST               0 (counterLines)

 54         916 LOAD_GLOBAL              1 (NULL + print)
            928 LOAD_CONST              17 ('- What is null in ASCII?')
            930 PRECALL                  1
            934 CALL                     1
            944 POP_TOP

 55         946 LOAD_FAST                2 (lines)
            948 LOAD_CONST              18 (4)
            950 BINARY_SUBSCR
            960 LOAD_METHOD              4 (strip)
            982 PRECALL                  0
            986 CALL                     0
            996 LOAD_METHOD              5 (decode)
           1018 LOAD_CONST               4 ('utf-8')
           1020 LOAD_CONST               5 ('ignore')
           1022 PRECALL                  2
           1026 CALL                     2
           1036 STORE_FAST               3 (input)

 56        1038 LOAD_FAST                3 (input)
           1040 LOAD_METHOD              6 (startswith)
           1062 LOAD_CONST              19 ('0')
           1064 PRECALL                  1
           1068 CALL                     1
           1078 EXTENDED_ARG             1
           1080 POP_JUMP_FORWARD_IF_FALSE   417 (to 1916)

 57        1082 LOAD_GLOBAL              1 (NULL + print)
           1094 LOAD_CONST               1 (0)
           1096 PRECALL                  1
           1100 CALL                     1
           1110 POP_TOP

 58        1112 LOAD_FAST                0 (counterLines)
           1114 LOAD_CONST               7 (1)
           1116 BINARY_OP               13 (+=)
           1120 STORE_FAST               0 (counterLines)

 60        1122 LOAD_GLOBAL              1 (NULL + print)
           1134 LOAD_CONST              20 ('- Oh i know! just repeat what i say...')
           1136 PRECALL                  1
           1140 CALL                     1
           1150 POP_TOP

 61        1152 LOAD_GLOBAL              1 (NULL + print)
           1164 LOAD_CONST              21 ('- e99a18c428cb38d5f260853678922e03')
           1166 PRECALL                  1
           1170 CALL                     1
           1180 POP_TOP

 62        1182 LOAD_GLOBAL              1 (NULL + print)
           1194 LOAD_CONST              22 ("- You don't understand it? maybe you can ask john")
           1196 PRECALL                  1
           1200 CALL                     1
           1210 POP_TOP

 63        1212 LOAD_FAST                2 (lines)
           1214 LOAD_CONST              23 (5)
           1216 BINARY_SUBSCR
           1226 LOAD_METHOD              4 (strip)
           1248 PRECALL                  0
           1252 CALL                     0
           1262 LOAD_METHOD              5 (decode)
           1284 LOAD_CONST               4 ('utf-8')
           1286 LOAD_CONST               5 ('ignore')
           1288 PRECALL                  2
           1292 CALL                     2
           1302 STORE_FAST               3 (input)

 64        1304 LOAD_FAST                3 (input)
           1306 LOAD_METHOD              6 (startswith)
           1328 LOAD_CONST              24 ('abc123')
           1330 PRECALL                  1
           1334 CALL                     1
           1344 POP_JUMP_FORWARD_IF_FALSE   218 (to 1782)

 65        1346 LOAD_GLOBAL              1 (NULL + print)
           1358 LOAD_FAST                3 (input)
           1360 PRECALL                  1
           1364 CALL                     1
           1374 POP_TOP

 66        1376 LOAD_FAST                0 (counterLines)
           1378 LOAD_CONST               7 (1)
           1380 BINARY_OP               13 (+=)
           1384 STORE_FAST               0 (counterLines)

 68        1386 LOAD_GLOBAL              1 (NULL + print)
           1398 LOAD_CONST              25 ('- Finally just XOR !($+<;.,; with the letter I')
           1400 PRECALL                  1
           1404 CALL                     1
           1414 POP_TOP

 69        1416 LOAD_FAST                2 (lines)
           1418 LOAD_CONST              26 (6)
           1420 BINARY_SUBSCR
           1430 LOAD_METHOD              4 (strip)
           1452 PRECALL                  0
           1456 CALL                     0
           1466 LOAD_METHOD              5 (decode)
           1488 LOAD_CONST               4 ('utf-8')
           1490 LOAD_CONST               5 ('ignore')
           1492 PRECALL                  2
           1496 CALL                     2
           1506 STORE_FAST               3 (input)

 70        1508 LOAD_FAST                3 (input)
           1510 LOAD_METHOD              6 (startswith)
           1532 LOAD_CONST              27 ('hamburger')
           1534 PRECALL                  1
           1538 CALL                     1
           1548 POP_JUMP_FORWARD_IF_FALSE    49 (to 1648)

 71        1550 LOAD_GLOBAL              1 (NULL + print)
           1562 LOAD_FAST                3 (input)
           1564 PRECALL                  1
           1568 CALL                     1
           1578 POP_TOP

 72        1580 LOAD_FAST                0 (counterLines)
           1582 LOAD_CONST               7 (1)
           1584 BINARY_OP               13 (+=)
           1588 STORE_FAST               0 (counterLines)

 74        1590 LOAD_GLOBAL              1 (NULL + print)
           1602 LOAD_CONST              28 ('- so about the Flag, may I tell you a seceRt? oh kingS of egypT, you already have the flag')
           1604 PRECALL                  1
           1608 CALL                     1
           1618 POP_TOP

 75        1620 NOP

...
```
</details>

As you can see, all the answers can be clearly seen inside. Of course, you'd still have the last riddle to solve yourself.