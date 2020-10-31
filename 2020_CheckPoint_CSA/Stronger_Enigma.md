# Stronger Enigma
Category: Reversing, 70 Points

## Description

> 
> ```
>          /   /  // --- /   /
>         /   /  // /O/ /   /
>        /   /  // --- /   /
>       /   /  //     /   /
>      |   |  || --- |   |
>      |   |  || |N| |   |
>      |   |  || --- |   |
>      |   |  || --- |   |
>      |---|  || |M| |---|
>      |   |  || --- |   |
>     /|   |  || --- |   |
>    / |   |  || |L| |   |     /
>   /_________________________/
>  |    __________________ | |
>  |   / .-.   .-.   .-.   \ |
>  |  | ( Q ) ( W ) ( E )  | |
>  |  |  "-".-."-".-."-"   | |
>  |O |    ( A ) ( S )     | O|
>  |  |  .-."-".-."-".-.   | |
>  |  | ( Z ) ( X ) ( C )  | |
>  |   \ "-"   "-"   "-"  /  |
>  |    ------------------   |
>  |___  .-. __ .-. __ .-.-._|
>   \   / 1 \  / 2 \  / 3 6 \ \ |
>   |\  \ Q /  \ W /  \ E Y /  \|
>   |--- "-"    "-"    "-"-" ---|
>          |      |      |
>         .-.    .-.    .-.
>        / ( \  / ) \  / ? \
>        \ Z /  \ X /  \ C /
>         "-"    "-"    "-"
> ```
> 
> John Savard
> 
> We caught an undercover agent in our facilities.
> 
> He was transmitting sensitive data to our enemies using some kind of crypto machine.
> 
> We were able to capture the machine but the agent destroyed the encryption configuration he used.
> 
> It's national security matter and we only trust you to solve the mystery - what was his message?
> 
> We were able to track the enemy's other machine in this address: 18.156.68.123:80.
> 
> We COUNT on you on this, don't let us down!
> 
> Good Luck!

A Python script was attached.

```python
import string
import random

def create_configuration():
    number_of_rotors = random.randrange(3, 6)
    rotates_amounts = [3, 5, 7, 11, 17, 19, 23]
    
    result = []
    for _ in range(number_of_rotors):
        rotor = "".join(random.sample(string.ascii_uppercase, 26))
        rotates_amount = random.choice(rotates_amounts)
        result.append([rotor, rotates_amount])
    return result


class StrongerEnigma:

    class Rotor:
        def __init__(self, configuration, rotate_amount):
            self.data = configuration
            self.rotate_amount = rotate_amount

        def rotate(self):
            self.data = self.data[self.rotate_amount:] + self.data[:self.rotate_amount]

        def encrypt(self, char):
            char_idx = string.ascii_uppercase.index(char)
            encrypted = self.data[char_idx]
            self.rotate()
            return encrypted

        def decrypt(self, char):
            char_idx = self.data.index(char)
            decrypted = string.ascii_uppercase[char_idx]
            self.rotate()
            return decrypted

    def __init__(self):
        todays_configuration = create_configuration()

        self.rotors = []
        for d in todays_configuration:
            self.rotors.append(StrongerEnigma.Rotor(d[0], d[1]))

    def __encrypt_char(self, char):
        encrypted = char
        if char in string.ascii_uppercase:
            for r in self.rotors:
                encrypted = r.encrypt(encrypted)
        return encrypted

    def __decrypt_char(self, char):
        decrypted = char
        if char in string.ascii_uppercase:
            for r in reversed(self.rotors):
                decrypted = r.decrypt(decrypted)
        return decrypted

    def encrypt(self, message):
        encrypted_message = ""
        for char in message:
            encrypted_message += self.__encrypt_char(char)
        return encrypted_message

    def decrypt(self, message):
        decrypted_message = ""
        for char in message:
            decrypted_message += self.__decrypt_char(char)
        return decrypted_message


def send_interface(machine):
    to_send = """
HELLO FIELD AGENT!
COMMANDS:
    SEND-SECRET-DATA
    GET-SECRET-DATA
    GOODBYE
    """
    message = machine.encrypt(to_send)
    send(message)
    return message


def process_message(server_machine, message):
    decrypted = server_machine.decrypt(message)

    if decrypted == "GET-SECRET-DATA":
        flag = open("flag.txt", "rt").read()
        encrypted = server_machine.encrypt(flag)
        send(encrypted)
    elif decrypted == "SEND-SECRET-DATA":
        encrypted = receive()
        decrypted = server_machine.decrypt(encrypted)
        open("secrets.txt", "a+").write(decrypted)
    elif decrypted == "GOODBYE":
        send("See you next time")
        exit()
    else:
        send("I don't understand you")


def send(message):
    pass #couldn't extract this part of code from the machine


def receive():
    pass #couldn't extract this part of code from the machine



def doEngima():
    starting_session_seconds = time.time()
    send("Insecure channel. Encrypting with today's configuration..")
    machine = StrongerEnigma()

    while True:
        send_interface(machine)
        client_message = receive()
        process_message(machine, client_message)
```

