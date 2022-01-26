# cq_cq_cq
Category: Crypto, 200 Points

## Description

A text file was attached: `A_Message_from_Mr._Phillips.txt`

```
10:38 18 -> CQ
FROM MISTER PHILLIPS TO ALL AGENTS STOP PLEASE UPDATE STOP
 
10:42 33 -> 18
FROM AGENT THREE FOUR SEVEN TO MISTER PHILLIPS STOP ALL IS READY STOP NEED FLAG STOP
 
11:17 18 -> CQ
HSVNN PBLMS XATWW PEBXT CRRCB MULAA LCDLO MGRKI
PALAC VXMUE CSWIK GVLQZ DALRC AACQT BZMYA EVSME
SIXDV CUWMS BLVSR BXGPQ FATUM MSAQV YMVKX QERVB
FLTSR ATSKE ERQBB XTE

```

## Solution

This obviously is some kind of cipher, but before decoding it we need to identify it. Practical Cryptography has a good page about [Identifying Unknown Ciphers](http://practicalcryptography.com/cryptanalysis/text-characterisation/identifying-unknown-ciphers/). One of the suggestions is to count the number of unique characters in the cipher (excluding spaces):

```python
>>> c = """HSVNN PBLMS XATWW PEBXT CRRCB MULAA LCDLO MGRKI
... PALAC VXMUE CSWIK GVLQZ DALRC AACQT BZMYA EVSME
... SIXDV CUWMS BLVSR BXGPQ FATUM MSAQV YMVKX QERVB
... FLTSR ATSKE ERQBB XTE"""
>>> "".join(sorted(set(c.replace(" ", "").replace("\n", ""))))
'ABCDEFGHIKLMNOPQRSTUVWXYZ'
>>> len("".join(sorted(set(c.replace(" ", "").replace("\n", "")))))
25
```

We have `25` unique characters:

> If there are 26 characters in the ciphertext, it rules out ciphers based on a 5 by 5 grid such as **playfair**, **foursquare** and **bifid**. If the ciphertext is fairly long and only 25 characters are present, it may indicate a cipher in this class has been used.

So there are three suggestion to check. Notice how the missing letter is `J`, which is pretty common for these ciphers (`J` is usually combined with `I`). If we're talking about a cipher that encrypts pairs of letters, this makes it significantly stronger than substitution ciphers since frequency analysis becomes much more difficult.

Studying the two plaintext messages we've received, we can see that both messages start with the pattern of "FROM X TO Y STOP" and end with "STOP". That gives us a [crib](https://en.wikipedia.org/wiki/Known-plaintext_attack) - a portion of plaintext that we assume is present in the ciphertext. Having a crib is very helpful in deciphering an encrypted message.

Since the third message is marked as `18 -> CQ` - just like the first one - we'll assume that it starts with "FROM MISTER PHILLIPS TO ALL AGENTS STOP". We'll also assume that it ends with "STOP":

```
fromm ister phill ipsto allag entss top
HSVNN PBLMS XATWW PEBXT CRRCB MULAA LCDLO MGRKI

PALAC VXMUE CSWIK GVLQZ DALRC AACQT BZMYA EVSME

SIXDV CUWMS BLVSR BXGPQ FATUM MSAQV YMVKX QERVB

                s top
FLTSR ATSKE ERQBB XTE
```

We have a few encouraging things here:

* Same letter encryption in `fromm` -> `HSVNN`
* Same letter encryption in `phill` -> `XATWW`
* Same letter encryption in `allag` -> `CRRCB`
* Same letter encryption in `entss` -> `MULAA`

So there's a good chance that we got the crib right.

Now, a few things here tell us that this is not Playfair:

 * In Playfair it's impossible to find a bigram composed of 2 identical letters, but we have `AA` (part of `MULAA`)
 * A Playfair digraph and its reverse (e.g. AB and BA) will decrypt to the same letter pattern in the plaintext (e.g. RE and ER), but we have `il -> TW` and `li -> WP`.

Ruling out Four-Square and Bifid is harder, but it's also pretty hard to crack these ciphers with the information we currently have. So, we return to the description for additional clues. The main thing that stands out in the description is the use of the name "Phillips". As it turns out, that's also the name of a [cipher](https://www.cryptogram.org/downloads/aca.info/ciphers/Phillips.pdf) that seems to fit very well with what we have:

> Starting with a basic 5x5 Polybius square, the first row is shifted down one row at a time form squares #2, 3, 4 and 5. Row two is then shifted down a row at a time to form squares #6, 7 and 8. Each square is  used in turn to encipher 5 plaintext letters. Each plaintext letter is enciphered by taking as substitute the letter diagonally down to the right using the proper square. A plaintext letter in the fifth column is replaced by the letter from the first column and the row below it; a plaintext letter in the fifth row is replaced by the letter in the first row and to its right.

It's worth stopping now and reviewing the example in the link above, since we won't repeat an example here.

As explained in the link, a 5x5 master box is formed from the key, and seven additional boxes are created by swapping rows in the master box. A Python representation of the row order would be:

```python
boxes = [
    [0, 1, 2, 3, 4],
    [1, 0, 2, 3, 4],
    [1, 2, 0, 3, 4],
    [1, 2, 3, 0, 4],
    [1, 2, 3, 4, 0],
    [2, 1, 3, 4, 0],
    [2, 3, 1, 4, 0],
    [2, 3, 4, 1, 0]
]
```

Among other things, this means that for each of the eight columns, the same ciphertext character is replaced by the same plaintext character:

```
fromm ister phill ipsto allag entss top
HSVNN PBLMS XATWW PEBXT CRRCB MULAA LCDLO MGRKI

        e                  la ss                
PALAC VXMUE CSWIK GVLQZ DALRC AACQT BZMYA EVSME

r   o    er       st i        e s              
SIXDV CUWMS BLVSR BXGPQ FATUM MSAQV YMVKX QERVB

   r    r      ss top
FLTSR ATSKE ERQBB XTE
```

Now, in order to crack the cipher, we'd like to first understand which letters belonged to which rows. Let's take for example the first character: `f` was encrypted to `H`. This means that in the first box, `f`'s row and `H`'s row were adjacent. We can use the following code to list all adjacent rows:

```python
def adjacent_pairs(lst):
    """
    Returns tuples of adjacent pairs in the list, with wraparound for the last row:
    [1, 2, 3, 0, 4] -> [(1, 2), (2, 3), (3, 0), (0, 4), (4, 1)]
    """
    length = len(lst)
    for i in range(length):
        yield (lst[i], lst[(i + 1) % length])
```

In our case, that means that `(f, H)` were in rows:

```python
>>> list(adjacent_pairs(boxes[0]))
[(0, 1), (1, 2), (2, 3), (3, 4), (4, 0)]
```

Now let's see what we can learn from combining several constraints. We can see that `t` was encrypted in several columns. For the second, sixth and seventh, it was encrypted to `L`. For the forth, it was encrypted to `X`. We'll check what that means about the row numbers:

```python
>>> list(adjacent_pairs(boxes[1])) # Second
[(1, 0), (0, 2), (2, 3), (3, 4), (4, 1)]
>>> list(adjacent_pairs(boxes[5])) # Sixth
[(2, 1), (1, 3), (3, 4), (4, 0), (0, 2)]
>>> list(adjacent_pairs(boxes[6])) # Seventh
[(2, 3), (3, 1), (1, 4), (4, 0), (0, 2)]
>>> list(adjacent_pairs(boxes[3])) # Forth
[(1, 2), (2, 3), (3, 0), (0, 4), (4, 1)]
```

Since for the first three boxes, `t` was encrypted to the same letter (`L`), it means that the rows for `t` and `L` were adjacent in all of these permutations. The only pair that appears in all three is `(0, 2)`, so we can conclude that `t` is on row `0` and `L` is on row `2`. We can also immediately learn from this that `X` is on the forth row, since now `(0, 4)` is the only pair that can match `t`'s placement for the forth box.

We can continue to deduce row placements this way. For example, let's look at `i`. `i` got encrypted to `P` for the second and forth boxes, and to `T` for the third box:

```python
>>> list(adjacent_pairs(boxes[1]))
[(1, 0), (0, 2), (2, 3), (3, 4), (4, 1)]
>>> list(adjacent_pairs(boxes[3]))
[(1, 2), (2, 3), (3, 0), (0, 4), (4, 1)]
>>> list(adjacent_pairs(boxes[2]))
[(1, 2), (2, 0), (0, 3), (3, 4), (4, 1)]
```

The common pair for the second and forth boxes are: `(2, 3), (4, 1)`. But we already know that `t` is on the first row, so based on the pairs of the third box we can select `(2, 0)` for `i -> T`, meaning `i` is on the third row and therefore the correct pair is `(2, 3)` (placing `P` on the forth row).

We can continue this way making placements given that:

 * If a character `x` gets encrypted to a character `Y` for multiple columns(/boxes), we can rule out any pair that doesn't exist in all these columns
 * If a character `x` get encrypted to `Y` for some columns and to `Z` for other columns, we can rule out any pairs which are common to all these columns
 * If a placement in a pair contradicts an already known placement, we can rule it out


It's actually not that hard to solve the challenge manually, given these constraints. Kind of like solving a Sudoku. For the sake of the writeup we'll use the following script though:

```python
import string
from pprint import pprint
from collections import defaultdict, namedtuple

text = """
HSVNN PBLMS XATWW PEBXT CRRCB MULAA LCDLO MGRKI
PALAC VXMUE CSWIK GVLQZ DALRC AACQT BZMYA EVSME
SIXDV CUWMS BLVSR BXGPQ FATUM MSAQV YMVKX QERVB
FLTSR ATSKE ERQBB XTE
""".strip()

RowPair = namedtuple("RowPair", "from_row_num to_row_num")

boxes = [
    [0, 1, 2, 3, 4],
    [1, 0, 2, 3, 4],
    [1, 2, 0, 3, 4],
    [1, 2, 3, 0, 4],
    [1, 2, 3, 4, 0],
    [2, 1, 3, 4, 0],
    [2, 3, 1, 4, 0],
    [2, 3, 4, 1, 0]
]

def adjacent_pairs(lst):
    """
    Returns tuples of adjacent pairs in the list, with wraparound for the last row:
    [1, 2, 3, 0, 4] -> [(1, 2), (2, 3), (3, 0), (0, 4), (4, 1)]
    """
    length = len(lst)
    for i in range(length):
        yield (lst[i], lst[(i + 1) % length])

# Divide the text into columns since each column is encrypted with a different box
text_col = [[] for _ in range(len(boxes))]
for i, chunk in enumerate(text.split()):
    text_col[i % len(boxes)].append(chunk)

# Based on the crib:
replacements = [
    {'f': 'H', 'r': 'S', 'o': 'V', 'm': 'N'}, 
    {'i': 'P', 's': 'B', 't': 'L', 'e': 'M', 'r': 'S'}, 
    {'p': 'X', 'h': 'A', 'i': 'T', 'l': 'W'}, 
    {'i': 'P', 'p': 'E', 's': 'B', 't': 'X', 'o': 'T'}, 
    {'a': 'C', 'l': 'R', 'g': 'B'}, 
    {'e': 'M', 'n': 'U', 't': 'L', 's': 'A'}, 
    {'t': 'L', 'o': 'C', 'p': 'D'}, 
    {}
]

# Mapping: overall_replacements[plaintext_char][ciphertext_char] = [box indices which encrypt plaintext_char as ciphertext_char]
overall_replacements = {c: defaultdict(list) for c in string.ascii_lowercase}

for i, replacement_dict in enumerate(replacements):
    for from_char, to_char in replacement_dict.items():
        overall_replacements[from_char][to_char].append(i)

print("Overall Replacements: ")
pprint(overall_replacements)

print(f"\nInferring Placements:")

# Mapping: known_placements[character] = row which this character resides in (0-4)
known_placements = {}
prev_placement_len = -1

while prev_placement_len != len(known_placements):
    prev_placement_len = len(known_placements)
    for plaintext_char in string.ascii_lowercase:

        # Mapping: filtered_pairs[ciphertext_char] = {pairs of row_num (from, to)}
        filtered_pairs = {}
        for ciphertext_char, box_indices in overall_replacements[plaintext_char].items():
            ciphertext_char = ciphertext_char.lower()

            # For each box, a set of row_num pairs (from, to) which don't contradict the known_placements
            per_box_pairs = []
            for box_index in box_indices:
                # Set of row_num pairs (from, to) which don't contradict the known_placements
                candidate_pairs = set()

                for pair in adjacent_pairs(boxes[box_index]):
                    row_pair = RowPair(*pair)
                    if row_pair.from_row_num != known_placements.get(plaintext_char, row_pair.from_row_num):
                        continue
                    if row_pair.to_row_num != known_placements.get(ciphertext_char, row_pair.to_row_num):
                        continue
                    candidate_pairs.add(row_pair)

                per_box_pairs.append(candidate_pairs)

            # We filter the list of pairs: If the same plaintext letter gets encrypted to the same ciphertext letter
            #  across multiple boxes, then the (from, to) pair that encrypts them must be common to all these boxes
            filtered_pairs[ciphertext_char] = set.intersection(*per_box_pairs)

        for ciphertext_char, pair_set in filtered_pairs.items():
            if len(pair_set) == 1:
                row_pair = pair_set.pop()
                if plaintext_char not in known_placements or ciphertext_char not in known_placements:
                    print(f"{plaintext_char} -> {row_pair.from_row_num}, {ciphertext_char} -> {row_pair.to_row_num}")
                known_placements[plaintext_char] = row_pair.from_row_num
                known_placements[ciphertext_char.lower()] = row_pair.to_row_num
    
print(f"Found placements for {len(known_placements)} characters.\n")

key = [[] for _ in range(5)]

for c, row_num in known_placements.items():
    key[row_num].append(c.upper())

print("Known master box:")
for row in key:
    print("".join(sorted(row)))

print("\nMissing characters:")
for c in set(string.ascii_lowercase) - set("j"):
    if c not in known_placements:
        print(c.upper(), end="")

print("")

```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/cq_cq_cq]
└─$ python3 solve.py
Overall Replacements:
{'a': defaultdict(<class 'list'>, {'C': [4]}),
 'b': defaultdict(<class 'list'>, {}),
 'c': defaultdict(<class 'list'>, {}),
 'd': defaultdict(<class 'list'>, {}),
 'e': defaultdict(<class 'list'>, {'M': [1, 5]}),
 'f': defaultdict(<class 'list'>, {'H': [0]}),
 'g': defaultdict(<class 'list'>, {'B': [4]}),
 'h': defaultdict(<class 'list'>, {'A': [2]}),
 'i': defaultdict(<class 'list'>, {'P': [1, 3], 'T': [2]}),
 'j': defaultdict(<class 'list'>, {}),
 'k': defaultdict(<class 'list'>, {}),
 'l': defaultdict(<class 'list'>, {'W': [2], 'R': [4]}),
 'm': defaultdict(<class 'list'>, {'N': [0]}),
 'n': defaultdict(<class 'list'>, {'U': [5]}),
 'o': defaultdict(<class 'list'>, {'V': [0], 'T': [3], 'C': [6]}),
 'p': defaultdict(<class 'list'>, {'X': [2], 'E': [3], 'D': [6]}),
 'q': defaultdict(<class 'list'>, {}),
 'r': defaultdict(<class 'list'>, {'S': [0, 1]}),
 's': defaultdict(<class 'list'>, {'B': [1, 3], 'A': [5]}),
 't': defaultdict(<class 'list'>, {'L': [1, 5, 6], 'X': [3]}),
 'u': defaultdict(<class 'list'>, {}),
 'v': defaultdict(<class 'list'>, {}),
 'w': defaultdict(<class 'list'>, {}),
 'x': defaultdict(<class 'list'>, {}),
 'y': defaultdict(<class 'list'>, {}),
 'z': defaultdict(<class 'list'>, {})}

Inferring Placements:
t -> 0, l -> 2
i -> 2, t -> 0
l -> 2, w -> 0
l -> 2, r -> 3
o -> 3, t -> 0
r -> 3, s -> 4
s -> 4, b -> 1
s -> 4, a -> 0
t -> 0, x -> 4
a -> 0, c -> 1
g -> 0, b -> 1
h -> 2, a -> 0
i -> 2, p -> 3
o -> 3, v -> 4
p -> 3, e -> 0
p -> 3, d -> 1
e -> 0, m -> 2
f -> 1, h -> 2
m -> 2, n -> 3
n -> 3, u -> 4
Found placements for 21 characters.

Known master box:
AEGTW
BCDF
HILM
NOPR
SUVX

Missing characters:
KQYZ
```

With the help of the script, we almost found our master box. The master box is usually composed of a keyword at the beginning, followed by the rest of the alphabet according to natural order. We need to try dividing the missing characters into those that belong to the keyword and those that don't. This requires some guessing, but it seems safe to assume that we can place `K` and `Q` as following to reach 5 characters in two more rows:

```
AEGTW
BCDF
HIKLM
NOPQR
SUVX
```

Now we have `Y` and `Z` left - one of them belongs to the bottom right corner and one to the keyword. The keyword is some combination of `AEGTW` and `Y/Z`, since we can tell that the remaining alphabet starts from `B`. Playing around with different combinations, it looks like the keyword is `GATEWAY`, which means that the box is:

```
GATEW
YBCDF
HIKLM
NOPQR
SUVXZ
```

Now we can use any Phillips solver, such as [this one](https://www.dcode.fr/phillips-cipher):

```
FROMMISTERPHILLIPSTOALLAGENTSSTOPTHEFLAGISCSAOPENCURLYBRACKETSCLASSICUNDERSCORECRYPTOUNDERSCORESTRIKESUNDERSCOREBACKCLOSECURLYBRACKETSSTOP
```

Adding spaces, we get:

```
FROM MISTER PHILLIPS TO ALL AGENTS STOP THE FLAG IS CSA OPEN CURLY BRACKETS CLASSIC UNDERSCORE CRYPTO UNDERSCORE STRIKES UNDERSCORE BACK CLOSE CURLY BRACKETS STOP
```

And the flag: `CSA{CLASSIC_CRYPTO_STRIKES_BACK}`.