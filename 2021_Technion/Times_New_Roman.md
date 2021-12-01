# Times New Roman
Category: Cryptography, 100 points

## Description

> I found this code in one of the learning center in the Technion, can you figure out what it is?
> 
> Note: the flag is all lowercase

```
👍💧❄☜👍☟☠✋⚐☠❀❄☟☜♉☝✌💧❄☜☼♉👍☜☠❄☜☼♉✋💧♉❄☟☜♉👌☜💧❄♉☝✌💧❄☜☼❝
```

## Solution

We know that the flag format is `cstechnion{}`. The 👍 character in the ciphertext seems to match the `c` of the flag format. Using the flag format, we can decipher the following:

```
cstechnion{the???ste??cente??is?the??est???ste?}
```

Using some common sense, we can guess a few more characters:

```
cstechnion{the_??ster_center_is_the_best_??ster}
```

Technion students probably will just know this, for outsiders it's harder but eventually we can find the "[Gaster Center](https://culture.web.technion.ac.il/rifkin/)" in Google (or just brute force the flag using some dictionary). 

The flag: `cstechnion{the_gaster_center_is_the_best_gaster}`.

### Intended Solution

After the CTF, I got a tip from [@dezutterjoachim](https://twitter.com/dezutterjoachim) that the font being used in the challenge is actually known as "W.D. Gaster's Alphabet" (more about that [here](https://wingdingstranslator.com/undertale/)). Using an online translator such as [this one](https://lingojam.com/W-D-Gaster) we can easily decrypt the flag.