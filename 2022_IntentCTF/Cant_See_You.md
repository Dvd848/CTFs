# Can't See You
* Category: Misc.
* 200 Points
* Solved by the JCTF Team

## Description

> It's too dark in here, can you help Alice find her way out?
>
> docker run -it g3rzi/rabbit_hole bash

## Solution

Let's run the attached docker. 

```
    |\\      //(
    | \\    //  '
    '  \\  //  /
     \  )\((  /
      )`    `/
     /   __  \
    /   (_O)  \
   /           \________
_.(_)           )      /
   (__,        /      /
    \         /      /
     \_______/      (
      \    /         \
       \  /           \
        \/             \
        /               )
       /               /
      / _     o__     /
     ( (_)   //\,\   (
      \    ``~---~`   )
\      \             /
 \      \           /
  \      \         /
   \____/ \_______/
------------------------------------------------
"...she found herself falling down a very deep well.
Either the well was very deep, or she fell very slowly,
for she had plenty of time as she went down to look about her and to wonder what was going to happen next.
First, she tried to look down and make out what she was coming to, but it was too dark to see anything."
/home #
```

Not too much to see here. The docker is running locally so this can't be an escape challenge. Let's inspect the docker container itself:


```console
$ docker history --no-trunc g3rzi/rabbit_hole
IMAGE                                                                     CREATED        CREATED BY                                                                                                       SIZE      COMMENT
sha256:6c206a5f4eb94555206db9eb5cea0883fcaea96d4a10aee4ea9d05220737d119   3 months ago   /bin/sh -c #(nop) WORKDIR /home                                                                                  0B
<missing>                                                                 3 months ago   /bin/sh -c #(nop)  ENTRYPOINT ["/bin/sh" "-c" "clear; cat /home/message; sh;"]                       0B
<missing>                                                                 3 months ago   /bin/sh -c rm /tmp/Wonderland                       0B
<missing>                                                                 3 months ago   /bin/sh -c #(nop) COPY file:562947c0a4dbfc439d967ce78b6a06318e475dff95157295a63e5cd1d357f299 in /home/message    932B
<missing>                                                                 3 months ago   /bin/sh -c #(nop) COPY dir:ce226651d1a2141a3df6d4bbb9135fb5de96b4f36ca8bd170d16f016f2ea00e2 in /                 11.7MB
<missing>                                                                 4 months ago   /bin/sh -c #(nop)  CMD ["/bin/sh"]                       0B
<missing>                                                                 4 months ago   /bin/sh -c #(nop) ADD file:2a949686d9886ac7c10582a6c29116fd29d3077d02755e87e111870d63607725 in /                 5.54MB
```

We can see that one of the layers removed a file! What are you, `/tmp/Wonderland`?

To find it, we export the docker image to a `tar` file with `docker save --output rabbit_hole.tar g3rzi/rabbit_hole`. Then we untar it:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ tar -xvf rabbit_hole.tar
1288696addccc4013c5bcf61c1b6c38128a7214a0942976792918b51912d90f7/
1288696addccc4013c5bcf61c1b6c38128a7214a0942976792918b51912d90f7/VERSION
1288696addccc4013c5bcf61c1b6c38128a7214a0942976792918b51912d90f7/json
1288696addccc4013c5bcf61c1b6c38128a7214a0942976792918b51912d90f7/layer.tar
60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/
60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/VERSION
60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/json
60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/layer.tar
6c206a5f4eb94555206db9eb5cea0883fcaea96d4a10aee4ea9d05220737d119.json
ac98da2332a56465c337fe58aca626d67aa296273a22c6d8d88a12fd449ccd77/
ac98da2332a56465c337fe58aca626d67aa296273a22c6d8d88a12fd449ccd77/VERSION
ac98da2332a56465c337fe58aca626d67aa296273a22c6d8d88a12fd449ccd77/json
ac98da2332a56465c337fe58aca626d67aa296273a22c6d8d88a12fd449ccd77/layer.tar
c9b634909bbfee5adf85d62a358060f24a5063828d9b95111ded28ff6058fab0/
c9b634909bbfee5adf85d62a358060f24a5063828d9b95111ded28ff6058fab0/VERSION
c9b634909bbfee5adf85d62a358060f24a5063828d9b95111ded28ff6058fab0/json
c9b634909bbfee5adf85d62a358060f24a5063828d9b95111ded28ff6058fab0/layer.tar
manifest.json
repositories
```

