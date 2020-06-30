# Check yourself so you won't wreck yourself
* Category: Forensics
* 100 Points
* Solved by the JCTF Team

## Description
> Dumbster diving.

A 6GB file was attached.

## Solution

Let's check the attached file:

```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# file checkyourself.E01
checkyourself.E01: EWF/Expert Witness/EnCase image file format
```

It's an image file, we can follow [this](https://www.andreafortuna.org/2018/04/11/how-to-mount-an-ewf-image-file-e01-on-linux/) guide in order to mount it:

```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# mkdir rawimage
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# ewfmount checkyourself.E01 ./rawimage/
ewfmount 20140807

root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# fdisk -l rawimage/ewf1
Disk rawimage/ewf1: 40 GiB, 42949672960 bytes, 83886080 sectors
Units: sectors of 1 * 512 = 512 bytes
Sector size (logical/physical): 512 bytes / 512 bytes
I/O size (minimum/optimal): 512 bytes / 512 bytes
Disklabel type: gpt
Disk identifier: E1EE055D-4293-4322-9DB1-11480F3861EB

Device            Start      End  Sectors  Size Type
rawimage/ewf1p1    2048  1085439  1083392  529M Windows recovery environment
rawimage/ewf1p2 1085440  1288191   202752   99M EFI System
rawimage/ewf1p3 1288192  1320959    32768   16M Microsoft reserved
rawimage/ewf1p4 1320960 83884031 82563072 39.4G Microsoft basic data
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# mkdir mountpoint
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself# mount ./rawimage/ewf1 ./mountpoint -o ro,loop,show_sys_files,streams_interace=windows,offset=$((1320960*512))
```

Now we can explore the mounted image:

```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint# ls
'$AttrDef'  '$Bitmap'  '$Extend'   '$MFTMirr'      '$Secure'  '$Volume'                  pagefile.sys   ProgramData     'Program Files (x86)'   swapfile.sys                 Users
'$BadClus'  '$Boot'    '$LogFile'  '$Recycle.Bin'  '$UpCase'  'Documents and Settings'   PerfLogs      'Program Files'   Recovery              'System Volume Information'   Windows
```

It looks like a Windows image. Let's take a look at the users' home directory:

```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint# cd Documents\ and\ Settings/BSIDESTLV2020/
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020# ls
'3D Objects'         Cookies     Links                  NetHood                                                                                        ntuser.dat.LOG1   PrintHood     'Start Menu'
 a                   Desktop    'Local Settings'        NTUSER.DAT                                                                                     ntuser.dat.LOG2   Recent         Templates
 AppData             Documents   MicrosoftEdgeBackups   NTUSER.DAT{fd9a35db-49fe-11e9-aa2c-248a07783950}.TM.blf                                        ntuser.ini       'Saved Games'   Videos
'Application Data'   Downloads   Music                  NTUSER.DAT{fd9a35db-49fe-11e9-aa2c-248a07783950}.TMContainer00000000000000000001.regtrans-ms   OneDrive          Searches
 Contacts            Favorites  'My Documents'          NTUSER.DAT{fd9a35db-49fe-11e9-aa2c-248a07783950}.TMContainer00000000000000000002.regtrans-ms   Pictures          SendTo
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020# cd Recent
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020/Recent# ls
 AutomaticDestinations   BSIDESTLV2020.lnk   CustomDestinations   desktop.ini   phrases.csv.lnk  'System and Security.lnk'   System.lnk
```

The "Recent" folder shows that a file called `BSIDESTLV2020.lnk` was recently accessed:
```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020/Recent# strings BSIDESTLV2020.lnk
Pw[
BSIDES~1.TXT
Pw[.
C:\Users\BSIDESTLV2020\Desktop\BSIDESTLV2020.txt
desktop-c3o3029
1SPS
```

Let's look for it:
```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020/Recent# cd ../Desktop
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/Documents and Settings/BSIDESTLV2020/Desktop# ls
 desktop.ini  'Microsoft Edge.lnk'   phrases.csv
```

It isn't there! Maybe it was deleted? Let's look for all deleted text files:

```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint# cd \$Recycle.Bin/
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/$Recycle.Bin# find . -type f -name '*.txt'
./S-1-5-21-3498983559-1615527653-2205644034-1001/$I0K6T9B.txt
./S-1-5-21-3498983559-1615527653-2205644034-1001/$R0K6T9B.txt
```

The flag is hiding in the second one:
```console
root@kali:/media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint/$Recycle.Bin# cat ./S-1-5-21-3498983559-1615527653-2205644034-1001/\$R0K6T9B.txt
BSIDESTLV{ICanSeeYouUnlessYouCleanUpAfterYourself}
```

Now we just need to unmount and we can free up 6GB.

```console
root@kali:~# umount /media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/mountpoint
root@kali:~# umount /media/sf_CTFs/bsidestlv/CheckYourself/checkyourself/rawimage
```