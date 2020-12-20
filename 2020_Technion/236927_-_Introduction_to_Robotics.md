# 236927 - Introduction to Robotics
Category: Web

## Description
> I created this awesome site for Introduction to Robotics and hidden a flag in it, it's hidden so well even google won't find it!

## Solution

The challenge name and description are clear references to `robots.txt`.

```console
root@kali:/media/sf_CTFs/technion/236927_-_Introduction_to_Robotics# curl http://ctf.cs.technion.ac.il:4008/robots.txt
Disallow: /my_very_secret_flag
```

Let's visit that page:
```console
root@kali:/media/sf_CTFs/technion/236927_-_Introduction_to_Robotics# curl http://ctf.cs.technion.ac.il:4008/my_very_secret_flag -s | grep cstechnion
cstechnion{th35e_4ReN7_tH3_r08ots_y0ur3_100kin6_fOr}
```

