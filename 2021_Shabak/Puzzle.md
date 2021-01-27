# Puzzle
Category: Web, 300 points


## Description

> Nati Plemental applied for a job in the ISA. As a recruitment assignment, he was challenged to solve our puzzle. Unfortunately, instead of focusing on actually solving the puzzle, Nati wasted his time and effort on adding meaningless features.
> 
> Nati was not good enough. We decided not to recruit him.
> 
> **Note:** This is not a typical WEB challenge.


## Solution:

Attached to this challenge is a zip archive called `solgen.zip`. Let's unzip it:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# unzip solgen.zip
Archive:  solgen.zip
  inflating: WpfAnimatedGif.dll
  inflating: Newtonsoft.Json.dll
  inflating: PuzzleSolver.exe
  inflating: PuzzleSolver.exe.config
  inflating: System.Net.Http.Formatting.dll
root@kali:/media/sf_CTFs/shabak/Puzzle# file PuzzleSolver.exe
PuzzleSolver.exe: PE32+ executable (GUI) x86-64 Mono/.Net assembly, for MS Windows
```

So this is a .NET Windows executable. Let's try to run it:

![](images/puzzle1.png)

Since we weren't even asked to provide a username and password, and since this is a web challenge after all, let's run Wireshark and see if there's any network traffic in the background.

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# tshark -r capture.pcapng
Running as user "root" and group "root". This could be dangerous.
    1   0.000000    10.0.2.15 → 192.168.1.1  DNS 87 Standard query 0xb378 A puzzle.shieldchallenges.com 55192 53
    2   0.015371  192.168.1.1 → 10.0.2.15    DNS 219 Standard query response 0xb378 A puzzle.shieldchallenges.com CNAME a0a6f4ca2b1cd40c293e90254b1d8789-599794142.eu-central-1.elb.amazonaws.com A 18.195.143.130 A 52.28.127.52 A 18.158.71.177 53 55192
    3   0.107807    10.0.2.15 → 18.195.143.130 TCP 66 49940 → 80 [SYN] Seq=0 Win=64240 Len=0 MSS=1460 WS=256 SACK_PERM=1 49940 80
    4   0.224201 18.195.143.130 → 10.0.2.15    TCP 60 80 → 49940 [SYN, ACK] Seq=0 Ack=1 Win=65535 Len=0 MSS=1460 80 49940
    5   0.224316    10.0.2.15 → 18.195.143.130 TCP 54 49940 → 80 [ACK] Seq=1 Ack=1 Win=64240 Len=0 49940 80
    6   0.226310    10.0.2.15 → 18.195.143.130 HTTP 172 GET /api/credentials HTTP/1.1  49940 80
    7   0.226985 18.195.143.130 → 10.0.2.15    TCP 60 80 → 49940 [ACK] Seq=1 Ack=119 Win=65535 Len=0 80 49940
    8   0.348690 18.195.143.130 → 10.0.2.15    HTTP 272 HTTP/1.1 200 OK  (application/json) 80 49940
    9   0.396551    10.0.2.15 → 18.195.143.130 TCP 54 49940 → 80 [ACK] Seq=119 Ack=219 Win=64022 Len=0 49940 80
   10   6.134784    10.0.2.15 → 18.195.143.130 TCP 54 49940 → 80 [RST, ACK] Seq=119 Ack=219 Win=0 Len=0 49940 80
```

We see a call to `/api/credentials` in frame #6 and a response in frame #8:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# tshark -nr capture.pcapng  -qz follow,tcp,ascii,0
Running as user "root" and group "root". This could be dangerous.

===================================================================
Follow: tcp,ascii
Filter: tcp.stream eq 0
Node 0: 10.0.2.15:49940
Node 1: 18.195.143.130:80
118
GET /api/credentials HTTP/1.1
Accept: application/json
Host: puzzle.shieldchallenges.com
Connection: Keep-Alive


        218
HTTP/1.1 200 OK
Server: nginx/1.15.3
Date: Mon, 11 Jan 2021 21:14:10 GMT
Content-Type: application/json
Content-Length: 64
Connection: keep-alive

{"username": null, "password": null, "Error": "Deprecated API"}

===================================================================
```

The response received claims that the credentials API is deprecated.

Let's try it ourselves:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/credentials
{"username": null, "password": null, "Error": "Deprecated API"}
```

