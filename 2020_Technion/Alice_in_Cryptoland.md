# Alice in Cryptoland
Category: Cryptography

## Description
> Alice Block Cipher (a.k.a ABC) is based on a Feistel cipher design.
> 
> This is a balanced Feistel network, using the following characteristics:
> * Block-size = 8 bytes
> * Key size = any
> * Rounds = 4 (numbered from 1 to 4)
>
> * The Feistel function is defined as (pseudo code):
> ```
> string = concatenation of (key, round_number, R) // the round_number as a char: "1", "2", "3", or "4"
> hash = md5(string)
> return (4 first bytes of the hash)
> ```
> * After the last round, L and R are swapped
> 
> We'll assume that each block is encrypted independently of the others, with the same key, and that the length of the messages is
divisible by the block size (i.e. there is no "partial block").
>
> For example:
> * plaintext: "test_CTF"
> * key: "CryptoIsFun"
> * The first time the Feistel Function is called, the string hashed by MD5 is: "CryptoIsFun1_CTF"
> * ciphertext (encoded in hex): 213d2e66bcf21fb4
> 
> Alice wants to make her ABC cipher better. So she doubled it (and called it ABCD); instead of using it once with one key, she
encrypts the plaintext with a first key, and encrypts the output another time with a second key. The output of the second
encryption is the ciphertext she sends to Bob. She used ABCD to encrypt the attached message, and you know that both keys
are found in the "rockyou.txt" dictionary file. Of course, Alice opens her message with a polite "Dear Bob".
> 
> Crack it.


## Solution

