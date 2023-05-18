
# Guns And Roses Concert Coupon

* Category: Mobile
* 200 points

## Description

> Everyone gives discounts for the Guns N' Roses concert.  
> Would you like a brainer ticket?  
> Just enter the right coupon.

An APK file was attached.

## Solution

Let's extract the APK file with ``:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Guns_And_Roses_Concert_Coupon]
└─$ /home/user/utils/reversing/jadx/bin/jadx -d output app-release.apk
INFO  - loading ...
INFO  - processing ...
ERROR - finished with errors, count: 2
```

The mail logic of the application can be found under `output/sources/com/challenge/gnr`:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Guns_And_Roses_Concert_Coupon]
└─$ ls output/sources/com/challenge/gnr
FirstFragment.java  MainActivity.java  R.java  SecondFragment.java
```

Let's take a look at the `onCreate` method of the main activity:

```java
public final void onCreate(Bundle bundle) {
    SharedPreferences.Editor edit = getApplicationContext().getSharedPreferences("ddjxd", 0).edit();
    edit.putString(getString(R.string.gkgkkdkdd), getString(R.string.dgsjfjsds));
    edit.commit();
    super.onCreate(bundle);
    View inflate = getLayoutInflater().inflate(R.layout.activity_main, (ViewGroup) null, false);
    Toolbar toolbar = (Toolbar) a0.w(inflate, R.id.toolbar);
    if (toolbar != null) {
        CoordinatorLayout coordinatorLayout = (CoordinatorLayout) inflate;
        this.f1978q = new m(coordinatorLayout, toolbar);
        setContentView(coordinatorLayout);
        p().v((Toolbar) this.f1978q.f683b);
        i a4 = t0.a0.a(this);
        s h3 = a4.h();
        HashSet hashSet = new HashSet();
        int i3 = s.f3583q;
        hashSet.add(Integer.valueOf(s.a.a(h3).f3577j));
        b bVar = new b(hashSet);
        this.f1977p = bVar;
        a aVar = new a(this, bVar);
        a4.f3526p.add(aVar);
        c<f> cVar = a4.f3517g;
        if (!cVar.isEmpty()) {
            f last = cVar.last();
            aVar.a(a4, last.f3497d, last.f3498e);
        }
        SQLiteDatabase writableDatabase = new d1.a(getApplicationContext()).getWritableDatabase();
        ContentValues contentValues = new ContentValues();
        contentValues.put("title", "welcometothejungle");
        contentValues.put("subtitle", "dljJDNHodV4wd1znNjIW8h86++7TA/zlohduLouuD0PMz0pU7xDFdw==");
        writableDatabase.insert("entry", null, contentValues);
        return;
    }
    throw new NullPointerException("Missing required view with ID: ".concat(inflate.getResources().getResourceName(R.id.toolbar)));
}
```

It's not very clear what happens here, but the base64 string does leap to the eye. We also notice strange string references such as `R.string.gkgkkdkdd` and `R.string.dgsjfjsds`. 
Searching for the former, we find another interesting function under `output/sources/androidx/lifecycle/p.java`:

```java
public final String a(String str) {
    Base64.Decoder decoder;
    Object obj = this.f1392b;
    try {
        String a4 = new d.q((Context) obj).a();
        ((Context) obj).getSharedPreferences("ddjxd", 0);
        SecretKeySpec secretKeySpec = new SecretKeySpec(((Context) obj).getString(R.string.gkgkkdkdd).getBytes(), "Blowfish");
        byte[] bArr = new byte[0];
        if (Build.VERSION.SDK_INT >= 26) {
            decoder = Base64.getDecoder();
            bArr = decoder.decode(a4);
        }
        Cipher cipher = Cipher.getInstance("Blowfish");
        cipher.init(2, secretKeySpec);
        if (!new String(cipher.doFinal(bArr), Charset.forName("UTF-8")).equals(str)) {
            return "";
        }
        return "Welcome To The Jungle!";
    } catch (Exception unused) {
        return "";
    }
}
```

If we take the string values from `output/resources/res/values/strings.xml`, we can try to replicate that logic:

```xml
    <string name="dgsjfjsds">kfGgk^sfgkjs@kshfgj!</string>
    <string name="gkgkkdkdd">dfHk@s9fl!gjk1dg4gd</string>
    <string name="gkjkkdkdd">dfHk@s9fl!gjk1dg4gd</string>
    <string name="hgsjfjsds">kfGgk^sfgkjs@kshfgj!</string>
    <string name="jsjkdfkaj">kaj1jfkf$dmskjtnnbn</string>
    <string name="jsjkjfkaj">kaj1jfkf$dmskjtnnbn</string>    
```

We create the following Java program:

```java
import javax.crypto.spec.SecretKeySpec;
import java.util.Base64;
import javax.crypto.Cipher;
import java.nio.charset.Charset;

class Decrypt {
    public static void main(String[] args) {
        try
        {
            SecretKeySpec secretKeySpec = new SecretKeySpec("dfHk@s9fl!gjk1dg4gd".getBytes(), "Blowfish");
            byte[] bArr = new byte[0];
            Base64.Decoder decoder;
            decoder = Base64.getDecoder();
            bArr = decoder.decode("dljJDNHodV4wd1znNjIW8h86++7TA/zlohduLouuD0PMz0pU7xDFdw==");
            Cipher cipher = Cipher.getInstance("Blowfish");
            cipher.init(2, secretKeySpec);
            System.out.println(new String(cipher.doFinal(bArr), Charset.forName("UTF-8")));
        }  catch (Exception unused) {
            System.out.println(unused);
        }
    }
}
```

Compile and run it to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Guns_And_Roses_Concert_Coupon]
└─$ javac Decrypt.java

┌──(user@kali)-[/media/sf_CTFs/appsec/Guns_And_Roses_Concert_Coupon]
└─$ java Decrypt
AppSec-IL{3ncr1pt10n_15_n0t_h@5h1ng}
```