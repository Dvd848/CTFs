# The search for a higher being (2/5)
Category: Innovation

## Description

> You are orbiting mars, is it there where the intelligent life reside?
> 
> https://s3.us-west-2.amazonaws.com/cyber-ctf.be/mars.html
> 

## Solution

We visit the attached website and see the following text:

```
Welcome! I'm captain Russel, We've been waiting for you.

We have detected some weird signals originating from the surface of the planet. We think it might be from a new undiscovered race! Can you find it?
```

By inspecting the source, we find the following hidden `div`:

```html
<div style="display: none;">
ZXh0ZXJuIGNyYXRlIGJhc2U2NDsKdXNlIHN0ZDo6c3RyOwoKLy8gV2hpbGUgZXhwbG9yaW5nIHRoZSBzdXJmYWNlIG9mIHRoZSByZWQgcGxhbmV0LCB5b3Ugc3R1bWJsZSB1cG9uIGFuIGVuY3J5cHRlZCBtZXNzYWdlLCB3aGljaCBtaWdodCBwcm92ZSB0aGVyZSdzIGEgbGlmZSBvbiB0aGlzIHBsYW5ldCEKLy8gQWZ0ZXIgc29tZSBzYW5kIGRpZ2dpbmcsIHlvdSBmb3VuZCBhIHJ1bmUsIGluZGljYXRpbmcgdGhlIGZvbGxvd2luZzoKLy8gQ01PYnc1akRsTU9kdzZKS0s4T1h3NVREb2NPVVNnPT0gfCBhbGllbnM/IGhlcmU/Ci8vIFlvdSBhcmUgdHJ5aW5nIHRvIGJ1aWxkIGEgY3lwaGVyIHJlYWRlciwgYnV0IHNvbWV0aGluZyBpcyBub3QgcXVpdGUgcmlnaHQuCi8vIFdoYXQgY291bGQgdGhpcyBhbGwgbWVhbj8KLy8KCnRyYWl0IElBYnN0cmFjdERlY3J5cHRvciB7CiAgICBmbiBhKCZzZWxmLCBtc2c6IFN0cmluZykgLT4gVmVjPHU4PjsKICAgIGZuIGIoJnNlbGYsIGI6IFZlYzx1OD4pIC0+IFN0cmluZzsKICAgIGZuIGMoJnNlbGYsIGk6IFZlYzx1OD4pIC0+IFZlYzx1OD47CiAgICBmbiBkKCZzZWxmLCBieTogVmVjPHU4PikgLT4gU3RyaW5nOwogICAgZm4gZSgmc2VsZiwgbWVzc2FnZTogU3RyaW5nKSAtPiBTdHJpbmc7Cn0KCnRyYWl0IElCYXNlRGVjcnlwdG9yOiBJQWJzdHJhY3REZWNyeXB0b3IgewogICAgZm4gaG93bF9sb3VkbHkoJnNlbGYpOwp9CgpzdHJ1Y3QgRGVjcnlwdG9yKCk7CgppbXBsIElBYnN0cmFjdERlY3J5cHRvciBmb3IgRGVjcnlwdG9yIHsKICAgIGZuIGEoJnNlbGYsIG1zZzogU3RyaW5nKSAtPiBWZWM8dTg+ewogICAgICAgIHJldHVybiBtc2cuaW50b19ieXRlcygpCgogICAgfQoKICAgIGZuIGMoJnNlbGYsIGk6IFZlYzx1OD4pIC0+IFZlYzx1OD4gewogICAgICAgIHJldHVybiBiYXNlNjQ6OmRlY29kZSgmaSkudW53cmFwKCk7CiAgICB9CgogICAgZm4gZCgmc2VsZiwgYnk6IFZlYzx1OD4pIC0+IFN0cmluZ3sKICAgICAgICBsZXQgYyA9IHNlbGYuYyhieSk7CiAgICAgICAgbGV0IGQgPSBzdHI6OmZyb21fdXRmOCgmYyk7CiAgICAgICAgcmV0dXJuIGQudW53cmFwKCkKICAgIH0KCiAgICBmbiBlKCZzZWxmLCBtZXNzYWdlOiBTdHJpbmcpIC0+IFN0cmluZ3sKICAgICAgICBsZXQgbXV0IHNzID0gU3RyaW5nOjpmcm9tKCIiKTsKICAgICAgICBmb3IgYyBpbiBtZXNzYWdlLmNoYXJzKCkgewogICAgICAgICAgICBsZXQgbXV0IGEgPSBjIGFzIHUzMjsKICAgICAgICAgICAgbGV0IG11dCBiID0gMTsKICAgICAgICAgICAgbGV0IG11dCBjID0gMDsKICAgICAgICAgICAgCiAgICAgICAgICAgIHdoaWxlIGEgPiAwIHsKICAgICAgICAgICAgICAgIGxldCBtdXQgZCA9IGEgJSAxMDsKICAgICAgICAgICAgICAgIGEgLz0gMTA7CiAgICAgICAgICAgICAgICBpZiBkID09IDAgewogICAgICAgICAgICAgICAgICAgIGQgPSAxMDsKICAgICAgICAgICAgICAgIH0KICAgICAgICAgICAgICAgIGMgPSBjICsgKGQrMSkgKiBiOwogICAgICAgICAgICAgICAgYiAqPSAxMDsKICAgICAgICAgICAgfQogICAgICAgICAgICBpZiBjIDwgMTAgewogICAgICAgICAgICAgICAgYyArPSAyOTAKICAgICAgICAgICAgfQoKICAgICAgICAgICAgc3MucHVzaChzdGQ6OmNoYXI6OmZyb21fdTMyKGMpLnVud3JhcCgpKQogICAgICAgIH0KICAgICAgICByZXR1cm4gc3M7CiAgICB9CgogICAgZm4gYigmc2VsZiwgYjogVmVjPHU4PikgLT4gU3RyaW5newogICAgICAgIGxldCBjID0gc2VsZi5kKGIpOwogICAgICAgIHJldHVybiBzZWxmLmUoYyk7CiAgICB9Cn0KCmltcGwgSUJhc2VEZWNyeXB0b3IgZm9yIERlY3J5cHRvciB7CiAgICBmbiBob3dsX2xvdWRseSgmc2VsZikgewogICAgICAgIHByaW50bG4hKCJvbiB2YSBsZSBjaGVyY2hlciB0b3V0ZSBsYSBqb3VybsOpZSEhISEhISEhIik7CiAgICB9Cn0KCmZuIG1haW4oKSB7CiAgICBsZXQgX21zZzpTdHJpbmcgPSBTdHJpbmc6OmZyb20oIllzT1hDTU9qS3dncnc1dkRuc09sdzVURG04T29LOE9UQ01PbyIpOwogICAgbGV0IGRlY3J5cHRvciA9IERlY3J5cHRvcigpOwogICAgbGV0IGEgPSBkZWNyeXB0b3IuYShfbXNnKTsKICAgIGxldCBleHRyYXRlcnJlc3RyaWFsX21zZyA9IGRlY3J5cHRvci5iKGEuY2xvbmUoKSk7Cn0=
</div>
```