No luck here. We can't get the credentials from the network. But the application needs to check the credentials against something, right? So let's pop up dotPeek (a .NET disassembler) and check the code:

```csharp
namespace PuzzleSolver
{
  public class APIClient
  {
    private HttpClient client;
    private string targetAPI = "puzzle.shieldchallenges.com";
    private int targetPort = 80;

    public APIClient()
    {
      this.client = new HttpClient()
      {
        BaseAddress = new Uri(string.Format("http://{0}:{1}", (object) this.targetAPI, (object) this.targetPort))
      };
      this.client.DefaultRequestHeaders.Accept.Clear();
      this.client.DefaultRequestHeaders.Accept.Add(new MediaTypeWithQualityHeaderValue("application/json"));
    }

    public async Task<bool> ValidateCredentialsAsync()
    {
      HttpResponseMessage async = await this.client.GetAsync("/api/credentials");
      if (async.IsSuccessStatusCode)
      {
        Credentials credentials = await async.Content.ReadAsAsync<Credentials>();
        if ("Test_User" == credentials.Username && "SecretPassw0rd" == credentials.Password)
          return true;
      }
      return false;
    }

    public async Task<Puzzle[]> GetPuzzlesAsync()
    {
      string requestUri = "/api/puzzle";
      PuzzleList puzzleList = (PuzzleList) null;
      HttpResponseMessage async = await this.client.GetAsync(requestUri);
      if (async.IsSuccessStatusCode)
        puzzleList = await async.Content.ReadAsAsync<PuzzleList>();
      Puzzle[] puzzles = puzzleList.puzzles;
      puzzleList = (PuzzleList) null;
      return puzzles;
    }

    public void TODO_GetToken(string username, string password, int puzzleID)
    {
      int num = (int) MessageBox.Show("Error - Not implemented yet", "/api/token");
    }

    public void TODO_PatchPuzzle(int puzzleID, string token)
    {
      int num = (int) MessageBox.Show("Error - Not implemented yet", string.Format("/api/puzzle/{0}", (object) puzzleID));
    }
  }
}
```

So we can see that the program stores the expected username and password in plaintext: `Test_User:SecretPassw0rd`. We also see a few more APIs which we'll get to later on. For now, let's try to spoof the API server in order to provide the program with the credentials it wants. For that, we edit our `hosts` file and add an entry for `127.0.0.1 puzzle.shieldchallenges.com`. Then, we launch the following Python script:

```python
from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import urlparse
import json

class RequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        parsed_path = urlparse(self.path)
        self.send_response(200)
        self.send_header('Content-type', 'application/json')
        self.end_headers()
        print(parsed_path)
        if parsed_path.path == "/api/credentials":
            self.wfile.write(json.dumps({
                'username': "Test_User",
                'password': "SecretPassw0rd"
            }).encode())


if __name__ == '__main__':
    server = HTTPServer(('localhost', 80), RequestHandler)
    print('Starting server at http://localhost:80')
    server.serve_forever()
```

Let's start the program again:

![](images/puzzle2.png)

This is closer but we still have that gray box in the middle to deal with. If we check the fake-server log, we can see that the program made another request to a different API:

```console
Starting server at http://localhost:80
127.0.0.1 - - [17/Jan/2021 22:16:17] "GET /api/credentials HTTP/1.1" 200 -
ParseResult(scheme='', netloc='', path='/api/credentials', params='', query='', fragment='')
127.0.0.1 - - [17/Jan/2021 22:16:45] "GET /api/puzzle HTTP/1.1" 200 -
ParseResult(scheme='', netloc='', path='/api/puzzle', params='', query='', fragment='')
```