In this challenge we have a custom [Feistel cipher](https://en.wikipedia.org/wiki/Feistel_cipher) called "`ABC`":

> In cryptography, a Feistel cipher (also known as Luby–Rackoff block cipher) is a symmetric structure used in the construction of block ciphers.

An additional cipher called "`ABCD`" is defined using `ABC`: It simply re-encrypts the output of `ABC` again and returns the result as the ciphertext. 

Let's start by implementing both ciphers according to the definition:

```python
import abc
import typing
import hashlib

BLOCK_SIZE = 8

def hex_str(byte_array):
    return "".join("{:02x}".format(c) for c in byte_array)

class FeistelException(Exception):
    pass

class Feistel(object):
    __metaclass__ = abc.ABCMeta
    
    def __init__(self, block_size, num_rounds):
        self.block_size = block_size
        self.num_rounds = num_rounds

    @abc.abstractmethod
    def func(self, key, round_num, R):
        raise NotImplementedError("Please Implement this method")

    @classmethod
    def xor_all(cls, a, b):
        for i in range(len(a)):
            a[i] ^= b[i]

    def block_generator(self, full_input):
        for offset in range(0, len(full_input), self.block_size):
            yield full_input[offset:offset+self.block_size]

    def Encrypt(self, plaintext, key):
        assert(isinstance(plaintext, typing.ByteString))
        res = self.perform_action(plaintext,
                                  key,
                                  range(self.num_rounds))
        return res
 

    def Decrypt(self, ciphertext, key):
        assert(isinstance(ciphertext, typing.ByteString))
        try:
            res = self.perform_action(ciphertext,
                                      key,
                                      range(self.num_rounds - 1, -1, -1))
            return res
        except TypeError:
            raise FeistelException("Error during decryption")

    def perform_action(self, input_arr, key, direction_iter):
        assert(len(input_arr) % self.block_size == 0)
        res = bytearray()
        key_arr = key
        for block_num, block in enumerate(self.block_generator(input_arr)):
            middle = len(block) // 2
            old_L = block[:middle]
            old_R = block[middle:]
            assert(len(old_L) == len(old_R))
            
            for i in direction_iter:
                L = old_R
                R = self.func(key_arr, i, old_R)
                self.xor_all(R, old_L)
                old_L = L
                old_R = R
            res += R + L
        return bytes(res)

class AliceBlockCipher(Feistel):
    def __init__(self):
        super(AliceBlockCipher, self).__init__(block_size = BLOCK_SIZE, num_rounds = 4)

    def func(self, key, round_num, R):
        string = key + str(round_num+1).encode("ascii") + R
        hsh = bytearray(hashlib.md5(string).digest())
        return hsh[:4]

class AliceBlockCipherDouble(object):
    def __init__(self):
        self.abc = AliceBlockCipher()

    def Encrypt(self, plaintext, key1, key2):
        ciphertext1 = self.abc.Encrypt(plaintext, key1)
        ciphertext2 = self.abc.Encrypt(ciphertext1, key2)
        return ciphertext2

    def Decrypt(self, ciphertext, key1, key2):
        plaintext1 = self.abc.Decrypt(ciphertext, key2)
        plaintext2 = self.abc.Decrypt(plaintext1, key1)
        return plaintext2

def test(c, keys, plaintext, expected_hash):
    print ("Starting Test")
    print ("-=-=-=-=-=-=-")
    print ("Encrypting '{}' with '{}'".format(plaintext, keys))
    ciphertext = c.Encrypt(plaintext, *keys)
    print (hex_str(ciphertext))
    assert(hex_str(ciphertext) == expected_hash)

    print ("*--*")

    print ("Decrypting '{}' with '{}'".format(hex_str(ciphertext), keys))
    decrypted_plaintext = c.Decrypt(ciphertext, *keys)
    print (decrypted_plaintext)
    assert(decrypted_plaintext == plaintext)
    print ("End of test\n\n")

def run_tests():
    c = AliceBlockCipher()
    d = AliceBlockCipherDouble()
    
    test(c, [b"CryptoIsFun"], b"test_CTF", "213d2e66bcf21fb4")

    test(c, [b"CryptoIsFun"], b"test_ABC", "0cc69bbfe9ccd791")
    test(c, [b"CryptoIsFun"], b"test_ABC12345678", "0cc69bbfe9ccd791bea946370225ceaf")
    
    test(d, [b"Testing123", b"Testing456"], b"test_ABC", "92a5db564cf83543")
    test(d, [b"Testing123", b"Testing456"], b"test_ABC12345678", "92a5db564cf83543fbcaf14dc0b70b1e")
    
    dec = c.Decrypt(bytearray.fromhex("b1bdad45992c6fe8a7c65a3086c9148f"
                                    + "2c3a0dc929719e2b8f6dcfc12aae5a03"
                                    + "eefd7cb9365e95c07dc9049206f8fed4"),
                   b"SimpleKey")
    assert(dec == b"This is fun so far, but it will get better soon.")
    
    
if __name__ == "__main__":
    run_tests()
```

Now, the problem with the encryption scheme defined for `ABCD` is that it isn't much better than the encryption scheme defined for `ABC`: Both of them require more or less the same magnitude of time to brute-force, if the attacker uses a method called [Meet-in-the-middle](https://en.wikipedia.org/wiki/Meet-in-the-middle_attack):

> When trying to improve the security of a block cipher, a tempting idea is to encrypt the data several times using multiple keys. One might think this doubles or even n-tuples the security of the multiple-encryption scheme, depending on the number of times the data is encrypted, because an exhaustive search on all possible combination of keys (simple brute-force) would take 2n·k attempts if the data is encrypted with k-bit keys n times.
> 
> The MITM is a generic attack which weakens the security benefits of using multiple encryptions by storing intermediate values from the encryptions or decryptions and using those to improve the time required to brute force the decryption keys. This makes a Meet-in-the-Middle attack (MITM) a generic space–time tradeoff cryptographic attack.
> 
> The MITM attack attempts to find the keys by using both the range (ciphertext) and domain (plaintext) of the composition of several functions (or block ciphers) such that the forward mapping through the first functions is the same as the backward mapping (inverse image) through the last functions, quite literally meeting in the middle of the composed function.


So, given that `ABCD` is defined as the follows (where `P` is the plaintext, `C` is the cipertext, and `K1`, `K2` are keys):
```
C = ABCD.Encrypt(K1, K2, P) = ABC.Encrypt(K2, ABC.Encrypt(K1, P))
P = ABCD.Decrypt(K1, K2, P) = ABC.Decrypt(K1, ABC.Decrypt(K2, C))
```

We can apply decryption to both sides of the equation and get:

```
ABC.Decrypt(K2, C) = ABC.Decrypt(K2, ABC.Encrypt(K2, ABC.Encrypt(K1, P))) = ABC.Encrypt(K1, P)
```

This means that we can attack the encryption scheme in two phases, assuming we know enough of the plaintext (in our case - the first block):

 * First we compute all possibilities for `ABC.Encrypt(K1, P)`. That is, we encrypt the plaintext with all possible keys, and then store a mapping between the result and the key.
   * In our case, we just encrypt the first block
 * Then, we compute (up to) all possibilities for `ABC.Decrypt(K2, C)`. That is, we decrypt the ciphertext with each of the keys until we find a result that exists in the mapping we've already computed.
   * In our case, we just decrypt the first block

If we find a match, it's very likely that we were able to recover both keys.

Let's implement this attack:

```python
from abcipher import *
import itertools
import re
import time


def main():
    prefix = b"Dear Bob"
    assert(len(prefix) == BLOCK_SIZE)

    abc = AliceBlockCipher()
    abcd = AliceBlockCipherDouble()

    ciphertext = """
    2b18e4d50348a814bf4468903c3c2da0
    ad9cb544f19134064bd08868de952dbf
    f014780e2c3466b972f2a4b68f326d92
    2b5f1aeb9d0383fa04f94022b2acbe5d
    213c3f97c69f4bdd090bbb1cd35a69e5
    8636720ad269f78afc401d6f55012eda
    da9540e7de5fc13e5f3dd5031c346fc6
    cc36c6bbc1d11ac79921b0af219ed34f
    89f43b04c9e1b2740ca4c7ff3789fcb0
    216f9799f811356e79ab2252cb610c74
    106cc5ff61ab104f9d9e9f24d33361c0
    """

    ciphertext = re.sub(r"\s+", "", ciphertext)
    ciphertext = bytearray.fromhex(ciphertext)
    word_file = "rockyou.txt"
        
    print ("CipherText: {}".format(hex_str(ciphertext)))
    first_block = ciphertext[:BLOCK_SIZE]
    print ("First Block: {}".format(hex_str(first_block)))

    print ("")
    phase1_dict = {}
    with open(word_file, "r", encoding = "utf8") as f:
        print ("Reading Words...")
        words = f.readlines()
        words = list(map(lambda word: word.rstrip().encode("utf8"), words))

        print ("Encrypting all words with key1...")
        for key1 in words:
            enc_with_key1 = abc.Encrypt(prefix, key1)
            phase1_dict[enc_with_key1] = key1

        print ("Decrypting block with all words...")
        for key2 in words:
            phase2_dec = abc.Decrypt(first_block, key2)
            if phase2_dec in phase1_dict:
                key1 = phase1_dict[phase2_dec]
                print ("Found keys: ({}, {}) \n'{}'".format(key1.decode("utf8"),
                                              key2.decode("utf8"),
                                              abcd.Decrypt(ciphertext, key1, key2).decode("ascii")))
                return
                
if __name__ == "__main__":
    start = time.time()
    main()
    end = time.time()
    print ("Execution ended in {} seconds".format(end - start))

```

Output:

```console
root@kali:/media/sf_CTFs/technion/Alice_in_Cryptoland# python3 -O crack_abcd.py
CipherText: 2b18e4d50348a814bf4468903c3c2da0ad9cb544f19134064bd08868de952dbff014780e2c3466b972f2a4b68f326d922b5f1aeb9d0383fa04f94022b2acbe5d213c3f97c69f4bdd090bbb1cd35a69e58636720ad269f78afc401d6f55012edada9540e7de5fc13e5f3dd5031c346fc6cc36c6bbc1d11ac79921b0af219ed34f89f43b04c9e1b2740ca4c7ff3789fcb0216f9799f811356e79ab2252cb610c74106cc5ff61ab104f9d9e9f24d33361c0
First Block: 2b18e4d50348a814

Reading Words...
Encrypting all words with key1...
Decrypting block with all words...
Found keys: (Intel2185, AMISRAELHAI) 
'Dear Bob, did you know that Michael Luby and Charles Rackoff proved that 4 rounds are sufficient to make a Feistel network a cstechnion{str0n9_p53ud0R4ndom_p3rmu7at1on}? Alice.'
Execution ended in 415.5057303905487 seconds
```

The flag: `cstechnion{str0n9_p53ud0R4ndom_p3rmu7at1on}`