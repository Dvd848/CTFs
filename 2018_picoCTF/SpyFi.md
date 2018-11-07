# SpyFi
Cryptography, 300 points

## Description:
> James Brahm, James Bond's less-franchised cousin, has left his secure communication with HQ running, but we couldn't find a way to steal his agent identification code. Can you?

```python
#!/usr/bin/python2 -u
from Crypto.Cipher import AES

agent_code = """flag"""

def pad(message):
    if len(message) % 16 != 0:
        message = message + '0'*(16 - len(message)%16 )
    return message

def encrypt(key, plain):
    cipher = AES.new( key.decode('hex'), AES.MODE_ECB )
    return cipher.encrypt(plain).encode('hex')

welcome = "Welcome, Agent 006!"
print welcome

sitrep = raw_input("Please enter your situation report: ")
message = """Agent,
Greetings. My situation report is as follows:
{0}
My agent identifying code is: {1}.
Down with the Soviets,
006
""".format( sitrep, agent_code )

message = pad(message)
print encrypt( """key""", message )

```

## Solution:

The script uses ECB mode to encrypt, which basically says that the plaintext is divided to blocks, and each block is encrypted independently. In other words, if two blocks contain the same plaintext, the outcome of the encryption will be identical. This makes it very easy to recover some or all of the plaintext in case we have the ability to perform a "chosen-plaintext attack" (CPA) - the ability to control part the plaintext and view the corresponding ciphertext.

In our case, we control the situation report, and will use that in order to recover the flag. 
Our strategy will be to start with a report text that will:
* Align the first letter of the flag at the last index of some 16-byte AES-ECB block, while the first 15 bytes of the block are known to us
* Create a block which starts with the same known 15-bytes, and iterate over the last byte until we receive an encrypted block which is identical to the other block

Once we've achieved this, we in fact know the first letter of the flag. We can then shift everything left, and use the same strategy to recover the second letter. We continue until we've recovered the complete flag.

Let's see an example.
The following is the message (I've replaced newlines with # to be able to view the whole message in one line) if we send a specially crafted report of "**************************ifying code is: !":
```
Agent,#Greetings. My situation report is as follows:#**************************ifying code is: !@@@@@@@@@@@@@@@@#My agent identifying code is: dummy_flag.#Down with the Soviets,#006
0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF
0...............1...............2...............3...............4...............5...............6...............7...............8...............9...............10..............11..............
```

The third line resembles block indices, and the second line resembles indices within the block. Also, we're currently using "dummy_flag" as a placeholder for the unknown flag.

Let's compare blocks 5 and 8:
```
fying code is: !
0123456789ABCDEF
5...............

fying code is: d
0123456789ABCDEF
8...............
```

Block 5 is under our full control, while for block 8 we can control the alignment but not the text. We've aligned block 8 so that the first letter of the flag is the last letter of the block (currently represented by "d", but we don't know yet what it really is). We've also created a "twin" block (block 5) where the first 15 letters of the block are identical to the ones in block 8. Now, we just need to iterate through all possible letters as the last letter of block 5, encrypt the message, and inspect the result. When we see that the outcome of block 5 and 8 is identical, we know that we found the first letter of the flag.

Let's see what happens when we try "p" as the first letter:

```console
root@kali:/media/sf_CTFs/pico/SpyFi# attempt="AAAAAAAAAAAAAAAAAAAAAAAAAAifying code is: pBBBBBBBBBBBBBBBB"
root@kali:/media/sf_CTFs/pico/SpyFi# echo "$attempt" | nc 2018shell3.picoctf.com 33893 | sed -r "s/([0-9a-f]{32})/\n\1/g" | cat -n
     1  Welcome, Agent 006!
     2  Please enter your situation report:
     3  54935ab8cba38e08ac1eb299f30694e6
     4  5b6dbb2f7eb37d79b84199dd2653edf4
     5  ca05b4040d721c2e72dfdd629f945832
     6  0e91d44f3a98c7b9b7941868ef1c49d7
     7  1e242c61c75ea0ed3c276692af97c218
     8  5780a0d3b6dd5aba3582217748bb77d7
     9  cc7f01716c208b1ba3341afbc296f2c6
    10  b8a5465062f0d1ce3ef656e45f822ac4
    11  5780a0d3b6dd5aba3582217748bb77d7
    12  3550f316ebfe0bca6046b0643a05f6de
    13  d52449bf46d083b6faf0eecc4e48e1f1
    14  91b65bed8e1bc70928f855b6a5f5edab
    15  4145e1549b998dd96194a8581cf0d9fb
    16  285cf34da7b32a9bd4607ab86f73a3d0
```

Notice how lines 8 and 11 are identical - this means that "p" is indeed the first letter of the flag.

