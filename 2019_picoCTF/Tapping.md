# Tapping
Cryptography, 200 points

## Description:
> Theres tapping coming in from the wires.

## Solution: 

Let's connect to the service:
```console
root@kali:/media/sf_CTFs/pico/Tapping# nc 2019shell1.picoctf.com 37920
.--. .. -.-. --- -.-. - ..-. { -- ----- .-. ... ...-- -.-. ----- -.. ...-- .---- ... ..-. ..- -. ..... ---.. ...-- ----. ----- ----- ----. ---.. .---- }
```

This looks like morse code.

We can decode this with the [CyberChef Online API](https://github.com/gchq/CyberChef/wiki/Node-API).

If you're using Kali, the NPM package which comes with the OS is outdated, and in order to update it you need to follow [these](https://npmerror.com/how-to-update-npm-in-linux/#comment-5) instructions. Then, you can install the CyberChef package globally using `npm install -g cyberchef  --unsafe-perm=true --allow-root`.

Now we can decode the message using:
```console
root@kali:/media/sf_CTFs/pico/Tapping# export NODE_PATH=$(npm root --quiet -g)
root@kali:/media/sf_CTFs/pico/Tapping# node
> const chef = require("cyberchef");
> chef.fromMorseCode(".--. .. -.-. --- -.-. - ..-.").toString() + "{" + chef.fromMorseCode(" -- ----- .-. ... ...-- -.-. ----- -.. ...-- .---- ... ..-. ..- -. ..... ---.. ...-- ----. ----- ----- ----. ---.. .---- ").toString() + "}"
'PICOCTF{M0RS3C0D31SFUN583900981}'
```



