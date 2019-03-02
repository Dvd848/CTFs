# Warm Up
Crypto, 50 points

## Description:

> Everyone says that PGP is hard to use. Show ‘em how it’s done.


## Solution:

The following files were provided:
```console
# ls
flag.html.enc  key.enc  mitre-ctf-2019-private.asc passphrase.txt
root@kali:/media/sf_CTFs/mitre/warmup# cat passphrase.txt
just use ctfd
```

The description says PGP, so let's use that to try and open the key.

We first need to import the key (the passphrase was supplied in the text file):
```console
root@kali:/media/sf_CTFs/mitre/warmup# gpg --list-keys
root@kali:/media/sf_CTFs/mitre/warmup# gpg --import mitre-ctf-2019-private.asc
┌────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐
│ Please enter the passphrase to import the OpenPGP secret key:                                                  │
│ "CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>"  │
│ 2048-bit RSA key, ID D70E64BECB374E23,                                                                         │
│ created 2018-12-03.                                                                                            │
│                                                                                                                │
│                                                                                                                │
│ Passphrase: *************_____________________________________________________________________________________ │
│                                                                                                                │
│                 <OK>                                                                    <Cancel>               │
└────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘
gpg: key D70E64BECB374E23: public key "CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>" imported
gpg: key D70E64BECB374E23: secret key imported
gpg: Total number processed: 1
gpg:               imported: 1
gpg:       secret keys read: 1
gpg:   secret keys imported: 1
root@kali:/media/sf_CTFs/mitre/warmup# gpg --list-keys
/root/.gnupg/pubring.kbx
------------------------
pub   rsa2048 2018-12-03 [SC]
      006812F0E6E68EC18A4868C3D70E64BECB374E23
uid           [ unknown] CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>
sub   rsa2048 2018-12-03 [E]

```

Now we decrypt the key:
```
root@kali:/media/sf_CTFs/mitre/warmup# gpg -o key.out --decrypt key.enc
gpg: encrypted with 2048-bit RSA key, ID 2D312D1F87BA2B5E, created 2018-12-03
      "CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>"
gpg: Signature made Tue 04 Dec 2018 00:48:09 IST
gpg:                using RSA key 587735E31F0B06751ACD0D53CDE38825F2FFFCB4
gpg: Can't check signature: No public key
```

We delete the keys so that we never accidentally use them after the CTF:
```console
root@kali:/media/sf_CTFs/mitre/warmup# gpg --delete-secret-key 006812F0E6E68EC18A4868C3D70E64BECB374E23
gpg (GnuPG) 2.2.12; Copyright (C) 2018 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.


sec  rsa2048/D70E64BECB374E23 2018-12-03 CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>

Delete this key from the keyring? (y/N) y
This is a secret key! - really delete? (y/N) y
root@kali:/media/sf_CTFs/mitre/warmup# gpg --delete-key 006812F0E6E68EC18A4868C3D70E64BECB374E23
gpg (GnuPG) 2.2.12; Copyright (C) 2018 Free Software Foundation, Inc.
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.


pub  rsa2048/D70E64BECB374E23 2018-12-03 CTF Competitor (This is private key for a 2019 MITRE CTF Competitor and should not be trusted!) <fake@fake>

Delete this key from the keyring? (y/N) y
```

Let's take a look at `flag.html.enc`:

```console
root@kali:/media/sf_CTFs/mitre/warmup# file flag.html.enc
flag.html.enc: openssl enc'd data with salted password
```

So this file is encrypted with OpenSSL.

After many-many trials for what should theoretically be straightforward, this is the command that decrypted the file:
```
root@kali:/media/sf_CTFs/mitre/warmup# openssl enc -d -in flag.html.enc -out flag.html -pass file:key.out -aes-256-cbc -md md5
root@kali:/media/sf_CTFs/mitre/warmup# cat flag.html
<!DOCTYPE html>
<html>
<head>
  <title>MITRE CTF 2019 Homepage.</title>
</head>
<body>
<h1>This is an HTML Page</h1>
<br>
<p>Test Flag please ignore:</p>
<p>MCA{0p3n55l_c0mm4nd_l1ne_ch4ll3ng3_fl4g}</p>
<p style="display:none;">MCA{66b2f50cd2d6b9622c6be902ee2b0976badb4684}</p>
</body>
</html>
```

I'm not sure if this was possible without "brute-forcing" the algorithm and hash type.

The flag: MCA{66b2f50cd2d6b9622c6be902ee2b0976badb4684}
