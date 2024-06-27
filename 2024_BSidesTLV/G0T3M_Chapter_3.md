# G0T3M - Chapter 3

 * Category: Osint
 * 50 Points
 * Solved by the JCTF Team

## Description

> Keep on going ...

## Solution

Based on the first challenge, we received a few social network credentials. One of them
is `SUP3RTR0YB0Y/TWTTR/TROY_CARTMAN`. So, we visit [Troy's Twitter Profile](https://twitter.com/SUP3RTR0YB0Y)
and find a single post with an image of a couple holding hands in front of a 
[Mastodon profile](https://mastodon.social/@J3nnyL0v3sTr0y).

We visit it and get [this link](https://pst.innomi.net/paste/vrq3j86kqwddnxcy82cv3pdm) from the profile status.

It contains:

```



Jenny's Super Awesome BLOG!!!!!!!!!!!!!!!
---------------------------------


HI YALL!!!!!!!!!!!!!!!
SO my baby troy (who's a master hacker XDDDDDDD) is telling me about this secret stuff they are up tooooooo
omfgggg his friends are so coooll with their black hoodies and cool laptops and green text ughhhh k wish i could hack.

UPDATE
Sooooo yalllllllllllllllllllllllll in a week apparently troy and his group are going to take down this streaming org that troy HATES, apperantly they screwed him over
back when he was working with them, and now my baby is getting revengeeeeeeeee
UGH HE IS SO COOOOOOOOOL

UPDATE 2!
OMFGGGG 4 DAYS IM EXCITED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! My baby and his boys ARE READY to take em down, although i will miss charles, but leo is a pain IN THE ASSSS OMFGGGGGGG

update 3
shit.
troy is at the hospital, Dan and him got into a fight and he got a brain hemmorage, or something, idk
i've been crying nonstop for a day, and to think in 3 days he wanted to attack these fuckers at the streaming org.
shit.

update 4
They got dan.

update 5
Found Troy's old laptop.
idk what to do.. he's still in a coma, i'm so confused.
Leo helped me learn a couple of things, he calls me a "skiddie", but tbh i dont care.
Me and Leo will carry out the attack.
for troy.

update 6
We did it.
L0LSQU@D 4 LYFE.
\\\\\\\\\\\\\\\\\\\\\\
FLAG NUMBER 3:

BSidesTLV2024{BR0K3NH3@R7}

\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
Now, for the final flag, can you try and find us? we ROTATED so much until we got to the hospital.

The flag is the hospital's name with underscore(s) "_" instead of space(s). 
\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

RIP Troy

```

The flag is `BSidesTLV2024{BR0K3NH3@R7}`.