# Danger, Will Robinson! (3/5)
Category: Innovation

## Description

> Ugh, while drinking your victory coffee after you found an alien message, some coffee spilled on one piece of the spaceship's equipment! We cannot return back home with this mess!
> 
> "This spaceship was very expensive to build, so we **focus** only on our equipment, even before the personals." - a voice from the radio is heard.
> 
> You can hear 50*50 annoying beeps from the equipement - PPPPPP, PPPPPP, along with some white noises from space...
> 
> Notes:
> 
> * Write your code in Python 3.8, standard libraries are available, but outbound connections are blocked.
> * A call to input() will return a string which value is the base64 representation of the attached picture (uhoh.txt's data is the return value of the input() function in your code)
> * Use print() to emit the expected output, the STDOUT is checked eventually. encode it in base64.
> * If you cannot solve it in this platform - use "Unlock hint for 200 points" for instructions about your original code (This will deduct 200 points from your total score).
> * Logicless solutions will not be scored for this specific challenge (Full 350 deduction)
> 
> example:
> 
> ```def main():
>  input_data_in_base64 = input()
>  // your logic here
>  // ...
>  base64_solution = ...
>  print(base64_solution)
> 
> main()
> ```

A `*.ppm` and a text file were attached.

## Solution

We receive an image in PPM format:

![](images/uhoh.png)

It looks like the coffee spilled on the spaceship controls. It's pretty clear what we need to do here - we need to write some Python code that erases the coffee.

A PPM file is a format used by the [Netpbm project](https://en.wikipedia.org/wiki/Netpbm), one of several formats designed to be easily exchanged between platforms. The format is pretty simple:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Danger_Will_Robinson]
└─$ xxd -g 1 uhoh.ppm | head -n 3
00000000: 50 36 0a 36 30 20 38 30 0a 32 35 35 0a ff ff ff  P6.60 80.255....
00000010: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff  ................
00000020: ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff  ................
```

It starts with a magic number representing the file type (in this case: `P6`, representing Binary Portable PixMap). Then come the width and height (`60` and `80`). After that comes the max value a pixel can have (`255` here), and a list of RGB values (as triplets).

In order to erase the coffee from the boxes, we need to identify where the boxes start and end. This is obviously very simple for a human eye, but how do we programmatically achieve this with a simple CTF-level script using just standard libraries?

Well, given the image we are provided with, we can take note of the following:

1. There are no boxes which touch the border.
2. The coffee never covers both the top and the bottom corner of the same "side" (right or left).

It possible to solve this even without these assumptions, but having them makes life a bit easier. Our strategy is to search for the side that has two full corners, calculate the height of the box, then repeatedly color the columns black in the correct direction until reaching past the end of the box (which is defined as a column where we have at least one white pixel). The first assumption about no boxes touching the corners simply allows us to skip some corner cases (pardon the pun). Don't try this at home, always handle corner cases in production!

Here's the code:

```python

# Mock input() to match the challenge description
def input():
    with open("uhoh.txt") as f:
        return f.read()

## ----------------------------------------------

import base64

# Basic Assumption: No box touches edges

class PPMImage():

    WHITE = '_'
    BLACK = '#'
    BROWN = '@'

    COLOR_MAPPING = {
        b'\xff\xff\xff': WHITE,
        b'\x00\x00\x00': BLACK,
        b'\x99\x66\x33': BROWN
    }

    def __init__(self, ppm_input):
        encoding, width, height, maxval, values = ppm_input.split(maxsplit = 4)
        assert(encoding == b'P6')
        self.width = int(width.decode("ascii"))
        self.height = int(height.decode("ascii"))

        self.arr = [[None for w in range(self.width)] for h in range(self.height)] 

        # Convert the PPM format to an internal 2D array representation

        for h in range(self.height):
            for w in range(self.width):
                offset = (self.width * h + w) * 3
                self.arr[h][w] = self.COLOR_MAPPING[values[offset:offset+3]]

    def __str__(self):
        res = ""
        for h in range(self.height):
            for w in range(self.width):
                res += self.arr[h][w]
            res += "\n"
        return res

    def to_ppm(self, top_h, left_w, bottom_h, right_w):
        """Convert the internal representation of the given bounding box back to PPM format."""

        reverse_color_mapping = {v:k for (k, v) in self.COLOR_MAPPING.items()}
        res = bytearray()
        height = bottom_h - top_h + 1
        width = right_w - left_w + 1
        res += "P6\n{} {}\n255\n".format(width, height).encode("ascii")
        for h in range(height):
            for w in range(width):
                res += reverse_color_mapping[self.arr[top_h+h][left_w+w]]
        return res

TOP_LEFT_CORNER  = 1
TOP_RIGHT_CORNER = -1
NOT_A_CORNER     = 0

def check_top_corner(img, h, w):
    """Check if the given coordinates belong to a corner."""
    if not img.arr[h][w] == img.BLACK:
        return False
    
    try:
        # This is a top corner if it's color is black, the pixel over it is white
        #  and the pixel to the left/right is white

        if img.arr[h - 1][w] == img.WHITE and img.arr[h][w - 1] == img.WHITE:
            return TOP_LEFT_CORNER

        if img.arr[h - 1][w] == img.WHITE and img.arr[h][w + 1] == img.WHITE:
            return TOP_RIGHT_CORNER
        
        return NOT_A_CORNER

    except IndexError:
        return NOT_A_CORNER

def get_box_height(img, top_corner_h, top_corner_w):
    """Get the box height for the box starting at the given top coordinates."""

    # Count the black pixels by moving down until reaching a white pixel.
    # Assumption: Every box has at least one corner where this is possible

    box_height = 0
    h = top_corner_h
    while img.arr[h][top_corner_w] == img.BLACK:
        box_height += 1
        h += 1
    
    if img.arr[h][top_corner_w] == img.WHITE:
        return box_height
    
    raise Exception("Can't find box height!")

def column_has_white(img, initial_h, initial_w, box_height):
    """Check if the column starting at the given coordinates and of the given hight has a white pixel."""
    for h_offset in range(box_height):
        if img.arr[initial_h + h_offset][initial_w] == img.WHITE:
            return True
    return False

def fill_column(img, initial_h, initial_w, box_height):
    """Fill the given column black"""
    for h_offset in range(box_height):
        img.arr[initial_h + h_offset][initial_w] = img.BLACK

def fill_box(img, top_corner_h, top_corner_w, box_height, direction):
    """Fill the box starting at the given top corner based on the given direction. Return the width."""
    w = top_corner_w
    filled_columns = 0

    # We continue filling the box until we find a white pixel in the column, indicating this isn't
    #  part of the box anymore

    while not column_has_white(img, top_corner_h, w, box_height):
        fill_column(img, top_corner_h, w, box_height)
        w += direction
        filled_columns += 1
    return filled_columns

def erase_coffee(img):

    # These coordinates are used to identify the bounding box containing all boxes (without the cup)
    top_h    = None
    bottom_h = None
    left_w   = None
    right_w  = None

    directions = {
        TOP_LEFT_CORNER:   1,
        TOP_RIGHT_CORNER: -1,
        NOT_A_CORNER:     0
    }

    for h in range(img.height):
        for w in range(img.width):
            corner_type = check_top_corner(img, h, w)
            if corner_type == NOT_A_CORNER:
                continue
            
            try:
                box_height = get_box_height(img, h, w)
            except Exception:
                continue

            filled_columns = fill_box(img, h, w, box_height, directions[corner_type])

            # Update bounding box dimensions:

            if box_height > 2 and filled_columns > 2:
                if top_h is None:
                    top_h = h
                if bottom_h is None or h + box_height - 1 > bottom_h:
                    bottom_h = h + box_height - 1
                if left_w is None or w < left_w:
                    left_w = w
                if right_w is None or w > right_w:
                    right_w = w

    # erase the rest of the coffee
    for h in range(img.height):
        for w in range(img.width):
            if img.arr[h][w] == img.BROWN:
                img.arr[h][w] = img.WHITE

    # Return the bounding box
    return (top_h, left_w, bottom_h, right_w)
           

def main():
    input_data_in_base64 = input()
    input_data = base64.b64decode(input_data_in_base64)
    
    img = PPMImage(input_data)
    bounding_box = erase_coffee(img)
    ppm = img.to_ppm(*bounding_box)

    #with open("out_test.ppm", "wb") as out:
    #    out.write(ppm)

    print(base64.b64encode(ppm).decode("ascii"), end="")

main()
```

There's one more point in the code that wasn't mentioned earlier: The code crops out the bounding box containing the spaceship controls and submits that as the answer. This is done since:

1. The original size wasn't accepted by the server
2. The description talks about **focus**ing on the equipment
3. The description mentions 50*50 beeps, and the bounding box is 50x50

