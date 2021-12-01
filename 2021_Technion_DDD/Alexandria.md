#  Alexandria
Category: Weekly Challenges, 10 points

## Description

> Don't judge a book by its cover...
> 
> The flag format is flag{...} with underscores between words, for example flag{example_flag}. Change the flag in accordance to this format.

```
1612130291
0140283331
0553588486
0307588378
031606792X
0762447699 
0439023483
0439708184
0143038419
0316042676
0142000671
0440242940
1400079276
```

## Solution

Searching these numbers via Google, we find that they are ISBNs for popular boots:

```
1612130291 - Fifty Shades of Grey
0140283331 - Lord of the Flies
0553588486 - A Game of Thrones
0307588378 - Gone Girl
031606792X - Breaking Dawn
0762447699 - You Are a Badass: How to Stop Doubting Your Greatness and Start Living an Awesome Life
0439023483 - The Hunger Games
0439708184 - Harry Potter and the Philosopher's Stone
0143038419 - Eat, Pray, Love: One Woman's Search for Everything Across Italy, India and Indonesia
0316042676 - Beautiful Creatures
0142000671 - Of Mice and Men
0440242940 - Outlander
1400079276 - Kafka on the Shore
```

Taking the first letter of each book, we get the flag: `flag{by_the_book}`.