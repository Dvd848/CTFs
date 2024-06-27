# xSerialGen

 * Category: Web
 * 380 Points
 * Solved by the JCTF Team

## Description

> During a reconnaissance, one of our analysts found a serial generator used by threat actors to operate a remote C2 licenses for his legit “customers” ... can you find a way to break implementation and get the flag?


## Solution

The attached website is a serial generator:

```html
<body>
  <div class="container">
    <h1 class="display-4 text-center mt-5">xSerial Generator</h1>
    <form action="#">
      <div class="form-group">
        Enter Your Name: <input type="text" id="serValInput" class="form-control">
      </div>
      <div class="form-group">
        Your Code Now Is: <input type="text" id="serValOutput" readonly class="form-control">
      </div>
      <input type="submit" id="ClickBtn" value="Generate!" class="btn btn-primary btn-block">
    </form>
  </div>
  <script>
    $(document).ready(function() {
        // jQuery code here
        $("#ClickBtn").click(function(){
          $.get("/api-gen?p=" + $("#serValInput").val(), function(data){
            if(data.code.length == 0){
              alert("No Valud Provided.");
            }else{
              $("#serValOutput").val(data.code);
            }
          });
          /**$.post("/convert?p={DecryptedValue}", function(flag){
            console.log(flag);
          });**/
          return false;
        });
    });
</script>
<!--- REMOVE BEORE PRODUCTION -->
<!-- @David: I've xored the information safely: 73605a535440677b670103050548675f10407a447f0347635900755b50064e -->
</body>
```

Notice the comment on the bottom of the page - we'll get back to that later.

We can play around with the API to see if it's consistent:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/xSerialGen]
└─$ curl 'https://bstlv24-xserialgen.chals.io/api-gen?p=test'
{"code":"45564043"}

┌──(user@kali)-[/media/sf_CTFs/bsides/xSerialGen]
└─$ curl 'https://bstlv24-xserialgen.chals.io/api-gen?p=test'
{"code":"45564043"}
```

If we try entering the flag format, we get a response similar to the binary sequence in the comment:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/xSerialGen]
└─$ curl 'https://bstlv24-xserialgen.chals.io/api-gen?p=BSidesTLV2024'
{"code":"73605a535440677b6701030505"}
```

Based on this and the comment content, we can guess that the server-side manipulation is just
a simple XOR. We can easily use this to calculate the key:

```python
>>> from pwn import *
>>> xor(b'BSidesTLV2024', bytes.fromhex('73605a535440677b6701030505'))
b'1337133713371'
```

This looks like a recurring key, `1337`. We can use it to decode the full binary sequence:

```python
>>> xor(bytes.fromhex('73605a535440677b670103050548675f10407a447f0347635900755b50064e'), b'1337')
b'BSidesTLV2024{Th!sIsN0tTh3Fla5}'
```

The result indeed isn't the flag, so what do we do with it?

Well, it turns out that if we send it to the `/convert?p={DecryptedValue}` API that appears
as a comment in the code, we get the real flag:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/xSerialGen]
└─$ curl --globoff --request POST 'https://bstlv24-xserialgen.chals.io/convert?p=BSidesTLV2024{Th!sIsN0tTh3Fla5}'
BSidesTLV2024{D0nTtRUSTX0RSerial}
```