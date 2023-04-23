
# Plague of Boils

## Description

> > “So the Lord said to Moses and Aaron, ‘Take for yourselves handfuls of ashes from a furnace, and let Moses scatter it toward the heavens in the sight of Pharaoh. And it will become fine dust in all the land of Egypt, and it will cause boils that break out in sores on man and beast throughout all the land of Egypt’” (Exodus 9:8-9).
> 
> Wow! so many folders. You know, sometime, what's in front of you is not what you think it is. Maybe if you'll look at it from a different perspective you'll see it.

A RAR file was attached.

## Solution

We extract the RAR to get the following folder structure:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -v
.
└── New folder
    ├── New folder
    │   ├── New folder
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   ├── New folder (2)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   │       └── New folder
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (3)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       │   └── New folder
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (4)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (5)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   ├── New folder (6)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   │   └── New folder
    │   │   │   └── New folder (4)
    │   │   │       └── New folder
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (7)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (8)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   │       └── New folder
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (9)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   │           └── New folder
    │   ├── New folder (10)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   ├── New folder (11)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       │   └── New folder
    │   │       ├── New folder (6)
    │   │       │   └── New folder
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   ├── New folder (12)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   │       └── New folder
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       │   └── New folder
    │   │       ├── New folder (7)
    │   │       └── New folder (8)
    │   ├── New folder (13)
    │   │   ├── New folder
    │   │   │   ├── New folder
    │   │   │   ├── New folder (2)
    │   │   │   │   └── New folder
    │   │   │   ├── New folder (3)
    │   │   │   └── New folder (4)
    │   │   │       └── New folder
    │   │   └── New folder (2)
    │   │       ├── New folder (5)
    │   │       ├── New folder (6)
    │   │       ├── New folder (7)
    │   │       │   └── New folder
    │   │       └── New folder (8)
    │   └── New folder (14)
    │       ├── New folder
    │       │   ├── New folder
    │       │   ├── New folder (2)
    │       │   │   └── New folder
    │       │   ├── New folder (3)
    │       │   │   └── New folder
    │       │   └── New folder (4)
    │       │       └── New folder
    │       └── New folder (2)
    │           ├── New folder (5)
    │           │   └── New folder
    │           ├── New folder (6)
    │           │   └── New folder
    │           ├── New folder (7)
    │           └── New folder (8)
    │               └── New folder
    └── New folder (2)
        ├── New folder
        ├── New folder (2)
        ├── New folder (3)
        ├── New folder (4)
        └── New folder (5)

210 directories, 0 files
```

That's a lot of folders. Let's try to zoom out a bit to get a better perspective:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -Lv 3
.
└── New folder
    ├── New folder
    │   ├── New folder
    │   ├── New folder (2)
    │   ├── New folder (3)
    │   ├── New folder (4)
    │   ├── New folder (5)
    │   ├── New folder (6)
    │   ├── New folder (7)
    │   ├── New folder (8)
    │   ├── New folder (9)
    │   ├── New folder (10)
    │   ├── New folder (11)
    │   ├── New folder (12)
    │   ├── New folder (13)
    │   └── New folder (14)
    └── New folder (2)
        ├── New folder
        ├── New folder (2)
        ├── New folder (3)
        ├── New folder (4)
        └── New folder (5)

22 directories, 0 files
```

So if we limit the depth to 3, we have two subfolders, the first one with 14 subfolders and the second with 5. Now let's zoom it a bit more, but isolate just one path out of the 14:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -Lv 3 New\ folder/New\ folder/New\ folder
New folder/New folder/New folder
├── New folder
│   ├── New folder
│   ├── New folder (2)
│   │   └── New folder
│   ├── New folder (3)
│   └── New folder (4)
└── New folder (2)
    ├── New folder (5)
    ├── New folder (6)
    ├── New folder (7)
    │   └── New folder
    └── New folder (8)

12 directories, 0 files
```

Compare it to the next path of the same level:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -Lv 3 New\ folder/New\ folder/New\ folder\ \(2\)
New folder/New folder/New folder (2)
├── New folder
│   ├── New folder
│   ├── New folder (2)
│   │   └── New folder
│   ├── New folder (3)
│   └── New folder (4)
│       └── New folder
└── New folder (2)
    ├── New folder (5)
    ├── New folder (6)
    ├── New folder (7)
    │   └── New folder
    └── New folder (8)
        └── New folder

14 directories, 0 files
```

And the third:

```console
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -Lv 3 New\ folder/New\ folder/New\ folder\ \(3\)
New folder/New folder/New folder (3)
├── New folder
│   ├── New folder
│   ├── New folder (2)
│   │   └── New folder
│   ├── New folder (3)
│   └── New folder (4)
└── New folder (2)
    ├── New folder (5)
    │   └── New folder
    ├── New folder (6)
    │   └── New folder
    ├── New folder (7)
    └── New folder (8)
        └── New folder

14 directories, 0 files
```

We can see that the next level always has two subfolders, each with 4 subfolders itself. The deepest level of subfolders sometimes exists and sometimes doesn't.

We can think of it as binary:

```
├── New folder              <-- Byte
│   ├── New folder          <-- Nibble
│   │   ├── New folder      <-- Bit (no subfolder, so bit value is 0)
│   │   ├── New folder (2)  <-- Bit (has subfolder, so bit value is 1)
│   │   │   └── New folder
│   │   ├── New folder (3)
│   │   └── New folder (4)
```

Let's try this for the first path:

```
┌──(user@kali)-[/media/sf_CTFs/basmach/Plague_of_boils/New_folder]
└─$ tree -v New\ folder/New\ folder/New\ folder
New folder/New folder/New folder
├── New folder
│   ├── New folder      <-- 0
│   ├── New folder (2)  <-- 1
│   │   └── New folder
│   ├── New folder (3)  <-- 0
│   └── New folder (4)  <-- 0
└── New folder (2)
    ├── New folder (5)  <-- 0
    ├── New folder (6)  <-- 0
    ├── New folder (7)  <-- 1
    │   └── New folder
    └── New folder (8)  <-- 0

12 directories, 0 files
```

We got `0b01000010`, which is `B`! This matches the flag format.

Following the same technique, we get:

```
01000010 01010011 01001101 01000011 01001000 01111011 01000001 01010011 01001001 01001000 01001110 01010100 01010010 01111101
```

We decode as binary to get the flag: `BSMCH{ASIHNTR}`