So we can check what's there and return it:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/puzzle
{"puzzles": [{"id": 0, "name": "SpaceInvaders", "rows": "1 1,1 1,7,2 3 2,11,1 7 1,1 1 1 1,2 2,0", "columns": "3,2,1 5,2 2 1,4 1,4,4 1,2 2 1,1 5,2,3"}, {"id": 1, "name": "X", "rows": "1 1,1,1 1", "columns": "1 1,1,1 1"}, {"id": 2, "name": "1337", "rows": "1 3 3 3,1 1 1 1,1 3 3 1,1 1 1 1,1 3 3 1", "columns": "5,0,1 1 1,1 1 1,5,0,1 1 1,1 1 1,5,0,1,1 2,3"}, {"id": 3, "name": "TopSecret", "rows": "7,12,3 2 2 4,4 2 2 2,2 2 2 2 2,3 2 2 2 2,4 2 2 2 2,4 2 2 2 2,1 2 2 2 2 2,2 2 2 2 2 2,2 2 2 2 2 2,4 2 2 2 2,3 2 2 2 2,2 2 2 2 2,2 4 2 2 2,5 2 2 2,3 2 2 2, 2 3 2 2,7 2 2,4 3 2,2 3 2,2 4 3,4 3,13,9", "columns": "8,13,3 2 5,8 2 3,8 2 4,2 2 2 2,13 2 2,12 2 3,1 2 4,17 2 2,16 3 2,2 2 2,2 3 2,19 2,17 2,2 2,2 2,2 2,2 2,2 2,2 2,2 3,3 3,13,9"}, {"id": 4, "name": "StarOfDavid", "rows": "1,3,2 2,13,1 2 2 1,2 2,2 2,2 2,2 2,1 2 2 1,13,2 2,3,1", "columns": "1 1,2 2,1 4 1,8,3 3,3 3,2 1 1 2,3 3,3 3,8,1 4 1,2 2,1 1"}, {"id": 5, "name": "QR", "rows": "7 2 1 7,1 1 1 1 1 1,1 3 1 1 1 1 1 3 1,1 3 1 1 2 1 3 1,1 3 1 3 1 3 1,1 1 1 1,7 1 1 1 7,2,4 1 1 1 1 3 1,1 2 1 1 1 1 3,1 1 2 1 1 5 3,2 3 2 1,2 1 2 1 2 1 1 1 1,1 1 1 1 1 1,7 2 1 2,1 1 6 1 1,1 3 1 2 1 1 1,1 3 1 1 1 1 1,1 3 1 1 1 3,1 1 5 1 2 2,7 2 2 1 1 1", "columns": "7 1 1 1 7,1 1 1 1 1 1,1 3 1 2 1 3 1,1 3 1 1 1 1 1 3 1,1 3 1 1 1 1 3 1,1 1 4 1 1,7 1 1 1 7,1 1,1 3 1 1 4 4,2 1 1 1 1 2,1 1 5 2 2 1,1 1 2 3,4 1 3 1 2,1 6 1,7 1 3 1,1 1 2 1 1,1 3 1 1 1 1 1 3,1 3 1 2 2 3,1 3 1 3 3 1,1 1 3 2 1 2,7 1 1 2 1 1 "}]}
```

We can modify our script to return this when the server is queried for `/api/puzzle` and try to run the program again:

![](images/puzzle3.png)

Now it's clear what this is - a [Nonogram](https://en.wikipedia.org/wiki/Nonogram) solver! Unfortunately, none of the buttons really work, and the only thing that seems to work is the dropdown menu that allows the user to move between the different puzzles. So, it looks like we won't be able to use the puzzle solver to solve this challenge.

Fortunately, there are other Nonogram solvers out there, such as [this one](https://github.com/fmakdemir/nonogram-solver/blob/master/nonogramsolver.py). Let's use it to solve the Nonograms.

```python
from enum import Enum
from pwn import *

# https://github.com/fmakdemir/nonogram-solver/blob/master/nonogramsolver.py
import nonogramsolver
import requests
import json
import io

from unittest.mock import patch, mock_open

BASE_URL = "http://puzzle.shieldchallenges.com"

def get_puzzles():
    res = {}
    r = requests.get(f"{BASE_URL}/api/puzzle")
    puzzles = r.json()
    for puzzle in puzzles["puzzles"]:
        res[str(puzzle["id"])] = puzzle
    return res

def translate_puzzle(service_format):
    nonogram_solver_format = {
        "r" : [[int(x) for x in row.strip(" ").split(" ")] for row in service_format["rows"].split(",")],
        "c" : [[int(x) for x in column.strip(" ").split(" ")] for column in service_format["columns"].split(",")],
    }

    return nonogram_solver_format

def solve(puzzle):
    with log.progress('Solving Nonogram...'):
        puzzle_data = json.dumps(translate_puzzle(puzzle))
        with patch("builtins.open", mock_open(read_data = puzzle_data)) as mock_file:
            solver = nonogramsolver.NonogramSolver(mock_file)
            solver.solve()
            return solver

