# Home Computer
Category: Forensics

## Description
> Blunderbussing your way through the decision making process, you figure that one is as good as the other and that further research into the importance of Work Life balance is of little interest to you. You're the decider after all. You confidently use the credentials to access the "Home Computer." Something called "desktop" presents itself, displaying a fascinating round and bumpy creature (much like yourself) labeled "cauliflower 4 work - GAN post." Your 40 hearts skip a beat. It looks somewhat like your neighbors on XiXaX3. ..Ah XiXaX3... You'd spend summers there at the beach, an awkward kid from ObarPool on a family vacation, yearning, but without nerve, to talk to those cool sophisticated locals. So are these "Cauliflowers" earthlings? Not at all the unrelatable bipeds you imagined them to be. Will they be at the party? Hopefully SarahH has left some other work data on her home computer for you to learn more.

A binary file was attached.

## Solution

Let's inspect the file:

```console
root@kali:/media/sf_CTFs/google/Home_Computer# file family.ntfs
family.ntfs: DOS/MBR boot sector, code offset 0x52+2, OEM-ID "NTFS    ", sectors/cluster 8, Media descriptor 0xf8, sectors/track 0, dos < 4.0 BootSector (0x80), FAT (1Y bit by descriptor); NTFS, sectors 51199, $MFT start cluster 4, $MFTMirror start cluster 3199, bytes/RecordSegment 2^(-1*246), clusters/index block 1, serial number 072643f694104cb6f
```

It's an NTFS image file, we can inspect the contents with `fls`:
```console
root@kali:/media/sf_CTFs/google/Home_Computer# fls -pr family.ntfs | head -n 20
r/r 4-128-1:    $AttrDef
r/r 8-128-2:    $BadClus
r/r 8-128-1:    $BadClus:$Bad
r/r 6-128-1:    $Bitmap
r/r 7-128-1:    $Boot
d/d 11-144-2:   $Extend
r/r 25-144-2:   $Extend/$ObjId:$O
r/r 24-144-3:   $Extend/$Quota:$O
r/r 24-144-2:   $Extend/$Quota:$Q
r/r 26-144-2:   $Extend/$Reparse:$R
r/r 2-128-1:    $LogFile
r/r 0-128-1:    $MFT
r/r 1-128-1:    $MFTMirr
r/r 9-128-2:    $Secure:$SDS
r/r 9-144-3:    $Secure:$SDH
r/r 9-144-4:    $Secure:$SII
r/r 10-128-1:   $UpCase
r/r 10-128-2:   $UpCase:$Info
r/r 3-128-3:    $Volume
r/r 118-128-2:  bootmgr
```

It's usually interesting to search for text files:
```console
root@kali:/media/sf_CTFs/google/Home_Computer# fls -pr family.ntfs | grep -i txt
r/r 185-128-2:  Program Files/Windows Defender/ThirdPartyNotices.txt
r/r 244-128-2:  Program Files/windows nt/tabletextservice/TableTextServiceArray.txt
r/r 245-128-2:  Program Files/windows nt/tabletextservice/TableTextServiceDaYi.txt
r/r 246-128-2:  Program Files/windows nt/tabletextservice/TableTextServiceTigrinya.txt
r/r 247-128-2:  Program Files/windows nt/tabletextservice/TableTextServiceYi.txt
r/r 522-128-2:  Program Files/WindowsApps/Microsoft.MicrosoftSolitaireCollection_4.0.1301.0_x86__8wekyb3d8bbwe/ThirdPartyNotices.txt
r/r 860-128-2:  Program Files/WindowsApps/Microsoft.Print3D_3.3.791.0_x64__8wekyb3d8bbwe/GCode.txt
r/r 1156-128-2: Program Files/WindowsApps/Microsoft.WebMediaExtensions_1.0.13321.0_x64__8wekyb3d8bbwe/Third Party Notices.txt
r/r 1507-128-2: Program Files/WindowsApps/Microsoft.Xbox.TCUI_1.24.10001.0_x64__8wekyb3d8bbwe/ThirdPartyNotice.txt
r/r 1760-128-2: Program Files (x86)/windows nt/tabletextservice/TableTextServiceArray.txt
r/r 1761-128-2: Program Files (x86)/windows nt/tabletextservice/TableTextServiceDaYi.txt
r/r 1762-128-2: Program Files (x86)/windows nt/tabletextservice/TableTextServiceTigrinya.txt
r/r 1763-128-2: Program Files (x86)/windows nt/tabletextservice/TableTextServiceYi.txt
r/r 13288-128-2:        Users/Family/Documents/credentials.txt
r/r 13288-128-4:        Users/Family/Documents/credentials.txt:FILE0
r/r 3803-128-2: Windows/Panther/Rollback/FolderMoveLog.TXT
r/r 3804-128-2: Windows/Panther/Rollback/LogRestore.TXT
r/r 4880-128-2: Windows/System32/catroot2/dberr.txt
r/r 6582-128-2: Windows/System32/MailContactsCalendarSync/LiveDomainList.txt
r/r 7804-128-2: Windows/System32/restore/MachineGuid.txt
r/r 9420-128-2: Windows/System32/WindowsCodecsRaw.txt
r/r 9675-128-2: Windows/System32/WsmTxt.xsl
r/- * 0:        Windows/System32/WsmTxt.xsl
r/r 11329-128-2:        Windows/SysWOW64/MailContactsCalendarSync/LiveDomainList.txt
r/r 13042-128-2:        Windows/SysWOW64/WindowsCodecsRaw.txt
r/r 13219-128-2:        Windows/SysWOW64/WsmTxt.xsl
```

`Users/Family/Documents/credentials.txt` seems interesting:
```console
root@kali:/media/sf_CTFs/google/Home_Computer# icat family.ntfs 13288-128-2
I keep pictures of my credentials in extended attributes.
```

Extract the extended attributes:
```console
root@kali:/media/sf_CTFs/google/Home_Computer# icat family.ntfs 13288-128-4 > out.bin
root@kali:/media/sf_CTFs/google/Home_Computer# file out.bin
out.bin: PNG image data, 1234 x 339, 8-bit/color RGB, non-interlaced
root@kali:/media/sf_CTFs/google/Home_Computer# mv out.bin out.png
```

It's a PNG file:

![](images/credentials.png)

The flag: CTF{congratsyoufoundmycreds}