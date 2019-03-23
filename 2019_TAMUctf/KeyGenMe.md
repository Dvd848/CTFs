# KeyGenMe
Reversing, 423 points

## Description:

A binary file was attached, in addition to an address and port.

## Solution:

Let's run the binary:

```console
root@kali:/media/sf_CTFs/tamu/KeyGenMe# ./keygenme

Please Enter a product key to continue:
```

So we'll need a valid product key in order to proceed.

Let's open in a disassembler:

```console
root@kali:/media/sf_CTFs/tamu/KeyGenMe# r2 keygenme
 -- Finnished a beer
[0x00000820]> aaa
[x] Analyze all flags starting with sym. and entry0 (aa)
[x] Analyze function calls (aac)
[x] Analyze len bytes of instructions for references (aar)
[x] Constructing a function name for fcn.* and sym.func.* functions (aan)
[x] Type matching analysis for all functions (afta)
[x] Use -AA or aaaa to perform additional experimental analysis.
[0x00000820]> afl
0x00000000   10 459  -> 461  sym.imp.__libc_start_main
0x00000750    3 23           sym._init
0x00000780    1 6            sym.imp.puts
0x00000790    1 6            sym.imp.strlen
0x000007a0    1 6            sym.imp.__stack_chk_fail
0x000007b0    1 6            sym.imp.printf
0x000007c0    1 6            sym.imp.fgets
0x000007d0    1 6            sym.imp.strcmp
0x000007e0    1 6            sym.imp.malloc
0x000007f0    1 6            sym.imp.setvbuf
0x00000800    1 6            sym.imp.fopen
0x00000810    1 6            sub.__cxa_finalize_810
0x00000820    1 43           entry0
0x00000850    4 50   -> 40   sym.deregister_tm_clones
0x00000890    4 66   -> 57   sym.register_tm_clones
0x000008e0    5 58   -> 51   sym.__do_global_dtors_aux
0x00000920    1 10           entry1.init
0x0000092a    4 176          sym.enc
0x000009da    5 108          sym.verify_key
0x00000a46    8 244          main
0x00000b40    3 101  -> 92   sym.__libc_csu_init
0x00000bb0    1 2            sym.__libc_csu_fini
0x00000bb4    1 9            sym._fini
[0x00000820]> s sym.main
```

An overview of `main` is:
```

                                                                .---------------------------------------------------------.
                                                                | [0xa46]                                                 |
                                                                | 0x00000a67 size_t size                                  |
                                                                | 0x00000a6c int mode                                     |
                                                                | 0x00000a71 char *buf                                    |
                                                                | 0x00000a76 FILE*stream                                  |
                                                                | 0x00000a79 call sym.imp.setvbuf                         |
                                                                | 0x00000000 [28] ---- section size 254 named .shstrtab   |
                                                                | 0x00000a7e const char *s                                |
                                                                | 0x00000a7e str.Please_Enter_a_product_key_to_continue:  |
                                                                | 0x00000a85 call sym.imp.puts                            |
                                                                | 0x00000a8a FILE *stream                                 |
                                                                | 0x00000a98 int size                                     |
                                                                | 0x00000a9d char *s                                      |
                                                                | 0x00000aa0 call sym.imp.fgets                           |
                                                                | 0x00000aaf call sym.verify_key                          |
                                                                `---------------------------------------------------------'
                                                                        f t
                                                                        | |
                                                                        | '------------------------------------------.
                                                        .---------------'                                            |
                                                        |                                                            |
                                                    .----------------------------------.                             |
                                                    |  0xab8 [gi]                      |                             |
                                                    | 0x00000ab8 const char *mode      |                             |
                                                    | 0x00000abf const char *filename  |                             |
                                                    | 0x00000abf str.flag.txt          |                             |
                                                    | 0x00000ac6 call sym.imp.fopen    |                             |
                                                    `----------------------------------'                             |
                                                            f t                                                      |
                                                            | |                                                      |
                                                            | '--------------------.                                 |
               .--------------------------------------------'                      |                                 |
               |                                                                   |                                 |
           .---------------------------------------------------------------.   .---------------------------------.   |
           |  0xadc [gk]                                                   |   |  0xaef [gh]                     |   |
           | 0x00000adc const char *s                                      |   | 0x00000aef FILE *stream         |   |
           | 0x00000adc str.Too_bad_the_flag_is_only_on_the_remote_server  |   | 0x00000afa int size             |   |
           | 0x00000ae3 call sym.imp.puts                                  |   | 0x00000aff char *s              |   |
           `---------------------------------------------------------------'   | 0x00000b02 call sym.imp.fgets   |   |
               v                                                               | 0x00000b0e const char *format   |   |
               |                                                               | 0x00000b1a call sym.imp.printf  |   |
               |                                                               `---------------------------------'   |
```

