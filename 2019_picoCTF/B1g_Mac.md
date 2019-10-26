# B1g_Mac
Reverse Engineering, 500 points

## Description:
> Here's a zip file.

## Solution:

Let's extract the file:

```console
E:\CTFs\pico\B1g_Mac>"c:\Program Files\7-Zip\7z.exe" x b1g_mac.zip

7-Zip 19.00 (x64) : Copyright (c) 1999-2018 Igor Pavlov : 2019-02-21

Scanning the drive for archives:
1 file, 295964 bytes (290 KiB)

Extracting archive: b1g_mac.zip
--
Path = b1g_mac.zip
Type = zip
Physical Size = 295964

Everything is Ok

Folders: 1
Files: 19
Size:       2408051
Compressed: 295964
```

We get the following contents:
```console
E:\CTFs\pico\B1g_Mac>dir /s
 Volume in drive E is CTF
 Volume Serial Number is 3C2B-8C21

 Directory of E:\CTFs\pico\B1g_Mac

10/25/2019  11:04 PM    <DIR>          .
10/25/2019  11:04 PM    <DIR>          ..
10/25/2019  02:02 PM           295,964 b1g_mac.zip
04/06/2019  08:24 PM           110,279 main.exe
04/06/2019  02:11 AM    <DIR>          test
               2 File(s)        406,243 bytes

 Directory of E:\CTFs\pico\B1g_Mac\test

04/06/2019  02:11 AM    <DIR>          .
04/06/2019  02:11 AM    <DIR>          ..
03/26/2019  02:20 AM           127,654 Item01 - Copy.bmp
03/26/2019  02:20 AM           127,654 Item01.bmp
03/26/2019  02:20 AM           127,654 Item02 - Copy.bmp
03/26/2019  02:20 AM           127,654 Item02.bmp
03/26/2019  02:20 AM           127,654 Item03 - Copy.bmp
03/26/2019  02:20 AM           127,654 Item03.bmp
03/26/2019  02:21 AM           127,654 Item04 - Copy.bmp
03/26/2019  02:21 AM           127,654 Item04.bmp
03/26/2019  02:21 AM           127,654 Item05 - Copy.bmp
03/26/2019  02:21 AM           127,654 Item05.bmp
03/26/2019  02:24 AM           127,654 Item06 - Copy.bmp
03/26/2019  02:24 AM           127,654 Item06.bmp
03/26/2019  02:24 AM           127,654 Item07 - Copy.bmp
03/26/2019  02:24 AM           127,654 Item07.bmp
03/26/2019  02:25 AM           127,654 Item08 - Copy.bmp
03/26/2019  02:25 AM           127,654 Item08.bmp
03/26/2019  03:15 AM           127,654 ItemTest - Copy.bmp
03/26/2019  03:15 AM           127,654 ItemTest.bmp
              18 File(s)      2,297,772 bytes

     Total Files Listed:
              20 File(s)      2,704,015 bytes
               5 Dir(s)  17,938,530,304 bytes free
```

We have an executable and a folder named `test`, with 18 images.

Let's run the executable:
```console
E:\CTFs\pico\B1g_Mac>main.exe
No flag found, please make sure this is run on the server
```

That's a strange error message, since this is a Windows executable and the server runs on Linux. However, we've seen this message in the past when the program required a `flag.txt` file in its directory, so lets create one and try again:

```console
E:\CTFs\pico\B1g_Mac>echo picoCTF{fake_flag}>flag.txt

E:\CTFs\pico\B1g_Mac>main.exe
Work is done!
Wait for 5 seconds to exit.
```

Ok, something happened. Since no new file appeared, let's re-extract the `test` directory from the zip file and compare it to the contents of the current `test` directory in order to see if any of the files have changed.

```console
E:\CTFs\pico\B1g_Mac>move test test_dirty
        1 dir(s) moved.

E:\CTFs\pico\B1g_Mac>"c:\Program Files\7-Zip\7z.exe" e -y b1g_mac.zip -otest test

7-Zip 19.00 (x64) : Copyright (c) 1999-2018 Igor Pavlov : 2019-02-21

Scanning the drive for archives:
1 file, 295964 bytes (290 KiB)

Extracting archive: b1g_mac.zip
--
Path = b1g_mac.zip
Type = zip
Physical Size = 295964

Everything is Ok

Folders: 1
Files: 18
Size:       2297772
Compressed: 295964
```

