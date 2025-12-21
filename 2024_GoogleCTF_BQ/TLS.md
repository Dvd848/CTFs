# TLS

Category: Crypto

## Description

> Let's see if you can crack this Mutual TLS authentication. Use curl in insecure mode with port 1337.

## Solution

Let's start by trying, as suggested, to connect to the server via `curl` in insecure mode:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ curl -k https://tls.2024-bq.ctfcompetition.com:1337
curl: (56) GnuTLS recv error (-12): A TLS fatal alert has been received.
```

We got an alert, i.e. a failure. We can capture the output with WireShark and view it:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ tshark -r capture1.pcapng
    1 0.000000000    10.0.2.15 → 10.0.2.3     DNS 90 Standard query 0xe2af A tls.2024-bq.ctfcompetition.com
    2 0.000240994    10.0.2.15 → 10.0.2.3     DNS 90 Standard query 0x5da9 AAAA tls.2024-bq.ctfcompetition.com
    3 0.012038664     10.0.2.3 → 10.0.2.15    DNS 106 Standard query response 0xe2af A tls.2024-bq.ctfcompetition.com A 35.205.223.136
    4 0.012038906     10.0.2.3 → 10.0.2.15    DNS 90 Standard query response 0x5da9 AAAA tls.2024-bq.ctfcompetition.com
    5 0.012459683    10.0.2.15 → 35.205.223.136 TCP 74 42062 → 1337 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 SACK_PERM TSval=4081519264 TSecr=0 WS=128
    6 0.071910934 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [SYN, ACK] Seq=0 Ack=1 Win=65535 Len=0 MSS=1460
    7 0.072027165    10.0.2.15 → 35.205.223.136 TCP 54 42062 → 1337 [ACK] Seq=1 Ack=1 Win=64240 Len=0
    8 0.074541111    10.0.2.15 → 35.205.223.136 TLSv1 484 Client Hello (SNI=tls.2024-bq.ctfcompetition.com)
    9 0.082884131 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [ACK] Seq=1 Ack=431 Win=65535 Len=0
   10 0.146425028 35.205.223.136 → 10.0.2.15    TLSv1.3 2934 Server Hello, Change Cipher Spec, Application Data, Application Data, Application Data
   11 0.146466949    10.0.2.15 → 35.205.223.136 TCP 54 42062 → 1337 [ACK] Seq=431 Ack=2881 Win=65535 Len=0
   12 0.146676253    10.0.2.15 → 35.205.223.136 TLSv1.3 60 Change Cipher Spec
   13 0.146904419 35.205.223.136 → 10.0.2.15    TLSv1.3 503 Application Data, Application Data
   14 0.146904501 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [ACK] Seq=3330 Ack=437 Win=65535 Len=0
   15 0.148006266    10.0.2.15 → 35.205.223.136 TLSv1.3 84 Application Data
   16 0.148058925    10.0.2.15 → 35.205.223.136 TLSv1.3 112 Application Data
   17 0.148210099 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [ACK] Seq=3330 Ack=525 Win=65535 Len=0
   18 0.148879849    10.0.2.15 → 35.205.223.136 TLSv1.3 194 Application Data
   19 0.149072471 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [ACK] Seq=3330 Ack=665 Win=65535 Len=0
   20 0.207057539 35.205.223.136 → 10.0.2.15    TLSv1.3 78 Application Data
   21 0.207057635 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [FIN, ACK] Seq=3354 Ack=665 Win=65535 Len=0
   22 0.207228565    10.0.2.15 → 35.205.223.136 TCP 54 42062 → 1337 [FIN, ACK] Seq=665 Ack=3355 Win=65535 Len=0
   23 0.207649744 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 42062 [ACK] Seq=3355 Ack=666 Win=65535 Len=0
```

Since this is a TLS connection, at some point the payload is encrypted and WireShark can't show
us the details. But we can instruct `curl` to save the encryption details to a debug file
and use it when inspecting the network capture:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ export SSLKEYLOGFILE=tls_key_curl_1

┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ curl -k https://tls.2024-bq.ctfcompetition.com:1337
curl: (56) GnuTLS recv error (-12): A TLS fatal alert has been received.

┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ tshark -r capture2.pcapng -o "tls.keylog_file: tls_key_curl_1"
    1 0.000000000    10.0.2.15 → 10.0.2.3     DNS 90 Standard query 0x7587 A tls.2024-bq.ctfcompetition.com
    2 0.000214620    10.0.2.15 → 10.0.2.3     DNS 90 Standard query 0xbd84 AAAA tls.2024-bq.ctfcompetition.com
    3 0.012235227     10.0.2.3 → 10.0.2.15    DNS 106 Standard query response 0x7587 A tls.2024-bq.ctfcompetition.com A 35.205.223.136
    4 0.012235527     10.0.2.3 → 10.0.2.15    DNS 90 Standard query response 0xbd84 AAAA tls.2024-bq.ctfcompetition.com
    5 0.013136133    10.0.2.15 → 35.205.223.136 TCP 74 54998 → 1337 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 SACK_PERM TSval=4082214480 TSecr=0 WS=128
    6 0.072946786 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [SYN, ACK] Seq=0 Ack=1 Win=65535 Len=0 MSS=1460
    7 0.073052114    10.0.2.15 → 35.205.223.136 TCP 54 54998 → 1337 [ACK] Seq=1 Ack=1 Win=64240 Len=0
    8 0.075882547    10.0.2.15 → 35.205.223.136 TLSv1 484 Client Hello (SNI=tls.2024-bq.ctfcompetition.com)
    9 0.076416489 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [ACK] Seq=1 Ack=431 Win=65535 Len=0
   10 0.146673965 35.205.223.136 → 10.0.2.15    TLSv1.3 2934 Server Hello, Change Cipher Spec, Encrypted Extensions, Certificate Request, Certificate
   11 0.146844332    10.0.2.15 → 35.205.223.136 TCP 54 54998 → 1337 [ACK] Seq=431 Ack=2881 Win=65535 Len=0
   12 0.147922624    10.0.2.15 → 35.205.223.136 TLSv1.3 60 Change Cipher Spec
   13 0.149376627 35.205.223.136 → 10.0.2.15    TLSv1.3 503 Certificate Verify, Finished
   14 0.149377153 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [ACK] Seq=3330 Ack=437 Win=65535 Len=0
   15 0.151246942    10.0.2.15 → 35.205.223.136 TLSv1.3 84 Certificate
   16 0.151302739    10.0.2.15 → 35.205.223.136 TLSv1.3 112 Finished
   17 0.154851169 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [ACK] Seq=3330 Ack=525 Win=65535 Len=0
   18 0.159663394    10.0.2.15 → 35.205.223.136 HTTP2 194 Magic, SETTINGS[0], WINDOW_UPDATE[0], HEADERS[1]: GET /
   19 0.160123672 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [ACK] Seq=3330 Ack=665 Win=65535 Len=0
   20 0.210183155 35.205.223.136 → 10.0.2.15    TLSv1.3 78 Alert (Level: Fatal, Description: Certificate Required)
   21 0.210183858 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [FIN, ACK] Seq=3354 Ack=665 Win=65535 Len=0
   22 0.211651815    10.0.2.15 → 35.205.223.136 TCP 54 54998 → 1337 [FIN, ACK] Seq=665 Ack=3355 Win=65535 Len=0
   23 0.222921416 35.205.223.136 → 10.0.2.15    TCP 60 1337 → 54998 [ACK] Seq=3355 Ack=666 Win=65535 Len=0

