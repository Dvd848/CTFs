# Strange Game
Category: Misc., 100 Points

## Description

> It's just winning a simple game.
> 
> What could go wrong?


## Solution

Let's connect to the attached service:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Strange_Game]
└─$ nc strange-game.csa-challenge.com 4444

==================
   Strange Game
==================

• We take turns picking numbers from 1 to 9.
• The same number can't be chosen twice.
• First player to hold 3 numbers which, when added, make 15, wins.
• If all numbers are exhausted without a winner, the game is a tie.
• We will play 15 rounds of this game, alternating who goes first.
• Win or tie all 15 rounds and I'll give you the flag.





Press any key...
```

Sounds simple enough. Let's try to play:
```
Round 1!
==================================================
New game
I'll go first.
==================================================
Available moves: 1 2 3 4 5 6 7 8 9
Past moves:
        Player One: None
        Player Two: None
Game still in progress
I choose to play 4.
==================================================
==================================================
Available moves: 1 2 3 5 6 7 8 9
Past moves:
        Player One: 4
        Player Two: None
Game still in progress
Choose your move: 6
==================================================
==================================================
Available moves: 1 2 3 5 7 8 9
Past moves:
        Player One: 4
        Player Two: 6
Game still in progress
I choose to play 8.
==================================================
==================================================
Available moves: 1 2 3 5 7 9
Past moves:
        Player One: 8 4
        Player Two: 6
Game still in progress
Choose your move: 3
==================================================
==================================================
Available moves: 1 2 5 7 9
Past moves:
        Player One: 8 4
        Player Two: 3 6
Game still in progress
I choose to play 2.
==================================================
==================================================
Available moves: 1 5 9 7
Past moves:
        Player One: 8 2 4
        Player Two: 3 6
Game still in progress
Choose your move: 5
==================================================
==================================================
Available moves: 1 9 7
Past moves:
        Player One: 8 2 4
        Player Two: 3 5 6
Game still in progress
I choose to play 9.
==================================================
==================================================
Available moves: None
Past moves:
        Player One: 8 9 2 4
        Player Two: 3 5 6
Winner: Player One
You lose!
Better luck next time!
```

Well, the computer seems pretty good at this. Doesn't seem very fair though, it can efficiently review all current and future moves to choose the optimal one. We should do the same.

We'll start by implementing a basic representation of the game, in a file called `number_scrabble.py` (since from a quick search, that's what the game is called).

```python
__all__ = ['NumberScrabble', 'Players']

from enum import Enum
from math import inf
from collections import namedtuple
from itertools import combinations
from functools import reduce
from typing import List, Tuple
import operator

TARGET_SCORE = 15
VALID_MOVES = range(1, 10)
TUPLE_LENGTH = 3

OR_BITS         = lambda bit_nums: reduce(operator.or_, (1 << bit_num for bit_num in bit_nums))
IS_BIT_SET      = lambda value, bit_num: value &   (1 << bit_num)
SET_BIT         = lambda value, bit_num: value |   (1 << bit_num)
UNSET_BIT       = lambda value, bit_num: value & (~(1 << bit_num))
GET_BITS_SET    = lambda value: [i for i in VALID_MOVES if IS_BIT_SET(value, i)]

WINNING_STATES_BITMASK = [OR_BITS(t) for t in combinations(VALID_MOVES, TUPLE_LENGTH) if sum(t) == TARGET_SCORE]
ALL_MOVES_BITMASK = OR_BITS(VALID_MOVES)

MiniMaxResult = namedtuple("MiniMaxResult", "move score")

class Players(Enum):
    MY_PLAYER    = 1
    OTHER_PLAYER = 2

class NumberScrabble():
    def __init__(self):
        self.moves = {
            Players.MY_PLAYER: 0,
            Players.OTHER_PLAYER: 0
        }
        pass

    def make_move(self, player: Players, move: int) -> None:
        if not IS_BIT_SET(self._available_moves, move):
            raise ValueError(f"Move {move} not available")
        self.moves[player] = SET_BIT(self.moves[player], move)

    def undo_move(self, player: Players, move: int) -> None:
        if not IS_BIT_SET(self.moves[player], move):
            raise ValueError(f"Move {move} never performed by player {player}")
        self.moves[player] = UNSET_BIT(self.moves[player], move)

    @property
    def _available_moves(self) -> int:
        return (~(self.moves[Players.MY_PLAYER] | self.moves[Players.OTHER_PLAYER])) & ALL_MOVES_BITMASK

    @property
    def available_moves(self) -> List[int]:
        return GET_BITS_SET(self._available_moves)

    def __str__(self) -> str:
        return f"NumberScrabble(My moves = {GET_BITS_SET(self.moves[Players.MY_PLAYER])}, " \
               f"Other moves = {GET_BITS_SET(self.moves[Players.OTHER_PLAYER])}, " \
               f"Available moves: {GET_BITS_SET(self._available_moves)})"

    def __repr__(self) -> str:
        return str(self)

    def player_wins(self, player: Players) -> bool:
        return any( ((self.moves[player] & winning_state) == winning_state) for winning_state in WINNING_STATES_BITMASK)

    def game_over(self) -> bool:
        return any(self.player_wins(player) for player in Players) or self._available_moves == 0

    def get_best_move(self) -> int:
        # TODO: Implement
