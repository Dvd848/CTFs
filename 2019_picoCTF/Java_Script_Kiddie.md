# Java Script Kiddie
Web Exploitation, 400 points

## Description:
> The image link appears broken...


## Solution: 

The source of the attached website is as follows:

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
				var key = "0000000000000000";
				var shifter;
				if(u_in.length == LEN){
					key = u_in;
				}
				var result = [];
				for(var i = 0; i < LEN; i++){
					shifter = key.charCodeAt(i) - 48;
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

If we enter a random value in the form, we receive a broken image. It looks like the javascript code first makes a request to receive a list of bytes:
```
137 147 206 121 126 59 121 231 0 4 121 188 174 107 242 107 0 124 231 234 119 50 0 0 1 147 229 170 73 153 65 191 164 121 48 249 53 254 26 10 84 199 169 75 0 28 1 53 48 82 78 249 171 61 1 114 224 133 203 169 155 66 255 130 145 80 1 183 76 131 68 65 89 229 134 32 113 72 148 82 12 0 0 184 2 218 186 76 169 250 151 249 194 192 96 108 170 0 134 157 119 75 131 120 112 231 157 255 131 65 68 235 231 12 6 192 0 222 228 69 249 69 255 1 157 110 95 204 59 230 195 71 13 0 183 112 6 0 78 68 32 78 138 41 76 175 239 114 0 10 107 243 179 0 0 13 25 144 54 75 141 137 226 2 135 0 74 146 212 120 0 0 49 119 154 121 231 191 113 63 61 73 207 252 109 29 156 237 237 205 44 181 255 234 45 201 81 7 111 130 76 72 160 71 252 189 222 4 45 102 16 164 153 122 6 236 97 221 248 223 189 159 191 15 186 67 214 172 254 6 103 1 239 222 111 241 128 173 157 249 176 240 95 173 172 207 128 248 254 100 121 231 32 123 102 156 187 25 219 126 99 10 127 136 254 208 144 42 223 245 199 191 126 63 242 65 146 62 217 103 195 245 123 51 216 107 122 239 128 4 205 81 87 164 77 166 255 1 56 191 144 200 56 6 36 54 208 38 51 163 146 136 97 152 102 86 67 136 54 254 211 246 158 221 31 179 191 243 21 0 230 168 247 251 222 113 110 91 32 183 22 244 45 1 220 168 97 180 247 205 207 14 199 186 146 106 250 144 115 222 151 173 111 131 220 144 233 122 255 219 176 73 150 157 22 143 74 217 68 101 232 176 170 113 254 92 169 250 106 153 148 251 31 124 122 156 191 239 186 121 94 197 5 252 248 34 55 29 252 124 181 226 55 127 124 206 48 75 126 234 94 137 135 121 191 89 24 135 209 221 81 56 181 115 52 122 68 77 240 90 247 182 197 190 111 203 15 236 79 123 223 170 144 93 244 56 247 7 190 219 76 219 245 39 129 110 133 226 176 46 193 253 78 206 160 247 86 91 37 36 24 234 87 241 98 57 36 107 14 56 242 135 213 233 55 229 121 228 96 149 95 6 92 30 230 184 44 149 107 102 16 42 182 95 228 158 18 21 32 205 77 53 94 221 173 252 21 127 15 82 95 163 242 120 224 214 163 0 230 91 72 244 111 255 32 95 225 170 135 57 178 172 87 45 193 84 78 90 202 51 178 127 63 249 165 189 47 239 27 208 191 248 67 223 59 130 249 207 207 49 252 164 114 231 181 214 188 181 227 26 125 27 198 147 64 200 215 229 143 249 29 171 227 252 115 152 80 6 143 53 160 77 210 113 143 214 204 219 20 236 102 252 153 126 12 3 249 152 188 213 49 94 226 157 55 144 180 159 251 199 200 141 203 31 146 9 206 111 7 85 183 119 73 111 109 171 243 240 189 215 29 215 242 235 162 178 174 106 73 165 173 84 219 232 242 250 59 147 235 27 119 211 11 99 98 142 102 37 139 64 58 30 49 178 226 245 73 253 239 252 179 115 177 18
```

It then performs swaps based on the key entered by the user. If the key is correct, the swaps will turn this byte array into a valid PNG image.

The key is 16 characters long, and judging according to the `key.charCodeAt(i) - 48` snippet, valid characters are only digits (since this is a well-known method to turn a digit's ASCII value to the actual digit it represents).

A [PNG file](http://www.libpng.org/pub/png/spec/1.2/PNG-Structure.html) starts with a magic signature of eight bytes (`89 50 4E 47 0D 0A 1A 0A`) followed by a series of chunks, where  each chunk header has a well-known structure: 4 bytes of length and 4 bytes of chunk type. The first chunk is called `IHDR` and has the length of 0XD, so we know that the next 8 bytes are `00 00 00 0D 49 48 44 52`.

```console
root@kali:/media/sf_CTFs/pico/Java_Script_Kiddie# xxd -g 1 ../Flags/flag.png | head
00000000: 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52  .PNG........IHDR
00000010: 00 00 06 6a 00 00 04 47 08 02 00 00 00 7c 8b ab  ...j...G.....|..
00000020: 78 00 00 00 01 73 52 47 42 00 ae ce 1c e9 00 00  x....sRGB.......
00000030: 00 04 67 41 4d 41 00 00 b1 8f 0b fc 61 05 00 00  ..gAMA......a...
00000040: 00 09 70 48 59 73 00 00 16 25 00 00 16 25 01 49  ..pHYs...%...%.I
00000050: 52 24 f0 00 00 a8 8e 49 44 41 54 78 5e ec dd 2d  R$.....IDATx^..-
00000060: 94 dc 46 da 30 ec 07 18 0e 34 0c 34 0c 0c da b3  ..F.0....4.4....
00000070: 30 30 70 e1 c2 3d 8b 0c 0d 0d 0d 03 16 18 06 06  00p..=..........
00000080: 06 e6 3c c8 e0 01 86 86 66 6b 18 68 e8 f3 a2 9c  ..<.....fk.h....
00000090: f3 7d 25 d5 8f ee 2a a9 66 ba 67 ba c7 3f b9 ae  .}%...*.f.g..?..
```

Since we know how the first 16 bytes of a PNG file should look like, we can guess the key.

For each character of the key, we just need to try all digits until we find one that places the expected value in the current location. 

Note that depending on our input, we might end up with several keys that correctly recreate the first 16 bytes of the PNG file. We should ignore any solution which doesn't produce a valid PNG file though.

Here's a script that does this:
```python
from PIL import Image
import itertools, io, os
KEY_LEN = 16

def create_png(bytes_arr, key, out_dir_path):
    if not os.path.isdir(out_dir_path):
        raise Exception("Output folder ('{}') does not exist!".format(out_dir_path))
    result = [0] * len(bytes_arr)
    for i in range(KEY_LEN):
        shifter = int(key[i])
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

for p in itertools.product(*shifters):
    key = "".join("{}".format(n) for n in p)
    create_png(bytes_arr, key, "out")
```

Running the script produces the following key:
```console
root@kali:/media/sf_CTFs/pico/Java_Script_Kiddie# python3 solve.py
[[0], [4], [3], [8], [8], [9], [2], [2], [0], [8, 9], [9], [9], [1], [4], [6], [4]]
Key 0438892208991464 produces a valid PNG - Saving
Key 0438892209991464 produces an invalid PNG - Ignoring
```

Using the valid key, we get the following image:

![](images/qr.png)

Which translates to the flag:

```console
root@kali:/media/sf_CTFs/pico/Java_Script_Kiddie# zbarimg out/0438892208991464.png
QR-Code:picoCTF{905765bf9ae368ad98261c10914d894e}
scanned 1 barcode symbols from 1 images in 0.11 seconds
```

