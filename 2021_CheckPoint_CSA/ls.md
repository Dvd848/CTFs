# ls
Category: Crypto, 300 Points

## Description

> Boss wanted to give people access to only see the directories list in a filesystem.
> 
> Now you can run anything on my OS... so long as it's `ls` (with parameters).
> 
> Send the commands in one or more files, then zip that file. I only accept zips, everything else is inefficient.
> 
> I'll verify that you aren't playing games, if I see a command that isn't `ls`, I'm not running it.
> 
> I don't like redundancy, so I'll save the hash of the zips that were already verified...using the best hash algorithm of them all: **MD5**!


## Solution

It's pretty obvious that we need to create an MD5 collision here: Two zip files, one containing a legal `ls` command and one containing a `cat` command to dump the flag. But, first thing first, let's check that we can communicate with the server:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls]
└─$ echo "ls -al" > ls.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls]
└─$ zip ls.zip ls.txt
  adding: ls.txt (stored 0%)

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls]
└─$ curl -X POST -H "Content-Type: application/zip" https://ls.csa-challenge.com/upload-zip --data-binary @ls.zip -s | python3 -m json.tool
{
    "statusCode": 200,
    "body": "[\"total 5\\ndrwxr-xr-x 2 root  root    57 Aug 19 06:30 .\\ndrwxr-xr-x 1 12569 users 4096 Aug 12 09:59 ..\\n-rwxr-xr-x 1 root  root    25 Aug  4 18:56 flag.txt\\n-rwxr-xr-x 1 root  root   504 Aug 19 06:30 lambda_function.py\\n\"]"
}
```

We can see `flag.txt` just waiting for us in the current directory, and all that we need to do is `cat` it. MD5 collisions have been achievable for a few years now, so the challenge here isn't to discover a way to perform the collision, but just to follow the super-laconic and unclear instructions of how to do so, as detailed in the bible of MD5/SHA1 hash collisions: [corkami](https://github.com/corkami/collisions#zip). 

Due to the structure of ZIP files, creating a zip file collision isn't trivial, but they offer a W/A: Finding a collision for a dual ZIP file, which is crafted to be able to host two different archive files. This dual ZIP file is provided to a tool which knows how to find hash collisions under some constraints: Given the original file, it will produce two files with the same MD5 and *almost* the same prefix. For example, if we provide the tool with `a.bin` as input, it will return `a1.bin` and `a2.bin` such that both files start with the contents of `a.bin` with just a few bits changed, and each file has some unpredictable suffix. The important thing is that the few changed bits are predictable, so one can craft a file where the changed bits are meaningful. In our case, the dual zip file is crafted so that one of the bits will control to which of the two archived files the zip applies for.

For the first stage, we need to take their provided [assembly source](https://github.com/corkami/collisions/blob/master/scripts/zip.asm), which describes the structure of a dual ZIP file, and edit it for our needs:

```assembly
; a nasm source to defines a dual ZIP for MD5 collisions

; build with `nasm -o zip.zip zip.asm`

; Ange Albertini 2018


BITS 32
%include "zip.inc"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; Replace File 1 and 2 values.

; reminder: incbin "<file>", <start>, <size>


; File 1

%macro file1.name 0
  db 'hello.txt'
%endmacro

%macro file1.content 0
%%start:
  db 'ls -al', 0ah
file1.compsize equ $ - %%start
%endmacro

