#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
    int x = atoi(argv[1]);
    fprintf(1, "%d", x);
    return 0;
}