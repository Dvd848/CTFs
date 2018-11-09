# quackme up
Reversing, 350 points

## Description:
> The duck puns continue. Can you crack, I mean quack this program as well?

A binary was attached.

## Solution:

Let's run the supplied binary:
```console
root@kali:/media/sf_CTFs/pico/quackme_up# ./main
We're moving along swimmingly. Is this one too fowl for you?
Enter text to encrypt: test
Here's your ciphertext: 51 40 21 51
Now quack it! : 11 80 20 E0 22 53 72 A1 01 41 55 20 A0 C0 25 E3 35 40 55 30 85 55 70 20 C1
That's all folks.
```

Looks like we will have to crack some ciphertext, but not to much to work with for now. The next step is launching a disassembler:
```assembly
root@kali:/media/sf_CTFs/pico/quackme_up# r2 main
 -- Your endian swaps
[0x08048450]> aa
[x] Analyze all flags starting with sym. and entry0 (aa)
[0x08048450]> afl
0x08048398    3 35           sym._init
0x080483d0    1 6            sym.imp.getline
0x080483e0    1 6            sym.imp.printf
0x080483f0    1 6            sym.imp.__stack_chk_fail
0x08048400    1 6            sym.imp.puts
0x08048410    1 6            sym.imp.strlen
0x08048420    1 6            sym.imp.__libc_start_main
0x08048430    1 6            sym.imp.putchar
0x08048440    1 6            fcn.08048440
0x08048450    1 33           entry0
0x08048480    1 4            sym.__x86.get_pc_thunk.bx
0x08048490    4 43           sym.deregister_tm_clones
0x080484c0    4 53           sym.register_tm_clones
0x08048500    3 30           sym.__do_global_dtors_aux
0x08048520    4 43   -> 40   entry1.init
0x0804854b    6 117          sym.read_input
0x080485c0    1 17           sym.do_magic
0x080485d1    1 39           sym.rol4
0x080485f8    1 39           sym.ror8
0x0804861f    6 96           sym.print_hex
0x0804867f    4 119          sym.encrypt
0x080486f6    1 158          sym.main
0x080487a0    4 93           sym.__libc_csu_init
0x08048800    1 2            sym.__libc_csu_fini
0x08048804    1 20           sym._fini
[0x08048450]> s sym.main
[0x080486f6]> pdf
            ;-- main:
/ (fcn) sym.main 158
|   sym.main (int argc, char **argv, char **envp);
|           ; var int local_10h @ ebp-0x10
|           ; var int local_ch @ ebp-0xc
|           ; var int local_4h @ ebp-0x4
|           ; arg int arg_4h @ esp+0x4
|           ; DATA XREF from entry0 (0x8048467)
|           0x080486f6      8d4c2404       lea ecx, [arg_4h]           ; 4
|           0x080486fa      83e4f0         and esp, 0xfffffff0
|           0x080486fd      ff71fc         push dword [ecx - 4]
|           0x08048700      55             push ebp
|           0x08048701      89e5           mov ebp, esp
|           0x08048703      51             push ecx
|           0x08048704      83ec14         sub esp, 0x14
|           0x08048707      83ec0c         sub esp, 0xc
|           0x0804870a      6884880408     push str.We_re_moving_along_swimmingly._Is_this_one_too_fowl_for_you ; 0x8048884 ; "We're moving along swimmingly. Is this one too fowl for you?"
|           0x0804870f      e8ecfcffff     call sym.imp.puts           ; int puts(const char *s)
|           0x08048714      83c410         add esp, 0x10
|           0x08048717      83ec0c         sub esp, 0xc
|           0x0804871a      68c1880408     push str.Enter_text_to_encrypt: ; 0x80488c1 ; "Enter text to encrypt: "
|           0x0804871f      e8bcfcffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048724      83c410         add esp, 0x10
|           0x08048727      e81ffeffff     call sym.read_input
|           0x0804872c      8945f0         mov dword [local_10h], eax
|           0x0804872f      83ec0c         sub esp, 0xc
|           0x08048732      ff75f0         push dword [local_10h]
|           0x08048735      e845ffffff     call sym.encrypt
|           0x0804873a      83c410         add esp, 0x10
|           0x0804873d      8945f4         mov dword [local_ch], eax
|           0x08048740      83ec0c         sub esp, 0xc
|           0x08048743      68d9880408     push str.Here_s_your_ciphertext: ; 0x80488d9 ; "Here's your ciphertext: "
|           0x08048748      e893fcffff     call sym.imp.printf         ; int printf(const char *format)
|           0x0804874d      83c410         add esp, 0x10
|           0x08048750      83ec08         sub esp, 8
|           0x08048753      ff75f4         push dword [local_ch]
|           0x08048756      ff75f0         push dword [local_10h]
|           0x08048759      e8c1feffff     call sym.print_hex
|           0x0804875e      83c410         add esp, 0x10
|           0x08048761      a130a00408     mov eax, dword obj.encryptedBuffer ; [0x804a030:4]=0x8048820 str.11_80_20_E0_22_53_72_A1_01_41_55_20_A0_C0_25_E3_35_40_55_30_85_55_70_20_C1 ; " \x88\x04\b"
|           0x08048766      83ec08         sub esp, 8
|           0x08048769      50             push eax
|           0x0804876a      68f2880408     push str.Now_quack_it__:__s ; 0x80488f2 ; "Now quack it! : %s\n"
|           0x0804876f      e86cfcffff     call sym.imp.printf         ; int printf(const char *format)
|           0x08048774      83c410         add esp, 0x10
|           0x08048777      83ec0c         sub esp, 0xc
|           0x0804877a      6806890408     push str.That_s_all_folks.  ; 0x8048906 ; "That's all folks."
|           0x0804877f      e87cfcffff     call sym.imp.puts           ; int puts(const char *s)
|           0x08048784      83c410         add esp, 0x10
|           0x08048787      b800000000     mov eax, 0
|           0x0804878c      8b4dfc         mov ecx, dword [local_4h]
|           0x0804878f      c9             leave
|           0x08048790      8d61fc         lea esp, [ecx - 4]
\           0x08048793      c3             ret
[0x080486f6]>
```

