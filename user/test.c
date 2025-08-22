#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[]) {
    // 测试管道在父程序就关闭,子程序是否可以使用
    // 所以说父进程关闭了管道,创建的子进程就不可以使用了, 但是子进程关闭父依旧可以使用, 其他子进程也可以. 所以fork会复制文件描述符,但是子进程即使close也不会影响,因为是软连接?
    int p[2];
    pipe(p);

    close(p[0]);
    close(p[1]);

    if(fork() == 0) {
        int x  = 1;
        int y  = 0;
        write(p[1], &x, 1);
        close(p[1]);
        read(p[0], &y, 1);
        close(p[0]);
        fprintf(1, "y = %d", y);    // 0
    }

    exit(0);
}