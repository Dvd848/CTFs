# KipodStab
Web, 5 points

## Description
> The one who stabbed me will never be exposed.

## Solution

Entering the provided link, we are presented with the Apache2 Debian Default Page ("It works!").

After basic enumeration did not lead anywhere, the next step was calling `dirseach` - a lightweight `dirbuster` clone which searches for common file names.

```console
root@kali:/media/sf_CTFs/kaf/KipodStab# python3 ~/utils/dirsearch/dirsearch.py -u http://ctf.kaf.sh:3040/ -e *

 _|. _ _  _  _  _ _|_    v0.3.8
(_||| _) (/_(_|| (_| )

Extensions: – | Threads: 10 | Wordlist size: 6040

Error Log: /hdd2/utils/dirsearch/logs/errors-19-12-20_20-47-36.log

Target: http://ctf.kaf.sh:3040/

[20:47:36] Starting:
[20:47:39] 403 -  199B  - /.htaccess-dev
[20:47:39] 403 -  199B  - /.ht_wsr.txt
[20:47:39] 403 -  199B  - /.hta
[20:47:39] 403 -  199B  - /.htaccess-local
[20:47:39] 403 -  199B  - /.htaccess.BAK
[20:47:39] 403 -  199B  - /.htaccess-marco
[20:47:39] 403 -  199B  - /.htaccess.bak1
[20:47:39] 403 -  199B  - /.htaccess.orig
[20:47:39] 403 -  199B  - /.htaccess.old
[20:47:39] 403 -  199B  - /.htaccess.save
[20:47:39] 403 -  199B  - /.htaccess.sample
[20:47:39] 403 -  199B  - /.htaccess.txt
[20:47:39] 403 -  199B  - /.htaccess_extra
[20:47:39] 403 -  199B  - /.htaccess_orig
[20:47:39] 403 -  199B  - /.htaccess_sc
[20:47:39] 403 -  199B  - /.htaccessBAK
[20:47:39] 403 -  199B  - /.htaccessOLD
[20:47:39] 403 -  199B  - /.htgroup
[20:47:39] 403 -  199B  - /.htaccessOLD2
[20:47:39] 403 -  199B  - /.htaccess~
[20:47:39] 403 -  199B  - /.htpasswd-old
[20:47:39] 403 -  199B  - /.htpasswd_test
[20:47:39] 403 -  199B  - /.htpasswds
[20:47:39] 403 -  199B  - /.htusers
[20:48:06] 200 -  107B  - /Dockerfile
[20:48:11] 301 -  237B  - /icons  ->  http://ctf.kaf.sh:3040/icons/
[20:48:13] 200 -    9KB - /index.html

Task Completed
```

Apart from `index.html`, the only other valid result is `/Dockerfile`, let's check it out:

```console
root@kali:/media/sf_CTFs/kaf/KipodStab# curl http://ctf.kaf.sh:3040/Dockerfile
FROM httpd:alpine

WORKDIR /usr/local/apache2/htdocs/

COPY . .

# Flag is KAF{dOn7_5748_doCK3R_CON741n3R2}
```

In retrospective, there was a hint in the page pointing to Docker:
```console
root@kali:/media/sf_CTFs/kaf/KipodStab# curl http://ctf.kaf.sh:3040/ -s | grep docker
          found by accessing the <a href="https://www.docker.com/" style="color: #389fee;">manual</a> if the <tt>apache2-doc</tt>
```