The magic seems to happen in `sym.encrypt`, let's inspect it:
```
[0x080486f6]> s sym.encrypt
[0x0804867f]> VV
```

The first block of logic:
```assembly
.----------------------------------------.
| [0x804867f]                            |
| (fcn) sym.encrypt 119                  |
|   sym.encrypt (int arg_8h);            |
| ; var int local_11h @ ebp-0x11         |
| ; var int local_10h @ ebp-0x10         |
| ; var int local_ch @ ebp-0xc           |
| ; arg int arg_8h @ ebp+0x8             |
| ; CALL XREF from sym.main (0x8048735)  |
| push ebp                               |
| mov ebp, esp                           |
| sub esp, 0x18                          |
| sub esp, 0xc                           |
| push dword [arg_8h]                    |
| ; size_t strlen(const char *s)         |
| call sym.imp.strlen;[ga]               |
| add esp, 0x10                          |
| mov dword [local_ch], eax              |
| mov dword [local_10h], 0               |
| jmp 0x80486e9;[gb]                     |
`----------------------------------------'
```

We'll rename local variables as we go by entering command mode with '`:`', e.g.:
```
Press <enter> to return to Visual mode.
:> afvn input arg_8h
:> afvn input_len local_ch
:> afvn i local_10h
```

In this block, we just calculate the input length and initialize some local variable.
We then jump to the [gd] block, which starts a loop:
```
.------------------.
|                | |
|          .------------------------------------------.
|          |  0x80486e9 [gb]                          |
|          | ; CODE XREF from sym.encrypt (0x804869d) |
|          | mov eax, dword [i]                       |
|          | cmp eax, dword [input_len]               |
|          | jl 0x804869f;[gf]                        |
|          `------------------------------------------'
|                t f
|                | |
|    .-----------' |
|    |             '---------------------------.
|    |                                         |
|.------------------------------------.    .---------------------------------.
||  0x804869f [gf]                    |    |  0x80486f1 [gg]                 |
|| mov edx, dword [i]                 |    | mov eax, dword [input_len]      |
|| ; [0x8:4]=-1                       |    | leave                           |
|| ; 8                                |    | ret                             |
|| mov eax, dword [input]             |    `---------------------------------'
|| add eax, edx                       |
|| movzx eax, byte [eax]              |
|| mov byte [local_11h], al           |
|| movsx eax, byte [local_11h]        |
|| sub esp, 0xc                       |
|| push eax                           |
|| call sym.rol4;[gd]                 |
|| add esp, 0x10                      |
|| mov byte [local_11h], al           |
|| xor byte [local_11h], 0x16         |
|| movsx eax, byte [local_11h]        |
|| sub esp, 0xc                       |
|| push eax                           |
|| call sym.ror8;[ge]                 |
|| add esp, 0x10                      |
|| mov byte [local_11h], al           |
|| mov edx, dword [i]                 |
|| ; [0x8:4]=-1                       |
|| ; 8                                |
|| mov eax, dword [input]             |
|| add edx, eax                       |
|| movzx eax, byte [local_11h]        |
|| mov byte [edx], al                 |
|| add dword [i], 1                   |
|`------------------------------------'
|    v
|    |
`----'          
```

The loop is executed for i in 0..input_len. It performs some manipulations on input[i] and stores the result in-place.
After the loop is done, the buffer contains the encrypted text.

A Python implementation of the encryption functions is as follows:
```python
from pwn import *

def encrypt(s):
    res = ""
    for i in range(len(s)):
        c = ord(s[i])
        c = rol(c, 4, word_size = 8)
        c ^= 0x16
        c = ror(c, 8, word_size = 8)
        res += chr(c)
    return res

assert (encrypt("test") == unhex("51 40 21 51".replace(" ", "")))
```

We test the correctness using the encrypted output received from the program.

In order to decrypt an encrypted message, we need to perform the opposite operation for each of the stages.
For XOR, we just XOR again with the same value. However, for ROL we have to ROR and vice versa (as explained [here](https://www.aldeid.com/wiki/Category:Encryption/rol-ror)).

Therefore, the decryption process is as follows:
```python
def decrypt(s):
    res = ""
    for i in range(len(s)):
        c = ord(s[i])
        c = rol(c, 8, word_size = 8)
        c ^= 0x16
        c = ror(c, 4, word_size = 8)
        res += chr(c)
    return res
```

Since that last step in the encryption process is `ror` by 8, the first thing in the decryption process is `rol` by 8.
Then we XOR with 0x16, just like in the encryption process. The last thing needed to decrypt is `ror` by 4, to negate the first step of encryption which was `rol` by 4.

Now, all that's left is to decrypt the buffer provided by the program:
```python
s = "11 80 20 E0 22 53 72 A1 01 41 55 20 A0 C0 25 E3 35 40 55 30 85 55 70 20 C1".replace(" ", "")
log.info("Decrypting {}".format(s))
print decrypt(unhex(s))
```

The output:
```console
[*] Decrypting 118020E0225372A101415520A0C025E33540553085557020C1
picoCTF{qu4ckm3_2e4b94fc}
```

The flag: picoCTF{qu4ckm3_2e4b94fc}
