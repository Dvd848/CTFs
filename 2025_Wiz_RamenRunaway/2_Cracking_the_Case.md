# Cracking the Case

## Description

> Great detective work! You've identified John's username, but his account is password-protected.
> 
> Of course it is - John takes security seriously.
> 
> The corporate audit logs might hold the key. Security teams love their detailed records, and they track everything... including password changes.
> 
> Time to dig deeper into the digital paper trail. Can you find John's password?

## Solution

This is where the `password_audit_log` table likely comes to play.

We'll query the different entries for John using his User ID:

```sql
SELECT * FROM password_audit_log 
WHERE uid = 64994
ORDER BY created_timestamp DESC
```

The result is:

```
uid_null,password_hash_null,created_timestamp_null,change_reason_null,source_ip_null
64994,d1df49745f75a81137379b4bfe468258,1749502800,user_requested,203.45.67.128
64994,7e3e77196dc74c2e333d8741cc4b595a,1749456789,user_requested,203.45.67.128
64994,0c21064fc5afc763bba2bb289bd138bc,1749401234,admin_reset,203.45.67.128
64994,a1b93dddbee0f2d11ac83b700f314e00,1749345678,user_requested,203.45.67.128
64994,08ef49b71bd525d01bc7f8d8fdb32f26,1749234567,admin_reset,203.45.67.128
64994,3ed74e14092710a091346ed31de35c21,1749123456,user_requested,203.45.67.128
64994,717814a5aabff61c77ca566895c6a753,1748956800,admin_reset,203.45.67.128
64994,d4209fafb80263396bd673d6d79d6d1b,1748923400,user_requested,203.45.67.128
64994,24b410b422ef79427fd808259d262277,1741726800,policy_expiry,203.45.67.128
64994,7205a7bd67d03f483aa1c2692b131f07,1733950800,policy_expiry,203.45.67.128

```

The hashes are 32 characters long, which means that the following algorithms are good candidates for their source:

```bash
$ hashid d1df49745f75a81137379b4bfe468258
Analyzing 'd1df49745f75a81137379b4bfe468258'
[+] MD2 
[+] MD5 
[+] MD4 
[+] Double MD5 
[+] LM 
[+] RIPEMD-128 
[+] Haval-128 
[+] Tiger-128 
[+] Skein-256(128) 
[+] Skein-512(128) 
[+] Lotus Notes/Domino 5 
[+] Skype 
[+] Snefru-128 
[+] NTLM 
[+] Domain Cached Credentials 
[+] Domain Cached Credentials 2 
[+] DNSSEC(NSEC3) 
[+] RAdmin v2.x
```

From the list above, `MD5` is the most commonly-known algorithms, so we can start from assuming it was the one used.

We can use a [reverse hash search engine](https://md5.gromweb.com/?md5=d1df49745f75a81137379b4bfe468258) to see if the given hash was indexed by it, revealing its source. 

> The MD5 hash `d1df49745f75a81137379b4bfe468258` was successfully reversed into the string `sakura123`

`sakura123` was accepted as the flag.

> Password cracked! 🔓
> You're almost there. One final challenge to restore the system. 