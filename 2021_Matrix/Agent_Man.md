# Agent Man
Category: Mobile, 200 Points

## Description

> 
> --- TRANSMISSION START---
> 
> Special Agent;
> ...
> Exciting news!
> 
> The wire is bustling with chatter about 'little green men' and how they were able to make their presence felt across the globe.
> 
> Your mission is to locate the group and find a way to uncover their secrets. 
> 
> Attached is a sample gathered by our brave field-agents, best of luck;
> 
> --- TRANSMISSION END ---

An apk was attached.

## Solution

Let's extract the APK:

```console
root@kali:/media/sf_CTFs/matrix/Agent_Man# ~/utils/jadx/build/jadx/bin/jadx -d output agentman.apk
INFO  - loading ...
INFO  - processing ...
ERROR - finished with errors, count: 1
```

We get lots of files, but the interesting ones are here:
```console
root@kali:/media/sf_CTFs/matrix/Agent_Man/output/sources/com/jagent# tree
.
└── javaagent
    ├── BuildConfig.java
    ├── DownloadFile.java
    ├── MainActivity.java
    └── R.java

1 directory, 4 files
```

`MainActivity.java` contains:

```java
package com.jagent.javaagent;

import android.os.Bundle;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
public class MainActivity extends AppCompatActivity {
    public native String agentMan(String str);

    static {
        System.loadLibrary("agent");
    }

    /* access modifiers changed from: protected */
    @Override // androidx.appcompat.app.AppCompatActivity, androidx.fragment.app.FragmentActivity, androidx.activity.ComponentActivity, androidx.core.app.ComponentActivity, android.app.Activity
    public void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        setContentView(R.layout.activity_main);
        TextView textView = (TextView) findViewById(R.id.mainText);
        String str = new DownloadFile().get("https://ctfroom.matrixcyberlabs.com/agent");
        if (str.isEmpty()) {
            textView.setText(agentMan(null));
        } else {
            textView.setText(agentMan(str));
        }
    }
}
```

The main function downloads a file from a remote server, then calls `agentMan` and sets the text on the screen based on the return value of this function.

`DownloadFile.java` is simply:

```java
package com.jagent.javaagent;
public class DownloadFile {
    /* access modifiers changed from: package-private */
    public String get(String str) {
        return "NULL";
    }
}
```

So given this implementation, we can see that `DownloadFile` will return the string `"NULL"` which will get assigned to `str`. Therefore, `agentMan("NULL")` will be called and its return value will be displayed on the screen. What will it display? We can run the application in an emulator and see:

![](images/agent_man1.png)

Well, it just prints "Nope". But where is the implementation? We can see that `agentMan` is declared as `native`:

```java
public native String agentMan(String str);
```

This means that we can find it under one of the native libraries of the application:
```console
root@kali:/media/sf_CTFs/matrix/Agent_Man/output# grep -r agentMan
Binary file resources/classes.dex matches
Binary file resources/lib/arm64-v8a/libagent.so matches
Binary file resources/lib/armeabi-v7a/libagent.so matches
Binary file resources/lib/x86/libagent.so matches
Binary file resources/lib/x86_64/libagent.so matches
sources/com/jagent/javaagent/MainActivity.java:    public native String agentMan(String str);
sources/com/jagent/javaagent/MainActivity.java:            textView.setText(agentMan(null));
sources/com/jagent/javaagent/MainActivity.java:            textView.setText(agentMan(str));
```

We'll choose `x86/libagent.so`, let's open it in Ghidra:

```c
undefined4
Java_com_jagent_javaagent_MainActivity_agentMan
          (_JNIEnv *param_1,undefined4 param_2,_jstring *param_3)

{
  char *pcVar1;
  char *pcVar2;
  int in_GS_OFFSET;
  int local_144;
  undefined4 local_130;
  basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>> local_11c [16];
  int local_10c;
  int local_108;
  basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>> local_104 [16];
  basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>> local_f4 [14];
  byte local_e6 [42];
  undefined4 local_bc [42];
  int local_14;
  
  local_14 = *(int *)(in_GS_OFFSET + 0x14);
  pcVar1 = (char *)_JNIEnv::GetStringUTFChars(param_1,param_3,(uchar *)0x0);
  std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
  basic_string<decltype(nullptr)>(local_f4,"NULL");
  if (pcVar1 != (char *)0x0) {
    pcVar2 = (char *)FUN_00018780(local_f4);
    pcVar2 = strstr(pcVar1,pcVar2);
    if (pcVar2 == (char *)0x0) {
      local_108 = 0x1ce9;
      memcpy(local_bc,&DAT_000387f4,0xa8);
      memset(local_e6,0,0x2a);
      local_10c = (int)((int)&local_14 - (int)local_bc) >> 2;
      local_144 = 0;
      while (local_144 < local_10c) {
        local_e6[local_144] = (byte)local_bc[local_144] ^ pcVar1[(local_144 + 2) * local_108];
        local_144 = local_144 + 1;
      }
                    /* try { // try from 00018542 to 00018561 has its CatchHandler @ 00018420 */
      std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
      basic_string<decltype(nullptr)>(local_11c,"Secret, Agent Man!");
                    /* try { // try from 00018573 to 000185c2 has its CatchHandler @ 000185fd */
      _JNIEnv::ReleaseStringUTFChars(param_1,param_3,pcVar1);
      pcVar1 = (char *)FUN_00018780(local_11c);
      local_130 = _JNIEnv::NewStringUTF(param_1,pcVar1);
      std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
      ~basic_string(local_11c);
      goto LAB_00018622;
    }
  }
                    /* try { // try from 0001838b to 000183aa has its CatchHandler @ 00018420 */
  std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
  basic_string<decltype(nullptr)>(local_104,"Nope");
  pcVar1 = (char *)FUN_00018780(local_104);
                    /* try { // try from 000183ce to 000183e5 has its CatchHandler @ 00018431 */
  local_130 = _JNIEnv::NewStringUTF(param_1,pcVar1);
  std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
  ~basic_string(local_104);
LAB_00018622:
  std::__ndk1::basic_string<char,std--__ndk1--char_traits<char>,std--__ndk1--allocator<char>>::
  ~basic_string(local_f4);
  if (*(int *)(in_GS_OFFSET + 0x14) == local_14) {
    return local_130;
  }
                    /* WARNING: Subroutine does not return */
  __stack_chk_fail();
}
```

