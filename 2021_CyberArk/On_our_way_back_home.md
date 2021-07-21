# On our way back home (4/5)
Category: Innovation

## Description

> Your equipement is fixed and you are now planning your way back.
> 
> http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/rss

An MP3 file was attached.

## Solution

The MP3 file contains around two minutes of a clock ticking tick-tock. Three words can be heard throughout the file: "One" at the beginning, "Two" after a minute or so, and "Sorry(?)" at the end.


Let's visit the RSS link:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ curl http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/rss

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ curl http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/rss -I
HTTP/1.1 301 Moved Permanently
x-amz-id-2: XlTIfqLDZNanDEW8/p6nYWH5QY6hgGDGV2j4iEWAJWG9M1v4TBHNvEa6drda0OvZDsziXeIx7h0=
x-amz-request-id: B0NBF1H53AWAS370
Date: Sun, 18 Jul 2021 20:06:24 GMT
Location: http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/chl4/feed.xml
Server: AmazonS3
Content-Length: 0


┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ curl http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/rss -L
<?xml version="1.0" encoding="UTF-8"?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/"><channel><title>CyberArk Innovation challenge 4</title><link>https://www.cyberark.com</link><description>RSS feed for: CyberArk Innovation</description><language>en-EN</language><lastBuildDate>Sun, 18 Jul 2021 20:06:19 GMT</lastBuildDate><generator>rfeed v1.1.1</generator><docs>https://github.com/svpino/rfeed/blob/master/README.md</docs><item><title>CyberArk's spaceship</title><description>Coming home</description><author>CyberArk Innovation</author><pubDate>Sun, 18 Jul 2021 20:06:19 GMT</pubDate></item><item><title>ln(8103.08392758)/3</title><link>https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/c.txt</link><author>CyberArk Innovation</author><pubDate>Sun, 18 Jul 2021 20:06:19 GMT</pubDate></item></channel></rss>
```

We can see here an interesting title: `ln(8103.08392758)/3` (which results in 3) and a link to a file (`https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/c.txt`). Let's get it:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ wget https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/c.txt
--2021-07-18 23:10:45--  https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/c.txt
Resolving s3.us-west-2.amazonaws.com (s3.us-west-2.amazonaws.com)... 52.218.170.0
Connecting to s3.us-west-2.amazonaws.com (s3.us-west-2.amazonaws.com)|52.218.170.0|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 10047 (9.8K) [text/plain]
Saving to: ‘c.txt’

c.txt                                                       100%[========================================================================================================================================>]   9.81K  --.-KB/s    in 0.004s

2021-07-18 23:10:46 (2.73 MB/s) - ‘c.txt’ saved [10047/10047]


┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ file c.txt
c.txt: ASCII text, with very long lines, with no line terminators

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ head -c 100 c.txt
KvXXTO70F6zerJe99t2zoQY1JmImalXeer5x3zz2w9YFOd8tyh+9N4HvZ6rueUMbj+q5l62HI/P1eyLHBxGC4AqUel0W96rOjGzz
```

Looks like base64. Decoding the file results in a binary output:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ base64 -d c.txt | xxd -g 1 | head
base64: invalid input
00000000: 2a f5 d7 4c ee f4 17 ac de ac 97 bd f6 dd b3 a1  *..L............
00000010: 06 35 26 62 26 6a 55 de 7a be 71 df 3c f6 c3 d6  .5&b&jU.z.q.<...
00000020: 05 39 df 2d ca 1f bd 37 81 ef 67 aa ee 79 43 1b  .9.-...7..g..yC.
00000030: 8f ea b9 97 ad 87 23 f3 f5 7b 22 c7 07 11 82 e0  ......#..{".....
00000040: 0a 94 7a 5d 16 f7 aa ce 8c 6c f3 f7 4e 34 fb 11  ..z].....l..N4..
00000050: 6c ca 18 b9 93 0d 56 e5 6e 43 e7 05 46 ae 5a 4b  l.....V.nC..F.ZK
00000060: a6 a2 79 29 a6 4e 0b f8 93 67 22 45 dc e8 97 8c  ..y).N...g"E....
00000070: d9 ef cf dc fb ed 5b de 79 53 4d ff d8 37 65 98  ......[.ySM..7e.
00000080: cb df f1 5a eb ff bd 0f f7 bc 70 d6 ee f7 78 a3  ...Z......p...x.
00000090: 38 ed df 39 a0 df c7 62 d3 96 65 9e 6b 5d b7 cb  8..9...b..e.k]..
```

Well, the MP3 file resembled a clock ticking, what if we try again after waiting for a while:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ curl http://cyber-ctf.be.s3-website-us-west-2.amazonaws.com/rss -L
<?xml version="1.0" encoding="UTF-8"?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/"><channel><title>CyberArk Innovation challenge 4</title><link>https://www.cyberark.com</link><description>RSS feed for: CyberArk Innovation</description><language>en-EN</language><lastBuildDate>Sun, 18 Jul 2021 20:12:20 GMT</lastBuildDate><generator>rfeed v1.1.1</generator><docs>https://github.com/svpino/rfeed/blob/master/README.md</docs><item><title>CyberArk's spaceship</title><description>Coming home</description><author>CyberArk Innovation</author><pubDate>Sun, 18 Jul 2021 20:12:20 GMT</pubDate></item><item><title>puzzle</title><link>https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/q.txt</link><author>CyberArk Innovation</author><pubDate>Sun, 18 Jul 2021 20:12:20 GMT</pubDate></item></channel></rss> 
```