Comparing the two folders using `WinMerge`, we can see that the files are binary same:
```
Compare E:\CTFs\pico\B1g_Mac\test with E:\CTFs\pico\B1g_Mac\test_dirty
10/25/2019 11:44:44 PM
Filename			      Comparison result						              Left Date							        Right Date						      Extension
test				        Left only: E:\CTFs\pico\B1g_Mac\test	  * 4/6/2019 2:11:26 AM				  									
Item01 - Copy.bmp	  Binary files are identical				        3/26/2019 2:20:08 AM				  3/26/2019 2:20:08 AM				bmp
Item01.bmp			    Binary files are identical				        3/26/2019 2:20:08 AM				  3/26/2019 2:20:08 AM				bmp
Item02 - Copy.bmp	  Binary files are identical				        3/26/2019 2:20:34 AM				  3/26/2019 2:20:34 AM				bmp
Item02.bmp			    Binary files are identical				        3/26/2019 2:20:34 AM				  3/26/2019 2:20:34 AM				bmp
Item03 - Copy.bmp	  Binary files are identical				        3/26/2019 2:20:50 AM				  3/26/2019 2:20:50 AM				bmp
Item03.bmp			    Binary files are identical				        3/26/2019 2:20:51 AM				  3/26/2019 2:20:51 AM				bmp
Item04 - Copy.bmp	  Binary files are identical				        3/26/2019 2:21:13 AM				  3/26/2019 2:21:13 AM				bmp
Item04.bmp			    Binary files are identical				        3/26/2019 2:21:13 AM				  3/26/2019 2:21:13 AM				bmp
Item05 - Copy.bmp	  Binary files are identical				        3/26/2019 2:21:28 AM				* 3/26/2019 2:21:28 AM				bmp
Item05.bmp			    Binary files are identical				        3/26/2019 2:21:29 AM				  3/26/2019 2:21:29 AM				bmp
Item06 - Copy.bmp	  Binary files are identical				        3/26/2019 2:24:17 AM				* 3/26/2019 2:24:17 AM				bmp
Item06.bmp			    Binary files are identical				        3/26/2019 2:24:18 AM				  3/26/2019 2:24:18 AM				bmp
Item07 - Copy.bmp	  Binary files are identical				      * 3/26/2019 2:24:47 AM				  3/26/2019 2:24:47 AM				bmp
Item07.bmp			    Binary files are identical				        3/26/2019 2:24:47 AM				  3/26/2019 2:24:47 AM				bmp
Item08 - Copy.bmp	  Binary files are identical				        3/26/2019 2:25:55 AM				* 3/26/2019 2:25:55 AM				bmp
Item08.bmp			    Binary files are identical				        3/26/2019 2:25:55 AM				  3/26/2019 2:25:55 AM				bmp
ItemTest - Copy.bmp	Binary files are identical				        3/26/2019 3:15:31 AM				* 3/26/2019 3:15:32 AM				bmp
ItemTest.bmp		    Binary files are identical				        3/26/2019 3:15:32 AM				  3/26/2019 3:15:32 AM				bmp
```

Let's dive into Ghidra's decompiled output:

```c
int __cdecl _main(int _Argc,char **_Argv,char **_Env)
{
  FILE *_File;
  size_t sVar1;
  undefined4 local_60;
  undefined local_5a [50];
  char local_28 [12];
  
  ___main();
  _isOver = 0;
                    /* _folderName = "./test" */
  local_28._0_4_ = 1702113070;
  local_28._4_4_ = 29811;
  local_28._8_4_ = 0;
  _folderName = local_28;
  _pLevel = 0;
  _File = open("flag.txt","r");
  if (_File == (FILE *)0x0) {
    puts("No flag found, please make sure this is run on the server");
  }
  sVar1 = fread(local_5a,1,18,_File);
  if ((int)sVar1 < 1) {
                    /* WARNING: Subroutine does not return */
    exit(0);
  }
  _flag = local_5a;
  _flag_size = 18;
  local_60 = 0;
  _flag_index = &local_60;
  puts("Work is done!");
  _listdir(0,_folderName);
  puts("Wait for 5 seconds to exit.");
  _sleep(5);
  return 2;
}
```

We can see that the program opens the flag file, reads 18 characters, initializes some globals (`_folderName`, `_flag`, `_flag_size`, `_flag_index`) and then calls `_listdir`. What does `_listdir` do?