def print_solution(solution):
    print("")
    print(str(solution).replace("X", u"\u2588\u2588").replace(".", "  "))
    print("")

def main():
    puzzles = get_puzzles()
    
    print("Available puzzles:")
    for puzzle in puzzles.values():
        print(f'\t{puzzle["id"]}: {puzzle["name"]}')
    
    puzzle_id = ""

    while puzzle_id not in puzzles.keys():
        puzzle_id = input("Please enter requested puzzle ID: ").strip()

    print(f"Selected puzzle: {puzzles[puzzle_id]['name']}")
    solution = solve(puzzles[puzzle_id])
    print_solution(solution)

if __name__ == "__main__":
    main()
```

The most intriguing puzzles are `QR` and `TopSecret`, let's solve them:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# python3 solve.py
Available puzzles:
        0: SpaceInvaders
        1: X
        2: 1337
        3: TopSecret
        4: StarOfDavid
        5: QR
Please enter requested puzzle ID: 3
Selected puzzle: TopSecret
                  ██████████████
              ████████████████████████
          ██████  ████    ████  ████████
        ████████  ████    ████        ████
      ████  ████  ████    ████          ████
    ██████  ████  ████    ████            ████
  ████████  ████  ████    ████              ████
  ████████  ████  ████    ████              ████
  ██  ████  ████  ████    ████                ████
████  ████  ████  ████    ████                ████
████  ████  ████  ████    ████                ████
████████    ████  ████    ████                ████
██████      ████  ████    ████                ████
████      ████    ████    ████                ████
████  ████████    ████    ████                ████
██████████        ████    ████                ████
██████          ████      ████                ████
  ████      ██████      ████                ████
  ██████████████        ████                ████
    ████████        ██████                ████
      ████        ██████                ████
        ████  ████████                ██████
          ████████                ██████
            ██████████████████████████
                ██████████████████
root@kali:/media/sf_CTFs/shabak/Puzzle# python3 solve.py
Available puzzles:
        0: SpaceInvaders
        1: X
        2: 1337
        3: TopSecret
        4: StarOfDavid
        5: QR
Please enter requested puzzle ID: 5
Selected puzzle: QR
██████████████  ████    ██  ██████████████
██          ██    ██    ██  ██          ██
██  ██████  ██  ██  ██  ██  ██  ██████  ██
██  ██████  ██  ██    ████  ██  ██████  ██
██  ██████  ██  ██████      ██  ██████  ██
██          ██              ██          ██
██████████████  ██  ██  ██  ██████████████
                  ████
████████    ██  ██  ██    ██    ██████  ██
    ██  ████  ██    ██  ██    ██  ██████
██    ██  ████  ██  ██  ██████████  ██████
        ████  ██████    ████          ██
████  ██  ████  ██  ████  ██    ██  ██  ██
                ██  ██    ██  ██    ██  ██
██████████████            ████  ██  ████
██          ██    ████████████    ██  ██
██  ██████  ██      ████    ██    ██    ██
██  ██████  ██  ██        ██    ██    ██
██  ██████  ██  ██    ██        ██████
██          ██  ██████████  ██  ████  ████
██████████████  ████  ████    ██  ██  ██
```

If you're wondering, the QR code maps to `1337`. So now what?

It's time to take another look at the additional APIs that the source code has revealed to us:

```csharp
    public void TODO_GetToken(string username, string password, int puzzleID)
    {
      int num = (int) MessageBox.Show("Error - Not implemented yet", "/api/token");
    }

    public void TODO_PatchPuzzle(int puzzleID, string token)
    {
      int num = (int) MessageBox.Show("Error - Not implemented yet", string.Format("/api/puzzle/{0}", (object) puzzleID));
    }
```