## Solution

Let's connect to the attached server:

```console
root@kali:/media/sf_CTFs/checkpoint/Stronger_Enigma# nc 18.156.68.123 80
Insecure channel. Encrypting with today's configuration..

ZXMFT DBSBX JSJWK!
IGGYBYFX:
    NYVY-AVKLEX-LYDJ
    SJC-LURSSA-XGYX
    JWBDLON
```

According to the sources, this is the program interface encoded with the current configuration:
```
HELLO FIELD AGENT!
COMMANDS:
    SEND-SECRET-DATA
    GET-SECRET-DATA
    GOODBYE
```


We can press "enter" multiple times and receive the same message encoded with the next configuration:

```console
root@kali:/media/sf_CTFs/checkpoint/Stronger_Enigma# nc 18.156.68.123 80
Insecure channel. Encrypting with today's configuration..

ZXMFT DBSBX JSJWK!
IGGYBYFX:
    NYVY-AVKLEX-LYDJ
    SJC-LURSSA-XGYX
    JWBDLON


I don't understand you

QLVBK QQJTI AWJXA!
PVRYFVYA:
    AZEW-DSPPMR-GVKA
    WJV-UCXVEV-TSKI
    ETJJCDM


I don't understand you

DXFTS KDJAP MKMOV!
FBLUCEWD:
    JUCT-UJOWUM-DRAM
    KMW-ZEQRVA-BOXO
    NKFTQFR


I don't understand you

QPAAX ICMGA LXXYA!
JJEIYCTU:
    PETO-LJPDCM-EXVL
    XXJ-JNSTSG-XJRK
    ASEUPJH


I don't understand you

DSFGH KUXMU PPLPG!
TFAXVTOL:
    CSKX-DMLXEL-DIAP
    PLY-RMBNJC-IAMQ
    FXBFDUY


I don't understand you

RKIML XFLVA BKXMC!
NEAGRKXD:
    XERT-EXCXNV-JOGB
    KXR-PRICJV-PMMP
    AHWYEPZ


I don't understand you

YEMVX VIXFX XLPRV!
VBORXRTE:
    AVBB-YLHSMA-TKCX
    LPU-UHPZMW-ALLB
    ZLTWPGU


I don't understand you

WMZFB VSPAG BOSZW!
DWCLIBBY:
    DSIX-WXNGRM-UQVB
    OSY-NYFGXJ-UPVF
    BXKTCLE


I don't understand you

ZUSAN OUSFF FIKNJ!
KTLEOIXW:
    UJWI-RPWMHR-FPWF
    IKK-AZJCLY-ABAW
    HBSOBNS


I don't understand you

MCYFP EAKIF COECY!
PKEAKWIR:
    LJXP-ESCHYM-YBJC
    OEX-JUTOXR-XXMJ
    INXXTAE
```

Obviously, each time we connect to the server from scratch, the initial configuration is randomly selected.

Let's try to break down how this [Enigma](https://en.wikipedia.org/wiki/Enigma_machine) works.

It first starts by selecting *today's configuration*. This means, creating a random number of *rotors* between 3 and 6, where each rotor has a random amount of rotates (selected from `{3, 5, 7, 11, 17, 19, 23}`).

This Enigma encrypts only ASCII uppercase letters. It does so by encrypting letter after letter, where each letter is encrypted by passing it through all the rotors. 

Let's see an example.
Suppose our initial configuration is:
```python
len(self.rotors)
3
self.rotors[0]
Rotor("ODEIXTCZYNSLBQRMGAVPHFWJUK", 11)
self.rotors[1]
Rotor("OLWYBQEVDKMASIGJFCHNZTXPRU", 3)
self.rotors[2]
Rotor("ULOHBPIVZJDQSNEAGXTMYRKFWC", 19)
```