```

This implementation uses bitmasks under the hood to represent the game state. Each player has an integer representing the moves they've made by setting the matching bit. For example, if we choose the number `1`, the value will be `0b0000000010`, and if we later choose the number `3`, the value will turn to `0b0000001010`.

We precalculate the different winning states once using:

```python
>>> WINNING_STATES_BITMASK = [OR_BITS(t) for t in combinations(VALID_MOVES, TUPLE_LENGTH) if sum(t) == TARGET_SCORE]
>>> [bin(x) for x in WINNING_STATES_BITMASK]
['0b1000100010', '0b101000010', '0b1000010100', '0b100100100', '0b11000100', '0b100011000', '0b10101000', '0b1110000']
>>> [GET_BITS_SET(x) for x in WINNING_STATES_BITMASK]
[[1, 5, 9], [1, 6, 8], [2, 4, 9], [2, 5, 8], [2, 6, 7], [3, 4, 8], [3, 5, 7], [4, 5, 6]]
```

This means that checking if a player won can simply be done using basic bitwise operations - we just need to check if the bits set in the player's moves are a match with one of the winning states.

Notice that we've left a placeholder for `get_best_move` - this is how we'll decide what's the optimal move we should make. We'll implement that soon enough.

At this point we can go on and implement the wrapping logic for our game representation. This is the logic that is responsible for interacting with the challenge server on one hand and with our game representation on the other hand.

```python
from pwn import *
from number_scrabble import *

NUM_ROUNDS = 15

context.newline = b'\r\n'

r = remote("strange-game.csa-challenge.com", 4444)

for round in range(NUM_ROUNDS):
    r.sendafter(b"Press any key...", b"\n")

    r.recvuntil(b"New game\r\n")
    current_player = {"I'll go first.": Players.OTHER_PLAYER, "You go first.": Players.MY_PLAYER}[r.recvlineS(keepends = False)]
    log.info(f"New game (round #{round + 1}), first player: {current_player.name}")

    ns = NumberScrabble()
    done = False

    with log.progress('Playing...') as p:
        while True:
            r.recvuntil(b"Available moves: ")
            available_moves = r.recvlineS(keepends = False)
            if available_moves == "None":
                r.recvline()
                result = r.recvuntilS(b'\r\n\r\n')
                break
            if current_player == Players.OTHER_PLAYER:
                r.recvuntil(b"I choose to play")
                move = int(r.recvuntil(b".", drop = True))
                p.status(f"They chose {move}")
                ns.make_move(current_player, move)
                current_player = Players.MY_PLAYER
            elif current_player == Players.MY_PLAYER:
                p.status(f"Calculating best move")
                best_move = ns.get_best_move()
                p.status(f"I chose {best_move}")
                ns.make_move(current_player, best_move)
                r.sendafter(b"Choose your move: ", f"{best_move}\n".encode("ascii"))
                current_player = Players.OTHER_PLAYER
    
    log.info(f"Game over:\n{result}")