They aren't implemented, but we can try to access them ourselves. The parameters of the function are a hint towards the parameters that should be sent as part of the request.

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/token
{"message": {"username": "Missing required parameter in the JSON body"}}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/token -H "Content-Type: application/json" --data '{"username":"Test_User","password":"SecretPassw0rd", "puzzle_id": 1}' --request GET
{"token": "eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3Mzc2OSwiZXhwIjoxNjExMDczODI5fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjo2fQ.-GqiASSqrkChVjShGdjy8xsbEXNTMfSNrM-K7ItBOm5ASaVsinKfhDRMyMMLw-q8TPyQgzYFARWUt1dfu--5RA", "duration": 60}
```

We got what looks like a JSON Web Token (JWT). Let's decode it:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# python3 ~/utils/jwt_tool/jwt_tool/jwt_tool.py eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3Mzc2OSwiZXhwIjoxNjExMDczODI5fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjo2fQ.-GqiASSqrkChVjShGdjy8xsbEXNTMfSNrM-K7ItBOm5ASaVsinKfhDRMyMMLw-q8TPyQgzYFARWUt1dfu--5RA

        \   \        \         \          \                    \
   \__   |   |  \     |\__    __| \__    __|                    |
         |   |   \    |      |          |       \         \     |
         |        \   |      |          |    __  \     __  \    |
  \      |      _     |      |          |   |     |   |     |   |
   |     |     / \    |      |          |   |     |   |     |   |
\        |    /   \   |      |          |\        |\        |   |
 \______/ \__/     \__|   \__|      \__| \______/  \______/ \__|
  Version 2.0                 \______|             @ticarpi

Original JWT:

=====================
Decoded Token Values:
=====================

Token header values:
[+] alg = "HS512"
[+] iat = 1611073769
[+] exp = 1611073829

Token payload values:
[+] puzzle_id = 1
[+] state = "........."
[+] cursor = 6

----------------------
JWT common timestamps:
iat = IssuedAt
exp = Expires
nbf = NotBefore
----------------------
```

So we have a cursor pointing at some location, and a `state` variable which interestingly contains the same amount of dots as the puzzle contains cells.

Let's move on the the next API:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/puzzle/1 -H "Content-Type: application/json" --request GET
{"message": "The method is not allowed for the requested URL."}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/puzzle/1 -H "Content-Type: application/json" --request POST
{"message": "The method is not allowed for the requested URL."}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/puzzle/1 -H "Content-Type: application/json" --request PATCH
{"message": "Invalid token"}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl http://puzzle.shieldchallenges.com/api/puzzle/1 -H "Content-Type: application/json" --request PATCH --data '{"token":"eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3Nzg2MSwiZXhwIjoxNjExMDc3OTIxfQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjo0fQ.YWQBUJYL2pCFnh2ov2W6KJWw2NC1kstOKFKDD6groBFOpG6HN-uQi11d_zz21zxYTDLT0WymM2C642gLIy4lxQ"}'
{"message": "Invalid token"}
```

So, as hinted by the C# method name, this API supports only `PATCH` calls, but it still won't accept our token. That it, at least until we find the correct way to provide it:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# curl --location --request PATCH 'http://puzzle.shieldchallenges.com/api/puzzle/1' --header 'Authorization: Bearer eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3ODEwOCwiZXhwIjoxNjExMDc4MTY4fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjozfQ.WPQGZWUo8Ohp6Tywh0Sy4PPl6sVnNqJHdiE8zVdNXEuO8BSq-Rbg7jiefnNvvBDogtran441shjkUTpghXWtIg'
{"message": {"instructions": "Missing required parameter 'instructions' in in the JSON body"}}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl --location --request PATCH 'http://puzzle.shieldchallenges.com/api/puzzle/1' --header 'Authorization: Bearer eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3ODEwOCwiZXhwIjoxNjExMDc4MTY4fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjozfQ.WPQGZWUo8Ohp6Tywh0Sy4PPl6sVnNqJHdiE8zVdNXEuO8BSq-Rbg7jiefnNvvBDogtran441shjkUTpghXWtIg' --data-raw '{"instructions": "test"}' --header 'Content-Type: application/json'
{"message": "Invalid hex encoding"}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl --location --request PATCH 'http://puzzle.shieldchallenges.com/api/puzzle/1' --header 'Authorization: Bearer eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3ODEwOCwiZXhwIjoxNjExMDc4MTY4fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjozfQ.WPQGZWUo8Ohp6Tywh0Sy4PPl6sVnNqJHdiE8zVdNXEuO8BSq-Rbg7jiefnNvvBDogtran441shjkUTpghXWtIg' --data-raw '{"instructions": "deadbeef"}' --header 'Content-Type: application/json'
{"message": "Instructions must be 3 bytes long"}
root@kali:/media/sf_CTFs/shabak/Puzzle# curl --location --request PATCH 'http://puzzle.shieldchallenges.com/api/puzzle/1' --header 'Authorization: Bearer eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3ODEwOCwiZXhwIjoxNjExMDc4MTY4fQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjozfQ.WPQGZWUo8Ohp6Tywh0Sy4PPl6sVnNqJHdiE8zVdNXEuO8BSq-Rbg7jiefnNvvBDogtran441shjkUTpghXWtIg' --data-raw '{"instructions": "aabbcc"}' --header 'Content-Type: application/json'
{"token": "eyJhbGciOiJIUzUxMiIsImlhdCI6MTYxMTA3ODE1MywiZXhwIjoxNjExMDc4MTYzfQ.eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uWC4uLi4uIiwiY3Vyc29yIjozfQ.fLTwzBmFYlsRMxB2_OAhke5ZCVhU5y42tALKbU9I0s3K_OhvQ_rsS8W7ipEHwki2y1IqWrft7b4XvMueEBJFig"}
```

