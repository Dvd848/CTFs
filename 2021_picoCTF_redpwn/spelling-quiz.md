# spelling-quiz
Crypto, 100 points

## Description

> I found the flag, but my brother wrote a program to encrypt all his text files. He has a spelling quiz study guide too, but I don't know if that helps.

Three files were attached.

`encrypt.py`:
```python
import random
import os

files = [
    os.path.join(path, file)
    for path, dirs, files in os.walk('.')
    for file in files
    if file.split('.')[-1] == 'txt'
]

alphabet = list('abcdefghijklmnopqrstuvwxyz')
random.shuffle(shuffled := alphabet[:])
dictionary = dict(zip(alphabet, shuffled))

for filename in files:
    text = open(filename, 'r').read()
    encrypted = ''.join([
        dictionary[c]
        if c in dictionary else c
        for c in text
    ])
    open(filename, 'w').write(encrypted)
```

`flag.txt`:
```
brcfxba_vfr_mid_hosbrm_iprc_exa_hoav_vwcrm
```

`study-guide.txt`:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/spelling-quiz]
└─$ cat public/study-guide.txt| head
gocnfwnwtr
sxlyrxaic
dcrrtfrxcv
uxbvwavcq
lwvicwtiwm
pwtmwnxvicq
avingciisa
ylwtmrcawx
mwaxdcrrxuwlwvq
yciflwnf

┌──(user@kali)-[/media/sf_CTFs/pico/spelling-quiz]
└─$ wc -l public/study-guide.txt
272543 public/study-guide.txt
```

## Solution

From the source code we see that the script encrypted both the flag and the study guide with a simple substitution cipher using a random key. So, we just need to find a key which decrypts the study guide to a sensible result.

[subbreaker](https://gitlab.com/guballa/SubstitutionBreaker) can easily break the substitution cipher with just a subset of the words:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/spelling-quiz]
└─$ subbreaker break --lang EN --ciphertext <(cat public/study-guide.txt | head -n 50)
Alphabet: abcdefghijklmnopqrstuvwxyz
Key:      xunmrydfwhglstibjcavopezqk
Fitness: 92.78
Nbr keys tried: 6175
Keys per second: 3418
Execution time (seconds): 1.807
Plaintext:
kurchicine
malfeasor
greenheart
baptistry
litorinoid
vindicatory
stockrooms
flindersia
disagreeability
frohlich
disamenity
outsparspying
preinclination
melanizing
preobedient
chloralformamide
nonelectrolytic
ascertainable
thoracoplasties
pinnaclet
paperweights
incarnation
nonpuerility
unprefined
brasslike
transpositive
glycerol
idolatrizer
hyperoartia
perimedullary
rendition
monstricide
extraspectral
monumentally
cholehematin
overrealism
dinnerless
carpellum
barrulee
extrovertedness
necessar
evincing
perspectivism
plasmolyzability
noctilucal
intertarsal
essoinment
paratypic
borstall
misadressing
```

Let's use the key to decipher the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/spelling-quiz]
└─$ subbreaker decode --key xunmrydfwhglstibjcavopezqk --ciphertext public/flag.txt
perhaps_the_dog_jumped_over_was_just_tired
```

The flag: `picoCTF{perhaps_the_dog_jumped_over_was_just_tired}`