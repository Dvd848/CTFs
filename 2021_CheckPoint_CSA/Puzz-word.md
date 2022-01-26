# Puzz-word
Category: Programming, 300 Points

## Description

>  One of my colleagues just loves puzzles.
> 
> He wrote this API that you can send and get a puzzle, and if you can solve it, you get the secret key to his bitcoin fortune.
> 
> We couldn't get it :(
> 
> Can you?

## Solution

Let's check the attached service:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Puzz-word]
└─$ curl https://puzzword.csa-challenge.com/help
Hello. To get your puzzle go to /puzzle instead of this /help.
The puzzle is a peg solitaire game. You are probably familiar with it, but if not,
you can read about it online, or even play it here: https://www.webgamesonline.com/peg-solitaire/
When you go to /puzzle, you will get the source and destination, and you will need to provide your solution.
The source will look something like this:
|   OOO   |
|   OOO   |
|OOOOOOOOO|
|OOOO.OOOO|
|OOOOOOOOO|
|   OOO   |
|   OOO   |

If you are familiar with this game, or you've just read about it, then you know what you need to do:
The pieces that are marked with 'O' can move above other pieces to an empty spot on the board (marked with '.')
The piece that you are jumping above will be taken out of the board.

You will need to provide the solution that will reach the destination.
The destination will look something like this:
|   ...   |
|   ...   |
|.........|
|....O....|
|.........|
|   ...   |
|   ...   |

The solution is the series of steps that will get you to the destination.
To describe a step we can use a coordinate system where 0,0 is the top left, 8,0 is top right
0,8 is bottom left and 8,8 is bottom right.

To describe the direction we can just use arrow-like characters:
< means left
> means right
and ^ and v mean up and down

The solution will be an array of moves that you need to send the /solve API
This should be a field in the JSON body of a POST request
Each move has 3 elements: x, y and direction.
For example:
{"solution": [[4,1,"v"], [6,2,"<"],......,[4,5,"^"]]}

In these first two steps we moved two pieces, and the board now looks something like this:
|   OOO   |
|   O.O   |
|OOOOO..OO|
|OOOOOOOOO|
|OOOOOOOOO|
|   OOO   |
|   OOO   |

Sending the needed steps to get to the destination will solve the puzzle!
```

The instructions are pretty detailed, let's see what we get from `/puzzle`:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Puzz-word]
└─$ curl https://puzzword.csa-challenge.com/puzzle
{"message": "{\"puzzle_id\": \"ee2d08c2-1adc-45f1-b34d-26c682d1a879\", \"source_board\": [\"  OOO  \", \"  OOO  \", \"OOOOOOO\", \"OOO.OOO\", \"OOOOOOO\", \"  OOO  \", \"  OOO  \"], \"destination_board\": [\"  ...  \", \"  ...  \", \".......\", \"...O...\", \".......\", \"  ...  \", \"  ...  \"]}"}
```

This maps to a pretty standard board:

```
      OOO           ...
      OOO           ...
    OOOOOOO       .......
    OOO.OOO  -->  ...O...
    OOOOOOO       .......
      OOO           ...
      OOO           ...
```

