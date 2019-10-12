# whats-the-difference
General Skills, 200 points

## Description:
> Can you spot the difference? kitters cattos.


## Solution: 

We receive two images: `cattos.jpg` and `kitters.jpg`. Visually, they seem similar, however `cattos.jpg` has some kind of corruption.

Let's diff them:
```console
root@kali:/media/sf_CTFs/pico/whats-the-difference# diff cattos.jpg kitters.jpg
Binary files cattos.jpg and kitters.jpg differ
```

Let's take a look at the difference itself (note that `cmp` uses a decimal system for offsets and octal for bytes)
```console
root@kali:/media/sf_CTFs/pico/whats-the-difference# cmp -l cattos.jpg kitters.jpg
  49734 160 231
  49735 151 235
  49736 143 230
  49737 157 310
  87663 103  12
 162650 124 364
 175231 106 153
 175232 173 261
 211986 164 230
 211987 150 222
 211988  63  15
 211989 171 330
 284427 162 122
 292340  63 174
 292341 137 371
 331830 141 216
 331831  65 365
 426632 137 346
 439903 144 360
 515770  61 112
 515771 146 252
 583608 146 341
 640996  63 310
 688795 162  77
 688796  63 107
 702943 156  23
 751424 164 243
 754731 137  61
 754732  64 113
 754733 163 274
 754734 137 304
 796226 142  43
 871159 165 256
 871160  67   6
 871161  67 316
 871162  63 346
 927506 162 347
 927507 137 212
 927508  64 122
 994666 156 376
 994667 144  43
 994668 137 377
1068577 152 234
1068578  63 344
1068579  61 203
1068580  61 222
1068581 171 162
1101444 137   5
1101445 141 173
1101446 163   7
1101447 154 300
1171017 153  53
1171018 152 147
1171019 146 356
1241182 144  51
1241183 163 224
1241184 141 200
1241185 154 106
1272572 153 217
1272573 146 156
1337150 163   4
1410459 154 345
1410460 153 340
1460208 146   5
1510914 154 237
1567157 153 322
1567158 152 100
1567159 144  42
1567160 163 220
1567161 146 205
1581925 144 315
1581926 163 244
1581927 172 215
1581928 155 147
1581929 172 257
1677065  61 176
1677066  60 341
1764510  65 357
1764511  64 210
1764512  70  31
1766742 175  13
```

The second column looks like ASCII, let's try to decode:

```console
root@kali:/media/sf_CTFs/pico/whats-the-difference# cmp -l cattos.jpg kitters.jpg | gawk '{printf "%c", strtonum(0$2)}' && echo
picoCTF{th3yr3_a5_d1ff3r3nt_4s_bu773r_4nd_j311y_aslkjfdsalkfslkflkjdsfdszmz10548}
```
