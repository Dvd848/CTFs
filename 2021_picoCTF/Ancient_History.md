# Ancient History
Category: Web, 10 points

## Description

> I must have been sleep hacking or something, I don't remember visiting all of these sites... 

## Solution

After visiting the attached website we notice that the browser's "back" button has become available. If we push "back" we arrive to a page whose URI ends with `?}`, if we go back again we arrive to `?d` and so on, until we arrive to the first available page whose URI ends with `?p`. So, the flag is hidden in the parameters of the different pages.

We can use the following script to collect the different flag characters and reconstruct the flag:

```javascript
var flag = "";
(function myLoop(i) {
  setTimeout(function() {
    flag += window.location.href.substr(-1);
    window.history.forward();
    if (--i) myLoop(i);
  }, 500)
})(window.history.length);   
```

The script needs to run from the browser developer console, while we're at the first page. It simply collects the last character from the URI and continues forward to the next page. Note that due to security limitations, Javascript isn't able to access the history addresses themselves so it can't loop over `window.history` and collect the flag characters from there.

The flag: `picoCTF{th4ts_k1nd4_n34t_0b0a1adf}`