We can solve it manually or using any automated solver and submit the answer:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Puzz-word]
└─$ curl -X POST -H "Content-Type: application/json" https://puzzword.csa-challenge.com/solve --data '{"puzzle_id": "ee2d08c2-1adc-45f1-b34d-26c682d1a879", "solution": [[1, 3, ">"], [2, 1, "v"], [0, 2, ">"], [0, 4, "^"], [2, 3, "^"], [2, 0, "v"], [2, 4, "<"], [2, 6, "^"], [3, 2, "<"], [0, 2, ">"], [3, 0, "v"], [3, 2, "<"], [3, 4, "<"], [0, 4, ">"], [3, 6, "^"], [3, 4, "<"], [5, 2, "<"], [4, 0, "v"], [4, 2, "<"], [1, 2, ">"], [3, 2, "v"], [4, 4, "<"], [1, 4, ">"], [4, 6, "^"], [4, 3, "v"], [6, 4, "<"], [3, 4, ">"], [6, 2, "v"], [6, 4, "<"], [4, 5, "^"], [5, 3, "<"]]}'
{"message": "{\"message\": \"Congrats! now try another one\", \"puzzle_id\": \"463a2d09-c12a-4276-9f93-f5dbc03323d1\", \"source_board\": [\"  OOO  \", \"  OOO  \", \"OOOOOOO\", \"OOO.OOO\", \"OOOOOOO\", \"  OOO  \", \"  OOO  \"], \"destination_board\": [\"  .OO  \", \"  .OO  \", \"O.OOOOO\", \"..OOOOO\", \".O.OOOO\", \"  .OO  \", \"  OOO  \"]}"}
```

Obviously the first one was too easy. Since we'll need to solve a few, let's automate it. The following script is based on [this solver](https://blog.c-01a.de/posts/solving-peg-solitaire/) with some modifications:

```python
#!/usr/bin/env python3
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Based on https://blog.c-01a.de/posts/solving-peg-solitaire/ with modifications

from typing import List, Tuple

HOLE    = 0
PEG     = 1
INVALID = 2

