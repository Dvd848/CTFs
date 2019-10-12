# la cifra de
Cryptography, 200 points

## Description:
> I found this cipher in an old book. Can you figure out what it says?


## Solution: 

We connect to the service and get the following output:
```console
root@kali:/media/sf_CTFs/pico/la_cifra_de# nc 2019shell1.picoctf.com 37608
Encrypted message:
 Ne iy nytkwpsznyg nth it mtsztcy vjzprj zfzjy rkhpibj nrkitt ltc tnnygy ysee itd tte cxjltk

Ifrosr tnj noawde uk siyyzre, yse Bnretèwp Cousex mls hjpn xjtnbjytki xatd eisjd

Iz bls lfwskqj azycihzeej yz Brftsk ip Volpnèxj ls oy hay tcimnyarqj dkxnrogpd os 1553 my Mnzvgs Mazytszf Merqlsu ny hox moup Wa inqrg ipl. Ynr. Gotgat Gltzndtg Gplrfdo

Ltc tnj tmvqpmkseaznzn uk ehox nivmpr g ylbrj ts ltcmki my yqtdosr tnj wocjc hgqq ol fy oxitngwj arusahje fuw ln guaaxjytrd catizm tzxbkw zf vqlckx hizm ceyupcz yz tnj fpvjc hgqqpohzCZK{m311a50_0x_a1rn3x3_h1ah3xj62p044a}

Zmp fowdt cjwl-jtnusjytki oeyhcivytot tq a vtwygqahggptoh nivmpr nthebjc, wgx xajj lruzyd 1467 hd Weus Mazytszf Llhjcto.

Yse Bnretèwp Cousex nd tnjceltce ytxeznxey hllrjo tnj Llhjcto Itsi tc Argprzn Nivmpr.

Os 1508, Uonfynkx Eroysesnfs osgetypd zmp su-hllrjo tggflg wpczf (l mgycid tq snnqtki llvmlbkyd) tnfe wuzwd rfeex gp a iwttohll itxpuspnz tq tnj Gimjyèrk Htpnjc.

Bkqwayt’d skhznj gzoqqpt guaegwpd os 1555 ls g hznznyugytot tq tnj qixxe. Tnj wocjc hgqgey tq tnj llvmlbkyd axj yoc xsilypd xjrurfcle, gft zmp arusahjes gso tnj tnjji lkyeexx lrk rtxki my sjlny tq a sspmustc qjj pnwlsk, bsiim nat gp dokqexjyt cneh kfnh itcrkxaotipnz.
```

[This site](https://www.guballa.de/vigenere-solver) is great at breaking Vigenère ciphers without knowing the key.

The result:

```
 It is interesting how in history people often receive credit for things they did not create

During the course of history, the Vigenère Cipher has been reinvented many times

It was falsely attributed to Blaise de Vigenère as it was originally described in 1553 by Giovan Battista Bellaso in his book La cifra del. Sig. Giovan Battista Bellaso

For the implementation of this cipher a table is formed by sliding the lower half of an ordinary alphabet for an apparently random number of places with respect to the upper halfpicoCTF{b311a50_0r_v1gn3r3_c1ph3re62e044a}

The first well-documented description of a polyalphabetic cipher however, was made around 1467 by Leon Battista Alberti.

The Vigenère Cipher is therefore sometimes called the Alberti Disc or Alberti Cipher.

In 1508, Johannes Trithemius invented the so-called tabula recta (a matrix of shifted alphabets) that would later be a critical component of the Vigenère Cipher.

Bellaso’s second booklet appeared in 1555 as a continuation of the first. The lower halves of the alphabets are now shifted regularly, but the alphabets and the index letters are mixed by means of a mnemonic key phrase, which can be different with each correspondent.
```

The flag: `picoCTF{b311a50_0r_v1gn3r3_c1ph3re62e044a}`.

The key was:
```
Key	"flag"
Key length	4
Cipher text length	952
Ratio
(cipher_len:key_len)	238.00
Difficulty
	easy
Clear text score (fitness)
	99.57
```