```

Now we can see the alert: `Alert (Level: Fatal, Description: Certificate Required)`. 
This means that the server has rejected our connection since it is requesting us to send
our own certificate (or certificate chain), which makes sense given the challenge description.

Alternatively, we could have used `openssl`, which probably is a bit more suited for this
challenge despite the advice in the description. We perform the initial connection as follows:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ openssl s_client -showcerts -connect tls.2024-bq.ctfcompetition.com:1337
Connecting to 35.205.223.136
CONNECTED(00000003)
depth=1 C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
verify error:num=19:self-signed certificate in certificate chain
verify return:1
depth=1 C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
verify return:1
depth=0 C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
verify return:1
---
Certificate chain
 0 s:C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
   i:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   a:PKEY: rsaEncryption, 4096 (bit); sigalg: RSA-SHA256
   v:NotBefore: Jul 29 12:25:38 2024 GMT; NotAfter: Jul 27 12:25:38 2034 GMT
-----BEGIN CERTIFICATE-----
MIIE7DCCA9SgAwIBAgIUQnzKKh7pg0oNNfE3orKz1cl8jTEwDQYJKoZIhvcNAQEL
BQAwgZExCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQH
DA1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKDAtHb29nbGUgSW5jLjEMMAoGA1UECwwD
Q1RGMTEwLwYDVQQDDChjYS5jcnlwdG8tdGxzLjIwMjQtYnEuY3RmY29tcGV0aXRp
b24uY29tMB4XDTI0MDcyOTEyMjUzOFoXDTM0MDcyNzEyMjUzOFowgYkxCzAJBgNV
BAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQHDA1Nb3VudGFpbiBW
aWV3MQ8wDQYDVQQKDAZHb29nbGUxDDAKBgNVBAsMA0NURjEuMCwGA1UEAwwlY3J5
cHRvLXRscy4yMDI0LWJxLmN0ZmNvbXBldGl0aW9uLmNvbTCCAiIwDQYJKoZIhvcN
AQEBBQADggIPADCCAgoCggIBALZDqF53koRP+RN2E0bSkqAAKxfdhIR1YAt6d1fw
i8cTVbaxPoSPffDKMXd+slp2CeDBe/RG0N5vpB08JmWOM08PLP8PnUrjK3gF+Z9Y
Bt/Ns6f37IG346tDZP1nNngIu3LT5snTZgFVckuKcUpXlkNwkivbbHjv8+iAWJQw
UnJ0upYAP+4QkUNIK+xxcbcPavjuZjIl94T7BTXdzvSke03uOPjGC7XvPU2M+Jvc
J5RolbtMxXIotAYhqc9i4XhkLONQDM0lsTkxp/guzWAMzOyeFkHusQJTGJITUKW7
hklTI6Wi6k+E3MKAyQ+XVoUAVUxVogr8IxzrI4fWBlxwYtU36TbPGVDIF4OgLFx3
HSAD0fueKkCW3By8TQsk96O3yr+9lahNu2V3W3+wUNn22QAo3ZakTb+XabVMrlZV
lPtF+ywuQAuMjwq0KJZUXwa8vWkw4FhTV/uQOqKF5zP8jV+x2mZSvfOMeBC4mtCB
h5Tk8tNe80XVnCasfx+JahvBJ5/CiWW4lTHDS/Ts7iOIxoemGPNuD8AZAAmvTc77
r8+uTf52zlM58xegm+z+YA06A+2Yprld5WFh/k3JSlkuiWZpKutUbHtiC3iiQ5N5
UmzRQrK/CKzy3ZeHuoySjqhRdOpR4fg64qDUcH59pGsTfzsdrNcWLiLLHPDtKnv5
YGATAgMBAAGjQjBAMB0GA1UdDgQWBBS3FX2t04ch/D8ZhjqRQUPirAUDyjAfBgNV
HSMEGDAWgBQuG1YbgYbWYmZvGzJ67tRgIIjlWjANBgkqhkiG9w0BAQsFAAOCAQEA
PilsT3O7aFoHFNbJhIqAFl99bqQqKLoVjD8RUctfHD//ttG/AWq1l2s/krd/FMQt
5GQdN/dQlDxOjrqkxOSLizHCj4rUZDAjX2I2keVmVDG4h3ZhDOsQ+aDTo3JE3S92
Z4lfUT8zIKiuw+2e5GaYpSgAHkroTHwOsWSA7XGXCS/pzX3Wx/pOdon7jeG4LGTm
TUVC7oiQpXWU8wmgIYhX/9+a1b5sLBiAeCYifRSXwRAPnuomYxcIq4CcqcMe7Sr8
8dRIO0J0VoR6S4wVk5lRYvU9e2U0Vli0cqfsnlYmgaioGG5cweh0U7WeWDGmMmzf
pUgrYXlQ8hs181QH9Hhs5w==
-----END CERTIFICATE-----
 1 s:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   i:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   a:PKEY: rsaEncryption, 2047 (bit); sigalg: RSA-SHA256
   v:NotBefore: Jul 29 12:16:47 2024 GMT; NotAfter: Jul 27 12:16:47 2034 GMT
-----BEGIN CERTIFICATE-----
MIIEBDCCAuygAwIBAgIUfhSPktdXWndrtTz3EZELXX08ZscwDQYJKoZIhvcNAQEL
BQAwgZExCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQH
DA1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKDAtHb29nbGUgSW5jLjEMMAoGA1UECwwD
Q1RGMTEwLwYDVQQDDChjYS5jcnlwdG8tdGxzLjIwMjQtYnEuY3RmY29tcGV0aXRp
b24uY29tMB4XDTI0MDcyOTEyMTY0N1oXDTM0MDcyNzEyMTY0N1owgZExCzAJBgNV
BAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQHDA1Nb3VudGFpbiBW
aWV3MRQwEgYDVQQKDAtHb29nbGUgSW5jLjEMMAoGA1UECwwDQ1RGMTEwLwYDVQQD
DChjYS5jcnlwdG8tdGxzLjIwMjQtYnEuY3RmY29tcGV0aXRpb24uY29tMIIBITAN
BgkqhkiG9w0BAQEFAAOCAQ4AMIIBCQKCAQBAQAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAABAQAAAAgIAAAAAgIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAA
AAAAAAAAAAAAAAAAAAAgAAAAAAAEAAAAAAAAAAAAQAAAAAIAAAAAQMAgAAAAAAAg
AAAAAQAEAAAAUTCAAAAACAAAAAAAAAAAAAAgAAAAAQAAAAAAQAAAAAAAAIAAAAAE
AAAAAAEAAAAAAEAAAAAAAAAAAAAAAAAAAAAAIBCAAAAABAAAAAAAAAAAAAAACEAA
AAAAAQgAIAAAAAAAAAAAAAAAAAAAAACACEAAAAAAAAAAAAAAAAAAAAAhAgMBAAGj
UzBRMB0GA1UdDgQWBBQuG1YbgYbWYmZvGzJ67tRgIIjlWjAfBgNVHSMEGDAWgBQu
G1YbgYbWYmZvGzJ67tRgIIjlWjAPBgNVHRMBAf8EBTADAQH/MA0GCSqGSIb3DQEB
CwUAA4IBAQAZIkugMh25NWNrydMTzCQO5/97/NMJQ5lyx8ydlN4Q+VoUpe8LnJ5E
AFQAttYSoAmgh4lai4ijlJXD30QhDvtKGe35uzhvOPVbYs41KVzcwXZI1sYYwfgb
8mpg2cJ4MZz6uNfQimTnHahIpF/27TznF3ThUo4TpCPGGFCCwwgtc0vR76LNiDpw
3f68AGtcemvmkGwYJ0sDGsw0paF+mJXflRJRyWUM4Kk2M9L9muRQt5VnBGDJyN80
e5s0vtJfkbi99bMhPPKrppgT4h+KKTRDBcybgs8WTpTENpKYaTl3h2TupjwTYabQ
u1Q9dW2N7AdYhknKOPShdeymxuseOPJb
-----END CERTIFICATE-----
---
Server certificate
subject=C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
issuer=C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
---
Acceptable client certificate CA names
C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512:RSA+SHA1:ECDSA+SHA1
Shared Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512
Peer signing digest: SHA256
Peer signature type: RSA-PSS
Server Temp Key: X25519, 253 bits
---
SSL handshake has read 3320 bytes and written 571 bytes
Verification error: self-signed certificate in certificate chain
---
New, TLSv1.3, Cipher is TLS_AES_128_GCM_SHA256
Protocol: TLSv1.3
Server public key is 4096 bit
This TLS version forbids renegotiation.
Compression: NONE
Expansion: NONE
No ALPN negotiated
Early data was not sent
Verify return code: 19 (self-signed certificate in certificate chain)
---
40E77D8CA17F0000:error:0A00045C:SSL routines:ssl3_read_bytes:tlsv13 alert certificate required:../ssl/record/rec_layer_s3.c:908:SSL alert number 116
```

