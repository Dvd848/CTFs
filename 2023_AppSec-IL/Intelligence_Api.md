
# Intelligence Api

* Category: Web
* 100 points

## Description

> Intelligence officers pay attention. The police will not remain behind.  
> Our recently released API lets you get information before starting a mission.  
> Stay put!

## Solution

A website was attached. It said:

```html
<div class="col-md-12">
    <span class="head-main">NOW OR NEVER</span>                
    <h3 class="head-last">Detective Angel, enter your id to get the intell for your task!</h3>                                
</div>
```

There was a form used to get intel given a detective name:

```html
<section class="for-full-back color-white" id="about-intell">
    <div class="container">
        <div class="row text-left g-pad-bottom">
            <div class="col-md-8 col-md-offset-2 ">                    
                <input type="text" name="detective_name" id="detective_name">                                                      
                <input type="button" onclick="get_intell()" value="Get Intell!"/>
                <p  id="message"></p>  
                <p  id="error"></p>  
            </div>

        </div>
    </div>
</section>

<!-- ... -->

<script>         
    async function get_intell() {         
        var detective_name = document.getElementById("detective_name").value;         
        var url = `/api/v2/detective/${detective_name}`;
    // Default options are marked with *
    const response = await fetch(url, {
        method: "GET", // *GET, POST, PUT, DELETE, etc.
        mode: "cors", // no-cors, *cors, same-origin
        cache: "no-cache", // *default, no-cache, reload, force-cache, only-if-cached
        credentials: "same-origin", // include, *same-origin, omit
        headers: {
        "Content-Type": "application/json",
        // 'Content-Type': 'application/x-www-form-urlencoded',
        },
        redirect: "follow", // manual, *follow, error
        referrerPolicy: "no-referrer", // no-referrer, *no-referrer-when-downgrade, origin, origin-when-cross-origin, same-origin, strict-origin, strict-origin-when-cross-origin, unsafe-url
        //body: JSON.stringify(data), // body data type must match "Content-Type" header
    })
    .then(response => response.json())    
    .then(data => displayMessage(JSON.stringify(data)))    
    .catch(error => {displayError("*** in the catch() method *** " + error);         
    return false;
    //document.getElementById("detective_name").value="";
    }
    );
}

function displayError(msg) {
    var html_data = "<div>" + "An Error has occured" + msg;
    document.getElementById("message").innerHTML=html_data;
    //document.getElementById("detective_name").value="";

}

function displayMessage(msg) {        
var dataobg = JSON.parse(msg)[0];//['detective_name'];
var html_data ="<div>Wrong Detective!";
if (dataobg){

    html_data = "<div>Detective Name: " + dataobg['detective_name'] + "<br/>";
    html_data += "Suspect Name: " + dataobg['suspect_name'] + "<br/>";
    html_data += "Suspect Address: " + dataobg['suspect_addr'] + "<br/>";
    html_data += "Suspect Spouse Name: " + dataobg['suspect_spouse'] + "<br/>";
    html_data += "Suspect Details: " + dataobg['suspect_details'] + "<br/>";
    html_data += "Suspect Point Of Interaction: " + dataobg['suspect_point_of_interaction'] + "<br/>";
    html_data += "Time Of Interaction: " + dataobg['suspect_hour_of_interaction'] + "<br/>";
    html_data+="</div>"
}
document.getElementById("message").innerHTML=html_data;
}
</script>
```

Let's try to get the intel for Angel:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Intelligence_Api]
└─$ curl 'https://intel-api.appsecil.ctf.today/api/v2/detective/Angel'
[] 
```

An empty array as a response means that the UI will display `Wrong Detective!`.  
At the end of the source code, there's an interesting comment:

```html
<!--delete delete delete 777--> 
```

Let's try that:

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Intelligence_Api]
└─$ curl 'https://intel-api.appsecil.ctf.today/api/v2/detective/777'
[{"detective_id":"777","detective_name":"Detecive Angel","suspect_name":"Goerge the vicioius","suspect_addr":"The black way 111, City of terror","suspect_spouse":"Joahana give me hope","suspect_details":"Loves client side programming, dealing with pixels, get angry and  kill someone","suspect_point_of_interaction":"The vicious way 222","suspect_hour_of_interaction":"0001"}]
```