Now we can move on the the seconds letter, e.g.:
```
Agent,#Greetings. My situation report is as follows:#*************************ifying code is: p!@@@@@@@@@@@@@@@#My agent identifying code is: dummy_flag.#Down with the Soviets,#006
0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF0123456789ABCDEF
0...............1...............2...............3...............4...............5...............6...............7...............8...............9...............10..............11..............
```

We continue doing this until we recover the whole block, then start again with the next block. Note that we keep using block 5 as the controlled block, but now we move to comparing against block 9.

The script:
```python
from pwn import *
import sys

AES_BLOCK_SIZE = 16

class SpyFi(object):
    ALIGNMENT_BLOCK_SIZE = 32 # Used for debug
    NUM_BLOCKS_PER_ROW = 2 # Used for debug
    DEBUG = True

    def __init__(self, get_process, message):
        if not "{user_controlled}" in message or not "{secret}" in message:
            raise ValueError("Expecting message with '{user_controlled}' and '{secret}' placeholders")
        self.get_process = get_process
        self.flag_parts = []
        self.message = message
        self.calculate_initial_values()
        

    def calculate_initial_values(self):
        left_placeholder = "*" * AES_BLOCK_SIZE
        right_placeholder = "%" * AES_BLOCK_SIZE
        secret_placeholder = "@" * AES_BLOCK_SIZE

        # Phase 1 - get block before secret
        msg = self.message.format(user_controlled = left_placeholder + right_placeholder, secret = secret_placeholder)
        secret_starts_at = msg.find(secret_placeholder)
        block_before_secret = msg[secret_starts_at - AES_BLOCK_SIZE:secret_starts_at]

        log.info("Block before secret: '{}'".format(block_before_secret))

        # Phase 2 - get padding_left alignment
        msg = self.message.format(user_controlled = left_placeholder + block_before_secret + '@' + right_placeholder, secret = secret_placeholder)
        self.padding_left_alignment = AES_BLOCK_SIZE - 1 - (msg.find(secret_placeholder) % AES_BLOCK_SIZE)
        log.info("Padding_left alignment: '{}'".format(self.padding_left_alignment))


        # Phase 3 - verify padding correctness
        pad_left = left_placeholder + ('&' * self.padding_left_alignment) + block_before_secret
        msg = self.message.format(user_controlled = pad_left + '@' + right_placeholder, secret = secret_placeholder)

        secret_starts_at = msg.find(secret_placeholder)
        user_controlled_starts_at = msg.find(left_placeholder)

        index_of_first_guessed_char = user_controlled_starts_at + len(pad_left)
        index_of_first_secret_char = secret_starts_at

        log.info("index_of_first_guessed_char: '{}'".format(index_of_first_guessed_char))
        log.info("index_of_first_secret_char: '{}'".format(index_of_first_secret_char))

        assert( index_of_first_guessed_char % AES_BLOCK_SIZE == (AES_BLOCK_SIZE - 1) )
        assert( index_of_first_secret_char % AES_BLOCK_SIZE == (AES_BLOCK_SIZE - 1) )

        # Phase 4 - get indices for "twin blocks" - one is user controlled and one holds the secret
        self.user_controlled_twin_block_index = index_of_first_guessed_char // AES_BLOCK_SIZE
        self.secret_twin_block_index = index_of_first_secret_char // AES_BLOCK_SIZE

        log.info("user_controlled_twin_block_index: '{}'".format(self.user_controlled_twin_block_index))
        log.info("secret_twin_block_index: '{}'".format(self.secret_twin_block_index))

        # Phase 5 - reset members
        self.reset_members(block_before_secret)


    def print_message(self, msg_to_send):
        if not self.DEBUG:
            return

        msg = self.message.format(user_controlled = msg_to_send, secret = "dummy_flag")
        
        print msg.replace("\n", "#")
        print ("0123456789ABCDEF")*( (len(msg)//AES_BLOCK_SIZE) + 1)
        for i in range( (len(msg)//AES_BLOCK_SIZE) + 1):
            sys.stdout.write("{}".format(i).ljust(AES_BLOCK_SIZE, "."))
        print ""

    def align(self, hex_str):
        hex_str = hex_str.strip()
        res = ""
        for i, x in enumerate(hex_str):
            if i == 0:
                pass
            elif i % (self.ALIGNMENT_BLOCK_SIZE * self.NUM_BLOCKS_PER_ROW) == 0:
                res += "\n"
            elif i % self.ALIGNMENT_BLOCK_SIZE == 0:
                res += (" ")
            res += x
        return res

    def encrypt(self, msg):
        while True:
            try:
                r = self.get_process()
                r.sendlineafter("Please enter your situation report:", msg)
                return r.recvall()
            except (socket.error, socket.gaierror, socket.herror, socket.timeout) as e:
                log.error(str(e))

    def reset_members(self, known_text):
        self.padding_left_len = AES_BLOCK_SIZE + self.padding_left_alignment
        self.padding_right_len = AES_BLOCK_SIZE
        self.partial_flag = ""
        self.known_text = known_text

    def brute_force(self):
        found_flag = False
        while (not found_flag):
            for x in xrange(ord(" "), ord("~")+1):
                flag_guess = self.partial_flag + chr(x)
                msg_to_send = "{}{}{}{}".format("*" * self.padding_left_len, 
                                                self.known_text, 
                                                flag_guess, 
                                                "@" * self.padding_right_len)
                self.print_message(msg_to_send)
                
                out = self.align(self.encrypt(msg_to_send))

                out_arr = out.split()
                
                if out_arr[self.user_controlled_twin_block_index] == out_arr[self.secret_twin_block_index + len(self.flag_parts)]:
                    self.partial_flag = flag_guess
                    self.padding_left_len -= 1
                    self.padding_right_len -= 1
                    log.info("Found match!, partial_flag so far is: {}".format(self.partial_flag))
                    if chr(x) == "}":
                        found_flag = True
                        self.flag_parts.append(self.partial_flag)
                        break #for
                    if (len(self.partial_flag) == AES_BLOCK_SIZE):
                        self.flag_parts.append(self.partial_flag)
                        self.reset_members(self.partial_flag)
                    break #for
            else:
                raise Exception("Can't find flag")
        return  "".join(self.flag_parts)

def get_process():
    r = remote("2018shell3.picoctf.com", 33893)
    return r

MESSAGE = """Agent,\nGreetings. My situation report is as follows:\n{user_controlled}\nMy agent identifying code is: {secret}.\nDown with the Soviets,\n006"""
spyfi = SpyFi(get_process, MESSAGE)

try:
    flag = spyfi.brute_force()
    log.success("Found flag: {}".format(flag))
except Exception as e:
    log.error(str(e))
```

