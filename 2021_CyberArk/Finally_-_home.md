# Finally - home! (5/5)
Category: Innovation

## Description

> https://s3.us-west-2.amazonaws.com/cyber-ctf.be/chl5/9f2d4057-618a-4016-a854-e6ed23d30b21.html

## Solution

We visit the attached website and get a form titled "Ready to Land?":

```html
<form id="form">

    <div class="form__group field">
        <input type="input" class="form__field" placeholder="Answer" name="answer" id='answer' required />
        <label for="answer" class="form__label">Answer</label>
    </div>

</form>
```

Behind the scenes, the logic is handled by a WebAssembly script:

```javascript
            const memory = new WebAssembly.Memory({ initial: 256, maximum: 256 });
            const importObj = {
                env: {
                    abortStackOverflow: () => { throw new Error('overflow'); },
                    table: new WebAssembly.Table({ initial: 0, maximum: 0, element: 'anyfunc' }),
                    __table_base: 0,
                    memory: memory,
                    __memory_base: 1024,
                    STACKTOP: 0,
                    STACK_MAX: memory.buffer.byteLength,
                }
            };

            document.getElementById('form').addEventListener('submit', function(e) {
                e.preventDefault();
                (async () => {
                const buff = new Uint8Array([
                    221, 188, 174, 176, 220, 221, 221, 221, 220, 245, 223, 189, 220, 162, 221, 189, 194, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 162, 220, 162, 223, 190, 216, 222, 184, 179, 171, 207, 188, 191, 178, 175, 169, 142, 169, 188, 190, 182, 146, 171, 184, 175, 187, 177, 178, 170, 221, 221, 222, 184, 179, 171, 208, 130, 130, 176, 184, 176, 178, 175, 164, 130, 191, 188, 174, 184, 222, 162, 221, 222, 184, 179, 171, 209, 130, 130, 169, 188, 191, 177, 184, 130, 191, 188, 174, 184, 222, 162, 221, 222, 184, 179, 171, 219, 176, 184, 176, 178, 175, 164, 223, 220, 93, 223, 93, 223, 222, 184, 179, 171, 216, 169, 188, 191, 177, 184, 220, 173, 220, 221, 221, 222, 223, 220, 220, 219, 202, 222, 162, 220, 156, 125, 205, 214, 162, 220, 156, 125, 77, 29, 223, 214, 160, 220, 158, 221, 221, 221, 221, 214, 218, 213, 220, 217, 130, 171, 184, 175, 221, 220, 215, 59, 199, 220, 62, 199, 220, 7, 223, 162, 254, 223, 252, 42, 223, 254, 223, 156, 93, 220, 183, 249, 223, 254, 223, 254, 222, 147, 217, 157, 156, 93, 220, 205, 221, 214, 253, 221, 252, 7, 220, 253, 220, 252, 6, 220, 253, 223, 252, 59, 220, 253, 222, 252, 44, 220, 253, 217, 252, 46, 220, 253, 216, 252, 41, 220, 253, 219, 252, 40, 220, 253, 218, 252, 43, 220, 253, 213, 252, 42, 220, 253, 212, 252, 37, 220, 253, 215, 252, 1, 220, 253, 214, 252, 0, 220, 253, 209, 252, 3, 220, 253, 208, 252, 2, 220, 253, 211, 252, 61, 220, 253, 210, 252, 60, 220, 253, 205, 252, 63, 220, 253, 204, 252, 62, 220, 253, 207, 252, 57, 220, 253, 206, 252, 56, 220, 253, 201, 252, 58, 220, 253, 200, 252, 53, 220, 253, 203, 252, 52, 220, 253, 202, 252, 55, 220, 253, 197, 252, 54, 220, 253, 196, 252, 49, 220, 253, 199, 252, 48, 220, 253, 198, 252, 51, 220, 253, 193, 252, 50, 220, 253, 192, 252, 45, 220, 253, 195, 252, 47, 220, 156, 221, 252, 11, 223, 253, 7, 220, 252, 194, 253, 194, 156, 162, 174, 252, 69, 223, 253, 69, 223, 156, 122, 220, 172, 252, 65, 220, 253, 7, 220, 252, 253, 253, 253, 156, 5, 163, 172, 252, 64, 220, 253, 65, 220, 253, 64, 220, 175, 252, 106, 223, 253, 106, 223, 156, 28, 220, 182, 252, 10, 223, 223, 162, 253, 10, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 36, 220, 253, 11, 223, 252, 246, 253, 246, 253, 36, 220, 183, 252, 160, 253, 160, 252, 11, 223, 253, 6, 220, 252, 235, 253, 235, 156, 162, 174, 252, 116, 223, 253, 116, 223, 156, 205, 172, 252, 25, 220, 253, 6, 220, 252, 156, 253, 156, 156, 178, 172, 252, 20, 220, 253, 25, 220, 253, 20, 220, 175, 252, 19, 223, 253, 19, 223, 156, 253, 182, 252, 50, 223, 223, 162, 253, 50, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 79, 223, 253, 11, 223, 252, 145, 253, 145, 253, 79, 223, 183, 252, 69, 220, 253, 69, 220, 252, 11, 223, 253, 59, 220, 252, 138, 253, 138, 156, 162, 174, 252, 104, 223, 253, 104, 223, 156, 45, 220, 172, 252, 67, 220, 253, 59, 220, 252, 191, 253, 191, 156, 82, 163, 172, 252, 126, 220, 253, 67, 220, 253, 126, 220, 175, 252, 102, 223, 253, 102, 223, 156, 95, 220, 182, 252, 1, 223, 223, 162, 253, 1, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 34, 220, 253, 11, 223, 252, 176, 253, 176, 253, 34, 220, 183, 252, 88, 220, 253, 88, 220, 252, 11, 223, 253, 44, 220, 252, 165, 253, 165, 156, 162, 174, 252, 127, 223, 253, 127, 223, 156, 66, 220, 172, 252, 105, 220, 253, 44, 220, 252, 252, 253, 252, 156, 61, 163, 172, 252, 100, 220, 253, 105, 220, 253, 100, 220, 175, 252, 27, 223, 253, 27, 223, 156, 29, 220, 182, 252, 58, 223, 223, 162, 253, 58, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 87, 223, 253, 11, 223, 252, 255, 253, 255, 253, 87, 223, 183, 252, 82, 220, 253, 82, 220, 252, 11, 223, 253, 46, 220, 252, 254, 253, 254, 156, 162, 174, 252, 118, 223, 253, 118, 223, 156, 45, 220, 172, 252, 31, 220, 253, 46, 220, 252, 249, 253, 249, 156, 82, 163, 172, 252, 30, 220, 253, 31, 220, 253, 30, 220, 175, 252, 23, 223, 253, 23, 223, 156, 75, 220, 182, 252, 55, 223, 223, 162, 253, 55, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 81, 223, 253, 11, 223, 252, 248, 253, 248, 253, 81, 223, 183, 252, 77, 220, 253, 77, 220, 252, 11, 223, 253, 41, 220, 252, 251, 253, 251, 156, 162, 174, 252, 113, 223, 253, 113, 223, 156, 28, 221, 172, 252, 24, 220, 253, 41, 220, 252, 250, 253, 250, 156, 99, 162, 172, 252, 27, 220, 253, 24, 220, 253, 27, 220, 175, 252, 22, 223, 253, 22, 223, 156, 233, 182, 252, 54, 223, 223, 162, 253, 54, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 80, 223, 253, 11, 223, 252, 245, 253, 245, 253, 80, 223, 183, 252, 76, 220, 253, 76, 220, 252, 11, 223, 253, 40, 220, 252, 244, 253, 244, 156, 162, 174, 252, 112, 223, 253, 112, 223, 156, 52, 221, 172, 252, 26, 220, 253, 40, 220, 252, 247, 253, 247, 156, 75, 162, 172, 252, 21, 220, 253, 26, 220, 253, 21, 220, 175, 252, 17, 223, 253, 17, 223, 156, 198, 182, 252, 49, 223, 223, 162, 253, 49, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 83, 223, 253, 11, 223, 252, 241, 253, 241, 253, 83, 223, 183, 252, 79, 220, 253, 79, 220, 252, 11, 223, 253, 43, 220, 252, 240, 253, 240, 156, 162, 174, 252, 115, 223, 253, 115, 223, 156, 236, 172, 252, 23, 220, 253, 43, 220, 252, 243, 253, 243, 156, 147, 172, 252, 22, 220, 253, 23, 220, 253, 22, 220, 175, 252, 16, 223, 253, 16, 223, 156, 24, 221, 182, 252, 48, 223, 223, 162, 253, 48, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 82, 223, 253, 11, 223, 252, 242, 253, 242, 253, 82, 223, 183, 252, 78, 220, 253, 78, 220, 252, 11, 223, 253, 42, 220, 252, 237, 253, 237, 156, 162, 174, 252, 114, 223, 253, 114, 223, 156, 42, 221, 172, 252, 17, 220, 253, 42, 220, 252, 236, 253, 236, 156, 85, 162, 172, 252, 16, 220, 253, 17, 220, 253, 16, 220, 175, 252, 18, 223, 253, 18, 223, 156, 194, 182, 252, 51, 223, 223, 162, 253, 51, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 77, 223, 253, 11, 223, 252, 239, 253, 239, 253, 77, 223, 183, 252, 73, 220, 253, 73, 220, 252, 11, 223, 253, 37, 220, 252, 238, 253, 238, 156, 162, 174, 252, 109, 223, 253, 109, 223, 156, 209, 172, 252, 19, 220, 253, 37, 220, 252, 233, 253, 233, 156, 174, 172, 252, 18, 220, 253, 19, 220, 253, 18, 220, 175, 252, 13, 223, 253, 13, 223, 156, 226, 182, 252, 45, 223, 223, 162, 253, 45, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 76, 223, 253, 11, 223, 252, 232, 253, 232, 253, 76, 223, 183, 252, 72, 220, 253, 72, 220, 252, 11, 223, 253, 1, 220, 252, 234, 253, 234, 156, 162, 174, 252, 108, 223, 253, 108, 223, 156, 15, 220, 172, 252, 13, 220, 253, 1, 220, 252, 229, 253, 229, 156, 112, 163, 172, 252, 12, 220, 253, 13, 220, 253, 12, 220, 175, 252, 12, 223, 253, 12, 223, 156, 125, 220, 182, 252, 44, 223, 223, 162, 253, 44, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 78, 223, 253, 11, 223, 252, 228, 253, 228, 253, 78, 223, 183, 252, 75, 220, 253, 75, 220, 252, 11, 223, 253, 0, 220, 252, 231, 253, 231, 156, 162, 174, 252, 111, 223, 253, 111, 223, 156, 49, 221, 172, 252, 15, 220, 253, 0, 220, 252, 230, 253, 230, 156, 78, 162, 172, 252, 14, 220, 253, 15, 220, 253, 14, 220, 175, 252, 15, 223, 253, 15, 223, 156, 238, 182, 252, 47, 223, 223, 162, 253, 47, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 73, 223, 253, 11, 223, 252, 225, 253, 225, 253, 73, 223, 183, 252, 74, 220, 253, 74, 220, 252, 11, 223, 253, 3, 220, 252, 224, 253, 224, 156, 162, 174, 252, 110, 223, 253, 110, 223, 156, 101, 220, 172, 252, 9, 220, 253, 3, 220, 252, 227, 253, 227, 156, 26, 163, 172, 252, 8, 220, 253, 9, 220, 253, 8, 220, 175, 252, 14, 223, 253, 14, 223, 156, 84, 220, 182, 252, 46, 223, 223, 162, 253, 46, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 72, 223, 253, 11, 223, 252, 226, 253, 226, 253, 72, 223, 183, 252, 68, 220, 253, 68, 220, 252, 11, 223, 253, 2, 220, 252, 157, 253, 157, 156, 162, 174, 252, 105, 223, 253, 105, 223, 156, 54, 220, 172, 252, 11, 220, 253, 2, 220, 252, 159, 253, 159, 156, 73, 163, 172, 252, 10, 220, 253, 11, 220, 253, 10, 220, 175, 252, 9, 223, 253, 9, 223, 156, 88, 220, 182, 252, 41, 223, 223, 162, 253, 41, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 75, 223, 253, 11, 223, 252, 158, 253, 158, 253, 75, 223, 183, 252, 71, 220, 253, 71, 220, 252, 11, 223, 253, 61, 220, 252, 153, 253, 153, 156, 162, 174, 252, 107, 223, 253, 107, 223, 156, 43, 221, 172, 252, 5, 220, 253, 61, 220, 252, 152, 253, 152, 156, 84, 162, 172, 252, 4, 220, 253, 5, 220, 253, 4, 220, 175, 252, 8, 223, 253, 8, 223, 156, 205, 182, 252, 40, 223, 223, 162, 253, 40, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 74, 223, 253, 11, 223, 252, 155, 253, 155, 253, 74, 223, 183, 252, 70, 220, 253, 70, 220, 252, 11, 223, 253, 60, 220, 252, 154, 253, 154, 156, 162, 174, 252, 68, 223, 253, 68, 223, 156, 233, 172, 252, 66, 220, 253, 60, 220, 252, 149, 253, 149, 156, 150, 172, 252, 125, 220, 253, 66, 220, 253, 125, 220, 175, 252, 101, 223, 253, 101, 223, 156, 217, 182, 252, 5, 223, 223, 162, 253, 5, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 39, 220, 253, 11, 223, 252, 148, 253, 148, 253, 39, 220, 183, 252, 163, 253, 163, 252, 11, 223, 253, 63, 220, 252, 151, 253, 151, 156, 162, 174, 252, 71, 223, 253, 71, 223, 156, 8, 220, 172, 252, 124, 220, 253, 63, 220, 252, 150, 253, 150, 156, 119, 163, 172, 252, 127, 220, 253, 124, 220, 253, 127, 220, 175, 252, 100, 223, 253, 100, 223, 156, 87, 220, 182, 252, 4, 223, 223, 162, 253, 4, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 38, 220, 253, 11, 223, 252, 144, 253, 144, 253, 38, 220, 183, 252, 162, 253, 162, 252, 11, 223, 253, 62, 220, 252, 147, 253, 147, 156, 162, 174, 252, 70, 223, 253, 70, 223, 156, 216, 172, 252, 121, 220, 253, 62, 220, 252, 146, 253, 146, 156, 167, 172, 252, 120, 220, 253, 121, 220, 253, 120, 220, 175, 252, 103, 223, 253, 103, 223, 156, 59, 221, 182, 252, 7, 223, 223, 162, 253, 7, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 33, 220, 253, 11, 223, 252, 141, 253, 141, 253, 33, 220, 183, 252, 93, 220, 253, 93, 220, 252, 11, 223, 253, 57, 220, 252, 140, 253, 140, 156, 162, 174, 252, 65, 223, 253, 65, 223, 156, 54, 221, 172, 252, 123, 220, 253, 57, 220, 252, 143, 253, 143, 156, 73, 162, 172, 252, 122, 220, 253, 123, 220, 253, 122, 220, 175, 252, 97, 223, 253, 97, 223, 156, 2, 221, 182, 252, 6, 223, 223, 162, 253, 6, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 32, 220, 253, 11, 223, 252, 142, 253, 142, 253, 32, 220, 183, 252, 92, 220, 253, 92, 220, 252, 11, 223, 253, 56, 220, 252, 137, 253, 137, 156, 162, 174, 252, 64, 223, 253, 64, 223, 156, 55, 221, 172, 252, 117, 220, 253, 56, 220, 252, 136, 253, 136, 156, 72, 162, 172, 252, 116, 220, 253, 117, 220, 253, 116, 220, 175, 252, 96, 223, 253, 96, 223, 156, 6, 221, 182, 252, 0, 223, 223, 162, 253, 0, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 35, 220, 253, 11, 223, 252, 139, 253, 139, 253, 35, 220, 183, 252, 95, 220, 253, 95, 220, 252, 11, 223, 253, 58, 220, 252, 133, 253, 133, 156, 162, 174, 252, 67, 223, 253, 67, 223, 156, 120, 220, 172, 252, 119, 220, 253, 58, 220, 252, 132, 253, 132, 156, 7, 163, 172, 252, 118, 220, 253, 119, 220, 253, 118, 220, 175, 252, 99, 223, 253, 99, 223, 156, 73, 220, 182, 252, 3, 223, 223, 162, 253, 3, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 93, 223, 253, 11, 223, 252, 135, 253, 135, 253, 93, 223, 183, 252, 94, 220, 253, 94, 220, 252, 11, 223, 253, 53, 220, 252, 134, 253, 134, 156, 162, 174, 252, 66, 223, 253, 66, 223, 156, 209, 172, 252, 113, 220, 253, 53, 220, 252, 129, 253, 129, 156, 174, 172, 252, 112, 220, 253, 113, 220, 253, 112, 220, 175, 252, 98, 223, 253, 98, 223, 156, 14, 221, 182, 252, 2, 223, 223, 162, 253, 2, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 92, 223, 253, 11, 223, 252, 128, 253, 128, 253, 92, 223, 183, 252, 89, 220, 253, 89, 220, 252, 11, 223, 253, 52, 220, 252, 131, 253, 131, 156, 162, 174, 252, 125, 223, 253, 125, 223, 156, 107, 220, 172, 252, 115, 220, 253, 52, 220, 252, 130, 253, 130, 156, 20, 163, 172, 252, 114, 220, 253, 115, 220, 253, 114, 220, 175, 252, 29, 223, 253, 29, 223, 156, 91, 220, 182, 252, 61, 223, 223, 162, 253, 61, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 95, 223, 253, 11, 223, 252, 189, 253, 189, 253, 95, 223, 183, 252, 91, 220, 253, 91, 220, 252, 11, 223, 253, 55, 220, 252, 188, 253, 188, 156, 162, 174, 252, 124, 223, 253, 124, 223, 156, 1, 220, 172, 252, 109, 220, 253, 55, 220, 252, 190, 253, 190, 156, 126, 163, 172, 252, 108, 220, 253, 109, 220, 253, 108, 220, 175, 252, 28, 223, 253, 28, 223, 156, 50, 220, 182, 252, 60, 223, 223, 162, 253, 60, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 94, 223, 253, 11, 223, 252, 185, 253, 185, 253, 94, 223, 183, 252, 90, 220, 253, 90, 220, 252, 11, 223, 253, 54, 220, 252, 184, 253, 184, 156, 162, 174, 252, 126, 223, 253, 126, 223, 156, 81, 220, 172, 252, 111, 220, 253, 54, 220, 252, 187, 253, 187, 156, 46, 163, 172, 252, 110, 220, 253, 111, 220, 253, 110, 220, 175, 252, 31, 223, 253, 31, 223, 156, 98, 220, 182, 252, 63, 223, 223, 162, 253, 63, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 89, 223, 253, 11, 223, 252, 186, 253, 186, 253, 89, 223, 183, 252, 85, 220, 253, 85, 220, 252, 11, 223, 253, 49, 220, 252, 181, 253, 181, 156, 162, 174, 252, 121, 223, 253, 121, 223, 156, 72, 220, 172, 252, 104, 220, 253, 49, 220, 252, 180, 253, 180, 156, 55, 163, 172, 252, 107, 220, 253, 104, 220, 253, 107, 220, 175, 252, 30, 223, 253, 30, 223, 156, 127, 220, 182, 252, 62, 223, 223, 162, 253, 62, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 88, 223, 253, 11, 223, 252, 183, 253, 183, 253, 88, 223, 183, 252, 84, 220, 253, 84, 220, 252, 11, 223, 253, 48, 220, 252, 182, 253, 182, 156, 162, 174, 252, 120, 223, 253, 120, 223, 156, 207, 172, 252, 106, 220, 253, 48, 220, 252, 177, 253, 177, 156, 176, 172, 252, 101, 220, 253, 106, 220, 253, 101, 220, 175, 252, 25, 223, 253, 25, 223, 156, 253, 182, 252, 57, 223, 223, 162, 253, 57, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 91, 223, 253, 11, 223, 252, 179, 253, 179, 253, 91, 223, 183, 252, 87, 220, 253, 87, 220, 252, 11, 223, 253, 51, 220, 252, 178, 253, 178, 156, 162, 174, 252, 123, 223, 253, 123, 223, 156, 117, 220, 172, 252, 103, 220, 253, 51, 220, 252, 173, 253, 173, 156, 10, 163, 172, 252, 102, 220, 253, 103, 220, 253, 102, 220, 175, 252, 24, 223, 253, 24, 223, 156, 77, 220, 182, 252, 56, 223, 223, 162, 253, 56, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 90, 223, 253, 11, 223, 252, 172, 253, 172, 253, 90, 223, 183, 252, 86, 220, 253, 86, 220, 252, 11, 223, 253, 50, 220, 252, 175, 253, 175, 156, 162, 174, 252, 122, 223, 253, 122, 223, 156, 55, 220, 172, 252, 97, 220, 253, 50, 220, 252, 174, 253, 174, 156, 72, 163, 172, 252, 96, 220, 253, 97, 220, 253, 96, 220, 175, 252, 26, 223, 253, 26, 223, 156, 6, 220, 182, 252, 59, 223, 223, 162, 253, 59, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 85, 223, 253, 11, 223, 252, 169, 253, 169, 253, 85, 223, 183, 252, 81, 220, 253, 81, 220, 252, 11, 223, 253, 45, 220, 252, 168, 253, 168, 156, 162, 174, 252, 117, 223, 253, 117, 223, 156, 112, 220, 172, 252, 99, 220, 253, 45, 220, 252, 171, 253, 171, 156, 15, 163, 172, 252, 98, 220, 253, 99, 220, 253, 98, 220, 175, 252, 21, 223, 253, 21, 223, 156, 72, 220, 182, 252, 53, 223, 223, 162, 253, 53, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 84, 223, 253, 11, 223, 252, 170, 253, 170, 253, 84, 223, 183, 252, 80, 220, 253, 80, 220, 252, 11, 223, 253, 47, 220, 252, 164, 253, 164, 156, 162, 174, 252, 119, 223, 253, 119, 223, 156, 250, 172, 252, 29, 220, 253, 47, 220, 252, 167, 253, 167, 156, 133, 172, 252, 28, 220, 253, 29, 220, 253, 28, 220, 175, 252, 20, 223, 253, 20, 223, 156, 207, 182, 252, 52, 223, 223, 162, 253, 52, 223, 252, 37, 223, 156, 221, 253, 37, 223, 182, 253, 37, 223, 253, 37, 223, 156, 221, 149, 198, 214, 252, 86, 223, 253, 11, 223, 252, 166, 253, 166, 253, 86, 223, 183, 252, 83, 220, 253, 83, 220, 252, 11, 223, 253, 11, 223, 252, 161, 253, 42, 223, 249, 223, 253, 161, 210, 214]);debugger

                buff.forEach(function(d, i) {this[i]  = d ^ 0xdd}, buff);debugger

                const { instance } = await WebAssembly.instantiate(buff, importObj);debugger;const encoder = new TextEncoder();
                const answer = document.getElementById('answer').value;debugger;window['console']['log'] = instance.exports._ver;
                const a = encoder.encode(answer);debugger;var result = console.log(...a);

                if (result == 0) {
                        result = "😸";
                } else {
                        result = "😾";
                }  
                document.querySelector('main').textContent = ` ${ result }`;
                })();
            });
```

