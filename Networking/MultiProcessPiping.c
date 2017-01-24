#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    int pfds[2];
    char buf[30];

    pipe(pfds);

    // Combining Fork with pipes.
    if(!fork()) {
        printf(" CHILD: writing to the pipe\n");
        // even though, it might seem like the child would have
        // it's own pipe, pipes are child safe, so children get
        // the file descriptors of the pipe initiated in the
        // parent.
        write(pfds[1], "CHILD: test", 5);
        printf(" CHILD: exiting\n");
        exit(0);
    } else {
        // The parent can read from the pipe, which would have been
        // written to by the child. - reading blocks if something is
        // writing to the pipe.
        printf("PARENT: reading from pipe\n");
        read(pfds[0], buf, 5);
        printf("PARENT: read \"%s\"\n", buf);
        wait(NULL);
    }
}