class Board(object):
    def __init__(self, src_board, dst_board):
        if len(src_board) == 0 or len(src_board[0]) == 0:
            raise RuntimeError("Illegal board!")

        self.board = src_board
        self.dst_board = dst_board  # Shared among clones

        self.max_x = len(self.board) - 1
        self.max_y = len(self.board[0]) - 1

        # Lazy initialization to support fast clone:
        self.boards_played = None # Shared among clones
        self.src_num_holes = None
        self.dst_num_holes = None

    def __hash__(self):
        return hash(tuple([tuple(row) for row in self.board]))

    def clone(self):
        # Ca. 2x faster than copy.deepcopy()
        board_copy = [[peg for peg in row] for row in self.board]

        b = Board(board_copy, self.dst_board)

        b.boards_played = self.boards_played
        b.src_num_holes = self.src_num_holes
        b.dst_num_holes = self.dst_num_holes

        # dst_board, boards_played are shared among all clones!

        return b

    def possible_moves(self) -> List[Tuple[Tuple[int, int], Tuple[int, int]]]:
        moves = []  # Format: ((from x, from y), (to x, to y))

        # For each board position
        for x, row in enumerate(self.board):
            for y, slot in enumerate(row):
                # If occupied by a peg
                if slot == PEG:
                    # Find valid moves for this peg
                    peg_moves = self.moves_for_peg(x, y)

                    moves.extend([((x, y), move) for move in peg_moves])

        return moves

    def moves_for_peg(self, x, y) -> List[Tuple[int, int]]:
        assert 0 <= x <= self.max_x
        assert 0 <= y <= self.max_y

        peg_moves = []

        # x, y offsets for moves towards top, bottom, left, and right
        move_offsets = [(-2, 0), (2, 0), (0, -2), (0, 2)]

        for (dx, dy) in move_offsets:
            new_x = x + dx
            new_y = y + dy

            # If new position is inside the board
            if 0 <= new_x <= self.max_x and 0 <= new_y <= self.max_y:
                # If the new position is free
                if self.board[new_x][new_y] == 0:
                    # If there is a peg next to the current peg in the move's direction
                    if self.board[(x + new_x) // 2][(y + new_y) // 2] == PEG:
                        peg_moves.append((new_x, new_y))

        return peg_moves

    def move(self, move: Tuple[Tuple[int, int], Tuple[int, int]]):
        (from_x, from_y), (to_x, to_y) = move

        # Delete peg from old position
        assert self.board[from_x][from_y] == PEG
        self.board[from_x][from_y] = HOLE
        # Place peg at new position
        assert self.board[to_x][to_y] == HOLE
        self.board[to_x][to_y] = PEG
        # Delete peg in between
        assert self.board[(from_x + to_x) // 2][(from_y + to_y) // 2] == PEG
        self.board[(from_x + to_x) // 2][(from_y + to_y) // 2] = HOLE

        # Use self.num_holes to make sure src_num_holes is initialized
        self.src_num_holes = self.num_holes + 1

        return self

    @classmethod
    def count_holes(cls, board):
        num_holes = 0
        for slot in [p for row in board for p in row]:
            num_holes += 1 if slot == HOLE else 0
        return num_holes

    @property
    def num_holes(self):
        if self.src_num_holes is None:
            self.src_num_holes = self.count_holes(self.board)
        return self.src_num_holes

    def dead_end(self):
        if self.boards_played is None:
            self.boards_played = set()

        h = hash(self)

        if h in self.boards_played:
            return True

        self.boards_played.add(h)

        if self.dst_num_holes is None:
            self.dst_num_holes = self.count_holes(self.dst_board)

        if self.num_holes > self.dst_num_holes:
            return True

        return False

    def is_solved(self):
        return self.board == self.dst_board

def solve_recursive(board, move_memo=()):
    if board.dead_end():
        return None

    moves = board.possible_moves()

    if board.is_solved():
        return move_memo
    else:
        for move in moves:
            result = solve_recursive(board.clone().move(move), [mm for mm in move_memo] + [move])
            if result:
                return result
    return None

board_slot_translation = {
   " ": INVALID,
   "O": PEG,
   ".": HOLE
}

def build_board(board_spec):
    board = [list(map(lambda c: board_slot_translation[c], row)) for row in board_spec]
    # Rotate board
    return list(map(list, zip(*board)))

def translate_moves(moves):
    res = []
    for m in moves:
        if m[0][0] > m[1][0]:
            move = "<"
        elif m[0][0] < m[1][0]:
            move = ">"
        elif m[0][1] > m[1][1]:
            move = "^"
        else:
            move = "v"
        res.append([m[0][0], m[0][1], move])
    return res


if __name__ == '__main__':
    from pwn import *
    import requests
    import json

    res = requests.get("https://puzzword.csa-challenge.com/puzzle")
    while "message" in res.json():
        json_res = res.json()
        message = json.loads(json_res["message"])

        if "message" in message:
            log.info(f"Got Message:\n{message['message']}")

        if not "source_board" in message or not "destination_board" in message or not "puzzle_id" in message:
            break

        log.info("Source board:\n{}\n\n".format('\n'.join(message['source_board'])))
        log.info("Destination board:\n{}\n\n".format('\n'.join(message['destination_board'])))

        src_board = build_board(message['source_board'])
        dst_board = build_board(message['destination_board'])

        moves = solve_recursive(Board(src_board, dst_board))
        if moves is None:
            log.error("Can't solve board!")

        solution = translate_moves(moves)

        log.info(f"Solved board, moves:\n{solution}")

        res = requests.post("https://puzzword.csa-challenge.com/solve", json={"puzzle_id": message["puzzle_id"], "solution": solution})

```

This is a recursive solver: For each state, it tries out all the legal derived states possible until the solution is found. However, in order to reduce the search time to a reasonable time, we must implement some kind of [pruning](https://en.wikipedia.org/wiki/Decision_tree_pruning), since exploring the complete search space is very time consuming and mostly redundant. The pruning heuristics are implemented in `dead_end`: This is the method that decides if it's worth to continue perusing this branch. The method will decide that this path is a *dead end* worth pruning if it already visited it from another path, or if the number of holes in the current state is larger than the number of holes in the destination state. This works quite well in our case (another algorithm that worked quite well on the puzzle set can be found [here](https://github.com/mkhrapov/peg-solitaire-solver)).

The output we get is:

```console
┌──(user@kali)-[/media/sf_CTFs/checkpoint/Puzz-word]
└─$ python3 solve.py -O
[*] Source board:
      OOO
      OOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
      OOO
      OOO

[*] Destination board:
      ...
      ...
    .......
    ...O...
    .......
      ...
      ...

[*] Solved board, moves:
    [[1, 3, '>'], [2, 1, 'v'], [0, 2, '>'], [0, 4, '^'], [2, 3, '^'], [2, 0, 'v'], [2, 4, '<'], [2, 6, '^'], [3, 2, '<'], [0, 2, '>'], [3, 0, 'v'], [3, 2, '<'], [3, 4, '<'], [0, 4, '>'], [3, 6, '^'], [3, 4, '<'], [5, 2, '<'], [4, 0, 'v'], [4, 2, '<'], [1, 2, '>'], [3, 2, 'v'], [4, 4, '<'], [1, 4, '>'], [4, 6, '^'], [4, 3, 'v'], [6, 4, '<'], [3, 4, '>'], [6, 2, 'v'], [6, 4, '<'], [4, 5, '^'], [5, 3, '<']]
[*] Got Message:
    Congrats! now try another one
[*] Source board:
      OOO
      OOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
      OOO
      OOO

[*] Destination board:
      OOO
      OOO
    ...OOOO
    ...OOOO
    OOO.OOO
      O.O
      OOO

[*] Solved board, moves:
    [[1, 3, '>'], [2, 1, 'v'], [0, 2, '>'], [3, 3, '<'], [0, 3, '>'], [2, 3, '^'], [3, 5, '^']]
[*] Got Message:
    Super! what about this french one?
[*] Source board:
      OOO
     OOOOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
     OOOOO
      OOO

[*] Destination board:
      OOO
     O..O.
    OOOO.OO
    OOOOO.O
    OOOOO.O
     OOOOO
      OOO

[*] Solved board, moves:
    [[3, 1, 'v'], [5, 1, '<'], [2, 1, '>'], [5, 2, '<'], [5, 4, '^']]
[*] Got Message:
    Amazing job! let's do another french
[*] Source board:
      OOO
     OOOOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
     OOOOO
      OOO

[*] Destination board:
      OO.
     OO.OO
    OO..OOO
    OOOOO.O
    OOOO..O
     OOO.O
      OOO

[*] Solved board, moves:
    [[3, 1, 'v'], [5, 2, '<'], [4, 0, 'v'], [4, 3, '^'], [2, 2, '>'], [4, 5, '^'], [5, 4, '^']]
[*] Got Message:
    {you_are_done!_good_job}
[*] Source board:
    OOOOOO
    OOOOOO
    OOOOOO
    O.OOOO
    OOOOOO

[*] Destination board:
    OOO.OO
    O.OOO.
    OOOO.O
    OO.O.O
    OO.OOO

[*] Solved board, moves:
    [[1, 1, 'v'], [3, 2, '<'], [2, 4, '^'], [3, 0, 'v'], [5, 1, '<'], [4, 3, '^']]
[*] Got Message:
    I was joking of course. but keep going
[*] Source board:
    OOOOOO
    OOOOOO
    OOOOOO
    O.OOOO
    OOOOOO

[*] Destination board:
    OOOOOO
    OOOO..
    OOOOOO
    ..OO..
    OOOO..

[*] Solved board, moves:
    [[3, 3, '<'], [0, 3, '>'], [3, 1, 'v'], [3, 4, '^'], [5, 1, '<'], [5, 3, '<'], [5, 4, '<']]
[*] Got Message:
    never under estimate your power. Check this out
[*] Source board:
    OOOOOOOOO
    OOOOOOOOO
    OO     OO
    OO     O.
    OO     OO
    OOOOOOOOO
    OOOOOOOOO

[*] Destination board:
    OOOOOOOOO
    OOOOOOOOO
    OO     OO
    OO     OO
    OO     O.
    OOOO..O.O
    OOOOOOOOO

[*] Solved board, moves:
    [[8, 5, '^'], [6, 5, '>'], [4, 5, '>']]
[*] Got Message:
    Too easy for you ha? What about this one?
[*] Source board:
    OOOOOOOOO
    OOOOOOOOO
    OO     OO
    OO     O.
    OO     OO
    OOOOOOOOO
    OOOOOOOOO

[*] Destination board:
    OOOOOOOOO
    OOOOOOOOO
    OO     O.
    OO     .O
    OO     O.
    OOOO..O..
    OOOOOOO.O

[*] Solved board, moves:
    [[8, 5, '^'], [6, 5, '>'], [4, 5, '>'], [7, 3, 'v'], [7, 6, '^'], [8, 2, 'v'], [8, 5, '^']]
[*] Got Message:
    help me find my sunglasses because you are on fire!
[*] Source board:
    OOOOOOOOO
    OOOOOOOOO
    OO     OO
    OO     O.
    OO     OO
    OOOOOOOOO
    OOOOOOOOO

[*] Destination board:
    OOOOOOOOO
    OOOO..O.O
    OO     OO
    OO     O.
    OO     .O
    OOOOO..O.
    OOOOOOO..

[*] Solved board, moves:
    [[8, 1, 'v'], [6, 1, '>'], [4, 1, '>'], [8, 4, '^'], [8, 6, '^'], [6, 6, '>'], [7, 4, 'v'], [5, 5, '>'], [8, 6, '<']]
[*] Got Message:
    3rd time is a charm
[*] Source board:
    OO  OO  OO  OO
    OO  OO  OO  OO
    OOOOOO..OOOOOO
    OO  OO  OO  OO
    OO  OO  OO  OO

[*] Destination board:
    OO  OO  OO  OO
    OO  OO  OO  OO
    OOOOOO....OOOO
    OO  OO  ..  OO
    OO  OO  ..  OO

[*] Solved board, moves:
    [[9, 2, '<'], [8, 4, '^'], [7, 2, '>'], [10, 2, '<'], [9, 4, '^'], [8, 2, '>']]
[*] Got Message:
    Oh wow. Keep it up
[*] Source board:
    .OOOOO..O
    OOO..OO.O
    OO     OO
    OO     O.
    OO     OO
    O..OOOOOO
    OOOOOO.OO

[*] Destination board:
    .OOO..O.O
    OOO.O..OO
    OO     .O
    OO     ..
    OO     OO
    O.O..OOOO
    OOOO.O..O

[*] Solved board, moves:
    [[4, 0, '>'], [4, 5, '<'], [4, 6, '>'], [6, 1, '<'], [7, 3, '^'], [7, 6, '<']]
[*] Got Message:
    ring the bells the final round is coming
[*] Source board:
         O
        OOO
       OOOOO
      OOOOOOO
     OOOO.OOOO
    OOOOOOOOOOO
     OOOOOOOOO
      OOOOOOO
       OOOOO
        OOO
         O

[*] Destination board:
         O
        OOO
       OOOOO
      OOOOOOO
     OOO.OOOOO
    O..O...OOOO
     ..OOOOOOO
      OOOOOOO
       OOOOO
        OOO
         O

[*] Solved board, moves:
    [[3, 4, '>'], [1, 4, '>'], [1, 6, '^'], [2, 6, '^'], [4, 6, '^'], [6, 5, '<'], [4, 4, 'v']]
[*] Got Message:
    Yes that was a diamond. Here is another one
[*] Source board:
         O
        OOO
       OO.OO
      O.OOO.O
     OOOO.OOOO
    OOO.OOO.OOO
     O.OOOOOOO
      OO.OO.O
       OOO.O
        O.O
         O

[*] Destination board:
         O
        OOO
       OO.OO
      O.O..O.
     OOOOO...O
    OOO.OOOOO.O
     O.OOOOOOO
      OOO...O
       OOO.O
        O.O
         O

[*] Solved board, moves:
    [[5, 3, '>'], [6, 7, '<'], [7, 4, '<'], [9, 5, '<'], [8, 3, 'v']]
[*] Got Message:
    wonderful job. Only 6 more to go!
[*] Source board:
      OOO
      OOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
      OOO
      OOO

[*] Destination board:
      ...
      ...
    .......
    ...O...
    .......
      ...
      ...

[*] Solved board, moves:
    [[1, 3, '>'], [2, 1, 'v'], [0, 2, '>'], [0, 4, '^'], [2, 3, '^'], [2, 0, 'v'], [2, 4, '<'], [2, 6, '^'], [3, 2, '<'], [0, 2, '>'], [3, 0, 'v'], [3, 2, '<'], [3, 4, '<'], [0, 4, '>'], [3, 6, '^'], [3, 4, '<'], [5, 2, '<'], [4, 0, 'v'], [4, 2, '<'], [1, 2, '>'], [3, 2, 'v'], [4, 4, '<'], [1, 4, '>'], [4, 6, '^'], [4, 3, 'v'], [6, 4, '<'], [3, 4, '>'], [6, 2, 'v'], [6, 4, '<'], [4, 5, '^'], [5, 3, '<']]
[*] Got Message:
    Easy right? I was just checking that you are awake
[*] Source board:
      OOO
     OOOOO
    OOOOOOO
    OOO.OOO
    OOOOOOO
     OOOOO
      OOO

[*] Destination board:
      OOO
     OOOOO
    O.OOOOO
    O.OO.OO
    OOOO.OO
     O.OO.
      O..

[*] Solved board, moves:
    [[3, 5, '^'], [1, 4, '>'], [1, 2, 'v'], [2, 6, '^'], [4, 6, '<'], [5, 5, '<'], [4, 3, 'v']]
[*] Got Message:
    thank you for solving this
[*] Source board:
    ..OO..   .OO.
      ..OO..  .OO
        ..OO..
          ..OO..
            ..OO.
    .       ...OO
    .OO..OO..OO..

[*] Destination board:
    .O....   ...O
      ....O.  .OO
        ..OO..
          ..OO..
            ..O..
    .       ....O
    ...OO....OOO.

[*] Solved board, moves:
    [[1, 6, '>'], [3, 0, '<'], [4, 1, '>'], [6, 6, '<'], [10, 0, '>'], [11, 4, 'v']]
[*] Got Message:
    Rock and roll!
[*] Source board:
    OOOOOOOOO
      ..O..
     O.OOO.O.
      OO.OO
    .O.O.O.O.

[*] Destination board:
    .O..OOOOO
      .....
     O.O...O.
      ..O..
    O..O.O.O.

[*] Solved board, moves:
    [[3, 3, '^'], [4, 1, '<'], [2, 0, 'v'], [0, 0, '>'], [2, 2, 'v'], [2, 4, '<'], [3, 0, '<'], [5, 2, '<'], [6, 3, '<']]
[*] Got Message:
    unbelieveble!
[*] Source board:
    OO.OO
    O.O.O
    OO.OO
         OO.OO
    .    O.O.O
    .    OO.OO

[*] Destination board:
    OO..O
    O....
    OOOO.
         OO..O
    .    O....
    .    .OO..

[*] Solved board, moves:
    [[4, 0, '<'], [2, 0, 'v'], [4, 2, '^'], [5, 5, '>'], [8, 5, '<'], [9, 3, '<'], [7, 3, 'v'], [9, 5, '^']]
[*] Got Message:
    Spectacular!
[*] Source board:
    OO  OO  OO  OO
    OO  OO  OO  OO
    OOOOOO..OOOOOO
    OO  OO  OO  OO
    OO  OO  OO  OO

[*] Destination board:
    OO  O.  O.  OO
    OO  ..  O.  OO
    OOOOOO.OO.O..O
    OO  O.  OO  OO
    OO  ..  OO  OO

[*] Solved board, moves:
    [[4, 2, '>'], [4, 4, '^'], [4, 1, 'v'], [5, 0, 'v'], [6, 2, '<'], [5, 4, '^'], [9, 2, '<'], [9, 0, 'v'], [10, 2, '<'], [12, 2, '<']]
[*] Got Message:
    Thank you for solving all the puzzles! Bye
```

You're welcome and everything, but where's the flag??

Well, there aren't too many places where the flag can hide. After running the script a few times, we note that:

 * The source boards and messages are always the same
 * The destination boards (and therefore solutions) change between runs
 * Some boards are symmetric, so clearly there's more than one way to solve some boards

This means that the flag is probably hiding in the constant parts of the challenge: The source boards or the messages. The source boards have consecutive repetitions so we'll concentrate on the messages.

Looking closely, we can see that the first message starts with "C", the second with "S" and the third with "A". If we continue collecting the first characters, we get: `CSA{In_Th3OrY_wE_tRuST`. The accepted flag was `CSA{In_Th3OrY_wE_tRuST}`.