file1.compression equ COMP_STORED
file1.decsize     equ file1.compsize
file1.CRC32       equ 0xafddb2c5

                                                          ; File 2

                                                          %macro file2.name 0
                                                              db 'bye.txt'
                                                          %endmacro

                                                          %macro file2.content 0
                                                          %%start:
                                                            db 'cat flag.txt', 0ah
                                                          file2.compsize equ $ - %%start
                                                          %endmacro

                                                          file2.compression equ COMP_STORED
                                                          file2.decsize     equ file2.compsize
                                                          file2.CRC32       equ 0xf5d90d17

                                                          ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                                                          file2:
                                                          istruc filerecord
                                                            at filerecord.frSignature,        db "PK", 3, 4
                                                            at filerecord.frVersion,          dw 0ah
                                                            at filerecord.frCompression,      dw file2.compression
                                                            at filerecord.frCrc,              dd file2.CRC32
                                                            at filerecord.frCompressedSize,   dd file2.compsize
                                                            at filerecord.frUncompressedSize, dd file2.decsize
                                                            at filerecord.frFileNameLength,   dw lfhname2.len
                                                            at filerecord.frExtraFieldLength, dw extra2.len
                                                          iend

                                                          lfhname2:
                                                            file2.name
                                                          lfhname2.len equ $ - lfhname2

                                                          extra2:
                                                            field2:
                                                              .id dw 0
                                                              .len dw extra2.len - 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

file1:
istruc filerecord
  at filerecord.frSignature,        db "PK", 3, 4
  at filerecord.frVersion,          dw 0ah
  at filerecord.frCompression,      dw file1.compression
  at filerecord.frCrc,              dd file1.CRC32
  at filerecord.frCompressedSize,   dd file1.compsize
  at filerecord.frUncompressedSize, dd file1.decsize
  at filerecord.frFileNameLength,   dw lfhname1.len
  at filerecord.frExtraFieldLength, dw extra1.len
iend

lfhname1:
  file1.name
lfhname1.len equ $ - lfhname1

extra1:
  field1:
    .id dw 0
    .len dw extra1.len - 4
                                                            extra2.len equ $ - extra2

                                                          data2:
                                                            file2.content
                                                          data2.len equ $ - data2

                                                          ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                                                          CD2:
                                                            istruc direntry
                                                              at direntry.deSignature,        db "PK", 1, 2
                                                              at direntry.deVersionToExtract, dw 0ah
                                                              at direntry.deCrc,              dd file2.CRC32
                                                              at direntry.deCompressedSize,   dd data2.len
                                                              at direntry.deUncompressedSize, dd data2.len
                                                              at direntry.deFileNameLength,   dw cdname2.len
                                                              at direntry.deFileCommentLength,dw cdcom2.len
                                                              at direntry.deHeaderOffset,     dd file2
                                                            iend

                                                            cdname2:
                                                              file2.name
                                                            .len equ $ - cdname2

                                                            cdcom2:
                                                              db 0 ; truncating
  extra1.len equ $ - extra1

data1:
  file1.content
data1.len equ $ - data1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

CD1:
  istruc direntry
    at direntry.deSignature,        db "PK", 1, 2
    at direntry.deVersionToExtract, dw 0ah
    at direntry.deCrc,              dd file1.CRC32
    at direntry.deCompressedSize,   dd data1.len
    at direntry.deUncompressedSize, dd data1.len
    at direntry.deFileNameLength,   dw cdname1.len
    at direntry.deFileCommentLength,dw cdcom1.len
    at direntry.deHeaderOffset,     dd file1
  iend
  cdname1:
    file1.name
  cdname1.len equ $ - cdname1

  cdcom1:
    db 0 ; truncating
    align 40h, db 0 ; to align EoCD1 for first collision

  cdcom1.len equ $ - cdcom1

CD1.len equ $ - CD1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

EoCD1:
istruc endlocator
  at endlocator.elSignature,          db "PK", 5, 0x06
  at endlocator.elEntriesOnDisk,      db 1
  at endlocator.elEntriesInDirectory, db 1
  at endlocator.elDirectorySize,      dd CD1.len
  at endlocator.elDirectoryOffset,    dd CD1
  at endlocator.elCommentLength,      dw EoCD1com.len
iend

