#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    int pfds[2];

    pipe(pfds);

    // Each of the sys commands reads from stdin,
    // and writes to stdout.
    if(!fork()) {
        // Closes stdout.
        close(1);
        // Sets the pipein to be stdout
        dup(pfds[1]);
        // we no longer need pipe out, as
        // in this process, pipe out is not
        // used.
        close(pfds[0]);
        // Runs the command ls
        execlp("ls", "ls", NULL);
    } else {
        // Closes stdin
        close(0);
        // Sets the pipe out to be stdin.
        dup(pfds[0]);
        // Close pipe in
        close(pfds[1]);
        execlp("wc", "wc", "-l", NULL);


    }

}