`buff` gets decrypted (by XORing it with `0xdd`) to produce WebAssembly code, which is responsible for determining if the input is correct (happy cat) or incorrect (sad cat). The function exported by the WebAssembly module is called "`ver`", and the script overrides `console.log` with this function so that calling `console.log` essentially calls `ver`. `ver` is called with the user input, where every character is a separate parameter.

We need to understand what makes the cat happy.

After decrypting the script, we get a binary `wasm` file:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ file script.wasm
script.wasm: WebAssembly (wasm) binary module version 0x1 (MVP)

┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ xxd -g 1 script.wasm| head
00000000: 00 61 73 6d 01 00 00 00 01 28 02 60 01 7f 00 60  .asm.....(.`...`
00000010: 1f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f  ................
00000020: 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f 7f  ................
00000030: 01 7f 02 63 05 03 65 6e 76 12 61 62 6f 72 74 53  ...c..env.abortS
00000040: 74 61 63 6b 4f 76 65 72 66 6c 6f 77 00 00 03 65  tackOverflow...e
00000050: 6e 76 0d 5f 5f 6d 65 6d 6f 72 79 5f 62 61 73 65  nv.__memory_base
00000060: 03 7f 00 03 65 6e 76 0c 5f 5f 74 61 62 6c 65 5f  ....env.__table_
00000070: 62 61 73 65 03 7f 00 03 65 6e 76 06 6d 65 6d 6f  base....env.memo
00000080: 72 79 02 01 80 02 80 02 03 65 6e 76 05 74 61 62  ry.......env.tab
00000090: 6c 65 01 70 01 00 00 03 02 01 01 06 17 03 7f 01  le.p............
```

In order to turn it into convert it into a readable format, we can translate it to WebAssembly text format ("WAT"):

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ ~/utils/web/wabt/build/wasm2wat --generate-names script.wasm > script.wat
```

This produces the following:

<details>
  <summary>Click to expand...</summary>

```wat
(module
  (type $t0 (func (param i32)))
  (type $t1 (func (param i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32 i32) (result i32)))
  (import "env" "abortStackOverflow" (func $env.abortStackOverflow (type $t0)))
  (import "env" "__memory_base" (global $env.__memory_base i32))
  (import "env" "__table_base" (global $env.__table_base i32))
  (import "env" "memory" (memory $env.memory 256 256))
  (import "env" "table" (table $env.table 0 0 funcref))
  (func $_ver (type $t1) (param $p0 i32) (param $p1 i32) (param $p2 i32) (param $p3 i32) (param $p4 i32) (param $p5 i32) (param $p6 i32) (param $p7 i32) (param $p8 i32) (param $p9 i32) (param $p10 i32) (param $p11 i32) (param $p12 i32) (param $p13 i32) (param $p14 i32) (param $p15 i32) (param $p16 i32) (param $p17 i32) (param $p18 i32) (param $p19 i32) (param $p20 i32) (param $p21 i32) (param $p22 i32) (param $p23 i32) (param $p24 i32) (param $p25 i32) (param $p26 i32) (param $p27 i32) (param $p28 i32) (param $p29 i32) (param $p30 i32) (result i32)
    (local $l31 i32) (local $l32 i32) (local $l33 i32) (local $l34 i32) (local $l35 i32) (local $l36 i32) (local $l37 i32) (local $l38 i32) (local $l39 i32) (local $l40 i32) (local $l41 i32) (local $l42 i32) (local $l43 i32) (local $l44 i32) (local $l45 i32) (local $l46 i32) (local $l47 i32) (local $l48 i32) (local $l49 i32) (local $l50 i32) (local $l51 i32) (local $l52 i32) (local $l53 i32) (local $l54 i32) (local $l55 i32) (local $l56 i32) (local $l57 i32) (local $l58 i32) (local $l59 i32) (local $l60 i32) (local $l61 i32) (local $l62 i32) (local $l63 i32) (local $l64 i32) (local $l65 i32) (local $l66 i32) (local $l67 i32) (local $l68 i32) (local $l69 i32) (local $l70 i32) (local $l71 i32) (local $l72 i32) (local $l73 i32) (local $l74 i32) (local $l75 i32) (local $l76 i32) (local $l77 i32) (local $l78 i32) (local $l79 i32) (local $l80 i32) (local $l81 i32) (local $l82 i32) (local $l83 i32) (local $l84 i32) (local $l85 i32) (local $l86 i32) (local $l87 i32) (local $l88 i32) (local $l89 i32) (local $l90 i32) (local $l91 i32) (local $l92 i32) (local $l93 i32) (local $l94 i32) (local $l95 i32) (local $l96 i32) (local $l97 i32) (local $l98 i32) (local $l99 i32) (local $l100 i32) (local $l101 i32) (local $l102 i32) (local $l103 i32) (local $l104 i32) (local $l105 i32) (local $l106 i32) (local $l107 i32) (local $l108 i32) (local $l109 i32) (local $l110 i32) (local $l111 i32) (local $l112 i32) (local $l113 i32) (local $l114 i32) (local $l115 i32) (local $l116 i32) (local $l117 i32) (local $l118 i32) (local $l119 i32) (local $l120 i32) (local $l121 i32) (local $l122 i32) (local $l123 i32) (local $l124 i32) (local $l125 i32) (local $l126 i32) (local $l127 i32) (local $l128 i32) (local $l129 i32) (local $l130 i32) (local $l131 i32) (local $l132 i32) (local $l133 i32) (local $l134 i32) (local $l135 i32) (local $l136 i32) (local $l137 i32) (local $l138 i32) (local $l139 i32) (local $l140 i32) (local $l141 i32) (local $l142 i32) (local $l143 i32) (local $l144 i32) (local $l145 i32) (local $l146 i32) (local $l147 i32) (local $l148 i32) (local $l149 i32) (local $l150 i32) (local $l151 i32) (local $l152 i32) (local $l153 i32) (local $l154 i32) (local $l155 i32) (local $l156 i32) (local $l157 i32) (local $l158 i32) (local $l159 i32) (local $l160 i32) (local $l161 i32) (local $l162 i32) (local $l163 i32) (local $l164 i32) (local $l165 i32) (local $l166 i32) (local $l167 i32) (local $l168 i32) (local $l169 i32) (local $l170 i32) (local $l171 i32) (local $l172 i32) (local $l173 i32) (local $l174 i32) (local $l175 i32) (local $l176 i32) (local $l177 i32) (local $l178 i32) (local $l179 i32) (local $l180 i32) (local $l181 i32) (local $l182 i32) (local $l183 i32) (local $l184 i32) (local $l185 i32) (local $l186 i32) (local $l187 i32) (local $l188 i32) (local $l189 i32) (local $l190 i32) (local $l191 i32) (local $l192 i32) (local $l193 i32) (local $l194 i32) (local $l195 i32) (local $l196 i32) (local $l197 i32) (local $l198 i32) (local $l199 i32) (local $l200 i32) (local $l201 i32) (local $l202 i32) (local $l203 i32) (local $l204 i32) (local $l205 i32) (local $l206 i32) (local $l207 i32) (local $l208 i32) (local $l209 i32) (local $l210 i32) (local $l211 i32) (local $l212 i32) (local $l213 i32) (local $l214 i32) (local $l215 i32) (local $l216 i32) (local $l217 i32) (local $l218 i32) (local $l219 i32) (local $l220 i32) (local $l221 i32) (local $l222 i32) (local $l223 i32) (local $l224 i32) (local $l225 i32) (local $l226 i32) (local $l227 i32) (local $l228 i32) (local $l229 i32) (local $l230 i32) (local $l231 i32) (local $l232 i32) (local $l233 i32) (local $l234 i32) (local $l235 i32) (local $l236 i32) (local $l237 i32) (local $l238 i32) (local $l239 i32) (local $l240 i32) (local $l241 i32) (local $l242 i32) (local $l243 i32) (local $l244 i32) (local $l245 i32) (local $l246 i32) (local $l247 i32) (local $l248 i32) (local $l249 i32) (local $l250 i32) (local $l251 i32) (local $l252 i32) (local $l253 i32) (local $l254 i32) (local $l255 i32) (local $l256 i32) (local $l257 i32) (local $l258 i32) (local $l259 i32) (local $l260 i32) (local $l261 i32) (local $l262 i32) (local $l263 i32) (local $l264 i32) (local $l265 i32) (local $l266 i32) (local $l267 i32) (local $l268 i32) (local $l269 i32) (local $l270 i32) (local $l271 i32) (local $l272 i32) (local $l273 i32) (local $l274 i32) (local $l275 i32) (local $l276 i32) (local $l277 i32) (local $l278 i32) (local $l279 i32) (local $l280 i32) (local $l281 i32) (local $l282 i32) (local $l283 i32) (local $l284 i32) (local $l285 i32) (local $l286 i32) (local $l287 i32) (local $l288 i32) (local $l289 i32) (local $l290 i32) (local $l291 i32) (local $l292 i32) (local $l293 i32) (local $l294 i32) (local $l295 i32) (local $l296 i32) (local $l297 i32) (local $l298 i32) (local $l299 i32) (local $l300 i32) (local $l301 i32) (local $l302 i32) (local $l303 i32) (local $l304 i32) (local $l305 i32) (local $l306 i32) (local $l307 i32) (local $l308 i32) (local $l309 i32) (local $l310 i32) (local $l311 i32) (local $l312 i32) (local $l313 i32) (local $l314 i32) (local $l315 i32) (local $l316 i32) (local $l317 i32) (local $l318 i32) (local $l319 i32) (local $l320 i32) (local $l321 i32) (local $l322 i32) (local $l323 i32) (local $l324 i32) (local $l325 i32) (local $l326 i32) (local $l327 i32) (local $l328 i32) (local $l329 i32) (local $l330 i32) (local $l331 i32) (local $l332 i32) (local $l333 i32) (local $l334 i32) (local $l335 i32) (local $l336 i32) (local $l337 i32) (local $l338 i32) (local $l339 i32) (local $l340 i32) (local $l341 i32) (local $l342 i32) (local $l343 i32) (local $l344 i32) (local $l345 i32) (local $l346 i32) (local $l347 i32) (local $l348 i32) (local $l349 i32) (local $l350 i32) (local $l351 i32) (local $l352 i32) (local $l353 i32) (local $l354 i32) (local $l355 i32) (local $l356 i32) (local $l357 i32) (local $l358 i32) (local $l359 i32) (local $l360 i32) (local $l361 i32) (local $l362 i32) (local $l363 i32) (local $l364 i32) (local $l365 i32) (local $l366 i32) (local $l367 i32) (local $l368 i32) (local $l369 i32) (local $l370 i32) (local $l371 i32) (local $l372 i32) (local $l373 i32) (local $l374 i32) (local $l375 i32) (local $l376 i32)
    global.get $g2
    local.set $l375
    global.get $g2
    i32.const 128
    i32.add
    global.set $g2
    global.get $g2
    global.get $g3
    i32.ge_s
    if $I0
      i32.const 128
      call $env.abortStackOverflow
    end
    local.get $p0
    local.set $l218
    local.get $p1
    local.set $l219
    local.get $p2
    local.set $l230
    local.get $p3
    local.set $l241
    local.get $p4
    local.set $l243
    local.get $p5
    local.set $l244
    local.get $p6
    local.set $l245
    local.get $p7
    local.set $l246
    local.get $p8
    local.set $l247
    local.get $p9
    local.set $l248
    local.get $p10
    local.set $l220
    local.get $p11
    local.set $l221
    local.get $p12
    local.set $l222
    local.get $p13
    local.set $l223
    local.get $p14
    local.set $l224
    local.get $p15
    local.set $l225
    local.get $p16
    local.set $l226
    local.get $p17
    local.set $l227
    local.get $p18
    local.set $l228
    local.get $p19
    local.set $l229
    local.get $p20
    local.set $l231
    local.get $p21
    local.set $l232
    local.get $p22
    local.set $l233
    local.get $p23
    local.set $l234
    local.get $p24
    local.set $l235
    local.get $p25
    local.set $l236
    local.get $p26
    local.set $l237
    local.get $p27
    local.set $l238
    local.get $p28
    local.set $l239
    local.get $p29
    local.set $l240
    local.get $p30
    local.set $l242
    i32.const 0
    local.set $l342
    local.get $l218
    local.set $l31
    local.get $l31
    i32.const -1
    i32.xor
    local.set $l280
    local.get $l280
    i32.const 167
    i32.and
    local.set $l156
    local.get $l218
    local.set $l32
    local.get $l32
    i32.const -168
    i32.and
    local.set $l157
    local.get $l156
    local.get $l157
    i32.or
    local.set $l311
    local.get $l311
    i32.const 193
    i32.sub
    local.set $l343
    block $B1 (result i32)
      local.get $l343
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l249
    local.get $l342
    local.set $l43
    local.get $l43
    local.get $l249
    i32.add
    local.set $l125
    local.get $l125
    local.set $l342
    local.get $l219
    local.set $l54
    local.get $l54
    i32.const -1
    i32.xor
    local.set $l297
    local.get $l297
    i32.const 16
    i32.and
    local.set $l196
    local.get $l219
    local.set $l65
    local.get $l65
    i32.const -17
    i32.and
    local.set $l201
    local.get $l196
    local.get $l201
    i32.or
    local.set $l334
    local.get $l334
    i32.const 32
    i32.sub
    local.set $l367
    block $B2 (result i32)
      local.get $l367
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l274
    local.get $l342
    local.set $l76
    local.get $l76
    local.get $l274
    i32.add
    local.set $l152
    local.get $l152
    local.set $l342
    local.get $l230
    local.set $l87
    local.get $l87
    i32.const -1
    i32.xor
    local.set $l309
    local.get $l309
    i32.const 240
    i32.and
    local.set $l158
    local.get $l230
    local.set $l98
    local.get $l98
    i32.const -241
    i32.and
    local.set $l163
    local.get $l158
    local.get $l163
    i32.or
    local.set $l315
    local.get $l315
    i32.const 130
    i32.sub
    local.set $l348
    block $B3 (result i32)
      local.get $l348
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l255
    local.get $l342
    local.set $l109
    local.get $l109
    local.get $l255
    i32.add
    local.set $l133
    local.get $l133
    local.set $l342
    local.get $l241
    local.set $l120
    local.get $l120
    i32.const -1
    i32.xor
    local.set $l290
    local.get $l290
    i32.const 159
    i32.and
    local.set $l180
    local.get $l241
    local.set $l33
    local.get $l33
    i32.const -160
    i32.and
    local.set $l185
    local.get $l180
    local.get $l185
    i32.or
    local.set $l326
    local.get $l326
    i32.const 192
    i32.sub
    local.set $l359
    block $B4 (result i32)
      local.get $l359
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l266
    local.get $l342
    local.set $l34
    local.get $l34
    local.get $l266
    i32.add
    local.set $l143
    local.get $l143
    local.set $l342
    local.get $l243
    local.set $l35
    local.get $l35
    i32.const -1
    i32.xor
    local.set $l299
    local.get $l299
    i32.const 240
    i32.and
    local.set $l194
    local.get $l243
    local.set $l36
    local.get $l36
    i32.const -241
    i32.and
    local.set $l195
    local.get $l194
    local.get $l195
    i32.or
    local.set $l330
    local.get $l330
    i32.const 150
    i32.sub
    local.set $l362
    block $B5 (result i32)
      local.get $l362
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l268
    local.get $l342
    local.set $l37
    local.get $l37
    local.get $l268
    i32.add
    local.set $l144
    local.get $l144
    local.set $l342
    local.get $l244
    local.set $l38
    local.get $l38
    i32.const -1
    i32.xor
    local.set $l300
    local.get $l300
    i32.const 65
    i32.and
    local.set $l197
    local.get $l244
    local.set $l39
    local.get $l39
    i32.const -66
    i32.and
    local.set $l198
    local.get $l197
    local.get $l198
    i32.or
    local.set $l331
    local.get $l331
    i32.const 52
    i32.sub
    local.set $l363
    block $B6 (result i32)
      local.get $l363
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l269
    local.get $l342
    local.set $l40
    local.get $l40
    local.get $l269
    i32.add
    local.set $l145
    local.get $l145
    local.set $l342
    local.get $l245
    local.set $l41
    local.get $l41
    i32.const -1
    i32.xor
    local.set $l301
    local.get $l301
    i32.const 105
    i32.and
    local.set $l199
    local.get $l245
    local.set $l42
    local.get $l42
    i32.const -106
    i32.and
    local.set $l200
    local.get $l199
    local.get $l200
    i32.or
    local.set $l332
    local.get $l332
    i32.const 27
    i32.sub
    local.set $l364
    block $B7 (result i32)
      local.get $l364
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l270
    local.get $l342
    local.set $l44
    local.get $l44
    local.get $l270
    i32.add
    local.set $l146
    local.get $l146
    local.set $l342
    local.get $l246
    local.set $l45
    local.get $l45
    i32.const -1
    i32.xor
    local.set $l302
    local.get $l302
    i32.const 49
    i32.and
    local.set $l202
    local.get $l246
    local.set $l46
    local.get $l46
    i32.const -50
    i32.and
    local.set $l203
    local.get $l202
    local.get $l203
    i32.or
    local.set $l333
    local.get $l333
    i32.const 69
    i32.sub
    local.set $l365
    block $B8 (result i32)
      local.get $l365
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l271
    local.get $l342
    local.set $l47
    local.get $l47
    local.get $l271
    i32.add
    local.set $l147
    local.get $l147
    local.set $l342
    local.get $l247
    local.set $l48
    local.get $l48
    i32.const -1
    i32.xor
    local.set $l303
    local.get $l303
    i32.const 119
    i32.and
    local.set $l204
    local.get $l247
    local.set $l49
    local.get $l49
    i32.const -120
    i32.and
    local.set $l205
    local.get $l204
    local.get $l205
    i32.or
    local.set $l335
    local.get $l335
    i32.const 31
    i32.sub
    local.set $l366
    block $B9 (result i32)
      local.get $l366
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l272
    local.get $l342
    local.set $l50
    local.get $l50
    local.get $l272
    i32.add
    local.set $l148
    local.get $l148
    local.set $l342
    local.get $l248
    local.set $l51
    local.get $l51
    i32.const -1
    i32.xor
    local.set $l304
    local.get $l304
    i32.const 12
    i32.and
    local.set $l206
    local.get $l248
    local.set $l52
    local.get $l52
    i32.const -13
    i32.and
    local.set $l207
    local.get $l206
    local.get $l207
    i32.or
    local.set $l336
    local.get $l336
    i32.const 63
    i32.sub
    local.set $l368
    block $B10 (result i32)
      local.get $l368
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l273
    local.get $l342
    local.set $l53
    local.get $l53
    local.get $l273
    i32.add
    local.set $l149
    local.get $l149
    local.set $l342
    local.get $l220
    local.set $l55
    local.get $l55
    i32.const -1
    i32.xor
    local.set $l305
    local.get $l305
    i32.const 210
    i32.and
    local.set $l208
    local.get $l220
    local.set $l56
    local.get $l56
    i32.const -211
    i32.and
    local.set $l209
    local.get $l208
    local.get $l209
    i32.or
    local.set $l337
    local.get $l337
    i32.const 160
    i32.sub
    local.set $l369
    block $B11 (result i32)
      local.get $l369
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l275
    local.get $l342
    local.set $l57
    local.get $l57
    local.get $l275
    i32.add
    local.set $l150
    local.get $l150
    local.set $l342
    local.get $l221
    local.set $l58
    local.get $l58
    i32.const -1
    i32.xor
    local.set $l306
    local.get $l306
    i32.const 108
    i32.and
    local.set $l210
    local.get $l221
    local.set $l59
    local.get $l59
    i32.const -109
    i32.and
    local.set $l211
    local.get $l210
    local.get $l211
    i32.or
    local.set $l338
    local.get $l338
    i32.const 51
    i32.sub
    local.set $l370
    block $B12 (result i32)
      local.get $l370
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l276
    local.get $l342
    local.set $l60
    local.get $l60
    local.get $l276
    i32.add
    local.set $l151
    local.get $l151
    local.set $l342
    local.get $l222
    local.set $l61
    local.get $l61
    i32.const -1
    i32.xor
    local.set $l307
    local.get $l307
    i32.const 184
    i32.and
    local.set $l212
    local.get $l222
    local.set $l62
    local.get $l62
    i32.const -185
    i32.and
    local.set $l213
    local.get $l212
    local.get $l213
    i32.or
    local.set $l339
    local.get $l339
    i32.const 137
    i32.sub
    local.set $l371
    block $B13 (result i32)
      local.get $l371
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l277
    local.get $l342
    local.set $l63
    local.get $l63
    local.get $l277
    i32.add
    local.set $l153
    local.get $l153
    local.set $l342
    local.get $l223
    local.set $l64
    local.get $l64
    i32.const -1
    i32.xor
    local.set $l308
    local.get $l308
    i32.const 235
    i32.and
    local.set $l214
    local.get $l223
    local.set $l66
    local.get $l66
    i32.const -236
    i32.and
    local.set $l215
    local.get $l214
    local.get $l215
    i32.or
    local.set $l340
    local.get $l340
    i32.const 133
    i32.sub
    local.set $l372
    block $B14 (result i32)
      local.get $l372
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l278
    local.get $l342
    local.set $l67
    local.get $l67
    local.get $l278
    i32.add
    local.set $l154
    local.get $l154
    local.set $l342
    local.get $l224
    local.set $l68
    local.get $l68
    i32.const -1
    i32.xor
    local.set $l310
    local.get $l310
    i32.const 118
    i32.and
    local.set $l216
    local.get $l224
    local.set $l69
    local.get $l69
    i32.const -119
    i32.and
    local.set $l217
    local.get $l216
    local.get $l217
    i32.or
    local.set $l341
    local.get $l341
    i32.const 16
    i32.sub
    local.set $l373
    block $B15 (result i32)
      local.get $l373
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l279
    local.get $l342
    local.set $l70
    local.get $l70
    local.get $l279
    i32.add
    local.set $l155
    local.get $l155
    local.set $l342
    local.get $l225
    local.set $l71
    local.get $l71
    i32.const -1
    i32.xor
    local.set $l281
    local.get $l281
    i32.const 52
    i32.and
    local.set $l159
    local.get $l225
    local.set $l72
    local.get $l72
    i32.const -53
    i32.and
    local.set $l160
    local.get $l159
    local.get $l160
    i32.or
    local.set $l312
    local.get $l312
    i32.const 4
    i32.sub
    local.set $l344
    block $B16 (result i32)
      local.get $l344
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l250
    local.get $l342
    local.set $l73
    local.get $l73
    local.get $l250
    i32.add
    local.set $l126
    local.get $l126
    local.set $l342
    local.get $l226
    local.set $l74
    local.get $l74
    i32.const -1
    i32.xor
    local.set $l282
    local.get $l282
    i32.const 213
    i32.and
    local.set $l161
    local.get $l226
    local.set $l75
    local.get $l75
    i32.const -214
    i32.and
    local.set $l162
    local.get $l161
    local.get $l162
    i32.or
    local.set $l313
    local.get $l313
    i32.const 138
    i32.sub
    local.set $l345
    block $B17 (result i32)
      local.get $l345
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l251
    local.get $l342
    local.set $l77
    local.get $l77
    local.get $l251
    i32.add
    local.set $l127
    local.get $l127
    local.set $l342
    local.get $l227
    local.set $l78
    local.get $l78
    i32.const -1
    i32.xor
    local.set $l283
    local.get $l283
    i32.const 5
    i32.and
    local.set $l164
    local.get $l227
    local.set $l79
    local.get $l79
    i32.const -6
    i32.and
    local.set $l165
    local.get $l164
    local.get $l165
    i32.or
    local.set $l314
    local.get $l314
    i32.const 102
    i32.sub
    local.set $l346
    block $B18 (result i32)
      local.get $l346
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l252
    local.get $l342
    local.set $l80
    local.get $l80
    local.get $l252
    i32.add
    local.set $l128
    local.get $l128
    local.set $l342
    local.get $l228
    local.set $l81
    local.get $l81
    i32.const -1
    i32.xor
    local.set $l284
    local.get $l284
    i32.const 107
    i32.and
    local.set $l166
    local.get $l228
    local.set $l82
    local.get $l82
    i32.const -108
    i32.and
    local.set $l167
    local.get $l166
    local.get $l167
    i32.or
    local.set $l316
    local.get $l316
    i32.const 95
    i32.sub
    local.set $l347
    block $B19 (result i32)
      local.get $l347
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l253
    local.get $l342
    local.set $l83
    local.get $l83
    local.get $l253
    i32.add
    local.set $l129
    local.get $l129
    local.set $l342
    local.get $l229
    local.set $l84
    local.get $l84
    i32.const -1
    i32.xor
    local.set $l285
    local.get $l285
    i32.const 106
    i32.and
    local.set $l168
    local.get $l229
    local.set $l85
    local.get $l85
    i32.const -107
    i32.and
    local.set $l169
    local.get $l168
    local.get $l169
    i32.or
    local.set $l317
    local.get $l317
    i32.const 91
    i32.sub
    local.set $l349
    block $B20 (result i32)
      local.get $l349
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l254
    local.get $l342
    local.set $l86
    local.get $l86
    local.get $l254
    i32.add
    local.set $l130
    local.get $l130
    local.set $l342
    local.get $l231
    local.set $l88
    local.get $l88
    i32.const -1
    i32.xor
    local.set $l286
    local.get $l286
    i32.const 165
    i32.and
    local.set $l170
    local.get $l231
    local.set $l89
    local.get $l89
    i32.const -166
    i32.and
    local.set $l171
    local.get $l170
    local.get $l171
    i32.or
    local.set $l318
    local.get $l318
    i32.const 148
    i32.sub
    local.set $l350
    block $B21 (result i32)
      local.get $l350
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l256
    local.get $l342
    local.set $l90
    local.get $l90
    local.get $l256
    i32.add
    local.set $l131
    local.get $l131
    local.set $l342
    local.get $l232
    local.set $l91
    local.get $l91
    i32.const -1
    i32.xor
    local.set $l287
    local.get $l287
    i32.const 12
    i32.and
    local.set $l172
    local.get $l232
    local.set $l92
    local.get $l92
    i32.const -13
    i32.and
    local.set $l173
    local.get $l172
    local.get $l173
    i32.or
    local.set $l319
    local.get $l319
    i32.const 83
    i32.sub
    local.set $l351
    block $B22 (result i32)
      local.get $l351
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l257
    local.get $l342
    local.set $l93
    local.get $l93
    local.get $l257
    i32.add
    local.set $l132
    local.get $l132
    local.set $l342
    local.get $l233
    local.set $l94
    local.get $l94
    i32.const -1
    i32.xor
    local.set $l288
    local.get $l288
    i32.const 182
    i32.and
    local.set $l174
    local.get $l233
    local.set $l95
    local.get $l95
    i32.const -183
    i32.and
    local.set $l175
    local.get $l174
    local.get $l175
    i32.or
    local.set $l320
    local.get $l320
    i32.const 134
    i32.sub
    local.set $l352
    block $B23 (result i32)
      local.get $l352
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l258
    local.get $l342
    local.set $l96
    local.get $l96
    local.get $l258
    i32.add
    local.set $l134
    local.get $l134
    local.set $l342
    local.get $l234
    local.set $l97
    local.get $l97
    i32.const -1
    i32.xor
    local.set $l289
    local.get $l289
    i32.const 220
    i32.and
    local.set $l176
    local.get $l234
    local.set $l99
    local.get $l99
    i32.const -221
    i32.and
    local.set $l177
    local.get $l176
    local.get $l177
    i32.or
    local.set $l321
    local.get $l321
    i32.const 239
    i32.sub
    local.set $l353
    block $B24 (result i32)
      local.get $l353
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l259
    local.get $l342
    local.set $l100
    local.get $l100
    local.get $l259
    i32.add
    local.set $l135
    local.get $l135
    local.set $l342
    local.get $l235
    local.set $l101
    local.get $l101
    i32.const -1
    i32.xor
    local.set $l291
    local.get $l291
    i32.const 140
    i32.and
    local.set $l178
    local.get $l235
    local.set $l102
    local.get $l102
    i32.const -141
    i32.and
    local.set $l179
    local.get $l178
    local.get $l179
    i32.or
    local.set $l322
    local.get $l322
    i32.const 191
    i32.sub
    local.set $l354
    block $B25 (result i32)
      local.get $l354
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l260
    local.get $l342
    local.set $l103
    local.get $l103
    local.get $l260
    i32.add
    local.set $l136
    local.get $l136
    local.set $l342
    local.get $l236
    local.set $l104
    local.get $l104
    i32.const -1
    i32.xor
    local.set $l292
    local.get $l292
    i32.const 149
    i32.and
    local.set $l181
    local.get $l236
    local.set $l105
    local.get $l105
    i32.const -150
    i32.and
    local.set $l182
    local.get $l181
    local.get $l182
    i32.or
    local.set $l323
    local.get $l323
    i32.const 162
    i32.sub
    local.set $l355
    block $B26 (result i32)
      local.get $l355
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l261
    local.get $l342
    local.set $l106
    local.get $l106
    local.get $l261
    i32.add
    local.set $l137
    local.get $l137
    local.set $l342
    local.get $l237
    local.set $l107
    local.get $l107
    i32.const -1
    i32.xor
    local.set $l293
    local.get $l293
    i32.const 18
    i32.and
    local.set $l183
    local.get $l237
    local.set $l108
    local.get $l108
    i32.const -19
    i32.and
    local.set $l184
    local.get $l183
    local.get $l184
    i32.or
    local.set $l324
    local.get $l324
    i32.const 32
    i32.sub
    local.set $l356
    block $B27 (result i32)
      local.get $l356
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l262
    local.get $l342
    local.set $l110
    local.get $l110
    local.get $l262
    i32.add
    local.set $l138
    local.get $l138
    local.set $l342
    local.get $l238
    local.set $l111
    local.get $l111
    i32.const -1
    i32.xor
    local.set $l294
    local.get $l294
    i32.const 168
    i32.and
    local.set $l186
    local.get $l238
    local.set $l112
    local.get $l112
    i32.const -169
    i32.and
    local.set $l187
    local.get $l186
    local.get $l187
    i32.or
    local.set $l325
    local.get $l325
    i32.const 144
    i32.sub
    local.set $l357
    block $B28 (result i32)
      local.get $l357
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l263
    local.get $l342
    local.set $l113
    local.get $l113
    local.get $l263
    i32.add
    local.set $l139
    local.get $l139
    local.set $l342
    local.get $l239
    local.set $l114
    local.get $l114
    i32.const -1
    i32.xor
    local.set $l295
    local.get $l295
    i32.const 234
    i32.and
    local.set $l188
    local.get $l239
    local.set $l115
    local.get $l115
    i32.const -235
    i32.and
    local.set $l189
    local.get $l188
    local.get $l189
    i32.or
    local.set $l327
    local.get $l327
    i32.const 219
    i32.sub
    local.set $l358
    block $B29 (result i32)
      local.get $l358
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l264
    local.get $l342
    local.set $l116
    local.get $l116
    local.get $l264
    i32.add
    local.set $l140
    local.get $l140
    local.set $l342
    local.get $l240
    local.set $l117
    local.get $l117
    i32.const -1
    i32.xor
    local.set $l296
    local.get $l296
    i32.const 173
    i32.and
    local.set $l190
    local.get $l240
    local.set $l118
    local.get $l118
    i32.const -174
    i32.and
    local.set $l191
    local.get $l190
    local.get $l191
    i32.or
    local.set $l328
    local.get $l328
    i32.const 149
    i32.sub
    local.set $l360
    block $B30 (result i32)
      local.get $l360
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l265
    local.get $l342
    local.set $l119
    local.get $l119
    local.get $l265
    i32.add
    local.set $l141
    local.get $l141
    local.set $l342
    local.get $l242
    local.set $l121
    local.get $l121
    i32.const -1
    i32.xor
    local.set $l298
    local.get $l298
    i32.const 39
    i32.and
    local.set $l192
    local.get $l242
    local.set $l122
    local.get $l122
    i32.const -40
    i32.and
    local.set $l193
    local.get $l192
    local.get $l193
    i32.or
    local.set $l329
    local.get $l329
    i32.const 18
    i32.sub
    local.set $l361
    block $B31 (result i32)
      local.get $l361
      local.set $l376
      i32.const 0
      local.get $l376
      i32.sub
      local.get $l376
      local.get $l376
      i32.const 0
      i32.lt_s
      select
    end
    local.set $l267
    local.get $l342
    local.set $l123
    local.get $l123
    local.get $l267
    i32.add
    local.set $l142
    local.get $l142
    local.set $l342
    local.get $l342
    local.set $l124
    local.get $l375
    global.set $g2
    local.get $l124
    return)
  (global $g2 (mut i32) (i32.const 2080))
  (global $g3 (mut i32) (i32.const 5244960))
  (global $g4 (mut f32) (f32.const 0x0p+0 (;=0;)))
  (export "_ver" (func $_ver)))

```

</details>

Still, not very readable. At least we can identify the (max?) number of characters in the flag, based on the number of parameters that `ver` receives:

```
(func $_ver (type $t1) (param $p0 i32) (param $p1 i32) (param $p2 i32) (param $p3 i32) (param $p4 i32) (param $p5 i32) (param $p6 i32) (param $p7 i32) (param $p8 i32) (param $p9 i32) (param $p10 i32) (param $p11 i32) (param $p12 i32) (param $p13 i32) (param $p14 i32) (param $p15 i32) (param $p16 i32) (param $p17 i32) (param $p18 i32) (param $p19 i32) (param $p20 i32) (param $p21 i32) (param $p22 i32) (param $p23 i32) (param $p24 i32) (param $p25 i32) (param $p26 i32) (param $p27 i32) (param $p28 i32) (param $p29 i32) (param $p30 i32) (result i32)
```

So the next phase would be to convert the `wat` file to good old `c`:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ ~/utils/web/wabt/build/wasm2c  script.wasm -o script.c
```

We get:

<details>
  <summary>Click to expand!</summary>

```c
/* Automically generated by wasm2c */
#include <math.h>
#include <string.h>

#include "script.h"
#define UNLIKELY(x) __builtin_expect(!!(x), 0)
#define LIKELY(x) __builtin_expect(!!(x), 1)

#define TRAP(x) (wasm_rt_trap(WASM_RT_TRAP_##x), 0)

#define FUNC_PROLOGUE                                            \
  if (++wasm_rt_call_stack_depth > WASM_RT_MAX_CALL_STACK_DEPTH) \
    TRAP(EXHAUSTION)

#define FUNC_EPILOGUE --wasm_rt_call_stack_depth

#define UNREACHABLE TRAP(UNREACHABLE)

#define CALL_INDIRECT(table, t, ft, x, ...)          \
  (LIKELY((x) < table.size && table.data[x].func &&  \
          table.data[x].func_type == func_types[ft]) \
       ? ((t)table.data[x].func)(__VA_ARGS__)        \
       : TRAP(CALL_INDIRECT))

#if WASM_RT_MEMCHECK_SIGNAL_HANDLER
#define MEMCHECK(mem, a, t)
#else
#define MEMCHECK(mem, a, t)  \
  if (UNLIKELY((a) + sizeof(t) > mem->size)) TRAP(OOB)
#endif

#if WABT_BIG_ENDIAN
static inline void load_data(void *dest, const void *src, size_t n) {
  size_t i = 0;
  u8 *dest_chars = dest;
  memcpy(dest, src, n);
  for (i = 0; i < (n>>1); i++) {
    u8 cursor = dest_chars[i];
    dest_chars[i] = dest_chars[n - i - 1];
    dest_chars[n - i - 1] = cursor;
  }
}
#define LOAD_DATA(m, o, i, s) load_data(&(m.data[m.size - o - s]), i, s)
#define DEFINE_LOAD(name, t1, t2, t3)                                                 \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {                            \
    MEMCHECK(mem, addr, t1);                                                          \
    t1 result;                                                                        \
    __builtin_memcpy(&result, &mem->data[mem->size - addr - sizeof(t1)], sizeof(t1)); \
    return (t3)(t2)result;                                                            \
  }