We decode it and get:

```rust
extern crate base64;
use std::str;

// While exploring the surface of the red planet, you stumble upon an encrypted message, which might prove there's a life on this planet!
// After some sand digging, you found a rune, indicating the following:
// CMObw5jDlMOdw6JKK8OXw5TDocOUSg== | aliens? here?
// You are trying to build a cypher reader, but something is not quite right.
// What could this all mean?
//

trait IAbstractDecryptor {
    fn a(&self, msg: String) -> Vec<u8>;
    fn b(&self, b: Vec<u8>) -> String;
    fn c(&self, i: Vec<u8>) -> Vec<u8>;
    fn d(&self, by: Vec<u8>) -> String;
    fn e(&self, message: String) -> String;
}

trait IBaseDecryptor: IAbstractDecryptor {
    fn howl_loudly(&self);
}

struct Decryptor();

impl IAbstractDecryptor for Decryptor {
    fn a(&self, msg: String) -> Vec<u8>{
        return msg.into_bytes()

    }

    fn c(&self, i: Vec<u8>) -> Vec<u8> {
        return base64::decode(&i).unwrap();
    }

    fn d(&self, by: Vec<u8>) -> String{
        let c = self.c(by);
        let d = str::from_utf8(&c);
        return d.unwrap()
    }

    fn e(&self, message: String) -> String{
        let mut ss = String::from("");
        for c in message.chars() {
            let mut a = c as u32;
            let mut b = 1;
            let mut c = 0;
            
            while a > 0 {
                let mut d = a % 10;
                a /= 10;
                if d == 0 {
                    d = 10;
                }
                c = c + (d+1) * b;
                b *= 10;
            }
            if c < 10 {
                c += 290
            }

            ss.push(std::char::from_u32(c).unwrap())
        }
        return ss;
    }

    fn b(&self, b: Vec<u8>) -> String{
        let c = self.d(b);
        return self.e(c);
    }
}

impl IBaseDecryptor for Decryptor {
    fn howl_loudly(&self) {
        println!("on va le chercher toute la journée!!!!!!!!");
    }
}

fn main() {
    let _msg:String = String::from("YsOXCMOjKwgrw5vDnsOlw5TDm8OoK8OTCMOo");
    let decryptor = Decryptor();
    let a = decryptor.a(_msg);
    let extraterrestrial_msg = decryptor.b(a.clone());
}
```