EoCD1com:
  db 0 ; truncating

  align 4, db 'M' ; UniColl prefix

  align 40h, db 0 ; end of first collision block
  
  ; second collision block
  db 0
  align 40h, db 0
                                                             cdcom2.len equ $ - cdcom2

                                                           CD2.len equ $ - CD2

                                                           ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

                                                           EoCD2:
                                                           istruc endlocator
                                                             at endlocator.elSignature,          db "PK", 5, 0x86 ; 0x06/0x86 because of UniColl
                                                             at endlocator.elEntriesOnDisk,      db 1
                                                             at endlocator.elEntriesInDirectory, db 1
                                                             at endlocator.elDirectorySize,      dd CD2.len
                                                             at endlocator.elDirectoryOffset,    dd CD2
                                                             at endlocator.elCommentLength,      dw EoCD2com.len
                                                           iend

                                                           EoCD2com:
                                                             db 0 ; truncating
                                                             
                                                             align 4, db 'M' ; Unicoll prefix

                                                             align 40h, db 0 ; end of first collision block
                                                             
                                                             ; second collision block
                                                             db 0
                                                             align 40h, db 0

                                                           EoCD2com.len equ $ - EoCD2com
EoCD1com.len equ $ - EoCD1com

```

The only things we need to change here are the contents of the two files (first one changes to `ls -al`, second one changes to `cat flag.txt`) and the matching CRC32 values. We can then build the zip file using `nasm`:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ ls
zip.asm  zip.inc

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ nasm -o zip.zip zip.asm

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ xxd -g 1 zip.zip
00000000: 50 4b 03 04 0a 00 00 00 00 00 00 00 00 00 17 0d  PK..............
00000010: d9 f5 0d 00 00 00 0d 00 00 00 07 00 2f 00 62 79  ............/.by
00000020: 65 2e 74 78 74 00 00 2b 00 50 4b 03 04 0a 00 00  e.txt..+.PK.....
00000030: 00 00 00 00 00 00 00 c5 b2 dd af 07 00 00 00 07  ................
00000040: 00 00 00 09 00 47 00 68 65 6c 6c 6f 2e 74 78 74  .....G.hello.txt
00000050: 00 00 43 00 63 61 74 20 66 6c 61 67 2e 74 78 74  ..C.cat flag.txt
00000060: 0a 50 4b 01 02 00 00 0a 00 00 00 00 00 00 00 00  .PK.............
00000070: 00 17 0d d9 f5 0d 00 00 00 0d 00 00 00 07 00 00  ................
00000080: 00 ea 00 00 00 00 00 00 00 00 00 00 00 00 00 62  ...............b
00000090: 79 65 2e 74 78 74 00 6c 73 20 2d 61 6c 0a 50 4b  ye.txt.ls -al.PK
000000a0: 01 02 00 00 0a 00 00 00 00 00 00 00 00 00 c5 b2  ................
000000b0: dd af 07 00 00 00 07 00 00 00 09 00 00 00 2b 00  ..............+.
000000c0: 00 00 00 00 00 00 00 00 29 00 00 00 68 65 6c 6c  ........)...hell
000000d0: 6f 2e 74 78 74 00 00 00 00 00 00 00 00 00 00 00  o.txt...........
000000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000100: 50 4b 05 06 00 00 00 00 01 00 01 00 62 00 00 00  PK..........b...
00000110: 9e 00 00 00 ea 00 00 4d 00 00 00 00 00 00 00 00  .......M........
00000120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000180: 50 4b 05 86 00 00 00 00 01 00 01 00 1f 01 00 00  PK..............
00000190: 61 00 00 00 6a 00 00 4d 00 00 00 00 00 00 00 00  a...j..M........
000001a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000001b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000001c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000001d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000001e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000001f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ file zip.zip
zip.zip: Zip archive data, at least v1.0 to extract
```

