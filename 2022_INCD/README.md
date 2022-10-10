# INCD - Junior Cyber Challenge 2022

This is a short CTF by the Israeli National Cyber Directorate, as part of a recruitment campaign. 
I didn't save the questions themselves but will try to create a very basic & lightweight writeup the challenges based on my notes.

## Junior Cyber Intelligence and IR Researcher

### Challenge #1

We get an event log file and need to find a username.

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Cyber_Intelligence_and_IR_Researcher/3]
└─$ file SecurityLog.evtx
SecurityLog.evtx: MS Windows Vista Event Log, 4 chunks (no. 3 in use), next record no. 157
```

We export the file to text with `evtxexport`:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Cyber_Intelligence_and_IR_Researcher/3]
└─$ evtxexport SecurityLog.evtx > out.txt
```

Then search for users:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Cyber_Intelligence_and_IR_Researcher/3]
└─$ cat out.txt| grep -i user
String: 9                       : "net.exe" USER support_388945a0 password /add /y /expires:never
String: 9                       : C:\Windows\system32\net1 USER support_388945a0 password /add /y /expires:never
String: 3                       : Users
String: 9                       : "C:\Windows\system32\NOTEPAD.EXE" C:\Users\nuckc.VFOREST\Desktop\sec.txt
```

`support_388945a0` was accepted as the answer.

### Challenge #2

We get the following login page:

```html
<!DOCTYPE Html />
<html>
    <head>
        <title>IR team CTF</title>
    </head>
    <body>
        <input type="text" name="flag" id="flag" value="Enter the flag" />
        <input type="button" id="prompt" value="Click to check the flag" />
        <script type="text/javascript">
            document.getElementById("prompt").onclick = function () {
                console.log(document.getElementById("flag").value)
                var flag = document.getElementById("flag").value;
                var rotFlag = flag.replace(/[a-zA-Z]/g, function(c){return String.fromCharCode((c <= "Z" ? 90 : 122) >= (c = c.charCodeAt(0) + 13) ? c : c - 26);});
                console.log(rotFlag)
                if ("plorez@plore.tbi.vy" == rotFlag) {
                    alert("Correct flag!, you are on the right way to the IR ");
                } else {
                    alert("Ops, Incorrect flag, rot again");
                }
            }
        </script>
    </body>
</html>
```

We need to find an input that will result in `"plorez@plore.tbi.vy"` after the given manipulations. This is simply ROT13, and the answer is `cyberm@cyber.gov.il`.

## Junior Vulnerability and Biometric Cyber Analyst Researcher

### Challenge #1

We get a PCAP network capture. Follow the TCP streams. There are several red herrings. The flag is found in one of the messages of `ip.addr == 133.7.133.7`, encoded in base62 and then base64:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Biometric_Cyber_Analyst_Researcher/1]
└─$ tshark -r pcapb.pcapng  -Y "ip.addr == 133.7.133.7" -T fields -e data.text -o data.show_as_text:TRUE
1OXEuiYOMOZxtTrPpzQzT5rfozAmR8E5OHcoYIJXSeWFi56hrE2wkRh9kubs2fncnA50u0a21gS3XIjXnAHLNXtkNOrE1XWaeghCBjh9bAyGxKEcKJS2nzxRBgTE7WbOud1P87E
EgaJxMZf1WQNuCSzeGrVad6novTEem
EgaJxMZf1WQNuCSzeGrVad6novTEem
b29vb29vb29vb29vb29vb29vb28gbm8gVGhlIGZsYWcgaXMgbm90IGhlcmU=
dGhlIGZsYWcgaXM6IG5vdCBoZXJlIDo=

┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Biometric_Cyber_Analyst_Researcher/1]
└─$ node
Welcome to Node.js v18.10.0.
Type ".help" for more information.
> const chef = require("/home/user/utils/CyberChef/");
undefined
> chef.fromBase64(chef.fromBase62("1OXEuiYOMOZxtTrPpzQzT5rfozAmR8E5OHcoYIJXSeWFi56hrE2wkRh9kubs2fncnA50u0a21gS3XIjXnAHLNXtkNOrE1XWaeghCBjh9bAyGxKEcKJS2nzxRBgTE7WbOud1P87E"))
Congratulations you captured the flag! The picture needs me: INCD_2022_FLAG
>
```

