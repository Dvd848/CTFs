# Time's Up, Again!
Reverse Engineering, 450 points

## Description:
> Previously you solved things fast. Now you've got to go faster. Much faster. Can you solve *this one* before time runs out? 


## Solution: 

This is the follow-up for [Time's Up](Times_Up.md).

Let's run the attached file:

```console
root@kali:/media/sf_CTFs/pico/Times_Up_Again# ./times-up-again
Challenge: (((((1735039958) * (-243100254)) * ((-1814728608) + (-721614403))) - (((25798291) + (1566207868)) - ((1371345736) - (328354048)))) + ((((-574011032) - (-976943047)) + ((1078373531) + (-764373806))) * ((((151190841) - (-1982829483)) - ((-1990831043) - (440995030))) - (((-1753759716) + (-1201408286)) * ((656319610) * (143280253))))))
Setting alarm...
Solution? Alarm clock
```

We got a mathematical expression, and shortly after, the program got killed by an alarm. How long is the alarm? Let's use `strace` to see:
```console
root@kali:/media/sf_CTFs/pico/Times_Up_Again# strace ./times-up-again 2>&1 | grep setitimer
setitimer(ITIMER_REAL, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=200}}, {it_interval={tv_sec=0, tv_usec=0}, it_value={tv_sec=0, tv_usec=0}}) = 0
```

We can see that the alarm is set for 200 uSeconds - much sorter than the last time. In this time, we must evaluate the equation and input the answer back to the program.

We won't have time to call `bc` to evaluate the expression this time, we'll have to choose a faster approach: A `C` program.

The standard approach for communicating with a program's `stdin` and `stdout` in `C` is via pipes: We implement a wrapping program which `fork`s itself - the child then executes the program we want to communicate with and the parent is able to read the child's `stdout` and write to its `stdin`.

I used a template based on [this article](https://jameshfisher.com/2017/02/17/how-do-i-call-a-program-in-c-with-pipes/) to setup the pipes which are used for communication. [This StackOverflow Answer](https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c) offered a suitable lightweight implementation of expression evaluation, and only minor changes were needed (support spaces in the expression, use `int64_t` instead of `int`).

Here's the `C` program:
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/prctl.h>

// ----------------------------------------------------------------------------------------------------------

//https://stackoverflow.com/questions/9329406/evaluating-arithmetic-expressions-from-string-in-c
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
    int64_t result = get() - '0';
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

int64_t term()
{
    int64_t result = factor();
    while (peek() == '*' || peek() == '/')
        if (get() == '*')
            result *= factor();
        else
            result /= factor();
    return result;
}

int64_t expression()
{
    int64_t result = term();
    while (peek() == '+' || peek() == '-')
        if (get() == '+')
            result += term();
        else
            result -= term();
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

    // Printing takes time 
    // Instead of reusing the same buffer, we have three different buffers, 
    //   allowing us to print the full details at the end
    char                expr_buffer[BUFFER_LENGTH]  = {0};
    char                res_buffer[BUFFER_LENGTH]   = {0};
    char                buffer[BUFFER_LENGTH]       = {0};

    size_t              skip_len = sizeof("Challenge: ") - 1;

    char*               child_argv[] = {"./times-up-again", NULL};
        
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

We can compile it with `gcc main.c -o main -O3` in order to enable the highest optimization level.

Output:
```console
root@kali:/media/sf_CTFs/pico/Times_Up_Again# gcc main.c -o main -O3
root@kali:/media/sf_CTFs/pico/Times_Up_Again# scp main dvdalt@2019shell1.picoctf.com:/home/dvdalt
main                                                                                  100%   17KB  17.3KB/s   00:01
dvdalt@pico-2019-shell1:~$ cd /problems/time-s-up--again-_2_55710a2388cfe35ec1afa8221b3f1ded
dvdalt@pico-2019-shell1:/problems/time-s-up--again-_2_55710a2388cfe35ec1afa8221b3f1ded$ ~/main
Expression: (((((1644541454) * (-1558748964)) * ((-1915732918) * (-537296725))) * (((-1229945264) + (-1124190767)) - ((-1075073684) - (616112321)))) - ((((201901501) * (-718856452)) + ((-114663071) - (1054737896))) - ((((652115406) * (-2075635321)) - ((1592210214) - (-870393698))) * (((1283042663) - (1137700101)) + ((1222165819) - (1980696317))))))
Result: 2936000942653684315

picoCTF{Hasten. Hurry. Ferrociously Speedy. #3230cac7}
```
