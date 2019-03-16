# Secrets
Android, 376 points

## Description:

> Can you find my secrets?

An APK file was attached.

## Solution:

First, I decomposed the APK file using `apktools`:

```console
root@kali:/media/sf_CTFs/tamu/_Secrets# apktool d howdyapp.apk
I: Using Apktool 2.3.4-dirty on howdyapp.apk
I: Loading resource table...
I: Decoding AndroidManifest.xml with resources...
I: Loading resource table from file: /root/.local/share/apktool/framework/1.apk
I: Regular manifest package...
I: Decoding file-resources...
I: Decoding values */* XMLs...
I: Baksmaling classes.dex...
I: Copying assets and libs...
I: Copying unknown files...
I: Copying original files...
```

Then I searched for the flag format - `gigem` - across all files:

```console
root@kali:/media/sf_CTFs/tamu/_Secrets# grep -rnw gigem
howdyapp/res/layout/activity_main.xml:5:    <TextView n1:id="@id/count" n1:layout_width="wrap_content" n1:layout_height="wrap_content" n1:text="@string/initial_count" n1:layout_toLeftOf="@id/gigem" n1:layout_below="@id/button" />
howdyapp/res/layout/activity_main.xml:6:    <TextView n1:id="@id/gigem" n1:layout_width="wrap_content" n1:layout_height="wrap_content" n1:text=" gig 'ems" n1:layout_below="@id/button" n1:layout_centerHorizontal="true" />
howdyapp/res/values/ids.xml:35:    <item type="id" name="gigem">false</item>
howdyapp/res/values/public.xml:592:    <public type="id" name="gigem" id="0x7f07002b" />
howdyapp/smali/com/tamu/ctf/howdyapp/R$id.smali:104:.field public static final gigem:I = 0x7f07002b
Binary file howdyapp.apk matches
```

Nothing seemed useful, so I searched for `flag`:
```console
root@kali:/media/sf_CTFs/tamu/_Secrets# grep -rnw flag
howdyapp/res/values/attrs.xml:76:        <flag name="bottom" value="0x00000050" />
howdyapp/res/values/attrs.xml:77:        <flag name="top" value="0x00000030" />
howdyapp/res/values/attrs.xml:123:        <flag name="disableHome" value="0x00000020" />
howdyapp/res/values/attrs.xml:124:        <flag name="homeAsUp" value="0x00000004" />
howdyapp/res/values/attrs.xml:125:        <flag name="none" value="0x00000000" />
howdyapp/res/values/attrs.xml:126:        <flag name="showCustom" value="0x00000010" />
howdyapp/res/values/attrs.xml:127:        <flag name="showHome" value="0x00000002" />
howdyapp/res/values/attrs.xml:128:        <flag name="showTitle" value="0x00000008" />
howdyapp/res/values/attrs.xml:129:        <flag name="useLogo" value="0x00000001" />
howdyapp/res/values/attrs.xml:262:        <flag name="barrier" value="0x00000002" />
howdyapp/res/values/attrs.xml:263:        <flag name="chains" value="0x00000004" />
howdyapp/res/values/attrs.xml:264:        <flag name="dimensions" value="0x00000008" />
howdyapp/res/values/attrs.xml:265:        <flag name="direct" value="0x00000001" />
howdyapp/res/values/attrs.xml:266:        <flag name="none" value="0x00000000" />
howdyapp/res/values/attrs.xml:267:        <flag name="standard" value="0x00000003" />
howdyapp/res/values/attrs.xml:317:        <flag name="always" value="0x00000002" />
howdyapp/res/values/attrs.xml:318:        <flag name="collapseActionView" value="0x00000008" />
howdyapp/res/values/attrs.xml:319:        <flag name="ifRoom" value="0x00000001" />
howdyapp/res/values/attrs.xml:320:        <flag name="never" value="0x00000000" />
howdyapp/res/values/attrs.xml:321:        <flag name="withText" value="0x00000004" />
howdyapp/res/values/attrs.xml:324:        <flag name="beginning" value="0x00000001" />
howdyapp/res/values/attrs.xml:325:        <flag name="end" value="0x00000004" />
howdyapp/res/values/attrs.xml:326:        <flag name="middle" value="0x00000002" />
howdyapp/res/values/attrs.xml:327:        <flag name="none" value="0x00000000" />
howdyapp/res/values/public.xml:743:    <public type="string" name="flag" id="0x7f0b0020" />
howdyapp/res/values/strings.xml:35:    <string name="flag">Z2lnZW17aW5maW5pdGVfZ2lnZW1zfQ==</string>
howdyapp/smali/android/support/annotation/IntDef.smali:12:        flag = false
howdyapp/smali/android/support/annotation/IntDef.smali:29:.method public abstract flag()Z
howdyapp/smali/android/support/v4/accessibilityservice/AccessibilityServiceInfoCompat.smali:301:    .param p0, "flag"    # I
howdyapp/smali/com/tamu/ctf/howdyapp/R$string.smali:82:.field public static final flag:I = 0x7f0b0020
Binary file howdyapp.apk matches
```

The following string stood out:
```
howdyapp/res/values/strings.xml:35:    <string name="flag">Z2lnZW17aW5maW5pdGVfZ2lnZW1zfQ==</string>
```

Decoded as base64, it gave the flag:
```console
root@kali:/media/sf_CTFs/tamu/_Secrets# echo Z2lnZW17aW5maW5pdGVfZ2lnZW1zfQ== | base64 -d
gigem{infinite_gigems}
```