```

Nothing too fancy here, just read the state from the server, update the game representation with the state and query the suggested move, then update the server and repeat.

All that's left is to implement `get_best_move`. Now, one very interesting thing about [Number Scrabble](https://en.wikipedia.org/wiki/Number_Scrabble) is:

> Number Scrabble (also known as Pick15 or 3 to 15) is a mathematical game where players take turns to select numbers from 1 to 9 without repeating any numbers previously used, and the first player with a sum of exactly 15 using any three of their number selections wins the game. The game is isomorphic to tic-tac-toe, as can be seen if the game is mapped onto a magic square.

This game is just a fancy way to play tic-tac-toe:

> The game is identical to tic-tac-toe, as can be seen by reference to a 3x3 magic square: If a player has selected three numbers which can be found in a line on a magic square, they will add up to 15. If they have selected any other three numbers, they will not.

```
+---+---+---+
| 2 | 7 | 6 |
+---+---+---+
| 9 | 5 | 1 |
+---+---+---+
| 4 | 3 | 8 |
+---+---+---+
```

So, if we research [tic-tac-toe](https://en.wikipedia.org/wiki/Tic-tac-toe) a bit, we'll find that "it is a solved game with a forced draw assuming best play from both players", and that the best play can be calculated using an algorithm called [Minimax](https://en.wikipedia.org/wiki/Minimax):

> Minimax (sometimes MinMax, MM[1] or saddle point[2]) is a decision rule used in artificial intelligence, decision theory, game theory, statistics, and philosophy for minimizing the possible loss for a worst case (maximum loss) scenario. When dealing with gains, it is referred to as "maximin"—to maximize the minimum gain.
> 
> ...
> 
> A simple version of the minimax algorithm, deals with games such as tic-tac-toe, where each player can win, lose, or draw. If player A can win in one move, their best move is that winning move. If player B knows that one move will lead to the situation where player A can win in one move, while another move will lead to the situation where player A can, at best, draw, then player B's best move is the one leading to a draw. Late in the game, it's easy to see what the "best" move is. The Minimax algorithm helps find the best move, by working backwards from the end of the game. At each step it assumes that player A is trying to maximize the chances of A winning, while on the next turn player B is trying to minimize the chances of A winning (i.e., to maximize B's own chances of winning). 

The Minimax algorithm is pretty simple to implement:

```python
def _calc_score(state: NumberScrabble) -> int:
    if state.player_wins(Players.MY_PLAYER):
        return +1
    elif state.player_wins(Players.OTHER_PLAYER):
        return -1
    else:
        return 0

def _minimax(depth, state: NumberScrabble, player: Players) -> Tuple[int, int]:
    if state.game_over():
        score = _calc_score(state)
        return MiniMaxResult(None, score)

    if player == Players.MY_PLAYER:
        best_result = MiniMaxResult(None, -inf)
        next_player = Players.OTHER_PLAYER
    else:
        assert(player == Players.OTHER_PLAYER)
        best_result = MiniMaxResult(None, +inf)
        next_player = Players.MY_PLAYER

    for move in state.available_moves:
        state.make_move(player, move)
        result = _minimax(depth + 1, state, next_player)
        state.undo_move(player, move)
        
        if player == Players.MY_PLAYER:
            if result.score > best_result.score:
                best_result = MiniMaxResult(move, result.score)
        else:
            if result.score < best_result.score:
                best_result = MiniMaxResult(move, result.score)

    return best_result
```

And our `get_best_move` method becomes:

```python
    def get_best_move(self) -> int:
        return _minimax(0, self, Players.MY_PLAYER).move
```

As stated in the Wikipedia article, by playing an optimal strategy we can always at least force a draw. Let's see if it works:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Strange_Game]
└─$ python3 solve.py
[+] Opening connection to strange-game.csa-challenge.com on port 4444: Done
[*] New game (round #1), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 3 6 7 8 9
        Player Two: 1 2 4 5
    Game tied

[*] New game (round #2), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 7
        Player Two: 8 9 5 6
    Game tied

[*] New game (round #3), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 5 6 7 8 9
        Player Two: 1 2 3 4
    Game tied

[*] New game (round #4), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 7
        Player Two: 8 9 5 6
    Game tied

[*] New game (round #5), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 3 6 7 8 9
        Player Two: 1 2 4 5
    Game tied

[*] New game (round #6), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 7
        Player Two: 8 9 5 6
    Game tied

[*] New game (round #7), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 5 6 7 8 9
        Player Two: 1 2 3 4
    Game tied

[*] New game (round #8), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 5
        Player Two: 8 9 6 7
    Game tied

[*] New game (round #9), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 3 6 7 8 9
        Player Two: 1 2 4 5
    Game tied

[*] New game (round #10), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 7
        Player Two: 8 9 5 6
    Game tied

[*] New game (round #11), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 2 3 6 8 9
        Player Two: 1 4 5 7
    Game tied

[*] New game (round #12), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 5
        Player Two: 8 9 6 7
    Game tied

[*] New game (round #13), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 4 6 7 8 9
        Player Two: 1 2 3 5
    Game tied

[*] New game (round #14), first player: MY_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 1 2 3 4 7
        Player Two: 8 9 5 6
    Game tied

[*] New game (round #15), first player: OTHER_PLAYER
[+] Playing...: Done
[*] Game over:
        Player One: 4 6 7 8 9
        Player Two: 1 2 3 5
    Game tied
    Congratulations!
    CSA{https://www.youtube.com/watch?v=NHWjlCaIrQo}

[*] Closed connection to strange-game.csa-challenge.com port 4444
```

Looks like "the only winning move is not to play".