In this case, the error is a bit different - it tells us that there's a self-signed certificate
(that we don't trust) in the chain received by the server. To overcome this without permanently 
trusting this certificate, we can save it as `root.pem` and explicitly provide it as a 
trusted root in subsequent calls:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ openssl s_client -connect tls.2024-bq.ctfcompetition.com:1337 -CAfile ./root.pem
Connecting to 35.205.223.136
CONNECTED(00000003)
depth=1 C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
verify return:1
depth=0 C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
verify return:1
---
Certificate chain
 0 s:C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
   i:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   a:PKEY: rsaEncryption, 4096 (bit); sigalg: RSA-SHA256
   v:NotBefore: Jul 29 12:25:38 2024 GMT; NotAfter: Jul 27 12:25:38 2034 GMT
 1 s:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   i:C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
   a:PKEY: rsaEncryption, 2047 (bit); sigalg: RSA-SHA256
   v:NotBefore: Jul 29 12:16:47 2024 GMT; NotAfter: Jul 27 12:16:47 2034 GMT
---
Server certificate
-----BEGIN CERTIFICATE-----
MIIE7DCCA9SgAwIBAgIUQnzKKh7pg0oNNfE3orKz1cl8jTEwDQYJKoZIhvcNAQEL
BQAwgZExCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQH
DA1Nb3VudGFpbiBWaWV3MRQwEgYDVQQKDAtHb29nbGUgSW5jLjEMMAoGA1UECwwD
Q1RGMTEwLwYDVQQDDChjYS5jcnlwdG8tdGxzLjIwMjQtYnEuY3RmY29tcGV0aXRp
b24uY29tMB4XDTI0MDcyOTEyMjUzOFoXDTM0MDcyNzEyMjUzOFowgYkxCzAJBgNV
BAYTAlVTMRMwEQYDVQQIDApDYWxpZm9ybmlhMRYwFAYDVQQHDA1Nb3VudGFpbiBW
aWV3MQ8wDQYDVQQKDAZHb29nbGUxDDAKBgNVBAsMA0NURjEuMCwGA1UEAwwlY3J5
cHRvLXRscy4yMDI0LWJxLmN0ZmNvbXBldGl0aW9uLmNvbTCCAiIwDQYJKoZIhvcN
AQEBBQADggIPADCCAgoCggIBALZDqF53koRP+RN2E0bSkqAAKxfdhIR1YAt6d1fw
i8cTVbaxPoSPffDKMXd+slp2CeDBe/RG0N5vpB08JmWOM08PLP8PnUrjK3gF+Z9Y
Bt/Ns6f37IG346tDZP1nNngIu3LT5snTZgFVckuKcUpXlkNwkivbbHjv8+iAWJQw
UnJ0upYAP+4QkUNIK+xxcbcPavjuZjIl94T7BTXdzvSke03uOPjGC7XvPU2M+Jvc
J5RolbtMxXIotAYhqc9i4XhkLONQDM0lsTkxp/guzWAMzOyeFkHusQJTGJITUKW7
hklTI6Wi6k+E3MKAyQ+XVoUAVUxVogr8IxzrI4fWBlxwYtU36TbPGVDIF4OgLFx3
HSAD0fueKkCW3By8TQsk96O3yr+9lahNu2V3W3+wUNn22QAo3ZakTb+XabVMrlZV
lPtF+ywuQAuMjwq0KJZUXwa8vWkw4FhTV/uQOqKF5zP8jV+x2mZSvfOMeBC4mtCB
h5Tk8tNe80XVnCasfx+JahvBJ5/CiWW4lTHDS/Ts7iOIxoemGPNuD8AZAAmvTc77
r8+uTf52zlM58xegm+z+YA06A+2Yprld5WFh/k3JSlkuiWZpKutUbHtiC3iiQ5N5
UmzRQrK/CKzy3ZeHuoySjqhRdOpR4fg64qDUcH59pGsTfzsdrNcWLiLLHPDtKnv5
YGATAgMBAAGjQjBAMB0GA1UdDgQWBBS3FX2t04ch/D8ZhjqRQUPirAUDyjAfBgNV
HSMEGDAWgBQuG1YbgYbWYmZvGzJ67tRgIIjlWjANBgkqhkiG9w0BAQsFAAOCAQEA
PilsT3O7aFoHFNbJhIqAFl99bqQqKLoVjD8RUctfHD//ttG/AWq1l2s/krd/FMQt
5GQdN/dQlDxOjrqkxOSLizHCj4rUZDAjX2I2keVmVDG4h3ZhDOsQ+aDTo3JE3S92
Z4lfUT8zIKiuw+2e5GaYpSgAHkroTHwOsWSA7XGXCS/pzX3Wx/pOdon7jeG4LGTm
TUVC7oiQpXWU8wmgIYhX/9+a1b5sLBiAeCYifRSXwRAPnuomYxcIq4CcqcMe7Sr8
8dRIO0J0VoR6S4wVk5lRYvU9e2U0Vli0cqfsnlYmgaioGG5cweh0U7WeWDGmMmzf
pUgrYXlQ8hs181QH9Hhs5w==
-----END CERTIFICATE-----
subject=C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
issuer=C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
---
Acceptable client certificate CA names
C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512:RSA+SHA1:ECDSA+SHA1
Shared Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512
Peer signing digest: SHA256
Peer signature type: RSA-PSS
Server Temp Key: X25519, 253 bits
---
SSL handshake has read 3320 bytes and written 571 bytes
Verification: OK
---
New, TLSv1.3, Cipher is TLS_AES_128_GCM_SHA256
Protocol: TLSv1.3
Server public key is 4096 bit
This TLS version forbids renegotiation.
Compression: NONE
Expansion: NONE
No ALPN negotiated
Early data was not sent
Verify return code: 0 (ok)
---
40F71D11267F0000:error:0A00045C:SSL routines:ssl3_read_bytes:tlsv13 alert certificate required:../ssl/record/rec_layer_s3.c:908:SSL alert number 116
```

Now verification is Ok, we just need to deal with SSL alert number 116, which is "Certificate Required".

The OpenSSL output tells us how the client certificate should look:

```
Acceptable client certificate CA names
C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512:RSA+SHA1:ECDSA+SHA1
Shared Requested Signature Algorithms: RSA-PSS+SHA256:ECDSA+SHA256:ed25519:RSA-PSS+SHA384:RSA-PSS+SHA512:RSA+SHA256:RSA+SHA384:RSA+SHA512:ECDSA+SHA384:ECDSA+SHA512
Peer signing digest: SHA256
Peer signature type: RSA-PSS
Server Temp Key: X25519, 253 bits
```

We can go ahead and create such a client certificate:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl genpkey -algorithm RSA -out ca.key -pkeyopt rsa_keygen_bits:2048
.....+..................+.........+.......+..................+...+..+...+...+....+...+........+++++++++++++++++++++++++++++++++++++++*......+...+....+...+..+.+..+............+...+...+....+........+++++++++++++++++++++++++++++++++++++++*....+.....+...................+..+.........+.+.....................+..+.+..+....+.....+..........+.........+..+.........+.........+.......+...+..+.........+.+.....+......................+..+...+.+......+.....+.........+.......+..+...+...+....+...+...++++++
........+.+........+......+.+.....+...+...+....+++++++++++++++++++++++++++++++++++++++*.+...............+.+........+....+..+.............+..+......+..........+.....+....+++++++++++++++++++++++++++++++++++++++*...+.......+.......................+...+.......+...+.....+.......+..+..........+..+...+......+.+...+...........++++++

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl req -x509 -new -nodes -key ca.key -sha256 -days 365 -out ca.crt \
    -subj "/C=US/ST=California/L=Mountain View/O=Google Inc./OU=CTF/CN=ca.crypto-tls.2024-bq.ctfcompetition.com" \
    -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl genpkey -algorithm RSA -out client.key -pkeyopt rsa_keygen_bits:2048
..........+.+...............+.....+......+.+++++++++++++++++++++++++++++++++++++++*......+.........+++++++++++++++++++++++++++++++++++++++*..+......+....+..+.+.....+....+....................+.+............+.....+.............+..+......+.......+........+.+......+...+...........+.........+....+..+....+.........+........+............+......+....+...+..+......+.+........+.............+..+......+....+...........+.+.....+....+..+............+.......+...+.....+.+.....+.......+.....+.......+..+.+..............+....+.........+..+...+..........+..+....+.................+....+...+..+....+.....+............+...+....+...........+.+......+.........+......+.........+...+...+............+..+......+.........+....+..+...+.......+.........+........+......+.+.........+..............+...+.......+......+...........+...+.+......+...+..................+..+.......+......+..+......+......+.+...+..+.......+...............+......+.....+......+.+..+..................+....+........+..........+............+.....+.+..+.............+..+...+..........+.................+.......+...+..+.........+.........+.+...............+......+...........+..........+.....+....+...+...+...+.....+....+......+......+.........+..+...+...................+..............................+...+.........+......+.....+.........+....+......+..............+.........+..........+.....+.+.................+...+.+...+...+..+......+.+...+........+........................+....+......+......+...+..+...+......+....+...............+......+.....+.........+.......+............+......+..+............+...+....+...+......+......+.....+...+......+.........+.+..+...+....+........+.......+........+...+....+...+..+...............+.+.........+............+.......................+......+.+.....+...+...+...+.+...+.................+...+...+....+.........+..+....+...+...+..+.+..+...+......+.........+......+......+.......+.....+.......+...+...+.....+...+......+.+.................+.+..+...+................+...+...+........+.......+..+......+....+...+..+...+.........+....+...........+....+.........+..+...+.+..+...+....+............+..............+................+...+........+.........+....+.....+....++++++
....+......+++++++++++++++++++++++++++++++++++++++*......+...+..+.......+..+......+...+++++++++++++++++++++++++++++++++++++++*.+.....+...............+...+.......+...+..+..................+..................+.+...+...........+..........+..+....+........+.+..+.......+........+...+....+...+...+.....................+.....+...+...+...+......+......+..........+..+...+.........+....+......+........+......+.+.....+...+......+.........+......+.......+..............+....+..+...+....+...+........................+.........+..+.........+.+...+......+..............+.+.....+......+...+.+..................+...+..+.......+..............+.+......+..+......+.+...+...+...+..............+...+...+.......+........+.+...+...+...+...........+...+...............+......+....+............+...+..+.........+.......+........+............+...+......+.+.....+...+................+...+..+...+......+.......+...............+..+......+.+......+..+............+...+.......+...+...............+..+...+............+......+............+.+...........+...+.+......+.....+........................+......+.............+.....+.......+.....+....+.........+...+...+...............+......+.....+......+....+.....+................+...+..+...+............+.........................+...+.....+.......+..............+...+......+.+......+..+.......+..................+..+...+.+.........+......+.....+.......+..+...+....+......+..+.......+.....+...+......+....+..+.+.....+...+.+...+...............+..+...+............+...+.+..+..........+...+..+.........+....+...........+....+......+...........+...+.+...++++++

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl req -new -key client.key -out client.csr \
    -subj "/C=US/ST=California/L=Mountain View/O=Google/OU=CTF/CN=crypto-tls.2024-bq.ctfcompetition.com"

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ nano v3.ext

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ cat v3.ext
authorityKeyIdentifier=keyid,issuer
basicConstraints=CA:FALSE
keyUsage = digitalSignature, keyEncipherment
extendedKeyUsage = clientAuth


┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl x509 -req -in client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out client.crt -days 365 -sha256 \
    -extfile v3.ext -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1
Certificate request self-signature ok
subject=C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl verify -CAfile ca.crt client.crt
client.crt: OK

┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ cat client.crt ca.crt > client-chain.crt
```

Let's try using that:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS/client_1]
└─$ openssl s_client -connect tls.2024-bq.ctfcompetition.com:1337 -CAfile ../root.pem -cert client-chain.crt -key client.key 1>/dev/null
Connecting to 35.205.223.136
depth=1 C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
verify return:1
depth=0 C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com
verify return:1
40E706AE4E7F0000:error:0A000418:SSL routines:ssl3_read_bytes:tlsv1 alert unknown ca:../ssl/record/rec_layer_s3.c:908:SSL alert number 48
```

The server won't accept our certificate ("Unknown CA"), which is the right thing to do, since
we just created a root for our client chain, given that fact that we don't have the server's
root certificate private key. So indeed, even if our chain is valid in the sense that our
root signed our leaf and the subject and issuer fields match etc., the server does not
trust our root and therefore rejects our chain. Well, it was worth a try, because that alternative
is that we somehow need to find the server's root private key, and that's a bit harder.

So, how do we find the server's root private key? Well, they might have used a weak key,
vulnerable to some attack, allowing us to crack it. There are quite a few attacks, many
of them automated in tools such as [RsaCtfTool](https://github.com/RsaCtfTool/RsaCtfTool),
but unfortunately it wasn't able to recover the key.

Now, inspecting the root certificate manually, we can see something quite peculiar: 

```console
┌──(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ openssl x509 -in root.pem -noout -text
Certificate:
    Data:
        Version: 3 (0x2)
        Serial Number:
            7e:14:8f:92:d7:57:5a:77:6b:b5:3c:f7:11:91:0b:5d:7d:3c:66:c7
        Signature Algorithm: sha256WithRSAEncryption
        Issuer: C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
        Validity
            Not Before: Jul 29 12:16:47 2024 GMT
            Not After : Jul 27 12:16:47 2034 GMT
        Subject: C=US, ST=California, L=Mountain View, O=Google Inc., OU=CTF, CN=ca.crypto-tls.2024-bq.ctfcompetition.com
        Subject Public Key Info:
            Public Key Algorithm: rsaEncryption
                Public-Key: (2047 bit)
                Modulus:
                    40:40:00:00:00:00:00:00:00:00:00:00:00:00:00:
                    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
                    00:00:00:40:40:00:00:02:02:00:00:00:00:80:80:
                    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
                    00:00:00:00:00:00:00:40:00:00:00:00:00:00:00:
                    00:00:00:00:00:00:00:00:00:20:00:00:00:00:00:
                    04:00:00:00:00:00:00:00:00:00:40:00:00:00:02:
                    00:00:00:00:40:c0:20:00:00:00:00:00:20:00:00:
                    00:01:00:04:00:00:00:51:30:80:00:00:00:08:00:
                    00:00:00:00:00:00:00:00:00:20:00:00:00:01:00:
                    00:00:00:00:40:00:00:00:00:00:00:80:00:00:00:
                    04:00:00:00:00:01:00:00:00:00:00:40:00:00:00:
                    00:00:00:00:00:00:00:00:00:00:00:00:00:20:10:
                    80:00:00:00:04:00:00:00:00:00:00:00:00:00:00:
                    00:08:40:00:00:00:00:01:08:00:20:00:00:00:00:
                    00:00:00:00:00:00:00:00:00:00:00:00:80:08:40:
                    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
                    21
                Exponent: 65537 (0x10001)
        X509v3 extensions:
            X509v3 Subject Key Identifier:
                2E:1B:56:1B:81:86:D6:62:66:6F:1B:32:7A:EE:D4:60:20:88:E5:5A
            X509v3 Authority Key Identifier:
                2E:1B:56:1B:81:86:D6:62:66:6F:1B:32:7A:EE:D4:60:20:88:E5:5A
            X509v3 Basic Constraints: critical
                CA:TRUE
    Signature Algorithm: sha256WithRSAEncryption
    Signature Value:
        19:22:4b:a0:32:1d:b9:35:63:6b:c9:d3:13:cc:24:0e:e7:ff:
        7b:fc:d3:09:43:99:72:c7:cc:9d:94:de:10:f9:5a:14:a5:ef:
        0b:9c:9e:44:00:54:00:b6:d6:12:a0:09:a0:87:89:5a:8b:88:
        a3:94:95:c3:df:44:21:0e:fb:4a:19:ed:f9:bb:38:6f:38:f5:
        5b:62:ce:35:29:5c:dc:c1:76:48:d6:c6:18:c1:f8:1b:f2:6a:
        60:d9:c2:78:31:9c:fa:b8:d7:d0:8a:64:e7:1d:a8:48:a4:5f:
        f6:ed:3c:e7:17:74:e1:52:8e:13:a4:23:c6:18:50:82:c3:08:
        2d:73:4b:d1:ef:a2:cd:88:3a:70:dd:fe:bc:00:6b:5c:7a:6b:
        e6:90:6c:18:27:4b:03:1a:cc:34:a5:a1:7e:98:95:df:95:12:
        51:c9:65:0c:e0:a9:36:33:d2:fd:9a:e4:50:b7:95:67:04:60:
        c9:c8:df:34:7b:9b:34:be:d2:5f:91:b8:bd:f5:b3:21:3c:f2:
        ab:a6:98:13:e2:1f:8a:29:34:43:05:cc:9b:82:cf:16:4e:94:
        c4:36:92:98:69:39:77:87:64:ee:a6:3c:13:61:a6:d0:bb:54:
        3d:75:6d:8d:ec:07:58:86:49:ca:38:f4:a1:75:ec:a6:c6:eb:
        1e:38:f2:5b
```

The Modulus (`n`) seems quite sparse. There are too many zeroes there, many more than
you'd usually see in a product of two random prime numbers. That can't be by chance.

At this point I knew this is odd, but I didn't know how to exploit this. Reached out to 
DHG (JCTF's crypto expert) and fortunately he was able to find an 
[old writeup](https://connor-mccartney.github.io/cryptography/rsa/VULPIX-squeamishossifrage) to tackle
a similar problem. 

Basically, these challenges involve exploiting RSA keys with factors that have a low Hamming weight 
(where the Hamming weight of a number is the count of '1' bits in its binary representation). 
The attack is based on the fact that primes with low Hamming weights are less secure because 
they can be more easily factored. The solution in the writeup involves implementing a search 
algorithm that systematically guesses bits of the prime factors, prioritizing guesses that 
maintain a low Hamming weight. This approach reduces the complexity of the factorization process, 
making it feasible to break the RSA key under these specific conditions.

The exact same script was able to recover `p` and `q` for our case as well!

```console
┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ python3 solve.py
p = 89884656743115795386465259539451236680898848947115328636715040578866337902750484598616137708088516025823585994686025863313986846698764657502915661884986403377838628930019378658051512073715573921213766867969662464632717290719130076168131592150888862486113302007173567965244388546763845682906661647228625485857
q = 90235768683518591462193639459527218074183610075814997889202208706127534535183100634972809905974095229351307162357320493981072208174423968712230057957704785088059674213092623486087004417727144362590734101539053643608719856442322673680034296833223264817813700752568111957733325623343615611536918274774558310401
```

So now we can use `p` and `q` to re-create the server's private key and correctly sign our
client leaf.

```console
┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ python3 RsaCtfTool/RsaCtfTool.py -p 89884656743115795386465259539451236680898848947115328636715040578866337902750484598616137708088516025823585994686025863313986846698764657502915661884986403377838628930019378658051512073715573921213766867969662464632717290719130076168131592150888862486113302007173567965244388546763845682906661647228625485857 -q 90235768683518591462193639459527218074183610075814997889202208706127534535183100634972809905974095229351307162357320493981072208174423968712230057957704785088059674213092623486087004417727144362590734101539053643608719856442322673680034296833223264817813700752568111957733325623343615611536918274774558310401 -e 65537 --private --output root_key.pem
['/tmp/tmp06m_ec6o']

Results for /tmp/tmp06m_ec6o:

Private key :
-----BEGIN RSA PRIVATE KEY-----
MIIEoQIBAAKCAQBAQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABAQAAA
AgIAAAAAgIAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAQAAAAAAAAAAAAAAAAAAAAAAg
AAAAAAAEAAAAAAAAAAAAQAAAAAIAAAAAQMAgAAAAAAAgAAAAAQAEAAAAUTCAAAAA
CAAAAAAAAAAAAAAgAAAAAQAAAAAAQAAAAAAAAIAAAAAEAAAAAAEAAAAAAEAAAAAA
AAAAAAAAAAAAAAAAIBCAAAAABAAAAAAAAAAAAAAACEAAAAAAAQgAIAAAAAAAAAAA
AAAAAAAAAACACEAAAAAAAAAAAAAAAAAAAAAhAgMBAAECggEAOTeFiHp3hYh6d4WI
eneFiHp3hYh6d4WIeneFiHp3hYh6sL0OAvPGyi810TyeQOG/HkDhvx5A4b8eQOG/
HkDhvx5A4fgcx+M4HMfjOBzH4zgcx+M4OUcmuNlHKkjBtz5Iwbc+SPq1xUo8fbmC
RrdjIVw+o8FcWyME3PwHAojpd13x0R4u4dg9/8IAPf/CAD3/3n+BgH9je5yEY7Sb
C2T0mwtk9JsLZPSbC2T0mwtk9JtEY3uchGN7nIRje5yEY3uchIAJH6bgWSM2yMk3
NsjJNzbIyT5WmalmVpqNYI8e0OEvHtDhLx7Q4S8e0OEvHtgA//8AAP//AAD//wAA
//8AAQKBgQCAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAACAAAAABAAA
AAABAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAAAAIQKBgQCAgAAA
AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
AAAAAAAAAAAAAAAAAAAAgAAAAAAAAAAAAAAAAAAAAABAAAAAAAAIAAAAAAAAAAAA
AAAAAAAAAAAAAABAAAAAAAAAAAAAAAAAAAAAAQKBgB0xYs6dMWLOnTFizp0xYs6d
MWLOnTFizp0xYs6dMWLOnU6UMWvPfbyCQ3325Qka9uUJGvblCRr25Qka9uUJGvbl
CRr25Qka9uUJGvblCRr25Qka9uUJGvblCRr25Qka9uUJGvblCRr25Qka9uUmTFmz
pkxZs6ZMWbOmTFmzpkxhAoGAcRy7Y0Scu2NEnLtjRJy7Y0Scu2NEnLtjRJy7Y0Sc
u2NEnLtjRJy7Y0Scu2NEnLtjRJy7Y0Scu2NEnLtjRJy7Y0Scu9PwrA9T8KwPU/Cs
D1PwrA9UKQIW/ekCHgip91YIqfdWCKn3Vgip91YIqfdWQP//AAD//wAA//8AAP//
AAECgYATrHiX8o7BXKk6ccmVBFT6f9F8kEdtp9G29nr4gvuPsF6ZUwL9DOyffe/o
oK6FbZgkhKo7GVBIHAE5JTYeX4AlItH68ctz60UItOAjhgnt6/I7PCmWe60FVU3C
+rXbY9BLNvpiTEoq4CXf1ifpRRGaMBzDZJTloAV4cM3qWmQ5rw==
-----END RSA PRIVATE KEY-----

┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS]
└─$ openssl rsa -in root_key.pem -text -noout
Private-Key: (2047 bit, 2 primes)
modulus:
    40:40:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:40:40:00:00:02:02:00:00:00:00:80:80:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:40:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:20:00:00:00:00:00:
    04:00:00:00:00:00:00:00:00:00:40:00:00:00:02:
    00:00:00:00:40:c0:20:00:00:00:00:00:20:00:00:
    00:01:00:04:00:00:00:51:30:80:00:00:00:08:00:
    00:00:00:00:00:00:00:00:00:20:00:00:00:01:00:
    00:00:00:00:40:00:00:00:00:00:00:80:00:00:00:
    04:00:00:00:00:01:00:00:00:00:00:40:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:20:10:
    80:00:00:00:04:00:00:00:00:00:00:00:00:00:00:
    00:08:40:00:00:00:00:01:08:00:20:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:80:08:40:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    21
publicExponent: 65537 (0x10001)
privateExponent:
    39:37:85:88:7a:77:85:88:7a:77:85:88:7a:77:85:
    88:7a:77:85:88:7a:77:85:88:7a:77:85:88:7a:77:
    85:88:7a:b0:bd:0e:02:f3:c6:ca:2f:35:d1:3c:9e:
    40:e1:bf:1e:40:e1:bf:1e:40:e1:bf:1e:40:e1:bf:
    1e:40:e1:bf:1e:40:e1:f8:1c:c7:e3:38:1c:c7:e3:
    38:1c:c7:e3:38:1c:c7:e3:38:39:47:26:b8:d9:47:
    2a:48:c1:b7:3e:48:c1:b7:3e:48:fa:b5:c5:4a:3c:
    7d:b9:82:46:b7:63:21:5c:3e:a3:c1:5c:5b:23:04:
    dc:fc:07:02:88:e9:77:5d:f1:d1:1e:2e:e1:d8:3d:
    ff:c2:00:3d:ff:c2:00:3d:ff:de:7f:81:80:7f:63:
    7b:9c:84:63:b4:9b:0b:64:f4:9b:0b:64:f4:9b:0b:
    64:f4:9b:0b:64:f4:9b:0b:64:f4:9b:44:63:7b:9c:
    84:63:7b:9c:84:63:7b:9c:84:63:7b:9c:84:80:09:
    1f:a6:e0:59:23:36:c8:c9:37:36:c8:c9:37:36:c8:
    c9:3e:56:99:a9:66:56:9a:8d:60:8f:1e:d0:e1:2f:
    1e:d0:e1:2f:1e:d0:e1:2f:1e:d0:e1:2f:1e:d8:00:
    ff:ff:00:00:ff:ff:00:00:ff:ff:00:00:ff:ff:00:
    01
prime1:
    00:80:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:80:00:00:00:04:00:00:00:00:01:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:80:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:21
prime2:
    00:80:80:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:80:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:00:00:40:00:00:00:00:
    00:08:00:00:00:00:00:00:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:40:00:00:00:00:00:00:00:
    00:00:00:00:00:00:00:00:01
exponent1:
    1d:31:62:ce:9d:31:62:ce:9d:31:62:ce:9d:31:62:
    ce:9d:31:62:ce:9d:31:62:ce:9d:31:62:ce:9d:31:
    62:ce:9d:4e:94:31:6b:cf:7d:bc:82:43:7d:f6:e5:
    09:1a:f6:e5:09:1a:f6:e5:09:1a:f6:e5:09:1a:f6:
    e5:09:1a:f6:e5:09:1a:f6:e5:09:1a:f6:e5:09:1a:
    f6:e5:09:1a:f6:e5:09:1a:f6:e5:09:1a:f6:e5:09:
    1a:f6:e5:09:1a:f6:e5:09:1a:f6:e5:09:1a:f6:e5:
    09:1a:f6:e5:26:4c:59:b3:a6:4c:59:b3:a6:4c:59:
    b3:a6:4c:59:b3:a6:4c:61
exponent2:
    71:1c:bb:63:44:9c:bb:63:44:9c:bb:63:44:9c:bb:
    63:44:9c:bb:63:44:9c:bb:63:44:9c:bb:63:44:9c:
    bb:63:44:9c:bb:63:44:9c:bb:63:44:9c:bb:63:44:
    9c:bb:63:44:9c:bb:63:44:9c:bb:63:44:9c:bb:63:
    44:9c:bb:63:44:9c:bb:d3:f0:ac:0f:53:f0:ac:0f:
    53:f0:ac:0f:53:f0:ac:0f:54:29:02:16:fd:e9:02:
    1e:08:a9:f7:56:08:a9:f7:56:08:a9:f7:56:08:a9:
    f7:56:08:a9:f7:56:40:ff:ff:00:00:ff:ff:00:00:
    ff:ff:00:00:ff:ff:00:01
coefficient:
    13:ac:78:97:f2:8e:c1:5c:a9:3a:71:c9:95:04:54:
    fa:7f:d1:7c:90:47:6d:a7:d1:b6:f6:7a:f8:82:fb:
    8f:b0:5e:99:53:02:fd:0c:ec:9f:7d:ef:e8:a0:ae:
    85:6d:98:24:84:aa:3b:19:50:48:1c:01:39:25:36:
    1e:5f:80:25:22:d1:fa:f1:cb:73:eb:45:08:b4:e0:
    23:86:09:ed:eb:f2:3b:3c:29:96:7b:ad:05:55:4d:
    c2:fa:b5:db:63:d0:4b:36:fa:62:4c:4a:2a:e0:25:
    df:d6:27:e9:45:11:9a:30:1c:c3:64:94:e5:a0:05:
    78:70:cd:ea:5a:64:39:af
```

We use it to sign the leaf:

```console
┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ openssl genpkey -algorithm RSA -out client.key -pkeyopt rsa_keygen_bits:2048
.+.....+.......+..+.......+..............+......+...+++++++++++++++++++++++++++++++++++++++*............+...+.......+++++++++++++++++++++++++++++++++++++++*..+............+...+......+.+..+......................+...+.....+.+..+...+....+..+....+...........+.+...+..+.........+.....................+.........+.+.....+.......+..+......+....+...+.....+.+.....+................+.....+...+......+.+...+.....+...+...+.+.........+..+.......+.......................+...+....+...+..+.............+........+.+........+...+....+.....+...+......+....+......+.........+..+....+.........+..+...+......+.+........+...+...+....+..................+........+.+.....+......+.........+....+............+...+.................+..........+........+.+..............+......+...+...+.........+...+...+..........+...+............+..+.......+.....+................+...+..+...+....+......+...........+....+.....+.+..................+..+.+........+......+....+..+.......+...........+.+...+...............+.....+......+.+.........+.....+................+..............+....+...+..+.......+........+.............+...+......+........+.+...........+...+.+.........+...+.....+...............+...+..........+...........+...+.+..+...+.......+.........+......+...............+..+.+.....+....+...+..............++++++
.......+.+......+........+......+.+++++++++++++++++++++++++++++++++++++++*.+..........+...+..+.+..............+++++++++++++++++++++++++++++++++++++++*..+....................+....+......+..+.........+.+...+..+.................................+.+.....+.+..+......+.+......+.....++++++

┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ openssl req -new -key client.key -out client.csr \
    -subj "/C=US/ST=California/L=Mountain View/O=Google/OU=CTF/CN=crypto-tls.2024-bq.ctfcompetition.com"

┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ cp ../client_1/v3.ext .

┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ openssl x509 -req -in client.csr -CA ../root.pem -CAkey ../root_key.pem -CAcreateserial -out client.crt -days 365 -sha256 \
    -extfile v3.ext -sigopt rsa_padding_mode:pss -sigopt rsa_pss_saltlen:-1
Certificate request self-signature ok
subject=C=US, ST=California, L=Mountain View, O=Google, OU=CTF, CN=crypto-tls.2024-bq.ctfcompetition.com

┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ openssl verify -CAfile ../root.pem client.crt
client.crt: OK
```

And finally, we use this client cert to get the flag:

```console
┌──(py_ctf_env)─(user@kali3)-[/media/sf_CTFs/google/TLS/client_2]
└─$ curl -k https://tls.2024-bq.ctfcompetition.com:1337 --cert client.crt --key client.key
FLAG{your_modulus_is_weak}
```