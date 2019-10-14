# dont-use-client-side
Web Exploitation, 100 points

## Description:
> Can you break into this super secure portal?


## Solution: 

We visit the website and get the following page:

```html
<html>
<head>
<title>Secure Login Portal</title>
</head>
<body bgcolor=blue>
<!-- standard MD5 implementation -->
<script type="text/javascript" src="md5.js"></script>

<script type="text/javascript">
  function verify() {
    checkpass = document.getElementById("pass").value;
    split = 4;
    if (checkpass.substring(0, split) == 'pico') {
      if (checkpass.substring(split*6, split*7) == '6a8e') {
        if (checkpass.substring(split, split*2) == 'CTF{') {
         if (checkpass.substring(split*4, split*5) == 'ts_p') {
          if (checkpass.substring(split*3, split*4) == 'lien') {
            if (checkpass.substring(split*5, split*6) == 'lz_5') {
              if (checkpass.substring(split*2, split*3) == 'no_c') {
                if (checkpass.substring(split*7, split*8) == 'b}') {
                  alert("Password Verified")
                  }
                }
              }
      
            }
          }
        }
      }
    }
    else {
      alert("Incorrect password");
    }
    
  }
</script>
<div style="position:relative; padding:5px;top:50px; left:38%; width:350px; height:140px; background-color:yellow">
<div style="text-align:center">
<p>This is the secure login portal</p>
<p>Enter valid credentials to proceed</p>
<form action="index.html" method="post">
<input type="password" id="pass" size="8" />
<br/>
<input type="submit" value="verify" onclick="verify(); return false;" />
</form>
</div>
</div>
</body>
</html>
```

There are obviously many easy ways to solve this, but just for fun(?) - here's a bash command that uses sorting and string replacement to display the flag:

```console
root@kali:/media/sf_CTFs/pico/dont-use-client-side# curl -s https://2019shell1.picoctf.com/problem/21888/ | grep substring | sed "s/') {/\n/g" | sed "s/[[:space:]]*if (checkpass.substring(\(split\*\|\)//g" | sort -n | awk '{ printf $4 }' | tr -d "'" && echo
picoCTF{no_clients_plz_56a8eb}
```