### Challenge #2

We get the following script and need to fix it:

```python

# counts the uppercase letters in a string
def count_uppercase(lst):
    return sum(letter.isupper() for letter in word for word in lst)


# removes all the numbers from a string and returns the char in index 17 of the remaining string
def remove_numbers(string):
    str = "".join(i for i in string if not i.int())
    return ord(str[17])


# removes all the abc chars from a string and slicing the string from index 13 to 15
def remove_chars(string):
    chars = "abcdefghijklmnopqrstuvwxyzABCEFGHIJKLMNOPQRSTUVWXYZ"
    for char in chars:
        string = string.replace(char, "")
    return chr(int(string[13:15]))


# fibonacci + 12
def fibonacci(num):
    a, b = 1, 1
    for i in range(1, num):
        a, b = b, a+b
    return b + 12


# decimal to ascii
def dec_to_ascii(a):
    return chr(a)


# get current year
def get_year():
    a = datetime.date.today().year
    return a


if __name__ == '__main__':
    first_var = ["SOLO", "hEllO", "Tea", "wHat", "Green", "Oolong", "Matcha", "WhIte", "Pu-Erh", "YelLow", "Mulberry black",
           "Chai", "CeiLlon", "Aerobacter","AeroBic", "aerodynaMic", "aeRoFoil", "aerogene", "AeroNauTic", "AerOsoL",
           "AerOSpace", "AeschyluS", "AesOp", "aesthete", "aesThetiC", "afar", "affable", "afair", "affect",
           "AFFeCtate", "hOrsE", "HorSebacK", "horsedom", "horsefLesh", "horsefly", "horSehair", "hoRseMaN",
           "HorSemen", "horsePlAy", "HoRsePower", "HoRSeshOe", "HorsEtaIl", "hOrseWomaN"]

    second_var = "I45gawq$645@8209CP356AD87N5686yAdseNa1789oWqret54XdsQe32"
    third_var = 9
    fourth_var = "I45gawq$645@8209CP356As87L5D36yAdseNa1789oWqret54XdsQe32"
    fifth_var = ["SOlO", "hEllO", "Tea", "wHat", "Green", "Oolong", "Matcha", "WhIte", "Pu-Erh", "YelLow", "Mulberry black",
           "Chai", "CeiLlon", "Aerobacter","AeroBic", "aerodynaMic", "aeRoFoil", "aerogene", "AeroNauTic", "AerOsoL",
           "AerOSpace", "AeschyluS", "AesOp", "aesthete", "aesThetiC", "afar", "affable", "afair", "affect",
           "AFFectate", "hOrsE", "HorSebacK", "horsedom", "horseflesh", "horsefly", "horSehair", "hoRseMaN",
           "HorSemen", "horsePlAy", "HoRsePower", "HoRSeshOe", "HorsEtaIl", "hOrseWomaN"]
    sixth_var = "I45gawq$645@8L09CP356AD87L5686yAdsLNa1789oWqret54XdsQe32"
    seventh_var = 9

    flag = str(count_uppercase(first_var)) + str(remove_numbers(second_var)) + \
           str(fibonacci(third_var)) + remove_chars(fourth_var) + "_" + str(get_year()) + "_PYTHON_" + \
           str(count_uppercase(fifth_var)) + str(remove_numbers(sixth_var)) + \
           str(fibonacci(seventh_var) - 2) + str(71)

    print("Congratulations! You have captured the {}".format(flag))

```

