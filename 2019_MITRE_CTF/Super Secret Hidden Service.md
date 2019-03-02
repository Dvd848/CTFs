# Super Secret Hidden Service
Web, 50 points

## Description:

> https://138.247.13.115


## Solution:

Trying to access the website, we get an error that the certificate provided by the server is not trusted.

If we try to bypass this with `curl -k`, we get:
```
421 Site 138.247.13.115 is not served on this interface
```

Let's inspect the certificate:

```console
root@kali:/media/sf_CTFs/mitre/SuperSecretHiddenService# openssl x509 -in CaddySelfSigned.crt -inform der  -text -noout
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            8c:96:2c:fd:b5:7c:b8:a9:ab:38:8b:bf:07:0a:80:e6
        Signature Algorithm: ecdsa-with-SHA256
        Issuer: O = Caddy Self-Signed
        Validity
            Not Before: Feb 22 17:44:31 2019 GMT
            Not After : Mar  1 17:44:31 2019 GMT
        Subject: O = Caddy Self-Signed
        Subject Public Key Info:
            Public Key Algorithm: id-ecPublicKey
                Public-Key: (256 bit)
                pub:
                    04:1b:56:82:6f:a8:f9:a3:4c:06:a8:1f:da:59:ea:
                    ef:a1:cd:c2:97:af:28:c7:ab:dc:b1:73:15:63:62:
                    be:6c:c3:13:f4:91:6e:7f:ce:e3:6d:19:a0:9b:38:
                    29:19:4e:c9:d8:04:b9:72:22:d4:48:3d:9d:de:32:
                    5c:3b:62:e0:f0
                ASN1 OID: prime256v1
                NIST CURVE: P-256
        X509v3 extensions:
            X509v3 Key Usage: critical
                Digital Signature, Key Encipherment
            X509v3 Extended Key Usage:
                TLS Web Server Authentication
            X509v3 Subject Alternative Name:
                DNS:138.247.13.115.xip.io
    Signature Algorithm: ecdsa-with-SHA256
         30:45:02:21:00:a9:95:71:a7:5e:06:ee:14:9d:1c:90:35:d8:
         b2:15:40:53:5f:2b:d5:20:c5:15:1b:db:12:3d:69:70:6c:4f:
         59:02:20:03:ad:a6:68:d0:7e:a5:e4:86:0b:16:d4:8f:1d:f1:
         5a:64:02:eb:11:13:3e:a1:ce:f5:43:8e:a7:b1:1d:90:bb
```

The certificate is issued to `138.247.13.115.xip.io`. 

We try to access this FQDN and get the flag:
```
root@kali:/media/sf_CTFs/mitre/SuperSecretHiddenService# curl -k https://138.247.13.115.xip.io
Flag is: MCA{shuHeimoowaiF5a}
```