The script runs for a while and requires ~1800 attempts in order to recover the flag.

Filtered output:
```console
root@kali:/media/sf_CTFs/pico/SpyFi# python exploit.txt | grep Found
[*] Found match!, partial_flag so far is: p
[*] Found match!, partial_flag so far is: pi
[*] Found match!, partial_flag so far is: pic
[*] Found match!, partial_flag so far is: pico
[*] Found match!, partial_flag so far is: picoC
[*] Found match!, partial_flag so far is: picoCT
[*] Found match!, partial_flag so far is: picoCTF
[*] Found match!, partial_flag so far is: picoCTF{
[*] Found match!, partial_flag so far is: picoCTF{@
[*] Found match!, partial_flag so far is: picoCTF{@g
[*] Found match!, partial_flag so far is: picoCTF{@g3
[*] Found match!, partial_flag so far is: picoCTF{@g3n
[*] Found match!, partial_flag so far is: picoCTF{@g3nt
[*] Found match!, partial_flag so far is: picoCTF{@g3nt6
[*] Found match!, partial_flag so far is: picoCTF{@g3nt6_
[*] Found match!, partial_flag so far is: picoCTF{@g3nt6_1
[*] Found match!, partial_flag so far is: $
[*] Found match!, partial_flag so far is: $_
[*] Found match!, partial_flag so far is: $_t
[*] Found match!, partial_flag so far is: $_th
[*] Found match!, partial_flag so far is: $_th3
[*] Found match!, partial_flag so far is: $_th3_
[*] Found match!, partial_flag so far is: $_th3_c
[*] Found match!, partial_flag so far is: $_th3_c0
[*] Found match!, partial_flag so far is: $_th3_c00
[*] Found match!, partial_flag so far is: $_th3_c00l
[*] Found match!, partial_flag so far is: $_th3_c00l3
[*] Found match!, partial_flag so far is: $_th3_c00l3$
[*] Found match!, partial_flag so far is: $_th3_c00l3$t
[*] Found match!, partial_flag so far is: $_th3_c00l3$t_
[*] Found match!, partial_flag so far is: $_th3_c00l3$t_9
[*] Found match!, partial_flag so far is: $_th3_c00l3$t_91
[*] Found match!, partial_flag so far is: 2
[*] Found match!, partial_flag so far is: 21
[*] Found match!, partial_flag so far is: 216
[*] Found match!, partial_flag so far is: 2160
[*] Found match!, partial_flag so far is: 21600
[*] Found match!, partial_flag so far is: 21600}
[+] Found flag: picoCTF{@g3nt6_1$_th3_c00l3$t_9121600}
```

The flag: picoCTF{@g3nt6_1$_th3_c00l3$t_9121600}