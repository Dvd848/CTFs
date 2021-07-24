# Fetcher
15 Points

## Solution

We get a page titled "I issue HTTP requests!" and the ability to enter a URL and click "go".

The developer console outputs the following messages:

```
Loading the code for the Fetcher (15 pt) page. To debug this code, open e9giz.js in Developer Tools.
Did you know you can get a wix site for free? just create a free account and you're good to go!
```

We enter a URL such as `http://www.google.com` and get back the following response in the developer console:

```
Common, we're not going to send our flag to Google, are we? only wix sites are allowed
```

What if we enter a Wix site (e.g. `https://username.wixsite.com/mysite`)? In that case, we get:

```
Yes! we LOVE this URL and we sent our flag your way! did you GET it? no? perhaps this can help https://www.wix.com/velo/reference/wix-http-functions
```

So Wix have the ability to run server side code upon receiving an HTTP request. They probably want us to follow the tutorial and set this up in a new Wix site. When we enter the appropriate URL to the fetcher, they will send up the flag.

So, we setup a Wix site and add the following code:

```javascript
import {fetch} from 'wix-fetch'; 

export async function perform_requst(data) {
  const url = 'https://en6w1e7xzpj5p.x.pipedream.net/';
    
  let fullUrl = url + data; 
  
  return fetch(fullUrl, {method: 'get'})
    .then(response => response.json())
    .then(json => json.main.temp);
}

export function get_flag(request) {
	const response = {
		"headers": {
			"Content-Type": "application/json"
		}
	};

	perform_requst(request)
	perform_requst(JSON.stringify(request))
	response.body = {
		"status": "OK"
	};
	return ok(response);

}
```

The code receives a get request and sends the request info to our requestbin.

We fetch `https://ctf680.wixsite.com/website/_functions/flag` and get the flag: `thisisyourflag!`.