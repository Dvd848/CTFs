# Johannesburg - Hideout
Category: Reversing

## Description

> Johannesburg is hot, and you are on your way to the secret lair. It seems like it is well fortified, even though you are expected under the alias of the assassin, perhaps it will be a better idea to sneak inside the lair, unseen? You climb up on a brick wall, and jump over it. On the other side you spot a lot of guards, quick, hide in a bush. Now you have to sneak past the guards into the main building's entrance.
> 
> Challenge: Konski-Hiakawa Law of Droids (reversing)
> 
> In this challenge, you have to find the flag using memory forensics. Good luck! Note, surround the flag with CTF{...} to submit it. Note, API Level 25 is what you're looking for.

Two files were attached: `bzImage` and `gCTF.apk`.

## Solution

We start by extracting the `apk` file and poking around:

```console
┌──(user@kali)-[/media/sf_CTFs/google/9_Johannesburg_-_Hideout]
└─$ ~/utils/android/jadx/build/jadx/bin/jadx -d output gCTF.apk
INFO  - loading ...
INFO  - processing ...
ERROR - finished with errors, count: 1
```

We get two interesting files. 

`MainActivity.java`:

```java
package gctf.researchctf;


import android.app.Activity;

import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;


import java.io.File;
import java.io.FileOutputStream;

import static android.os.Environment.DIRECTORY_DOWNLOADS;

public class MainActivity extends Activity {
    private EditText tv = null;
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);
        try{
            File bf = new File(Environment.getExternalStoragePublicDirectory(DIRECTORY_DOWNLOADS), "Raven.txt");
            File f = new File(getApplicationContext().getFilesDir(), "SusanSue.txt");
            FileOutputStream fos = new FileOutputStream(f);
            fos.write("Sctf:key:".getBytes());
            fos.close();
            FileOutputStream bkfos = new FileOutputStream(bf);
            bkfos.write("BKUP:KEY:SecretKey:P@$$".getBytes());
            bkfos.close();

        }catch(Exception e){
            e.printStackTrace();
        }
        tv = findViewById(R.id.editTextTextPersonName);
        tv.setText(stringFromJNI());
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native String getCTF(String enteredKey);

    public native int validateCTF(String enteredKey);

    public void read(View view) {
        try{
            File f = new File(getApplicationContext().getFilesDir(), "SusanSue.txt");
            int s = validateCTF(tv.getText().toString());
            if(s == 0){
                Log.v("WATCHOUT", "You are the winner");
                Toast.makeText(getApplicationContext(), "YOU ARE THE WINNER!!!!", Toast.LENGTH_LONG).show();
            }else{
                Log.v("WATCHOUT", "Please keep trying");
                Toast.makeText(getApplicationContext(), "Please keep trying!!!!", Toast.LENGTH_LONG).show();
            }
        }catch (Exception e){
            e.printStackTrace();
        }

    }
}
```

`native-lib.cpp`:

```cpp
#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_gctf_researchctf_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    FILE* tgt_filez_handle = NULL;

    //tgt_filez_handle = fopen(reinterpret_cast<const char *>(tgt_filez), "w");
    tgt_filez_handle = fopen("/data/data/gctf.researchctf/files/SusanSue.txt", "w");
    const char s[] = {'g', 'C', 'T', 'F', ':', 'K', 'E', 'Y', ':', '\0'};
    fputs(s, tgt_filez_handle);
    fclose(tgt_filez_handle);
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_gctf_researchctf_MainActivity_getCTF(JNIEnv *env, jobject thiz, jstring entered_key) {
    FILE* tgt_file_handle = fopen("/data/data/gctf.researchctf/files/SusanSue.txt", "r");
    char *content = static_cast<char *>(calloc(256, sizeof(char)));

    //fgets(content, 256, tgt_file_handle);
    fscanf(tgt_file_handle, "%s", content);
    fclose(tgt_file_handle);

    return env->NewStringUTF(content);
}

extern "C"
JNIEXPORT jint JNICALL
Java_gctf_researchctf_MainActivity_validateCTF(JNIEnv *env, jobject thiz, jstring entered_key) {
    FILE* tgt_file_handle = fopen("/data/data/gctf.researchctf/files/SusanSue.txt", "r");
    char *content = static_cast<char *>(calloc(10, sizeof(char)));

    fgets(content, 10, tgt_file_handle);
    //fscanf(tgt_file_handle, "%s", content);
    fclose(tgt_file_handle);



    const char* ch = env->GetStringUTFChars(entered_key, 0);

    env->ReleaseStringUTFChars(entered_key, ch);
    //__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "entered <%s>", reinterpret_cast<const char *>(ch));
    //printf("<%s>", ch);
    //__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "CONTENT <%s>", reinterpret_cast<const char *>(content));
    //printf("<%s>", content);
    int comp = strcmp(content, reinterpret_cast<const char *>(ch));
    //printf("<%d>", comp);
    return comp;
}
```

