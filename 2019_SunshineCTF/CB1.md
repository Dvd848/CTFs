# CB1
Crypto, 50 points

## Description

> We picked up a new numbers station that's been active in the shortwave bands. We need to figure out how to crack his code.
> 
> Here's an example file, crack the cipher and send us the plaintext message.
>
> NOTE: NON-Standard Flag Format

A WAV file was attached.

## Solution

We listen to the file and hear the speaker spell out the following text using [NATO phonetic alphabet](https://en.wikipedia.org/wiki/NATO_phonetic_alphabet):

> hkcgxkznkojkyulsgxin

It's a total overkill, but the following script will try all ROT options and output the best guess for the solution, based on frequency analysis:

```python
from collections import namedtuple
from pprint import pprint
import argparse
import string
import math
import sys

class Rot(object):
    _ROT47_FROM = ord("!")
    _ROT47_TO = ord("~")

    ROT47_LENGTH = _ROT47_TO - _ROT47_FROM + 1
    ROT_LENGTH = len(string.ascii_lowercase)

    @classmethod
    def rot_n(cls, n, input_str):
        translation = str.maketrans(string.ascii_lowercase + string.ascii_uppercase, 
                                    string.ascii_lowercase[n:] + string.ascii_lowercase[:n] 
                                    + string.ascii_uppercase[n:] + string.ascii_uppercase[:n])
        return input_str.translate(translation)

    @classmethod
    def rot47_n(cls, n, input_str):
        x = []
        for i in range(len(input_str)):
            c = ord(input_str[i])
            if cls._ROT47_FROM <= c <= cls._ROT47_TO:
                x.append(chr(cls._ROT47_FROM + ((c - cls._ROT47_FROM + n) % cls.ROT47_LENGTH)))
            else:
                x.append(input_str[i])
        return ''.join(x)

    @classmethod
    def get_algorithms(cls):
        Rot_Algo = namedtuple("Rot_Algo", "method length name")
        return {cls.rot_n: Rot_Algo(cls.rot_n, cls.ROT_LENGTH, "rot_n"), 
                cls.rot47_n: Rot_Algo(cls.rot47_n, cls.ROT47_LENGTH, "rot47_n")}


Candidate = namedtuple('Candidate', 'string score key algorithm')

# Letter frequencies - taken from http://en.wikipedia.org/wiki/Letter_frequency
_LETTER_FREQUENCY = {'E': 12.70, 'T': 9.06, 'A': 8.17, 'O': 7.51, 'I': 6.97, 'N': 6.75, 'S': 6.33, 
                                'H': 6.09, 'R': 5.99, 'D': 4.25, 'L': 4.03, 'C': 2.78, 'U': 2.76, 'M': 2.41, 
                                'W': 2.36, 'F': 2.23, 'G': 2.02, 'Y': 1.97, 'P': 1.93, 'B': 1.29, 'V': 0.98, 
                                'K': 0.77, 'J': 0.15, 'X': 0.15, 'Q': 0.10, 'Z': 0.07}


# A list of common punctuation characters
_COMMON_PUNCTUATION = " ,."

# Default frequency for uncommon characters.
# Used to apply a penalty for these characters.
# Chosen based on estimation.
_FREQUENCY_FOR_UNCOMMON_CHARS = 0.03 

def is_str_printable(input_str):
    """
    Returns True iff the given string consists of printable characters only.
    """
    alphabet = string.printable + '\n'
    return all(x in alphabet for x in input_str)

def get_score_using_chi_square(candidate, use_penalty = False):
    """
    A scoring system based on calculating the Chi Square distance between the given string and the 
    average letter frequencies.
    Optionally, can apply penalty to candidates which have uncommon characters.
    """
    freq = _get_frequency(candidate)
        
    chi_sum = 0
    for c, f in freq.items():
        if c in string.ascii_uppercase:
            chi_sum += float((f - _LETTER_FREQUENCY[c]) ** 2) / _LETTER_FREQUENCY[c]
        elif use_penalty and c not in _COMMON_PUNCTUATION:
            chi_sum += float((f - _FREQUENCY_FOR_UNCOMMON_CHARS) ** 2) / _FREQUENCY_FOR_UNCOMMON_CHARS # Penalty for uncommon punctuation characters
    return chi_sum * -1


def _get_frequency(s):
    """
    Returns a dictionary containing characters in the given string, and their frequencies.
    """
    string = s.upper()
    freq = {}
    string_len = len(string)

    for c in set(string):
        freq[c] = 100.0 * string.count(c) / string_len

    return freq

def print_all(algo, input_str):
    length = Rot.get_algorithms()[algo].length
    for i in range(length):
        candidate = algo(i, input_str)
        if is_str_printable(candidate):
            print ("{:02}: {}".format(i, candidate))

def solve_rot(input_str):
    candidates = []
    for (algo, length, name) in Rot.get_algorithms().values():
        for i in range(length):
            candidate = algo(i, input_str)
            if is_str_printable(candidate):
                score = get_score_using_chi_square(candidate, use_penalty = True)
                candidates.append(Candidate(candidate, score, i, name)) # highest score = best match

    if len(candidates) == 0:
        return None

    candidates.sort(key = lambda x: x.score, reverse = True)
    #pprint(candidates)
    return candidates[0]

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-v", "--verbose", help="Verbose output", action = "store_true")

    input_group = parser.add_mutually_exclusive_group(required = True)
    input_group.add_argument("-s", "--string", help="Input string for ROT algorithm", action = "store")
    input_group.add_argument("-i", "--infile", nargs="?", type=argparse.FileType("r"), const=sys.stdin)

    action_group = parser.add_mutually_exclusive_group(required = True)
    action_group.add_argument("-g", "--guess", help="Try all options and print best candidate", action = "store_true")
    action_group.add_argument("--rot_n", help="Perform ROT_n with a shift of N", action = "store", nargs='?', const="all", metavar="N")
    action_group.add_argument("--rot47_n", help="Perform ROT47_n with a shift of N", action = "store", nargs='?', const="all", metavar="N")
    args = parser.parse_args()

    if args.string:
        in_string = args.string
    else:
        in_string = args.infile.read().rstrip()

    if args.rot_n == "all":
        print_all(Rot.rot_n, in_string)
    elif args.rot_n:
        if not args.rot_n.isdigit():
            parser.error("Error: N must be a positive number")
        print (Rot.rot_n(int(args.rot_n), in_string))
    elif args.rot47_n == "all":
        print_all(Rot.rot47_n, in_string)
    elif args.rot47_n:
        if not args.rot47_n.isdigit():
            parser.error("Error: N must be a positive number")
        print (Rot.rot47_n(int(args.rot47_n), in_string))
    elif args.guess:
        sol = solve_rot(in_string)
        if sol is not None:
            print (sol.string)
            if args.verbose:
                print ("Key: {}".format(sol.key))
                print ("Algorithm: {}".format(sol.algorithm))
        else:
            print ("Error: Could not find solution")

```

Output:

```console
root@kali:/media/sf_CTFs/sunshine/CB1# python3 rot.py -g -s "hkcgxkznkojkyulsgxin"
bewaretheidesofmarch
```

The flag: bewaretheidesofmarch