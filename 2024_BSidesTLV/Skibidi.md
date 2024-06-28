# Skibidi

 * Category: Forensics
 * 70 Points
 * Solved by the JCTF Team

## Description

> My friend sent me some skibidi data... Can you help me understand his goofy sentences?

A `*.docx` file was attached.

## Solution

`*.docx` files are actually ZIP archives. We can extract the file and search through its
content. Eventually, we find the following file under `.\word\img.xml`:

```xml
<root>
  <person
  firstname="QnNpZGVzVExWMjAyNHt3M19k"
  lastname="M2YzNXQzZF90aDNfc2sxYjFkaX0="
  city="Haifa"
  country="Israel"
  firstname2="QnNpZGVzVExWMjAyNHt3M19k"
  lastname2="M2YzNXQzZF90aDNfc2sxYjFkaX0="
  email="Ulrike.Leffen@yopmail.com"
  />
  <random>6</random>
  <random_float>89.838</random_float>
  <bool>true</bool>
  <date>1986-09-28</date>
  <regEx>helloooooooooooooooooooooooooooooooooooooooooooo world</regEx>
  <enum>generator</enum>
  <elt>Alyssa</elt><elt>Flory</elt><elt>Ulrike</elt><elt>Teriann</elt><elt>Reeba</elt>  
  <Ulrike>
    <age>56</age>
  </Ulrike>
</root>
```

We decode the Base64 string and get the flag:

```python
>>> import base64
>>> base64.b64decode("QnNpZGVzVExWMjAyNHt3M19kM2YzNXQzZF90aDNfc2sxYjFkaX0=")
b'BsidesTLV2024{w3_d3f35t3d_th3_sk1b1di}'
```