One of the layers will contain the file:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ find . -name "layer.tar" -exec bash -c "echo {} && tar -tvf {} | grep Wonderland" \;
./1288696addccc4013c5bcf61c1b6c38128a7214a0942976792918b51912d90f7/layer.tar
./60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/layer.tar
-rw-rw-r-- 0/0         6163456 2022-09-14 15:02 tmp/Wonderland
./ac98da2332a56465c337fe58aca626d67aa296273a22c6d8d88a12fd449ccd77/layer.tar
-rw------- 0/0               0 2022-09-14 15:02 tmp/.wh.Wonderland
./c9b634909bbfee5adf85d62a358060f24a5063828d9b95111ded28ff6058fab0/layer.tar
```

Let's extract it:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ tar -xvf ./60ba9005d6f27b9225eae9f87baebfced06103206174e465a956c01137381f96/layer.tar tmp/Wonderland
tmp/Wonderland

┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ file tmp/Wonderland
tmp/Wonderland: POSIX tar archive
```

It's another `tar` file. In we go.

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ tar -xvf Wonderland
047229ac3650fb687d05206f9c8b505b5d5214382e16ee09830ee1ccbca7c7fe/
047229ac3650fb687d05206f9c8b505b5d5214382e16ee09830ee1ccbca7c7fe/VERSION
047229ac3650fb687d05206f9c8b505b5d5214382e16ee09830ee1ccbca7c7fe/json
047229ac3650fb687d05206f9c8b505b5d5214382e16ee09830ee1ccbca7c7fe/layer.tar
0925e1c7e551c36f296419983a429564a6cf219a2857e7da4fa36d80b8ad607c/
0925e1c7e551c36f296419983a429564a6cf219a2857e7da4fa36d80b8ad607c/VERSION
0925e1c7e551c36f296419983a429564a6cf219a2857e7da4fa36d80b8ad607c/json
0925e1c7e551c36f296419983a429564a6cf219a2857e7da4fa36d80b8ad607c/layer.tar
...
fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/
fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/VERSION
fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/json
fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/layer.tar
fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/
fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/VERSION
fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/json
fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/layer.tar
manifest.json
repositories
```

More layers. Most of them just have a file called `door`:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ find . -name "*.tar" -exec bash -c "echo {} && tar -tvf {}" \; | tail
./efd3bc07eee5db05b82a77d899f59a27ab648f1e9a534a58e8fc9c9a35733e3f/layer.tar
-rw-r--r-- 0/0               2 2022-09-14 15:00 door
./effe4bc861a420e0ac1a85d415606ca4cacbcb66c7ffcd2d3e2db55167aab69f/layer.tar
-rw-r--r-- 0/0               2 2022-09-14 14:59 door
./f15b16837781b337f7f99b5b549515afeb9fdef405f11a115299611d1720365d/layer.tar
-rw-r--r-- 0/0               2 2022-09-14 14:59 door
./fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/layer.tar
-rw-r--r-- 0/0               2 2022-09-14 14:59 door
./fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/layer.tar
-rw-r--r-- 0/0               2 2022-09-14 15:00 door
```

Let's take a look at a few:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ tar -xvf ./fe588e64e86c132eb6b7064f08a860cb0ec02de1d0d196c6da09ff68f535ccf3/layer.tar
door

┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ cat door
z

┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ tar -xvf ./fae8145d76a4d64fa960075ac73fabfa2b36676a2c140fc1a380e6012cb68b1c/layer.tar
door

┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you/tmp]
└─$ cat door
5
```

Random characters. So let's extract them all and try to piece together a message. We'll follow the order from `manifest.json`.

```python
import json
import tarfile
from pathlib import Path

BASE_PATH = Path("tmp")
DOOR_FILE = "door"

output = b""

with open(BASE_PATH / "manifest.json") as manifest_file:
    manifest = json.load(manifest_file)[0]
    
    for layer in manifest["Layers"]:
        with tarfile.open(BASE_PATH / layer) as tar:
            if DOOR_FILE in tar.getnames():
                door = tar.extractfile(DOOR_FILE)
                output += door.read().strip()

print(output)
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ python3 solve.py
b'SU5URU5Ue2FfbDE3N2wzX2cwbGQzbl9rM3lfZjByX2FfbDE3N2wzX2QwMHJ9'
```

That looks like base64. We add `print(base64.b64decode(output).decode())` to the script in order to get the flag:

```console
┌──(user@kali)-[/media/sf_CTFs/intent/Cant_see_you]
└─$ python3 solve.py
b'SU5URU5Ue2FfbDE3N2wzX2cwbGQzbl9rM3lfZjByX2FfbDE3N2wzX2QwMHJ9'
INTENT{a_l177l3_g0ld3n_k3y_f0r_a_l177l3_d00r}
```