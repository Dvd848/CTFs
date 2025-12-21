# None Shall Pass

Category: Reversing

## Description

> Can you find the password to this Android App?

An Android application was attached.

## Solution

We start by using `jadx` to unpack the APK:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/None_Shall_Pass]
└─$ /home/user/utils/reversing/jadx/build/jadx/bin/jadx -d output NoneShallPass.apk
INFO  - loading ...
INFO  - processing ...
ERROR - finished with errors, count: 2
```

In the application's main activity (`output\sources\io\brewfault\noneshallpass\MainActivity.java`), we find the following logic:

```java
package io.brewfault.noneshallpass;

import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import androidx.appcompat.app.AppCompatActivity;
import io.brewfault.noneshallpass.databinding.ActivityMainBinding;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

/* loaded from: classes.dex */
public class MainActivity extends AppCompatActivity {
    private ActivityMainBinding binding;
    private TextView flagText;
    private TextView inputText;
    private Button submitButton;
    private String userText;
    private String logTag = "NONESHALLPASS";
    private byte[] encIv = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    private String encryptedFlag = "SowTrUiy0uSeQrZxZOvp5mYd4yAvPB+UhVwFzHU/ULetHhO2mlulk0Zjx9l+FaNhQMgCbWoM/m/BzOc+ZfHppnVkZ4s5oTT3d9KtFYdb6b0=";

    public native boolean checkPassword(String str);

    static {
        System.loadLibrary("noneshallpass");
    }

    private String decryptFlag(String str) {
        try {
            SecretKeySpec secretKeySpec = new SecretKeySpec(str.getBytes(StandardCharsets.UTF_8), "AES");
            IvParameterSpec ivParameterSpec = new IvParameterSpec(this.encIv);
            Cipher cipher = Cipher.getInstance("AES/CBC/PKCS7Padding");
            cipher.init(2, secretKeySpec, ivParameterSpec);
            return new String(cipher.doFinal(Base64.decode(this.encryptedFlag.getBytes(StandardCharsets.UTF_8), 0)), "UTF-8");
        } catch (UnsupportedEncodingException | InvalidAlgorithmParameterException | InvalidKeyException | NoSuchAlgorithmException | BadPaddingException | IllegalBlockSizeException | NoSuchPaddingException e) {
            Log.w(this.logTag, "Failed to decrypt flag with " + e);
            return "";
        }
    }

    @Override // androidx.fragment.app.FragmentActivity, androidx.activity.ComponentActivity, androidx.core.app.ComponentActivity, android.app.Activity
    protected void onCreate(Bundle bundle) {
        super.onCreate(bundle);
        ActivityMainBinding inflate = ActivityMainBinding.inflate(getLayoutInflater());
        this.binding = inflate;
        setContentView(inflate.getRoot());
        Button button = (Button) findViewById(R.id.submitButton);
        this.submitButton = button;
        button.setOnClickListener(new View.OnClickListener() { // from class: io.brewfault.noneshallpass.MainActivity$$ExternalSyntheticLambda0
            @Override // android.view.View.OnClickListener
            public final void onClick(View view) {
                MainActivity.this.m146lambda$onCreate$0$iobrewfaultnoneshallpassMainActivity(view);
            }
        });
    }

