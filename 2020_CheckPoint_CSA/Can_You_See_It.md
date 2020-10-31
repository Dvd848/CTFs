# Can You See It?
Category: Misc., 40 Points

## Description

> All we got is this video.
> 
> Can you please help?
> 
> Good Luck!

A video was attached.

## Solution

Playing the video, we get a quick flickering of background colors. We can extract a portion to a GIF file as a demonstration:

```console
root@kali:/media/sf_CTFs/checkpoint/Can_You_See_It/Can_You_See_It# ffmpeg -i Can_You_See_It.mp4 -ss 00:00:00 -to 00:00:03 can_you_see_it.gif
ffmpeg version 4.1-1 Copyright (c) 2000-2018 the FFmpeg developers
  built with gcc 8 (Debian 8.2.0-12)
  configuration: --prefix=/usr --extra-version=1 --toolchain=hardened --libdir=/usr/lib/x86_64-linux-gnu --incdir=/usr/include/x86_64-linux-gnu --arch=amd64 --enable-gpl --disable-stripping --enable-avresample --disable-filter=resample --enable-avisynth --enable-gnutls --enable-ladspa --enable-libaom --enable-libass --enable-libbluray --enable-libbs2b --enable-libcaca --enable-libcdio --enable-libcodec2 --enable-libflite --enable-libfontconfig --enable-libfreetype --enable-libfribidi --enable-libgme --enable-libgsm --enable-libjack --enable-libmp3lame --enable-libmysofa --enable-libopenjpeg --enable-libopenmpt --enable-libopus --enable-libpulse --enable-librsvg --enable-librubberband --enable-libshine --enable-libsnappy --enable-libsoxr --enable-libspeex --enable-libssh --enable-libtheora --enable-libtwolame --enable-libvidstab --enable-libvorbis --enable-libvpx --enable-libwavpack --enable-libwebp --enable-libx265 --enable-libxml2 --enable-libxvid --enable-libzmq --enable-libzvbi --enable-lv2 --enable-omx --enable-openal --enable-opengl --enable-sdl2 --enable-libdc1394 --enable-libdrm --enable-libiec61883 --enable-chromaprint --enable-frei0r --enable-libx264 --enable-shared
  libavutil      56. 22.100 / 56. 22.100
  libavcodec     58. 35.100 / 58. 35.100
  libavformat    58. 20.100 / 58. 20.100
  libavdevice    58.  5.100 / 58.  5.100
  libavfilter     7. 40.101 /  7. 40.101
  libavresample   4.  0.  0 /  4.  0.  0
  libswscale      5.  3.100 /  5.  3.100
  libswresample   3.  3.100 /  3.  3.100
  libpostproc    55.  3.100 / 55.  3.100
Input #0, mov,mp4,m4a,3gp,3g2,mj2, from 'Can_You_See_It.mp4':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2mp41
    encoder         : Lavf58.35.100
  Duration: 12:39:44.60, start: 0.000000, bitrate: 2 kb/s
    Stream #0:0(und): Video: mpeg4 (Simple Profile) (mp4v / 0x7634706D), yuv420p, 24x24 [SAR 1:1 DAR 1:1], 2 kb/s, 10 fps, 10 tbr, 10240 tbn, 10 tbc (default)
    Metadata:
      handler_name    : VideoHandler
Stream mapping:
  Stream #0:0 -> #0:0 (mpeg4 (native) -> gif (native))
Press [q] to stop, [?] for help
[swscaler @ 0x55dfbd1454c0] No accelerated colorspace conversion found from yuv420p to bgr8.
Output #0, gif, to 'can_you_see_it.gif':
  Metadata:
    major_brand     : isom
    minor_version   : 512
    compatible_brands: isomiso2mp41
    encoder         : Lavf58.20.100
    Stream #0:0(und): Video: gif, bgr8, 24x24 [SAR 1:1 DAR 1:1], q=2-31, 200 kb/s, 10 fps, 100 tbn, 10 tbc (default)
    Metadata:
      handler_name    : VideoHandler
      encoder         : Lavc58.35.100 gif
frame=   30 fps=0.0 q=-0.0 Lsize=       5kB time=00:00:02.91 bitrate=  12.9kbits/s speed= 160x
video:4kB audio:0kB subtitle:0kB other streams:0kB global headers:0kB muxing overhead: 28.442623%
```

The result is: 

![](images/can_you_see_it.gif)