A comment precedes the code, and the interesting part is:

```
CMObw5jDlMOdw6JKK8OXw5TDocOUSg== | aliens? here?
```

Let's try to decode the base64 string first and see what we get:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/The_search_for_a_higher_being]
└─$ echo CMObw5jDlMOdw6JKK8OXw5TDocOUSg== | base64 -d
ÛØÔÝâJ+×ÔáÔJ
```

Doesn't really look like much.

Back to the program itself. Trying to run it, we get an error:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/The_search_for_a_higher_being]
└─$ cargo run
    Updating crates.io index
  Downloaded base64 v0.13.0
  Downloaded 1 crate (62.1 KB) in 0.70s
   Compiling base64 v0.13.0
   Compiling Program v0.1.0 (/media/sf_CTFs/cyberark/The_search_for_a_higher_being)
error[E0308]: mismatched types
  --> program.rs:38:16
   |
35 |     fn d(&self, by: Vec<u8>) -> String{
   |                                 ------ expected `String` because of return type
...
38 |         return d.unwrap()
   |                ^^^^^^^^^^
   |                |
   |                expected struct `String`, found `&str`
   |                help: try using a conversion method: `d.unwrap().to_string()`

error: aborting due to previous error

For more information about this error, try `rustc --explain E0308`.
error: could not compile `Program`.

To learn more, run the command again with --verbose.
```

So, we need to change `d.unwrap()` to `d.unwrap().to_string()`, only to get a warning that `extraterrestrial_msg` is unused. After adding `println!("{}", extraterrestrial_msg);`, we finally get the output:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/The_search_for_a_higher_being]
└─$ target/debug/program
mņīŒ6ī6ŊōŔŃŊŗ6łīŗ
```

Well, that doesn't really say much either. We can see that the program is trying to decrypt `"YsOXCMOjKwgrw5vDnsOlw5TDm8OoK8OTCMOo"`, what if we change it to the base64 string we saw in the comment (`"CMObw5jDlMOdw6JKK8OXw5TDocOUSg=="`)?

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/The_search_for_a_higher_being]
└─$ target/debug/program
īŊŇŃŌőU6ņŃŐŃU
```

Indeed, as the comment stated, "*something is not quite right*".

At this point, let's follow the basic flow of the program. It starts with a base64-encoded string, then sends it to `a()` in order to convert it into bytes. The result gets sent to `b()`, which immediately calls `d()` with the input. `d()` decodes the base64 by calling `c()`, then decodes the result as UTF8. Finally, some kind of manipulation is performed on the string by `e()`. Looks like something is incorrect in that manipulation based on the results we're getting.

