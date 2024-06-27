# G0T3M - Chapter 1

 * Category: Osint
 * 50 Points
 * Solved by the JCTF Team

## Description

> You are a team of threat analysts for a large streaming conglomerate called "Shows and Such". The conglomerate just spent 500 million USD to produce a new show called "Chefs In Space! Flavours beyond the cosmos!". But one night before the show's premiere, a malicious hacker (or hackers) corrupted all of the show's footage, leaving behind only one video as a token of their horrid act. Millions of heartbroken fans waited to see the world-renowned chef Cordon Brandy make a brioche beyond the stratosphere.
> 
> Now, it's up to you, the analysts, to find the culprits!
> 
> The CTF begins as you receive a corrupt video file from your team leader, the video the hackers left behind. The investigation will take you through data inspection, social media profiles, and uncovering the real culprit, with a bonus flag waiting for the sharpest minds.
> 
> Are you ready to take on the challenge and restore the faith of millions of fans? The hunt begins now.

An MP4 file was attached.

## Solution

Watching the attached video file and searching for clues, we observe the following caption:

> L0LSQU@D: Shadows of the Web

We can search the web for `L0LSQU@D` and locate [this](https://www.reddit.com/user/L3OL33TS/)
Reddit profile, belonging to a user called `L3OL33TS`. The profile contains the following status:

>  L3OLEETS | L0LSQU@D HAXX0R | STATUS: ACTIVE. 

It doesn't point to any posts or comments though.

Let's go back to the video and inspect the EXIF info. In the comment, we find:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/G0T3M_-_Chapter_1]
└─$ exiftool h4ack3d_modified_.mp4 | grep Comment
Comment                         : J3 J1YY A3I3E O3 Y34X3Q! A0G 3I3A 0A C@$73O1A!
```

This looks like some kind of simple substitution cipher. We try a few and get lucky with ROT13:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/G0T3M_-_Chapter_1]
└─$ node
Welcome to Node.js v18.10.0.
Type ".help" for more information.
> const chef = require("/home/user/utils/CyberChef/");
undefined
> chef.ROT13("J3 J1YY A3I3E O3 Y34X3Q! A0G 3I3A 0A C@$73O1A!");
W3 W1LL N3V3R B3 L34K3D! N0T 3V3N 0N P@$73B1N!
```

So we search [PasteBin](https://pastebin.com) for `L0LSQU@D` and find [this paste](https://pastebin.com/1pvX42VH):

```
------------THIS LEAK IS PRESENTED BY THE:------------
 
______  _____ ______  _      _____  _____ ______  _____  _____ ______  ______
| ___ \|  _  ||  ___|| |    /  __ \|  _  || ___ \|_   _||____ || ___ \|___  /
| |_/ /| |/' || |_   | |    | /  \/| |/' || |_/ /  | |      / /| |_/ /   / / 
|    / |  /| ||  _|  | |    | |    |  /| ||  __/   | |      \ \|    /   / /  
| |\ \ \ |_/ /| |    | |____| \__/\\ |_/ /| |      | |  .___/ /| |\ \ ./ /___
\_| \_| \___/ \_|    \_____/ \____/ \___/ \_|      \_/  \____/ \_| \_|\_____/
 
 
 ROFL:ROFL:ROFL:ROFL
         _^___
 L    __/   [] \
LOL===__        \
 L      \________]
         I   I
        --------/
 
 
_______________________L0LSQU@D G3TS PWN3D_______________________
 
USERNAME/PLATFORM/IRLNAME
 
Parr0t3Xp3Rt/IG/Rasjesh_Patel
 
V1K1nGHaXeRMAN/TWTTR/EDGAR_ALLEN_PETTERSEN
 
CHARL1EL0V3SDEB/MASTODON/CHARLES_MCCORMICK
 
LEOL33TS/REDDIT/LEONARD_HOLT
 
DANPWNS1/IG/DAN_WINCHESTER
 
SUP3RTR0YB0Y/TWTTR/TROY_CARTMAN
 
HERE IS FLAG NUMBER #1: 
BSidesTLV2024{7H3$QU@DG37$PWN3D!}
```

The flag is `BSidesTLV2024{7H3$QU@DG37$PWN3D!}`.