# README
Category: Misc., 100 Points

## Description

>  Readme

A zip file was attached.

## Solution

Let's check the attached file:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/README]
└─$ unzip readme.zip
Archive:  readme.zip
 extracting: key_checker_data
  inflating: readme
  inflating: solution_checker.py
```

We get a Python file:

```python
from Crypto.Cipher import ARC4

def check_key(key, key_checker_data):
    """ returns True is the key is correct.
        Usage:
        check_key('{I_think_this_is_the_key}', key_checker_data)
    """
    return ARC4.new(("CSA" + key).encode()).decrypt(key_checker_data) == b'success'

```

A binary file:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/README]
└─$ xxd -g 1 key_checker_data
00000000: e0 33 70 95 a1 e5 31                             .3p...1
```

And a README file:

```
The flag used to be {hey_that_is_the_great_puzzle}, but its gone :(
-------------

41ic3 3@S b391nn1ng T* 93t V3Ry TlrE6 *7 51ttin9 8Y HER 515TER 0n TH3 b@nK, 42D *7 hAVln9 20thing T* D0: *2©3 0R T3lc3 Sh3 H@D p3Ep36 12T* tHE 800k H3r SiST3r wAS r34D1ng, b[T 1t H4D n* 91CT[rE5 0R C02v3R5@t10n5 l2 lT, 'An6 3H4t 1S thE uS3 *F @ 8*0k,' Th0ugHt 41lcE 'wiTH*[t 9iCt[R35 0r ©0nvERS@t1*n?'
5* Sh3 3A5 ©*2SlDEr129 1n HEr 032 8126 (A5 wELL @5 SHE ©0[16, 70r tH3 h*t 64Y 8A63 h3r F33L VERY 513Epy @26 ST[pl6), wHETh3R tH3 9LE@SuR3 *F 84king 4 64lSy-Ch@in 30u1D b3 30rTh th3 TR*u813 *7 g3tTin9 up 42D plCKin9 thE DAlS1E5, wH3n S[DD3nLy 4 3HlT3 r4881t 3lTH 9lnk 3YE5 RAn ©1053 by hEr.
ThERE w4S n0Thin9 5* VEry r384RKA8L3 i2 tHAT; 20R Di6 411©3 TH12K it S* v3Ry MuCh 0uT 0F Th3 34y T0 HE@R TH3 R@8biT S4y t* 1t53L7, '*h 63@r! *h 63AR! 1 Sh4L1 b3 1@T3!' (3HEn 5H3 tH0[ghT 1T *vER 4Ft3R34R6S, iT *c©[rr3D T* h3R TH@t ShE 0u9HT T* h4vE w02DEr3D 4T thiS, buT At th3 Ti8E iT 4L1 5E3M3D QulTE 24T[rA1); 8[T whEn th3 RAbb1t A©tu@L1y t*0K 4 w@tch 0[t 07 1TS 3A1STC*4T-90cKET, A2D L**K3D 4T 1T, @n6 Th32 h[rR13D *n, @1i©3 5T@Rt36 T0 h3R 7E3T, 70R lT 71@Sh3D 4Cr*S5 HER 8l2D tH4t SHE h@D nEVEr 83F0rE 5E32 4 R48bit wltH 3iThEr 4 w@l5TC0At-90CkET, 0r A 34T©h T* t@kE *[T *F iT, An6 8[rning witH c[R10SiTY, 5H3 rAn Acr055 th3 Fi3LD A7t3R 1T, @2D 70Rtun@T31y w45 J[5T 12 tlM3 t0 S33 iT p09 D0w2 A 14rgE rAbblt-H0LE [nD3R tH3 HEDg3.
1n @n*Th3R 808Ent 6032 332T A11C3 4FtEr 1t, 2EVEr 02©E c*25163rl29 h*3 1n THE 3*rL6 Sh3 w45 t* gEt *ut @g4i2.
thE R48blt-h0LE 3E2t StrA19hT *2 LlKE @ t[nn31 F0R 508E 34Y, @n6 ThEn 6i9p3D SuD6EnLY 6*3n, S0 5[6D3n1y thAt 4Li©3 h@D n*T A 8*MEnt t* THl2K Ab*[T 5T099i29 h3RS31F bEF0RE Sh3 F0unD hERS3L7 F4L11n9 6*wn 4 V3Ry 633p 331L.
3ith3r THE w31L w45 v3rY 6339, 0r 5hE FEL1 vERy 51*wLy, 70r 5h3 hAD p13nTy *F T1ME @5 SHE wEnt D0w2 t* 10*k @8*ut h3r @26 T* w0n63r 3h@t w@S 90129 t0 h@p93n 23*t. Flr5T, 5H3 tRlED T0 L0*k D0wn @nD 8@KE 0[T 3H4T SHE 34S ©*812g t*, but 1t wAS T0* 6ArK T0 53E 42yThi2g; TH3n Sh3 L00K36 @t th3 SiD3S 0F thE wELL, 4n6 2*Tic36 ThAT ThEY 3ER3 7iL13D 3lth Cu9b0AR65 @26 b00k-Sh3LvE5; hErE @26 THERE 5h3 5@w 8@95 @26 picT[rES h[n9 u9*2 9E95. 5hE t00K D0wn A 7@R FR*8 *n3 *7 ThE 5h3LV35 4S 5HE pAS53D; it 34S LA831LE6 '0RAn93 M4RM4146E', 8[T t* h3r grEAT 615App0i2T8EnT 1t wAS EMpty: SHE 6l6 n0t L1K3 T0 6r*p THE J4R 70R 7EAr 07 k11Ll29 5*MEb*6Y, 5* 84n4g3D t0 9ut it l2T* 02E *7 Th3 C[9b*4r65 A5 SHE 7E1L p@5t 1t.
'wE1L!' tH*[9hT @1l©3 t0 hEr5ELF, '@7TER S[CH A 7A11 45 th15, i Sh411 Thi2K n0tHi2g 0F T[88Li29 D*3n STAirS! H0w 8r@v3 Th3y'L1 AL1 THl2k 83 4t H08E! wHy, i w0uL6n'T SAy A2YThi2g A80[T 1T, EVE2 i7 i 7EL1 0F7 ThE t09 07 th3 H*[5E!' (3h1ch 3@S vErY 1iK31Y Tr[3.)
D03n, D*32, D*32. 3*u16 ThE 7ALL 2EV3r c*8E t* @n EnD! 'l w0n63r H*3 8@2y 811ES 1'vE F41L3n 8Y ThlS tlM33' 5h3 SAiD AL0u6. '1 MuST 83 9EtT12g S08E3hErE nEAr th3 c3nTR3 *F THE E@RtH. 1ET M3 533: TH@t w0uL6 b3 F*[r tH0uSAn6 811E5 D*3n, 1 THink--' (70r, y0[ 5EE, @LiCE HAD 1E4r2T 53VEr41 thl2g5 *F thiS S*RT l2 H3r 13S5*n5 12 THE ScH001r00M, A2D th0[gH Th1S 34S n*t A vERY g0*6 *pp*rTu2itY F0R 5h03lng 07F H3r k2*31ED93, @S th3R3 w@S n* 0n3 T* 1l5T32 T0 H3r, StiLL 1T wA5 9*0D 9R@©ti©3 T* S4y iT *vEr) '--Y35, tHAt'5 @8*ut thE Ri9HT 61StA2©3--8[T thE2 l w02DER 3h@t 14TlTuDE *R 102giT[63 1'V3 g*t t0?' (4LlcE h@6 20 i6E@ 3h4T 1Atlt[6E 3@5, *R L*29ltu63 ElthEr, b[T tH0[ght thEY wErE 2i©3 gr@26 30RDS T* SAy.)
9rESE2tLY Sh3 bEgA2 A9@l2. '1 w*nDER l7 1 Sh41L 74L1 rlgHT THR*[gh th3 EArTh! H0w 7[22y it'L1 SE3M t* C0M3 *[t A8*29 th3 p30pL3 thAt 34Lk w1tH thElr HE4D5 D0wnw4r6! tHE A2tip@tH1ES, 1 THlnk--' (ShE wA5 RAtH3R 91AD tH3R3 34S 2* 023 1l5TE2i2g, tH1S Ti8E, AS 1t 6lD2'T S*[n6 @t AL1 tH3 rl9Ht 30RD) '--b[T i 5h4L1 HAV3 T* 4SK th3M wh4t tHE n@M3 07 THE C0unTRY 15, y*u Kn*3. 9LE@S3, 8@'4M, 1S Thl5 n3w ZEA14n6 0r 4[5tR@Li4?' (426 Sh3 triED T0 CurtSEY 4S 5HE Sp*K3--F4n©y C[RTS3Ying A5 Y*['r3 F@L1ing thR0[gh tHE 4lr! 60 Y*u Thl2K y*[ ©*[L6 84nA93 lT?) '426 3h4t An lgn*r42t 11TT1E giR1 5H3'11 THinK 83 7*R ASK12g! 20, iT'LL nEvEr D0 t0 A5K: 9ERH@p5 1 5HALL S33 it 3RltT3n [9 508EwH3RE.'

-------------

Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, "and what is the use of a book," thought Alice "without pictures or conversation?"
So she was considering in her own mind (as well as she could, for the hot day made her feel very sleepy and stupid), whether the pleasure of making a daisy-chain would be worth the trouble of getting up and picking the daisies, when suddenly a White Rabbit with pink eyes ran close by her.
There was nothing so very remarkable in that; nor did Alice think it so very much out of the way to hear the Rabbit say to itself, "Oh dear! Oh dear! I shall be late!" (when she thought it over afterwards, it occurred to her that she ought to have wondered at this, but at the time it all seemed quite natural); but when the Rabbit actually took a watch out of its waistcoat-pocket, and looked at it, and then hurried on, Alice started to her feet, for it flashed across her mind that she had never before seen a rabbit with either a waistcoat-pocket, or a watch to take out of it, and burning with curiosity, she ran across the field after it, and fortunately was just in time to see it pop down a large rabbit-hole under the hedge.
In another moment down went Alice after it, never once considering how in the world she was to get out again.
The rabbit-hole went straight on like a tunnel for some way, and then dipped suddenly down, so suddenly that Alice had not a moment to think about stopping herself before she found herself falling down a very deep well.
Either the well was very deep, or she fell very slowly, for she had plenty of time as she went down to look about her and to wonder what was going to happen next. First, she tried to look down and make out what she was coming to, but it was too dark to see anything; then she looked at the sides of the well, and noticed that they were filled with cupboards and book-shelves; here and there she saw maps and pictures hung upon pegs. She took down a jar from one of the shelves as she passed; it was labelled "ORANGE MARMALADE", but to her great disappointment it was empty: she did not like to drop the jar for fear of killing somebody, so managed to put it into one of the cupboards as she fell past it.
"Well!" thought Alice to herself, "after such a fall as this, I shall think nothing of tumbling down stairs! How brave they"ll all think me at home! Why, I wouldn"t say anything about it, even if I fell off the top of the house!" (Which was very likely true.)
Down, down, down. Would the fall never come to an end! "I wonder how many miles I"ve fallen by this time?" she said aloud. "I must be getting somewhere near the centre of the earth. Let me see: that would be four thousand miles down, I think--" (for, you see, Alice had learnt several things of this sort in her lessons in the schoolroom, and though this was not a VERY good opportunity for showing off her knowledge, as there was no one to listen to her, still it was good practice to say it over) "--yes, that"s about the right distance--but then I wonder what Latitude or Longitude I"ve got to?" (Alice had no idea what Latitude was, or Longitude either, but thought they were nice grand words to say.)
Presently she began again. "I wonder if I shall fall right through the earth! How funny it"ll seem to come out among the people that walk with their heads downward! The Antipathies, I think--" (she was rather glad there WAS no one listening, this time, as it didn"t sound at all the right word) "--but I shall have to ask them what the name of the country is, you know. Please, Ma"am, is this New Zealand or Australia?" (and she tried to curtsey as she spoke--fancy curtseying as you"re falling through the air! Do you think you could manage it?) "And what an ignorant little girl she"ll think me for asking! No, it"ll never do to ask: perhaps I shall see it written up somewhere."

```