```c
void __cdecl _listdir(int param_1,undefined4 folder_name)
{
  int iVar1;
  BOOL BVar2;
  char local_958 [2048];
  _WIN32_FIND_DATAA local_158;
  HANDLE local_18;
  bool local_11;
  int local_10;
  
  local_18 = (HANDLE)0x0;
  sprintf(local_958,"%s\\*.*",folder_name);
  local_18 = FindFirstFileA(local_958,(LPWIN32_FIND_DATAA)&local_158);
  if (local_18 == (HANDLE)0xffffffff) {
    printf("Path not found: [%s]\n",folder_name);
  }
  else {
    local_10 = 1;
    local_11 = true;
    while (local_11 != false) {
      iVar1 = strcmp(local_158.cFileName,".");
      if ((iVar1 != 0) && (iVar1 = strcmp(local_158.cFileName,".."), iVar1 != 0)) {
        sprintf(local_958,"%s\\%s",folder_name,local_158.cFileName);
        if ((local_158.dwFileAttributes & 0x10) == 0) {
          if (local_10 == 1) {
            if (param_1 == 0) {
              _hideInFile(local_958);
            }
            else {
              if (param_1 == 1) {
                _decodeBytes(local_958);
              }
            }
          }
          local_10 = 1 - local_10;
        }
        else {
          printf("Folder: %s\n",local_958);
          _listdir(param_1,local_958);
        }
      }
      if (_isOver != '\0') break;
      BVar2 = FindNextFileA(local_18,(LPWIN32_FIND_DATAA)&local_158);
      local_11 = BVar2 != 0;
    }
    FindClose(local_18);
  }
  return;
}
```

It looks like `_listdir` is a recursive function which iterates over files in the folder (in our case it was called with the folder "`./test`" by the main function) and calls `_hideInFile` for every other file... assuming `param_1` is set to `0`. If `param_1` is set to `1`, an interesting function called `_decodeBytes` is called on the file instead.

Who calls `_listdir` with `param_1 = 1`? An interesting uncalled function named `_decode`:

```c
void _decode(void)
{
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  
  local_24 = 1634496360;
  local_20 = 2105376;
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  _buff_size = 0x12;
  local_28 = 0;
  local_10 = 1;
  _buff = &local_24;
  _buff_index = &local_28;
  _listdir(1,_folderName);
  printf("value of DECODE %s \n",_buff);
  puts("Wait for 5 seconds to exit.");
  _sleep(5);
                    /* WARNING: Subroutine does not return */
  exit(0);
}
```

This function relies on the `_folderName` global and seems very promising. We can call it independently using the debugger after `_folderName` gets initialized in the main function.

Using `x64dbg`, we launch the program and execute:

```
> SetBPX 00401bda
```

This will set a breakpoint in main, right after initializing _folderName:
```assembly
        00401bc9 8d 44 24 48     LEA        EAX=>local_28,[ESP + 0x48]
        00401bcd a3 34 54        MOV        [_folderName],EAX                                = ??
                 40 00
        00401bd2 c7 44 24        MOV        dword ptr [ESP + local_14],0x0
                 5c 00 00 
                 00 00
        00401bda c7 05 48        MOV        dword ptr [_pLevel],0x31                         = ??
                 54 40 00 
                 31 00 00 00

```

We run and hit the breakpoint. Now we just need to change `eip` to `_decode` and run:
```
> EIP=00401afe
```

Now we just need to run, and we get the flag:
```
value of DECODE picoCTF{M4cTim35!}
Wait for 5 seconds to exit.
```

So where did the flag hide?

Let's take a look at the `_hideInFile` function:

```c
void __cdecl _hideInFile(LPCSTR param_1)

{
  BOOL BVar1;
  _FILETIME local_2c;
  _FILETIME local_24;
  _FILETIME local_1c;
  char local_12;
  char local_11;
  HANDLE local_10;
  
  local_10 = CreateFileA(param_1,0x100,0,(LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
  .text(local_10);
  if (local_10 == (HANDLE)0xffffffff) {
    printf("Error:INVALID_HANDLED_VALUE");
  }
  else {
    BVar1 = GetFileTime(local_10,(LPFILETIME)&local_1c,(LPFILETIME)&local_24,(LPFILETIME)&local_2c);
    if (BVar1 == 0) {
      printf("Error: C-GFT-01");
    }
    else {
      local_11 = *(char *)(*_flag_index + _flag);
      *_flag_index = *_flag_index + 1;
      local_12 = *(char *)(*_flag_index + _flag);
      *_flag_index = *_flag_index + 1;
      _encodeBytes(local_11,local_12,(uint *)&local_2c);
      if (0 < _pLevel) {
        local_11 = *(char *)(*_flag_index + _flag);
        *_flag_index = *_flag_index + 1;
        local_12 = *(char *)(*_flag_index + _flag);
        *_flag_index = *_flag_index + 1;
        _encodeBytes(local_11,local_12,(uint *)&local_1c);
      }
      if (_pLevel == 2) {
        local_11 = *(char *)(*_flag_index + _flag);
        *_flag_index = *_flag_index + 1;
        local_12 = *(char *)(*_flag_index + _flag);
        *_flag_index = *_flag_index + 1;
        _encodeBytes(local_11,local_12,(uint *)&local_24);
      }
      BVar1 = SetFileTime(local_10,(FILETIME *)&local_1c,(FILETIME *)&local_24,(FILETIME *)&local_2c
                         );
      if (BVar1 == 0) {
        printf("Error: C-SFT-01");
      }
      else {
        if (_flag_size <= *_flag_index) {
          _isOver = 1;
        }
        CloseHandle(local_10);
      }
    }
  }
  return;
}
```

