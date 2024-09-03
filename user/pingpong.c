#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int p1[2], p2[2];
    char buf[4];
    
    // 创建两个管道
    pipe(p1); // p1 用于子进程 -> 父进程
    pipe(p2); // p2 用于父进程 -> 子进程
    
/*p1[0] 和 p2[0] 是管道的读取端（读端）。
p1[1] 和 p2[1] 是管道的写入端（写端）。*/

    int pid = fork();

    if(pid > 0) {
        // 父进程代码
        close(p1[1]); // 关闭p1管道的写端
        close(p2[0]); // 关闭p2管道的读端

        // 向子进程发送“ping”
        write(p2[1], "ping", 4);

        // 从子进程接收“pong”
        read(p1[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);

        // 关闭剩余的管道端
        close(p1[0]);
        close(p2[1]);
    } else if(pid == 0) {
        // 子进程代码
        close(p1[0]); // 关闭p1管道的读端
        close(p2[1]); // 关闭p2管道的写端

        // 从父进程接收“ping”
        read(p2[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);

        // 向父进程发送“pong”
        write(p1[1], "pong", 4);

        // 关闭剩余的管道端
        close(p1[1]);
        close(p2[0]);
    } else {
        // fork 失败
        printf("fork error\n");
        exit(1);
    }

    exit(0);
}