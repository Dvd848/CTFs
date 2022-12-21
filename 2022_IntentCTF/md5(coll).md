# md5(coll)
* Category: Misc.
* 200 Points
* Solved by the JCTF Team

## Description

> A picture is worth a thousand words, and a flag too.

```python
from socket import timeout
from tkinter.tix import TEXT
from flask import Flask, jsonify, make_response, request, render_template, url_for
import os
import sys
import io
import hashlib
import requests
import tempfile
import subprocess
from PIL import Image, ImageChops, ImageOps

app = Flask(__name__)

FLAG = r"INTENT{flag_goes_here}"
TEXT_SEARCH = "INTENT, give me the flag"

def save_image(im):
    if im.format == "GIF":
        final_image = im.convert("1")
        for frame_num in range(im.n_frames):
            im.seek(frame_num)
            final_image = ImageChops.logical_and(final_image, im.convert("1"))
        im = final_image
    else:
        im = im.convert("1")
    temp_file_path = tempfile.NamedTemporaryFile(suffix='.jpg').name
    im = im.resize((im.size[0]*4, im.size[1]*4), Image.ANTIALIAS)
    im.save(temp_file_path)
    return temp_file_path

@app.route('/')
def index():
    img_url = request.args.get("imgurl", "")
    if img_url == "":
        return render_template('index.html')
    else:
        out_text = ""
        try:
            resp = requests.get(img_url, timeout=10, stream=True, verify=False)
            img_content = resp.raw.read(int(1024 * 1024 * 0.2))
            img_pil = Image.open(io.BytesIO(img_content))
            md5_str = hashlib.md5(img_content).hexdigest()
            out_text += f"[-] Got {len(img_content)} bytes for image type '{img_pil.format}' with MD5 hash: {md5_str}<br>"
            saved_jpg_path = save_image(img_pil)
            process = subprocess.Popen(["tesseract", saved_jpg_path, "stdout"], stdout=subprocess.PIPE,
                                       stderr=subprocess.DEVNULL)
            image_text = process.stdout.read().decode()
            img_processing = f"OCR SAYS: {image_text}"
            if not(TEXT_SEARCH.encode().lower() in img_content.lower() and \
                    TEXT_SEARCH in image_text):                    
                out_text += f"You should include the following string in your GIF -> {TEXT_SEARCH}<br>"
            elif md5_str in image_text:
                out_text += f"OK take it!: {FLAG}<br>"
            else:
                out_text += ":((((<br>"
            os.remove(saved_jpg_path)
            return render_template("index.html", run_results=out_text, img_processing=img_processing)
        except Exception as e:
            return render_template("index.html", img_processing=f":(((((((((((((((((((")



def main():
    with app.app_context():
        app.run(host="0.0.0.0", debug=True)


if '__main__' == __name__:
    main()

```

## Solution

This was a cool challenge related to MD5 collisions.

The attached script specifies exactly what we need to do in order to get the flag - provide a GIF that:

 * Displays the string "INTENT, give me the flag" in the image itself
 * Contains the string "INTENT, give me the flag" in its binary (lowercase allowed)
 * Displays its own MD5 hash

Wait, what? DISPLAYS ITS OWN MD5 HASH? 

Well, thankfully someone has already provided a PoC for a GIF displaying its own hash. The very interesting writeup can be found [here](https://www.rogdham.net/2017/03/12/gif-md5-hashquine.en) with a link to the code. In short, we need to:

 1. Modify `background.gif` to include our requested text (and remove the rest)
 2. Modify `generate.py` to include our text in the `comment`
 3. Use larger and more OCR-friendly fonts (since it turns out that `tesseract` is practically half-blind)
    a. [This post](https://superuser.com/questions/1096425/optimal-font-for-tesseract-specifically-the-net-wrapper/1543382#1543382) contains recommendations. We used `Roboto` bold except for some characters which the OCR would always misinterpret, making us fallback to `DejaVu_Serif`.

We can then run the script by simply calling `make` to generate a matching GIF. The GIF generation process is very slow (around 30 minutes), which makes it extremely important to
get the logic right, since every miss is very painful. 
The original PoC generates just one GIF. To overcome the limited OCR capabilities, we can modify it to generate a large amount of GIFs after calculating the needed collisions, as the time-consuming part is the collision generation. For example, by modifying the bruteforcing part to something like:

```python
        print('Bruteforcing final md5...')
        generated_gif_copy = generated_gif[:]
        res = []
        generated_gifs = []

        for garbage in range(1 << 32):  # 32 bits of bf should be enough
            generated_gif = generated_gif_copy[:]
            end = struct.pack('<BIBB',
                              4, garbage,  # comment sub-block
                              0,  # end comment
                              0x3b)  # trailer
            new_md5 = current_md5.copy()
            new_md5.update(end)
            for mask_char, md5_char in zip(self.md5_mask, new_md5.hexdigest()):
                if mask_char != ' ' and mask_char != md5_char:
                    #break
                    generated_gifs.append(generated_gif)
            else:
                generated_gif += end
                #break
                generated_gifs.append(generated_gif)
            if len(generated_gifs) > 5000:
                break
                
        else:
            raise ValueError('Did not find a GIF matching the md5 mask')
            
        for generated_gif in generated_gifs:
            # replace colls to show md5
            print('Target md5:', md5(generated_gif).hexdigest())
            for char_pos, char in enumerate(md5(generated_gif).hexdigest()):
                if self.md5_mask[char_pos] != ' ':
                    continue
                coll_pos, coll = alternatives[char_pos, int(char, 16)]
                generated_gif = (
                    generated_gif[:coll_pos] + coll +
                    generated_gif[coll_pos + len(coll):]
                )
            print('Final md5: ', md5(generated_gif).hexdigest())
            res.append(generated_gif)
```

An example GIF that matches the rules:

![](images/hashquine.gif)

Submitting it gives us the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/md5(coll)]
└─$ curl 'https://intent-md5coll.chals.io/?imgurl=https%3A%2F%2Fraw.githubusercontent.com%2FDvd848%2FCTFs%2Fmaster%2F2022_IntentCTF%2Fimages%2Fhashquine.gif' -s | grep INTENT -B 1 -A 2
                        Run result:
                        [-] Got 185149 bytes for image type 'GIF' with MD5 hash: 1b03dd29be235e4df115476bc3c76f01<br>OK take it!: <b>INTENT{md5(A)=md5(B)->md5(A+C)=md5(B+C)}</b><br>
                        <br>
                        OCR SAYS: INTENT, give me the flag
1b03dd29be235e4df115476bc3c76f01
```