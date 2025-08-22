#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define RD 0
#define WT 1

const uint INT_LEN = sizeof(int);

// 获取left的第一个值(必为prime<2为prime>)[第一个数据被消失,为prime则print]
bool get_left_first(int p[], int *first) {
    if(read(p[RD], first, INT_LEN) == INT_LEN) {
        fprintf(1, "prime %d\n", *first);
        return true;
    }
    return false;
}

// 左管道输出到右边管道
void left2right(int lp[], int rp[], int first) {
    close(lp[WT]);
    int data;
    while (read(lp[RD], &data, INT_LEN) == INT_LEN) {
        if (data % first != 0) {
            write(rp[WT], &data, INT_LEN);
        }
    }
    close(lp[RD]);
}

void primes(int lp[]) {
    int prime;
    if (get_left_first(lp, &prime) == true) {
        int p[2];
        if (pipe(p) == -1) {
            fprintf(2, "pipe failed...\n");
            exit(1);
        }

        left2right(lp, p, prime);

        if (fork() == 0) {
            primes(p);
        }
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    if (pipe(p) == -1) {
        fprintf(2, "pipe failed...\n");
        exit(1);
    }
    
    for (int i = 2; i <= 35; ++i) {
        write(p[WT], &i, INT_LEN);
    }

    primes(p);

    exit(0);
}