Now we want to encrypt the letter `H`. For each letter, we run three encryption iterations - one for each rotor. First, we identify `H`'s index in the alphabet: `7`. The result of the first iteration is therefore `rotor.data[7]`: `Z`. Now we rotate the rotor by `11`.

Next, we encrypt `Z` with the second rotor: `U`. We rotate the second rotor by `3` after this. Finally, we encrypt `U` with the third rotor: `Y`, and rotate the third rotor by `19`.

This is how the rotors are now configured after each of them was rotated `self.rotate_amount` times:

```python
self.rotors[0]
Rotor(LBQRMGAVPHFWJUKODEIXTCZYNS, 11)
self.rotors[1]
Rotor(YBQEVDKMASIGJFCHNZTXPRUOLW, 3)
self.rotors[2]
Rotor(MYRKFWCULOHBPIVZJDQSNEAGXT, 19)
```

The encryption of the next letter starts from this configuration.

The key to cracking this encryption is the understanding that each rotor returns to its initial configuration after 26 rounds, no matter what `rotate_amount` is:

```python
>>> import string, random
>>> rotor = "".join(random.sample(string.ascii_uppercase, 26))
>>> initial_rotor = rotor
>>> for rotate_amount in range(26):
...     for i in range(26):
...         rotor = rotor[rotate_amount:] + rotor[:rotate_amount]
...     assert(rotor == initial_rotor)
...
>>>
```

So, after every 26 letters that a rotor encrypts, it returns to the initial configuration. 

Our message to be encrypted, excluding non-ASCII-uppercase letters, is:

```python
>>> import re
>>> PLAINTEXT = """
...             HELLO FIELD AGENT!
...             COMMANDS:
...                 SEND-SECRET-DATA
...                 GET-SECRET-DATA
...                 GOODBYE
...             """
>>> clean_plaintext = re.sub("[^A-Z]", "", PLAINTEXT)
>>> clean_plaintext
'HELLOFIELDAGENTCOMMANDSSENDSECRETDATAGETSECRETDATAGOODBYE'
```

This means that after encrypting `clean_plaintext[:26]` (`'HELLOFIELDAGENTCOMMANDSSEN'`), the configuration is back to the initial one. Only this time, the initial configuration will be used to encrypt `clean_plaintext[26]` which is `D` and not `clean_plaintext[0]` which is `H`.

So we now can observe what both `H` and `D` are encrypted to in the initial configuration.

This, together with the fact that we can keep encrypting the same known message again and again, will allow us to create a mapping between plaintext and encrypted letters for (almost) every machine state. We don't even need to know the number of rotors or the actual initial configuration of each rotor for that.

We'll use the following Python class to wrap the Enigma interface and crack it by using the mechanism described above:

```python
from pwn import *
import string
from collections import namedtuple
import itertools
from pprint import pprint
import re

r = remote("18.156.68.123", 80)


PLAINTEXT = """
            HELLO FIELD AGENT!
            COMMANDS:
                SEND-SECRET-DATA
                GET-SECRET-DATA
                GOODBYE
            """


class EnigmaWrapper:
    def __init__(self, machine, machine_alphabet, interface_plaintext):
        self.machine = machine
        self.machine_alphabet = machine_alphabet
        self.clean_interface_plaintext = self._clean_message(interface_plaintext)
        self.state = 0
        self.encrypted_chars_read_from_machine = 0
        self.encrypted_chars_sent_to_machine   = 0
        self.queue = []
        self.state_to_encryption_map = [dict() for i in range(len(machine_alphabet))]
        self.state_to_decryption_map = [dict() for i in range(len(machine_alphabet))]

        self.machine.recvuntil("Insecure channel. Encrypting with today's configuration..\n")
        self._crack_code()

    def _clean_message(self, message):
        return [c for c in message if c in self.machine_alphabet]

    def _read_encrypted_interface(self):
        output = self.machine.recvuntil("    \n").decode("ascii")
        clean_output = self._clean_message(output)
        assert(len(clean_output) == len(self.clean_interface_plaintext))
        self.encrypted_chars_read_from_machine += len(clean_output)
        self.queue += list(clean_output)
        
    def _get_next_char(self):
        if len(self.queue) == 0:
            if self.encrypted_chars_read_from_machine > 0:
                self.machine.send("\n")
                self.machine.recvuntil("I don't understand you\n")
            self._read_encrypted_interface()
        return self.queue.pop(0)

    def _crack_code(self):
        with log.progress('Cracking code') as p:
            visited_combinations = set()
            for i in itertools.count():
                current_char_offset = i % len(self.clean_interface_plaintext)
                current_machine_state = i % len(self.machine_alphabet)
                state_offset_combination = (current_machine_state, current_char_offset)
                if state_offset_combination in visited_combinations:
                    break

                plain_char = self.clean_interface_plaintext[current_char_offset]
                cipher_char = self._get_next_char()
                p.status("Mapping {}<->{} in machine state {}".format(plain_char, cipher_char, state_offset_combination))
                self.state_to_encryption_map[current_machine_state][plain_char] = cipher_char
                self.state_to_decryption_map[current_machine_state][cipher_char] = plain_char

                visited_combinations.add(state_offset_combination)
            assert(i == len(self.clean_interface_plaintext) * len(self.machine_alphabet))
```

