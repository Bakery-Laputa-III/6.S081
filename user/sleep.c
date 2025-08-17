#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// usage: sleep time
// time中如果有非0-9字符,则返回该字符以前的数字含义, 首字母即不为0-9字符返回0
int
main(int argc, char *argv[]) {  
    int time;

    if (argc != 2) {
        fprintf(2, "Usage: sleep time...\n");
        exit(1);
    }

    time = atoi(argv[1]);

    /*atoi特性:首字符非0-9数字则返回0, 由此以下代码无法正常实现*/
    // if (time < 0) {     
    //     fprintf(2, "Error: sleep time must be non-negative\n");
    //     exit(1);
    // }

    sleep(time);

    exit(0);
}