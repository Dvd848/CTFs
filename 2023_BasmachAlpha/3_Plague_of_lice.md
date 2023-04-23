
# Plague of Lice

## Description

> > “So the Lord said to Moses, ‘Say to Aaron, “Stretch out your rod, and strike the dust of the land, so that it may become lice throughout all the land of Egypt.”’ And they did so. For Aaron stretched out his hand with his rod and struck the dust of the earth, and it became lice on man and beast. All the dust of the land became lice throughout all the land of Egypt” (Exodus 8:16-17).
> 
> I hate lice. Every time they come they never leave, and the worst part is that there are SO MANY of them.
> 
> P.S: we are not using ads on our file system

A RAR archive was attached.

## Solution

After extracting the archive, we find the following directory structure:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_lice/Passover]
└─$ tree -d
.
└── almost here
    ├── its here
    │   ├── could be here
    │   └── or here
    │       └── try here
    └── maybe here
        └── here
            ├── not here
            └── trust me it's here

9 directories
```

Inside the directories there are around 26,000 files:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_lice/Passover]
└─$ find . -type f | wc -l
26052
```

All the files are text files called `FLAG<n>.txt` where `<n>` is a running number. Let's see a few:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_lice/Passover]
└─$ find . -name "*.txt" | head -n 10 | xargs -d '\n' -i sh -c "cat '{}'; echo"
BSMCH{posS_onVA}
BSMCH{pesS_orVQ}
BSMCH{pssS_oyVQ}
BSMCH{prsS_ogVJ}
BSMCH{pdsS_omVU}
BSMCH{posS_owVZ}
BSMCH{phsS_ogVT}
BSMCH{pnsS_okVB}
BSMCH{pssS_otVT}
BSMCH{pdsS_opVF}
```

... All, but one!

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_lice/Passover]
└─$ find . -type f | file -f - | grep -v ASCII
./almost here/its here/FLAG53.txt:                                 PNG image data, 406 x 280, 8-bit/color RGBA, non-interlaced
```

Unfortunately that's a red herring too (image just says "Flag!").

Let's go back to the description:

> P.S: we are not using ads on our file system

In the context of file system, "ads" can refer to ["Alternate Data Streams"](https://en.wikipedia.org/wiki/NTFS#Alternate_data_stream_(ADS)) which is practically a way to "hide" more data in files. It turns out that [RAR is one of the only archives that supports ADS](https://superuser.com/questions/156971/ntfs-alternate-stream-in-compressed-archive). So what if THEY ARE using ADS in their file system?

One of the simplest ways to display an alternate data stream in the current folder is to run `dir /r` under Windows. Let's use that to try and recursively search for ADSs:

```console
D:\CTFs\basmach\Plague_of_lice>dir /r /s | findstr ":\$"
                                   22 Passover:Louse:$DATA
                                  267 Passover.rar:Zone.Identifier:$DATA
                                   22 .:Louse:$DATA
                                   22 ..:Louse:$DATA
                                   92 FLAG53.txt:Zone.Identifier:$DATA
```

It looks like the `Passover` folder itself has an ADS called `Passover:Louse`, let's print it:

```console
D:\CTFs\basmach\Plague_of_lice>more < Passover:Louse
BSMCH{T1nCh3mUcHKi}
```