The next thing we need to do is clone [HashClash](https://github.com/cr-marcstevens/hashclash) and edit the UniColl script `scripts/poc_no.sh`. We just have to change the `N=1` to `N=2` as instructed by the *corkami* tutorial:

> Another approach could be to just merge both archives, with their merged layers, and using UniColl - but with N=2, which introduces a difference on the 4th byte - to kill the magic signature of the `End of Central Directory`.

What this does is instruct `poc_no.sh` to run the second PoC out of the following three:

```sh
# Note that positive message bit differences are given with bit indices 1,...,32
# and with -1,...,-32 for a negative bit difference
case $N in

	1)
	diff="--diffm2 9"
	tmid=32
	;;

	2)
	diff="--diffm13 28 --diffm0 32 --diffm6 32"
	tmid=32
	;;

	3)
	diff="--diffm6 9 --diffm9 32 --diffm15 32"
	tmid=32
	tconnect=10
	datalower=500000
	dataupper=500000
	;;

esac
```

`N` essentially controls which bits get changed as part of the collision process. Since the `diff` syntax is a bit unclear, we'll explain: Each PoC finds a collision by changing some bits in the provided prefix.

```sh
case $N in

	1)
	diff="--diffm2 9" # <-- Bit #8 in 2nd DWORD
	tmid=32
	;;

	2)
	diff="--diffm13 28 --diffm0 32 --diffm6 32" # <-- Bit #27 in 13th DWORD, bit #31 in 0th DWORD, bit #31 in 6th DWORD
	tmid=32
	;;

	3)
	diff="--diffm6 9 --diffm9 32 --diffm15 32" # <-- Bit #8 in 6th DWORD, bit #31 in 9th DWORD, bit #31 in 15th DWORD
	tmid=32
	tconnect=10
	datalower=500000
	dataupper=500000
	;;
```

As we can see, `N=2` changes the forth byte (bit #31 in 0th DWORD). The dual ZIP file was crafted so that if that bit is `0`, the zip will point to one file, and if it's `1`, the zip will point to the other file. 

Now, we need to edit `zip.zip`:

> This means one could collide two arbitrary ZIP with a single UniColl and 24 bytes of set prefix.

We need to change the byte at offset `0x183` from `86` to `06` and delete everything from `0x198` onwards, to get:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ xxd -g 1 prefix.bin
00000000: 50 4b 03 04 0a 00 00 00 00 00 00 00 00 00 17 0d  PK..............
00000010: d9 f5 0d 00 00 00 0d 00 00 00 07 00 2f 00 62 79  ............/.by
00000020: 65 2e 74 78 74 00 00 2b 00 50 4b 03 04 0a 00 00  e.txt..+.PK.....
00000030: 00 00 00 00 00 00 00 c5 b2 dd af 07 00 00 00 07  ................
00000040: 00 00 00 09 00 47 00 68 65 6c 6c 6f 2e 74 78 74  .....G.hello.txt
00000050: 00 00 43 00 63 61 74 20 66 6c 61 67 2e 74 78 74  ..C.cat flag.txt
00000060: 0a 50 4b 01 02 00 00 0a 00 00 00 00 00 00 00 00  .PK.............
00000070: 00 17 0d d9 f5 0d 00 00 00 0d 00 00 00 07 00 00  ................
00000080: 00 ea 00 00 00 00 00 00 00 00 00 00 00 00 00 62  ...............b
00000090: 79 65 2e 74 78 74 00 6c 73 20 2d 61 6c 0a 50 4b  ye.txt.ls -al.PK
000000a0: 01 02 00 00 0a 00 00 00 00 00 00 00 00 00 c5 b2  ................
000000b0: dd af 07 00 00 00 07 00 00 00 09 00 00 00 2b 00  ..............+.
000000c0: 00 00 00 00 00 00 00 00 29 00 00 00 68 65 6c 6c  ........)...hell
000000d0: 6f 2e 74 78 74 00 00 00 00 00 00 00 00 00 00 00  o.txt...........
000000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000100: 50 4b 05 06 00 00 00 00 01 00 01 00 62 00 00 00  PK..........b...
00000110: 9e 00 00 00 ea 00 00 4d 00 00 00 00 00 00 00 00  .......M........
00000120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000180: 50 4b 05 06 00 00 00 00 01 00 01 00 1f 01 00 00  PK..............
00000190: 61 00 00 00 6a 00 00 4d                          a...j..M
```

This is what we feed into `unicoll`:

```console
┌──(user@kali)-[~/utils/crypto/hashclash]
└─$ mkdir workspace_zip_clash

┌──(user@kali)-[~/utils/crypto/hashclash]
└─$ cd workspace_zip_clash

┌──(user@kali)-[~/utils/crypto/hashclash/workspace_zip_clash]
└─$ cp /media/sf_CTFs/checkpoint/ls/workspace/prefix.bin .

┌──(user@kali)-[~/utils/crypto/hashclash/workspace_zip_clash]
└─$ ../scripts/poc_no.sh prefix.bin

# ...

Block 1: ./data/coll1_3795903542
e2 25 3a 3c 1a 29 47 08 5f c2 c7 48 7c 46 5e 1c
e3 f1 b0 27 2c e5 22 bc 5a 91 1e 7f 89 f5 e0 e8
81 4c 50 b0 3d ab 4e ab 64 7b a6 22 f3 4f 23 bc
53 4a 3c 7d c0 29 7a a3 9b 3b 53 e4 03 b8 3b f1
Block 2: ./data/coll2_3795903542
e2 25 3a bc 1a 29 47 08 5f c2 c7 48 7c 46 5e 1c
e3 f1 b0 27 2c e5 22 bc 5a 91 1e ff 89 f5 e0 e8
81 4c 50 b0 3d ab 4e ab 64 7b a6 22 f3 4f 23 bc
53 4a 3c 7d c0 29 7a ab 9b 3b 53 e4 03 b8 3b f1
Found collision!
61141afe3fee5057f5dc8a3d678060d0  collision1.bin
61141afe3fee5057f5dc8a3d678060d0  collision2.bin
9b56f26715754e51fc7968dcedf1c09b8445c174  collision1.bin
436971b1051e988f1807a789ab1992ad57204506  collision2.bin
4 -rw-r--r-- 1 user user 512 Sep 11 20:53 collision1.bin
4 -rw-r--r-- 1 user user 512 Sep 11 20:53 collision2.bin
```

Let's inspect the files we got:

```console
┌──(user@kali)-[~/utils/crypto/hashclash/workspace_zip_clash]
└─$ cp collision*.bin /media/sf_CTFs/checkpoint/ls/workspace

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ md5sum collision*.bin
61141afe3fee5057f5dc8a3d678060d0  collision1.bin
61141afe3fee5057f5dc8a3d678060d0  collision2.bin

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ file collision*.bin
collision1.bin: Zip archive data, at least v1.0 to extract
collision2.bin: Zip archive data, at least v1.0 to extract

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ unzip collision1.bin
Archive:  collision1.bin
 extracting: bye.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ cat bye.txt
cat flag.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ mv collision1.bin zip_cat_flag.zip

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ unzip collision2.bin
Archive:  collision2.bin
 extracting: hello.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ cat hello.txt
ls -al

┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ mv collision2.bin zip_ls.zip
```

The magic happens at offset `0x183`:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ diff -y <(xxd -g 1 zip_ls.zip) <(xxd -g 1 zip_cat_flag.zip)
00000000: 50 4b 03 04 0a 00 00 00 00 00 00 00 00 00 17 0d  PK   00000000: 50 4b 03 04 0a 00 00 00 00 00 00 00 00 00 17 0d  PK
00000010: d9 f5 0d 00 00 00 0d 00 00 00 07 00 2f 00 62 79  ..   00000010: d9 f5 0d 00 00 00 0d 00 00 00 07 00 2f 00 62 79  ..
00000020: 65 2e 74 78 74 00 00 2b 00 50 4b 03 04 0a 00 00  e.   00000020: 65 2e 74 78 74 00 00 2b 00 50 4b 03 04 0a 00 00  e.
000000b0: dd af 07 00 00 00 07 00 00 00 09 00 00 00 2b 00  ..   000000b0: dd af 07 00 00 00 07 00 00 00 09 00 00 00 2b 00  ..
000000c0: 00 00 00 00 00 00 00 00 29 00 00 00 68 65 6c 6c  ..   000000c0: 00 00 00 00 00 00 00 00 29 00 00 00 68 65 6c 6c  ..
000000d0: 6f 2e 74 78 74 00 00 00 00 00 00 00 00 00 00 00  o.   000000d0: 6f 2e 74 78 74 00 00 00 00 00 00 00 00 00 00 00  o.
000000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   000000e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
000000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   000000f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000100: 50 4b 05 06 00 00 00 00 01 00 01 00 62 00 00 00  PK   00000100: 50 4b 05 06 00 00 00 00 01 00 01 00 62 00 00 00  PK
00000110: 9e 00 00 00 ea 00 00 4d 00 00 00 00 00 00 00 00  ..   00000110: 9e 00 00 00 ea 00 00 4d 00 00 00 00 00 00 00 00  ..
00000120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000120: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000130: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000140: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000150: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000160: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..   00000170: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ..
00000180: 50 4b 05 86 00 00 00 00 01 00 01 00 1f 01 00 00  PK | 00000180: 50 4b 05 06 00 00 00 00 01 00 01 00 1f 01 00 00  PK
00000190: 61 00 00 00 6a 00 00 4d 8c a2 7a c7 fc 21 4c 26  a. | 00000190: 61 00 00 00 6a 00 00 4d 8c a2 7a 47 fc 21 4c 26  a.
000001a0: 25 10 d6 ce a3 d3 e7 fb a5 88 67 35 41 b9 4f 2c  %.   000001a0: 25 10 d6 ce a3 d3 e7 fb a5 88 67 35 41 b9 4f 2c  %.
000001b0: 83 7a c6 9a ba d7 75 75 c4 a6 87 21 2b 5c a2 c2  .z | 000001b0: 83 7a c6 9a ba d7 75 6d c4 a6 87 21 2b 5c a2 c2  .z
000001c0: e2 25 3a 3c 1a 29 47 08 5f c2 c7 48 7c 46 5e 1c  .% | 000001c0: e2 25 3a bc 1a 29 47 08 5f c2 c7 48 7c 46 5e 1c  .%
000001d0: e3 f1 b0 27 2c e5 22 bc 5a 91 1e 7f 89 f5 e0 e8  .. | 000001d0: e3 f1 b0 27 2c e5 22 bc 5a 91 1e ff 89 f5 e0 e8  ..
000001e0: 81 4c 50 b0 3d ab 4e ab 64 7b a6 22 f3 4f 23 bc  .L   000001e0: 81 4c 50 b0 3d ab 4e ab 64 7b a6 22 f3 4f 23 bc  .L
000001f0: 53 4a 3c 7d c0 29 7a a3 9b 3b 53 e4 03 b8 3b f1  SJ | 000001f0: 53 4a 3c 7d c0 29 7a ab 9b 3b 53 e4 03 b8 3b f1  SJ
```

The right-hand side has a valid `End of Central Directory` signature starting at `0x180` (`.P .K 05 06`), so when the zip parser scans the file (from bottom up) it meets the valid signature and uses it to jump to the file it points to: `bye.txt`. However, on the left-hand side the offset at `0x180` contains an invalid signature (`.P .K 05 86`). So, the parser skips the entry that points to `bye.txt` and instead arrives to the valid entry at `0x100` - the one that points to `hello.txt`. 

All that's left is to send the files:

```python
import requests
import json

for file in ["zip_ls.zip", "zip_cat_flag.zip"]:
    print(f"Sending {file}\n")
    with open(file,'rb') as payload:
        headers = {'content-type': 'application/zip'}
        r = requests.post('https://ls.csa-challenge.com/upload-zip',
                          data=payload, headers=headers)
        j = r.json()
        print(json.loads(j["body"])[0])
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/ls/workspace]
└─$ python3 post.py
Sending zip_ls.zip

total 5
drwxr-xr-x 2 root  root    57 Aug 19 06:30 .
drwxr-xr-x 1 12569 users 4096 Aug 12 09:59 ..
-rwxr-xr-x 1 root  root    25 Aug  4 18:56 flag.txt
-rwxr-xr-x 1 root  root   504 Aug 19 06:30 lambda_function.py

Sending zip_cat_flag.zip

CSA{K1LL_MD5_A1r34dy_p1z}
```