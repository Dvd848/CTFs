# NLP
Category: AI Research, 250 points


## Description

> A jaded attacker, who installed a malware on your computer, was able to access all your text files and a managed to encrypt them all. By analysing the timestamps, he used the last document you worked on and iterated over it sentence by sentence, and for each sentence used a pre-trained BERT model to create a 768D embedding vector which was persisted to permanent storage.
> 
> The vector he created was computed using the 'bert-base-multilingual-uncased' model. The vector is an average of the second-to-last hidden layer outputs, over all tokens (including special ones).
> 
> Subsequently, he erased the document from your hard-drive and kept a copy to himself. Knowing how significant the loss of the document is to you, he challenges you to solve a riddle to gain back access to your files.
> 
> He provides you with a single feature vector (a 768d embedding), of a specific sentence in the document.
> 
> He guarantees that if you are able to find the actual sentence, he will unlock your hard drive.
> 
> Each sentence in the original document had a line number associated with it, you are to provide him with that number.
> 
> A big hint to find this number is hidden in the first image file provided with this challenge. The second txt file is the BERT embedding.


## Solution:

So, according to the description, we have a `768D embedding vector` which is the result of running some kind of algorithm on a sentence from one of our documents. We need to somehow recover the original sentence from the vector. 

Luckily, we have a hint:

![](images/hint_-_nlp.png)

This is just the ISA logo. But if we open it with `stegsolve` and browse the planes, we can find a hidden message:

![](images/nlp_stegsolve.png)

It's a bit hard to identify the hidden message with the `stegsolve` output, [this website](https://incoherency.co.uk/image-steganography/#unhide) does a much better job:

![](images/nlp_inco.png)

This is the arabic word البقرة which Google translates to *Al-Baqarah*:

> Al-Baqarah is the second and longest chapter of the Quran. It consists of 286 verses, 6,201 words and 25,500 letters.

This gives us what seems to be the original text. It matches the description that states that *"Each sentence in the original document had a line number associated with it"* - that's how chapters of the Quran are structured. So, the plan is to iterate each sentence from the chapter, calculate the score using the same method used by the attacker and choose the closest result. 

Here's the Python script for the job:

```python
from pwn import *
import numpy as np
from scipy import spatial
from transformers import BertTokenizer, BertModel, BertConfig
import os

tokenizer = BertTokenizer.from_pretrained('bert-base-multilingual-uncased')
config    = BertConfig.from_pretrained("bert-base-multilingual-uncased", output_hidden_states = True)
model     = BertModel.from_pretrained("bert-base-multilingual-uncased", config = config)

VECTOR_LEN = 768

def calc_layers_average(layers):
    res = np.zeros(VECTOR_LEN)

    count = 0
    for layer in layers:
        for level1 in layer:
            for level2 in level1:
                assert(len(level2) == VECTOR_LEN)
                res += level2.detach().numpy()
                count += 1

    res /= count
    return res

def calc_score(text):
    inputs = tokenizer(text, return_tensors="pt")
    outputs = model(**inputs)

    hidden_states = outputs[2]

    embedding_output = hidden_states[0]
    attention_hidden_states = hidden_states[1:]

    return calc_layers_average(attention_hidden_states)

scores = []

with log.progress('Calculating scores...') as p:
    with open("AlBaqarah.txt", encoding = "utf8") as f:
        line_num = 0
        for line in f:
            line = line.strip()
            if line != "":
                p.status(f"Working on line #{line_num}")
                scores.append(calc_score(line))
                line_num += 1

with log.progress('Searching for best match...'):
    data = np.load("nlp_embedding.npy")
    tree = spatial.KDTree(scores)
    res = tree.query(data)

log.success(f"Best match: line #{res[1] + 1}")
```

Output:

```console
root@kali:/media/sf_CTFs/shabak/NLP# python3 solve.py
[+] Calculating scores...: Done
[+] Searching for best match...: Done
[+] Best match: line #147
```

Indeed, `147` was accepted as the correct answer.