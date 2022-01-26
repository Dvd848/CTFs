# Moses
Category: Misc., 100 Points

## Description

>  Grab that flag will you?

Two files were attached: `book.txt.` and `moses.txt`.

## Solution

Let's check the attached files:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ cat moses.txt
.... . .-.. .-.. ---   .-- .   --. --- -   - .... .   ..-. --- .-.. .-.. --- .-- .. -. --.   -- . ... ... .- --. .   ..-. .-. --- --     --- -. .   --- ..-.   --- ..- .-.   ... .--. .. . ...   -... ..- -   ... .... .   -.-. --- ..- .-.. -..   -. --- -   ... .--. .- -.-. .   -... . - .-- . . -.   .-.. . - - . .-. ...   .- -. -..   .-- --- .-. -.. ...   .-- .   -.- -. --- .--   ... .... .   .-- .- ...   - .-. -.-- .. -. --.   - ---   ... . -. -..   .-   ... . --.- ..- . -. -.-. .   --- ..-.   .-- --- .-. -.. ...   .- ...   - .... . -.--   .- .--. .--. . .- .-.   .. -.   - .... .. ...   -... --- --- -.-   .... . .-.. .--.   ..- ...   ..-. .. -. -..   - .... .   ... . --.- ..- . -. -.-. .   ..- -. ..-. --- .-. - ..- -. .- - . .-.. .-.. -.--   ... --- -- .   --- ..-.   - --- -. . ...   .-- . .-. .   -. --- -   -.-. .-.. . .- .-.   ... ---   .-- .   .-. . .--. .-.. .- -.-. . -..   - .... . --   .-- .. - ....   -..-   -... . -.-. .- ..- ... .   .-- .   .-- . .-. .   -. --- -   ... ..- .-. .   .. ..-.   .. -   .-- .- ...   .-   -.. --- -   --- .-.   .-   .-.. .. -. .
----------------------------------------
x.xx...x.xxx..-xx-.xxxx.-.-xxx.-.x..x.xxxx..x.xxx.-.-.xx-.-xxx..-.xx.x.x.--x.xxx
----------------------------------------
- .... .   ..-. .-.. .- --.   .. ...   - .... .   ... . --.- ..- . -. -.-. .   ... . .--. .- .-. .- - . -..   -... -.--   ..- -. -.. . .-. ... -.-. --- .-. . ...   .. -. ... .. -.. .   -.-. ..- .-. .-.. -.--   -... .-. .- -.-. -.- . - ...   .- ..-. - . .-.   ..- .--. .--. . .-.   -.-. .- ... .   -.-. ... .-
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ wc -c book.txt
718176 book.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ wc -l book.txt
12257 book.txt

┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ head book.txt

The Project Gutenberg EBook of The Odyssey, by Homer

This eBook is for the use of anyone anywhere in the United States and most
other parts of the world at no cost and with almost no restrictions
whatsoever.  You may copy it, give it away or re-use it under the terms of
the Project Gutenberg License included with this eBook or online at
www.gutenberg.org.  If you are not located in the United States, you'll have
to check the laws of the country where you are located before using this ebook.
```

The first file looks like morse code. The second one is a pretty long file based on The Project Gutenberg EBook of The Odyssey, by Homer.

We can use `CyberChef` to decode the morse:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ node
> const chef = require("/home/user/utils/CyberChef/");
undefined
> let file = new chef.Dish(fs.readFileSync("moses.txt"));
undefined
> file.apply(chef.fromMorseCode);
HELLOWEGOTTHEFOLLOWINGMESSAGEFROMONEOFOURSPIESBUTSHECOULDNOTSPACEBETWEENLETTERSANDWORDSWEKNOWSHEWASTRYINGTOSENDASEQUENCEOFWORDSASTHEYAPPEARINTHISBOOKHELPUSFINDTHESEQUENCEUNFORTUNATELLYSOMEOFTONESWERENOTCLEARSOWEREPLACEDTHEMWITHXBECAUSEWEWERENOTSUREIFITWASADOTORALINE    THEFLAGISTHESEQUENCESEPARATEDBYUNDERSCORESINSIDECURLYBRACKETSAFTERUPPERCASECSA
```

Let's format that:

```
HELLO WE GOT THE FOLLOWING MESSAGE FROM ONE OF OUR SPIES BUT SHE COULD NOT SPACE BETWEEN LETTERS AND WORDS WE KNOW SHE WAS TRYING TO SEND A SEQUENCE OF WORDS AS THEY APPEAR IN THIS BOOK HELP US FIND THE SEQUENCE UNFORTUNATELLY SOME OF TONES WERE NOT CLEAR SO WE REPLACED THEM WITH X BECAUSE WE WERE NOT SURE IF IT WAS A DOT OR A LINE

----------------------------------------
x.xx...x.xxx..-xx-.xxxx.-.-xxx.-.x..x.xxxx..x.xxx.-.-.xx-.-xxx..-.xx.x.x.--x.xxx
----------------------------------------

THE FLAG IS THE SEQUENCE SEPARATED BY UNDERSCORES INSIDE CURLY BRACKETS AFTER UPPER CASE CSA
```


Ok, so we need to process the text, which is easier with Python. We'll implement some basic functions to translate to and from morse:

```python
import re

SPACE_MARKER = '/'
LETTER_SEPARATOR = ' '

MORSE_CODE = {
    ' ': SPACE_MARKER, 
    "'": '.----.', '(': '-.--.-', ')': '-.--.-', ',': '--..--', '-': '-....-', 
    '.': '.-.-.-', '/': '-..-.', '0': '-----', '1': '.----', '2': '..---', '3': '...--', 
    '4': '....-', '5': '.....', '6': '-....', '7': '--...', '8': '---..', '9': '----.', 
    ':': '---...', ';': '-.-.-.', '?': '..--..', 'A': '.-', 'B': '-...', 'C': '-.-.', 
    'D': '-..', 'E': '.', 'F': '..-.', 'G': '--.', 'H': '....', 'I': '..', 'J': '.---', 
    'K': '-.-', 'L': '.-..', 'M': '--', 'N': '-.', 'O': '---', 'P': '.--.', 'Q': '--.-', 
    'R': '.-.', 'S': '...', 'T': '-', 'U': '..-', 'V': '...-', 'W': '.--', 'X': '-..-', 
    'Y': '-.--', 'Z': '--..', '_': '..--.-'
}

MORSE_CODE_REV = {v: k for k, v in MORSE_CODE.items()}

def to_morse_code(plaintext: str, ignore_unknown: bool = False) -> str:
    res = ""
    for c in plaintext.upper():
        try:
            res += MORSE_CODE[c] + LETTER_SEPARATOR
        except KeyError:
            if not ignore_unknown:
                raise

    return res

def from_morse_code(code: str) -> str:
    res = ""
    for c in code.split(LETTER_SEPARATOR):
        try:
            res += MORSE_CODE_REV[c]
        except KeyError:
            if c != '':
                raise
    return res
```

Now, we need to translate the book to morse, and remove the spaces (since the agent did not include spaces):

```python
with open("book.txt", "r", encoding="utf8") as f:
    pattern = "x.xx...x.xxx..-xx-.xxxx.-.-xxx.-.x..x.xxxx..x.xxx.-.-.xx-.-xxx..-.xx.x.x.--x.xxx"
    book = f.read()
    book_morse = to_morse_code(book, ignore_unknown = True)
    book_morse_no_spaces = book_morse.replace(LETTER_SEPARATOR, "").replace(SPACE_MARKER, "")
    print(f"[*] Searching for pattern: '{pattern}'")
    matches = list(re.finditer(pattern.replace(".", "\.").replace("x", "."), book_morse_no_spaces))
    print(f"[*] Found {len(matches)} matches")
    for match_index, match in enumerate(matches):
        print(f"[*] Match #{match_index + 1}: '{match.group(0)}' at index {match.start()}")
```

Notice that we transformed the pattern as received by the agent to a regex pattern by replacing `'.'` (morse dot) with `'\.'` (regex escaped literal dot) and `'x'` (morse "match-all") to `'.'` (regex match-all).

Running the script, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ python3 solve.py
[*] Searching for pattern: 'x.xx...x.xxx..-xx-.xxxx.-.-xxx.-.x..x.xxxx..x.xxx.-.-.xx-.-xxx..-.xx.x.x.--x.xxx'
[*] Found 1 matches
[*] Match #1: '...-...-...-..----......-.---..-.-.....---.....--.-.-..--.----..-......-.---.-..' at index 651773
```

Now we need to somehow align this result to the version we have with spaces, so that we can correctly decode the morse.

We do that by iterating the version that has spacing, but counting only dots and dashes, until we arrive to `651773`. Using the same method we also calculate the end offset of the match:

```python
        index = 0
        num_chars = 0

        no_spaces_range   = [match.start(), match.end()]
        with_spaces_range = [0, 0]

        for i in range(len(with_spaces_range)):
            while num_chars < no_spaces_range[i]:
                char = book_morse[index]
                if char in ['.', '-']:
                    num_chars += 1
                index += 1
            with_spaces_range[i] = index

        match_with_spaces = book_morse[with_spaces_range[0]:with_spaces_range[1]]
        print(f"[*]  Match with spaces: '{match_with_spaces}'")
```

Now we just need to decode from morse back to English and format according to the instructions:

```python
        decoded = from_morse_code(match_with_spaces)
        print(f"[*]  Decoded: '{decoded}'")
        
        print(f"[*]  Flag: 'CSA{{{decoded.strip().replace(' ', '_').lower()}}}'")
```

The output:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Moses]
└─$ python3 solve.py
[*] Searching for pattern: 'x.xx...x.xxx..-xx-.xxxx.-.-xxx.-.x..x.xxxx..x.xxx.-.-.xx-.-xxx..-.xx.x.x.--x.xxx'
[*] Found 1 matches
[*] Match #1: '...-...-...-..----......-.---..-.-.....---.....--.-.-..--.----..-......-.---.-..' at index 651773
[*]  Match with spaces: ' / ... - .. .-.. .-.. / --- - .... . .-. / --- ..-. / - .... . -- / - .... .- - / .- .-. . / --. --- -. . / -... . ..-. --- .-. .'
[*]  Decoded: ' STILL OTHER OF THEM THAT ARE GONE BEFORE'
[*]  Flag: 'CSA{still_other_of_them_that_are_gone_before}'
```