We can see the "Nope" here:

```c
basic_string<decltype(nullptr)>(local_104,"Nope");
```

We also see another string that can get printed:

```c
basic_string<decltype(nullptr)>(local_11c,"Secret, Agent Man!");
```

But what's most interesting is this code, which seems to perform some kind of basic XOR decryption:

```c
while (local_144 < local_10c) {
    local_e6[local_144] = (byte)local_bc[local_144] ^ pcVar1[(local_144 + 2) * local_108];
    local_144 = local_144 + 1;
}
```

The XOR is between `pcVar1` which seems to come from the function argument (i.e. what `DownloadFile().get` returned) and `local_bc` which essentially comes from a global array - `DAT_000387f4`:

```assembly
                             DAT_000387f4                                    XREF[2]:     Java_com_jagent_javaagent_MainAc
                                                                                          Java_com_jagent_javaagent_MainAc
        000387f4 a5              ??         A5h
        000387f5 00              ??         00h
        000387f6 00              ??         00h
        000387f7 00              ??         00h
        000387f8 bc              ??         BCh
        000387f9 00              ??         00h
        000387fa 00              ??         00h
        000387fb 00              ??         00h
        000387fc bc              ??         BCh
        000387fd 00              ??         00h
        000387fe 00              ??         00h
        000387ff 00              ??         00h
        00038800 a1              ??         A1h
        00038801 00              ??         00h
        00038802 00              ??         00h
        00038803 00              ??         00h
        ; ...
        00038894 e1              ??         E1h
        00038895 00              ??         00h
        00038896 00              ??         00h
        00038897 00              ??         00h
        00038898 55              ??         55h    U
        00038899 00              ??         00h
        0003889a 00              ??         00h
        0003889b 00              ??         00h

```

Well, right now we don't execute this code at all, since `DownloadFile().get` returns the string `"NULL"`, and the code explicitly checks for this (via `strstr`) and skips to the part that prints `"Nope"`. But, if we return something else from `DownloadFile().get`, we will execute the decryption code. What should it return? The answer is probably the file it was requested to download. In our case, `https://ctfroom.matrixcyberlabs.com/agent`.

```console
root@kali:/media/sf_CTFs/matrix/Agent_Man# curl https://ctfroom.matrixcyberlabs.com/agent
<!DOCTYPE html>
<html>
  <head>
        <style type="text/css">
        #bg {
            background: no-repeat center center fixed;
            -webkit-background-size: cover;
            -moz-background-size: cover;
            -o-background-size: cover;
            background-size: cover;
            width:100%;
        height:100%;
        }
        </style>
    <title>Matrix Cyber-Labs CTF</title>
<!-- Google Tag Manager -->
<script>(function(w,d,s,l,i){w[l]=w[l]||[];w[l].push({'gtm.start':
new Date().getTime(),event:'gtm.js'});var f=d.getElementsByTagName(s)[0],
j=d.createElement(s),dl=l!='dataLayer'?'&l='+l:'';j.async=true;j.src=
'https://www.googletagmanager.com/gtm.js?id='+i+dl;f.parentNode.insertBefore(j,f);
})(window,document,'script','dataLayer','GTM-WCPGJ25');</script>
<!-- End Google Tag Manager -->
  </head>

  <body>
      <div style="text-align: center">
        <img src="/static/bc3403beec4074fb96466a557dfb7e53/digDeeper.jpg" id="bg"/>
<!-- Google Tag Manager (noscript) -->
<noscript><iframe src="https://www.googletagmanager.com/ns.html?id=GTM-WCPGJ25"
height="0" width="0" style="display:none;visibility:hidden"></iframe></noscript>
<!-- End Google Tag Manager (noscript) -->
      </div>
  </body>
</html>
```

