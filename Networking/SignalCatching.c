#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
// Error handling function to catch SIGINT.
void sigint_handler(int sig) {
    // We have no idea when the handler function
    // will be called, thus we have to use an
    // async compatible function.
    write(0, "Ahhh, SIGINT!\n", 14);
}

int main()
{
    void sigint_handler(int sig); // Prototype def
    char s[200];
    struct sigaction sa;

    // Set's up the handler to catch errors
    sa.sa_handler = sigint_handler; // Function to call.
    sa.sa_flags = SA_RESTART; // How to respond
    sigemptyset(&sa.sa_mask);

    //sigaction sets up the struct sa to be used to deal with
    // SIGINT issues.
    if(sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("Enter a string:\n");
    if(fgets(s, sizeof s, stdin) == NULL)
        perror("fgets");
    else
        printf("You entered: %s\n", s);
    return 0;

}

