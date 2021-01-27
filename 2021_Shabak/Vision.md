# Vision
Category: AI Research, 250 points


## Description

> During your research on image similarity methods, you discover a corpus made of images, only to find out that the original images were deleted by their creator and only their 512D feature vector was saved. You are now provided with a single such binary vector (a pickled numpy array), which was created from a ResNet18 model (PyTorch), pre-trained on the ImageNet corpus.
> 
> Your task is to identify the object that appeared in the original image.
> 
> The attached image file contains a hint regarding the identity of said object.


## Solution:

Our strategy was to iterate ImageNet categories, pick 10 random images for each category, and test the similarity to the vector we've received.

```python
from pwn import *
from PIL import Image
from multiprocessing.dummy import Pool as ThreadPool

import os
import json
import torch
import torchvision
import torchvision.models as models
import numpy as np
import requests
import logging
import multiprocessing
import matplotlib.pyplot as plt

THRESHOLD = 0.7
OUTPUT_FOLDER = "output"
NUM_THREADS = 20
MAX_IMAGES_PER_LABEL = 10

# Load the pretrained model
model = models.resnet18(pretrained = True)

# Use the model object to select the desired layer
layer = model._modules.get('avgpool')

# Set model to evaluation mode
model.eval()

transforms = torchvision.transforms.Compose([
    torchvision.transforms.Resize(256),
    torchvision.transforms.CenterCrop(224),
    torchvision.transforms.ToTensor(),
    torchvision.transforms.Normalize(mean = [0.5, 0.5, 0.5], std = [0.5, 0.5, 0.5]),
])

def get_vector(image):
    # Create a PyTorch tensor with the transformed image
    t_img = transforms(image)
    # Create a vector of zeros that will hold our feature vector
    # The 'avgpool' layer has an output size of 512
    my_embedding = torch.zeros(512)

    # Define a function that will copy the output of a layer
    def copy_data(m, i, o):
        my_embedding.copy_(o.flatten())                 # <-- flatten

    # Attach that function to our selected layer
    h = layer.register_forward_hook(copy_data)
    # Run the model on our transformed image
    with torch.no_grad():                               # <-- no_grad context
        model(t_img.unsqueeze(0))                       # <-- unsqueeze
    # Detach our copy function from the layer
    h.remove()
    # Return the feature vector
    return my_embedding


def download_images(input_queue, output_queue):
    while not input_queue.empty():
        key, id, label = input_queue.get()
        
        try:
            r = requests.get("http://www.image-net.org/api/text/imagenet.synset.geturls?wnid={}".format(id))
        except Exception:
            continue

        added_images = 0
        for i, image_url in enumerate(r.text.split("\n")):
            if image_url == "":
                continue 

            if added_images > MAX_IMAGES_PER_LABEL:
                break

            try:
                img = Image.open(requests.get(image_url, stream = True).raw)
                output_queue.put((key, id, label, i, img))
                added_images += 1
            except Exception:
                pass


def main():

    if not os.path.exists(OUTPUT_FOLDER):
        os.mkdir(OUTPUT_FOLDER)

    img_list_queue  = multiprocessing.Queue()
    result_queue    = multiprocessing.Queue()

    v = np.load("image_embedding.npy")
    cos = torch.nn.CosineSimilarity(dim = 0)

    # https://s3.amazonaws.com/deep-learning-models/image-models/imagenet_class_index.json
    with open("imagenet_class_index.json", "r") as imagenet_class_index:
        labels = json.load(imagenet_class_index)
        for key, (id, label) in labels.items():
            img_list_queue.put((key, id, label))

    img_list_thread_pool = ThreadPool(NUM_THREADS, download_images, (img_list_queue, result_queue))

    with log.progress('Searching for similar objects...') as p:
        while not img_list_queue.empty():
            try:
                key, id, label, i, img = result_queue.get(timeout = 60)
                p.status(f"{key}: Label '{label}', image #{i}")

                pic_vector = get_vector(img).numpy()

                cosine_similarity = cos(torch.from_numpy(v), torch.from_numpy(pic_vector))   

                if cosine_similarity > THRESHOLD:
                    log.info(f"Found similar object: {label}, similarity: {cosine_similarity}")
                    np.save(f"{OUTPUT_FOLDER}/{label}_{i}_vector.npy", pic_vector)
                    plt.scatter(range(v.shape[0]), v)
                    plt.scatter(range(pic_vector.shape[0]), pic_vector)
                    plt.savefig(f"{OUTPUT_FOLDER}/{label}_{i}_image.png")
                    plt.clf()
            except multiprocessing.queues.Empty:
                break
            except RuntimeError as e:
                pass
        

    img_list_thread_pool.close()

    img_list_thread_pool.join()

if __name__ == "__main__":
    main()   
```

Eventually we get the following results:

```console
root@kali:/media/sf_CTFs/shabak/Vision# python3 solve.py
[↖] Searching for similar objects...: 420: Label 'banjo', image #26
[*] Found similar object: assault_rifle, similarity: 0.7856280207633972
[*] Found similar object: assault_rifle, similarity: 0.7135688662528992
[*] Found similar object: assault_rifle, similarity: 0.7446579337120056
```

The accepted solution was "assault rifle".