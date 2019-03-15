# Snakes over cheese
Reversing, 100 points

## Description:

> What kind of file is this?

A *.pyc file was attached.

## Solution:

PYC files are compiled Python. Let's decompile the file:

```console
root@kali:/media/sf_CTFs/tamu/Snakes_over_cheese# uncompyle6 reversing2.pyc
# uncompyle6 version 3.2.5
# Python bytecode 2.7 (62211)
# Decompiled from: Python 2.7.15 (default, May  1 2018, 05:55:50)
# [GCC 7.3.0]
# Embedded file name: reversing2.py
# Compiled at: 2018-10-07 22:28:58
from datetime import datetime
Fqaa = [102, 108, 97, 103, 123, 100, 101, 99, 111, 109, 112, 105, 108, 101, 125]
XidT = [83, 117, 112, 101, 114, 83, 101, 99, 114, 101, 116, 75, 101, 121]

def main():
    print 'Clock.exe'
    input = raw_input('>: ').strip()
    kUIl = ''
    for i in XidT:
        kUIl += chr(i)

    if input == kUIl:
        alYe = ''
        for i in Fqaa:
            alYe += chr(i)

        print alYe
    else:
        print datetime.now()


if __name__ == '__main__':
    main()
# okay decompiling reversing2.pyc
```

Logic is pretty straightforward, we can recover the key with the following code:

```python
>>> XidT = [83, 117, 112, 101, 114, 83, 101, 99, 114, 101, 116, 75, 101, 121]
>>> "".join(chr(x) for x in XidT)
'SuperSecretKey'
```

Then enter it when the script asks for it:

```console
root@kali:/media/sf_CTFs/tamu/Snakes_over_cheese# python reversing2.pyc
Clock.exe
>: SuperSecretKey
flag{decompile}
```

