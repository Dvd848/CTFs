
# The ART of IOT 

 * Category: Forensics
 * Solved by the JCTF Team

## Description

> Why So Serial?

A binary file was attached, `The-ART-of-IOT.atkdl`.

## Solution

Let's check the file type:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/the_art_of_iot]
└─$ file The-ART-of-IOT.atkdl
The-ART-of-IOT.atkdl: Zip archive data, at least v2.0 to extract, compression method=deflate
```

It's just a zip file, we can extract it and inspect it:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/the_art_of_iot]
└─$ unzip The-ART-of-IOT.atkdl -d The-ART-of-IOT
Archive:  The-ART-of-IOT.atkdl
  inflating: The-ART-of-IOT/0/channel.ini
  inflating: The-ART-of-IOT/1/0-0.bin
  inflating: The-ART-of-IOT/1/0-1.bin
  inflating: The-ART-of-IOT/1/channel.ini
  inflating: The-ART-of-IOT/10/channel.ini
  inflating: The-ART-of-IOT/11/channel.ini
  inflating: The-ART-of-IOT/12/channel.ini
  inflating: The-ART-of-IOT/13/channel.ini
  inflating: The-ART-of-IOT/14/channel.ini
  inflating: The-ART-of-IOT/15/channel.ini
  inflating: The-ART-of-IOT/2/channel.ini
  inflating: The-ART-of-IOT/3/channel.ini
  inflating: The-ART-of-IOT/4/channel.ini
  inflating: The-ART-of-IOT/5/channel.ini
  inflating: The-ART-of-IOT/6/channel.ini
  inflating: The-ART-of-IOT/7/channel.ini
  inflating: The-ART-of-IOT/8/channel.ini
  inflating: The-ART-of-IOT/9/channel.ini
  inflating: The-ART-of-IOT/channel.ini
  inflating: The-ART-of-IOT/set.ini
  inflating: The-ART-of-IOT/vernier.ini
```

