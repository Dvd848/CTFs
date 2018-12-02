# Firmware
RE

## Description:
> After unpacking the firmware archive, you now have a binary in which to go hunting. Its now time to walk around the firmware and see if you can find anything.

A binary file was attached.

## Solution:

Let's inspect the binary file:

```console
root@kali:/media/sf_CTFs/google/firmware# file 9522120f36028c8ab86a37394903b100ce90b81830cee9357113c54fd3fc84bf
9522120f36028c8ab86a37394903b100ce90b81830cee9357113c54fd3fc84bf: Zip archive data, at least v2.0 to extract
```

Unzip it:
```console
root@kali:/media/sf_CTFs/google/firmware# unzip 9522120f36028c8ab86a37394903b100ce90b81830cee9357113c54fd3fc84bf
Archive:  9522120f36028c8ab86a37394903b100ce90b81830cee9357113c54fd3fc84bf
 extracting: challenge.ext4.gz
```

Extract that:
```console
root@kali:/media/sf_CTFs/google/firmware# gunzip challenge.ext4.gz
root@kali:/media/sf_CTFs/google/firmware# ls
9522120f36028c8ab86a37394903b100ce90b81830cee9357113c54fd3fc84bf  challenge.ext4
```

Let's try to mount the file system:
```console
root@kali:/media/sf_CTFs/google/firmware# mount challenge.ext4 /media/m
mount: /media/m: wrong fs type, bad option, bad superblock on /dev/loop0, missing codepage or helper program, or other error.
```

Is it really ext4?
```console
root@kali:/media/sf_CTFs/google/firmware# file challenge.ext4
challenge.ext4: Linux rev 1.0 ext4 filesystem data, UUID=00ed61e1-1230-4818-bffa-305e19e53758 (extents) (64bit) (large files) (huge files)
```

Seems so. Any errors?
```console
root@kali:/media/sf_CTFs/google/firmware# e2fsck challenge.ext4
e2fsck 1.44.1 (24-Mar-2018)
challenge.ext4: clean, 10889/76912 files, 236741/307200 blocks
```

Looks ok. At last, I try to copy from NTFS share to local file system:
```console
root@kali:/media/sf_CTFs/google/firmware# cp challenge.ext4 ~/CTFs/google/
root@kali:~/CTFs/google# mount challenge.ext4 /media/m
root@kali:/media/m# ls -al /media/m
total 44
drwxr-xr-x 22 root root  1024 Jun 22 16:54 .
drwxr-xr-x  6 root root  4096 Nov 26 22:54 ..
drwxr-xr-x  2 root root  3072 Jun 22 16:54 bin
drwxr-xr-x  2 root root  1024 Jun 22 16:54 boot
drwxr-xr-x  4 root root  1024 Jun 22 16:54 dev
drwxr-xr-x 52 root root  4096 Jun 22 16:54 etc
drwxr-xr-x  2 root root  1024 Jun 22 16:54 home
drwxr-xr-x 12 root root  1024 Jun 22 16:54 lib
drwxr-xr-x  2 root root  1024 Jun 22 16:54 lib64
drwx------  2 root root 12288 Jun 22 16:51 lost+found
drwxr-xr-x  2 root root  1024 Jun 22 16:54 media
-rw-r--r--  1 root root    40 Jun 22 16:54 .mediapc_backdoor_password.gz
drwxr-xr-x  2 root root  1024 Jun 22 16:54 mnt
drwxr-xr-x  2 root root  1024 Jun 22 16:54 opt
drwxr-xr-x  2 root root  1024 Jun 22 16:54 proc
drwx------  2 root root  1024 Jun 22 16:54 root
drwxr-xr-x  4 root root  1024 Jun 22 16:54 run
drwxr-xr-x  2 root root  3072 Jun 22 16:54 sbin
drwxr-xr-x  2 root root  1024 Jun 22 16:54 srv
drwxr-xr-x  2 root root  1024 Jun 22 16:54 sys
drwxr-xr-x  2 root root  1024 Jun 22 16:54 tmp
drwxr-xr-x 10 root root  1024 Jun 22 16:54 usr
drwxr-xr-x  9 root root  1024 Jun 22 16:54 var
```

One file stands out:
```console
root@kali:~/CTFs/google# cp /media/m/.mediapc_backdoor_password.gz .
root@kali:~/CTFs/google# gunzip .mediapc_backdoor_password.gz
root@kali:~/CTFs/google# cat .mediapc_backdoor_password
CTF{I_kn0W_tH15_Fs}
root@kali:~/CTFs/google# umount /media/m
```

The flag: CTF{I_kn0W_tH15_Fs}