So we now know that the instructions should be provided as a 3-byte hex-encoded string. But what do they do? Notice that when we provided a correctly formatted instruction, we received a new token. Let's compare both tokens:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# echo eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uLi4uLi4uIiwiY3Vyc29yIjozfQ | base64 -d
{"puzzle_id":1,"state":".........","cursor":3}
root@kali:/media/sf_CTFs/shabak/Puzzle# echo eyJwdXp6bGVfaWQiOjEsInN0YXRlIjoiLi4uWC4uLi4uIiwiY3Vyc29yIjozfQ | base64 -d
{"puzzle_id":1,"state":"...X.....","cursor":3}
```

We managed to draw an X somewhere. This must be the equivalent of coloring a Nonogram cell. With a good share of trial and error we can map the different instructions available:

| Instruction | Encoding |
| ----------- | -------- |
| Do nothing  | 0b000    |
| Move right  | 0b001    |
| Move up     | 0b010    |
| Go to 0     | 0b011    | 
| Move left   | 0b100    |
| Move down   | 0b101    |
| Set white   | 0b110    |
| Set black   | 0b111    |

But if there are only 8 3-bit instructions, why is there need for 3 whole bytes? Well, that must be the reason the field is called "instructions" and not "instruction" - instructions can be chained and up to 8 instructions can be sent at a time.

All that's left is to send the appropriate instructions to solve the puzzle:

```python
from enum import Enum
from pwn import *

# https://github.com/fmakdemir/nonogram-solver/blob/master/nonogramsolver.py
import nonogramsolver
import requests
import json
import io

from unittest.mock import patch, mock_open

BASE_URL = "http://puzzle.shieldchallenges.com"

def get_puzzles():
    res = {}
    r = requests.get(f"{BASE_URL}/api/puzzle")
    puzzles = r.json()
    for puzzle in puzzles["puzzles"]:
        res[str(puzzle["id"])] = puzzle
    return res

def translate_puzzle(service_format):
    nonogram_solver_format = {
        "r" : [[int(x) for x in row.strip(" ").split(" ")] for row in service_format["rows"].split(",")],
        "c" : [[int(x) for x in column.strip(" ").split(" ")] for column in service_format["columns"].split(",")],
    }

    return nonogram_solver_format

def solve(puzzle):
    with log.progress('Solving Nonogram...'):
        puzzle_data = json.dumps(translate_puzzle(puzzle))
        with patch("builtins.open", mock_open(read_data = puzzle_data)) as mock_file:
            solver = nonogramsolver.NonogramSolver(mock_file)
            solver.solve()
            return solver

def print_solution(solution):
    print("")
    print(str(solution).replace("X", u"\u2588\u2588").replace(".", "  "))
    print("")

def get_token(puzzle_id):
    r = requests.get(f'{BASE_URL}/api/token', 
        json = {"username": "Test_User",
                "password": "SecretPassw0rd", 
                "puzzle_id": puzzle_id
        })

    resp_json = r.json()
    return resp_json["token"]

def send_command(puzzle_id, token, command):
   r = requests.patch(f'http://puzzle.shieldchallenges.com/api/puzzle/{puzzle_id}',
   json = {"instructions": f"{command:06X}"}, 
   headers = {
      'Authorization': 'Bearer {}'.format(token),
      'Content-Type': 'application/json'
   })
   
   return r.json()

