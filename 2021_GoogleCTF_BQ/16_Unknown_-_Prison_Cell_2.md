# Unknown - Prison Cell 2
Category: Misc.

## Description

> Well, okay, you’re back in the cell again, and they changed the lock to something quite heavier. This one cannot be picked with a paperclip… So, is this where the mission ends? PLING, another message from the boss. Another GIF… No wait, not only a GIF, also text: "Hi AGENT, I was just contacting you to say that we’re running out of time, if you fail to reach the office and pull the self destruction lever in under 30 minutes, they will already have executed their evil plan. I’m counting on you!." Well, that wasn’t too helpful... What to do, what to do?
> 
> Challenge: Hash-meee (misc)
> 
> I heard BotBot, the resident Discord bot, is experimenting with hashing. He specifically wants to see 2 different strings, both starting with `gctf`, that have the same md5 hash. He will reward this with a flag. You can access our Discord with the following invite link: https://discord.gg/FbrXTjvv To solve this challenge DM BotBot on discord using the command `!hashme` followed by the two strings, encoded in hex. E.g. if your strings are "gctfhello" and "gctfhola" you would send `!hashme 6763746668656c6c6f 67637466686f6c61`

## Solution

We need to find an MD5 hash collision, and [this repo](https://github.com/corkami/collisions) contains everything you need to know about hash collisions. It recommends a tool called `fastcoll` for a fast identical prefix with almost no constraints. Here's how to build it these days:

```console
┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ wget https://downloads.sourceforge.net/project/boost/boost/1.54.0/boost_1_54_0.tar.gz

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ tar -xvf boost_1_54_0.tar.gz

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ cd boost_1_54_0

┌──(user@kali)-[~/utils/crypto/fastcoll/boost_1_54_0]
└─$ ./bootstrap.sh --with-libraries=filesystem,program_options,system
Building Boost.Build engine with toolset gcc... tools/build/v2/engine/bin.linuxx86_64/b2
Unicode/ICU support for Boost.Regex?... /usr
Generating Boost.Build configuration in project-config.jam...

Bootstrapping is done. To build, run:

    ./b2

To adjust configuration, edit 'project-config.jam'.
Further information:

   - Command line help:
     ./b2 --help

   - Getting started guide:
     http://www.boost.org/more/getting_started/unix-variants.html

   - Boost.Build documentation:
     http://www.boost.org/boost-build2/doc/html/index.html
     
┌──(user@kali)-[~/utils/crypto/fastcoll/boost_1_54_0]
└─$ ./b2 --with-filesystem --with-program_options --with-system

# ...

The Boost C++ Libraries were successfully built!

The following directory should be added to compiler include paths:

    /home/user/utils/crypto/fastcoll/boost_1_54_0

The following directory should be added to linker library paths:

    /home/user/utils/crypto/fastcoll/boost_1_54_0/stage/lib
    
┌──(user@kali)-[~/utils/crypto/fastcoll/boost_1_54_0]
└─$ cd ..

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ wget https://www.win.tue.nl/hashclash/fastcoll_v1.0.0.5-1_source.zip

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ unzip fastcoll_v1.0.0.5-1_source.zip
Archive:  fastcoll_v1.0.0.5-1_source.zip
  inflating: block0.cpp
  inflating: block1.cpp
  inflating: block1stevens00.cpp
  inflating: block1stevens01.cpp
  inflating: block1stevens10.cpp
  inflating: block1stevens11.cpp
  inflating: block1wang.cpp
  inflating: main.cpp
  inflating: main.hpp
  inflating: md5.cpp

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ g++ -O3 *.cpp -lboost_filesystem -lboost_program_options -lboost_system -o fastcoll -static -I/home/user/utils/crypto/fastcoll/boost_1_54_0 -L/home/user/utils/crypto/fastcoll/boost_1_54_0/stage/lib

┌──(user@kali)-[~/utils/crypto/fastcoll]
└─$ ./fastcoll
MD5 collision generator v1.5
by Marc Stevens (http://www.win.tue.nl/hashclash/)

Allowed options:
  -h [ --help ]           Show options.
  -q [ --quiet ]          Be less verbose.
  -i [ --ihv ] arg        Use specified initial value. Default is MD5 initial
                          value.
  -p [ --prefixfile ] arg Calculate initial value using given prefixfile. Also
                          copies data to output files.
  -o [ --out ] arg        Set output filenames. This must be the last option
                          and exactly 2 filenames must be specified.
                          Default: -o msg1.bin msg2.bin
```

We feed it the common prefix we want, and it generates a collision almost immediately:

```console
┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ echo -n "gctf" > prefix

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ ~/utils/crypto/fastcoll/fastcoll -p prefix
MD5 collision generator v1.5
by Marc Stevens (http://www.win.tue.nl/hashclash/)

Using output filenames: 'msg1.bin' and 'msg2.bin'
Using prefixfile: 'prefix'
Using initial value: 0b32d30483f183101ccaf35ac1595746

Generating first block: .....
Generating second block: S10...........................................................
Running time: 1.21746 s

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ md5sum msg1.bin
9a1db794ee600dc52ea6cde799151b33  msg1.bin

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ md5sum msg2.bin
9a1db794ee600dc52ea6cde799151b33  msg2.bin

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ xxd -g 1 msg1.bin
00000000: 67 63 74 66 00 00 00 00 00 00 00 00 00 00 00 00  gctf............
00000010: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000040: bb 67 31 a0 a4 4e 3a ff b0 a3 1f f9 89 9f 15 bb  .g1..N:.........
00000050: 2f b6 4b d7 28 ba 2b b9 39 ba 76 f5 f9 19 8e ed  /.K.(.+.9.v.....
00000060: 3a b3 0a ba 6e 4e 48 ae eb ed e5 41 e3 d3 ba 82  :...nNH....A....
00000070: a8 49 c3 06 23 15 55 03 1d 27 57 6f 49 b4 d5 2f  .I..#.U..'WoI../
00000080: cd 1b 6f 11 99 ec b5 93 b1 e3 98 2b a8 bc 9d d3  ..o........+....
00000090: 97 b5 7a 47 70 db 02 29 2b b3 10 60 06 60 68 b8  ..zGp..)+..`.`h.
000000a0: d3 3b f2 53 af 48 92 34 b5 46 82 9b b3 a3 1b 14  .;.S.H.4.F......
000000b0: 00 db f4 44 28 74 b6 1a 15 33 d7 e4 ab 05 88 7c  ...D(t...3.....|

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ xxd -g 2 msg1.bin
00000000: 6763 7466 0000 0000 0000 0000 0000 0000  gctf............
00000010: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000020: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000030: 0000 0000 0000 0000 0000 0000 0000 0000  ................
00000040: bb67 31a0 a44e 3aff b0a3 1ff9 899f 15bb  .g1..N:.........
00000050: 2fb6 4bd7 28ba 2bb9 39ba 76f5 f919 8eed  /.K.(.+.9.v.....
00000060: 3ab3 0aba 6e4e 48ae ebed e541 e3d3 ba82  :...nNH....A....
00000070: a849 c306 2315 5503 1d27 576f 49b4 d52f  .I..#.U..'WoI../
00000080: cd1b 6f11 99ec b593 b1e3 982b a8bc 9dd3  ..o........+....
00000090: 97b5 7a47 70db 0229 2bb3 1060 0660 68b8  ..zGp..)+..`.`h.
000000a0: d33b f253 af48 9234 b546 829b b3a3 1b14  .;.S.H.4.F......
000000b0: 00db f444 2874 b61a 1533 d7e4 ab05 887c  ...D(t...3.....|

```

All that's left is to format the output and send it to the bot:

```console
┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ xxd -p msg1.bin | tr -d '\n'
67637466000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000bb6731a0a44e3affb0a31ff9899f15bb2fb64bd728ba2bb939ba76f5f9198eed3ab30aba6e4e48aeebede541e3d3ba82a849c306231555031d27576f49b4d52fcd1b6f1199ecb593b1e3982ba8bc9dd397b57a4770db02292bb31060066068b8d33bf253af489234b546829bb3a31b1400dbf4442874b61a1533d7e4ab05887c

┌──(user@kali)-[/media/sf_CTFs/google/16_Unknown_-_Prison_Cell_2]
└─$ xxd -p msg2.bin | tr -d '\n'
67637466000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000bb6731a0a44e3affb0a31ff9899f15bb2fb64b5728ba2bb939ba76f5f9198eed3ab30aba6e4e48aeebede541e353bb82a849c306231555031d2757ef49b4d52fcd1b6f1199ecb593b1e3982ba8bc9dd397b57ac770db02292bb31060066068b8d33bf253af489234b546829bb3231b1400dbf4442874b61a1533d764ab05887c
```

And we get the flag:

```
BotBot
 — 
Today at 10:04 PM
Good job! Here is a flag for you! CTF{h4sh_m3_tw1c3_1245fd3}
```