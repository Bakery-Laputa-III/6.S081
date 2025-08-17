#include "kernel/types.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
    int p[2];
    char x = 'o';

    if (pipe(p) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    int pid = fork();

    if (pid > 0) {
        char c;
        write(p[1], &x, 1);
        close(p[1]);
        wait(0);
        read(p[0], &c, 1);
        fprintf(1, "%d: received pong\n", getpid());
        close(p[0]);
        exit(0);
    } else if (pid == 0) {
        char c;
        read(p[0], &c, 1);
        fprintf(1, "%d: received ping\n", getpid());
        close(p[0]);
        write(p[1], &x, 1);
        close(p[1]);
        exit(0);
    }

    exit(0);
}