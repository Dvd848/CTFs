# Finding Ducky - Part 3
Category: Steganography, 100 points

## Description

> A message from ducky! and he seems to be in trouble. Can you figure out what he meant with the new logo?

A JPEG file was attached.

## Solution

The [previous](Finding_Ducky_-_Part_2.md) challenge said:

> As you might have figured out by now, someone is following me, I might even be kidnapped by now.
> I don't know by who or why yet, but I think it has something to do with my research.
> I can't give you any information because I think they're watching my traffic,
> But I can tell you this, I hid something in the new logo with the passphrase duck_in_the_code.
> They're not smart enough to figure it out, but I know you are.
> Hope to see you soon.

Given that we got a passphrase ("duck_in_the_code"), the first thing we should try is `steghide`:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_3]
└─$ steghide extract -sf new_logo.jpeg -p duck_in_the_code
wrote extracted data to "cipher.txt".
```

The contents is:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Finding_Ducky_-_Part_3]
└─$ cat cipher.txt
Look at my twitter, the flag is cstechnion{the_new_logo_is_great_right}
84 366 446 496 402 66 111 340 280 243 131 157 429 263 181 273 143 407 470 356 125 176 333 467 343 161 188 115 306 179 419 466 253 156 278 316 72 317 485 348 320
```