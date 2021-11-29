# Course Crawling
Category: Misc., 250 points

## Description

> The zip file is encrypted using a name of one of the courses in the Technion, see if you can crack it. The password format is `<course number>:<course name in english>`
> 
> Note: Please try to crawl smartly and do not spam any site with request (we won't take any responsibility if you do so).

A zip file was attached.

## Solution

The easiest way to crawl the English Technion course names is using their official [search engine](https://students.technion.ac.il/local/technionsearch/search?lang=en). It is session-based, so we basically need to open it in a browser and just perform a simple search with the default parameters. This action redirects us to `https://students.technion.ac.il/local/technionsearch/results`, but if we grab the session ID and use it for a cURL request, we can access the same data via the command line.

Each course name is included in a `div` such as:

```html
<div class="d-flex w-100 justify-content-between">
    <h6 class="mb-1" style="font-weight: bold;">
        14003 - Statistics | Winter 2021/22
    </h6>
    <small>
            |Undergraduate Studies
    </small>
</div>
```

After applying some bash magic, we can get the course list for each search page:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Course_Crawling]
└─$  curl 'https://students.technion.ac.il/local/technionsearch/results?page=0' -X POST -H 'Cookie: MoodleSessionstudentsprod=jr4nar1q2md2a3a7tae043mqae'  -s | grep "<h6" -A 1 | grep "|" | egrep -o '^[^|]+' | sed -e 's/^[[:space:]]*//'
14003 - Statistics
14004 - System Analysis
14005 - Engineering Laboratory
14006 - Introduction to Numerical Methods
14008 - Graphical and Engineering Information
14010 - Engineering For Developing Communities
14101 - Project in Structures
14103 - Intro.to Engineering Mechanics
14107 - Introduction to The Theory of Elasticity
14108 - Structural Analysis
```

Now we just need to loop for each search page result. From our search via the browser we can see that currently there are 440 result pages.

```bash
#!/bin/bash

output_file=course_list.txt

echo "" > $output_file

for i in {0..440}
do
    echo -ne "$i\r"
    curl "https://students.technion.ac.il/local/technionsearch/results?page=$i" -X POST -H 'Cookie: MoodleSessionstudentsprod=jr4nar1q2md2a3a7tae043mqae'  -s | grep "<h6" -A 1 | grep "|" | egrep -o '^[^|]+' | sed -e 's/^[[:space:]]*//' >> $output_file
    sleep 0.1
done

head $output_file
```

Once we have all the course names and numbers, all that's left is to try them all:

```python
from pwn import *
import zipfile
import zlib

pwd_filename = "course_list.txt"
zip_filename = "course_crawling.zip"

with open(pwd_filename, "r") as passwords, log.progress("Brute forcing password") as p:
    zip_file = zipfile.ZipFile(zip_filename)

    for line in passwords:
        password = line.strip()

        course_number, course_name = password.split(" - ", maxsplit = 1)
        password = ":".join([course_number, course_name])

        try:
            p.status(f"Trying {password}")
            zip_file.extractall(path="Output",  pwd=bytes(password, 'utf-8'))
            log.success(f"Password Found: {password}")
            break
        
        except (RuntimeError, zlib.error):
            continue
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Course_Crawling]
└─$ python3 solve.py
[+] Brute forcing password: Done                                                                                                                                                                                 [+] Password Found: 324432:Psychology of Music
```

And the contents:

```console
┌──(user@kali)-[/media/sf_CTFs/technion/Course_Crawling]
└─$ cat Output/course_crawling.txt
cstechnion{waiting_for_a_course_on_CTFs}
```