So the interesting part looks like `verify_key`:

```


                                                                               .---------------------------------.
                                                                               | [0x9da]                         |
                                                                               | 0x000009e2 arg1                 |
                                                                               | 0x000009ea const char *s        |
                                                                               | 0x000009ed call sym.imp.strlen  |
                                                                               `---------------------------------'
                                                                                       f t
                                                                                       | |
                                                                                       | '-----------------.
                                                                         .-------------'                   |
                                                                         |                                 |
                                                                     .---------------------------------.   |
                                                                     |  0x9f8 [ge]                     |   |
                                                                     | 0x000009fc const char *s        |   |
                                                                     | 0x000009ff call sym.imp.strlen  |   |
                                                                     `---------------------------------'   |
                                                                           t f                             |
                                                                           | |                             |
                                                        .------------------' |                             |
                                                        |                    '--------------------.        |
                                                        |                                         | .------'
                                                        |                                         | |
                                                    .----------------------------------.    .--------------------.
                                                    |  0xa11 [gd]                      |    |  0xa0a [gb]        |
                                                    | 0x00000a18 call sym.enc          |    `--------------------'
                                                    | 0x00000a21 str.OIonU2____nK_KsK  |        v
                                                    | 0x00000a34 const char *s2        |        |
                                                    | 0x00000a37 const char *s1        |        |
                                                    | 0x00000a3a call sym.imp.strcmp   |        |
                                                    `----------------------------------'        |
                                                        v                                       |
                                                        |                                       |
                                                        |                        .--------------'
                                                        '--------------------------.
                                                                                 | |
                                                                           .--------------------.
                                                                           |  0xa44 [gf]        |
                                                                           `--------------------'
