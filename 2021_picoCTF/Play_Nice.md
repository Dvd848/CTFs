# Play Nice
Category: Crypto, 110 points

## Description

> Not all ancient ciphers were so bad... The flag is not in standard format.

A Python file was attached.

```python
#!/usr/bin/python3 -u
import signal

SQUARE_SIZE = 6


def generate_square(alphabet):
	assert len(alphabet) == pow(SQUARE_SIZE, 2)
	matrix = []
	for i, letter in enumerate(alphabet):
		if i % SQUARE_SIZE == 0:
			row = []
		row.append(letter)
		if i % SQUARE_SIZE == (SQUARE_SIZE - 1):
			matrix.append(row)
	return matrix

def get_index(letter, matrix):
	for row in range(SQUARE_SIZE):
		for col in range(SQUARE_SIZE):
			if matrix[row][col] == letter:
				return (row, col)
	print("letter not found in matrix.")
	exit()

def encrypt_pair(pair, matrix):
	p1 = get_index(pair[0], matrix)
	p2 = get_index(pair[1], matrix)

	if p1[0] == p2[0]:
		return matrix[p1[0]][(p1[1] + 1)  % SQUARE_SIZE] + matrix[p2[0]][(p2[1] + 1)  % SQUARE_SIZE]
	elif p1[1] == p2[1]:
		return matrix[(p1[0] + 1)  % SQUARE_SIZE][p1[1]] + matrix[(p2[0] + 1)  % SQUARE_SIZE][p2[1]]
	else:
		return matrix[p1[0]][p2[1]] + matrix[p2[0]][p1[1]]

def encrypt_string(s, matrix):
	result = ""
	if len(s) % 2 == 0:
		plain = s
	else:
		plain = s + "meiktp6yh4wxruavj9no13fb8d027c5glzsq"[0]
	for i in range(0, len(plain), 2):
		result += encrypt_pair(plain[i:i + 2], matrix)
	return result

alphabet = open("key").read().rstrip()
m = generate_square(alphabet)
msg = open("msg").read().rstrip()
enc_msg = encrypt_string(msg, m)
print("Here is the alphabet: {}\nHere is the encrypted message: {}".format(alphabet, enc_msg))
signal.alarm(18)
resp = input("What is the plaintext message? ").rstrip()
if resp and resp == msg:
	print("Congratulations! Here's the flag: {}".format(open("flag").read()))

```

## Solution

Let's connect to the attached service:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Play_Nice]
└─$ nc mercury.picoctf.net 6057
Here is the alphabet: meiktp6yh4wxruavj9no13fb8d027c5glzsq
Here is the encrypted message: y7bcvefqecwfste224508y1ufb21ld
What is the plaintext message?
```

This is a [Playfair cipher](https://en.wikipedia.org/wiki/Playfair_cipher), as hinted by the title:

> The Playfair cipher is a manual symmetric encryption technique and was the first literal digram substitution cipher.
> 
> The technique encrypts pairs of letters (bigrams or digrams), instead of single letters as in the simple substitution cipher and rather more complex Vigenère cipher systems then in use.

To decrypt, we form a grid using the alphabet:

```
m e i k t p
6 y h 4 w x
r u a v j 9
n o 1 3 f b
8 d 0 2 7 c
5 g l z s q
```

Then, we work according to the following rules:

 * For each pair of letters:
   * If both letters are on the same row, replace them by the ones on their left. Wraparound to the right if needed.
   * If both letters are on the same column, replace them by the ones above them.  Wraparound to the bottom if needed.
   * Otherwise, locate the two letters which form a rectangle with the current pair's letters. Take the letter that's on the same row as the first ciphertext letter, then the one that's on the second's ciphertext letter.
   * (If after decrypting the message there's an extra `X` or `Q` that doesn't make sense, drop it)

The first pair is `y7`. They aren't on the same row or column, so we form a rectangle with `w` and `d`, then decode as `wd`. Next, we have `bc` which are on the same column. Therefore they become `9b`, and so on.

[This site](https://www.dcode.fr/playfair-cipher) can automatically decode the ciphertext. The plaintext provided by the site is: `wd9bukbspdtj7skd3kl8d6oa3f03g0`.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/Play_Nice]
└─$ nc mercury.picoctf.net 6057
Here is the alphabet: meiktp6yh4wxruavj9no13fb8d027c5glzsq
Here is the encrypted message: y7bcvefqecwfste224508y1ufb21ld
What is the plaintext message? wd9bukbspdtj7skd3kl8d6oa3f03g0
Congratulations! Here's the flag: 2e71b99fd3d07af3808f8dff2652ae0e
```

The flag: `2e71b99fd3d07af3808f8dff2652ae0e`

