# We have a liftoff! (1/5)
Category: Innovation

## Description

> Welcome cadets!
> 
> This is the mission's captain speaking, We are thrilled that you have joined us on this important mission to find extraterrestrial life.
> 
> Sit back and enjoy the ride - your spaceship's kitchen is equipped with cookies and coffee. Just remember, don't spill anything on the equipment, we don't want some undesirable incidents in space.
> 
> It seems that one of our satellites that is orbiting the moon has caught up with a signal...
> 
> ```
> https://v0tw7ojr02.execute-api.us-west-2.amazonaws.com/fireitup
> id: 0c786488-544c-4d6f-b04a-e093165989da
> POST
> ```

## Solution


As guided, let's try to perform a POST request to the given URL with the given parameter:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ curl --location --request POST 'https://v0tw7ojr02.execute-api.us-west-2.amazonaws.com/fireitup' \
--header 'Content-Type: application/json' \
--data-raw '{"id": "0c786488-544c-4d6f-b04a-e093165989da"}' -s > response

┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ file response
response: ASCII text, with very long lines, with no line terminators
```

We got a text file, let's peek into it by printing the first 100 characters:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ head -c 100 response
iVBORw0KGwoAAAANSUhEUgAADXAAAAWgCAYAAABNcorEAAAEGWlDQ1BrQ0dDb2xvclNwYWNlR2VuZXJpY1JHQgAAOI2NVV1oHFUU
```

Looks like base64. Let's decode it:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ base64 -d response > response.bin

┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ file response.bin
response.bin: data
```

So `file` doesn't recognize it, let's take a look inside:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ cat response.bin | xxd -g 1 | head
00000000: 89 50 4e 47 0d 0a 1b 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 0d 70 00 00 05 a0 08 06 00 00 00 4d 72 8a  ...p.........Mr.
00000020: c4 00 00 04 19 69 43 43 50 6b 43 47 43 6f 6c 6f  .....iCCPkCGColo
00000030: 72 53 70 61 63 65 47 65 6e 65 72 69 63 52 47 42  rSpaceGenericRGB
00000040: 00 00 38 8d 8d 55 5d 68 1c 55 14 3e bb 73 67 23  ..8..U]h.U.>.sg#
00000050: 24 ce 53 6c 34 85 74 a8 3f 0d 25 0d 93 56 34 a1  $.Sl4.t.?.%..V4.
00000060: b4 ba 7f dd dd 36 6e 96 49 36 da 22 e8 64 f6 ee  .....6n.I6.".d..
00000070: ce 98 c9 ce 38 33 bb fd a1 4f 45 50 7c 31 ea 9b  ....83...OEP|1..
00000080: 14 c4 bf b7 80 20 28 f5 0f db 3e b4 2f 95 0a 25  ..... (...>./..%
00000090: da d4 20 28 3e b4 f8 83 50 e8 8b a6 eb 99 3b 33  .. (>...P.....;3
```

This really looks like a `PNG` file, yet `file` won't recognize it. `file` is a signature-based tool, so something must be wrong with the signature. 
`pngcheck` can give us a clue as to the problem:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ pngcheck -v -f response.bin
File: response.bin (3346832 bytes)
  File is CORRUPTED.  It seems to have suffered EOL conversion.
ERRORS DETECTED in response.bin
```

According to the [PNG Specifications](https://en.wikipedia.org/wiki/Portable_Network_Graphics), a `PNG` file should always start with:


| Values (hex) | Purpose                                                                                                                                                                         |
| ------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `89`         | Has the high bit set to detect transmission systems that do not support 8-bit data and to reduce the chance that a text file is mistakenly interpreted as a PNG, or vice versa. |
| `50 4E 47`   | In ASCII, the letters PNG, allowing a person to identify the format easily if it is viewed in a text editor.                                                                    |
| `0D 0A`      | A DOS-style line ending (CRLF) to detect DOS-Unix line ending conversion of the data.                                                                                           |
| `1A`         | A byte that stops display of the file under DOS when the command type has been used—the end-of-file character.                                                                  |
| `0A`         | A Unix-style line ending (LF) to detect Unix-DOS line ending conversion.                                                                                                        |

We see that the PNG format has several safeguards to detect line-ending conversions. In our case, the incorrect byte seems to be at offset `0x6`, containing `0x1b` instead of `0x1a`. Let's fix that:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ cp response.bin fixed.png

┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ printf '\x1a' | dd conv=notrunc of=fixed.png bs=1 seek=6
1+0 records in
1+0 records out
1 byte copied, 0.0047586 s, 0.2 kB/s

┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ file fixed.png
fixed.png: PNG image data, 3440 x 1440, 8-bit/color RGBA, non-interlaced
```

We got a valid PNG file. It contains the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/We_have_a_liftoff]
└─$ tesseract fixed.png - --dpi 209
V3ryCl0seT0H0m3
```
