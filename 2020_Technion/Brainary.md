# Brainary
Category: Cryptography

## Description
> It stares at the endless abyss of darkness. The abyss reflects. Knowing it can never see light again, it seeks the truth.
> 
> The universe reveals its mysteries through endless streams of numericals.
> 
> Burned with the passion to learn, the raven child seeks new ways to reach out.

A text file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/technion/Brainary# cat brainary.txt
⠴⠂⠴⠂⠂⠴⠴⠂⠀⠴⠴⠂⠂⠴⠴⠂⠂⠀⠴⠂⠴⠴⠂⠂⠂⠴⠀⠴⠴⠂⠂⠴⠴⠴⠴⠀⠴⠂⠴⠂⠂⠴⠂⠴⠀⠴⠂⠴⠂⠴⠂⠂⠂⠀⠴⠂⠴⠴⠂⠂⠂⠴⠀⠴⠂⠂⠴⠂⠂⠂⠂⠀⠴⠂⠂⠴⠴⠴⠂⠴⠀⠴⠂⠂⠴⠂⠂⠴⠂⠀⠴⠂⠂⠴⠂⠂⠴⠴⠀⠴⠂⠂⠂⠴⠂⠂⠴⠀⠴⠂⠂⠴⠴⠴⠂⠴⠀⠴⠂⠂⠴⠂⠂⠂⠴⠀⠴⠂⠂⠂⠴⠂⠴⠴⠀⠴⠴⠂⠂⠴⠴⠴⠴⠀⠴⠂⠂⠴⠴⠴⠴⠂⠀⠴⠂⠴⠴⠴⠂⠂⠂⠀⠴⠂⠴⠂⠴⠂⠂⠴⠀⠴⠂⠂⠴⠴⠂⠂⠴⠀⠴⠂⠂⠴⠴⠂⠴⠴⠀⠴⠂⠴⠴⠴⠂⠂⠂⠀⠴⠂⠂⠴⠂⠴⠴⠴⠀⠴⠂⠂⠂⠴⠴⠴⠴⠀⠴⠂⠂⠴⠴⠴⠂⠂⠀⠴⠂⠂⠴⠂⠂⠴⠂⠀⠴⠂⠴⠂⠴⠴⠂⠴⠀⠴⠂⠂⠴⠴⠂⠂⠴⠀⠴⠂⠴⠂⠂⠴⠂⠴⠀⠴⠂⠴⠂⠂⠴⠴⠴⠀⠴⠂⠂⠴⠂⠂⠴⠴⠀⠴⠂⠂⠴⠂⠂⠴⠴⠀⠴⠂⠴⠂⠂⠴⠴⠴⠀⠴⠴⠂⠂⠴⠴⠂⠴⠀⠴⠂⠂⠴⠂⠴⠴⠴⠀⠴⠂⠂⠴⠂⠴⠴⠴⠀⠴⠂⠂⠴⠴⠴⠂⠂⠀⠴⠴⠂⠂⠴⠴⠴⠂⠀⠴⠴⠂⠂⠂⠴⠴⠂⠀⠴⠂⠂⠴⠂⠴⠴⠴⠀⠴⠂⠂⠴⠴⠂⠴⠴⠀⠴⠴⠂⠂⠴⠴⠂⠴⠀⠴⠂⠴⠴⠴⠂⠂⠴⠀⠴⠂⠂⠂⠴⠴⠂⠴⠀⠴⠂⠴⠂⠂⠴⠂⠴⠀⠴⠂⠴⠂⠴⠂⠂⠂⠀⠴⠴⠂⠂⠴⠂⠴⠂⠀⠴⠂⠂⠴⠂⠂⠴⠴⠀⠴⠂⠴⠂⠂⠴⠂⠴⠀⠴⠂⠴⠴⠂⠴⠴⠴⠀⠴⠴⠂⠂⠴⠴⠴⠴⠀⠴⠴⠂⠂⠂⠂⠴⠂
```

Looks like braille. It can be decoded with CyberChef:

```console
root@kali:/media/sf_CTFs/technion/Brainary# export NODE_PATH=$(npm root --quiet -g)
root@kali:/media/sf_CTFs/technion/Brainary# node
> const chef = require("cyberchef");
undefined
> const fs = require('fs');
> const b = fs.readFileSync('brainary.txt', 'utf8');
```

Let's decode as Braille:

```javascript
> chef.fromBraille(b)
01011001 00110011 01001110 00110000 01011010 01010111 01001110 01101111 01100010 01101101 01101100 01110110 01100010 01101110 01110100 00110000 01100001 01000111 01010110 01100110 01100100 01000111 01101000 01110000 01100011 01101101 01010010 01100110 01011010 01011000 01101100 01101100 01011000 00110010 01101000 01101000 01100011 00110001 00111001 01101000 01100100 00110010 01000110 01110010 01011010 01010111 00110101 01101100 01011010 01001000 00110000 00111101
```

Now it looks like binary:

```javascript
> chef.bake(b, [chef.fromBraille, chef.fromBinary])
Y3N0ZWNobmlvbnt0aGVfdGhpcmRfZXllX2hhc19hd2FrZW5lZH0=
```

Now it looks like Base64:

```javascript
> chef.bake(b, [chef.fromBraille, chef.fromBinary, chef.fromBase64])
cstechnion{the_third_eye_has_awakened}
```

