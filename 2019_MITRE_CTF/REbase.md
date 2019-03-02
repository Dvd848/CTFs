# REbase
Binary RE, 400 points

## Description:
> You receive an ELF binary which you must unlock with a key. Find the key and it will contain your flag.



## Solution:

Let's start by inspecting the binary:
```console
root@kali:/media/sf_CTFs/mitre/rebase# file REbase-fix
REbase-fix: ELF 64-bit LSB executable, x86-64, version 1 (GNU/Linux), statically linked, stripped
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix
Usage: ./REbase flag
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix test
4
rUC/C/==
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix my_flag
7
wBsyMdbibi==
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
```

It looks like the program is outputting base64-encoded messages, let's try to decode them:

```console
root@kali:/media/sf_CTFs/mitre/rebase# echo rUC/C/== | base64 -d && echo
@
root@kali:/media/sf_CTFs/mitre/rebase# echo wBsyMdbibi== | base64 -d && echo
1n
root@kali:/media/sf_CTFs/mitre/rebase# echo ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8= | base64 -d && echo
N2_fy"s
```

This does not appear to be meaningful. However the resemblance to Base64 behavior is very high:
```console
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix a
1
QQ==
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix aa
2
QWi=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix aaa
3
NVYi
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
(root@kali:/media/sf_CTFs/mitre/rebase# echo -n 'a' | base64
YQ==
root@kali:/media/sf_CTFs/mitre/rebase# echo -n 'aa' | base64
YWE=
root@kali:/media/sf_CTFs/mitre/rebase# echo -n 'aaa' | base64
YWFh)
```

It looks like the padding character is identical to Base64 ("`=`"), and that each encoded character represents 6 bits of data. Therefore, three 8-bit bytes are represented by four 6-bit encoded characters - just like Base64.

Another observation is that if we try to enter "`MCA{`" as the prefix for the flag, we get a prefix which is identical to the second (constant) encoded string that the program is printing:
```console
root@kali:/media/sf_CTFs/mitre/rebase# ./REbase-fix MCA{test_flag}
14
ZXFWt2Kse2KyMdbiUY8=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
```

Base64 is based on a dictionary of 64 characters where every character represents an index. The standard dictionary is `"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"`. 
The algorithm iterates the input string as 6-bit indices, and for each index uses the appropriate character from the dictionary. For example, "A" is 0, and "B" is 1.

In standard Base64, the string "MCA" will be decoded as `TUNB`:

```
M        C        A       
01001101 01000011 01000001 

# Arrange as 6-bit indices:
010011 010100 001101 000001 
T-19   U-20   N-13   B-1
```

In our case, "MCA" is decoded as `ZXFW` - so it looks like we are using a different dictionary, one where "Z" is in the 19th place, "U" is in the 20th, "N" is 13 and "W" is 1.

The naive implementation usually uses a long string to represent the dictionary, so let's look for one in the code:

```console
# strings REbase-fix | grep -x '.\{30,\}'
VBNMqwertyuiopasdfghjklzxcvbn/+m
!"''''$&(,''''-./0''''1349''''EFGH''''ISTU''''VWXY''''Z[\]''''^_`a''''bcde''''fghi''''jklm''''nopq''''rstu''''vwxy''''z{|}''''~
MN''''OPRT''''UVWY''''Z[\_''''`abc''''defh''''ijk
NNNN !"#NNNN$%&'NNNN()*+NNNN,-./NNNN0123NNNN4567NNNN89:;NNNN<=>?NNNN@ABCNNNNDEFGNNNNHIJKNNNNLMNONNNNPQRSNNNNTVWXNNNNYZ[\NNNN]^_`NNNNabcdNNNNefghNNNNijklNNNNmnopNNNNqrstNNNNuvwxNNNNyz{|NNNN}~
 ''''!"#$''''%&'('''')*+,''''-./0''''1234''''5678''''9;<=''''>@AB''''CDFJ''''KLMN''''OPRS''''TUVW''''XYZ[''''\]^_''''`abc''''defg''''hijk''''lmno''''pqrs''''tuvw''''xyz{''''|}~
$Info: This file is packed with the UPX executable packer http://upx.sf.net $
$Id: UPX 3.95 Copyright (C) 1996-2018 the UPX Team. All Rights Reserved. $
```

The command above shows only strings of length >= 30. The first string looks almost like our dictionary (notice the suffix which ends with `/+` - just like in the standard Base64 dictionary) - but it's much shorter than expected. However, another interesting string has surfaced from this query: "This file is packed with the UPX executable packer".

Let's try to unpack it and see if it helps:
```
root@kali:/media/sf_CTFs/mitre/rebase# upx -d REbase-fix
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2018
UPX 3.95        Markus Oberhumer, Laszlo Molnar & John Reiser   Aug 26th 2018

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
    682696 <-    274236   40.17%   linux/amd64   REbase-fix

Unpacked 1 file.
```

Searching for long strings again:
```console
root@kali:/media/sf_CTFs/mitre/rebase# strings REbase-fix | grep -x '.\{30,\}' | head
QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbn/+m1234567890
__ehdr_start.e_phentsize == sizeof *GL(dl_phdr)
Unexpected reloc type in static binary.
FATAL: cannot determine kernel version
cannot set %fs base address for thread-local storage
%s%s%s:%u: %s%sAssertion `%s' failed.
Fatal error: glibc detected an invalid stdio handle
%s%s%s:%u: %s%sAssertion `%s' failed.
replaced_arena->attached_threads > 0
malloc_consolidate(): invalid chunk size
```

Now we have many more results, and the first one looks very much like a Base64 dictionary!

Let's test this theory with a basic Base64 implementation (based on [this](https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64)):


```python
import re

def base64_encode(s, dictionary):
    # the result/encoded string, the padding string, and the pad count
    r = ""
    p = ""
    c = len(s) % 3

    # add a right zero pad to make this string a multiple of 3 characters
    if (c > 0):
        for i in range(c, 3):
            p += '='
            s += "\0"

    # increment over the length of the string, three characters at a time
    for c in range(0, len(s), 3):
        # these three 8-bit (ASCII) characters become one 24-bit number
        n = (ord(s[c]) << 16) + (ord(s[c+1]) << 8) + (ord(s[c+2]))

        # this 24-bit number gets separated into four 6-bit numbers
        n = [(n >> 18) & 0x3F, (n >> 12) & 0x3F, (n >> 6) & 0x3F, n & 0x3F]

        # those four 6-bit numbers are used as indices into the base64 character list
        r += dictionary[n[0]] + dictionary[n[1]] + dictionary[n[2]] + dictionary[n[3]]
    
    # add the actual padding string, after removing the zero pad
    return r[0:len(r) - len(p)]  + p



def base64_decode(s, dictionary):
    base64inv = {}
    for i in range(len(dictionary)):
        base64inv[dictionary[i]] = i

    s = s.replace("\n", "")
    if not re.match(r"^([{alphabet}]{{4}})*([{alphabet}]{{3}}=|[{alphabet}]{{2}}==)?$".format(alphabet = dictionary), s):
        raise ValueError("Invalid input: {}".format(s))

    if len(s) == 0:
        return ""

    # replace any incoming padding with a zero pad (the 'A' character is zero)
    p = "" if (s[-1] != "=") else "AA" if (len(s) > 1 and s[-2] == "=") else "A"

    r = ""
    s = s[0:len(s) - len(p)] + p

    # increment over the length of this encoded string, four characters at a time
    for c in range(0, len(s), 4):

        # each of these four characters represents a 6-bit index in the base64 characters list
        #  which, when concatenated, will give the 24-bit number for the original 3 characters
        n = (base64inv[s[c]] << 18) + (base64inv[s[c+1]] << 12) + (base64inv[s[c+2]] << 6) + base64inv[s[c+3]]

        # split the 24-bit number into the original three 8-bit (ASCII) characters
        r += chr((n >> 16) & 255) + chr((n >> 8) & 255) + chr(n & 255)
    
    # remove any zero pad that was added to make this a multiple of 24 bits
    return r[0:len(r) - len(p)]

def test_base64():
    import base64
    import string
    import random

    dictionary = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
    
    def random_string(length):
        return ''.join(random.choice(string.ascii_letters) for m in range(length))
        
    for i in range(100):
        s = random_string(i)
        encoded = base64_encode(s, dictionary)
        assert(encoded == base64.b64encode(s))
        assert(s == base64_decode(encoded, dictionary))


if __name__ == "__main__":
    #test_base64()

    dictionary =  "QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbn/+m1234567890"
    print(base64_decode("ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=", dictionary), end='')
```

The output:
```console
root@kali:/media/sf_CTFs/mitre/rebase# python3 my_base64.py
MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33z
root@kali:/media/sf_CTFs/mitre/rebase# python3 my_base64.py | xxd -g 1
00000000: 4d 43 41 7b 54 68 31 35 5f 77 55 7a 5f 45 61 5a  MCA{Th15_wUz_EaZ
00000010: 79 5f 50 65 40 5a 79 5f 4c 33 6d 30 6e 5f 53 71  y_Pe@Zy_L3m0n_Sq
00000020: 55 33 33 7a 1e 9f                                U33z..
```

Looks like the algorithm is producing two non-ASCII characters at the end of the string: `0x1e` and `0x9f`.

What will the program tell us if we enter this result as input?

```
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix $(python3 my_base64.py)
39
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5IlAy
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
```

Almost there, but not quite...

We know that the flag should end with "}", let's try guessing some options:
```console
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33za}
38
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Congratulations!
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33zb}
38
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Congratulations!
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33ze}
38
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Congratulations!
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33zy}
38
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Congratulations!
root@kali:/media/sf_CTFs/mitre/rebase# ./REBase-fix MCA{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33zy{
38
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih6=
ZXFWtmKgDZCyrmC5B+CiVfsyXUCQVfsyZRFzDU4yX2YCD/F5Ih8=
Try Again :(
```

Looks like anything we input as the last letter of the flag contents is accepted, as long as the last character is `}`. The output produced for these trials is identical, and matches the expected output of the program. This actually looks to me like a bug in the program, and is definitely not base64-compliant (no two strings should map to the same encoded value).

The server accepted only `{Th15_wUz_EaZy_Pe@Zy_L3m0n_SqU33zy}` as the flag, which was easy to guess from the prefix.