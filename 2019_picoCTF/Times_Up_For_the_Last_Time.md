# Time's Up, For the Last Time!
Reverse Engineering, 500 points

## Description:
> You've solved things fast. You've solved things faster! Now do the impossible.


## Solution: 

This is the follow-up for [Time's Up, Again!](Times_Up_Again.md).

Let's run the attached file:

```console
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# ./times-up-one-last-time
Challenge: (((((-1584302183) + (-971218067)) / ((1583603162) + (-545554333))) * (((1232213110) % (196087411)) t (((-1081550733) - (-182471578)) / ((-661669944) ^ (-1055552917))))) ^ ((((663349295) o (-1576825584)) r ((430910707) + (-1708745368))) * (((-169273934) % ((-1532525736) o (1490257609))) x ((-1897158128) ^ (-1518431513)))))
Setting alarm...
Solution? Alarm clock
```

We got a mathematical expression (with new and weird operators), and shortly after, the program got killed by an alarm. How long is the alarm? Let's use `strace` to see:
```console
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# strace ./times-up-one-last-time  2>&1 | grep setitimer
setitimer(ITIMER_REAL, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=10}}, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=0}}) = 0
```

10 uSeconds? There's nothing really we can do in such a short period. There must be a workaround.

Searching for methods to disable an `alarm()`, a repository named [preeny](https://github.com/zardus/preeny) surfaces.

> Preeny helps you pwn noobs by making it easier to interact with services locally. It disables fork(), rand(), and alarm() and, if you want, can convert a server application to a console one using clever/hackish tricks, and can even patch binaries!

This solution creates dummy libraries which can replace the standard libraries using the `LD_PRELOAD` environment variable. When this variable is set to the path of a shared object, that file will be loaded before any other library (including the C runtime, `libc.so`). Therefore, it's possible to replace the functionality of any C library function. 

Let's try it locally. Since our program uses `ualarm` and not `alarm`, I had to implement another function in `preeny/src/dealarm.c` (using `LD_DEBUG=all` was very helpful in identifying this):

```console
root@kali:~/utils/preeny# cat src/dealarm.c
#include "logging.h"
#include <unistd.h>

unsigned int alarm(unsigned int seconds)
{
        preeny_info("alarm blocked\n");
        return 0;
}

useconds_t ualarm(useconds_t usecs, useconds_t interval)
{
        preeny_info("ualarm blocked\n");
        return 0;
}
```

Now we can use `LD_PRELOAD` and the alarm won't trigger:
```console
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so ./times-up-one-last-time
Challenge: ((((((-1300698887) ^ (-949765080)) r (1864106005)) - ((-1324978037) | (573959822))) * (((107131526) o (62906438)) x ((1530078919) % ((1826216697) t (-1912045987))))) o ((((-1157469812) - (1887024561)) t ((-896795094) / (-521129896))) * (((-1434685180) + (-1436379568)) r ((-698190073) * (458457333)))))
Setting alarm...
Solution? 1234
Nope!
```

Unfortunately, this won't work on the remote server, since the program has `setgid` attribute set, allowing it to elevate privileges when needed (e.g. when it wants to read the flag).

```console
dvdalt@pico-2019-shell1:/problems/time-s-up--for-the-last-time-_1_a7830af9d51a361ee5d3b9eece69c22f$ ls -al
total 92
drwxr-xr-x   2 root       root                             4096 Sep 28 21:53 .
drwxr-x--x 684 root       root                            69632 Oct 10 18:02 ..
-r--r-----   1 hacksports time-s-up--for-the-last-time-_1    46 Sep 28 21:52 flag.txt
-rwxr-sr-x   1 hacksports time-s-up--for-the-last-time-_1 10224 Sep 28 21:52 times-up-one-last-time
```

> For security purposes, the invoking user is usually prohibited by the system from altering the new process in any way, such as by using ptrace, LD_LIBRARY_PATH or sending signals to it, to exploit the raised privilege, although signals from the terminal will still be accepted. (Source: [Wikipedia](https://en.wikipedia.org/wiki/Setuid))

This includes also `LD_PRELOAD`. However, the article didn't mention **blocking signals**. 

> A signal may be blocked, which means that it will not be delivered until it is later unblocked.  Between the time when it is generated and when it is delivered a signal is said to be pending.
> 
> ...
> 
> A child created via fork(2) inherits a copy of its parent's signal mask; the signal mask is preserved across execve(2). ([Source](http://man7.org/linux/man-pages/man7/signal.7.html))

So we should be able to reuse our `C` program from the previous challenge. This time, before `fork`-ing, we must block the `SIGALRM` signal as well.

This brings us to our next problem: How do we solve the new expression and what are all these new operators? 

It's time to dive into Ghidra's disassembly:

```c
undefined8 main(void)
{
  init_random();
  printf("Challenge: ");
  create_expression();
  putchar(10);
  fflush(stdout);
  puts("Setting alarm...");
  fflush(stdout);
  ualarm(10,0);
  printf("Solution? ");
  __isoc99_scanf(&g_format,&g_user_input);
  if (g_user_input == g_expected_result) {
    puts("Congrats! Here is the flag!");
    system("/bin/cat flag.txt");
  }
  else {
    puts("Nope!");
  }
  return 0;
}
```

The interesting function is `create_expression`:
```c
void create_expression(void)
{
  g_expected_result = get_expression(4);
  return;
}

ulong get_expression(uint depth)
{
  undefined8 uVar1;
  ulong uVar2;
  ulong uVar3;
  ulong uVar4;
  
  if (depth == 0) {
    uVar1 = get_random_number();
    uVar2 = SEXT48((int)uVar1);
    printf("(%lld)",uVar2);
  }
  else {
    uVar2 = maybe_decrease_depth(depth);
    uVar3 = maybe_decrease_depth(depth);
    uVar4 = get_random_operator();
    putchar('(');
    uVar2 = get_expression((uint)uVar2);
    printf(" %c ",(ulong)(uint)(int)(char)uVar4);
    uVar3 = get_expression((uint)uVar3);
    putchar(')');
    uVar2 = solve_expression((char)uVar4,uVar2,uVar3);
  }
  return uVar2;
}
```

What we see here is a function that recursively builds an expression using random choices. In parallel, also the expected result is built via calls to `solve_expression`:

```c
ulong solve_expression(undefined param_1,ulong param_2,ulong param_3)

{
  switch(param_1) {
  case 0x25: // %
    if (param_3 != 0) {
      param_2 = (long)param_2 % param_3;
    }
    break;
  case 0x26: // &
    param_2 = param_2 & param_3;
    break;
  default:
                    /* WARNING: Subroutine does not return */
    exit(1);
  case 0x2a: // *
    param_2 = param_2 * param_3;
    break;
  case 0x2b: // +
    param_2 = param_3 + param_2;
    break;
  case 0x2d: // -
    param_2 = param_2 - param_3;
    break;
  case 0x2f: // /
    if (param_3 != 0) {
      param_2 = (long)param_2 / (long)param_3;
    }
    break;
  case 0x5e: // ^
    param_2 = param_2 ^ param_3;
    break;
  case 0x66: // f
    break;
  case 0x6f: // o
    param_2 = param_3;
    break;
  case 0x72: // r
    param_2 = param_3;
    break;
  case 0x74: // t
    break;
  case 0x78: // x
    param_2 = param_3;
    break;
  case 0x7c: // |
    param_2 = param_2 | param_3;
  }
  return param_2;
}
```

This is the function that explains what the new operators do. We should be able to modify our previous solution to account for the new operators, but we'd like to generate a few examples to make sure our implementation is correct.

Let's do that with `GDB`. What we need is the expression (which is printed to `stdout` by the program) and the expected result (which is visible in `create_expression`). Since the executable has `PIE` enabled, we start by calling `set disable-randomization on` to disable address randomization, then step through the code untl we arrive to `create_expression`:

```
gdb-peda$ disas 0x0000555555554e96, 0x555555554ead
Dump of assembler code from 0x555555554e96 to 0x555555554ead:
   0x0000555555554e96:  push   rbp
   0x0000555555554e97:  mov    rbp,rsp
   0x0000555555554e9a:  mov    edi,0x4
   0x0000555555554e9f:  call   0x555555554dce
=> 0x0000555555554ea4:  mov    QWORD PTR [rip+0x2038cd],rax        # 0x555555758778
   0x0000555555554eab:  nop
   0x0000555555554eac:  pop    rbp
End of assembler dump.
```

Our result is the return value of `call   0x555555554dce`, visible in `rax` at address `0x0000555555554ea4`. In order to save the need to repeat the process multiple times, let's rephrase this as a one-liner which will print the expression and the expected result directly from the command line:

```
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so gdb -ex 'set disable-randomization on' -ex 'dprintf *0x555555554ea4, "Result: %lld\n", $rax' -ex 'r <<< "0\n"' -ex 'q' ./test 2>&1 | grep "Result" -A 1
Result: -1077552436
Challenge: (((((1010419654) f (833747938)) % ((1326039599) & (-1838119057))) r (((-645474463) f (-323135453)) o ((-1741026520) + (1399763289)))) - ((((1010473730) | (-10720867)) * ((-830514850) x (-1313980518))) x (((1863694028) % (1127404824)) | (((284466338) + (966994831)) / (660177013)))))
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so gdb -ex 'set disable-randomization on' -ex 'dprintf *0x555555554ea4, "Result: %lld\n", $rax' -ex 'r <<< "0\n"' -ex 'q' ./test 2>&1 | grep "Result" -A 1
Result: -1567918110277765
Challenge: (((((1591946486) r (-1331969196)) o (((-1911788283) o (1371174778)) & (1747890195))) - (((-53926145) * (199210871)) - ((2133037019) * (196902112)))) | ((((842431978) & (481528503)) | ((-1399236960) - (-1419357215))) * (((-1890214325) x (-1261630713)) % ((1427727991) ^ (1213549524)))))
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so gdb -ex 'set disable-randomization on' -ex 'dprintf *0x555555554ea4, "Result: %lld\n", $rax' -ex 'r <<< "0\n"' -ex 'q' ./test 2>&1 | grep "Result" -A 1
Result: -92577501878885090
Challenge: (((((1209766958) * (-1502282569)) & ((29553023) - (-466860704))) * (((1886285652) + (-781523424)) + ((-2092310292) x (-1793126629)))) f ((((-1933428190) * (-1349379277)) r ((-651888835) % (-1712739931))) + ((((-279846789) - (-1521590110)) | ((255665580) t (-1236705863))) - ((1878626940) r (361305662)))))
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so gdb -ex 'set disable-randomization on' -ex 'dprintf *0x555555554ea4, "Result: %lld\n", $rax' -ex 'r <<< "0\n"' -ex 'q' ./test 2>&1 | grep "Result" -A 1
Result: 373430904388490120
Challenge: (((((-1702607148) ^ (-70680992)) + ((-446151739) & (-629732860))) * (((1851976134) / (712844382)) + ((-1376285629) ^ (-1970105546)))) t ((((25694460) / (960535758)) % ((-2069553659) o (-38889948))) / (((2043401392) f (-1555367099)) o ((-900709696) x (-248816132)))))
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# LD_PRELOAD=/root/utils/preeny/x86_64-linux-gnu/dealarm.so gdb -ex 'set disable-randomization on' -ex 'dprintf *0x555555554ea4, "Result: %lld\n", $rax' -ex 'r <<< "0\n"' -ex 'q' ./test 2>&1 | grep "Result" -A 1
Result: -143688065
Challenge: (((((-1044282253) | (1371394754)) + ((1107954027) + (851034211))) + (((-929168316) x (1981424730)) * ((-64515369) t (967367402)))) | ((((-1340669973) + (-579974564)) % ((-49649651) + (-838622239))) f (((75578907) ^ (1899437617)) & ((171087065) f (828726573)))))
```

What we're doing here is call `GDB` with a set of instructions to execute. The important one is `dprintf *0x555555554ea4, "Result: %lld\n", $rax` which means: "When executing the command at `0x555555554ea4`, also print the value of `rax` as a formatted string".

This output allows us to implement and test our new expression parser. Putting all the pieces together, we get:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

// ----------------------------------------------------------------------------------------------------------

//Based on https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
const char* g_expr_to_parse = NULL;

char peek()
{
    while(*g_expr_to_parse == ' ')
    {
        g_expr_to_parse++;
    }
    return *g_expr_to_parse;
}

char get()
{
    while(*g_expr_to_parse == ' ')
    {
        g_expr_to_parse++;
    }
    return *g_expr_to_parse++;
}

int64_t expression();

int64_t number()
{
    int64_t zero = '0';
    int64_t result = get() - zero;

    while (peek() >= '0' && peek() <= '9')
    {
        result = 10 * result + get() - '0';
    }
    return result;
}

int64_t factor()
{
    if (peek() >= '0' && peek() <= '9')
        return number();
    else if (peek() == '(')
    {
        get(); // '('
        int64_t result = expression();
        get(); // ')'
        return result;
    }
    else if (peek() == '-')
    {
        get();
        return -factor();
    }

    perror("factoring error"); 
    exit(EXIT_FAILURE); 
}

bool is_operator(char c)
{
    char operators[] = "+-*/%^|&fortx";
    int i;

    for (i = 0; i < sizeof(operators) - 1; i++)
    {
        if (operators[i] == c)
        {
            return true;
        }
    }
    return false;
}

int64_t solve_expression(char operator, int64_t operand1, int64_t operand2)
{
    switch (operator) 
    {
        case '%':
            if (operand2 != 0) 
            {
                operand1 = operand1 % operand2;
            }
            break;
        case '&':
            operand1 = operand1 & operand2;
            break;
        default:
            perror("Unknown operator");
            exit(EXIT_FAILURE);
        case '*':
            operand1 = operand1 * operand2;
            break;
        case '+':
            operand1 = operand2 + operand1;
            break;
        case '-':
            operand1 = operand1 - operand2;
            break;
        case '/':
            if (operand2 != 0) 
            {
                operand1 = operand1 / operand2;
            }
            break;
        case '^':
            operand1 = operand1 ^ operand2;
            break;
        case 'f':
            break;
        case 'o':
            operand1 = operand2;
            break;
        case 'r':
            operand1 = operand2;
            break;
        case 't':
            break;
        case 'x':
            operand1 = operand2;
            break;
        case '|':
            operand1 = operand1 | operand2;
    }
    return operand1;
}

int64_t expression()
{
    char operator;
    int64_t result = factor();
    int64_t op2;
    while (is_operator(peek()))
    {
        operator = get();
        op2 = factor();
        result = solve_expression(operator, result, op2);
    }
    return result;
}

int64_t parse_expression(const char* p_expr)
{
    g_expr_to_parse = p_expr;
    return expression();
}

// ----------------------------------------------------------------------------------------------------------


#define PIPE_READ 0
#define PIPE_WRITE 1

struct subprocess 
{
    pid_t   pid;
    int     stdin;
    int     stdout;
    int     stderr;
};

void close_fd(int fd) 
{
    if (close(fd) == -1) 
    {
        perror("Could not close pipe end" ); 
        exit(EXIT_FAILURE); 
    }
}

void mk_pipe(int fds[2]) 
{
    if (pipe(fds) == -1) 
    { 
        perror("Could not create pipe"); 
        exit(EXIT_FAILURE); 
    }
}

void mv_fd(int fd1, int fd2) 
{
    if (dup2(fd1,  fd2) == -1) 
    { 
        perror("Could not duplicate pipe end"); 
        exit(EXIT_FAILURE); 
    }
    close_fd(fd1);
}


// Start program at argv[0] with arguments argv.
// Set up new stdin, stdout and stderr.
// Puts references to new process and pipes into `p`.
// https://jameshfisher.com/2017/02/17/how-do-i-call-a-program-in-c-with-pipes/
void call(char* argv[], struct subprocess* p) {
    int child_in[2] = {-1, -1};     // Parent to child
    int child_out[2] = {-1, -1}; ;  // Child to Parent
    int child_err[2] = {-1, -1}; ;  // Child to Parent

    mk_pipe(child_in); 
    mk_pipe(child_out); 
    mk_pipe(child_err);

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        perror("Could not fork"); 
        exit(EXIT_FAILURE); 
    }
    else if (child_pid == 0) 
    {
        //
        // Child
        //

        // Close parent pipes
        close_fd(STDIN_FILENO);
        close_fd(STDOUT_FILENO);
        close_fd(STDERR_FILENO);

        // Close unused child pipe ends
        close_fd(child_in[PIPE_WRITE]); 
        close_fd(child_out[PIPE_READ]);
        close_fd(child_err[PIPE_READ]); 

        mv_fd(child_in[PIPE_READ], STDIN_FILENO); 
        mv_fd(child_out[PIPE_WRITE], STDOUT_FILENO);
        mv_fd(child_err[PIPE_WRITE], STDERR_FILENO);

        // Kill child if parent dies
        prctl(PR_SET_PDEATHSIG, SIGTERM);

        char* envp[] = { NULL };
        execve(argv[0], argv, envp);

        // Shouldn't get here
        perror("Child Error");
    } 
    else 
    {
        //
        // Parent
        //

        // unused child pipe ends
        close_fd(child_in[PIPE_READ]); 
        close_fd(child_out[PIPE_WRITE]); 
        close_fd(child_err[PIPE_WRITE]); 

        p->pid = child_pid;
        p->stdin  = child_in[PIPE_WRITE];  // Parent wants to write to subprocess child_in
        p->stdout = child_out[PIPE_READ];  // Parent wants to read from subprocess child_out
        p->stderr = child_err[PIPE_READ];  // Parent wants to read from subprocess child_err
    }
}

// ----------------------------------------------------------------------------------------------------------

#define BUFFER_LENGTH 512

int main(int argc, char* argv[])
{
    
    struct subprocess   proc;
    ssize_t             nbytes;
    char*               newline_location = NULL;
    size_t              write_len;
    int                 status;
    int64_t             result;
    int                 total_bytes_read = 0;
    sigset_t            intmask;

    // We have three different buffers, allowing us to print the full details at the end
    char                expr_buffer[BUFFER_LENGTH]  = {0};
    char                res_buffer[BUFFER_LENGTH]   = {0};
    char                buffer[BUFFER_LENGTH]       = {0};

    size_t              skip_len = sizeof("Challenge: ") - 1;

    char*               child_argv[] = {"./times-up-one-last-time", NULL};
    
    // Block alarm signal
    if (sigemptyset(&intmask) == -1)
    {
        perror("Failed to initialize signal set");
        exit(EXIT_FAILURE);
    }

    if (sigaddset(&intmask, SIGALRM) == -1)
    {
        perror("Failed to add signal to set");
        exit(EXIT_FAILURE);
    }

    if (sigprocmask(SIG_BLOCK, &intmask, NULL) == -1)
    {
        perror("Failed to block signal set");
        exit(EXIT_FAILURE);
    }

    printf("Alarm blocked!\n");

    // Fork child process
    call(child_argv, &proc);

    // Read expression from child STDOUT
    nbytes = read(proc.stdout, expr_buffer, sizeof(expr_buffer) - 1);
    if (nbytes < 0)
    {
        perror("Could not read from child stdout"); 
        exit(EXIT_FAILURE);
    }

    expr_buffer[nbytes] = '\0';

    // Format expression so that parse_expression() will accept it
    newline_location = strchr(expr_buffer, '\n');
    if (newline_location == NULL)
    {
        perror("Could not find newline"); 
        exit(EXIT_FAILURE);
    }

    *newline_location = '\0';
    
    // Evaluate expression
    result = parse_expression(&expr_buffer[skip_len]);
    write_len = snprintf(res_buffer, sizeof(res_buffer), "%ld\n", result);
    if ( (write_len < 0) || (write_len >= sizeof(res_buffer)) )
    {
        perror("Could not sprintf result"); 
        exit(EXIT_FAILURE);
    }

    // Write answer back to the child processes' STDIN
    nbytes = write(proc.stdin, res_buffer, write_len);
    if (nbytes != write_len)
    {
        perror("Could not write to stdin"); 
        exit(EXIT_FAILURE);
    }
        
    // Read all output from child process STDOUT
    while ((nbytes = read(proc.stdout, buffer + total_bytes_read, sizeof(buffer) - total_bytes_read - 1)) > 0)
    {
        total_bytes_read += nbytes;
    }
    buffer[total_bytes_read] = '\0';

    // Print details
    printf("Expression: %s\n", &expr_buffer[skip_len]);
    printf("Result: %s\n", res_buffer);
    printf("%s", buffer);

    // Cleanup and wait for child to exit
    close_fd(proc.stdin); 
    close_fd(proc.stdout); 
    close_fd(proc.stderr); 

    waitpid(proc.pid, &status, 0);
    
    return EXIT_SUCCESS;
}
```

Ouput:

```console
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# gcc main.c -o tuftlt
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# scp tuftlt dvdalt@2019shell1.picoctf.com:/home/dvdalt
tuftlt                                                                                100%   17KB   5.8KB/s   00:03
root@kali:/media/sf_CTFs/pico/Times_Up_For_the_Last_Time# ssh dvdalt@2019shell1.picoctf.com
dvdalt@pico-2019-shell1:~$ cd /problems/time-s-up--for-the-last-time-_1_a7830af9d51a361ee5d3b9eece69c22f
dvdalt@pico-2019-shell1:/problems/time-s-up--for-the-last-time-_1_a7830af9d51a361ee5d3b9eece69c22f$ ~/tuftlt
Alarm blocked!
Expression: (((((1278126002) f (1043275798)) x ((-464689218) - (322949368))) - (((1447574425) & (-1224253887)) x ((-1216973929) o (116652416)))) % ((((1762630346) % (1604569453)) * ((-592128242) f (778774872))) r (((783785328) & (1511276257)) ^ ((-2081807565) - (-502728913)))))
Result: -904291002

picoCTF{And now you can hack time! #2e0a37d1}
Solution? Congrats! Here is the flag!
```