If we take a look at the bytes of the base64-decoded string, it's obviously unicode:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/The_search_for_a_higher_being]
└─$ echo CMObw5jDlMOdw6JKK8OXw5TDocOUSg== | base64 -d | xxd -g 1
00000000: 08 c3 9b c3 98 c3 94 c3 9d c3 a2 4a 2b c3 97 c3  ...........J+...
00000010: 94 c3 a1 c3 94 4a                                .....J
```

But we can copy the rust program's logic and decode the string as unicode, then use Python's `ord()` to get the code point for each character in a similar manner to what `e()` does by calling `message.chars()` and `c as u32`. Let's do that with the pair we got from the comment:

```python
>>> codepoints = lambda s: [f'{ord(c):02x}' for c in s]
>>> codepoints(base64.b64decode("CMObw5jDlMOdw6JKK8OXw5TDocOUSg==").decode("utf8"))
['08', 'db', 'd8', 'd4', 'dd', 'e2', '4a', '2b', 'd7', 'd4', 'e1', 'd4', '4a']
>>> codepoints("aliens? here?")
['61', '6c', '69', '65', '6e', '73', '3f', '20', '68', '65', '72', '65', '3f']
>>> [f'{c.ljust(2)}' for c in "aliens? here?"]
['a ', 'l ', 'i ', 'e ', 'n ', 's ', '? ', '  ', 'h ', 'e ', 'r ', 'e ', '? ']
```

Comparing the output, it is pretty obvious that this is a simple substitution cipher. We only have a small subset of the key, but is it enough to decipher the other string?

We can map characters from the known ciphertext to their plaintext equivalent:

```python
>>> dict = {cipher_char:plain_char for plain_char, cipher_char in zip("aliens? here?", base64.b64decode("CMObw5jDlMOdw6JKK8OXw5TDocOUSg==").decode("utf8"))}
>>> dict
{'\x08': 'a', 'Û': 'l', 'Ø': 'i', 'Ô': 'e', 'Ý': 'n', 'â': 's', 'J': '?', '+': ' ', '×': 'h', 'á': 'r'}
```

Then use that to try and decode the other ciphertext:

```python
>>> "".join([dict.get(c, "?") for c in base64.b64decode("YsOXCMOjKwgrw5vDnsOlw5TDm8OoK8OTCMOo").decode("utf8")])
'?ha? a l??el? ?a?'
```

That's not bad. To complete the picture, let's take a look at the codepoints themselves:

```python
>>> codepoints(base64.b64decode("YsOXCMOjKwgrw5vDnsOlw5TDm8OoK8OTCMOo").decode("utf8"))
['62', 'd7', '08', 'e3', '2b', '08', '2b', 'db', 'de', 'e5', 'd4', 'db', 'e8', '2b', 'd3', '08', 'e8']
```

Using a [Crossword Quick Solve](https://www.crosswordsolver.org/quicksolve.php) service we can enter the letters we already have and get a list of potential matches. For example, the choices that make sense for the third word are:

```
lifely
likely
lively
lonely
lovely
```

And for the forth word:

```
day
way
```

The answer that was accepted was: `What a lovely day`. Notice how the code-point for the first letter is pretty far away from the others, and that it mapped to a capital letter.

Luckily, we did not have to understand `e()`'s implementation at all. In retrospective, when comparing the code-point of the ciphertext to that of the plaintext, the following pattern is revealed:

```
(a ->)   | (97  -> 8)   |  diff: -89
(l -> Û) | (108 -> 219) |  diff: 111
(i -> Ø) | (105 -> 216) |  diff: 111
(e -> Ô) | (101 -> 212) |  diff: 111
(n -> Ý) | (110 -> 221) |  diff: 111
(s -> â) | (115 -> 226) |  diff: 111
(? -> J) | (63  -> 74)  |  diff: 11
(  -> +) | (32  -> 43)  |  diff: 11
(h -> ×) | (104 -> 215) |  diff: 111
(e -> Ô) | (101 -> 212) |  diff: 111
(r -> á) | (114 -> 225) |  diff: 111
(e -> Ô) | (101 -> 212) |  diff: 111
(? -> J) | (63  -> 74)  |  diff: 11

(W -> b) | (87  -> 98)  |  diff: 11
(h -> ×) | (104 -> 215) |  diff: 111
(a ->)   | (97  -> 8)   |  diff: -89
(t -> ã) | (116 -> 227) |  diff: 111
(  -> +) | (32  -> 43)  |  diff: 11
(a ->)   | (97  -> 8)   |  diff: -89
(  -> +) | (32  -> 43)  |  diff: 11
(l -> Û) | (108 -> 219) |  diff: 111
(o -> Þ) | (111 -> 222) |  diff: 111
(v -> å) | (118 -> 229) |  diff: 111
(e -> Ô) | (101 -> 212) |  diff: 111
(l -> Û) | (108 -> 219) |  diff: 111
(y -> è) | (121 -> 232) |  diff: 111
(  -> +) | (32  -> 43)  |  diff: 11
(d -> Ó) | (100 -> 211) |  diff: 111
(a ->)   | (97  -> 8)   |  diff: -89
(y -> è) | (121 -> 232) |  diff: 111
```