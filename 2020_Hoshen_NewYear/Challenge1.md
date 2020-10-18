# Challenge 1

Let's inspect the contents of the IP we got in the intro level:

```console
root@kali:/media/sf_CTFs/hoshen/1# curl 129.213.32.20
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<title>Hoshen Challenge</title>
<style type="text/css">
<!--
body {
        color:#000000;
        background-color:#0072C6;
        margin:0;
}

#container {
        margin-left:auto;
        margin-right:auto;
        text-align:center;
        }

a img {
        border:none;
}

-->
</style>
</head>
<body>
<div id="container">
<img src="Sukkah.jpg" alt="IIS" width="960" height="600" /></a>
</div>
</body>
</html>
```

We have a webpage with a single image: `Sukkah.jpg`. Let's download and inspect it:

```console
root@kali:/media/sf_CTFs/hoshen/1# wget -q http://129.213.32.20/Sukkah.jpg
root@kali:/media/sf_CTFs/hoshen/1# file Sukkah.jpg
Sukkah.jpg: JPEG image data, JFIF standard 1.01, resolution (DPI), density 96x96, segment length 16, comment: "CREATOR: gd-jpeg v1.0 (using IJG JPEG v90), quality = 80", baseline, precision 8, 1247x876, frames 3
```

Indeed, this is a JPEG image. But anything else hidden in there?

```console
root@kali:/media/sf_CTFs/hoshen/1# binwalk Sukkah.jpg

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JPEG image data, JFIF standard 1.01
194152        0x2F668         RAR archive data, version 5.x
```

A RAR file! Let's extract it from the JPEG file:

```console
root@kali:/media/sf_CTFs/hoshen/1# binwalk -e Sukkah.jpg

DECIMAL       HEXADECIMAL     DESCRIPTION
--------------------------------------------------------------------------------
0             0x0             JPEG image data, JFIF standard 1.01

194152        0x2F668         RAR archive data, version 5.x

root@kali:/media/sf_CTFs/hoshen/1# file _Sukkah.jpg.extracted/2F668.rar
_Sukkah.jpg.extracted/2F668.rar: RAR archive data, v5
```