Well, we found Angel! But what now?  
Maybe we can get a list of all detectives?

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Intelligence_Api]
└─$ curl 'https://intel-api.appsecil.ctf.today/api/v2/detective/'
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Error</title>
</head>
<body>
<pre>Cannot GET /api/v2/detective/</pre>
</body>
</html>
```

Notice how the API is marked as `v2`. What about `v1`?

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Intelligence_Api]
└─$ curl 'https://intel-api.appsecil.ctf.today/api/v1/detective/'
W3siZGV0ZWN0aXZlX2lkIjoiNzc3IiwiZGV0ZWN0aXZlX25hbWUiOiJEZXRlY2l2ZSBBbmdlbCIsInN1c3BlY3RfbmFtZSI6IkdvZXJnZSB0aGUgdmljaW9pdXMiLCJzdXNwZWN0X2FkZHIiOiJUaGUgYmxhY2sgd2F5IDExMSwgQ2l0eSBvZiB0ZXJyb3IiLCJzdXNwZWN0X3Nwb3VzZSI6IkpvYWhhbmEgZ2l2ZSBtZSBob3BlIiwic3VzcGVjdF9kZXRhaWxzIjoiTG92ZXMgY2xpZW50IHNpZGUgcHJvZ3JhbW1pbmcsIGRlYWxpbmcgd2l0aCBwaXhlbHMsIGdldCBhbmdyeSBhbmQgIGtpbGwgc29tZW9uZSIsInN1c3BlY3RfcG9pbnRfb2ZfaW50ZXJhY3Rpb24iOiJUaGUgdmljaW91cyB3YXkgMjIyIiwic3VzcGVjdF9ob3VyX29mX2ludGVyYWN0aW9uIjoiMDAwMSJ9LHsiZGV0ZWN0aXZlX2lkIjoiODg4IiwiZGV0ZWN0aXZlX25hbWUiOiJEZXRlY2l2ZSBEaW5nIERvbmciLCJzdXNwZWN0X25hbWUiOiJDaGFybGllIiwic3VzcGVjdF9hZGRyIjoiVGhlIGhhY2tlciB3YXkgNzc3LCBIYWNrbGVvbWEgIiwic3VzcGVjdF9zcG91c2UiOiJObyB3aWZlcywgd2hvIGhhcyB0aW1lIGZvciB0aGF0IEkgd2FudCB0byBoYWNrIHRoZSBwZXh0eGd4biIsInN1c3BlY3RfZGV0YWlscyI6IndoYXRldmVyIEkgY2FuIGhhY2sgQXBwU2VjLUlMezBsZF9AcDE1X2QxM19oQHJkfSIsInN1c3BlY3RfcG9pbnRfb2ZfaW50ZXJhY3Rpb24iOiJtYXJzIDc3NyIsInN1c3BlY3RfaG91cl9vZl9pbnRlcmFjdGlvbiI6IjA0MDEifV0=
```

This seems to return something!

```console
┌──(user@kali)-[/media/sf_CTFs/appsec/Intelligence_Api]
└─$ curl 'https://intel-api.appsecil.ctf.today/api/v1/detective/' -s | base64 -d
[{"detective_id":"777","detective_name":"Detecive Angel","suspect_name":"Goerge the vicioius","suspect_addr":"The black way 111, City of terror","suspect_spouse":"Joahana give me hope","suspect_details":"Loves client side programming, dealing with pixels, get angry and  kill someone","suspect_point_of_interaction":"The vicious way 222","suspect_hour_of_interaction":"0001"},{"detective_id":"888","detective_name":"Detecive Ding Dong","suspect_name":"Charlie","suspect_addr":"The hacker way 777, Hackleoma ","suspect_spouse":"No wifes, who has time for that I want to hack the pextxgxn","suspect_details":"whatever I can hack AppSec-IL{0ld_@p15_d13_h@rd}","suspect_point_of_interaction":"mars 777","suspect_hour_of_interaction":"0401"}]
```

The response includes the flag!