# INCD - Junior Cyber Challenge 2021

This is a short CTF by the Israeli National Cyber Directorate, as part of a recruitment campaign. 

## Junior Cyber Analyst

### Document Challenge

> find the flag

We get a file called `jctf.7zp`:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Document_Challenge]
└─$ file jctf.7zp
jctf.7zp: Microsoft Word 2007+
```

A Word document file is just an archive:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Document_Challenge]
└─$ 7z l jctf.7zp

7-Zip [64] 16.02 : Copyright (c) 1999-2016 Igor Pavlov : 2016-05-21
p7zip Version 16.02 (locale=en_IL,Utf16=on,HugeFiles=on,64 bits,4 CPUs 12th Gen Intel(R) Core(TM) i9-12900K (90672),ASM)

Scanning the drive for archives:
1 file, 12228 bytes (12 KiB)

Listing archive: jctf.7zp

--
Path = jctf.7zp
Type = zip
Physical Size = 12228

   Date      Time    Attr         Size   Compressed  Name
------------------- ----- ------------ ------------  ------------------------
1980-01-01 00:00:00 .....         1312          346  [Content_Types].xml
1980-01-01 00:00:00 .....          590          239  _rels/.rels
1980-01-01 00:00:00 .....         4102          983  word/document.xml
1980-01-01 00:00:00 .....          817          244  word/_rels/document.xml.rels
1980-01-01 00:00:00 .....         8393         1746  word/theme/theme1.xml
1980-01-01 00:00:00 .....         3014         1054  word/settings.xml
1980-01-01 00:00:00 .....        29455         2949  word/styles.xml
1980-01-01 00:00:00 .....          894          334  word/webSettings.xml
1980-01-01 00:00:00 .....         1922          514  word/fontTable.xml
1980-01-01 00:00:00 .....          733          361  docProps/core.xml
1980-01-01 00:00:00 .....          714          370  docProps/app.xml
------------------- ----- ------------ ------------  ------------------------
1980-01-01 00:00:00              51946         9140  11 files
```

We can rename to `docx` and open with Word, or just extract the strings from `word/document.xml`

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Document_Challenge]
└─$ sed -e 's/<[^>]*>/\n/g' document.xml | strings
Dfsdkjfhsldkf;lkfhsd
;lfkhsddfghksfglk
Skhdflskdfhlksksdjfhksjdfhskdjfhskdjf
Hjfskjdfhksdjfhskdjfsdkfhsdkfjhkjhsdf
ZmxhZyh5b3UgZm91bmQgbWUhISEp
```

The last one looks like base64:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Document_Challenge]
└─$ echo ZmxhZyh5b3UgZm91bmQgbWUhISEp | base64 -d
flag(you found me!!!)
```

### Network Challenge

> Find the Flag

We get a network capture file. We extract the packet contents and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Network_Challenge]
└─$ tshark -r ctfpcap.pcap -T fields -e data.text -o data.show_as_text:TRUE  2>/dev/null | strings
flag(user:incdctf|pass:WeWantYour$ki11$)
flag(user:incdctf|pass:WeWantYour$ki11$)
```

### Picture Challenge

> Find the Flag

We get a PNG image. Let's check it:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Picture_Challenge]
└─$ pngcheck INCD.png
INCD.png  additional data after IEND chunk
ERROR: INCD.png
```

After the `IEND` chunk there is extra data:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD_2021/Junior_Cyber_Analyst/Picture_Challenge]
└─$ strings INCD.png | tail
Zn~mQ
oC^6
Li*!
Rv(:
Rv(:
Rv(:
Rv(:
Rv(:
IEND
01000110 01101100 01100001 01100111 01111011 01111001 01101111 01110101 00100000 01100110 01101111 01110101 01101110 01100100 00100000 01101101 01100101 00100001 00100001 00100001 01111101
```

Decode as binary to get the flag: `Flag{you found me!!!}`


## Junior Threat Intelligence Analyst

### Last Mile

> On your first day on the job, you received a notification that someone had uploaded sensitive material to where over 65 million developers share the future of software, together .
>
> Your task is to find the leak and act on what you have discovered
>
> Your leak indicators: INCDGOV 641636f0faa0eda117eda14ad309fb5a306b1c50 
>
> Good luck !

We search Google for the phrase *"where over 65 million developers share the future of software, together"* and find GitHub.

From there we find [this commit](https://github.com/INCDGOV/Junior_CTF/blob/47779cdbea3c4fe9b2e2dc0730cb89fb29293df3/641636f0faa0eda117eda14ad309fb5a306b1c50).

The contents contains the flag:

```
641636f0faa0eda117eda14ad309fb5a306b1c50
 _____       _                 _____                   _ 
/  __ \     | |               |_   _|                 | |
| /  \/_   _| |__   ___ _ __    | | ___ _ __ __ _  ___| |
| |   | | | | '_ \ / _ \ '__|   | |/ __| '__/ _` |/ _ \ |
| \__/\ |_| | |_) |  __/ |     _| |\__ \ | | (_| |  __/ |
 \____/\__, |_.__/ \___|_|     \___/___/_|  \__,_|\___|_|
        __/ |                                            
       |___/       
       '''


Congratulations on Completing Your Challenge!
Your flag is: cyberm@cyber.gov.il

Please send your CV, the code "31/21" and your registered email to the challange via our Email address: cyberm@cyber.gov.il

Stay safe ;
```