Assuming we want to arrive to `Log.v("WATCHOUT", "You are the winner")`, we need `validateCTF` to return `0`. So, we inspect the code for `Java_gctf_researchctf_MainActivity_validateCTF` and see that we need to return the `10` characters which are saved in `/data/data/gctf.researchctf/files/SusanSue.txt` so that `strcmp` will return `0`. `SusanSue.txt` is set by `onCreate` to `Sctf:key:`, then by `stringFromJNI` to `gCTF:KEY:`. However, none are accepted as the flag, so what's going on?

To answer that, we need to inspect `bzImage`, which is a format that contains a compressed `vmlinux` kernel. We'll use [vmlinux-to-elf](https://github.com/marin-m/vmlinux-to-elf) to extract it and obtain debug symbols.

```console
┌──(user@kali)-[~/CTFs/google/9_Johannesburg_-_Hideout]
└─$ ~/utils/reversing/vmlinux-to-elf/vmlinux-to-elf bzImage outputKernel.elf
[+] Kernel successfully decompressed in-memory (the offsets that follow will be given relative to the decompressed binary)
[+] Version string: Linux version 3.10.0+ (vldinesh@lab) (gcc version 4.9.x 20150123 (prerelease) (GCC) ) #31 SMP PREEMPT Sun Jul 4 11:14:54 IST 2021
[+] Guessed architecture: x86_64 successfully in 2.68 seconds
[+] Found kallsyms_token_table at file offset 0x00bb5bd0
[+] Found kallsyms_token_index at file offset 0x00bb5f28
[+] Found kallsyms_markers at file offset 0x00bb57c8
[+] Found kallsyms_names at file offset 0x00b56348
[+] Found kallsyms_num_syms at file offset 0x00b56340
[i] Null addresses overall: 0.0060983 %
[+] Found kallsyms_addresses at file offset 0x00b16260
[+] Successfully wrote the new ELF kernel to outputKernel.elf
```


Now we can open the ELF in Ghidra. Searching for interesting strings, we find:

```c
undefined8
sys_write(ulong param_1,char *param_2,ulong *param_3,undefined8 param_4,ulong *param_5,
         long **param_6)
{
  ulong *puVar1;
  char *pcVar2;
  undefined8 uVar3;
  ulong **ppuVar4;
  long in_GS_OFFSET;
  undefined auVar5 [16];
  ulong *puStack48;
  
  auVar5 = fget_light(param_1,(undefined4 *)&puStack48,param_3,param_4,param_5);
  puVar1 = SUB168(auVar5,0);
  pcVar2 = strstr((char *)(*(long *)(&DAT_0000a7c0 + in_GS_OFFSET) + 0x5c0),"researchctf");
  if ((pcVar2 != (char *)0x0) && (param_2 != (char *)0x0)) {
    pcVar2 = strstr(param_2,"gCTF:KEY:");
    if (pcVar2 != (char *)0x0) {
      param_3 = (ulong *)&DAT_00000009;
      strcpy(param_2,"SB:575756");
    }
  }
  uVar3 = 0xfffffffffffffff7;
  if (puVar1 != (ulong *)0x0) {
    puStack48 = (ulong *)puVar1[9];
    ppuVar4 = &puStack48;
    auVar5 = vfs_write((ulong)puVar1,(ulong)param_2,param_3,ppuVar4,param_5,param_6);
    uVar3 = SUB168(auVar5,0);
    puVar1[9] = (ulong)puStack48;
    if ((int)puStack48 != 0) {
      fput(puVar1,param_2,puStack48,ppuVar4,(ulong)param_5,param_6);
    }
  }
  return uVar3;
}
```

It looks like the kernel is hooking `sys_write`, and checking if `researchctf` and `gCTF:KEY:` are contained within the syscall. If so, the content is replaced with `SB:575756`. This means that our flag is in fact `CTF{SB:575756}`.