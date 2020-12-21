# Static Scripting
Category: Steganography

## Description
> Who doesn't like to watch some static noise?
> 
> Hint: The video duration is not random

A video was attached.

## Solution

Attached was a video of 24:00 minutes where each frame looked like a collection of random colorful pixels such as:

![](images/static1.png)

The hint said that the video duration isn't random. Let's try to collect more information about the video:

```python
import numpy
import hashlib
import cv2

frames = set()

vidcap = cv2.VideoCapture('static.avi')
success, image = vidcap.read()

while success:
    frame = image.view(numpy.uint8)
    frame_hash = hashlib.md5(frame).hexdigest()
    frames.add(frame_hash)
    success, image = vidcap.read()

print("Number of frames:        {}".format(int(vidcap.get(cv2.CAP_PROP_FRAME_COUNT))))
print("Number of unique frames: {}".format(len(frames)))
print("Video width:             {}".format(int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))))
print("Video height:            {}".format(int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))))
```

Output:
```console
root@kali:/media/sf_CTFs/technion/Static_Scripting/static# python3 info.py
Number of frames:        14400
Number of unique frames: 14400
Video width:             160
Video height:            90
```

So all 14400 frames are unique. Also, in what seems to be a too-odd-to-be-true coincidence (especially given the hint), video dimensions are 160x90, which adds up to 14400 as well.

So, perhaps each frame gives us some kind of information needed in order to create an additional image of 160x90? Should we somehow transform each frame to a single pixel?

After some trial an error, the method that worked was extracting one pixel from each frame and placing it in a new image. The pixel location was based on the frame number: The first pixel was extracted from (0, 0), the next one from (1, 0) and so on.

Script:

```python
from PIL import Image
import numpy
import hashlib
import cv2


def bgr2rgb(bgr):
    return tuple(bgr)[::-1]

def main():
    vidcap = cv2.VideoCapture('static.avi')

    frame_count = int(vidcap.get(cv2.CAP_PROP_FRAME_COUNT))
    width       = int(vidcap.get(cv2.CAP_PROP_FRAME_WIDTH))
    height      = int(vidcap.get(cv2.CAP_PROP_FRAME_HEIGHT))

    assert(width * height == frame_count)

    img = Image.new('RGB', (width, height))
    img_pixels = img.load()

    for h in range(height):
        for w in range(width):
            success, frame = vidcap.read()
            assert(success)

            # Translate from cv2 format (BRG) to PIL format (RGB)
            img_pixels[w, h] = bgr2rgb(frame[h, w])

    img.save("out.png")

    success, _ = vidcap.read()
    assert(success == False)


if __name__ == "__main__":
    main()
```

The output image contained the flag: `cstechnion{s7atic_fL4G}`