Now we have to send a message to the Enigma: `SEND-SECRET-DATA`. To do that we must encrypt it using the current configuration, which we must keep track of through the `_crack_code` process. We do that by counting the number of letters that were encrypted/decrypted by the machine:

```python
    @property
    def machine_state(self):
        return (self.encrypted_chars_read_from_machine + self.encrypted_chars_sent_to_machine) % len(self.machine_alphabet)

    def send_encrypted_message(self, plaintext_message):
        encrypted_message = ""
        num_encrypted_chars = 0
        initial_state = self.machine_state
        for c in plaintext_message:
            if c in self.machine_alphabet:
                encrypted_message += self.state_to_encryption_map[(initial_state + num_encrypted_chars) % len(self.machine_alphabet)][c]
                num_encrypted_chars += 1
            else:
                encrypted_message += c
        self.machine.sendline(encrypted_message)
        log.info("Encrypting '{}' to '{}'".format(plaintext_message, encrypted_message))
        self.encrypted_chars_sent_to_machine += num_encrypted_chars
        return self.machine.recvuntil("\n", drop = True).decode("ascii")
```

Finally, we'll have to decrypt the message received from the machine in the same way:
```python
    def decrypt_message(self, encrypted_message):
        decrypted_message = ""
        num_decrypted_chars = 0
        for c in encrypted_message:
            if c in self.machine_alphabet:
                decrypted_message += self.state_to_decryption_map[(self.machine_state + num_decrypted_chars) % len(self.machine_alphabet)].get(c, '?')
                num_decrypted_chars += 1
            else:
                decrypted_message += c
        log.info("Decrypted '{}' as '{}'".format(encrypted_message, decrypted_message))
        return decrypted_message
```

What's left is to trigger the flow with:
```python
ew = EnigmaWrapper(r, string.ascii_uppercase, PLAINTEXT)
flag = ew.send_encrypted_message("GET-SECRET-DATA")
print(ew.decrypt_message(flag))
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Stronger_Enigma/enigma# python3 solve.py
[+] Opening connection to 18.156.68.123 on port 80: Done
[+] Cracking code: Done
[*] Encrypting 'GET-SECRET-DATA' to 'JWY-QTWBKP-OWHG'
[*] Decrypted 'DBB{MY_U_HNITJMZ_RT_XGZJQRBO_JK_FK_ALUDPKXQZ_JZ_VUUUUO_NEGK_SG_DAGUODEVXDD}' as 'CSA{IF_A_MACHINE_IS_E??ECTED_TO_BE_INFALLIBE_IT_CANNOT_ALSO_BE_INTELLIGENT}'
CSA{IF_A_MACHINE_IS_E??ECTED_TO_BE_INFALLIBE_IT_CANNOT_ALSO_BE_INTELLIGENT}
```

It looks like we weren't able to decrypt two characters:
```
CSA{IF_A_MACHINE_IS_E??ECTED_TO_BE_INFALLIBE_IT_CANNOT_ALSO_BE_INTELLIGENT}
                     ^^
```

This happened because these two letters where encrypted in a state which we haven't previously visited when encrypted them. Fortunately, we can guess the missing letters.

The flag: `CSA{IF_A_MACHINE_IS_EXPECTED_TO_BE_INFALLIBE_IT_CANNOT_ALSO_BE_INTELLIGENT}`