Among the different files, we find this one:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/the_art_of_iot]
└─$ cat The-ART-of-IOT/channel.ini
SessionName=DL16 Plus
SamplingFrequency=20000
SamplingDepth=40000000
TriggerSamplingDepth=400000
```

The important hint here is "DL16 Plus", which is the name of a logic analyzer.  
We can download its matching software and open the `*.atkdl` file with it.

Using the software, we can inspect some signal captured by the logic analyzer. 
But what does it represent? Based on the challenge name and description, there's
a good chance this is UART:

> UART (Universal Asynchronous Receiver/Transmitter) is a hardware communication 
> protocol used to transmit and receive serial data between devices, typically 
> over two wires (TX and RX), without needing a shared clock.

We can try and decode the output manually, but for a common protocol such as UART the 
program has a built-in decoder. So, we select Decoder -> Protocol -> UART, and play around with the configuration. Eventually, we'll arrive to:

 * RX: D1 - Channel 1
 * TX: -
 * Baud Rate: 115200
 * Data bits: 8
 * Parity: None
 * Stop bits: 0.0
 * Bit order: lsb-first
 * Data format: ASCII
 * Show start and stop bit: No

We export the result and get:

<details>

<summary>Click to expand...</summary>

```
Index,Start,Time,Type,Data,
0,272.309150 ms,8.650000 μs,RX bits,0,
1,272.317800 ms,8.700000 μs,RX bits,0,
2,272.326500 ms,8.650000 μs,RX bits,0,
3,272.335150 ms,8.700000 μs,RX bits,0,
4,272.343850 ms,8.700000 μs,RX bits,0,
5,272.352550 ms,8.650000 μs,RX bits,0,
6,272.361200 ms,8.700000 μs,RX bits,0,
7,272.369900 ms,8.650000 μs,RX bits,0,
8,272.309150 ms,69.400000 μs,RX data,[00],
9,272.300450 ms,253.944850 ms,RX breaks,Break condition,
10,526.383250 ms,8.650000 μs,RX bits,1,
11,526.391900 ms,8.700000 μs,RX bits,0,
12,526.400600 ms,8.650000 μs,RX bits,0,
13,526.409250 ms,8.700000 μs,RX bits,0,
14,526.417950 ms,8.700000 μs,RX bits,0,
15,526.426650 ms,8.650000 μs,RX bits,0,
16,526.435300 ms,8.700000 μs,RX bits,1,
17,526.444000 ms,8.650000 μs,RX bits,0,
18,526.383250 ms,69.400000 μs,RX data,A,
19,526.470050 ms,8.650000 μs,RX bits,0,
20,526.478700 ms,8.700000 μs,RX bits,0,
21,526.487400 ms,8.650000 μs,RX bits,0,
22,526.496050 ms,8.700000 μs,RX bits,0,
23,526.504750 ms,8.700000 μs,RX bits,1,
24,526.513450 ms,8.650000 μs,RX bits,1,
25,526.522100 ms,8.700000 μs,RX bits,1,
26,526.530800 ms,8.650000 μs,RX bits,0,
27,526.470050 ms,69.400000 μs,RX data,p,
28,526.556800 ms,8.650000 μs,RX bits,0,
29,526.565450 ms,8.700000 μs,RX bits,0,
30,526.574150 ms,8.650000 μs,RX bits,0,
31,526.582800 ms,8.700000 μs,RX bits,0,
32,526.591500 ms,8.700000 μs,RX bits,1,
33,526.600200 ms,8.650000 μs,RX bits,1,
34,526.608850 ms,8.700000 μs,RX bits,1,
35,526.617550 ms,8.650000 μs,RX bits,0,
36,526.556800 ms,69.400000 μs,RX data,p,
37,526.643650 ms,8.650000 μs,RX bits,1,
38,526.652300 ms,8.700000 μs,RX bits,1,
39,526.661000 ms,8.650000 μs,RX bits,0,
40,526.669650 ms,8.700000 μs,RX bits,0,
41,526.678350 ms,8.700000 μs,RX bits,1,
42,526.687050 ms,8.650000 μs,RX bits,0,
43,526.695700 ms,8.700000 μs,RX bits,1,
44,526.704400 ms,8.650000 μs,RX bits,0,
45,526.643650 ms,69.400000 μs,RX data,S,
46,526.730400 ms,8.650000 μs,RX bits,1,
47,526.739050 ms,8.700000 μs,RX bits,0,
48,526.747750 ms,8.650000 μs,RX bits,1,
49,526.756400 ms,8.700000 μs,RX bits,0,
50,526.765100 ms,8.700000 μs,RX bits,0,
51,526.773800 ms,8.650000 μs,RX bits,1,
52,526.782450 ms,8.700000 μs,RX bits,1,
53,526.791150 ms,8.650000 μs,RX bits,0,
54,526.730400 ms,69.400000 μs,RX data,e,
55,526.817200 ms,8.650000 μs,RX bits,1,
56,526.825850 ms,8.700000 μs,RX bits,1,
57,526.834550 ms,8.650000 μs,RX bits,0,
58,526.843200 ms,8.700000 μs,RX bits,0,
59,526.851900 ms,8.700000 μs,RX bits,0,
60,526.860600 ms,8.650000 μs,RX bits,1,
61,526.869250 ms,8.700000 μs,RX bits,1,
62,526.877950 ms,8.650000 μs,RX bits,0,
63,526.817200 ms,69.400000 μs,RX data,c,
64,526.904000 ms,8.650000 μs,RX bits,1,
65,526.912650 ms,8.700000 μs,RX bits,0,
66,526.921350 ms,8.650000 μs,RX bits,1,
67,526.930000 ms,8.700000 μs,RX bits,1,
68,526.938700 ms,8.700000 μs,RX bits,0,
69,526.947400 ms,8.650000 μs,RX bits,1,
70,526.956050 ms,8.700000 μs,RX bits,0,
71,526.964750 ms,8.650000 μs,RX bits,0,
72,526.904000 ms,69.400000 μs,RX data,-,
73,526.990800 ms,8.650000 μs,RX bits,1,
74,526.999450 ms,8.700000 μs,RX bits,0,
75,527.008150 ms,8.650000 μs,RX bits,0,
76,527.016800 ms,8.700000 μs,RX bits,1,
77,527.025500 ms,8.700000 μs,RX bits,0,
78,527.034200 ms,8.650000 μs,RX bits,0,
79,527.042850 ms,8.700000 μs,RX bits,1,
80,527.051550 ms,8.650000 μs,RX bits,0,
81,526.990800 ms,69.400000 μs,RX data,I,
82,527.077600 ms,8.650000 μs,RX bits,0,
83,527.086250 ms,8.700000 μs,RX bits,0,
84,527.094950 ms,8.650000 μs,RX bits,1,
85,527.103600 ms,8.700000 μs,RX bits,1,
86,527.112300 ms,8.700000 μs,RX bits,0,
87,527.121000 ms,8.650000 μs,RX bits,0,
88,527.129650 ms,8.700000 μs,RX bits,1,
89,527.138350 ms,8.650000 μs,RX bits,0,
90,527.077600 ms,69.400000 μs,RX data,L,
91,527.164400 ms,8.650000 μs,RX bits,1,
92,527.173050 ms,8.700000 μs,RX bits,1,
93,527.181750 ms,8.650000 μs,RX bits,0,
94,527.190400 ms,8.700000 μs,RX bits,1,
95,527.199100 ms,8.700000 μs,RX bits,1,
96,527.207800 ms,8.650000 μs,RX bits,1,
97,527.216450 ms,8.700000 μs,RX bits,1,
98,527.225150 ms,8.650000 μs,RX bits,0,
99,527.164400 ms,69.400000 μs,RX data,{,
100,527.251200 ms,8.650000 μs,RX bits,0,
101,527.259850 ms,8.700000 μs,RX bits,1,
102,527.268550 ms,8.650000 μs,RX bits,1,
103,527.277200 ms,8.700000 μs,RX bits,0,
104,527.285900 ms,8.700000 μs,RX bits,0,
105,527.294600 ms,8.650000 μs,RX bits,1,
106,527.303250 ms,8.700000 μs,RX bits,1,
107,527.311950 ms,8.650000 μs,RX bits,0,
108,527.251200 ms,69.400000 μs,RX data,f,
109,527.338000 ms,8.650000 μs,RX bits,0,
110,527.346650 ms,8.700000 μs,RX bits,1,
111,527.355350 ms,8.650000 μs,RX bits,0,
112,527.364000 ms,8.700000 μs,RX bits,0,
113,527.372700 ms,8.700000 μs,RX bits,1,
114,527.381400 ms,8.650000 μs,RX bits,1,
115,527.390050 ms,8.700000 μs,RX bits,1,
116,527.398750 ms,8.650000 μs,RX bits,0,
117,527.338000 ms,69.400000 μs,RX data,r,
118,527.424800 ms,8.650000 μs,RX bits,0,
119,527.433450 ms,8.700000 μs,RX bits,0,
120,527.442150 ms,8.650000 μs,RX bits,0,
121,527.450800 ms,8.700000 μs,RX bits,0,
122,527.459500 ms,8.700000 μs,RX bits,1,
123,527.468200 ms,8.650000 μs,RX bits,1,
124,527.476850 ms,8.700000 μs,RX bits,0,
125,527.485550 ms,8.650000 μs,RX bits,0,
126,527.424800 ms,69.400000 μs,RX data,0,
127,527.511600 ms,8.650000 μs,RX bits,1,
128,527.520250 ms,8.700000 μs,RX bits,0,
129,527.528950 ms,8.650000 μs,RX bits,1,
130,527.537600 ms,8.700000 μs,RX bits,1,
131,527.546300 ms,8.700000 μs,RX bits,0,
132,527.555000 ms,8.650000 μs,RX bits,1,
133,527.563650 ms,8.700000 μs,RX bits,1,
134,527.572350 ms,8.650000 μs,RX bits,0,
135,527.511600 ms,69.400000 μs,RX data,m,
136,527.598400 ms,8.650000 μs,RX bits,1,
137,527.607050 ms,8.700000 μs,RX bits,1,
138,527.615750 ms,8.650000 μs,RX bits,1,
139,527.624400 ms,8.700000 μs,RX bits,1,
140,527.633100 ms,8.700000 μs,RX bits,1,
141,527.641800 ms,8.650000 μs,RX bits,0,
142,527.650450 ms,8.700000 μs,RX bits,1,
143,527.659150 ms,8.650000 μs,RX bits,0,
144,527.598400 ms,69.400000 μs,RX data,_,
145,527.685200 ms,8.650000 μs,RX bits,1,
146,527.693850 ms,8.700000 μs,RX bits,1,
147,527.702550 ms,8.650000 μs,RX bits,1,
148,527.711200 ms,8.700000 μs,RX bits,0,
149,527.719900 ms,8.700000 μs,RX bits,1,
150,527.728600 ms,8.650000 μs,RX bits,1,
151,527.737250 ms,8.700000 μs,RX bits,1,
152,527.745950 ms,8.650000 μs,RX bits,0,
153,527.685200 ms,69.400000 μs,RX data,w,
154,527.772000 ms,8.650000 μs,RX bits,1,
155,527.780650 ms,8.700000 μs,RX bits,0,
156,527.789350 ms,8.650000 μs,RX bits,0,
157,527.798000 ms,8.700000 μs,RX bits,0,
158,527.806700 ms,8.700000 μs,RX bits,1,
159,527.815400 ms,8.650000 μs,RX bits,1,
160,527.824050 ms,8.700000 μs,RX bits,0,
161,527.832750 ms,8.650000 μs,RX bits,0,
162,527.772000 ms,69.400000 μs,RX data,1,
163,527.858800 ms,8.650000 μs,RX bits,0,
164,527.867450 ms,8.700000 μs,RX bits,1,
165,527.876150 ms,8.650000 μs,RX bits,0,
166,527.884800 ms,8.700000 μs,RX bits,0,
167,527.893500 ms,8.700000 μs,RX bits,1,
168,527.902200 ms,8.650000 μs,RX bits,1,
169,527.910850 ms,8.700000 μs,RX bits,1,
170,527.919550 ms,8.650000 μs,RX bits,0,
171,527.858800 ms,69.400000 μs,RX data,r,
172,527.945550 ms,8.650000 μs,RX bits,1,
173,527.954200 ms,8.700000 μs,RX bits,1,
174,527.962900 ms,8.650000 μs,RX bits,0,
175,527.971550 ms,8.700000 μs,RX bits,0,
176,527.980250 ms,8.700000 μs,RX bits,1,
177,527.988950 ms,8.650000 μs,RX bits,1,
178,527.997600 ms,8.700000 μs,RX bits,0,
179,528.006300 ms,8.650000 μs,RX bits,0,
180,527.945550 ms,69.400000 μs,RX data,3,
181,528.032400 ms,8.650000 μs,RX bits,1,
182,528.041050 ms,8.700000 μs,RX bits,1,
183,528.049750 ms,8.650000 μs,RX bits,1,
184,528.058400 ms,8.700000 μs,RX bits,1,
185,528.067100 ms,8.700000 μs,RX bits,1,
186,528.075800 ms,8.650000 μs,RX bits,0,
187,528.084450 ms,8.700000 μs,RX bits,1,
188,528.093150 ms,8.650000 μs,RX bits,0,
189,528.032400 ms,69.400000 μs,RX data,_,
190,528.119150 ms,8.650000 μs,RX bits,0,
191,528.127800 ms,8.700000 μs,RX bits,0,
192,528.136500 ms,8.650000 μs,RX bits,1,
193,528.145150 ms,8.700000 μs,RX bits,0,
194,528.153850 ms,8.700000 μs,RX bits,1,
195,528.162550 ms,8.650000 μs,RX bits,1,
196,528.171200 ms,8.700000 μs,RX bits,1,
197,528.179900 ms,8.650000 μs,RX bits,0,
198,528.119150 ms,69.400000 μs,RX data,t,
199,528.205950 ms,8.650000 μs,RX bits,1,
200,528.214600 ms,8.700000 μs,RX bits,1,
201,528.223300 ms,8.650000 μs,RX bits,1,
202,528.231950 ms,8.700000 μs,RX bits,1,
203,528.240650 ms,8.700000 μs,RX bits,0,
204,528.249350 ms,8.650000 μs,RX bits,1,
205,528.258000 ms,8.700000 μs,RX bits,1,
206,528.266700 ms,8.650000 μs,RX bits,0,
207,528.205950 ms,69.400000 μs,RX data,o,
208,528.292750 ms,8.650000 μs,RX bits,1,
209,528.301400 ms,8.700000 μs,RX bits,1,
210,528.310100 ms,8.650000 μs,RX bits,1,
211,528.318750 ms,8.700000 μs,RX bits,1,
212,528.327450 ms,8.700000 μs,RX bits,1,
213,528.336150 ms,8.650000 μs,RX bits,0,
214,528.344800 ms,8.700000 μs,RX bits,1,
215,528.353500 ms,8.650000 μs,RX bits,0,
216,528.292750 ms,69.400000 μs,RX data,_,
217,528.379550 ms,8.650000 μs,RX bits,0,
218,528.388200 ms,8.700000 μs,RX bits,1,
219,528.396900 ms,8.650000 μs,RX bits,1,
220,528.405550 ms,8.700000 μs,RX bits,0,
221,528.414250 ms,8.700000 μs,RX bits,0,
222,528.422950 ms,8.650000 μs,RX bits,1,
223,528.431600 ms,8.700000 μs,RX bits,1,
224,528.440300 ms,8.650000 μs,RX bits,0,
225,528.379550 ms,69.400000 μs,RX data,f,
226,528.466350 ms,8.650000 μs,RX bits,0,
227,528.475000 ms,8.700000 μs,RX bits,0,
228,528.483700 ms,8.650000 μs,RX bits,1,
229,528.492350 ms,8.700000 μs,RX bits,1,
230,528.501050 ms,8.700000 μs,RX bits,0,
231,528.509750 ms,8.650000 μs,RX bits,1,
232,528.518400 ms,8.700000 μs,RX bits,1,
233,528.527100 ms,8.650000 μs,RX bits,0,
234,528.466350 ms,69.400000 μs,RX data,l,
235,528.553150 ms,8.650000 μs,RX bits,1,
236,528.561800 ms,8.700000 μs,RX bits,0,
237,528.570500 ms,8.650000 μs,RX bits,0,
238,528.579150 ms,8.700000 μs,RX bits,0,
239,528.587850 ms,8.700000 μs,RX bits,0,
240,528.596550 ms,8.650000 μs,RX bits,1,
241,528.605200 ms,8.700000 μs,RX bits,1,
242,528.613900 ms,8.650000 μs,RX bits,0,
243,528.553150 ms,69.400000 μs,RX data,a,
244,528.639950 ms,8.650000 μs,RX bits,1,
245,528.648600 ms,8.700000 μs,RX bits,1,
246,528.657300 ms,8.650000 μs,RX bits,1,
247,528.665950 ms,8.700000 μs,RX bits,0,
248,528.674650 ms,8.700000 μs,RX bits,0,
249,528.683350 ms,8.650000 μs,RX bits,1,
250,528.692000 ms,8.700000 μs,RX bits,1,
251,528.700700 ms,8.650000 μs,RX bits,0,
252,528.639950 ms,69.400000 μs,RX data,g,
253,528.726750 ms,8.650000 μs,RX bits,1,
254,528.735400 ms,8.700000 μs,RX bits,0,
255,528.744100 ms,8.650000 μs,RX bits,1,
256,528.752750 ms,8.700000 μs,RX bits,1,
257,528.761450 ms,8.700000 μs,RX bits,1,
258,528.770150 ms,8.650000 μs,RX bits,1,
259,528.778800 ms,8.700000 μs,RX bits,1,
260,528.787500 ms,8.650000 μs,RX bits,0,
261,528.726750 ms,69.400000 μs,RX data,},
262,528.813550 ms,8.650000 μs,RX bits,1,
263,528.822200 ms,8.700000 μs,RX bits,0,
264,528.830900 ms,8.650000 μs,RX bits,1,
265,528.839550 ms,8.700000 μs,RX bits,1,
266,528.848250 ms,8.700000 μs,RX bits,0,
267,528.856950 ms,8.650000 μs,RX bits,0,
268,528.865600 ms,8.700000 μs,RX bits,0,
269,528.874300 ms,8.650000 μs,RX bits,0,
270,528.813550 ms,69.400000 μs,RX data,[0D],
271,528.900350 ms,8.650000 μs,RX bits,0,
272,528.909000 ms,8.700000 μs,RX bits,1,
273,528.917700 ms,8.650000 μs,RX bits,0,
274,528.926350 ms,8.700000 μs,RX bits,1,
275,528.935050 ms,8.700000 μs,RX bits,0,
276,528.943750 ms,8.650000 μs,RX bits,0,
277,528.952400 ms,8.700000 μs,RX bits,0,
278,528.961100 ms,8.650000 μs,RX bits,0,
279,528.900350 ms,69.400000 μs,RX data,[0A],
```

</details>


If we only select the "RX data" entries, we can construct the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/the_art_of_iot]
└─$ cat out.csv| grep "RX data" | awk -F',' '{ printf $5 }'
[00]AppSec-IL{fr0m_w1r3_to_flag}[0D][0A]
```