What we have in the README is a chapter from *Alice in Wonderland* quoted twice, once in plain english and once in [leetspeak](https://en.wikipedia.org/wiki/Leet). The README also says that "The flag used to be {hey_that_is_the_great_puzzle}". So, the obvious thing to do is to try and translate the original flag to leetspeak. However, in leetspeak, each English letter can be mapped to multiple results, how do we know how to translate the flag? Well, we just try all of them, utilizing `check_key` to see if we got the correct one.

First, we'll compare the original and translated text to check what are the mappings used by this leetspeak dialect:

```python
from collections import defaultdict
from pprint import pprint

with open("readme", "r", encoding="utf8") as f:
    data = f.read()
    preface, leetspeak_text, original_text = map(lambda x: x.strip(), data.split("-------------"))

    assert(len(leetspeak_text) == len(original_text))

    leet_map = defaultdict(set)

    for a, b in zip(leetspeak_text, original_text):
        leet_map[b].add(a)

    pprint(leet_map)
```

Running it, we get:

```
┌──(user@kali)-[/media/sf_CTFs/checkpoint/README]
└─$ python3 solve.py
defaultdict(<class 'set'>,
            {'\n': {'\n'},
             ' ': {' '},
             '!': {'!'},
             '"': {"'"},
             '(': {'('},
             ')': {')'},
             ',': {','},
             '-': {'-'},
             '.': {'.'},
             ':': {':'},
             ';': {';'},
             '?': {'?', '3'},
             'A': {'@', '4', 'A'},
             'D': {'D', '6'},
             'E': {'E', '3'},
             'F': {'F'},
             'G': {'9'},
             'H': {'H'},
             'I': {'1', 'i', 'l'},
             'L': {'1', 'L'},
             'M': {'M', '8'},
             'N': {'n', '2'},
             'O': {'*', '0'},
             'P': {'9'},
             'R': {'R', 'r'},
             'S': {'S', '5'},
             'T': {'T', 't'},
             'V': {'v'},
             'W': {'3', 'w'},
             'Y': {'Y'},
             'Z': {'Z'},
             'a': {'@', '4', 'A'},
             'b': {'8', 'b'},
             'c': {'C', 'c', '©'},
             'd': {'D', '6'},
             'e': {'E', '3'},
             'f': {'7', 'F'},
             'g': {'g', '9'},
             'h': {'h', 'H'},
             'i': {'i', '1', 'l'},
             'j': {'J', '7'},
             'k': {'k', 'K'},
             'l': {'1', 'L'},
             'm': {'8', 'M'},
             'n': {'n', '2'},
             'o': {'*', '0'},
             'p': {'p', '9'},
             'q': {'Q'},
             'r': {'R', 'r'},
             's': {'S', '5'},
             't': {'T', 't'},
             'u': {'[', 'u'},
             'v': {'V', 'v'},
             'w': {'3', 'w'},
             'x': {'*'},
             'y': {'Y', 'y'}})
```

Now let's use backtracking to iterate over all possible options:

```python
import solution_checker

with open("key_checker_data", "rb") as f:
    key_checker_data = f.read()

flag = "{hey_that_is_the_great_puzzle}"

def recurse(index, candidate):
    if index == len(flag):
        str_candidate = "".join(candidate)
        if solution_checker.check_key(str_candidate, key_checker_data):
            return str_candidate
        return None
    
    for c in leet_map.get(flag[index], flag[index]):
        candidate[index] = c
        result = recurse(index + 1, candidate)
        if result is not None:
            return result

print(f"[*] Searching for flag...")
result = recurse(0, list(flag))
if result:
    print(f"[*] Found flag: CSA{result}" )
else:
    print(f"[-] Can't find flag!" )
```

After running for a while, the script returns with an empty result. Notice however that the original flag includes a `z`, but the `leet_map` we've constructed does not. So, it looks like we need to manually add a mapping for `z`:

```python
leet_map['z'].add('Z')
leet_map['z'].add('z')
```

We can obviously add even more mapping, such as:

```python
for k, v in leet_map.items():
    v.add(k.lower())
    v.add(k.upper())
```

However each such entry multiples the amount of candidates and lengthens the runtime, so we need to be careful about adding too many options.

After adding a mapping for `z`, we run the script again and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/README]
└─$ python3 solve.py
[*] Searching for flag...
[*] Found flag: CSA{hEY_th@T_l5_thE_9RE4T_p[ZZL3}
```