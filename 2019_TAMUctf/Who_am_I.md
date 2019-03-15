# Who am I
Misc, 100 points

## Description:

> What is the A record for tamuctf.com?
> (Not in standard gigem{flag} format)

## Solution:

We can use `host` to determine the A record for the server.

```console
root@kali:/media/sf_CTFs/tamu# host -a tamuctf.com
Trying "tamuctf.com"
;; ->>HEADER<<- opcode: QUERY, status: NOERROR, id: 60722
;; flags: qr rd ra; QUERY: 1, ANSWER: 4, AUTHORITY: 0, ADDITIONAL: 0

;; QUESTION SECTION:
;tamuctf.com.                   IN      ANY

;; ANSWER SECTION:
tamuctf.com.            85865   IN      A       52.33.57.247
tamuctf.com.            85865   IN      NS      c.ns.joker.com.
tamuctf.com.            85865   IN      NS      a.ns.joker.com.
tamuctf.com.            85865   IN      NS      b.ns.joker.com.
```