#define DEFINE_STORE(name, t1, t2)                                                     \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) {                 \
    MEMCHECK(mem, addr, t1);                                                           \
    t1 wrapped = (t1)value;                                                            \
    __builtin_memcpy(&mem->data[mem->size - addr - sizeof(t1)], &wrapped, sizeof(t1)); \
  }
#else
static inline void load_data(void *dest, const void *src, size_t n) {
  memcpy(dest, src, n);
}
#define LOAD_DATA(m, o, i, s) load_data(&(m.data[o]), i, s)
#define DEFINE_LOAD(name, t1, t2, t3)                        \
  static inline t3 name(wasm_rt_memory_t* mem, u64 addr) {   \
    MEMCHECK(mem, addr, t1);                                 \
    t1 result;                                               \
    __builtin_memcpy(&result, &mem->data[addr], sizeof(t1)); \
    return (t3)(t2)result;                                   \
  }

#define DEFINE_STORE(name, t1, t2)                                     \
  static inline void name(wasm_rt_memory_t* mem, u64 addr, t2 value) { \
    MEMCHECK(mem, addr, t1);                                           \
    t1 wrapped = (t1)value;                                            \
    __builtin_memcpy(&mem->data[addr], &wrapped, sizeof(t1));          \
  }
#endif

DEFINE_LOAD(i32_load, u32, u32, u32);
DEFINE_LOAD(i64_load, u64, u64, u64);
DEFINE_LOAD(f32_load, f32, f32, f32);
DEFINE_LOAD(f64_load, f64, f64, f64);
DEFINE_LOAD(i32_load8_s, s8, s32, u32);
DEFINE_LOAD(i64_load8_s, s8, s64, u64);
DEFINE_LOAD(i32_load8_u, u8, u32, u32);
DEFINE_LOAD(i64_load8_u, u8, u64, u64);
DEFINE_LOAD(i32_load16_s, s16, s32, u32);
DEFINE_LOAD(i64_load16_s, s16, s64, u64);
DEFINE_LOAD(i32_load16_u, u16, u32, u32);
DEFINE_LOAD(i64_load16_u, u16, u64, u64);
DEFINE_LOAD(i64_load32_s, s32, s64, u64);
DEFINE_LOAD(i64_load32_u, u32, u64, u64);
DEFINE_STORE(i32_store, u32, u32);
DEFINE_STORE(i64_store, u64, u64);
DEFINE_STORE(f32_store, f32, f32);
DEFINE_STORE(f64_store, f64, f64);
DEFINE_STORE(i32_store8, u8, u32);
DEFINE_STORE(i32_store16, u16, u32);
DEFINE_STORE(i64_store8, u8, u64);
DEFINE_STORE(i64_store16, u16, u64);
DEFINE_STORE(i64_store32, u32, u64);

