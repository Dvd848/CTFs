# Java Script Kiddie 2
Web Exploitation, 450 points

## Description:
> The image link appears broken... twice as badly...


## Solution: 

This is the follow-up for [Java Script Kiddie](Java_Script_Kiddie.md).

The website source:

```html
<html>
	<head>    
		<script src="jquery-3.3.1.min.js"></script>
		<script>
			var bytes = [];
			$.get("bytes", function(resp) {
				bytes = Array.from(resp.split(" "), x => Number(x));
			});

			function assemble_png(u_in){
				var LEN = 16;
				var key = "00000000000000000000000000000000";
				var shifter;
				if(u_in.length == key.length){
					key = u_in;
				}
				var result = [];
				for(var i = 0; i < LEN; i++){
					shifter = Number(key.slice((i*2),(i*2)+1));
					for(var j = 0; j < (bytes.length / LEN); j ++){
						result[(j * LEN) + i] = bytes[(((j + shifter) * LEN) % bytes.length) + i]
					}
				}
				while(result[result.length-1] == 0){
					result = result.slice(0,result.length-1);
				}
				document.getElementById("Area").src = "data:image/png;base64," + btoa(String.fromCharCode.apply(null, new Uint8Array(result)));
				return false;
			}
		</script>
	</head>
	<body>

		<center>
			<form action="#" onsubmit="assemble_png(document.getElementById('user_in').value)">
				<input type="text" id="user_in">
				<input type="submit" value="Submit">
			</form>
			<img id="Area" src=""/>
		</center>

	</body>
</html>
```

We can see one main changes from last time: The key length is still 16 but the key itself has 32 characters. It looks like every second character is just a filler (`shifter = Number(key.slice((i*2),(i*2)+1))`) so this shouldn't matter much.

Our bytes array this time is:
```
78 126 175 206 13 83 94 147 206 120 69 141 0 81 146 131 137 72 175 71 0 54 30 32 157 13 227 0 73 31 47 15 0 58 14 196 141 215 63 54 222 0 0 0 0 147 155 75 164 32 174 86 198 254 73 15 116 0 0 13 155 222 250 205 48 243 78 66 48 76 57 0 0 0 0 0 204 0 156 69 51 198 1 71 140 130 20 10 0 120 0 237 166 0 97 0 234 63 0 114 215 10 239 114 1 3 156 81 178 72 248 82 20 68 68 2 10 0 168 65 84 138 244 216 222 192 73 108 255 80 20 35 101 73 0 65 170 91 89 20 129 75 0 164 27 0 208 14 146 72 26 150 4 227 132 55 129 205 68 117 156 0 19 12 1 18 1 136 207 64 141 51 124 235 95 173 61 16 103 225 154 50 68 192 120 0 182 32 235 157 142 52 190 124 49 142 232 183 44 176 3 99 126 202 181 120 74 141 229 214 56 126 169 100 94 146 70 72 252 91 222 88 211 117 254 190 116 73 241 222 23 159 214 232 86 184 154 88 206 61 220 140 120 105 60 238 112 78 239 129 156 201 7 204 227 84 253 234 253 38 114 248 11 204 84 188 249 240 47 49 190 29 155 0 124 85 155 43 75 177 5 33 73 84 127 30 65 215 73 156 87 35 242 106 249 87 34 77 141 241 64 221 35 55 255 122 77 191 0 79 113 30 191 55 191 198 0 210 126 169 31 31 185 64 148 31 1 16 239 233 253 86 228 250 227 22 52 107 146 145 109 233 1 68 191 8 58 129 210 66 233 129 35 237 147 234 205 98 119 0 228 55 250 69 233 133 55 23 61 80 74 95 74 9 71 1 205 212 254 19 229 104 161 219 30 132 1 136 99 43 105 189 233 201 55 17 169 226 247 130 90 4 40 205 116 111 189 0 156 116 228 100 22 121 103 128 147 30 159 107 150 202 236 173 186 0 170 204 227 197 103 231 230 231 79 253 120 182 55 119 227 103 59 176 127 34 105 252 245 58 54 94 24 123 176 172 10 222 34 126 253 161 23 254 165 242 1 212 158 87 29 124 104 9 169 237 106 18 230 196 250 40 74 126 227 252 49 190 9 36 141 121 118 16 201 101 114 154 41 173 211 98 33 114 79 221 141 60 185 63 107 219 228 236 252 111 243 203 230 245 141 42 228 43 94 15 13 47 82 52 193 61 61 189 202 166 230 103 245 34 172 5 235 114 82 80 165 121 114 95 247 57 151 214 106 200 38 111 139 183 123 141 119 30 87 135 232 239 6 255 78 167 248 0 202 218 148 195 157 123 206 215 36 139 40 101 200 235 154 242 224 140 140 213 210 6 225 111 242 252 83 109 119 199 39 222 140 57 118 183 246 252 127 247 10 26 7 159 163 73 102 170 16 60 255 155 186 55 196 42 226 159 43 139 151 91 82 132 184 165 135 44 153 94 74 167 252 34 215 90 36 61 23 31 23 189 165 165 242 95 233 242 96 82 17 68 64 187 239 221 212 122 36 133 143 127 211 79 159 1 93 166 120 248 75 4 214 8 61 247 185 241 1 121 161 158 159 141 223 235 55 190 225 189 249 95 153 42 96 199 60 165 247 148 67 172 0 128 150 224
```

