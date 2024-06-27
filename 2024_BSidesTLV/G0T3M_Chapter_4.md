# G0T3M - Chapter 4

 * Category: Osint
 * 50 Points
 * Solved by the JCTF Team

## Description

> Youv'e got them! Can you find their location and end this case? Don't forget the flag format!

## Solution

The previous challenge gave us the following instructions:

> Now, for the final flag, can you try and find us? we ROTATED so much until we got to the hospital.
> 
> The flag is the hospital's name with underscore(s) "_" instead of space(s). 

It also featured an image, which apart from a Mastodon profile also contained some coordinates:

```
81.52631374978306, -40.46423693910173 R??5
```

These coordinates point to the middle of nowhere (Greenland), but if we combine the clues we
got ("ROTATED" and "R??5") we can guess that the intention is to apply ROT5 to the coordinates
and transform them to `36.07186829423851, -95.91978148465628`. These coordinates point to the
*Saint Francis Hospital*.

The flag is `BSidesTLV2024{St_Francis_Hospital}`.