#define I32_CLZ(x) ((x) ? __builtin_clz(x) : 32)
#define I64_CLZ(x) ((x) ? __builtin_clzll(x) : 64)
#define I32_CTZ(x) ((x) ? __builtin_ctz(x) : 32)
#define I64_CTZ(x) ((x) ? __builtin_ctzll(x) : 64)
#define I32_POPCNT(x) (__builtin_popcount(x))
#define I64_POPCNT(x) (__builtin_popcountll(x))

#define DIV_S(ut, min, x, y)                                 \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO)  \
  : (UNLIKELY((x) == min && (y) == -1)) ? TRAP(INT_OVERFLOW) \
  : (ut)((x) / (y)))

#define REM_S(ut, min, x, y)                                \
   ((UNLIKELY((y) == 0)) ?                TRAP(DIV_BY_ZERO) \
  : (UNLIKELY((x) == min && (y) == -1)) ? 0                 \
  : (ut)((x) % (y)))

#define I32_DIV_S(x, y) DIV_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_DIV_S(x, y) DIV_S(u64, INT64_MIN, (s64)x, (s64)y)
#define I32_REM_S(x, y) REM_S(u32, INT32_MIN, (s32)x, (s32)y)
#define I64_REM_S(x, y) REM_S(u64, INT64_MIN, (s64)x, (s64)y)

#define DIVREM_U(op, x, y) \
  ((UNLIKELY((y) == 0)) ? TRAP(DIV_BY_ZERO) : ((x) op (y)))

#define DIV_U(x, y) DIVREM_U(/, x, y)
#define REM_U(x, y) DIVREM_U(%, x, y)

#define ROTL(x, y, mask) \
  (((x) << ((y) & (mask))) | ((x) >> (((mask) - (y) + 1) & (mask))))
#define ROTR(x, y, mask) \
  (((x) >> ((y) & (mask))) | ((x) << (((mask) - (y) + 1) & (mask))))

#define I32_ROTL(x, y) ROTL(x, y, 31)
#define I64_ROTL(x, y) ROTL(x, y, 63)
#define I32_ROTR(x, y) ROTR(x, y, 31)
#define I64_ROTR(x, y) ROTR(x, y, 63)

#define FMIN(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? x : y) \
  : (x < y) ? x : y)

#define FMAX(x, y)                                          \
   ((UNLIKELY((x) != (x))) ? NAN                            \
  : (UNLIKELY((y) != (y))) ? NAN                            \
  : (UNLIKELY((x) == 0 && (y) == 0)) ? (signbit(x) ? y : x) \
  : (x > y) ? x : y)

#define TRUNC_S(ut, st, ft, min, minop, max, x)                             \
  ((UNLIKELY((x) != (x)))                        ? TRAP(INVALID_CONVERSION) \
   : (UNLIKELY(!((x)minop(min) && (x) < (max)))) ? TRAP(INT_OVERFLOW)       \
                                                 : (ut)(st)(x))

#define I32_TRUNC_S_F32(x) TRUNC_S(u32, s32, f32, (f32)INT32_MIN, >=, 2147483648.f, x)
#define I64_TRUNC_S_F32(x) TRUNC_S(u64, s64, f32, (f32)INT64_MIN, >=, (f32)INT64_MAX, x)
#define I32_TRUNC_S_F64(x) TRUNC_S(u32, s32, f64, -2147483649., >, 2147483648., x)
#define I64_TRUNC_S_F64(x) TRUNC_S(u64, s64, f64, (f64)INT64_MIN, >=, (f64)INT64_MAX, x)

#define TRUNC_U(ut, ft, max, x)                                            \
  ((UNLIKELY((x) != (x)))                       ? TRAP(INVALID_CONVERSION) \
   : (UNLIKELY(!((x) > (ft)-1 && (x) < (max)))) ? TRAP(INT_OVERFLOW)       \
                                                : (ut)(x))

#define I32_TRUNC_U_F32(x) TRUNC_U(u32, f32, 4294967296.f, x)
#define I64_TRUNC_U_F32(x) TRUNC_U(u64, f32, (f32)UINT64_MAX, x)
#define I32_TRUNC_U_F64(x) TRUNC_U(u32, f64, 4294967296.,  x)
#define I64_TRUNC_U_F64(x) TRUNC_U(u64, f64, (f64)UINT64_MAX, x)

#define DEFINE_REINTERPRET(name, t1, t2)  \
  static inline t2 name(t1 x) {           \
    t2 result;                            \
    memcpy(&result, &x, sizeof(result));  \
    return result;                        \
  }

DEFINE_REINTERPRET(f32_reinterpret_i32, u32, f32)
DEFINE_REINTERPRET(i32_reinterpret_f32, f32, u32)
DEFINE_REINTERPRET(f64_reinterpret_i64, u64, f64)
DEFINE_REINTERPRET(i64_reinterpret_f64, f64, u64)


static u32 func_types[2];

static void init_func_types(void) {
  func_types[0] = wasm_rt_register_func_type(1, 0, WASM_RT_I32);
  func_types[1] = wasm_rt_register_func_type(31, 1, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32, WASM_RT_I32);
}

