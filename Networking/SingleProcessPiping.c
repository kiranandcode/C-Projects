#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    int pfds[2];
    char buf[30];

    // creates a pipe, returning the file descriptors
    // to the opening and closing point of the pipe.
    // arr[0] is the output, and arr[1] is the input.
    if(pipe(pfds) == -1) {
        perror("pipe");
        exit(1);
    }
    // Write can be used to write to the file, via the
    // file descriptor, rather than the FILE *
    printf("writing to file description #%d\n", pfds[0]);
    write(pfds[1], "test", 5);
    printf("reading from file descriptor #%d\n", pfds[1]);
    read(pfds[0], buf, 5);
    printf("read \"%s\"\n", buf);

    return 0;
}

