# Statistics
Category: AI Research, 20 points


## Description

> Imagine you are a data scientist working alongside Marie Curie; a Laurent and a physicist. Inspired by her work on radiation, she comes up with an idea for creating a new stochastic neural network layer entitled "RadiumDropout". The suggested layer behaves as follows.
> 
> For each K'th neuron, an eight-sided die is tossed K times. The dropout is activated with probability of 1 if and only if the sum of the K tosses is larger than 3^K, and a probability of 0 otherwise.
> 
> What is the probability that the first neuron is dropped?


## Solution:

The "statistics" questions were warmup questions, significantly easier than the rest of the challenges.

For the first neuron, an eight-sided die is tossed once. The dropout is activated with probability of 1 if and only if the sum of the toss is larger than 3.

So, if the result is `{1, 2, 3}` the dropout won't be activated, and if it's `{4, 5, 6, 7, 8}` it will. That gives us a `5/8` chance that the dropout will be activated.