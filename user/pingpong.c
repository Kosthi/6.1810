//
// Created by Koschei on 2024/2/17.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p[2];
    char buffer;

    pipe(p);
    if(fork() == 0) {
        // 标准输入重定向为管道输入
        // 关闭标准输入，文件描述符0可用
        close(0);
        // 将p[0]复制到当前可用的最小文件描述符（即0，因为0被关闭了）
        // 0和p[0]指向同一个文件
        dup(p[0]);
        // 关闭多余的文件描述符
        close(p[0]);
        // 关闭写端，否则会阻塞读
        read(0, &buffer, 1);
        fprintf(1, "%d: received ping\n", getpid());
        write(p[1], "b", 1);
        close(p[1]);
    } else {
        close(0);
        dup(p[0]);

        close(p[0]);

        write(p[1], "b", 1);
        close(p[1]);

        read(0, &buffer, 1);

        wait(0);

        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}