After applying minor modifications to the script from the previous challenge, we get:

```python
from PIL import Image
import itertools, io, os
KEY_LEN = 16

def create_png(bytes_arr, key, out_dir_path):
    if not os.path.isdir(out_dir_path):
        raise Exception("Output folder ('{}') does not exist!".format(out_dir_path))
    result = [0] * len(bytes_arr)
    for i in range(KEY_LEN):
        shifter = int(key[i*2:i*2+1])
        for j in range(len(bytes_arr) // KEY_LEN):
            result[(j * KEY_LEN) + i] = bytes_arr[(((j + shifter) * KEY_LEN) % len(bytes_arr)) + i]
    img_bytes = io.BytesIO(bytes(result))

    try:
        img = Image.open(img_bytes)
        img.save(os.path.join(out_dir_path, "{}.png".format(key)))
        print ("Key {} produces a valid PNG - Saving".format(key))
    except IOError:
        print ("Key {} produces an invalid PNG - Ignoring".format(key))


shifters = []
for i in range(KEY_LEN):
    shifters.append([])
expected = [0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52]
with open("bytes.txt") as f:
    bytes_arr = list(map(int, f.read().split(" ")))
    for i in range(KEY_LEN):
        for shifter in range(10):
            j = 0
            offset = (((j + shifter) * KEY_LEN) % len(bytes_arr)) + i
            if bytes_arr[offset] == expected[i]:
                shifters[i].append(shifter)               

print (shifters)

for p in itertools.product(*shifters):
    key = "".join("{}a".format(n) for n in p)
    create_png(bytes_arr, key, "out")
```

We use '`a`' as the filler value for the key.

Running the script, we get:

```console
root@kali:/media/sf_CTFs/pico/Java_Script_Kiddie_2# python3 solve.py
[[1], [8], [4], [1, 5], [0], [6], [9], [5], [4, 5], [2, 3, 4], [2, 3, 4, 5], [3], [1], [6], [9], [6]]
Key 1a8a4a1a0a6a9a5a4a2a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a2a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a2a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a2a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a3a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a3a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a3a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a3a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a4a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a4a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a4a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a4a4a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a2a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a2a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a2a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a2a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a3a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a3a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a3a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a3a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a4a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a4a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a4a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a1a0a6a9a5a5a4a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a2a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a2a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a2a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a2a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a3a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a3a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a3a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a3a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a4a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a4a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a4a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a4a4a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a2a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a2a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a2a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a2a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a3a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a3a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a3a4a3a1a6a9a6a produces a valid PNG - Saving
Key 1a8a4a5a0a6a9a5a5a3a5a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a4a2a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a4a3a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a4a4a3a1a6a9a6a produces an invalid PNG - Ignoring
Key 1a8a4a5a0a6a9a5a5a4a5a3a1a6a9a6a produces an invalid PNG - Ignoring
```

The key that produced a valid image is `1a8a4a5a0a6a9a5a5a3a4a3a1a6a9a6a`:

![](images/qr2.png)

Which translated to:

```console
root@kali:/media/sf_CTFs/pico/Java_Script_Kiddie_2# zbarimg out/1a8a4a5a0a6a9a5a5a3a4a3a1a6a9a6a.png
QR-Code:picoCTF{e1f443bfe40e958050e0d74aec4daa48}
scanned 1 barcode symbols from 1 images in 0.05 seconds
```