    /* renamed from: lambda$onCreate$0$io-brewfault-noneshallpass-MainActivity, reason: not valid java name */
    /* synthetic */ void m146lambda$onCreate$0$iobrewfaultnoneshallpassMainActivity(View view) {
        TextView textView = (TextView) findViewById(R.id.submitText);
        this.inputText = textView;
        this.userText = textView.getText().toString();
        this.flagText = (TextView) findViewById(R.id.flagTextBox);
        if (checkPassword(this.userText)) {
            this.flagText.setText("Password correct! The flag is:\n" + decryptFlag(this.userText));
        } else {
            this.flagText.setText("I MOVE FOR NO MAN");
        }
    }
}
```

So, we need to provide a password, which will allow us to decrypt the AES-encrypted `encryptedFlag`.  
How do we know what's the correct password? Well, hopefully `checkPassword` will give us a hint.
This function is located in the native code, so we need to use a native decompiler such as Ghidra or IDA to 
inspect it:

```c
bool __fastcall Java_io_brewfault_noneshallpass_MainActivity_checkPassword(__int64 a1, __int64 a2, __int64 a3)
{
  __int64 i; // rbx
  char *user_input; // r15
  size_t user_input_len; // rax
  unsigned __int8 *buffer; // r14
  int num_match; // ebp
  const char *current_password; // r12
  size_t current_password_len; // rax
  unsigned __int8 encoded_encrypted_user_input[8]; // [rsp+8h] [rbp-50h] BYREF
  __int64 v12; // [rsp+10h] [rbp-48h]
  void *ptr; // [rsp+18h] [rbp-40h]
  unsigned __int64 v14; // [rsp+20h] [rbp-38h]

  v14 = __readfsqword(0x28u);
  i = 0LL;
  user_input = (char *)(*(__int64 (__fastcall **)(__int64, __int64, _QWORD))(*(_QWORD *)a1 + 1352LL))(a1, a3, 0LL);
  user_input_len = strlen(user_input);
  buffer = (unsigned __int8 *)malloc(user_input_len);
  doEncryption(montyPythonKey, user_input, buffer);
  strlen(user_input);
  simple_b64_encode(encoded_encrypted_user_input, (unsigned int)buffer);
  num_match = 0;
  do
  {
    current_password = passwords[i];
    current_password_len = strlen(current_password);
    if ( (encoded_encrypted_user_input[0] & 1) != 0 )
    {
      if ( current_password_len != v12 )
        goto LABEL_2;
    }
    else if ( current_password_len != (unsigned __int64)encoded_encrypted_user_input[0] >> 1 )
    {
      goto LABEL_2;
    }
    num_match += std::string::compare(encoded_encrypted_user_input, 0LL, -1LL, current_password, current_password_len) == 0;
LABEL_2:
    ++i;
  }
  while ( i != 256 );
  free(buffer);
  if ( (encoded_encrypted_user_input[0] & 1) != 0 )
    operator delete(ptr);
  return num_match > 0;
}
```

This function references two globals: `montyPythonKey` which resolves to `IHaveNoQuarrelWithYouGoodSir` 
and `passwords`, which is a list of base64-encoded strings:

```
                 public passwords
.data:000000000004C680 passwords       dq offset aQxu6pkdrcvutja
.data:000000000004C680                                         ; DATA XREF: LOAD:0000000000001CB0↑o
.data:000000000004C680                                         ; .got:passwords_ptr↑o
.data:000000000004C680                                         ; "QXu6pKdrCvuTjA=="
.data:000000000004C688                 dq offset aEij6Cpx6lg0a ; "Eij6+/cpX6LG0A=="
.data:000000000004C690                 dq offset aBcdnCvx6bg0a ; "BCDn+/cvX6bG0A=="
.data:000000000004C698                 dq offset aGczlP02vlfexq ; "GCzl/P02VLfexQ=="
.data:000000000004C6A0                 dq offset aEyh7Eepurpc2yi ; "EyH7+eEpUrPC2YI="
.data:000000000004C6A8                 dq offset aQhi7o6zocSshq ; "QHi7o6ZoC/SShQ=="
...
.data:000000000004CE68                 dq offset aHcb9fmxsqltzw ; "HCb/9fMxSqLTzw=="
.data:000000000004CE70                 dq offset aGsxm5vc5xk3exq ; "GSXm5vc5XK3ExQ=="
.data:000000000004CE78                 dq offset aEybn8Ctxqrf0g ; "Eybn8/ctXqrF0g=="
```

The function encrypts and base64-encodes the user input, then searches the list of passwords
to see if there's a match. So, if we were able to base64-decode and decrypt each of the 
strings, one of them should give us the correct password.

The function `doEncryption` is implemented as:

```c
__int64 __fastcall doEncryption(const char *a1, char *a2, unsigned __int8 *a3)
{
  return _Z12doEncryptionPKcPcPh(a1, a2, a3);
}
```

If we take the `_Z12doEncryptionPKcPcPh` assembly and feed to it ChatGPT, we are told
that it's standard RC4. Let's hope it's correct and proceed.

```python
from Crypto.Cipher import ARC4
from base64 import b64decode

