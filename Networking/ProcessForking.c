#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    int rv;
    // Fork is a function which splits a process off, and returns a different value depending on which process it is called on.
    // The chain of execution in this processes is first in the initial parent down to switch, whereupon a child process is launched
    // and the return value is sent to the main process.
    // The main process executes the default branch, and reaches wait.
    // Simultaneously the child process begins execution from main, and reaches the switch. When fork is called on the child process it
    // returns 0 and creates no new process.
    // The child enters the 0 branch and executes it's commands, before calling exit returning the value to the parent.
    // The wait function receives the value and can then print it.
    // The return value actually encodes a lot more information than just the return value, but also some extra stuff.
    // Each process it's like a function - it get's it's own copies of the values.
    switch(pid = fork()) {
    case -1:
        perror("fork");
        exit(1);
    case 0:
        printf(" CHILD: This is the child process!\n");
        printf(" CHILD: My PID is %d\n", getpid());
        printf(" CHILD: My parent's PID is %d\n", getppid());
        printf(" CHILD: Enter my exit status (make it small): ");
        scanf("%d", &rv);
        fflush(stdin);
        printf(" CHILD: I'm outa here\n");
        exit(rv);
    default:
        printf("PARENT: This is the parent process!\n");
        printf("PARENT: My PID %d\n", getpid());
        printf("PARENT: My child's pid is %d\n", pid);
        printf("PARENT: I'm now waiting for my child to exit()...\n");
        wait(&rv);
        printf("PARENT: My child's exit status is %d.\n", WEXITSTATUS(rv));
        printf("PARENT: I'm outta here!\n");
    }

}