These are the changes needed:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Biometric_Cyber_Analyst_Researcher/3]
└─$ diff incd_ctf_buggy.py incd_ctf_fixed.py
1a2,3
> import datetime
>
4c6
<     return sum(letter.isupper() for letter in word for word in lst)
---
>     return chr(sum(letter.isupper() for word in lst for letter in word))
9,10c11,12
<     str = "".join(i for i in string if not i.int())
<     return ord(str[17])
---
>     s = "".join(i for i in string if not i.isdigit())
>     return s[17]
18c20
<     return chr(int(string[13:15]))
---
>     return string[13:15]
59c61
<            str(fibonacci(third_var)) + remove_chars(fourth_var) + "_" + str(get_year()) + "_PYTHON_" + \
---
>            str(chr(fibonacci(third_var))) + chr(int(remove_chars(fourth_var))) + "_" + str(get_year()) + "_PYTHON_" + \
61c63
<            str(fibonacci(seventh_var) - 2) + str(71)
---
>            str(chr(fibonacci(seventh_var) - 2)) + str(chr(71))
```

The result:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Biometric_Cyber_Analyst_Researcher/3]
└─$ python3 incd_ctf_fixed.py
Congratulations! You have captured the INCD_2022_PYTHON_FLAG
```

## Junior Vulnerability and Cyber Analyst Researcher

### Challenge #1

We get a PNG image. We run some basic tests on it:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Cyber_Analyst_Researcher/2]
└─$ pngcheck incd_picture_challange.PNG
incd_picture_challange.PNG  additional data after IEND chunk
ERROR: incd_picture_challange.PNG
```

We extract it with `binwalk`:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Junior_Vulnerability_and_Cyber_Analyst_Researcher/2]
└─$ binwalk --dd='.*' incd_picture_challange.PNG

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             PNG image, 788 x 563, 8-bit/color RGBA, non-interlaced
91            0x5B            Zlib compressed data, compressed
652877        0x9F64D         7-zip archive data, version 0.4
```

It's a 7Zip archive, extract it again:

```console
┌──(user@kali)-[/media/…/INCD/Junior_Vulnerability_and_Cyber_Analyst_Researcher/2/_incd_picture_challange.PNG.extracted]
└─$ 7z e 9F64D

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,4 CPUs 12th Gen Intel(R) Core(TM) i9-12900K (90672),ASM)

Scanning the drive for archives:
1 file, 13843 bytes (14 KiB)

Extracting archive: 9F64D
--
Path = 9F64D
Type = 7z
Physical Size = 13843
Headers Size = 162
Method = LZMA2:24k
Solid = -
Blocks = 1

Everything is Ok

Size:       18432
Compressed: 13843
```

We get an encrypted `docx` file:

```console
┌──(user@kali)-[/media/…/INCD/Junior_Vulnerability_and_Cyber_Analyst_Researcher/2/_incd_picture_challange.PNG.extracted]
└─$ file incd_picture_challange.docx
incd_picture_challange.docx: CDFV2 Encrypted
```

Decrypt it using the password from a previous challenge:

```console
┌──(user@kali)-[/media/…/INCD/Junior_Vulnerability_and_Cyber_Analyst_Researcher/2/_incd_picture_challange.PNG.extracted]
└─$ msoffcrypto-tool incd_picture_challange.docx decrypted.docx -p INCD_2022_FLAG
```

We get the following text:

```
01010001 00110010 00111001 01110101 01011010 00110011 01001010 01101000 01100100 01001000 01010110 01110011 01011001 01011000 01010010 01110000 01100010 00110010 00110101 01111010 01001001 01010011 01000010 01011010 01100010 00110011 01010101 01100111 01100001 01000111 01000110 00110010 01011010 01010011 01000010 01101010 01011001 01011000 01000010 00110000 01100100 01011000 01001010 01101100 01011010 01000011 01000010 00110000 01100001 01000111 01010101 01100111 01011010 01010111 00110101 01101010 01100011 01101110 01101100 01110111 01100100 01000111 01010110 01101011 01001001 01001000 01000010 01110000 01011001 00110011 01010010 00110001 01100011 01101101 01010101 01100111 01011010 01101101 01111000 01101000 01011010 01111010 01101111 01100111 01010011 01010101 00110101 01000100 01010010 01000110 00111000 01111001 01001101 01000100 01001001 01111001 01011000 00110001 01000010 01001010 01010001 00110001 01010010 01010110 01010101 01101011 01010110 01100110 01010010 01101011 01111000 01000010 01010010 01110111 00111101 00111101
```

Decoded from binary and then base64, it becomes:

```
Congratulations! You have captured the encrypted picture flag: INCD_2022_PICTURE_FLAG
```