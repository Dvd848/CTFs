# Netflix and Script
Category: Steganography

## Description
> I found this movie while scouring Netflix for something to watch, can you figure out what the hell is going on with this movie and if it's good or not?

An `*.avi` file was attached.

## Solution

From a brief inspection of the file, we get a five and a half movie of flickering black and white screens. Therefore, the obvious next step is to convert it to binary and see what we get. In this case - nothing much.

Another option is to use the frames as pixels and try to create an image with them - however this doesn't yield any result either.

Let's try to map identify the unique frames in the movie and inspect them:

```python
from collections import defaultdict
from pprint import pprint as pp
import numpy
import hashlib
import cv2

frames = set()

vidcap = cv2.VideoCapture('netflix_and_script.avi')
success, image = vidcap.read()

while success:
    frame = image.view(numpy.uint8)
    frame_hash = hashlib.md5(frame).hexdigest()
    if frame_hash not in frames:
        cv2.imwrite("frame_u_{}.jpg".format(len(frames)), image)
    frames.add(frame_hash)
    success, image = vidcap.read()

pp(frames)
```

Running the script, we get three(!) unique frames:

```console
root@kali:/media/sf_CTFs/technion/Netflix_and_Script/netflix_and_script# python3 solve.py
{'348555c176cf73a8c601723d3c9e0a3c',
 '4e1210b1572c1c043895026cef620105',
 'b94a50342ad1ea4b9819d3fd3894db2f'}
```

One of the frames contained the flag: `cstechnion{St1ll_b3ttEr_7h4n_tw1ligh7}`.