We got a new file! The title is "puzzle", and the link gives us:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ wget https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/q.txt
--2021-07-18 23:14:35--  https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/q.txt
Resolving s3.us-west-2.amazonaws.com (s3.us-west-2.amazonaws.com)... 52.92.129.184
Connecting to s3.us-west-2.amazonaws.com (s3.us-west-2.amazonaws.com)|52.92.129.184|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 10046 (9.8K) [text/plain]
Saving to: ‘q.txt’

q.txt                                                       100%[========================================================================================================================================>]   9.81K  --.-KB/s    in 0.003s

2021-07-18 23:14:36 (3.10 MB/s) - ‘q.txt’ saved [10046/10046]


┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ base64 -d q.txt | xxd -g 1 | head
base64: invalid input
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 02 b2 00 00 02 b2 08 02 00 00 00 a6 1b aa  ................
00000020: d7 00 00 58 12 49 44 41 54 78 9c ed dd 77 b8 2c  ...X.IDATx...w.,
00000030: 57 79 e7 fb f7 5d ab aa bb 77 3c 39 4b 47 39 23  Wy...]...w<9KG9#
00000040: 21 04 4a 80 24 40 48 42 e4 6c a2 31 78 b0 b1 8d  !.J.$@HB.l.1x...
00000050: cd d8 1e e7 19 8f e7 da 33 f6 0c 33 66 c0 e0 34  ........3..3f..4
00000060: 26 19 b0 09 26 8a 0c 22 18 50 42 01 09 e5 7c 72  &...&..".PB...|r
00000070: de b9 63 d5 5a ef fd a3 97 b0 98 eb d9 17 66 ab  ..c.Z.........f.
00000080: d5 b5 cf f9 7e 1e 9e 07 90 76 57 55 57 57 55 ff  ....~....vWUWWU.
00000090: 7a 55 d5 af d4 cc 04 00 00 40 c4 0d 7b 01 00 00  zU.......@..{...
```

This actually looks like a PNG file! Let's check what we got:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ base64 -d q.txt > q.png
base64: invalid input

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ pngcheck q.png
q.png  EOF while reading IDAT data
ERROR: q.png
```

Well, it's corrupted. But let's try again to wait and query the RSS feed, to get:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<rss version="2.0" xmlns:dc="http://purl.org/dc/elements/1.1/"><channel><title>CyberArk Innovation challenge 4</title><link>https://www.cyberark.com</link><description>RSS feed for: CyberArk Innovation</description><language>en-EN</language><lastBuildDate>Thu, 15 Jul 2021 22:08:19 GMT</lastBuildDate><generator>rfeed v1.1.1</generator><docs>https://github.com/svpino/rfeed/blob/master/README.md</docs><item><title>CyberArk's spaceship</title><description>Coming home</description><author>CyberArk Innovation</author><pubDate>Thu, 15 Jul 2021 22:08:19 GMT</pubDate></item><item><title>ln(22026.4657948)/5</title><link>https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl4/r.txt</link><author>CyberArk Innovation</author><pubDate>Thu, 15 Jul 2021 22:08:19 GMT</pubDate></item></channel></rss>
```

Now we have a link to `r.txt`, and a title of `ln(22026.4657948)/5` (which is `2`). Again, it's a base64 encoded file.

So, to proceed, we'll take `q.txt` ("puzzle"), append to it `r.txt` ("2") and `c.txt` ("3"), then try to decode it:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ cat q.txt r.txt c.txt | base64 -d > out.bin

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ file out.bin
out.bin: PNG image data, 690 x 690, 8-bit/color RGB, non-interlaced

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ pngcheck out.bin
OK: out.bin (690x690, 24-bit RGB, non-interlaced, 98.4%).
```

Surprisingly, we got a QR Code:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ zbarimg out.bin
QR-Code:https://bitbucket.org/space-expedition/expedition/src/master/, AKIA5FNOHSTOQECIOTPZ, S/UmZqK5bEl8edYiUtQqBKzanqMhtHOBbSDzj6GM
scanned 1 barcode symbols from 1 images in 0.06 seconds
```

We got a bitbucket repo, and what looks like AWS credentials!

Let's clone the repo:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home]
└─$ git clone https://bitbucket.org/space-expedition/expedition.git
Cloning into 'expedition'...
Unpacking objects: 100% (9/9), 1.35 KiB | 4.00 KiB/s, done.

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/expedition]
└─$ ls
bitbucket-pipelines.yml  README.md

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/expedition]
└─$ cat bitbucket-pipelines.yml
image:
  name: 904992888029.dkr.ecr.us-west-2.amazonaws.com/wayhome:latest
  aws:
    access-key: $AWS_ACCESS_KEY_ID
    secret-key: $AWS_SECRET_ACCESS_KEY
pipelines:
  default:
    - step:
        script:
          - echo 'note to myself - don’t spill coffee on spaceship parts'                                                                                                                                                                    

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/expedition]
└─$ cat README.md
After fixing the equipment on the spaceship, you are ready to go back home. But, did you lose those important coordinates of the landing site?

```

We should probably check the commit log:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/expedition]
└─$ git log
commit 850f3bb2bd09fdc843d1d06c67efd1d01b42a2cb (HEAD -> master, origin/master, origin/HEAD)
Author: Eli Shemesh <eli.shemesh@cyberark.com>
Date:   Wed Jun 30 14:51:29 2021 +0000

    Initial commit

