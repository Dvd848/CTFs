# Marinated Salad
Category: Cryptography

## Description
> Excuse me waiter, P Dvbsk sprl h Jhlzhy Zhshk dpao 
> 
> okarozuvaf{C1tqF3y3_f0dJf_v_Y34F_c1a4uYy3Gfw_wy3MKl}

## Solution

If we run `"P Dvbsk sprl h Jhlzhy Zhshk dpao okarozuvaf{C1tqF3y3_f0dJf_v_Y34F_c1a4uYy3Gfw_wy3MKl}"` through ROT-19, we get:

```console
root@kali:/media/sf_CTFs/technion/Marinated_Salad# export NODE_PATH=$(npm root --quiet -g)
root@kali:/media/sf_CTFs/technion/Marinated_Salad# node
> const chef = require("cyberchef");
undefined
> chef.ROT13("P Dvbsk sprl h Jhlzhy Zhshk dpao okarozuvaf{C1tqF3y3_f0dJf_v_Y34F_c1a4uYy3Gfw_wy3MKl}", [true, true, 19])
I Would like a Caesar Salad with hdtkhsnoty{V1mjY3r3_y0wCy_o_R34Y_v1t4nRr3Zyp_pr3FDe}
```

This means that the two part of the sentence are encrypted with different ciphers.

ROT ciphers are often called "Caesar cipher". From [Wikipedia](https://en.wikipedia.org/wiki/Caesar_cipher):

> The encryption step performed by a Caesar cipher is often incorporated as part of more complex schemes, such as the Vigenère cipher, and still has modern application in the ROT13 system.

Caesar salad are often served with vinaigrette, which sounds like Vigenère. So without doubt the seconds part of the ciphertext must be encoded with a Vigenère cipher.

After some trial an error with Vigenère ciphers, `hdtkhsnoty{V1mjY3r3_y0wCy_o_R34Y_v1t4nRr3Zyp_pr3FDe}` we find the cipher key and decrypted message:

```console
> chef.vigenèreDecode("hdtkhsnoty{V1mjY3r3_y0wCy_o_R34Y_v1t4nRr3Zyp_pr3FDe}", ["flag"])
cstechnion{V1geN3r3_s0rRy_i_M34N_v1n4iGr3Tte_pl3ASe}
```