Let's check the metadata:
```console
root@kali:/media/sf_CTFs/checkpoint/Can_You_See_It# exiftool Can_You_See_It/Can_You_See_It.mp4
ExifTool Version Number         : 11.10
File Name                       : Can_You_See_It.mp4
Directory                       : Can_You_See_It
File Size                       : 11 MB
File Modification Date/Time     : 2020:04:23 18:16:26+03:00
File Access Date/Time           : 2020:05:08 00:20:54+03:00
File Inode Change Date/Time     : 2020:05:08 00:21:36+03:00
File Permissions                : rwxrwx---
File Type                       : MP4
File Type Extension             : mp4
MIME Type                       : video/mp4
Major Brand                     : MP4  Base Media v1 [IS0 14496-12:2003]
Minor Version                   : 0.2.0
Compatible Brands               : isom, iso2, mp41
Movie Data Size                 : 11851996
Movie Data Offset               : 44
Movie Header Version            : 0
Create Date                     : 0000:00:00 00:00:00
Modify Date                     : 0000:00:00 00:00:00
Time Scale                      : 1000
Duration                        : 12:39:44
Preferred Rate                  : 1
Preferred Volume                : 100.00%
Preview Time                    : 0 s
Preview Duration                : 0 s
Poster Time                     : 0 s
Selection Time                  : 0 s
Selection Duration              : 0 s
Current Time                    : 0 s
Next Track ID                   : 2
Track Header Version            : 0
Track Create Date               : 0000:00:00 00:00:00
Track Modify Date               : 0000:00:00 00:00:00
Track ID                        : 1
Track Duration                  : 12:39:44
Track Layer                     : 0
Track Volume                    : 0.00%
Matrix Structure                : 1 0 0 0 1 0 0 0 1
Image Width                     : 24
Image Height                    : 24
Media Header Version            : 0
Media Create Date               : 0000:00:00 00:00:00
Media Modify Date               : 0000:00:00 00:00:00
Media Time Scale                : 10240
Media Duration                  : 12:39:44
Media Language Code             : und
Handler Description             : VideoHandler
Graphics Mode                   : srcCopy
Op Color                        : 0 0 0
Compressor ID                   : mp4v
Source Image Width              : 24
Source Image Height             : 24
X Resolution                    : 72
Y Resolution                    : 72
Bit Depth                       : 24
Video Frame Rate                : 10
Handler Type                    : Metadata
Handler Vendor ID               : Apple
Encoder                         : Lavf58.35.100
Avg Bitrate                     : 2.08 kbps
Image Size                      : 24x24
Megapixels                      : 0.000576
Rotation                        : 0
```

So we have over 12 hours of this. Let's analyze the frames:

```python
from collections import defaultdict
from pprint import pprint as pp
import numpy
import hashlib
import cv2

frames = set()

vidcap = cv2.VideoCapture('Can_You_See_It/Can_You_See_It.mp4')
success, image = vidcap.read()

while success:
    frame = image.view(numpy.uint8)
    frame_hash = hashlib.md5(frame).hexdigest()
    if frame_hash not in frames:
        cv2.imwrite("frame{}.jpg".format(len(frames)), image)
    frames.add(frame_hash)
    success, image = vidcap.read()

pp(frames)
```

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Can_You_See_It# python3 analyze.py
{'4184601e0763495d086a2677b41cda4b',
 '442f447dedf30e3a44cd5c05b803ac2e',
 '7bb1a90c3b6fb7d417f98816a16f5cdf'}
```

We see that we have three distinct frames altogether. They are plain white, plain gray and plain black. This means that we probably aren't dealing with encoded bits, since then we would expect only two distinct frames.

This is where the name of the challenge provides a hint. "Can you see it?". See what, you ask? See the big picture, of course.

If we treat each frame as a pixel, we can try to create an image. But first, we need to decide what the image dimensions will be, and for that, we need to know the amount of frames.

```python
>>> import cv2
>>> vidcap = cv2.VideoCapture('Can_You_See_It/Can_You_See_It.mp4')
>>> vidcap.get(cv2.CAP_PROP_FRAME_COUNT)
455846.0
```

Since the width and the height need to be integers, there's only a limited number of values we can use. We just need to find all multipliers of 455846.

The following script will try all legal image dimensions (assuming both dimensions must be at least 10 pixels), writing a unique pixel value for each unique frame.

```python
import numpy
import hashlib
import cv2
import pickle, os, math
from PIL import Image

CACHE_FILE = "cache.db"
COLORS = {0: (0, 0, 0), 1: (0xff, 0xff, 0xff), 2: (0xff, 0, 0)}
MIN_DIMENSION_VAL = 10

# https://stackoverflow.com/questions/44061928/
def multipliers(m):
    yield (m, 1)

    finalVal = int(math.sqrt(m))
    increment = 2 if m % 2 != 0 else 1
    i = 3 if  m % 2 != 0 else 2

    while (i <= finalVal):
        if (m % i == 0):
            yield (m // i, i)

        i += increment

if not os.path.exists(CACHE_FILE):
    frames_map = {} # Map a unique frame hash to a running index
    frames = [] # A cacheable list of all frame values as indexes

    vidcap = cv2.VideoCapture('Can_You_See_It/Can_You_See_It.mp4')
    success, image = vidcap.read()

    while success:
        arr = image.view(numpy.uint8)
        arr_hash = hashlib.md5(arr).hexdigest()
        if arr_hash not in frames_map:
            frames_map[arr_hash] = len(frames_map)
        frames.append(frames_map[arr_hash])
        success,image = vidcap.read()
    
    assert(len(COLORS) >= len(frames_map))

    with open(CACHE_FILE, "wb") as cache:
        pickle.dump(frames, cache)    
else:
    with open(CACHE_FILE, "rb") as cache:
        frames = pickle.load(cache)

def get_next_color():
    for x in frames:
        yield COLORS[x]

for resolution in multipliers(len(frames)):
    if all(x > MIN_DIMENSION_VAL for x in resolution):
        colors = get_next_color()
        img = Image.new( 'RGB', (resolution[0], resolution[1]), "black")
        pixels = img.load() 
        for j in range(img.size[1]):
            for i in range(img.size[0]):
                pixels[i, j] = next(colors)
        img.save("output_{}_{}.png".format(resolution[0], resolution[1]), "PNG")
```

Running it, we get two images:

```console
root@kali:/media/sf_CTFs/checkpoint/Can_You_See_It# ls *.png
output_1438_317.png  output_719_634.png
```

`output_1438_317.png` contains the flag:
![](images/output_1438_317.png)

The flag: `CSA{Ev3ry_mov1e_must_hav3_a_Purp0s3}`