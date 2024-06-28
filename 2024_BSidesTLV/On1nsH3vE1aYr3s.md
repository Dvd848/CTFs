# On1nsH3vE1aYr3s

 * Category: Steganography
 * 100 Points
 * Solved by the JCTF Team

## Description

> I Know the flag was there, I remember it was made out of 3 pieces...
> 
> What i forgot is how to extract it, maybe i made it easy for myself and it wont take me a year to solve it.....
> 
> Can you find the pieces and assemble the flag ?
> 
> when you find the flag dont forget to submit it in the correct format


## Solution

In the attached website we find a list of multiple image files to download, e.g.:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/On1nsH3vE1aYr3s]
└─$ curl https://bstlv24-on1nsh3ve1ayr3s.chals.io/ -s | tail
<li><a href="image_yyDAyx4YlHmgc3jYGkFa4y8.png"> image_yyDAyx4YlHmgc3jYGkFa4y8.png</a></li>
<li><a href="image_z0n6YwRN75nlwEPYsJRLZzq.png"> image_z0n6YwRN75nlwEPYsJRLZzq.png</a></li>
<li><a href="image_z2qZ3YTPBbBsEQv5ce7wdxT.png"> image_z2qZ3YTPBbBsEQv5ce7wdxT.png</a></li>
<li><a href="image_z9CA5heiTCp12yw7ymzYnSA.png"> image_z9CA5heiTCp12yw7ymzYnSA.png</a></li>
<li><a href="image_zNpHouxFeyWLbNGYVXrHXiZ.png"> image_zNpHouxFeyWLbNGYVXrHXiZ.png</a></li>
<li><a href="image_zQN9CqhnAe85if9bLAhgXdO.png"> image_zQN9CqhnAe85if9bLAhgXdO.png</a></li>
<li><a href="image_zdk92CAzWUtG2S4ziBV02yQ.png"> image_zdk92CAzWUtG2S4ziBV02yQ.png</a></li>
<li><a href="image_zp2g5rcvHQSxJBoAjBnawKf.png"> image_zp2g5rcvHQSxJBoAjBnawKf.png</a></li>
</ul>
</body></html>
```

Let's download all the files:

```console
URL="https://bstlv24-on1nsh3ve1ayr3s.chals.io/" && wget -q -O - "$URL" | grep -oP '(?<=href=")[^"]*' | while read -r link; do wget "$URL/$link"; done
```

Each image contained a short text, e.g. "{I will have overtime pay on this paycheck}".  
We'll use an OCR to find the ones that contain the flag format:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/On1nsH3vE1aYr3s/tmp]
└─$ find . -name "*.png" | xargs -I {} sh -c 'if tesseract "{}" stdout | grep -q "^BSidesTLV"; then echo {}; fi' 2>/dev/null
./image_pYNVWis8fR73UaVDD9v2024.png
./image_UeP0IjxMNfl5k6CxmJ02024.png
./image_58dki2vB3jxNmUXemaW2024.png
```

OCR didn't manage to correctly extract the full file contents, but here it is:

```
BSidesTLV2024{_o___o__i__V__t__m}
BSidesTLV2024{G__dM__n_n__i__n__}
BSidesTLV2024{__0___r___9__3__A_}
```

Combine the three and we get `BSidesTLV2024{Go0dM0rnin9Vi3tnAm}`.