```

This function verifies that the string length is between some legal values (9 and 0x40 to be precise), calls `enc()` to encrypt the string and compares it to some known result:

```
[0x000009da]> psz @ str.OIonU2____nK_KsK
[OIonU2_<__nK<KsK
```

So we need to make sure that our input, after passing through `enc`, will be equal to `[OIonU2_<__nK<KsK`.

What does `enc` do?

```

            .------------------------------------------.
            | [0x92a]                                  |
            | (fcn) sym.enc 176                        |
            |   sym.enc (char *arg1);                  |
            | ; var char *s @ rbp-0x28                 |
            | ; var int last_char @ rbp-0x11           |
            | ; var int i @ rbp-0x10                   |
            | ; var size_t input_len @ rbp-0xc         |
            | ; var void *p_output @ rbp-0x8           |
            | ; arg char *arg1 @ rdi                   |
            | ; CALL XREF from sym.verify_key (0xa18)  |
            | push rbp                                 |
            | mov rbp, rsp                             |
            | ; '0'                                    |
            | sub rsp, 0x30                            |
            | ; arg1                                   |
            | mov qword [s], rdi                       |
            | ; size_t size                            |
            | ; segment.PHDR                           |
            | mov edi, 0x40                            |
            | ; void *malloc(size_t size)              |
            | call sym.imp.malloc;[ga]                 |
            | mov qword [p_output], rax                |
            | mov rax, qword [s]                       |
            | ; const char *s                          |
            | mov rdi, rax                             |
            | ; size_t strlen(const char *s)           |
            | call sym.imp.strlen;[gb]                 |
            | mov dword [input_len], eax               |
            | ; 'H'                                    |
            | mov byte [last_char], 0x48               |
            | mov dword [i], 0                         |
            | jmp 0x9cc;[gc]                           |
            `------------------------------------------'
                v
                |
                '-----.
 .----------------------.
 |                    | |
 |              .-----------------------------------.
 |              |  0x9cc [gc]                       |
 |              | ; CODE XREF from sym.enc (0x95e)  |
 |              | mov eax, dword [i]                |
 |              | cmp eax, dword [input_len]        |
 |              | jl 0x960;[ge]                     |
 |              `-----------------------------------'
 |                    t f
 |                    | |
 |    .---------------' |
 |    |                 '-----------------------.
 |    |                                         |
 |.------------------------------------.    .---------------------------------.
 ||  0x960 [ge]                        |    |  0x9d4 [gf]                     |
 || ; CODE XREF from sym.enc (0x9d2)   |    | mov rax, qword [p_output]       |
 || mov eax, dword [i]                 |    | leave                           |
 || movsxd rdx, eax                    |    | ret                             |
 || mov rax, qword [s]                 |    `---------------------------------'
 || ; '('                              |
 || add rax, rdx                       |
 || movzx eax, byte [rax]              |
 || movsx eax, al                      |
 || lea edx, [rax + 0xc]               |
 || movzx eax, byte [last_char]        |
 || imul eax, edx                      |
 || lea ecx, [rax + 0x11]              |
 || mov edx, 0xea0ea0eb                |
 || mov eax, ecx                       |
 || imul edx                           |
 || lea eax, [rdx + rcx]               |
 || sar eax, 6                         |
 || mov edx, eax                       |
 || mov eax, ecx                       |
 || sar eax, 0x1f                      |
 || sub edx, eax                       |
 || mov eax, edx                       |
 || imul eax, eax, 0x46                |
 || sub ecx, eax                       |
 || mov eax, ecx                       |
 || ; '0'                              | 
 || lea ecx, [rax + 0x30]              |
 || mov eax, dword [i]                 |
 || movsxd rdx, eax                    |
 || mov rax, qword [p_output]          |
 || ; '('                              |
 || add rax, rdx                       |
 || mov edx, ecx                       |
 || mov byte [rax], dl                 |
 || mov eax, dword [i]                 |
 || movsxd rdx, eax                    |
 || mov rax, qword [p_output]          |
 || ; '('                              |
 || add rax, rdx                       |
 || movzx eax, byte [rax]              |
 || mov byte [last_char], al           |
 || add dword [i], 1                   |
 |`------------------------------------'
 |    v
 |    |
 `----' 
```

We can see that it allocates an output buffer for the result, and that it iterates over each character of the input and performs multiple manipulation in order to arrive to an output character. The manipulations, performed in the `[ge]` block, don't seem trivial to reverse, and as we can see - they include some relation to the previous character (stored in `last_char`). It would probably be easier to brute-force this index-by-index.

In order to do so, we can choose to extract/reimplement the encryption function in a different program, or peek into the existing one during runtime. I chose the latter since it's less error-prone (and much cooler), making use of `ltrace` for the task.

From the Linux man page:
> ltrace is a program that simply runs the specified command until it exits.  It intercepts and records the dynamic library calls which are called by the executed process and the signals which are received by that process.  It can also intercept and print the system calls executed by the program.

For example, if we call `ltrace` on the program using a random key, we see:

```console
root@kali:/media/sf_CTFs/tamu/KeyGenMe# echo abcdefghij | ltrace ./keygenme
setvbuf(0x7f86fab16760, 0, 2, 0)                                                                                  = 0
puts("\nPlease Enter a product key to c"...
Please Enter a product key to continue:
)                                                                      = 42
fgets("abcdefghij\n", 65, 0x7f86fab15a00)                                                                         = 0x7ffc75e42fa0
strlen("abcdefghij\n")                                                                                            = 11
strlen("abcdefghij\n")                                                                                            = 11
malloc(64)                                                                                                        = 0x55bf58a9c270
strlen("abcdefghij\n")                                                                                            = 11
strcmp("[OIonU2_<__nK<KsK", "IsZA<s<_2Us")                                                                        = 18
+++ exited (status 0) +++
```

The important like is this one:
```
strcmp("[OIonU2_<__nK<KsK", "IsZA<s<_2Us")  
```

We see that the program compared the hardcoded string `[OIonU2_<__nK<KsK` to `IsZA<s<_2Us`, meaning that `IsZA<s<_2Us` is the output of `enc("abcdefghij")`.

The expected result is 17 characters long, and we see that an input of length n produces a result of length (n+1), so we conclude that we need to enter an input of length 16.

Our strategy is as follows:
1. Input a string of length 16, where the first letter is the first printable letter of the ASCII table. The range of printable letters in the ASCII table is 33 to 126, i.e. `!` to `~`.
2. Peek into the program using `ltrace` and see what the input was encrypted to.
3. If the first letter of the encrypted result isn't `[`, continue to the next letter.
4. Once we have a match for the first letter, set it as the first letter of the key and move on to the next index.

I chose to practice my bash skills and implement the whole thing as a bash script:

```bash
#!/bin/bash


target='[OIonU2_<__nK<KsK'
echo "Target: $target"

# Key length is len(target)-1
key_len=$((${#target}-1))

# Create a dummy key of length 16: "################"
key=$(for i in `seq 0 $(($key_len-1))`; do printf "#"; done; echo "")

# For each index in the key
for i in `seq 0 $(($key_len-1))`;
do
    # Extract the character at index i from the target
    target_char=$(echo "${target:$i:1}")

    # For each printable character in the ASCII table
    for j in `seq 33 127`;
    do
        #Turn the decimal value of the character into a real ASCII character
        replacement=$(printf "$(printf %x $j)" | xxd -p -r)
        
        # The new key is key[:i] + replacement + key[i+1:]
        key=$(echo "${key:0:$i}$replacement${key:$(($i+1))}")
        
        # Run the program through ltrace, using the key, and extract the output of enc() using regex.
        # grep's "\K" is a lookbehind alternative, telling the engine to start capturing only from the \K itself.
        # -Po tells grep to work with Perl-compatible expressions (P) and to and to print only the matched (non-empty) parts of a matching line (o).
        res=$(echo $key | ltrace ./keygenme 2>&1 | grep -Po "strcmp\(\".{${#target}}\", \"\K.{$key_len}")
        
        # Extract the character at index i from the output of enc()
        res_char=$(echo "${res:$i:1}")
        
        # Check if they match
        if  [ "$res_char" = "$target_char" ]
        then
            break
        fi
    done
    echo "Key so far: $key"
done

echo "Key:"
echo $key

echo $key | nc rev.tamuctf.com 7223
```

The output:
```console
root@kali:/media/sf_CTFs/tamu/_KeyGenMe# ./run.sh
Target: [OIonU2_<__nK<KsK
Key so far: $###############
Key so far: $*##############
Key so far: $*Z#############
Key so far: $*Z2############
Key so far: $*Z2S###########
Key so far: $*Z2S"##########
Key so far: $*Z2S"+#########
Key so far: $*Z2S"+'########
Key so far: $*Z2S"+')#######
Key so far: $*Z2S"+')"######
Key so far: $*Z2S"+')""#####
Key so far: $*Z2S"+')""+####
Key so far: $*Z2S"+')""+'###
Key so far: $*Z2S"+')""+'+##
Key so far: $*Z2S"+')""+'+$#
Key so far: $*Z2S"+')""+'+$(
Key:
$*Z2S"+')""+'+$(

Please Enter a product key to continue:
gigem{k3y63n_m3?_k3y63n_y0u!}
```

