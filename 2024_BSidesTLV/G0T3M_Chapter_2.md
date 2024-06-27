# G0T3M - Chapter 2

 * Category: Osint
 * 50 Points
 * Solved by the JCTF Team

## Description

> Keep looking for the truth ...

## Solution

Based on the previous challenge, we received a few social network credentials. One of them
is `DANPWNS1/IG/DAN_WINCHESTER`. So, we visit [Dan's Instagram Profile](https://www.instagram.com/DANPWNS1/)
and find the following bio:

```
https://pst.innomi.net/paste/oos92a9tx787qymt56n27h44
my bl06
```

We visit the link and get the following:

```


This  is it.
in 3 weeks, I'm going to hack those dumb asses at "Shows And Such Inc." I'll make them pay for these prices, and for what they did to Troy.
We'll demolish their servers, and take our revenge!

UPDATE-
2 Weeks to go, I'm getting ready, This is exciting.

UPDATE 2-
3 More days, we WILL PREVAIL!

UPDATE 3-
2 More days

Shit, shit, shit, shit, shit, THE COPS ARE HERE CRAP THEY'RE AT MY DOOR, SHI-




THIS BLOG IS NOW SEIZED BY THE FBI

YOU FOUND FLAG NUMBER 2
BSidesTLV2024{D@NPWNN&N0L0N63R}
```

The flag is `BSidesTLV2024{D@NPWNN&N0L0N63R}`.