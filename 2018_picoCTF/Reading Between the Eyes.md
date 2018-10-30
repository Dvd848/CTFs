# Reading Between the Eyes
Forensics, 150 points

## Description:
> Stego-Saurus hid a message for you in this image, can you retreive it? 
![Husky](images/husky.png)



## Solution:
We can use [zsteg](https://github.com/zed-0xff/zsteg) in order to recover the flag.
`zsteg` is a tool to detect stegano-hidden data in PNGs & BMPs.

```console
root@kali:/media/sf_CTFs/pico/Reading_Between_the_Eyes# zsteg husky.png 
b1,r,lsb,xy         .. text: "^5>c[rvyzrf@"
b1,rgb,lsb,xy       .. text: "picoCTF{r34d1ng_b37w33n_7h3_by73s}"
b1,abgr,msb,xy      .. file: PGP\011Secret Sub-key -
b2,g,msb,xy         .. text: "ADTU@PEPA"
b3,abgr,msb,xy      .. text: "t@Wv!Wt\tGtA"
b4,r,msb,xy         .. text: "0Tt7F3Saf"
b4,g,msb,xy         .. text: "2g'uV `3"
b4,b,lsb,xy         .. text: "##3\"TC%\"2f"
b4,b,msb,xy         .. text: " uvb&b@f!"
b4,rgb,lsb,xy       .. text: "1C5\"RdWD"
b4,rgb,msb,xy       .. text: "T E2d##B#VuQ`"
b4,bgr,lsb,xy       .. text: "A%2RTdGG"
b4,bgr,msb,xy       .. text: "EPD%4\"c\"#CUVqa "
b4,rgba,lsb,xy      .. text: "?5/%/d_tO"
b4,abgr,msb,xy      .. text: "EO%O#/c/2/C_e_q"
```

The flag: picoCTF{r34d1ng_b37w33n_7h3_by73s}