class Instructions(Enum):
    DO_NOTHING  = 0b000
    MOVE_RIGHT  = 0b001
    MOVE_UP     = 0b010
    GO_TO_ZERO  = 0b011
    MOVE_LEFT   = 0b100
    MOVE_DOWN   = 0b101
    SET_WHITE   = 0b110
    SET_BLACK   = 0b111

COMMAND_BYTE_LENGTH      = 3
BITS_PER_BYTE            = 8
INSTRUCTION_BIT_LENGTH   = 3
INSTRUCTIONS_PER_COMMAND = COMMAND_BYTE_LENGTH * BITS_PER_BYTE // INSTRUCTION_BIT_LENGTH

def main():
    puzzles = get_puzzles()
    
    print("Available puzzles:")
    for puzzle in puzzles.values():
        print(f'\t{puzzle["id"]}: {puzzle["name"]}')
    
    puzzle_id = ""

    while puzzle_id not in puzzles.keys():
        puzzle_id = input("Please enter requested puzzle ID: ").strip()

    print(f"Selected puzzle: {puzzles[puzzle_id]['name']}")
    solution = solve(puzzles[puzzle_id])
    print_solution(solution)

    instructions = [Instructions.GO_TO_ZERO]

    for row in range(len(solution.NONO)):
        for col in range(len(solution.NONO[row])):
            if solution.NONO[row][col] == 1:
                instructions.append(Instructions.SET_BLACK)
            instructions.append(Instructions.MOVE_RIGHT)
        instructions.append(Instructions.MOVE_DOWN)

    log.info(f"Number of instructions: {len(instructions)}")

    token = get_token(puzzle_id)

    with log.progress('Sending instructions...') as p:
        while ( (len(instructions) > 0) and (token is not None) ):
            command = 0
            try:
                for i in range(INSTRUCTIONS_PER_COMMAND):
                    instruction = instructions.pop(0)
                    command = command << INSTRUCTION_BIT_LENGTH
                    command |= instruction.value
            except IndexError:
                pass
            p.status(f"Raw command: {command:06X}")
            resp_json = send_command(puzzle_id, token, command)
            token = resp_json.get("token", None)

    log.success(f"Flag: {resp_json['flag']}")


if __name__ == "__main__":
    main()
```


Output:

```console
root@kali:/media/sf_CTFs/shabak/Puzzle# python3 solve.py
Available puzzles:
    0: SpaceInvaders
    1: X
    2: 1337
    3: TopSecret
    4: StarOfDavid
    5: QR
Please enter requested puzzle ID: 0
Selected puzzle: SpaceInvaders
[+] Solving Nonogram...: Done

    ██          ██
      ██      ██
    ██████████████
  ████  ██████  ████
██████████████████████
██  ██████████████  ██
██  ██          ██  ██
      ████  ████


[*] Number of instructions: 155
[+] Sending instructions...: Done
[+] Flag: Try_Harder
root@kali:/media/sf_CTFs/shabak/Puzzle# python3 solve.py
Available puzzles:
    0: SpaceInvaders
    1: X
    2: 1337
    3: TopSecret
    4: StarOfDavid
    5: QR
Please enter requested puzzle ID: 3
Selected puzzle: TopSecret
[+] Solving Nonogram...: Done

                  ██████████████
              ████████████████████████
          ██████  ████    ████  ████████
        ████████  ████    ████        ████
      ████  ████  ████    ████          ████
    ██████  ████  ████    ████            ████
  ████████  ████  ████    ████              ████
  ████████  ████  ████    ████              ████
  ██  ████  ████  ████    ████                ████
████  ████  ████  ████    ████                ████
████  ████  ████  ████    ████                ████
████████    ████  ████    ████                ████
██████      ████  ████    ████                ████
████      ████    ████    ████                ████
████  ████████    ████    ████                ████
██████████        ████    ████                ████
██████          ████      ████                ████
  ████      ██████      ████                ████
  ██████████████        ████                ████
    ████████        ██████                ████
      ████        ██████                ████
        ████  ████████                ██████
          ████████                ██████
            ██████████████████████████
                ██████████████████

[*] Number of instructions: 910
[+] Sending instructions...: Done
[+] Flag: FLAG{S01ving_Puzz13s_Is_Fun!}
```