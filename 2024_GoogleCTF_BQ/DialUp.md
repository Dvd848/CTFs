# DialUp

Category: Forensics

## Description

> It's a familiar sound to anyone who's ever dialed a phone. What's this caller trying to say?

A `*.wav` file, `dtmf_challenge.wav`, was attached.

## Solution

The WAV file contains DTMF tones, as the name suggests. Using a tool such as 
[this one](https://dtmf.netlify.app/), we can decode the content:

```
0001s 111..000..222..777..444..
0002s 000..444..555..333..666..
0003s 888..777..111..444..222..
0004s 888..555..222..333..111..
0005s 777..888..666..444..000..
0006s 999..888..777..888..444..
0007s 222..999..999..666..222..
0008s 666..111..888..333..222..
0009s 999..777..444..666..444..
0010s 111..000..000..222..222..
0011s 111..

Decoded: 102740453687142852317864098784299626183297464100221
```

Let's view that as ASCII:

```python
>>> from Crypto.Util.number import long_to_bytes
>>> long_to_bytes(102740453687142852317864098784299626183297464100221)
b'FLAG{can_you_hear_me}'
```