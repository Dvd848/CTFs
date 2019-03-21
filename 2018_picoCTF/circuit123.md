# circuit123
Reversing, 800 points

## Description:
> Can you crack the key to decrypt map2 for us? The key to map1 is 11443513758266689915. 

```python
#!/usr/bin/python2

from hashlib import sha512
import sys

def verify(x, chalbox):
    length, gates, check = chalbox
    b = [(x >> i) & 1 for i in range(length)]
    for name, args in gates:
        if name == 'true':
            b.append(1)
        else:
            u1 = b[args[0][0]] ^ args[0][1]
            u2 = b[args[1][0]] ^ args[1][1]
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
    return b[check[0]] ^ check[1]
    
def dec(x, w):
    z = int(sha512(str(int(x))).hexdigest(), 16)
    return '{:x}'.format(w ^ z).decode('hex')

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: ' + sys.argv[0] + ' <key> <map.txt>'
        print 'Example: Try Running ' + sys.argv[0] + ' 11443513758266689915 map1.txt'
        exit(1)
    with open(sys.argv[2], 'r') as f:
        cipher, chalbox = eval(f.read())
    
    key = int(sys.argv[1]) % (1 << chalbox[0])
    print 'Attempting to decrypt ' + sys.argv[2] + '...'
    if verify(key, chalbox):
        print 'Congrats the flag for ' + sys.argv[2] + ' is:', dec(key, cipher)
    else:
        print 'Wrong key for ' + sys.argv[2] + '.'
```

Map1.txt:
```
(1091562780682878452932647567206562803258945860781462102555439111325671293639822353361220777655154004326830877696329866178864341430343894025596404608627826L, (64, [('true', []), ('xor', [(0, False), (64, False)]), ('xor', [(65, False), (64, True)]), ('or', [(64, True), (64, False)]), ('or', [(0, True), (64, False)]), ('or', [(67, True), (68, True)]), ..., ('or', [(560, False), (567, False)]), ('or', [(553, False), (568, False)]), ('or', [(538, False), (569, False)])], (570, True)))
```

Map2.txt:
```
(11290419911155290710690302751351816427340816196576026120444648063369847565343076531411187044376577503480139343099182304342421923153437113486849423485713547L, (128, [('true', []), ('xor', [(0, False), (64, False)]), ('xor', [(129, False), (128, True)]), ('or', [(64, True), (128, False)]), ('or', [(0, True), (128, False)]), ('or', [(131, True), (132, True)]), ('or', [(0, True), (64, True)]), ..., ('or', [(1076, False), (1139, False)])], (1140, True)))
```

Hint:
> Have you heard of z3?

## Solution:

We can see that the key is decomposed into an array of bits:

```python
b = [(x >> i) & 1 for i in range(length)]
```

This array is later extended by a series of logical gates read from the map file:
```python
    for name, args in gates:
        if name == 'true':
            b.append(1)
        else:
            u1 = b[args[0][0]] ^ args[0][1]
            u2 = b[args[1][0]] ^ args[1][1]
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
```

Finally one last calculation is made, forcing the result to match some expected result read from the file:

```python
return b[check[0]] ^ check[1]
```

Therefore, we can actually say that the key is any number which complies with the set of constrains tested in `verify()`.
This is a task for a constraint solver such as `z3`.

We only need some minor modifications in order to solve the set of constraints with `z3`:

```python
from z3 import *
from hashlib import sha512
import sys

def verify(x, chalbox):
    length, gates, check = chalbox
    b = [(x >> i) & 1 for i in range(length)]
    for name, args in gates:
        if name == 'true':
            b.append(1)
        else:
            u1 = b[args[0][0]] ^ args[0][1]
            u2 = b[args[1][0]] ^ args[1][1]
            if name == 'or':
                b.append(u1 | u2)
            elif name == 'xor':
                b.append(u1 ^ u2)
    return b[check[0]] ^ check[1]
    
def dec(x, w):
    z = int(sha512(str(int(x))).hexdigest(), 16)
    return '{:x}'.format(w ^ z).decode('hex')

if __name__ == '__main__':
    s = Solver()

    #if len(sys.argv) < 3:
    #    print 'Usage: ' + sys.argv[0] + ' <key> <map.txt>'
    #    print 'Example: Try Running ' + sys.argv[0] + ' 11443513758266689915 map1.txt'
    #    exit(1)

    if len(sys.argv) < 2:
        print 'Usage: ' + sys.argv[0] + ' <map.txt>'
        exit(1)

    with open(sys.argv[1], 'r') as f:
        cipher, chalbox = eval(f.read())
    
    #key = int(sys.argv[1]) % (1 << chalbox[0])
    #print 'Attempting to decrypt ' + sys.argv[2] + '...'
    #if verify(key, chalbox):
    #    print 'Congrats the flag for ' + sys.argv[2] + ' is:', dec(key, cipher)
    #else:
    #    print 'Wrong key for ' + sys.argv[2] + '.'

    key = BitVec('key', chalbox[0])
    s.add(verify(key, chalbox) != 0)
    if s.check() == sat:
        m = s.model()
        print(m[key])
        print dec(m[key].as_long(), cipher)
```

`verify()` and `dec()` remain unmodified. The main function uses a `BitVec` (a `z3` data type) instead of a given key, and finds a solution where this `BitVec` complies with the single constraint of `verify(key, chalbox) != 0`.

The output:
```console
root@kali:/media/sf_CTFs/pico/circuit123# python solve.py map1.txt
11443513758266689915
not_a_flag{Real_flag_will_be_loooooooonger_than_me}
root@kali:/media/sf_CTFs/pico/circuit123# python solve.py map2.txt
219465169949186335766963147192904921805
picoCTF{36cc0cc10d273941c34694abdb21580d__aw350m3_ari7hm37ic__}
```

The flag: picoCTF{36cc0cc10d273941c34694abdb21580d__aw350m3_ari7hm37ic__}