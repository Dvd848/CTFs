# Gatekeeper
RE

## Description:
> It's a media PC! All fully purchased through the online subscription revolution empire "GimmeDa$". The PC has a remote control service running that looks like it'll cause all kinds of problems or that was written by someone who watched too many 1990s movies. You download the binary from the vendor and begin reversing it. Nothing is the right way around.

A binary file was attached.

## Solution:

Let's run the file:
```console
root@kali:/media/sf_CTFs/google/gatekeeper# ./gatekeeper
/===========================================================================\
|               Gatekeeper - Access your PC from everywhere!                |
+===========================================================================+
[ERROR] Login information missing
Usage: ./gatekeeper <username> <password>
```

Let's provide some username and password:
```console
root@kali:/media/sf_CTFs/google/gatekeeper# ./gatekeeper my_username my_password
/===========================================================================\
|               Gatekeeper - Access your PC from everywhere!                |
+===========================================================================+
 ~> Verifying....
ACCESS DENIED
 ~> Incorrect username
```

`ltrace` can be very useful in order to get an overview of the calls a userspace application makes to shared libraries.

Let's run it and search for comparisons (e.g. `strcmp`):
```console
root@kali:/media/sf_CTFs/google/gatekeeper# ltrace ./gatekeeper my_user my_password 2>&1 | grep cmp
strcmp("my_user", "0n3_W4rM")                    = 61
```

Looks like out username was compared to "0n3_W4rM", let's try that as the username:
```console
root@kali:/media/sf_CTFs/google/gatekeeper# ltrace ./gatekeeper 0n3_W4rM my_password 2>&1 | grep cmp
strcmp("0n3_W4rM", "0n3_W4rM")                   = 0
strcmp("drowssap_ym", "zLl1ks_d4m_T0g_I")        = -22
```

Looks like we made some progress, now our (reversed) password is being compared to another string. Let's try that:
```console
root@kali:/media/sf_CTFs/google/gatekeeper# ./gatekeeper 0n3_W4rM $(echo "zLl1ks_d4m_T0g_I" | rev)
/===========================================================================\
|               Gatekeeper - Access your PC from everywhere!                |
+===========================================================================+
 ~> Verifying.......Correct!
Welcome back!
CTF{I_g0T_m4d_sk1lLz}
```

The flag: CTF{I_g0T_m4d_sk1lLz}