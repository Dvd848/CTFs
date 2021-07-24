# Calculator
5 Points

## Solution

We get a page titled "A nice calculator" with an input field saying "Formula e.g. 5*5+1". We can enter a formula and receive the result.

A developer message is printed to the developer console: `Loading the code for the Calculator (5 pt) page. To debug this code, open d3wwc.js in Developer Tools`. So, let's check the code (`CTRL`+`P` in the Chrome "Sources" tab, then search for `d3wwc.js`. There should be two results, if the current result says "Source map detected", choose the other one):

```javascript
import { calculate} from "backend/calculator.jsw"

$w.onReady(function () {

});

/**
 *	Adds an event handler that runs when the element is clicked.
 *	 @param {$w.MouseEvent} event
 */
export function Calculate_click(event) {
	calculate($w("#formula").value).then(resultObj => {
		if(!resultObj) {
			$w('#result').text = "Error: Make sure you insert a legal formula!"
		}else{
			$w('#result').text = "= " + resultObj.result
			console.log(resultObj.description)
		}
	}).catch(err => {
			$w('#result').text = "Error: Make sure you insert a legal formula!"
			console.error(err)
			});
}
```

Nothing much here, let's see what happens when we enter enter different inputs. Entering `7*7` triggers a request to `https://www.wixctf.com/_api/wix-code-public-dispatcher/siteview/backend/calculator.jsw/calculate.ajax`, and the response is a JSON object:

```json
{"result":{"result":49,"description":"Failed to execute passChallenge"}}
```

Failed to execute passChallenge, you say? So what if we enter `passChallenge` in the formula? Well, we get the following response:

```json
{"result":{"result":"You passed the challenge R3M073_c0d3_3x3CU7i0n","description":"RCE successfully done"}}
```

Ok then...