commit cc2291d0d230e974d708854a96bdddb00982b528
Author: Eli Shemesh <eli.shemesh@cyberark.com>
Date:   Wed Jun 30 10:35:56 2021 +0000

    Initial commit

commit b6fda9453a5dfbd39cbe89200a8ddf450060248b
Author: Eli Shemesh <eli.shemesh@cyberark.com>
Date:   Wed Jun 30 10:34:39 2021 +0000

    Initial commit
```

Long story short, trying to access anything via the `awscli` fails - the user doesn't have permissions for almost anything. The only thing that does work is:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/new2]
└─$ export AWS_ACCESS_KEY_ID=AKIA5FNOHSTOQECIOTPZ

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/new2]
└─$ export AWS_SECRET_ACCESS_KEY=S/UmZqK5bEl8edYiUtQqBKzanqMhtHOBbSDzj6GM

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/new2]
└─$ export AWS_DEFAULT_REGION=us-west-2

┌──(user@kali)-[/media/sf_CTFs/cyberark/On_our_way_back_home/new2]
└─$ aws sts get-caller-identity
{
    "UserId": "AIDA5FNOHSTOX4C2Z76AU",
    "Account": "904992888029",
    "Arn": "arn:aws:iam::904992888029:user/ctf-bitbucket-access"
}
```

Trying to pull the `904992888029.dkr.ecr.us-west-2.amazonaws.com/wayhome:latest` docker image independently fails as well.

BUT - we have the pipeline, right? It's already deployed to bitbucket, how does it work? 

We clone the project and start with the following pipeline definition:

```yaml
image:
  name: 904992888029.dkr.ecr.us-west-2.amazonaws.com/wayhome:latest
  aws:
    access-key: AKIA5FNOHSTOQECIOTPZ
    secret-key: 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
pipelines:
  default:
     - step:
         script:
           - ls -al
```

A pipeline is automatically triggered. Output is:

```
+ ls -al
total 24
drwxrwxrwx 3 root root 4096 Jul 18 22:08 .
drwxr-xr-x 8 root root 4096 Jul 18 22:08 ..
drwxrwxrwx 8 root root 4096 Jul 18 22:08 .git
-rw-rw-rw- 1 root root  624 Jul 18 22:08 .gitignore
-rw-rw-rw- 1 root root  144 Jul 18 22:08 README.md
-rw-rw-rw- 1 root root  475 Jul 18 22:08 bitbucket-pipelines.yml
```

Now we try adding `ls /`:

```
+ ls /
bin
boot
dev
earth
etc
home
lib
lib64
media
mnt
opt
proc
root
run
sbin
srv
sys
tmp
usr
var
```

We can see `/earth`, what's there? We add `ls /earth`:

```
+ ls /earth
coords.txt.gpg
```

So this is a GPG encrypted text file with coordinates. But we need the password. Where can we find it?

Well, after searching around everywhere and even trying to brute force the password with `john`, it turns out that it's hiding in the environment variables:

