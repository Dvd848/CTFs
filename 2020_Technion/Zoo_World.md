# Zoo World
Category: Beginners

## Description
> Passwords have become one of the only layers of protections between hackers and our personal information, we use passwords for almost everything, but some don't use them well.
> 
> Using the same password for multiple services or using a weak and predictable password is one of the leading casues for information theft and account hacking, in this challenge we'll see how easy it is to crack a weak and predictable password.
> 
> For this challenge we'll need 2 things, the first one is a hash cracking tool, the 2 major tools used today are Hashcat and John the Ripper, the second thing is a password list, preferably ordered by most used to least used, for that we're going to use rockyou.txt, which you can find here, rockyou.txt is a list of password created after the 2009 data breach of rockyou that exposed 3.2 million unhashed passwords of its users, this passwords list is used widely today for password cracking and strong passwords verification.
> 
> Now using this tools try to get the flag from this file.

A binary file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/technion/Zoo_World# file my_personal_flag
my_personal_flag: Keepass password database 2.x KDBX
```

We can use `John the Ripper` to crack its password:

```console
oot@kali:/media/sf_CTFs/technion/Zoo_World# keepass2john my_personal_flag > my_personal_flag.hash
root@kali:/media/sf_CTFs/technion/Zoo_World# ~/utils/john/run/john my_personal_flag.hash  --wordlist=/root/utils/dictionaries/rockyou.txt
Using default input encoding: UTF-8
Loaded 1 password hash (KeePass [SHA256 AES 32/64 OpenSSL])
Cost 1 (iteration count) is 60000 for all loaded hashes
Cost 2 (version) is 2 for all loaded hashes
Cost 3 (algorithm [0=AES, 1=TwoFish, 2=ChaCha]) is 0 for all loaded hashes
Warning: OpenMP is disabled; a non-OpenMP build may be faster
Press 'q' or Ctrl-C to abort, almost any other key for status
trouble          (my_personal_flag)
1g 0:00:00:17 DONE (2020-12-12 19:37) 0.05685g/s 54.00p/s 54.00c/s 54.00C/s trouble
Use the "--show" option to display all of the cracked passwords reliably
Session completed
```

The password is `trouble`.

Now we can use [pykeepass](https://pypi.org/project/pykeepass/) to export the file:


```python
>>> from pykeepass import PyKeePass
>>> kp = PyKeePass('my_personal_flag', 'trouble')
>>> kp.dump_xml("out.xml")
```

The flag in in the XML:

```console
root@kali:/media/sf_CTFs/technion/Zoo_World# cat out.xml | grep cstechnion
        <Value>cstechnion{pl3ase_us3_str0ng_pass}</Value>
```