We can see that this is a simple HTML page with a single image in it. Now, we can choose to proceed in two ways:

 * Implement `DownloadFile().get` (using [frida](https://frida.re/) for example) by having it download the file and return it, then run the application in an emulator and see what happens
 * Download the file, extract the decryption code and decrypt the message offline

The second option seems simple enough, we'll go for it.

```console
root@kali:/media/sf_CTFs/matrix/Agent_Man# wget https://ctfroom.matrixcyberlabs.com/static/bc3403beec4074fb96466a557dfb7e53/digDeeper.jpg
--2021-03-02 23:06:59--  https://ctfroom.matrixcyberlabs.com/static/bc3403beec4074fb96466a557dfb7e53/digDeeper.jpg
Resolving ctfroom.matrixcyberlabs.com (ctfroom.matrixcyberlabs.com)... 89.237.91.77
Connecting to ctfroom.matrixcyberlabs.com (ctfroom.matrixcyberlabs.com)|89.237.91.77|:443... connected.
HTTP request sent, awaiting response... 200 OK
Length: 1610554 (1.5M) [image/jpeg]
Saving to: ‘digDeeper.jpg’

digDeeper.jpg                                       100%[==================================================================================================================>]   1.54M   388KB/s    in 4.1s

2021-03-02 23:07:03 (380 KB/s) - ‘digDeeper.jpg’ saved [1610554/1610554]
```

We also need the global array, `radare2` can print it in Python syntax:

```
[0x00008270]> pcp 0xa8 @ 0x287f4
import struct
buf = struct.pack ("168B", *[
0xa5,0x00,0x00,0x00,0xbc,0x00,0x00,0x00,0xbc,0x00,0x00,
0x00,0xa1,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x6c,0x00,
0x00,0x00,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,
0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,
0x42,0x00,0x00,0x00,0x2d,0x00,0x00,0x00,0x28,0x00,0x00,
0x00,0xdb,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc0,0x00,
0x00,0x00,0x27,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x29,
0x00,0x00,0x00,0x41,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x19,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x44,0x00,0x00,
0x00,0x8b,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x2f,0x00,
0x00,0x00,0x27,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x03,
0x00,0x00,0x00,0xb2,0x00,0x00,0x00,0x3d,0x00,0x00,0x00,
0xf3,0x00,0x00,0x00,0xed,0x00,0x00,0x00,0x14,0x00,0x00,
0x00,0x15,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x2b,0x00,
0x00,0x00,0xd6,0x00,0x00,0x00,0xe1,0x00,0x00,0x00,0x55,
0x00,0x00,0x00])
```

All that's left is to XOR the values according to the recipe:

```python
import mmap, struct, os

def memory_map(filename, access=mmap.ACCESS_READ):
    size = os.path.getsize(filename)
    fd = os.open(filename, os.O_RDONLY)
    return mmap.mmap(fd, size, access=access)

buf = struct.pack ("168B", *[
0xa5,0x00,0x00,0x00,0xbc,0x00,0x00,0x00,0xbc,0x00,0x00,
0x00,0xa1,0x00,0x00,0x00,0x5c,0x00,0x00,0x00,0x6c,0x00,
0x00,0x00,0xdc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3c,
0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x9c,0x00,0x00,0x00,
0x42,0x00,0x00,0x00,0x2d,0x00,0x00,0x00,0x28,0x00,0x00,
0x00,0xdb,0x00,0x00,0x00,0xc8,0x00,0x00,0x00,0xc0,0x00,
0x00,0x00,0x27,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x29,
0x00,0x00,0x00,0x41,0x00,0x00,0x00,0x08,0x00,0x00,0x00,
0x19,0x00,0x00,0x00,0xc0,0x00,0x00,0x00,0x44,0x00,0x00,
0x00,0x8b,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x2f,0x00,
0x00,0x00,0x27,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x03,
0x00,0x00,0x00,0xb2,0x00,0x00,0x00,0x3d,0x00,0x00,0x00,
0xf3,0x00,0x00,0x00,0xed,0x00,0x00,0x00,0x14,0x00,0x00,
0x00,0x15,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x2b,0x00,
0x00,0x00,0xd6,0x00,0x00,0x00,0xe1,0x00,0x00,0x00,0x55,
0x00,0x00,0x00])

# Unpack as little endian integers
local_bc = struct.unpack(f"<{str(len(buf)//4)}I", buf)

flag = ""

with memory_map("digDeeper.jpg") as pcVar1:
    local_108 = 0x1ce9
    for i, x in enumerate(local_bc):
        flag += chr(local_bc[i] ^ pcVar1[(i + 2) * local_108])

print(flag)
```

Output:

```console
root@kali:/media/sf_CTFs/matrix/Agent_Man# python3 solve.py
MCL{li7tl3_5P3c1al_S3crET_Ag3n7_'\'m3n'\'}
```