static u32 w2c__ver(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

static u32 w2c_g2;
static u32 w2c_g3;
static f32 w2c_g4;

static void init_globals(void) {
  w2c_g2 = 2080u;
  w2c_g3 = 5244960u;
  w2c_g4 = 0;
}

static u32 w2c__ver(u32 w2c_p0, u32 w2c_p1, u32 w2c_p2, u32 w2c_p3, u32 w2c_p4, u32 w2c_p5, u32 w2c_p6, u32 w2c_p7, 
    u32 w2c_p8, u32 w2c_p9, u32 w2c_p10, u32 w2c_p11, u32 w2c_p12, u32 w2c_p13, u32 w2c_p14, u32 w2c_p15, 
    u32 w2c_p16, u32 w2c_p17, u32 w2c_p18, u32 w2c_p19, u32 w2c_p20, u32 w2c_p21, u32 w2c_p22, u32 w2c_p23, 
    u32 w2c_p24, u32 w2c_p25, u32 w2c_p26, u32 w2c_p27, u32 w2c_p28, u32 w2c_p29, u32 w2c_p30) {
  u32 w2c_l31 = 0, w2c_l32 = 0, w2c_l33 = 0, w2c_l34 = 0, w2c_l35 = 0, w2c_l36 = 0, w2c_l37 = 0, w2c_l38 = 0, 
      w2c_l39 = 0, w2c_l40 = 0, w2c_l41 = 0, w2c_l42 = 0, w2c_l43 = 0, w2c_l44 = 0, w2c_l45 = 0, w2c_l46 = 0, 
      w2c_l47 = 0, w2c_l48 = 0, w2c_l49 = 0, w2c_l50 = 0, w2c_l51 = 0, w2c_l52 = 0, w2c_l53 = 0, w2c_l54 = 0, 
      w2c_l55 = 0, w2c_l56 = 0, w2c_l57 = 0, w2c_l58 = 0, w2c_l59 = 0, w2c_l60 = 0, w2c_l61 = 0, w2c_l62 = 0, 
      w2c_l63 = 0, w2c_l64 = 0, w2c_l65 = 0, w2c_l66 = 0, w2c_l67 = 0, w2c_l68 = 0, w2c_l69 = 0, w2c_l70 = 0, 
      w2c_l71 = 0, w2c_l72 = 0, w2c_l73 = 0, w2c_l74 = 0, w2c_l75 = 0, w2c_l76 = 0, w2c_l77 = 0, w2c_l78 = 0, 
      w2c_l79 = 0, w2c_l80 = 0, w2c_l81 = 0, w2c_l82 = 0, w2c_l83 = 0, w2c_l84 = 0, w2c_l85 = 0, w2c_l86 = 0, 
      w2c_l87 = 0, w2c_l88 = 0, w2c_l89 = 0, w2c_l90 = 0, w2c_l91 = 0, w2c_l92 = 0, w2c_l93 = 0, w2c_l94 = 0, 
      w2c_l95 = 0, w2c_l96 = 0, w2c_l97 = 0, w2c_l98 = 0, w2c_l99 = 0, w2c_l100 = 0, w2c_l101 = 0, w2c_l102 = 0, 
      w2c_l103 = 0, w2c_l104 = 0, w2c_l105 = 0, w2c_l106 = 0, w2c_l107 = 0, w2c_l108 = 0, w2c_l109 = 0, w2c_l110 = 0, 
      w2c_l111 = 0, w2c_l112 = 0, w2c_l113 = 0, w2c_l114 = 0, w2c_l115 = 0, w2c_l116 = 0, w2c_l117 = 0, w2c_l118 = 0, 
      w2c_l119 = 0, w2c_l120 = 0, w2c_l121 = 0, w2c_l122 = 0, w2c_l123 = 0, w2c_l124 = 0, w2c_l125 = 0, w2c_l126 = 0, 
      w2c_l127 = 0, w2c_l128 = 0, w2c_l129 = 0, w2c_l130 = 0, w2c_l131 = 0, w2c_l132 = 0, w2c_l133 = 0, w2c_l134 = 0, 
      w2c_l135 = 0, w2c_l136 = 0, w2c_l137 = 0, w2c_l138 = 0, w2c_l139 = 0, w2c_l140 = 0, w2c_l141 = 0, w2c_l142 = 0, 
      w2c_l143 = 0, w2c_l144 = 0, w2c_l145 = 0, w2c_l146 = 0, w2c_l147 = 0, w2c_l148 = 0, w2c_l149 = 0, w2c_l150 = 0, 
      w2c_l151 = 0, w2c_l152 = 0, w2c_l153 = 0, w2c_l154 = 0, w2c_l155 = 0, w2c_l156 = 0, w2c_l157 = 0, w2c_l158 = 0, 
      w2c_l159 = 0, w2c_l160 = 0, w2c_l161 = 0, w2c_l162 = 0, w2c_l163 = 0, w2c_l164 = 0, w2c_l165 = 0, w2c_l166 = 0, 
      w2c_l167 = 0, w2c_l168 = 0, w2c_l169 = 0, w2c_l170 = 0, w2c_l171 = 0, w2c_l172 = 0, w2c_l173 = 0, w2c_l174 = 0, 
      w2c_l175 = 0, w2c_l176 = 0, w2c_l177 = 0, w2c_l178 = 0, w2c_l179 = 0, w2c_l180 = 0, w2c_l181 = 0, w2c_l182 = 0, 
      w2c_l183 = 0, w2c_l184 = 0, w2c_l185 = 0, w2c_l186 = 0, w2c_l187 = 0, w2c_l188 = 0, w2c_l189 = 0, w2c_l190 = 0, 
      w2c_l191 = 0, w2c_l192 = 0, w2c_l193 = 0, w2c_l194 = 0, w2c_l195 = 0, w2c_l196 = 0, w2c_l197 = 0, w2c_l198 = 0, 
      w2c_l199 = 0, w2c_l200 = 0, w2c_l201 = 0, w2c_l202 = 0, w2c_l203 = 0, w2c_l204 = 0, w2c_l205 = 0, w2c_l206 = 0, 
      w2c_l207 = 0, w2c_l208 = 0, w2c_l209 = 0, w2c_l210 = 0, w2c_l211 = 0, w2c_l212 = 0, w2c_l213 = 0, w2c_l214 = 0, 
      w2c_l215 = 0, w2c_l216 = 0, w2c_l217 = 0, w2c_l218 = 0, w2c_l219 = 0, w2c_l220 = 0, w2c_l221 = 0, w2c_l222 = 0, 
      w2c_l223 = 0, w2c_l224 = 0, w2c_l225 = 0, w2c_l226 = 0, w2c_l227 = 0, w2c_l228 = 0, w2c_l229 = 0, w2c_l230 = 0, 
      w2c_l231 = 0, w2c_l232 = 0, w2c_l233 = 0, w2c_l234 = 0, w2c_l235 = 0, w2c_l236 = 0, w2c_l237 = 0, w2c_l238 = 0, 
      w2c_l239 = 0, w2c_l240 = 0, w2c_l241 = 0, w2c_l242 = 0, w2c_l243 = 0, w2c_l244 = 0, w2c_l245 = 0, w2c_l246 = 0, 
      w2c_l247 = 0, w2c_l248 = 0, w2c_l249 = 0, w2c_l250 = 0, w2c_l251 = 0, w2c_l252 = 0, w2c_l253 = 0, w2c_l254 = 0, 
      w2c_l255 = 0, w2c_l256 = 0, w2c_l257 = 0, w2c_l258 = 0, w2c_l259 = 0, w2c_l260 = 0, w2c_l261 = 0, w2c_l262 = 0, 
      w2c_l263 = 0, w2c_l264 = 0, w2c_l265 = 0, w2c_l266 = 0, w2c_l267 = 0, w2c_l268 = 0, w2c_l269 = 0, w2c_l270 = 0, 
      w2c_l271 = 0, w2c_l272 = 0, w2c_l273 = 0, w2c_l274 = 0, w2c_l275 = 0, w2c_l276 = 0, w2c_l277 = 0, w2c_l278 = 0, 
      w2c_l279 = 0, w2c_l280 = 0, w2c_l281 = 0, w2c_l282 = 0, w2c_l283 = 0, w2c_l284 = 0, w2c_l285 = 0, w2c_l286 = 0, 
      w2c_l287 = 0, w2c_l288 = 0, w2c_l289 = 0, w2c_l290 = 0, w2c_l291 = 0, w2c_l292 = 0, w2c_l293 = 0, w2c_l294 = 0, 
      w2c_l295 = 0, w2c_l296 = 0, w2c_l297 = 0, w2c_l298 = 0, w2c_l299 = 0, w2c_l300 = 0, w2c_l301 = 0, w2c_l302 = 0, 
      w2c_l303 = 0, w2c_l304 = 0, w2c_l305 = 0, w2c_l306 = 0, w2c_l307 = 0, w2c_l308 = 0, w2c_l309 = 0, w2c_l310 = 0, 
      w2c_l311 = 0, w2c_l312 = 0, w2c_l313 = 0, w2c_l314 = 0, w2c_l315 = 0, w2c_l316 = 0, w2c_l317 = 0, w2c_l318 = 0, 
      w2c_l319 = 0, w2c_l320 = 0, w2c_l321 = 0, w2c_l322 = 0, w2c_l323 = 0, w2c_l324 = 0, w2c_l325 = 0, w2c_l326 = 0, 
      w2c_l327 = 0, w2c_l328 = 0, w2c_l329 = 0, w2c_l330 = 0, w2c_l331 = 0, w2c_l332 = 0, w2c_l333 = 0, w2c_l334 = 0, 
      w2c_l335 = 0, w2c_l336 = 0, w2c_l337 = 0, w2c_l338 = 0, w2c_l339 = 0, w2c_l340 = 0, w2c_l341 = 0, w2c_l342 = 0, 
      w2c_l343 = 0, w2c_l344 = 0, w2c_l345 = 0, w2c_l346 = 0, w2c_l347 = 0, w2c_l348 = 0, w2c_l349 = 0, w2c_l350 = 0, 
      w2c_l351 = 0, w2c_l352 = 0, w2c_l353 = 0, w2c_l354 = 0, w2c_l355 = 0, w2c_l356 = 0, w2c_l357 = 0, w2c_l358 = 0, 
      w2c_l359 = 0, w2c_l360 = 0, w2c_l361 = 0, w2c_l362 = 0, w2c_l363 = 0, w2c_l364 = 0, w2c_l365 = 0, w2c_l366 = 0, 
      w2c_l367 = 0, w2c_l368 = 0, w2c_l369 = 0, w2c_l370 = 0, w2c_l371 = 0, w2c_l372 = 0, w2c_l373 = 0, w2c_l374 = 0, 
      w2c_l375 = 0, w2c_l376 = 0;
  FUNC_PROLOGUE;
  u32 w2c_i0, w2c_i1, w2c_i2, w2c_i3;
  w2c_i0 = w2c_g2;
  w2c_l375 = w2c_i0;
  w2c_i0 = w2c_g2;
  w2c_i1 = 128u;
  w2c_i0 += w2c_i1;
  w2c_g2 = w2c_i0;
  w2c_i0 = w2c_g2;
  w2c_i1 = w2c_g3;
  w2c_i0 = (u32)((s32)w2c_i0 >= (s32)w2c_i1);
  if (w2c_i0) {
    w2c_i0 = 128u;
    //(*Z_envZ_abortStackOverflowZ_vi)(w2c_i0);
  }
  w2c_i0 = w2c_p0;
  w2c_l218 = w2c_i0;
  w2c_i0 = w2c_p1;
  w2c_l219 = w2c_i0;
  w2c_i0 = w2c_p2;
  w2c_l230 = w2c_i0;
  w2c_i0 = w2c_p3;
  w2c_l241 = w2c_i0;
  w2c_i0 = w2c_p4;
  w2c_l243 = w2c_i0;
  w2c_i0 = w2c_p5;
  w2c_l244 = w2c_i0;
  w2c_i0 = w2c_p6;
  w2c_l245 = w2c_i0;
  w2c_i0 = w2c_p7;
  w2c_l246 = w2c_i0;
  w2c_i0 = w2c_p8;
  w2c_l247 = w2c_i0;
  w2c_i0 = w2c_p9;
  w2c_l248 = w2c_i0;
  w2c_i0 = w2c_p10;
  w2c_l220 = w2c_i0;
  w2c_i0 = w2c_p11;
  w2c_l221 = w2c_i0;
  w2c_i0 = w2c_p12;
  w2c_l222 = w2c_i0;
  w2c_i0 = w2c_p13;
  w2c_l223 = w2c_i0;
  w2c_i0 = w2c_p14;
  w2c_l224 = w2c_i0;
  w2c_i0 = w2c_p15;
  w2c_l225 = w2c_i0;
  w2c_i0 = w2c_p16;
  w2c_l226 = w2c_i0;
  w2c_i0 = w2c_p17;
  w2c_l227 = w2c_i0;
  w2c_i0 = w2c_p18;
  w2c_l228 = w2c_i0;
  w2c_i0 = w2c_p19;
  w2c_l229 = w2c_i0;
  w2c_i0 = w2c_p20;
  w2c_l231 = w2c_i0;
  w2c_i0 = w2c_p21;
  w2c_l232 = w2c_i0;
  w2c_i0 = w2c_p22;
  w2c_l233 = w2c_i0;
  w2c_i0 = w2c_p23;
  w2c_l234 = w2c_i0;
  w2c_i0 = w2c_p24;
  w2c_l235 = w2c_i0;
  w2c_i0 = w2c_p25;
  w2c_l236 = w2c_i0;
  w2c_i0 = w2c_p26;
  w2c_l237 = w2c_i0;
  w2c_i0 = w2c_p27;
  w2c_l238 = w2c_i0;
  w2c_i0 = w2c_p28;
  w2c_l239 = w2c_i0;
  w2c_i0 = w2c_p29;
  w2c_l240 = w2c_i0;
  w2c_i0 = w2c_p30;
  w2c_l242 = w2c_i0;
  w2c_i0 = 0u;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l218;
  w2c_l31 = w2c_i0;
  w2c_i0 = w2c_l31;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l280 = w2c_i0;
  w2c_i0 = w2c_l280;
  w2c_i1 = 167u;
  w2c_i0 &= w2c_i1;
  w2c_l156 = w2c_i0;
  w2c_i0 = w2c_l218;
  w2c_l32 = w2c_i0;
  w2c_i0 = w2c_l32;
  w2c_i1 = 4294967128u;
  w2c_i0 &= w2c_i1;
  w2c_l157 = w2c_i0;
  w2c_i0 = w2c_l156;
  w2c_i1 = w2c_l157;
  w2c_i0 |= w2c_i1;
  w2c_l311 = w2c_i0;
  w2c_i0 = w2c_l311;
  w2c_i1 = 193u;
  w2c_i0 -= w2c_i1;
  w2c_l343 = w2c_i0;
  w2c_i0 = w2c_l343;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l249 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l43 = w2c_i0;
  w2c_i0 = w2c_l43;
  w2c_i1 = w2c_l249;
  w2c_i0 += w2c_i1;
  w2c_l125 = w2c_i0;
  w2c_i0 = w2c_l125;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l219;
  w2c_l54 = w2c_i0;
  w2c_i0 = w2c_l54;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l297 = w2c_i0;
  w2c_i0 = w2c_l297;
  w2c_i1 = 16u;
  w2c_i0 &= w2c_i1;
  w2c_l196 = w2c_i0;
  w2c_i0 = w2c_l219;
  w2c_l65 = w2c_i0;
  w2c_i0 = w2c_l65;
  w2c_i1 = 4294967279u;
  w2c_i0 &= w2c_i1;
  w2c_l201 = w2c_i0;
  w2c_i0 = w2c_l196;
  w2c_i1 = w2c_l201;
  w2c_i0 |= w2c_i1;
  w2c_l334 = w2c_i0;
  w2c_i0 = w2c_l334;
  w2c_i1 = 32u;
  w2c_i0 -= w2c_i1;
  w2c_l367 = w2c_i0;
  w2c_i0 = w2c_l367;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l274 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l76 = w2c_i0;
  w2c_i0 = w2c_l76;
  w2c_i1 = w2c_l274;
  w2c_i0 += w2c_i1;
  w2c_l152 = w2c_i0;
  w2c_i0 = w2c_l152;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l230;
  w2c_l87 = w2c_i0;
  w2c_i0 = w2c_l87;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l309 = w2c_i0;
  w2c_i0 = w2c_l309;
  w2c_i1 = 240u;
  w2c_i0 &= w2c_i1;
  w2c_l158 = w2c_i0;
  w2c_i0 = w2c_l230;
  w2c_l98 = w2c_i0;
  w2c_i0 = w2c_l98;
  w2c_i1 = 4294967055u;
  w2c_i0 &= w2c_i1;
  w2c_l163 = w2c_i0;
  w2c_i0 = w2c_l158;
  w2c_i1 = w2c_l163;
  w2c_i0 |= w2c_i1;
  w2c_l315 = w2c_i0;
  w2c_i0 = w2c_l315;
  w2c_i1 = 130u;
  w2c_i0 -= w2c_i1;
  w2c_l348 = w2c_i0;
  w2c_i0 = w2c_l348;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l255 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l109 = w2c_i0;
  w2c_i0 = w2c_l109;
  w2c_i1 = w2c_l255;
  w2c_i0 += w2c_i1;
  w2c_l133 = w2c_i0;
  w2c_i0 = w2c_l133;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l241;
  w2c_l120 = w2c_i0;
  w2c_i0 = w2c_l120;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l290 = w2c_i0;
  w2c_i0 = w2c_l290;
  w2c_i1 = 159u;
  w2c_i0 &= w2c_i1;
  w2c_l180 = w2c_i0;
  w2c_i0 = w2c_l241;
  w2c_l33 = w2c_i0;
  w2c_i0 = w2c_l33;
  w2c_i1 = 4294967136u;
  w2c_i0 &= w2c_i1;
  w2c_l185 = w2c_i0;
  w2c_i0 = w2c_l180;
  w2c_i1 = w2c_l185;
  w2c_i0 |= w2c_i1;
  w2c_l326 = w2c_i0;
  w2c_i0 = w2c_l326;
  w2c_i1 = 192u;
  w2c_i0 -= w2c_i1;
  w2c_l359 = w2c_i0;
  w2c_i0 = w2c_l359;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l266 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l34 = w2c_i0;
  w2c_i0 = w2c_l34;
  w2c_i1 = w2c_l266;
  w2c_i0 += w2c_i1;
  w2c_l143 = w2c_i0;
  w2c_i0 = w2c_l143;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l243;
  w2c_l35 = w2c_i0;
  w2c_i0 = w2c_l35;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l299 = w2c_i0;
  w2c_i0 = w2c_l299;
  w2c_i1 = 240u;
  w2c_i0 &= w2c_i1;
  w2c_l194 = w2c_i0;
  w2c_i0 = w2c_l243;
  w2c_l36 = w2c_i0;
  w2c_i0 = w2c_l36;
  w2c_i1 = 4294967055u;
  w2c_i0 &= w2c_i1;
  w2c_l195 = w2c_i0;
  w2c_i0 = w2c_l194;
  w2c_i1 = w2c_l195;
  w2c_i0 |= w2c_i1;
  w2c_l330 = w2c_i0;
  w2c_i0 = w2c_l330;
  w2c_i1 = 150u;
  w2c_i0 -= w2c_i1;
  w2c_l362 = w2c_i0;
  w2c_i0 = w2c_l362;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l268 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l37 = w2c_i0;
  w2c_i0 = w2c_l37;
  w2c_i1 = w2c_l268;
  w2c_i0 += w2c_i1;
  w2c_l144 = w2c_i0;
  w2c_i0 = w2c_l144;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l244;
  w2c_l38 = w2c_i0;
  w2c_i0 = w2c_l38;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l300 = w2c_i0;
  w2c_i0 = w2c_l300;
  w2c_i1 = 65u;
  w2c_i0 &= w2c_i1;
  w2c_l197 = w2c_i0;
  w2c_i0 = w2c_l244;
  w2c_l39 = w2c_i0;
  w2c_i0 = w2c_l39;
  w2c_i1 = 4294967230u;
  w2c_i0 &= w2c_i1;
  w2c_l198 = w2c_i0;
  w2c_i0 = w2c_l197;
  w2c_i1 = w2c_l198;
  w2c_i0 |= w2c_i1;
  w2c_l331 = w2c_i0;
  w2c_i0 = w2c_l331;
  w2c_i1 = 52u;
  w2c_i0 -= w2c_i1;
  w2c_l363 = w2c_i0;
  w2c_i0 = w2c_l363;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l269 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l40 = w2c_i0;
  w2c_i0 = w2c_l40;
  w2c_i1 = w2c_l269;
  w2c_i0 += w2c_i1;
  w2c_l145 = w2c_i0;
  w2c_i0 = w2c_l145;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l245;
  w2c_l41 = w2c_i0;
  w2c_i0 = w2c_l41;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l301 = w2c_i0;
  w2c_i0 = w2c_l301;
  w2c_i1 = 105u;
  w2c_i0 &= w2c_i1;
  w2c_l199 = w2c_i0;
  w2c_i0 = w2c_l245;
  w2c_l42 = w2c_i0;
  w2c_i0 = w2c_l42;
  w2c_i1 = 4294967190u;
  w2c_i0 &= w2c_i1;
  w2c_l200 = w2c_i0;
  w2c_i0 = w2c_l199;
  w2c_i1 = w2c_l200;
  w2c_i0 |= w2c_i1;
  w2c_l332 = w2c_i0;
  w2c_i0 = w2c_l332;
  w2c_i1 = 27u;
  w2c_i0 -= w2c_i1;
  w2c_l364 = w2c_i0;
  w2c_i0 = w2c_l364;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l270 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l44 = w2c_i0;
  w2c_i0 = w2c_l44;
  w2c_i1 = w2c_l270;
  w2c_i0 += w2c_i1;
  w2c_l146 = w2c_i0;
  w2c_i0 = w2c_l146;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l246;
  w2c_l45 = w2c_i0;
  w2c_i0 = w2c_l45;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l302 = w2c_i0;
  w2c_i0 = w2c_l302;
  w2c_i1 = 49u;
  w2c_i0 &= w2c_i1;
  w2c_l202 = w2c_i0;
  w2c_i0 = w2c_l246;
  w2c_l46 = w2c_i0;
  w2c_i0 = w2c_l46;
  w2c_i1 = 4294967246u;
  w2c_i0 &= w2c_i1;
  w2c_l203 = w2c_i0;
  w2c_i0 = w2c_l202;
  w2c_i1 = w2c_l203;
  w2c_i0 |= w2c_i1;
  w2c_l333 = w2c_i0;
  w2c_i0 = w2c_l333;
  w2c_i1 = 69u;
  w2c_i0 -= w2c_i1;
  w2c_l365 = w2c_i0;
  w2c_i0 = w2c_l365;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l271 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l47 = w2c_i0;
  w2c_i0 = w2c_l47;
  w2c_i1 = w2c_l271;
  w2c_i0 += w2c_i1;
  w2c_l147 = w2c_i0;
  w2c_i0 = w2c_l147;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l247;
  w2c_l48 = w2c_i0;
  w2c_i0 = w2c_l48;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l303 = w2c_i0;
  w2c_i0 = w2c_l303;
  w2c_i1 = 119u;
  w2c_i0 &= w2c_i1;
  w2c_l204 = w2c_i0;
  w2c_i0 = w2c_l247;
  w2c_l49 = w2c_i0;
  w2c_i0 = w2c_l49;
  w2c_i1 = 4294967176u;
  w2c_i0 &= w2c_i1;
  w2c_l205 = w2c_i0;
  w2c_i0 = w2c_l204;
  w2c_i1 = w2c_l205;
  w2c_i0 |= w2c_i1;
  w2c_l335 = w2c_i0;
  w2c_i0 = w2c_l335;
  w2c_i1 = 31u;
  w2c_i0 -= w2c_i1;
  w2c_l366 = w2c_i0;
  w2c_i0 = w2c_l366;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l272 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l50 = w2c_i0;
  w2c_i0 = w2c_l50;
  w2c_i1 = w2c_l272;
  w2c_i0 += w2c_i1;
  w2c_l148 = w2c_i0;
  w2c_i0 = w2c_l148;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l248;
  w2c_l51 = w2c_i0;
  w2c_i0 = w2c_l51;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l304 = w2c_i0;
  w2c_i0 = w2c_l304;
  w2c_i1 = 12u;
  w2c_i0 &= w2c_i1;
  w2c_l206 = w2c_i0;
  w2c_i0 = w2c_l248;
  w2c_l52 = w2c_i0;
  w2c_i0 = w2c_l52;
  w2c_i1 = 4294967283u;
  w2c_i0 &= w2c_i1;
  w2c_l207 = w2c_i0;
  w2c_i0 = w2c_l206;
  w2c_i1 = w2c_l207;
  w2c_i0 |= w2c_i1;
  w2c_l336 = w2c_i0;
  w2c_i0 = w2c_l336;
  w2c_i1 = 63u;
  w2c_i0 -= w2c_i1;
  w2c_l368 = w2c_i0;
  w2c_i0 = w2c_l368;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l273 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l53 = w2c_i0;
  w2c_i0 = w2c_l53;
  w2c_i1 = w2c_l273;
  w2c_i0 += w2c_i1;
  w2c_l149 = w2c_i0;
  w2c_i0 = w2c_l149;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l220;
  w2c_l55 = w2c_i0;
  w2c_i0 = w2c_l55;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l305 = w2c_i0;
  w2c_i0 = w2c_l305;
  w2c_i1 = 210u;
  w2c_i0 &= w2c_i1;
  w2c_l208 = w2c_i0;
  w2c_i0 = w2c_l220;
  w2c_l56 = w2c_i0;
  w2c_i0 = w2c_l56;
  w2c_i1 = 4294967085u;
  w2c_i0 &= w2c_i1;
  w2c_l209 = w2c_i0;
  w2c_i0 = w2c_l208;
  w2c_i1 = w2c_l209;
  w2c_i0 |= w2c_i1;
  w2c_l337 = w2c_i0;
  w2c_i0 = w2c_l337;
  w2c_i1 = 160u;
  w2c_i0 -= w2c_i1;
  w2c_l369 = w2c_i0;
  w2c_i0 = w2c_l369;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l275 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l57 = w2c_i0;
  w2c_i0 = w2c_l57;
  w2c_i1 = w2c_l275;
  w2c_i0 += w2c_i1;
  w2c_l150 = w2c_i0;
  w2c_i0 = w2c_l150;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l221;
  w2c_l58 = w2c_i0;
  w2c_i0 = w2c_l58;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l306 = w2c_i0;
  w2c_i0 = w2c_l306;
  w2c_i1 = 108u;
  w2c_i0 &= w2c_i1;
  w2c_l210 = w2c_i0;
  w2c_i0 = w2c_l221;
  w2c_l59 = w2c_i0;
  w2c_i0 = w2c_l59;
  w2c_i1 = 4294967187u;
  w2c_i0 &= w2c_i1;
  w2c_l211 = w2c_i0;
  w2c_i0 = w2c_l210;
  w2c_i1 = w2c_l211;
  w2c_i0 |= w2c_i1;
  w2c_l338 = w2c_i0;
  w2c_i0 = w2c_l338;
  w2c_i1 = 51u;
  w2c_i0 -= w2c_i1;
  w2c_l370 = w2c_i0;
  w2c_i0 = w2c_l370;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l276 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l60 = w2c_i0;
  w2c_i0 = w2c_l60;
  w2c_i1 = w2c_l276;
  w2c_i0 += w2c_i1;
  w2c_l151 = w2c_i0;
  w2c_i0 = w2c_l151;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l222;
  w2c_l61 = w2c_i0;
  w2c_i0 = w2c_l61;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l307 = w2c_i0;
  w2c_i0 = w2c_l307;
  w2c_i1 = 184u;
  w2c_i0 &= w2c_i1;
  w2c_l212 = w2c_i0;
  w2c_i0 = w2c_l222;
  w2c_l62 = w2c_i0;
  w2c_i0 = w2c_l62;
  w2c_i1 = 4294967111u;
  w2c_i0 &= w2c_i1;
  w2c_l213 = w2c_i0;
  w2c_i0 = w2c_l212;
  w2c_i1 = w2c_l213;
  w2c_i0 |= w2c_i1;
  w2c_l339 = w2c_i0;
  w2c_i0 = w2c_l339;
  w2c_i1 = 137u;
  w2c_i0 -= w2c_i1;
  w2c_l371 = w2c_i0;
  w2c_i0 = w2c_l371;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l277 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l63 = w2c_i0;
  w2c_i0 = w2c_l63;
  w2c_i1 = w2c_l277;
  w2c_i0 += w2c_i1;
  w2c_l153 = w2c_i0;
  w2c_i0 = w2c_l153;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l223;
  w2c_l64 = w2c_i0;
  w2c_i0 = w2c_l64;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l308 = w2c_i0;
  w2c_i0 = w2c_l308;
  w2c_i1 = 235u;
  w2c_i0 &= w2c_i1;
  w2c_l214 = w2c_i0;
  w2c_i0 = w2c_l223;
  w2c_l66 = w2c_i0;
  w2c_i0 = w2c_l66;
  w2c_i1 = 4294967060u;
  w2c_i0 &= w2c_i1;
  w2c_l215 = w2c_i0;
  w2c_i0 = w2c_l214;
  w2c_i1 = w2c_l215;
  w2c_i0 |= w2c_i1;
  w2c_l340 = w2c_i0;
  w2c_i0 = w2c_l340;
  w2c_i1 = 133u;
  w2c_i0 -= w2c_i1;
  w2c_l372 = w2c_i0;
  w2c_i0 = w2c_l372;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l278 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l67 = w2c_i0;
  w2c_i0 = w2c_l67;
  w2c_i1 = w2c_l278;
  w2c_i0 += w2c_i1;
  w2c_l154 = w2c_i0;
  w2c_i0 = w2c_l154;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l224;
  w2c_l68 = w2c_i0;
  w2c_i0 = w2c_l68;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l310 = w2c_i0;
  w2c_i0 = w2c_l310;
  w2c_i1 = 118u;
  w2c_i0 &= w2c_i1;
  w2c_l216 = w2c_i0;
  w2c_i0 = w2c_l224;
  w2c_l69 = w2c_i0;
  w2c_i0 = w2c_l69;
  w2c_i1 = 4294967177u;
  w2c_i0 &= w2c_i1;
  w2c_l217 = w2c_i0;
  w2c_i0 = w2c_l216;
  w2c_i1 = w2c_l217;
  w2c_i0 |= w2c_i1;
  w2c_l341 = w2c_i0;
  w2c_i0 = w2c_l341;
  w2c_i1 = 16u;
  w2c_i0 -= w2c_i1;
  w2c_l373 = w2c_i0;
  w2c_i0 = w2c_l373;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l279 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l70 = w2c_i0;
  w2c_i0 = w2c_l70;
  w2c_i1 = w2c_l279;
  w2c_i0 += w2c_i1;
  w2c_l155 = w2c_i0;
  w2c_i0 = w2c_l155;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l225;
  w2c_l71 = w2c_i0;
  w2c_i0 = w2c_l71;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l281 = w2c_i0;
  w2c_i0 = w2c_l281;
  w2c_i1 = 52u;
  w2c_i0 &= w2c_i1;
  w2c_l159 = w2c_i0;
  w2c_i0 = w2c_l225;
  w2c_l72 = w2c_i0;
  w2c_i0 = w2c_l72;
  w2c_i1 = 4294967243u;
  w2c_i0 &= w2c_i1;
  w2c_l160 = w2c_i0;
  w2c_i0 = w2c_l159;
  w2c_i1 = w2c_l160;
  w2c_i0 |= w2c_i1;
  w2c_l312 = w2c_i0;
  w2c_i0 = w2c_l312;
  w2c_i1 = 4u;
  w2c_i0 -= w2c_i1;
  w2c_l344 = w2c_i0;
  w2c_i0 = w2c_l344;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l250 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l73 = w2c_i0;
  w2c_i0 = w2c_l73;
  w2c_i1 = w2c_l250;
  w2c_i0 += w2c_i1;
  w2c_l126 = w2c_i0;
  w2c_i0 = w2c_l126;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l226;
  w2c_l74 = w2c_i0;
  w2c_i0 = w2c_l74;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l282 = w2c_i0;
  w2c_i0 = w2c_l282;
  w2c_i1 = 213u;
  w2c_i0 &= w2c_i1;
  w2c_l161 = w2c_i0;
  w2c_i0 = w2c_l226;
  w2c_l75 = w2c_i0;
  w2c_i0 = w2c_l75;
  w2c_i1 = 4294967082u;
  w2c_i0 &= w2c_i1;
  w2c_l162 = w2c_i0;
  w2c_i0 = w2c_l161;
  w2c_i1 = w2c_l162;
  w2c_i0 |= w2c_i1;
  w2c_l313 = w2c_i0;
  w2c_i0 = w2c_l313;
  w2c_i1 = 138u;
  w2c_i0 -= w2c_i1;
  w2c_l345 = w2c_i0;
  w2c_i0 = w2c_l345;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l251 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l77 = w2c_i0;
  w2c_i0 = w2c_l77;
  w2c_i1 = w2c_l251;
  w2c_i0 += w2c_i1;
  w2c_l127 = w2c_i0;
  w2c_i0 = w2c_l127;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l227;
  w2c_l78 = w2c_i0;
  w2c_i0 = w2c_l78;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l283 = w2c_i0;
  w2c_i0 = w2c_l283;
  w2c_i1 = 5u;
  w2c_i0 &= w2c_i1;
  w2c_l164 = w2c_i0;
  w2c_i0 = w2c_l227;
  w2c_l79 = w2c_i0;
  w2c_i0 = w2c_l79;
  w2c_i1 = 4294967290u;
  w2c_i0 &= w2c_i1;
  w2c_l165 = w2c_i0;
  w2c_i0 = w2c_l164;
  w2c_i1 = w2c_l165;
  w2c_i0 |= w2c_i1;
  w2c_l314 = w2c_i0;
  w2c_i0 = w2c_l314;
  w2c_i1 = 102u;
  w2c_i0 -= w2c_i1;
  w2c_l346 = w2c_i0;
  w2c_i0 = w2c_l346;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l252 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l80 = w2c_i0;
  w2c_i0 = w2c_l80;
  w2c_i1 = w2c_l252;
  w2c_i0 += w2c_i1;
  w2c_l128 = w2c_i0;
  w2c_i0 = w2c_l128;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l228;
  w2c_l81 = w2c_i0;
  w2c_i0 = w2c_l81;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l284 = w2c_i0;
  w2c_i0 = w2c_l284;
  w2c_i1 = 107u;
  w2c_i0 &= w2c_i1;
  w2c_l166 = w2c_i0;
  w2c_i0 = w2c_l228;
  w2c_l82 = w2c_i0;
  w2c_i0 = w2c_l82;
  w2c_i1 = 4294967188u;
  w2c_i0 &= w2c_i1;
  w2c_l167 = w2c_i0;
  w2c_i0 = w2c_l166;
  w2c_i1 = w2c_l167;
  w2c_i0 |= w2c_i1;
  w2c_l316 = w2c_i0;
  w2c_i0 = w2c_l316;
  w2c_i1 = 95u;
  w2c_i0 -= w2c_i1;
  w2c_l347 = w2c_i0;
  w2c_i0 = w2c_l347;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l253 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l83 = w2c_i0;
  w2c_i0 = w2c_l83;
  w2c_i1 = w2c_l253;
  w2c_i0 += w2c_i1;
  w2c_l129 = w2c_i0;
  w2c_i0 = w2c_l129;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l229;
  w2c_l84 = w2c_i0;
  w2c_i0 = w2c_l84;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l285 = w2c_i0;
  w2c_i0 = w2c_l285;
  w2c_i1 = 106u;
  w2c_i0 &= w2c_i1;
  w2c_l168 = w2c_i0;
  w2c_i0 = w2c_l229;
  w2c_l85 = w2c_i0;
  w2c_i0 = w2c_l85;
  w2c_i1 = 4294967189u;
  w2c_i0 &= w2c_i1;
  w2c_l169 = w2c_i0;
  w2c_i0 = w2c_l168;
  w2c_i1 = w2c_l169;
  w2c_i0 |= w2c_i1;
  w2c_l317 = w2c_i0;
  w2c_i0 = w2c_l317;
  w2c_i1 = 91u;
  w2c_i0 -= w2c_i1;
  w2c_l349 = w2c_i0;
  w2c_i0 = w2c_l349;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l254 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l86 = w2c_i0;
  w2c_i0 = w2c_l86;
  w2c_i1 = w2c_l254;
  w2c_i0 += w2c_i1;
  w2c_l130 = w2c_i0;
  w2c_i0 = w2c_l130;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l231;
  w2c_l88 = w2c_i0;
  w2c_i0 = w2c_l88;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l286 = w2c_i0;
  w2c_i0 = w2c_l286;
  w2c_i1 = 165u;
  w2c_i0 &= w2c_i1;
  w2c_l170 = w2c_i0;
  w2c_i0 = w2c_l231;
  w2c_l89 = w2c_i0;
  w2c_i0 = w2c_l89;
  w2c_i1 = 4294967130u;
  w2c_i0 &= w2c_i1;
  w2c_l171 = w2c_i0;
  w2c_i0 = w2c_l170;
  w2c_i1 = w2c_l171;
  w2c_i0 |= w2c_i1;
  w2c_l318 = w2c_i0;
  w2c_i0 = w2c_l318;
  w2c_i1 = 148u;
  w2c_i0 -= w2c_i1;
  w2c_l350 = w2c_i0;
  w2c_i0 = w2c_l350;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l256 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l90 = w2c_i0;
  w2c_i0 = w2c_l90;
  w2c_i1 = w2c_l256;
  w2c_i0 += w2c_i1;
  w2c_l131 = w2c_i0;
  w2c_i0 = w2c_l131;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l232;
  w2c_l91 = w2c_i0;
  w2c_i0 = w2c_l91;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l287 = w2c_i0;
  w2c_i0 = w2c_l287;
  w2c_i1 = 12u;
  w2c_i0 &= w2c_i1;
  w2c_l172 = w2c_i0;
  w2c_i0 = w2c_l232;
  w2c_l92 = w2c_i0;
  w2c_i0 = w2c_l92;
  w2c_i1 = 4294967283u;
  w2c_i0 &= w2c_i1;
  w2c_l173 = w2c_i0;
  w2c_i0 = w2c_l172;
  w2c_i1 = w2c_l173;
  w2c_i0 |= w2c_i1;
  w2c_l319 = w2c_i0;
  w2c_i0 = w2c_l319;
  w2c_i1 = 83u;
  w2c_i0 -= w2c_i1;
  w2c_l351 = w2c_i0;
  w2c_i0 = w2c_l351;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l257 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l93 = w2c_i0;
  w2c_i0 = w2c_l93;
  w2c_i1 = w2c_l257;
  w2c_i0 += w2c_i1;
  w2c_l132 = w2c_i0;
  w2c_i0 = w2c_l132;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l233;
  w2c_l94 = w2c_i0;
  w2c_i0 = w2c_l94;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l288 = w2c_i0;
  w2c_i0 = w2c_l288;
  w2c_i1 = 182u;
  w2c_i0 &= w2c_i1;
  w2c_l174 = w2c_i0;
  w2c_i0 = w2c_l233;
  w2c_l95 = w2c_i0;
  w2c_i0 = w2c_l95;
  w2c_i1 = 4294967113u;
  w2c_i0 &= w2c_i1;
  w2c_l175 = w2c_i0;
  w2c_i0 = w2c_l174;
  w2c_i1 = w2c_l175;
  w2c_i0 |= w2c_i1;
  w2c_l320 = w2c_i0;
  w2c_i0 = w2c_l320;
  w2c_i1 = 134u;
  w2c_i0 -= w2c_i1;
  w2c_l352 = w2c_i0;
  w2c_i0 = w2c_l352;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l258 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l96 = w2c_i0;
  w2c_i0 = w2c_l96;
  w2c_i1 = w2c_l258;
  w2c_i0 += w2c_i1;
  w2c_l134 = w2c_i0;
  w2c_i0 = w2c_l134;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l234;
  w2c_l97 = w2c_i0;
  w2c_i0 = w2c_l97;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l289 = w2c_i0;
  w2c_i0 = w2c_l289;
  w2c_i1 = 220u;
  w2c_i0 &= w2c_i1;
  w2c_l176 = w2c_i0;
  w2c_i0 = w2c_l234;
  w2c_l99 = w2c_i0;
  w2c_i0 = w2c_l99;
  w2c_i1 = 4294967075u;
  w2c_i0 &= w2c_i1;
  w2c_l177 = w2c_i0;
  w2c_i0 = w2c_l176;
  w2c_i1 = w2c_l177;
  w2c_i0 |= w2c_i1;
  w2c_l321 = w2c_i0;
  w2c_i0 = w2c_l321;
  w2c_i1 = 239u;
  w2c_i0 -= w2c_i1;
  w2c_l353 = w2c_i0;
  w2c_i0 = w2c_l353;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l259 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l100 = w2c_i0;
  w2c_i0 = w2c_l100;
  w2c_i1 = w2c_l259;
  w2c_i0 += w2c_i1;
  w2c_l135 = w2c_i0;
  w2c_i0 = w2c_l135;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l235;
  w2c_l101 = w2c_i0;
  w2c_i0 = w2c_l101;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l291 = w2c_i0;
  w2c_i0 = w2c_l291;
  w2c_i1 = 140u;
  w2c_i0 &= w2c_i1;
  w2c_l178 = w2c_i0;
  w2c_i0 = w2c_l235;
  w2c_l102 = w2c_i0;
  w2c_i0 = w2c_l102;
  w2c_i1 = 4294967155u;
  w2c_i0 &= w2c_i1;
  w2c_l179 = w2c_i0;
  w2c_i0 = w2c_l178;
  w2c_i1 = w2c_l179;
  w2c_i0 |= w2c_i1;
  w2c_l322 = w2c_i0;
  w2c_i0 = w2c_l322;
  w2c_i1 = 191u;
  w2c_i0 -= w2c_i1;
  w2c_l354 = w2c_i0;
  w2c_i0 = w2c_l354;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l260 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l103 = w2c_i0;
  w2c_i0 = w2c_l103;
  w2c_i1 = w2c_l260;
  w2c_i0 += w2c_i1;
  w2c_l136 = w2c_i0;
  w2c_i0 = w2c_l136;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l236;
  w2c_l104 = w2c_i0;
  w2c_i0 = w2c_l104;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l292 = w2c_i0;
  w2c_i0 = w2c_l292;
  w2c_i1 = 149u;
  w2c_i0 &= w2c_i1;
  w2c_l181 = w2c_i0;
  w2c_i0 = w2c_l236;
  w2c_l105 = w2c_i0;
  w2c_i0 = w2c_l105;
  w2c_i1 = 4294967146u;
  w2c_i0 &= w2c_i1;
  w2c_l182 = w2c_i0;
  w2c_i0 = w2c_l181;
  w2c_i1 = w2c_l182;
  w2c_i0 |= w2c_i1;
  w2c_l323 = w2c_i0;
  w2c_i0 = w2c_l323;
  w2c_i1 = 162u;
  w2c_i0 -= w2c_i1;
  w2c_l355 = w2c_i0;
  w2c_i0 = w2c_l355;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l261 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l106 = w2c_i0;
  w2c_i0 = w2c_l106;
  w2c_i1 = w2c_l261;
  w2c_i0 += w2c_i1;
  w2c_l137 = w2c_i0;
  w2c_i0 = w2c_l137;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l237;
  w2c_l107 = w2c_i0;
  w2c_i0 = w2c_l107;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l293 = w2c_i0;
  w2c_i0 = w2c_l293;
  w2c_i1 = 18u;
  w2c_i0 &= w2c_i1;
  w2c_l183 = w2c_i0;
  w2c_i0 = w2c_l237;
  w2c_l108 = w2c_i0;
  w2c_i0 = w2c_l108;
  w2c_i1 = 4294967277u;
  w2c_i0 &= w2c_i1;
  w2c_l184 = w2c_i0;
  w2c_i0 = w2c_l183;
  w2c_i1 = w2c_l184;
  w2c_i0 |= w2c_i1;
  w2c_l324 = w2c_i0;
  w2c_i0 = w2c_l324;
  w2c_i1 = 32u;
  w2c_i0 -= w2c_i1;
  w2c_l356 = w2c_i0;
  w2c_i0 = w2c_l356;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l262 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l110 = w2c_i0;
  w2c_i0 = w2c_l110;
  w2c_i1 = w2c_l262;
  w2c_i0 += w2c_i1;
  w2c_l138 = w2c_i0;
  w2c_i0 = w2c_l138;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l238;
  w2c_l111 = w2c_i0;
  w2c_i0 = w2c_l111;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l294 = w2c_i0;
  w2c_i0 = w2c_l294;
  w2c_i1 = 168u;
  w2c_i0 &= w2c_i1;
  w2c_l186 = w2c_i0;
  w2c_i0 = w2c_l238;
  w2c_l112 = w2c_i0;
  w2c_i0 = w2c_l112;
  w2c_i1 = 4294967127u;
  w2c_i0 &= w2c_i1;
  w2c_l187 = w2c_i0;
  w2c_i0 = w2c_l186;
  w2c_i1 = w2c_l187;
  w2c_i0 |= w2c_i1;
  w2c_l325 = w2c_i0;
  w2c_i0 = w2c_l325;
  w2c_i1 = 144u;
  w2c_i0 -= w2c_i1;
  w2c_l357 = w2c_i0;
  w2c_i0 = w2c_l357;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l263 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l113 = w2c_i0;
  w2c_i0 = w2c_l113;
  w2c_i1 = w2c_l263;
  w2c_i0 += w2c_i1;
  w2c_l139 = w2c_i0;
  w2c_i0 = w2c_l139;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l239;
  w2c_l114 = w2c_i0;
  w2c_i0 = w2c_l114;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l295 = w2c_i0;
  w2c_i0 = w2c_l295;
  w2c_i1 = 234u;
  w2c_i0 &= w2c_i1;
  w2c_l188 = w2c_i0;
  w2c_i0 = w2c_l239;
  w2c_l115 = w2c_i0;
  w2c_i0 = w2c_l115;
  w2c_i1 = 4294967061u;
  w2c_i0 &= w2c_i1;
  w2c_l189 = w2c_i0;
  w2c_i0 = w2c_l188;
  w2c_i1 = w2c_l189;
  w2c_i0 |= w2c_i1;
  w2c_l327 = w2c_i0;
  w2c_i0 = w2c_l327;
  w2c_i1 = 219u;
  w2c_i0 -= w2c_i1;
  w2c_l358 = w2c_i0;
  w2c_i0 = w2c_l358;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l264 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l116 = w2c_i0;
  w2c_i0 = w2c_l116;
  w2c_i1 = w2c_l264;
  w2c_i0 += w2c_i1;
  w2c_l140 = w2c_i0;
  w2c_i0 = w2c_l140;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l240;
  w2c_l117 = w2c_i0;
  w2c_i0 = w2c_l117;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l296 = w2c_i0;
  w2c_i0 = w2c_l296;
  w2c_i1 = 173u;
  w2c_i0 &= w2c_i1;
  w2c_l190 = w2c_i0;
  w2c_i0 = w2c_l240;
  w2c_l118 = w2c_i0;
  w2c_i0 = w2c_l118;
  w2c_i1 = 4294967122u;
  w2c_i0 &= w2c_i1;
  w2c_l191 = w2c_i0;
  w2c_i0 = w2c_l190;
  w2c_i1 = w2c_l191;
  w2c_i0 |= w2c_i1;
  w2c_l328 = w2c_i0;
  w2c_i0 = w2c_l328;
  w2c_i1 = 149u;
  w2c_i0 -= w2c_i1;
  w2c_l360 = w2c_i0;
  w2c_i0 = w2c_l360;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l265 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l119 = w2c_i0;
  w2c_i0 = w2c_l119;
  w2c_i1 = w2c_l265;
  w2c_i0 += w2c_i1;
  w2c_l141 = w2c_i0;
  w2c_i0 = w2c_l141;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l242;
  w2c_l121 = w2c_i0;
  w2c_i0 = w2c_l121;
  w2c_i1 = 4294967295u;
  w2c_i0 ^= w2c_i1;
  w2c_l298 = w2c_i0;
  w2c_i0 = w2c_l298;
  w2c_i1 = 39u;
  w2c_i0 &= w2c_i1;
  w2c_l192 = w2c_i0;
  w2c_i0 = w2c_l242;
  w2c_l122 = w2c_i0;
  w2c_i0 = w2c_l122;
  w2c_i1 = 4294967256u;
  w2c_i0 &= w2c_i1;
  w2c_l193 = w2c_i0;
  w2c_i0 = w2c_l192;
  w2c_i1 = w2c_l193;
  w2c_i0 |= w2c_i1;
  w2c_l329 = w2c_i0;
  w2c_i0 = w2c_l329;
  w2c_i1 = 18u;
  w2c_i0 -= w2c_i1;
  w2c_l361 = w2c_i0;
  w2c_i0 = w2c_l361;
  w2c_l376 = w2c_i0;
  w2c_i0 = 0u;
  w2c_i1 = w2c_l376;
  w2c_i0 -= w2c_i1;
  w2c_i1 = w2c_l376;
  w2c_i2 = w2c_l376;
  w2c_i3 = 0u;
  w2c_i2 = (u32)((s32)w2c_i2 < (s32)w2c_i3);
  w2c_i0 = w2c_i2 ? w2c_i0 : w2c_i1;
  w2c_l267 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l123 = w2c_i0;
  w2c_i0 = w2c_l123;
  w2c_i1 = w2c_l267;
  w2c_i0 += w2c_i1;
  w2c_l142 = w2c_i0;
  w2c_i0 = w2c_l142;
  w2c_l342 = w2c_i0;
  w2c_i0 = w2c_l342;
  w2c_l124 = w2c_i0;
  w2c_i0 = w2c_l375;
  w2c_g2 = w2c_i0;
  w2c_i0 = w2c_l124;
  goto w2c_Bfunc;
  w2c_Bfunc:;
  FUNC_EPILOGUE;
  return w2c_i0;
}


static void init_memory(void) {
}

static void init_table(void) {
  uint32_t offset;
}

/* export: '_ver' */
u32 (*WASM_RT_ADD_PREFIX(Z__verZ_iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii))(u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32, u32);

static void init_exports(void) {
  /* export: '_ver' */
  WASM_RT_ADD_PREFIX(Z__verZ_iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii) = (&w2c__ver);
}

void WASM_RT_ADD_PREFIX(init)(void) {
  init_func_types();
  init_globals();
  init_memory();
  init_table();
  init_exports();
}

```

</details>

That not very easy to read, but in C you could do worse. We can still translate the C into a slightly more readable form of pseudocode:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ ~/utils/web/wabt/build/wasm-decompile script.wasm -o script.dcmp
```

This is what we get:

<details>
  <summary>Click to expand!</summary>

```
import memory env_memory;

import global env_memory_base:int;
import global env_table_base:int;
global g_c:int = 2080;
global g_d:int = 5244960;
global g_e:float = 0.0f;

import table env_table:funcref;

import function env_abortStackOverflow(a:int);

export function ver(a:int, b:int, c:int, d:int, e:int, f:int, g:int, h:int, i:int, j:int, k:int, l:int, m:int, n:int, o:int, p:int, q:int, r:int, s:int, t:int, u:int, v:int, w:int, x:int, y:int, z:int, aa:int, ba:int, ca:int, da:int, ea:int):int {
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var mn:int;
  var ln:int = g_c;
  g_c = g_c + 128;
  if (g_c >= g_d) { env_abortStackOverflow(128) }
  var kh:int = a;
  var lh:int = b;
  var wh:int = c;
  var hi:int = d;
  var ji:int = e;
  var ki:int = f;
  var li:int = g;
  var mi:int = h;
  var ni:int = i;
  var oi:int = j;
  var mh:int = k;
  var nh:int = l;
  var oh:int = m;
  var ph:int = n;
  var qh:int = o;
  var rh:int = p;
  var sh:int = q;
  var th:int = r;
  var uh:int = s;
  var vh:int = t;
  var xh:int = u;
  var yh:int = v;
  var zh:int = w;
  var ai:int = x;
  var bi:int = y;
  var ci:int = z;
  var di:int = aa;
  var ei:int = ba;
  var fi:int = ca;
  var gi:int = da;
  var ii:int = ea;
  var em:int = 0;
  var fa:int = kh;
  var uj:int = fa ^ -1;
  var af:int = uj & 167;
  var ga:int = kh;
  var bf:int = ga & -168;
  var zk:int = af | bf;
  var fm:int = zk - 193;
  var pi:int = {
                 mn = fm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_b:
               }
  var ra:int = em;
  var vd:int = ra + pi;
  em = vd;
  var cb:int = lh;
  var lk:int = cb ^ -1;
  var og:int = lk & 16;
  var nb:int = lh;
  var tg:int = nb & -17;
  var wl:int = og | tg;
  var dn:int = wl - 32;
  var oj:int = {
                 mn = dn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_c:
               }
  var yb:int = em;
  var we:int = yb + oj;
  em = we;
  var jc:int = wh;
  var xk:int = jc ^ -1;
  var cf:int = xk & 240;
  var uc:int = wh;
  var hf:int = uc & -241;
  var dl:int = cf | hf;
  var km:int = dl - 130;
  var vi:int = {
                 mn = km;
                 select_if(0 - mn, mn, mn < 0);
                 label B_d:
               }
  var fd:int = em;
  var de:int = fd + vi;
  em = de;
  var qd:int = hi;
  var ek:int = qd ^ -1;
  var yf:int = ek & 159;
  var ha:int = hi;
  var dg:int = ha & -160;
  var ol:int = yf | dg;
  var vm:int = ol - 192;
  var gj:int = {
                 mn = vm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_e:
               }
  var ia:int = em;
  var ne:int = ia + gj;
  em = ne;
  var ja:int = ji;
  var nk:int = ja ^ -1;
  var mg:int = nk & 240;
  var ka:int = ji;
  var ng:int = ka & -241;
  var sl:int = mg | ng;
  var ym:int = sl - 150;
  var ij:int = {
                 mn = ym;
                 select_if(0 - mn, mn, mn < 0);
                 label B_f:
               }
  var la:int = em;
  var oe:int = la + ij;
  em = oe;
  var ma:int = ki;
  var ok:int = ma ^ -1;
  var pg:int = ok & 65;
  var na:int = ki;
  var qg:int = na & -66;
  var tl:int = pg | qg;
  var zm:int = tl - 52;
  var jj:int = {
                 mn = zm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_g:
               }
  var oa:int = em;
  var pe:int = oa + jj;
  em = pe;
  var pa:int = li;
  var pk:int = pa ^ -1;
  var rg:int = pk & 105;
  var qa:int = li;
  var sg:int = qa & -106;
  var ul:int = rg | sg;
  var an:int = ul - 27;
  var kj:int = {
                 mn = an;
                 select_if(0 - mn, mn, mn < 0);
                 label B_h:
               }
  var sa:int = em;
  var qe:int = sa + kj;
  em = qe;
  var ta:int = mi;
  var qk:int = ta ^ -1;
  var ug:int = qk & 49;
  var ua:int = mi;
  var vg:int = ua & -50;
  var vl:int = ug | vg;
  var bn:int = vl - 69;
  var lj:int = {
                 mn = bn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_i:
               }
  var va:int = em;
  var re:int = va + lj;
  em = re;
  var wa:int = ni;
  var rk:int = wa ^ -1;
  var wg:int = rk & 119;
  var xa:int = ni;
  var xg:int = xa & -120;
  var xl:int = wg | xg;
  var cn:int = xl - 31;
  var mj:int = {
                 mn = cn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_j:
               }
  var ya:int = em;
  var se:int = ya + mj;
  em = se;
  var za:int = oi;
  var sk:int = za ^ -1;
  var yg:int = sk & 12;
  var ab:int = oi;
  var zg:int = ab & -13;
  var yl:int = yg | zg;
  var en:int = yl - 63;
  var nj:int = {
                 mn = en;
                 select_if(0 - mn, mn, mn < 0);
                 label B_k:
               }
  var bb:int = em;
  var te:int = bb + nj;
  em = te;
  var db:int = mh;
  var tk:int = db ^ -1;
  var ah:int = tk & 210;
  var eb:int = mh;
  var bh:int = eb & -211;
  var zl:int = ah | bh;
  var fn:int = zl - 160;
  var pj:int = {
                 mn = fn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_l:
               }
  var fb:int = em;
  var ue:int = fb + pj;
  em = ue;
  var gb:int = nh;
  var uk:int = gb ^ -1;
  var ch:int = uk & 108;
  var hb:int = nh;
  var dh:int = hb & -109;
  var am:int = ch | dh;
  var gn:int = am - 51;
  var qj:int = {
                 mn = gn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_m:
               }
  var ib:int = em;
  var ve:int = ib + qj;
  em = ve;
  var jb:int = oh;
  var vk:int = jb ^ -1;
  var eh:int = vk & 184;
  var kb:int = oh;
  var fh:int = kb & -185;
  var bm:int = eh | fh;
  var hn:int = bm - 137;
  var rj:int = {
                 mn = hn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_n:
               }
  var lb:int = em;
  var xe:int = lb + rj;
  em = xe;
  var mb:int = ph;
  var wk:int = mb ^ -1;
  var gh:int = wk & 235;
  var ob:int = ph;
  var hh:int = ob & -236;
  var cm:int = gh | hh;
  var in:int = cm - 133;
  var sj:int = {
                 mn = in;
                 select_if(0 - mn, mn, mn < 0);
                 label B_o:
               }
  var pb:int = em;
  var ye:int = pb + sj;
  em = ye;
  var qb:int = qh;
  var yk:int = qb ^ -1;
  var ih:int = yk & 118;
  var rb:int = qh;
  var jh:int = rb & -119;
  var dm:int = ih | jh;
  var jn:int = dm - 16;
  var tj:int = {
                 mn = jn;
                 select_if(0 - mn, mn, mn < 0);
                 label B_p:
               }
  var sb:int = em;
  var ze:int = sb + tj;
  em = ze;
  var tb:int = rh;
  var vj:int = tb ^ -1;
  var df:int = vj & 52;
  var ub:int = rh;
  var ef:int = ub & -53;
  var al:int = df | ef;
  var gm:int = al - 4;
  var qi:int = {
                 mn = gm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_q:
               }
  var vb:int = em;
  var wd:int = vb + qi;
  em = wd;
  var wb:int = sh;
  var wj:int = wb ^ -1;
  var ff:int = wj & 213;
  var xb:int = sh;
  var gf:int = xb & -214;
  var bl:int = ff | gf;
  var hm:int = bl - 138;
  var ri:int = {
                 mn = hm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_r:
               }
  var zb:int = em;
  var xd:int = zb + ri;
  em = xd;
  var ac:int = th;
  var xj:int = ac ^ -1;
  var if:int = xj & 5;
  var bc:int = th;
  var jf:int = bc & -6;
  var cl:int = if | jf;
  var im:int = cl - 102;
  var si:int = {
                 mn = im;
                 select_if(0 - mn, mn, mn < 0);
                 label B_s:
               }
  var cc:int = em;
  var yd:int = cc + si;
  em = yd;
  var dc:int = uh;
  var yj:int = dc ^ -1;
  var kf:int = yj & 107;
  var ec:int = uh;
  var lf:int = ec & -108;
  var el:int = kf | lf;
  var jm:int = el - 95;
  var ti:int = {
                 mn = jm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_t:
               }
  var fc:int = em;
  var zd:int = fc + ti;
  em = zd;
  var gc:int = vh;
  var zj:int = gc ^ -1;
  var mf:int = zj & 106;
  var hc:int = vh;
  var nf:int = hc & -107;
  var fl:int = mf | nf;
  var lm:int = fl - 91;
  var ui:int = {
                 mn = lm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_u:
               }
  var ic:int = em;
  var ae:int = ic + ui;
  em = ae;
  var kc:int = xh;
  var ak:int = kc ^ -1;
  var of:int = ak & 165;
  var lc:int = xh;
  var pf:int = lc & -166;
  var gl:int = of | pf;
  var mm:int = gl - 148;
  var wi:int = {
                 mn = mm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_v:
               }
  var mc:int = em;
  var be:int = mc + wi;
  em = be;
  var nc:int = yh;
  var bk:int = nc ^ -1;
  var qf:int = bk & 12;
  var oc:int = yh;
  var rf:int = oc & -13;
  var hl:int = qf | rf;
  var nm:int = hl - 83;
  var xi:int = {
                 mn = nm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_w:
               }
  var pc:int = em;
  var ce:int = pc + xi;
  em = ce;
  var qc:int = zh;
  var ck:int = qc ^ -1;
  var sf:int = ck & 182;
  var rc:int = zh;
  var tf:int = rc & -183;
  var il:int = sf | tf;
  var om:int = il - 134;
  var yi:int = {
                 mn = om;
                 select_if(0 - mn, mn, mn < 0);
                 label B_x:
               }
  var sc:int = em;
  var ee:int = sc + yi;
  em = ee;
  var tc:int = ai;
  var dk:int = tc ^ -1;
  var uf:int = dk & 220;
  var vc:int = ai;
  var vf:int = vc & -221;
  var jl:int = uf | vf;
  var pm:int = jl - 239;
  var zi:int = {
                 mn = pm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_y:
               }
  var wc:int = em;
  var fe:int = wc + zi;
  em = fe;
  var xc:int = bi;
  var fk:int = xc ^ -1;
  var wf:int = fk & 140;
  var yc:int = bi;
  var xf:int = yc & -141;
  var kl:int = wf | xf;
  var qm:int = kl - 191;
  var aj:int = {
                 mn = qm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_z:
               }
  var zc:int = em;
  var ge:int = zc + aj;
  em = ge;
  var ad:int = ci;
  var gk:int = ad ^ -1;
  var zf:int = gk & 149;
  var bd:int = ci;
  var ag:int = bd & -150;
  var ll:int = zf | ag;
  var rm:int = ll - 162;
  var bj:int = {
                 mn = rm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_aa:
               }
  var cd:int = em;
  var he:int = cd + bj;
  em = he;
  var dd:int = di;
  var hk:int = dd ^ -1;
  var bg:int = hk & 18;
  var ed:int = di;
  var cg:int = ed & -19;
  var ml:int = bg | cg;
  var sm:int = ml - 32;
  var cj:int = {
                 mn = sm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_ba:
               }
  var gd:int = em;
  var ie:int = gd + cj;
  em = ie;
  var hd:int = ei;
  var ik:int = hd ^ -1;
  var eg:int = ik & 168;
  var id:int = ei;
  var fg:int = id & -169;
  var nl:int = eg | fg;
  var tm:int = nl - 144;
  var dj:int = {
                 mn = tm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_ca:
               }
  var jd:int = em;
  var je:int = jd + dj;
  em = je;
  var kd:int = fi;
  var jk:int = kd ^ -1;
  var gg:int = jk & 234;
  var ld:int = fi;
  var hg:int = ld & -235;
  var pl:int = gg | hg;
  var um:int = pl - 219;
  var ej:int = {
                 mn = um;
                 select_if(0 - mn, mn, mn < 0);
                 label B_da:
               }
  var md:int = em;
  var ke:int = md + ej;
  em = ke;
  var nd:int = gi;
  var kk:int = nd ^ -1;
  var ig:int = kk & 173;
  var od:int = gi;
  var jg:int = od & -174;
  var ql:int = ig | jg;
  var wm:int = ql - 149;
  var fj:int = {
                 mn = wm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_ea:
               }
  var pd:int = em;
  var le:int = pd + fj;
  em = le;
  var rd:int = ii;
  var mk:int = rd ^ -1;
  var kg:int = mk & 39;
  var sd:int = ii;
  var lg:int = sd & -40;
  var rl:int = kg | lg;
  var xm:int = rl - 18;
  var hj:int = {
                 mn = xm;
                 select_if(0 - mn, mn, mn < 0);
                 label B_fa:
               }
  var td:int = em;
  var me:int = td + hj;
  em = me;
  var ud:int = em;
  g_c = ln;
  return ud;
}


```

</details>

We're getting there, but took the wrong turn. Ghidra would do a much better job at explaining what's going on. So, we just need to compile the program (full instructions can be found [here](https://github.com/WebAssembly/wabt/blob/main/wasm2c/README.md), but it's enough to build an object file):

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ cp /home/user/utils/web/wabt/wasm2c/wasm-rt.h .

┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ gcc -c  script.c -o  script.o

```

Then open the file in Ghidra and finally see something we can work with:

```c
int w2c__ver(uint param_1,uint param_2,uint param_3,uint param_4,uint param_5,uint param_6,
            uint param_7,uint param_8,uint param_9,uint param_10,uint param_11,uint param_12,
            uint param_13,uint param_14,uint param_15,uint param_16,uint param_17,uint param_18,
            uint param_19,uint param_20,uint param_21,uint param_22,uint param_23,uint param_24,
            uint param_25,uint param_26,uint param_27,uint param_28,uint param_29,uint param_30,
            uint param_31)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int iVar13;
  int iVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  int iVar19;
  int iVar20;
  int iVar21;
  int iVar22;
  int iVar23;
  int iVar24;
  int iVar25;
  int iVar26;
  int iVar27;
  int iVar28;
  int iVar29;
  int iVar30;
  int iVar31;
  int iVar32;
  
  wasm_rt_call_stack_depth = wasm_rt_call_stack_depth + 1;
  if (500 < wasm_rt_call_stack_depth) {
    wasm_rt_trap(7);
  }
  iVar1 = w2c_g2;
  w2c_g2 = w2c_g2 + 0x80;
  if (w2c_g3 <= w2c_g2) {
    (*Z_envZ_abortStackOverflowZ_vi)(0x80);
  }
  iVar2 = ((param_1 ^ 0xffffffff) & 0xa7 | param_1 & 0xffffff58) - 0xc1;
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  iVar3 = ((param_2 ^ 0xffffffff) & 0x10 | param_2 & 0xffffffef) - 0x20;
  if (iVar3 < 0) {
    iVar3 = -iVar3;
  }
  iVar4 = ((param_3 ^ 0xffffffff) & 0xf0 | param_3 & 0xffffff0f) - 0x82;
  if (iVar4 < 0) {
    iVar4 = -iVar4;
  }
  iVar5 = ((param_4 ^ 0xffffffff) & 0x9f | param_4 & 0xffffff60) - 0xc0;
  if (iVar5 < 0) {
    iVar5 = -iVar5;
  }
  iVar6 = ((param_5 ^ 0xffffffff) & 0xf0 | param_5 & 0xffffff0f) - 0x96;
  if (iVar6 < 0) {
    iVar6 = -iVar6;
  }
  iVar7 = ((param_6 ^ 0xffffffff) & 0x41 | param_6 & 0xffffffbe) - 0x34;
  if (iVar7 < 0) {
    iVar7 = -iVar7;
  }
  iVar8 = ((param_7 ^ 0xffffffff) & 0x69 | param_7 & 0xffffff96) - 0x1b;
  if (iVar8 < 0) {
    iVar8 = -iVar8;
  }
  iVar9 = ((param_8 ^ 0xffffffff) & 0x31 | param_8 & 0xffffffce) - 0x45;
  if (iVar9 < 0) {
    iVar9 = -iVar9;
  }
  iVar10 = ((param_9 ^ 0xffffffff) & 0x77 | param_9 & 0xffffff88) - 0x1f;
  if (iVar10 < 0) {
    iVar10 = -iVar10;
  }
  iVar11 = ((param_10 ^ 0xffffffff) & 0xc | param_10 & 0xfffffff3) - 0x3f;
  if (iVar11 < 0) {
    iVar11 = -iVar11;
  }
  iVar12 = ((param_11 ^ 0xffffffff) & 0xd2 | param_11 & 0xffffff2d) - 0xa0;
  if (iVar12 < 0) {
    iVar12 = -iVar12;
  }
  iVar13 = ((param_12 ^ 0xffffffff) & 0x6c | param_12 & 0xffffff93) - 0x33;
  if (iVar13 < 0) {
    iVar13 = -iVar13;
  }
  iVar14 = ((param_13 ^ 0xffffffff) & 0xb8 | param_13 & 0xffffff47) - 0x89;
  if (iVar14 < 0) {
    iVar14 = -iVar14;
  }
  iVar15 = ((param_14 ^ 0xffffffff) & 0xeb | param_14 & 0xffffff14) - 0x85;
  if (iVar15 < 0) {
    iVar15 = -iVar15;
  }
  iVar16 = ((param_15 ^ 0xffffffff) & 0x76 | param_15 & 0xffffff89) - 0x10;
  if (iVar16 < 0) {
    iVar16 = -iVar16;
  }
  iVar17 = ((param_16 ^ 0xffffffff) & 0x34 | param_16 & 0xffffffcb) - 4;
  if (iVar17 < 0) {
    iVar17 = -iVar17;
  }
  iVar18 = ((param_17 ^ 0xffffffff) & 0xd5 | param_17 & 0xffffff2a) - 0x8a;
  if (iVar18 < 0) {
    iVar18 = -iVar18;
  }
  iVar19 = ((param_18 ^ 0xffffffff) & 5 | param_18 & 0xfffffffa) - 0x66;
  if (iVar19 < 0) {
    iVar19 = -iVar19;
  }
  iVar20 = ((param_19 ^ 0xffffffff) & 0x6b | param_19 & 0xffffff94) - 0x5f;
  if (iVar20 < 0) {
    iVar20 = -iVar20;
  }
  iVar21 = ((param_20 ^ 0xffffffff) & 0x6a | param_20 & 0xffffff95) - 0x5b;
  if (iVar21 < 0) {
    iVar21 = -iVar21;
  }
  iVar22 = ((param_21 ^ 0xffffffff) & 0xa5 | param_21 & 0xffffff5a) - 0x94;
  if (iVar22 < 0) {
    iVar22 = -iVar22;
  }
  iVar23 = ((param_22 ^ 0xffffffff) & 0xc | param_22 & 0xfffffff3) - 0x53;
  if (iVar23 < 0) {
    iVar23 = -iVar23;
  }
  iVar24 = ((param_23 ^ 0xffffffff) & 0xb6 | param_23 & 0xffffff49) - 0x86;
  if (iVar24 < 0) {
    iVar24 = -iVar24;
  }
  iVar25 = ((param_24 ^ 0xffffffff) & 0xdc | param_24 & 0xffffff23) - 0xef;
  if (iVar25 < 0) {
    iVar25 = -iVar25;
  }
  iVar26 = ((param_25 ^ 0xffffffff) & 0x8c | param_25 & 0xffffff73) - 0xbf;
  if (iVar26 < 0) {
    iVar26 = -iVar26;
  }
  iVar27 = ((param_26 ^ 0xffffffff) & 0x95 | param_26 & 0xffffff6a) - 0xa2;
  if (iVar27 < 0) {
    iVar27 = -iVar27;
  }
  iVar28 = ((param_27 ^ 0xffffffff) & 0x12 | param_27 & 0xffffffed) - 0x20;
  if (iVar28 < 0) {
    iVar28 = -iVar28;
  }
  iVar29 = ((param_28 ^ 0xffffffff) & 0xa8 | param_28 & 0xffffff57) - 0x90;
  if (iVar29 < 0) {
    iVar29 = -iVar29;
  }
  iVar30 = ((param_29 ^ 0xffffffff) & 0xea | param_29 & 0xffffff15) - 0xdb;
  if (iVar30 < 0) {
    iVar30 = -iVar30;
  }
  iVar31 = ((param_30 ^ 0xffffffff) & 0xad | param_30 & 0xffffff52) - 0x95;
  if (iVar31 < 0) {
    iVar31 = -iVar31;
  }
  iVar32 = ((param_31 ^ 0xffffffff) & 0x27 | param_31 & 0xffffffd8) - 0x12;
  if (iVar32 < 0) {
    iVar32 = -iVar32;
  }
  w2c_g2 = iVar1;
  wasm_rt_call_stack_depth = wasm_rt_call_stack_depth - 1;
  return iVar2 + iVar3 + iVar4 + iVar5 + iVar6 + iVar7 + iVar8 + iVar9 + iVar10 + iVar11 + iVar12 +
         iVar13 + iVar14 + iVar15 + iVar16 + iVar17 + iVar18 + iVar19 + iVar20 + iVar21 + iVar22 +
         iVar23 + iVar24 + iVar25 + iVar26 + iVar27 + iVar28 + iVar29 + iVar30 + iVar31 + iVar32;
}
```

Easy as pie, we just had to convert the WASM to WAT, then to C, then to assembly and back to C for this.

Anyway, looks like each character is being tested on its own. The sum of all characters needs to be zero, and each character is ensured to be positive. So, assuming we're not aiming for a wraparound, each character on its own should produce a zero after being manipulated.

We *could* throw this into Z3 but it seems simple enough to just brute-force, and we do have a long way ahead of us:

```python
import string
import re

pattern = re.compile(r"\(\(.* \^ (.*)\) \& (.*) \| .* \& (.*)\) \- (.*)\;")

with open("ghidra.c") as f:
    result = ""
    groups = pattern.findall(f.read())
    for group in groups:
        for c in string.printable:
            if ((ord(c) ^ int(group[0], 16)) & int(group[1], 16) | ord(c) & int(group[2], 16)) - int(group[3], 16) == 0:
                result += c
    print(result)
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/cyberark/Finally_-_home]
└─$ python3 solve.py
f0r_furth3r_1nf0_c411_033728185
```

Great! We found the input that is accepted by the form. However, it isn't accepted as the flag. Should we really call the number?

On the other side of the phone we are greeted with the following message:

```
Countdown for landing. 10 C B A 9 8 7 6 5 4 3 2 1 0. Successful landing. Great job cadet. We understand you've found a message from extraterrestrial life. Please, send that important flag to us.
```

Well, in the second stage ("The search for a higher being") we decrypted an extraterrestrial message, and the third stage's description claimed that "while drinking your victory coffee after you found an alien message, some coffee spilled on one piece of the spaceship's equipment!". So, they must want the flag from the second stage: "What a lovely day". Unfortunately, it isn't accepted as the flag. Maybe the base64-encoded form? Or the incorrectly-rust-decrypted form? Or is it maybe the "on va le chercher toute la journée!!!!!!!!" howl found in the rust code, encrypted/encoded/translated from French? Unfortunately, those didn't work either. 

We are ignoring the elephant in the room though. What's that "C B A" stuck in the middle of the countdown? It must be a clue. And since the simplest explanation is usually the best one, we must deduce that if someone is counting down like that, they must be counting down in base 13.

So, we grab a random [base13 encoder](https://github.com/alanhett/ace/blob/master/ace.js), modify it a bit to suit our needs and get the following code:

```javascript
>>> function base13_encode(s) {
        a = '0123456789ABC';
        return s.split('').map(i => {
            i = i.charCodeAt(0).toString(13);
            return a[parseInt(i[0], 13)] + a[parseInt(i[1], 13)];
        }).join('');
    }

>>> base13_encode("What a lovely day");
    "6980768C2676268487917A849426797694"
```
Finally, `"6980768C2676268487917A849426797694"` was accepted as the flag and we've completed our journey home.

