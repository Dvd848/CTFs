# Biblical 2
Category: Cryptography

## Description
> My friend found another message! can you figure this one out too?

A text file was attached.

## Solution

Let's check the text file:

```console
root@kali:/media/sf_CTFs/technion/Biblical_2# cat biblical.txt
אנגליה אנחנו בוחנים בטמן האנגלי סולח נס גנבים אדמוני עם מגף חימום צאו לפח המון מבחני נמוך יעזוב קאהוט לאף קוטב מלכה חולני מיאמי מדגים חוק ימים יעזוב נאום דוחף אפל להביע סיום סיכה הפוכה אמזון אימן סטודיו מכון עולה סטייל חפוז כחולים יפה הופך ללבם ידועה סדנא חולני והודיע מטוס נאום מוגן לאיגודים וומבט וומבט וומבט ליפה
```

This is a list of unrelated works. Since this is a biblical cipher, we can try to calculate the [Gematria](https://en.wikipedia.org/wiki/Gematria) value of each word. 

> Gematria is an alphanumeric code of assigning a numerical value to a name, word or phrase based on its letters.

Script:

```python
# -*- coding: utf-8 -*-

mapping = {
     u'א': 1, 
     u'ב': 2, 
     u'ג': 3, 
     u'ד': 4, 
     u'ה': 5, 
     u'ו': 6, 
     u'ז': 7, 
     u'ח': 8, 
     u'ט': 9, 
     u'י': 10,
     u'כ': 20,
     u'ך': 20,
     u'ל': 30,
     u'מ': 40,
     u'ם': 40,
     u'נ': 50,
     u'ן': 50,
     u'ס': 60,
     u'ע': 70,
     u'פ': 80,
     u'ף': 80,
     u'צ': 90,
     u'ץ': 90,
     u'ק': 100,
     u'ר': 200,
     u'ש': 300,
     u'ת': 400
}

def calc_gematria(string):
    res = 0
    for char in string:
        res += mapping[char]
    return res

flag = []

with open("biblical.txt", encoding = "utf8") as f:
    words = f.read().split()
    for word in words:
        flag.append(calc_gematria(word))

    print("".join(chr(x) for x in flag))
```

Output:

```console
root@kali:/media/sf_CTFs/technion/Biblical_2# python3 solve.py
cstechnion{havent_you_heard_about_the_tower_of_shesach???}
```