with open("base64.txt", "r") as f:
    for line in f:
        line = line.strip()
        buffer = b64decode(line)
        cipher = ARC4.new(b'IHaveNoQuarrelWithYouGoodSir')
        plaintext = cipher.decrypt(buffer)
        print(plaintext)
```

We run it and get a list of reasonable passwords:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/None_Shall_Pass]
└─$ python3 solve.py
b'jonasbrothers'
b'tequieromucho'
b'pussycatdolls'
b'1234567890'
b'basketball'
b'tinkerbell'
b'hellokitty'
b'christopher'
b'0123456789'
b'volleyball'
b'chrisbrown'
b'strawberry'
b'qwertyuiop'
b'harrypotter'
b'sweetheart'
b'12345678910'
b'manchester'
b'linkinpark'
b'bestfriend'
b'estrellita'
b'california'
b'friendster'
b'bestfriends'
b'daddysgirl'
b'tokiohotel'
b'undertaker'
b'cheerleader'
b'cinderella'
b'jesuschrist'
b'ilovejesus'
b'princesita'
b'jesucristo'
b'0987654321'
b'butterfly1'
b'friendship'
b'simpleplan'
b'diosesamor'
b'beautiful1'
b'mississippi'
b'mickeymouse'
b'prettygirl'
b'elizabeth1'
b'9876543210'
b'watermelon'
b'juancarlos'
b'teamomucho'
b'0000000000'
b'falloutboy'
b'spongebob1'
b'1111111111'
b'realmadrid'
b'ronaldinho'
b'password123'
b'chocolate1'
b'tweetybird'
b'smallville'
b'ilovemyself'
b'ilovechris'
b'gymnastics'
b'sailormoon'
b'dramaqueen'
b'computadora'
b'liverpoolfc'
b'1212312121'
b'password12'
b'evanescence'
b'motherfucker'
b'jacqueline'
b'jesusfreak'
b'triskelion'
b'liverpool1'
b'magandaako'
b'mypassword'
b'abercrombie'
b'candyfloss'
b'winniethepooh'
b'puertorico'
b'capricornio'
b'1123581321'
b'hannahmontana'
b'girlfriend'
b'skateboard'
b'IAMAR7HURKINGOFTHEBRITONSJOINME!'
b'blingbling'
b'celtic1888'
b'playstation'
b'johnnydepp'
b'princess12'
b'alexandria'
b'hollister1'
b'christian1'
b'heartbreaker'
b'dragonball'
b'avrillavigne'
b'pinkpanther'
b'bernadette'
b'septiembre'
b'eastenders'
b'cheesecake'
b'hilaryduff'
b'manchesterunited'
b'alexander1'
b'7894561230'
b'daddyyankee'
b'matematica'
b'floricienta'
b'123456789a'
b'iamthebest'
b'flowerpower'
b'bobesponja'
b'klapaucius'
b'washington'
b'butterflies'
b'mychemicalromance'
b'1122334455'
b'babygirl12'
b'calculator'
b'ilovejames'
b'5555555555'
b'dragonballz'
b'livestrong'
b'123456789123456'
b'philippines'
b'serendipity'
b'heartbroken'
b'timberlake'
b'playboy123'
b'loveforever'
b'villanueva'
b'cheerleading'
b'superstar1'
b'dulcemaria'
b'ilovedavid'
b'highschool'
b'princess13'
b'sweetiepie'
b'underground'
b'cheekymonkey'
b'spiderman1'
b'miguelangel'
b'sweetangel'
b'ironmaiden'
b'brokenheart'
b'ilovemymom'
b'hotmail.com'
b'goodcharlotte'
b'soylamejor'
b'password11'
b'ilovemyfamily'
b'astonvilla'
b'stephanie1'
b'zxcvbnm,./'
b'contrase\xc3\xb1a'
b'peanutbutter'
b'jesussaves'
b'finalfantasy'
b'password13'
b'friends4ever'
b'backstreet'
b'summertime'
b'mileycyrus'
b'ilovejason'
b'newzealand'
b'monkeybutt'
b'bigbrother'
b'barbiegirl'
b'babygirl13'
b'complicated'
b'information'
b'alessandro'
b'cookiemonster'
b'iloveyou12'
b'jesuslovesme'
b'groovychick'
b'godlovesme'
b'sexybitch1'
b'princess123'
b'friendsforever'
b'wachtwoord'
b'aquamarine'
b'hunnybunny'
b'independent'
b'alessandra'
b'poiuytrewq'
b'minniemouse'
b'florentina'
b'hondacivic'
b'lovelygirl'
b'disneyland'
b'teddybear1'
b'memyselfandi'
b'wonderland'
b'happybunny'
b'1q2w3e4r5t'
b'hellothere'
b'computador'
b'lamborghini'
b'terminator'
b'jesusislord'
b'braveheart'
b'princess11'
b'university'
b'spongecola'
b'teamomiamor'
b'tekieromucho'
b'jellybeans'
b'1234554321'
b'1029384756'
b'orlandobloom'
b'sagitarius'
b'newpassword'
b'metallica1'
b'estrelinha'
b'juliocesar'
b'butterfly2'
b'rockandroll'
b'losangeles'
b'leavemealone'
b'princess01'
b'revolution'
b'felicidade'
b'universidad'
b'timberland'
b'quiksilver'
b'softball12'
b'limpbizkit'
b'ichliebedich'
b'hellohello'
b'christina1'
b'mostwanted'
b'iloveyou123'
b'dominicana'
b'antoinette'
b'elsalvador'
b'tekieromuxo'
b'prettyricky'
b'tekelomuxo'
b'strawberries'
b'madagascar'
b'chaparrita'
b'alianzalima'
b'9999999999'
b'ilovejustin'
b'honeybunch'
b'anggandako'
b'evangelion'
b'buttercup1'
b'nickcarter'
b'a123456789'
b'minicooper'
b'ilovejamie'
b'guadalajara'
b'quicksilver'
b'lovealways'
b'ilovedanny'
b'concepcion'
```

