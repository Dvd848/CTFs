# CyberTank AutoPilot
Miscellaneous

## Geo distance (100 points)

> CyberTank's autopilot backup system is not responding. Can you respond to the backup system's distance calculation? 
> 
> Flag format: CTF{32-hex}

A TCP server address was attached. Let's connect to it:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_AutoPilot# nc portal.hackazon.org 17001

            __.---.___
        ___/__\_O_/___\___
       /___ __________ ___\
       |===|\________/|===|
_______|===|__________|===|______

   CYBERTANK AutoPilot System


[X] We are in Republic of Iceland
[X] Current position: (-17.990612, 63.647031)
[X] Navigating to: (-16.815328, 64.150291)
[?] Distance (km):
```

Looks like we need to provide the distance between two coordinates. These coordinates look like longitude and latitude:

```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_AutoPilot# curl -s https://geocode.xyz/63.647031,-17.990612?json=1 | tr ',' '\n' | grep country
   "country" : "Iceland"
```

The country is Iceland like the server claimed.

We can use a Python library like `geopy` to calculate the correct distance:

```python
from pwn import *
import geopy.distance
import re

def get_coordinates(line):
    match = re.search(r"[^(]+\(([^)]+), ([^)]+)\)", line)
    if match:
        long = match.group(1)
        lat  = match.group(2)
        return long, lat
    raise Exception("Can't find coordinates in '{}'".format(line))

def calculate_distance(lon1, lat1, lon2, lat2):
    distance = geopy.distance.distance((lon1, lat1), (lon2, lat2)).km
    return distance

r = remote("portal.hackazon.org", 17001)

r.recvuntil("CYBERTANK AutoPilot System\n\n\t\n")

while True:
    line = r.recvline()
    if "CTF" in line:
        log.success(line)
        break
    else: 
        location = line.replace("[X] We are in", "")

    log.info("Location: {}".format(location))

    current_position = r.recvline()
    destination = r.recvline()

    long1, lat1 = get_coordinates(current_position)
    log.info("Current position: ({}, {})".format(long1, lat1))

    long2, lat2 = get_coordinates(destination)
    log.info("Destination: ({}, {})".format(long2, lat2))

    distance = calculate_distance(float(long1), float(lat1), float(long2), float(lat2))
    answer = "{0:.16f}".format(distance)
    log.info("Distance: {}".format(answer))

    r.sendlineafter("[?] Distance (km):", answer)
    status = r.recvline()
    if "FAILED" in status:
        break
```

Output:
```console
root@kali:/media/sf_CTFs/hackazon/CyberTank_AutoPilot# python 2.py
[+] Opening connection to portal.hackazon.org on port 17001: Done
[*] Location:  Republic of Tajikistan
[*] Current position: (73.357784, 39.801215)
[*] Destination: (72.962823, 40.222384)
[*] Distance: 46.1346027233250169
[*] Location:
[*] Current position: (12.880536, 35.500332)
[*] Destination: (12.678708, 35.503398)
[*] Distance: 22.3304364088240561
[*] Location:  Froyar Is. (Faeroe Is.)
[*] Current position: (-6.866839, 62.018628)
[*] Destination: (-7.023020, 62.323813)
[*] Distance: 37.8910294796722127
[*] Location:
[*] Current position: (17.187432, 45.126085)
[*] Destination: (18.666251, 44.064077)
[*] Distance: 198.6175621435025676
[*] Location:  Republic of Kenya
[*] Current position: (34.494219, 1.418301)
[*] Destination: (40.215718, 1.065226)
[*] Distance: 635.7662949929762135
[+] [+] Autopilot tests passed. Have a flag! CTF{57aaa5145945d96f59b3a0d4c335f440}
[*] Closed connection to portal.hackazon.org port 17001
```
