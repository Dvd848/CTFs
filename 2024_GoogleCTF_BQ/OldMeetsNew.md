# OldMeetsNew

Category: Forensics

## Description

> Does it kind of defeat the purpose of me learning Morse code if I have to send it as a WAV anyway? Especially when the file won't even load properly!

A `*.wav` file, `morse_challenge.wav`, was attached.

## Solution

The attached file has a `*.wav` prefix, but media players will fail to play it, and `file` won't
recognize it as a WAV file:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/OldMeetsNew]
└─$ file morse_challenge.wav
morse_challenge.wav: data
```

WAV files start with a magic signature of `RIFF`, but this one seems to start with `WIFF`:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/OldMeetsNew]
└─$ xxd -g 1 morse_challenge.wav| head
00000000: 57 49 46 46 ec a7 03 00 57 41 56 45 66 6d 74 20  WIFF....WAVEfmt
00000010: 10 00 00 00 01 00 01 00 40 1f 00 00 40 1f 00 00  ........@...@...
00000020: 01 00 08 00 64 61 74 61 c8 a7 03 00 80 83 91 ab  ....data........
00000030: cb e3 ec df bd 8d 58 2b 0c 04 14 37 69 9e ce f0  ......X+...7i...
00000040: fc f2 d3 a5 70 3e 18 05 09 23 4e 82 b6 e0 f8 fa  ....p>...#N.....
00000050: e6 bf 8d 58 2a 0d 04 12 35 65 9a ca ed fc f4 d7  ...X*...5e......
00000060: a9 74 42 1a 06 08 20 49 7d b1 dc f7 fb e9 c3 92  .tB... I}.......
00000070: 5d 2e 0e 04 10 31 60 95 c6 eb fc f6 da ae 79 46  ]....1`.......yF
00000080: 1d 07 07 1d 45 78 ad d9 f5 fc eb c8 96 61 32 11  ....Ex.......a2.
00000090: 04 0e 2d 5b 90 c2 e8 fb f7 dd b2 7e 4a 20 08 06  ..-[.......~J ..
```

Now we can use `morse2ascii` to translate the morse to a string:

```
┌──(user@kali3)-[/media/sf_CTFs/google/OldMeetsNew]
└─$ morse2ascii morse_challenge_modified.wav

MORSE2ASCII 0.2
by Luigi Auriemma
e-mail: aluigi@autistici.org
web:    aluigi.org

- open morse_challenge_modified.wav
  wave size      239560
  format tag     1
  channels:      1
  samples/sec:   8000
  avg/bytes/sec: 8000
  block align:   1
  bits:          8
  samples:       239560
  bias adjust:   0
  volume peaks:  -31744 31744
  normalize:     1023
  resampling to  8000hz

- decoded morse data:
izgecr33nbqxmzk7nzxv64tfnvxxe43fpu=======
```

That might be base32, let's try:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/OldMeetsNew]
└─$ echo izgecr33nbqxmzk7nzxv64tfnvxxe43fpu======= | tr '[:lower:]' '[:upper:]' | base32 -d
FLAG{have_no_remorse}base32: invalid input
```

Close enough, the flag is `FLAG{have_no_remorse}`.