```
+ printenv
NVM_RC_VERSION=
PASS=wDmL7uC3H8FTr3vS
HOSTNAME=19fb8a86-fad3-42a4-ab43-f9e150d7b3ab-pl8xh
KUBERNETES_PORT_443_TCP_PORT=443
BITBUCKET_GIT_HTTP_ORIGIN=http://bitbucket.org/avivco94/expedition-test
KUBERNETES_PORT=tcp://10.34.224.1:443
BITBUCKET_PROJECT_KEY=TEST
DOCKER_HOST=tcp://localhost:2375
BITBUCKET_DOCKER_HOST_INTERNAL=10.39.129.148
KUBERNETES_SERVICE_PORT=443
KUBERNETES_SERVICE_HOST=10.34.224.1
BITBUCKET_COMMIT=2d7fd9d3ed13dcb54f6881a6238e691995aaade7
BITBUCKET_CLONE_DIR=/opt/atlassian/pipelines/agent/build
BITBUCKET_STEP_TRIGGERER_UUID={5eb3cb57-b0ba-46bc-b023-2bd28ad15d4c}
NVM_DIR=/root/.nvm
BITBUCKET_REPO_IS_PRIVATE=true
LS_COLORS=
BITBUCKET_REPO_SLUG=expedition-test
BITBUCKET_REPO_OWNER_UUID={ccc1ce6e-a4cc-4ebb-809e-0ea4457dc6f1}
PIPELINES_JWT_TOKEN=$PIPELINES_JWT_TOKEN
PATH=/root/.nvm:/bin/versions/node/v4.2.1/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
NVM_NODEJS_ORG_MIRROR=https://nodejs.org/dist
PWD=/opt/atlassian/pipelines/agent/build
BITBUCKET_PIPELINE_UUID={d32eeb5c-14fd-4162-845c-6897bdb4d2ce}
BITBUCKET_BUILD_NUMBER=52
BITBUCKET_STEP_UUID={19fb8a86-fad3-42a4-ab43-f9e150d7b3ab}
BITBUCKET_WORKSPACE=avivco94
SHLVL=1
BITBUCKET_PROJECT_UUID={d4d15a2d-69cf-4d8f-9ba0-8a5c1b2cb9d6}
HOME=/root
BITBUCKET_GIT_SSH_ORIGIN=git@bitbucket.org:avivco94/expedition-test.git
KUBERNETES_PORT_443_TCP_PROTO=tcp
KUBERNETES_SERVICE_PORT_HTTPS=443
CI=true
BITBUCKET_REPO_UUID={e181a250-6b59-48e3-80a4-41d10ae6d663}
BITBUCKET_REPO_OWNER=avivco94
BITBUCKET_BRANCH=master
LESSOPEN=| /usr/bin/lesspipe %s
NVM_IOJS_ORG_MIRROR=https://iojs.org/dist
BITBUCKET_REPO_FULL_NAME=avivco94/expedition-test
BITBUCKET_STEP_RUN_NUMBER=1
DISPLAY=:99
KUBERNETES_PORT_443_TCP_ADDR=10.34.224.1
KUBERNETES_PORT_443_TCP=tcp://10.34.224.1:443
LESSCLOSE=/usr/bin/lesspipe %s %s
_=/usr/bin/printenv
```

So we have the password: `wDmL7uC3H8FTr3vS`. Let's use it to decode the file:

```
+ gpg --no-armor -o - --passphrase wDmL7uC3H8FTr3vS -d coords.txt.gpg > coords.txt
gpg: CAST5 encrypted data
gpg: gpg-agent is not available in this session
gpg: encrypted with 1 passphrase
gpg: WARNING: message was not integrity protected
```

And print the contents:

```
+ cat coords.txt
28.49662233341438, -80.54938494992965
```

The full yaml file:

```yaml
image:
  name: 904992888029.dkr.ecr.us-west-2.amazonaws.com/wayhome:latest
  aws:
    access-key: AKIA5FNOHSTOQECIOTPZ
    secret-key: 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
pipelines:
  default:
     - step:
         script:
           - ls -al
           - ls /
           - ls /earth
           - printenv
           - cd /earth
           - gpg --no-armor -o - --passphrase wDmL7uC3H8FTr3vS -d coords.txt.gpg > coords.txt
           - ls -al
           - cat coords.txt
```

We visit the coordinates in [Google Maps](https://www.google.com/maps/place/28%C2%B029'47.8%22N+80%C2%B032'57.8%22W/@28.496627,-80.5515736,17z/data=!3m1!4b1!4m5!3m4!1s0x0:0x0!8m2!3d28.4966223!4d-80.5493849) and get the flag: `Launch Complex 15`.