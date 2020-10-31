# Dinner Party
Category: Logic, 20 Points

## Description

> Five women sat in a row at the dinner table. Each had got there in a different way, each was eating a different kind of food, each had brought with her a different object and each had left at home a different pet. 
> 
> How were the women arranged, and which woman owned what object?
> 
> Invoke submit.py with your solution to obtain the flag. The solution should consist of "woman object" pairs (without quotes, case sensitive), in order of seating from left to right. 
> 
> For example:
> 
> submit.py Lovelace telescope Germain abacus Franklin laptop Curie pencil Noether scales
> 
> IMPORTANT: Give the program a minute or two to produce the flag for you.
> 
> Good Luck!

The following puzzle description was attached:
```
Five women sat in a row at the dinner table. Each had got there in a different way, each was eating a different kind of food, each had brought with her a different object and each had left at home a different pet.

Noether had many difficulties during the journey there by foot; Germain, who was at the far left, heard the woman sitting next to her telling tales of her journey by boat. The woman who had got there by train sat left of someone who had got there by plane; the woman eating burger mentioned that getting there was her first time traveling by train. The woman who had got there by bus mentioned how she had left her pet rabbit at home. One of the ladies opened her bag and took out her laptop; the woman next to her recalled how she used to have one of those too, but then her pet rabbit chewed it and spat it out completely useless. 

So Lovelace reached for her pocket and pulled out the pencil she had brought with her, at which the woman who owned a dog wondered whether it was more useful for her research than the scales that she herself owned. One of the ladies waved her abacus close to her neighbor's face; the neighbor, who owned a cat, retaliated by flipping over her assailant's soup. 

Franklin tried to ignore the noise and just eat her salad in peace. The woman who owned a fish was unhappily eating her cake, envying the chicken plate enjoyed by the guest at the center seat. When all was said and done, Curie had learned a lot from her colleagues but was happy to go home to her pet fish.

It was only two weeks later that one of the guests realized that she had forgotten her telescope there.

How were the women arranged, and which woman owned what object? Invoke submit.py with your solution to obtain the flag. The solution should consist of "woman object" pairs (without quotes, case sensitive), in order of seating from left to right. For example:

submit.py Lovelace telescope Germain abacus Franklin laptop Curie pencil Noether scales

IMPORTANT: Give the program a minute or two to produce the flag for you (or chastise you for providing the wrong solution).
```

In addition, `flag.txt.enc` and `submit.py` were attached.

## Solution

Let's use `z3` to solve this.

First, we start by creating Z3 objects for all possible objects in the puzzle:
```python
Name, name_consts = EnumSort("Name",
                             ["Germain", "Franklin", "Lovelace", "Curie", "Noether"])
germain, franklin, lovelace, curie, noether = name_consts

Arrival, arrival_consts = EnumSort("Arrival",
                                ["Boat", "Train", "Plane", "Foot", "Bus"])
boat, train, plane, foot, bus = arrival_consts

Food, food_consts = EnumSort("Food",
                                ["Burger", "Soup", "Salad", "Cake", "Chicken"])
burger, soup, salad, cake, chicken = food_consts

Object, object_consts = EnumSort("Object",
                                ["Laptop", "Pencil", "Scales", "Abacus", "Telescope"])
laptop, pencil, scales, abacus, telescope = object_consts

Pet, pet_consts = EnumSort("Pet",
                                ["Rabbit", "Dog", "Cat", "Fish", "Dont_know"])
rabbit, dog, cat, fish, dont_know = pet_consts
```

We create functions to be able to refer to the objects:
```python
location = Function("location", Name, IntSort())

arrival = Function("arrival", Name, Arrival)

food = Function("food", Name, Food)

obj = Function("obj", Name, Object)

pet = Function("pet", Name, Pet)
```

Now we start applying the constraints in the description:

```python
# Five women sat in a row at the dinner table. 
# Each had got there in a different way, each was eating a different kind of food, each had brought with her a different object and each had left at home a different pet.
constraints.append(Distinct([obj(name) for name in name_consts]))
constraints.append(Distinct([food(name) for name in name_consts]))
constraints.append(Distinct([arrival(name) for name in name_consts]))
constraints.append(Distinct([location(name) for name in name_consts]))
constraints.append(Distinct([pet(name) for name in name_consts]))

for name in name_consts:
    constraints.append(Or([location(name) == x for x in range(len(name_consts))]))
    constraints.append(Or([arrival(name) == x for x in arrival_consts]))
    constraints.append(Or([food(name) == x for x in food_consts]))
    constraints.append(Or([obj(name) == x for x in object_consts]))
    constraints.append(Or([pet(name) == x for x in pet_consts]))

# Noether had many difficulties during the journey there by foot; 
constraints.append(arrival(noether) == foot)

# Germain, who was at the far left, heard the woman sitting next to her telling tales of her journey by boat.
constraints.append(location(germain) == 0)
name_clue1 = Const("name_clue1", Name)
constraints.append(location(name_clue1) == 1)
constraints.append(arrival(name_clue1) == boat)

# The woman who had got there by train sat left of someone who had got there by plane;
name_clue2 = Const("name_clue2", Name)
name_clue3 = Const("name_clue3", Name)
constraints.append(arrival(name_clue2) == train)
constraints.append(arrival(name_clue3) == plane)
constraints.append(location(name_clue2) < location(name_clue3))

# the woman eating burger mentioned that getting there was her first time traveling by train
name_clue4 = Const("name_clue4", Name)
constraints.append(arrival(name_clue4) == train)
constraints.append(food(name_clue4) == burger)

# The woman who had got there by bus mentioned how she had left her pet rabbit at home
name_clue5 = Const("name_clue5", Name)
constraints.append(arrival(name_clue5) == bus)
constraints.append(pet(name_clue5) == rabbit)

# One of the ladies opened her bag and took out her laptop; 
# the woman next to her recalled how she used to have one of those too, but then her pet rabbit chewed it and spat it out completely useless.
name_clue6 = Const("name_clue6", Name)
name_clue7 = Const("name_clue7", Name)
constraints.append(pet(name_clue6) == rabbit)
constraints.append(obj(name_clue7) == laptop)
constraints.append(Or(location(name_clue6) == location(name_clue7) - 1, location(name_clue6) == location(name_clue7) + 1))

# So Lovelace reached for her pocket and pulled out the pencil she had brought with her, 
# at which the woman who owned a dog wondered whether it was more useful for her research than the scales that she herself owned
constraints.append(obj(lovelace) == pencil)
name_clue8 = Const("name_clue8", Name)
constraints.append(pet(name_clue8) == dog)
constraints.append(obj(name_clue8) == scales)

# One of the ladies waved her abacus close to her neighbor's face; the neighbor, who owned a cat, retaliated by flipping over her assailant's soup. 
name_clue9 = Const("name_clue9", Name)
constraints.append(obj(name_clue9) == abacus)
constraints.append(food(name_clue9) == soup)

name_clue10 = Const("name_clue10", Name)
name_clue11 = Const("name_clue11", Name)
constraints.append(obj(name_clue10) == abacus)
constraints.append(pet(name_clue11) == cat)
constraints.append(Or(location(name_clue10) == location(name_clue11) - 1, location(name_clue10) == location(name_clue11) + 1))

# Franklin tried to ignore the noise and just eat her salad in peace.
constraints.append(food(franklin) == salad)

# The woman who owned a fish was unhappily eating her cake, envying the chicken plate enjoyed by the guest at the center seat
name_clue12 = Const("name_clue12", Name)
constraints.append(pet(name_clue12) == fish)
constraints.append(food(name_clue12) == cake)

name_clue13 = Const("name_clue13", Name)
constraints.append(food(name_clue13) == chicken)
constraints.append(location(name_clue13) == 2)

# When all was said and done, Curie had learned a lot from her colleagues but was happy to go home to her pet fish.
constraints.append(pet(curie) == fish)
```

All that's left to do is wrap it all up:
```python
from z3 import *

solver = Solver()
constraints = []

### Insert constraints here ###

solver.add(constraints)

def print_model(m):
    s = [None] * len(name_consts)
    for name in name_consts:
        object_name = str(m.eval(obj(name)))
        s[m.eval(location(name)).as_long()] = "{} {}".format(name, object_name.lower())
    print(" ".join(s))

while solver.check() == sat:
    m = solver.model()
    print_model(m)

    expressions = []
    for name in name_consts:
        expressions.append(location(name) != m.eval(location(name)))
        expressions.append(arrival(name) != m.eval(arrival(name)))
        expressions.append(food(name) != m.eval(food(name)))
        expressions.append(obj(name) != m.eval(obj(name)))
        expressions.append(pet(name) != m.eval(pet(name)))
    
    solver.add(Or(expressions))
```

We want to get all possible answers, so each time we get a model we add a constraint that the next answer should have something different.

Output:
```console
root@kali:/media/sf_CTFs/checkpoint/Dinner_Party# python3 solve.py
Germain abacus Franklin laptop Noether scales Lovelace pencil Curie telescope
Germain scales Curie laptop Lovelace pencil Noether abacus Franklin telescope
Germain scales Curie telescope Lovelace pencil Franklin laptop Noether abacus
Germain scales Curie laptop Lovelace pencil Franklin telescope Noether abacus
```

We should try all four answers. The accepted answer is:
```console
root@kali:/media/sf_CTFs/checkpoint/Dinner_Party/dinner_party# python3 submit.py Germain abacus Franklin laptop Noether scales Lovelace pencil Curie telescope
Congratulations!
Your flag is:  CSA{70n19H7_We_d1Ne_1n_HELL}
```