Only one of them really stands out, but for the sake of completeness, let's approach this in 
a thorough manner by trying them all:

```python
from Crypto.Cipher import ARC4
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad

from base64 import b64decode

encrypted_flag = b64decode("SowTrUiy0uSeQrZxZOvp5mYd4yAvPB+UhVwFzHU/ULetHhO2mlulk0Zjx9l+FaNhQMgCbWoM/m/BzOc+ZfHppnVkZ4s5oTT3d9KtFYdb6b0=")
iv =  bytes([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15])

with open("base64.txt", "r") as f:
    for line in f:
        line = line.strip()
        buffer = b64decode(line)
        cipher = ARC4.new(b'IHaveNoQuarrelWithYouGoodSir')
        password = cipher.decrypt(buffer)

        try:
            cipher = AES.new(password, AES.MODE_CBC, iv)
            decrypted_data = cipher.decrypt(encrypted_flag)
            decrypted_data = unpad(decrypted_data, AES.block_size)

            print(f"Key: {password.decode()}")
            print(f"{decrypted_data.decode()}\n")
        except ValueError:
            pass
```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/None_Shall_Pass]
└─$ python3 solve.py
Key: IAMAR7HURKINGOFTHEBRITONSJOINME!
FLAG{Av3rage_w1ngsp3ed_velocity_of_unl@den_3urpean_sw4llow_is_20.1mph}
```