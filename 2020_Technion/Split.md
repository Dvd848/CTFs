# Split
Category: Miscellaneous

## Description
> Oh noes! I've accidentally dropped the file on the desktop and it shattered into pieces... (150x150) Now how am I going to face the manager to publish the movie's poster? :( Please help, I got no backups!

A zip file was attached.

## Solution

In the zip file, we find 22,500 images named `image_0.jpg` to `image_22499.jpg`. Each image was 9x13. We need to combine the images into one big image in order to see the full picture.

The following script will combine the image by building row by row, then combining them:

```python
import numpy as np
import PIL
from PIL import Image
from pwn import *

WIDTH = HEIGHT = 150

rows = []

with log.progress('Recreating image...') as p:
    for row in range(HEIGHT):
        p.status(f"Creating row #{row}")

        start = row * WIDTH
        end   = start + WIDTH
        images    = [ PIL.Image.open(f'images/image_{i}.jpg') for i in range(start, end) ]
        combined = np.hstack( list(np.asarray( img ) for img in images ) )

        combined = PIL.Image.fromarray( combined)
        rows.append(combined)

    p.status(f"Combining rows")

    combined = np.vstack( list(np.asarray( row ) for row in rows ) )
    combined = PIL.Image.fromarray(combined)
    combined.save( 'output.jpg' )

```

The flag is in the image: `cstechnion{Broken_n3v3r_again}`