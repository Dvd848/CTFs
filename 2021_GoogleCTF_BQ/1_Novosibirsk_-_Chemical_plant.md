# Novosibirsk - Chemical plant
Category: Web

## Description

> "You must wonder why we have summoned you, AGENT? It has come to our attention that something terrible is about to take place. There is still time to prevent the disaster, and we could not think of anyone more suited for this task than you. We believe that if you can’t solve this quest, neither can anybody else. You have to travel to Novosibirsk, and investigate a suspicious chemical plant. This mission must be executed in secrecy. It’s classified, and it regards the safety of the whole world, therefore we can’t tell you anything more just yet. Go now, you have the fate of the world in your hands."
> 
> Challenge: CCTV (rev)
> 
> You arrive at your destination. The weather isn't great, so you figure there's no reason to stay outside and you make your way to one of the buildings. No one bothered you so far, so you decide to play it bold - you make yourself a cup of coffee in the social area like you totally belong here and proceed to find an empty room with a desk and a chair. You pull out our laptop, hook it up to the ethernet socket in the wall, and quickly find an internal CCTV panel - that's a way better way to look around unnoticed. Only problem is... it wants a password.

## Solution

The attached website contained a form which requested a password.

We can inspect the source and find the following HTML:

```html
<body>
  <div class="outer">
    <div class="top"><div>Industrial CCTV by YesWeAreSecure&trade;</div></div>
    <div class="container">
      <div class="panel">
        <p>Password:</p>
        <input id="password">
        <p><button id="go">Log in</button></p>
      </div>
    </div>
    <div class="bottom"><div>Use the panel to log in.</div></div>
  </div>
</body>
<script>
const checkPassword = () => {
  const v = document.getElementById("password").value;
  const p = Array.from(v).map(a => 0xCafe + a.charCodeAt(0));

  if(p[0] === 52037 &&
     p[6] === 52081 &&
     p[5] === 52063 &&
     p[1] === 52077 &&
     p[9] === 52077 &&
     p[10] === 52080 &&
     p[4] === 52046 &&
     p[3] === 52066 &&
     p[8] === 52085 &&
     p[7] === 52081 &&
     p[2] === 52077 &&
     p[11] === 52066) {
    window.location.replace(v + ".html");
  } else {
    alert("Wrong password!");
  }
}

window.addEventListener("DOMContentLoaded", () => {
  document.getElementById("go").addEventListener("click", checkPassword);
  document.getElementById("password").addEventListener("keydown", e => {
    if (e.keyCode === 13) {
      checkPassword();
    }
  });
}, false);
</script>
```

It looks like the password is checked in the client side. Therefore we can reverse the check and get the expected password:

```console
┌──(user@kali)-[/media/sf_CTFs/google/1_Novosibirsk_-_Chemical_plant]
└─$ node
> logic = `p[0] === 52037 &&
...      p[6] === 52081 &&
...      p[5] === 52063 &&
...      p[1] === 52077 &&
...      p[9] === 52077 &&
...      p[10] === 52080 &&
...      p[4] === 52046 &&
...      p[3] === 52066 &&
...      p[8] === 52085 &&
...      p[7] === 52081 &&
...      p[2] === 52077 &&
...      p[11] === 52066`
'p[0] === 52037 &&\n     p[6] === 52081 &&\n     p[5] === 52063 &&\n     p[1] === 52077 &&\n     p[9] === 52077 &&\n     p[10] === 52080 &&\n     p[4] === 52046 &&\n     p[3] === 52066 &&\n     p[8] === 52085 &&\n     p[7] === 52081 &&\n     p[2] === 52077 &&\n     p[11] === 52066'
> p = []
[]
> eval(logic.replace(/===/g, "=").replace(/&&/g, ";"))
52066
> p
[ 52037,
  52077,
  52077,
  52066,
  52046,
  52063,
  52081,
  52081,
  52085,
  52077,
  52080,
  52066 ]
> p.map(a => String.fromCharCode(a - 0xCafe)).join("")
'GoodPassword'
```

What we did here is to take the original expression and replace "===" with "=" and "&&" with ";" in order to create the following expression:

```console
> console.log(logic.replace(/===/g, "=").replace(/&&/g, ";"))
     p[0] = 52037 ;
     p[6] = 52081 ;
     p[5] = 52063 ;
     p[1] = 52077 ;
     p[9] = 52077 ;
     p[10] = 52080 ;
     p[4] = 52046 ;
     p[3] = 52066 ;
     p[8] = 52085 ;
     p[7] = 52081 ;
     p[2] = 52077 ;
     p[11] = 52066
```

We evaluate the expression with `eval` to recreate the `p` array, then use `map` to subtract `0xCafe` from each array element and receive the original expected input: `GoodPassword`.

We enter the password and get redirected to a page with the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/google/1_Novosibirsk_-_Chemical_plant]
└─$ curl https://cctv-web.2021.ctfcompetition.com/GoodPassword.html -s | grep CTF
    <div class="bottom"><div>Important note: CTF{IJustHopeThisIsNotOnShodan}</div></div>
```