We can see that the function is using `GetFileTime` and `SetFileTime`, and this also explains the challenge name ([MAC](https://en.wikipedia.org/wiki/MAC_times) stands for "Modification, Access, Creation"). The Windows user interface shows us the date and H:M:S, but NTFS file systems have a resolution of 100 Nanoseconds for these fields (The `FILETIME` structure represents the time in 100-nanosecond intervals since January 1, 1601).

This also explains why some files had an asterisk in the `WinMerge` report, even though the date didn't seem different:
```
Item05 - Copy.bmp	  Binary files are identical				        3/26/2019 2:21:28 AM				* 3/26/2019 2:21:28 AM				bmp
```

Now, if we want to create a script to extract the flag, we won't be able to use any standard Python APIs since their resolution isn't good enough:

```python
>>> os.stat("Item01 - Copy.bmp")
os.stat_result(st_mode=33206, st_ino=562949953448601, st_dev=1009486881, st_nlink=1, st_uid=0, st_gid=0, st_size=127654, st_atime=1554572121, st_mtime=1553556008, st_ctime=1554505885)
>>> os.stat("Item01 - Copy.bmp").st_mtime
1553556008.0027752
>>> os.path.getmtime("Item01 - Copy.bmp")
1553556008.0027752
```

We can either write a `C` program that uses `WinAPI` or use the Python `ctypes` module to call Windows API functions directly.

We just need to notice that the program sets `_pLevel` to `0` (so that only the file modification time is used to hide the flag) and that the `_encodeBytes` logic is simply:
```c
void __cdecl _encodeBytes(char param_1,char param_2,uint *param_3)
{
  *param_3 = (*param_3 & 0xffff0000) + (int)param_2 + (int)param_1 * 0x100;
  return;
}
```

Meaning, every two bytes of the flag are encoded as the two least significant bytes of the modification time of a file.

Here's how to extract the flag with Python:

```python
from ctypes import windll, Structure, byref
from ctypes.wintypes import LPWSTR, DWORD, FILETIME
import os, glob 

class WIN32_FILE_ATTRIBUTE_DATA(Structure):
    _fields_ = [("dwFileAttributes", DWORD),
                ("ftCreationTime", FILETIME),
                ("ftLastAccessTime", FILETIME),
                ("ftLastWriteTime", FILETIME),
                ("nFileSizeHigh", DWORD),
                ("nFileSizeLow", DWORD)]

# https://stackoverflow.com/questions/38508351/getting-last-change-time-in-python-on-windows
def get_mtime(filename):
    wfad = WIN32_FILE_ATTRIBUTE_DATA()
    GetFileExInfoStandard = 0
    windll.kernel32.GetFileAttributesExW(LPWSTR(filename), GetFileExInfoStandard, byref(wfad))
    lowtime = long(wfad.ftLastWriteTime.dwLowDateTime)
    hightime = long(wfad.ftLastWriteTime.dwHighDateTime)
    filetime = (hightime << 32) + lowtime
    return filetime

skip = True
flag = ""
for path in glob.iglob('test/*.bmp'):
    skip = not skip
    if skip:
        continue
    time = get_mtime(path)
    print "{}: \t{}".format(path, time)
    flag += chr((time & 0xff00) >> 8)
    flag += chr(time & 0xff)

print flag

```

Output:
```
test\Item01 - Copy.bmp:         131980296080027753
test\Item02 - Copy.bmp:         131980296340005743
test\Item03 - Copy.bmp:         131980296509997908
test\Item04 - Copy.bmp:         131980296730003067
test\Item05 - Copy.bmp:         131980296889978164
test\Item06 - Copy.bmp:         131980298579960660
test\Item07 - Copy.bmp:         131980298870024557
test\Item08 - Copy.bmp:         131980299550012213
test\ItemTest - Copy.bmp:       131980329319997821
picoCTF{M4cTim35!}
```