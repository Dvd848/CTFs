# punkShop
20 Points

## Solution

We get a page titled "REAL SHOP" with the motto of "Meet our pros, buy the right stuff, improve your life". There's a button saying "Shop".

The developer console prints a debug message: `Loading the code for the punkShop (20 pt) page. To debug this code, open ccbt.js in Developer Tools.`. Let's check the sources:

```javascript
import wixUsers from 'wix-users';
import wixLocation from 'wix-location';
import {getProducts} from 'backend/collectionspoofing';


$w.onReady(function () {
	$w('#repeater1').hide();
	let user = wixUsers.currentUser;
	let isLoggedIn = user.loggedIn;
	if (!isLoggedIn)
		wixLocation.to("/")
$w('#text9').hide();
	$w('#button1').onClick(async() => {
		$w('#group1').hide();
		const items = await getProducts("products")
		console.log("test")
		if (typeof(items) === 'string'){
			$w('#text9').text = "Flag{"+items+"}"
			$w('#text9').show();
		}
		$w('#repeater1').data = items;
		$w("#repeater1").onItemReady(($item, itemData, index) => {
			$item("#image12").src = itemData.image;
			$item("#text7").text = itemData.description;
			$item("#text8").text = `Price: ${itemData.price}$` 
		})
		$w('#repeater1').show()
		console.log(items)
		console.log("solution in the hidden page")

	})
   
  
});
```

We can already see a flag string returning from `getProducts`. We also see a hint: "solution in the hidden page". The hint is outputted to the console once we click the "shop" button.

`getProducts` is defined in another javascript file:

```javascript
exports.getProducts = function getProducts() {return elementorySupport.getJSON('/backend/collectionspoofing.jsw/getProducts.ajax', arguments)}
exports.getCollectionArray = function getCollectionArray() {return elementorySupport.getJSON('/backend/collectionspoofing.jsw/getCollectionArray.ajax', arguments)}
```

The result received from `getProducts` doesn't look like a flag at all, it's:

```json
{
   "result":[
      {
         "image":"wix:image://v1/11062b_a054d8e3040d44cdabd08eec9e293b35~mv2.jpg/Hot%20Air%20Balloons%20.jpg#originWidth=6480&originHeight=4320",
         "description":"Hot Air Baloon",
         "_id":"00fa558e-b8d4-48ef-84e7-9dea89a3ce05",
         "_owner":"aab1bb0b-a1c0-4bdd-a691-de9e032d0d8a",
         "_createdDate":{
            "$date":"2021-06-02T08:44:25.360Z"
         },
         "price":"360",
         "_updatedDate":{
            "$date":"2021-06-16T08:05:59.684Z"
         }
      },
      {
         "image":"wix:image://v1/11062b_857f12c5807643cabaebc02337720bfe~mv2.jpg/Subway%20Approaching%20the%20Station.jpg#originWidth=5472&originHeight=3648",
         "description":"SubWay Train- Half Price",
         "_id":"eb306847-8492-469c-ab70-3ae1f789b91c",
         "_owner":"aab1bb0b-a1c0-4bdd-a691-de9e032d0d8a",
         "_createdDate":{
            "$date":"2021-06-02T08:43:33.193Z"
         },
         "price":"60",
         "_updatedDate":{
            "$date":"2021-06-16T08:07:13.991Z"
         }
      },
      {
         "image":"wix:image://v1/36cba4097d3f46949621d475eaa493e3.jpg/Cocktail%20Final%20Touch.jpg#originWidth=4256&originHeight=2832",
         "description":"A Yummy Cocktail",
         "_id":"a982a416-18cb-4b54-8708-6b18084d507b",
         "_owner":"aab1bb0b-a1c0-4bdd-a691-de9e032d0d8a",
         "_createdDate":{
            "$date":"2021-06-02T08:38:12.988Z"
         },
         "price":"400",
         "_updatedDate":{
            "$date":"2021-06-16T08:08:42.091Z"
         }
      }
   ]
}
```

What is this about a hidden page? We can check the sources for all the different pages in this Wix-based site:

```javascript
{
    "baseUrl": "https:\/\/www.wixctf.com",
    "mainPageId": "hzjue",
    "pages": {
        "pi8l0": "aab1bb_5978083327ece32f463ac401b4448617_576",
        "vua5i": "aab1bb_2924361d0ce4673d4e945d452e705f89_534",
        "zonm2": "aab1bb_7d3d5815df5b2f09955ec0d88cecd920_463",
        "ezd9z": "aab1bb_5bc167f71413d434c931ee8231d0cc8e_553",
        "ccbt": "aab1bb_a0f243fc8eab17e13dd92580c75e2ec7_490",
        "d7r9x": "aab1bb_fdda44f0bfcf90f55b1f94f91ac60433_553",
        "bbtsx": "aab1bb_1e40d2403eb01ddaf77c0845ab402d32_504",
        "o2txy": "aab1bb_b8cfeacc9945ae9284ee8be450da5d74_547",
        "d3wwc": "aab1bb_acde360c3c1d811df62dbec0ddaaed9d_462",
        "hzjue": "aab1bb_ce96c374ff8d8e6fd5a08a8f0534dbcd_576",
        "bcoda": "aab1bb_7c3c08eed8b685208fd70230028c7816_576",
        "bcv5o": "aab1bb_000b3cb3f1034ab75711c2d0682043d9_575",
        "e9giz": "aab1bb_6ea9ce7bf05e045bd56f0dcc48152d9f_547",
        "l23ob": "aab1bb_c84fa1e9279cca0f6de06930f37595e9_388"
    },
    "routes": {
        ".\/getauth": {
            "type": "Static",
            "pageId": "pi8l0"
        },
        ".\/popup-vua5i": {
            "type": "Static",
            "pageId": "vua5i"
        },
        ".\/submit": {
            "type": "Static",
            "pageId": "zonm2"
        },
        ".\/blank-2": {
            "type": "Static",
            "pageId": "ezd9z"
        },
        ".\/shop": {
            "type": "Static",
            "pageId": "ccbt"
        },
        ".\/blank-3": {
            "type": "Static",
            "pageId": "d7r9x"
        },
        ".\/status": {
            "type": "Static",
            "pageId": "bbtsx"
        },
        ".\/tryme": {
            "type": "Static",
            "pageId": "o2txy"
        },
        ".\/blank": {
            "type": "Static",
            "pageId": "d3wwc"
        },
        ".\/blank-1": {
            "type": "Static",
            "pageId": "hzjue"
        },
        ".\/scores": {
            "type": "Static",
            "pageId": "bcv5o"
        },
        ".\/fetcher": {
            "type": "Static",
            "pageId": "e9giz"
        },
        ".\/30": {
            "type": "Static",
            "pageId": "l23ob"
        },
        ".\/account": {
            "type": "Dynamic"
        },
        ".\/profile": {
            "type": "Dynamic"
        },
        ".\/": {
            "type": "Static",
            "pageId": "hzjue"
        }
    },
    "isWixSite": false
}
```

Most of the pages belong to the different challenges or the CTF administration, but there's one that doesn't seem related to anything:

```javascript
".\/30": {
	"type": "Static",
	"pageId": "l23ob"
},
```

However, visiting `https://www.wixctf.com/30` results in a blank page. What about `getCollectionArray` we saw earlier? If we try to call it, we get:

```json
{
  "result": [
    "30",
    "YanivCollection",
    "clients",
    "contactForm",
    "log",
    "products",
    "scoreBoard",
    "sortedData",
    "successLog",
    "Marketing/Coupons",
    "CRM/Tasks",
    "Members/PrivateMembersData",
    "Users/PublicMemberProfiles",
    "Members/PublicData",
    "qaVertical/noSortsAllowed",
    "qaVertical/supportsEverything",
    "qaVertical/allowsReadInsert",
    "qaVertical/allowsReadRemove",
    "qaVertical/allowsGetFindUpdate",
    "test/songs",
    "test/albums",
    "test/genres",
    "test/genresWithParent",
    "test/genresWithSelfRefencingField",
    "test/songsInvalid",
    "test/albumsInvalid",
    "test/testCollection",
    "Members/Badges",
    "Billing/Invoices",
    "Billing/PriceQuotes",
    "Media/Files",
    "Media/Folders"
  ]
}
```

Originally, `getProducts` gets called by the website with `["products"]`, what if we change it to the `["30"]` collection instead?

We get:

```json
{
  "result": [
    {
      "_id": "e15c4715-2c58-43fd-98b4-5b76ab3d17f2",
      "_owner": "aab1bb0b-a1c0-4bdd-a691-de9e032d0d8a",
      "_createdDate": {
        "$date": "2021-06-08T11:03:29.975Z"
      },
      "_updatedDate": {
        "$date": "2021-07-21T12:45:45.250Z"
      },
      "flag": "8169765b-b7f9-4575-b0e0-e29d96dd8e53"
    }
  ]
}
```

We got the flag.