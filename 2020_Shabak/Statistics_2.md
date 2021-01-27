#  Statistics 2 
Category: AI Research, 20 points


## Description

> Still refering to challenge Statistics1:
> 
> What is the maximum number of neurons that will be dropped?


## Solution:

This is the follow-up question for [Statistics](Statistics.md).

Reminder:

> For each K'th neuron, an eight-sided die is tossed K times. The dropout is activated with probability of 1 if and only if the sum of the K tosses is larger than 3^K, and a probability of 0 otherwise.

We need to understand when it's impossible that the sum of the K tosses will be larger than `3^k`.

```python
>>> for k in itertools.count(1):
...     if ( (k * 8) < (3 ** k)):
...             print(k)
...             break
...
3
```

So when `k >= 3`, there's no chance that the neuron will be dropped. That gives us `k = {1, 2}`. Therefore, the answer should be `2`. For some reason, at first the accepted answer was `0`, and this was later fixed.

