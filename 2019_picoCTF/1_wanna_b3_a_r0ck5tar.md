# 1_wanna_b3_a_r0ck5tar
General Skills, 350 points

## Description:
> I wrote you another song. Put the flag in the picoCTF{} flag format

```
Rocknroll is right              
Silence is wrong                
A guitar is a six-string        
Tommy's been down               
Music is a billboard-burning razzmatazz!
Listen to the music             
If the music is a guitar                  
Say "Keep on rocking!"                
Listen to the rhythm
If the rhythm without Music is nothing
Tommy is rockin guitar
Shout Tommy!                    
Music is amazing sensation 
Jamming is awesome presence
Scream Music!                   
Scream Jamming!                 
Tommy is playing rock           
Scream Tommy!       
They are dazzled audiences                  
Shout it!
Rock is electric heaven                     
Scream it!
Tommy is jukebox god            
Say it!                                     
Break it down
Shout "Bring on the rock!"
Else Whisper "That ain't it, Chief"                 
Break it down 
```

## Solution: 

This is the follow-up for [mus1c](mus1c.md).

Same as before, we paste the program into the [online interpreter](https://codewithrockstar.com/online) and run it. However, this time the program asks for input. Entering a random string doesn't lead anywhere. We'll have to understand the program.

We can either choose to understand the [Language Specification](https://codewithrockstar.com/docs) or just use a [Python transpiler](https://github.com/yanorestes/rockstar-py) to translate the program to Python. Using the transpiler seems faster.

```console
root@kali:/media/sf_CTFs/pico/1_wanna_b3_a_r0ck5tar# rockstar-py --output rockstar.py -i lyrics.txt
root@kali:/media/sf_CTFs/pico/1_wanna_b3_a_r0ck5tar# cat rockstar.py
Rocknroll = True
Silence = False
a_guitar = 19
Tommy = 44
Music = 160
the_music = input()
if the_music == a_guitar:
    print("Keep on rocking!")
    the_rhythm = input()
    if the_rhythm - Music == False:
        Tommy = 66
        print(Tommy!)
        Music = 79
        Jamming = 78
        print(Music!)
        print(Jamming!)
        Tommy = 74
        print(Tommy!)
        They are dazzled audiences
        print(it!)
        Rock = 86
        print(it!)
        Tommy = 73
        print(it!)
        break
        print("Bring on the rock!")
        Else print("That ain't it, Chief")
        break
```

This code, of course, doesn't run:
```console
root@kali:/media/sf_CTFs/pico/1_wanna_b3_a_r0ck5tar# python rockstar.py
  File "rockstar.py", line 12
    print(Tommy!)
               ^
SyntaxError: invalid syntax
```

Fixing it a bit using some common sense, we get:

```python
Rocknroll = True
Silence = False
a_guitar = 19
Tommy = 44
Music = 160
the_music = input()
if int(the_music) == a_guitar:
    print("Keep on rocking!")
    the_rhythm = input()
    if int(the_rhythm) - Music == 0:
        Tommy = 66
        print(Tommy)
        Music = 79
        Jamming = 78
        print(Music)
        print(Jamming)
        Tommy = 74
        print(Tommy)
        #They are dazzled audiences
        print(it)
        Rock = 86
        print(it)
        Tommy = 73
        print(it)
        #break
        print("Bring on the rock!")
    else:
        print("That ain't it, Chief")
        #break
```

So, in order to bypass the first condition, we should enter `19` as input for `the_music`.

However, the official interpreter does not accept this, and it turns out the transpiler isn't decoding the language correctly.

This section explains how number literals are assigned.

> #### Poetic Number Literals
> 
> A poetic number literal begins with a variable name, followed by the keyword is, or the aliases was or were. As long as the next symbol is not a Literal Word, the rest of the line is treated as a decimal number in which the values of consecutive digits are given by the lengths of the subsequent barewords, up until the end of the line. To allow the digit zero, and to compensate for a lack of suitably rock’n’roll 1- and 2-letter words, word lengths are parsed modulo 10. A period (.) character denotes a decimal place. Other than the first period, any non-alphabetical characters are ignored.
> 
>  *   Tommy was a lovestruck ladykiller initialises Tommy with the value 100
>  *   Sweet Lucy was a dancer - initialises Sweet Lucy with the value 16


Therefore, the following command:
```
A guitar is a six-string        
```

Should be considered 136, and not 19. Entering 136 indeed prints "Keep on rocking!" and brings us to the next input request.

Now we need to enter the correct value for `the rhythm`, so that `the rhythm without Music is nothing`. 

`without` is a reserved word which indicates substraction:


```
Operator 	Operation 	    Aliases
+ 	        addition 	    plus, with
- 	        subtraction 	minus, without
* 	        multiplication 	times, of
/ 	        division 	    over
```

`nothing` is a reserved word which resembles zero:

> Null - the null type. Evaluates as equal to zero and equal to false. The keywords nothing, nowhere, nobody, empty and gone are defined as aliases for null

So as the transpiler correctly identified, we need to enter a value for `the rhythm` so that `the rhythm - Music == 0`.

According to the logic above, the value of `Music` is `a billboard-burning razzmatazz! == 1970`. Entering this gives us the following output in the online interpreter:

```
Keep on rocking!
66
79
78
74
79
86
73
```

This is pretty close to what the transpiler understood, only 79 is missing.

In any case, translated to ASCII, we get:

```python
>>> ascii = """66
... 79
... 78
... 74
... 79
... 86
... 73"""
>>> for c in ascii.split():
...     print(chr(int(c)), end='')
...
BONJOVI
```

So the flag is `picoCTF{BONJOVI}`.

### Update (November 2020)

It seems as though the Rockstar language specs have changed since the original challange was posted, and the hyphen has received a special meaning:

> The hyphen (-) is counted as a letter – so you can use terms like ‘all-consuming’ (13 letters > 3) and ‘power-hungry’ (12 letters > 2) instead of having to think of 12- and 13-letter words.

Therefore, using the updated specs, `a six-string` should count as `10` and `a billboard-burning razzmatazz` now equals `170`.

See [here](https://github.com/Dvd848/CTFs/issues/8) and [here](https://github.com/Dvd848/CTFs/issues/9) for more details.

