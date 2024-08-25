#include "kernel/types.h"
#include "user/user.h"

int main()
{
    int p1[2], p2[2];
    char buf[4];
    
    // ���������ܵ�
    pipe(p1); // p1 �����ӽ��� -> ������
    pipe(p2); // p2 ���ڸ����� -> �ӽ���
    
/*p1[0] �� p2[0] �ǹܵ��Ķ�ȡ�ˣ����ˣ���
p1[1] �� p2[1] �ǹܵ���д��ˣ�д�ˣ���*/

    int pid = fork();

    if(pid > 0) {
        // �����̴���
        close(p1[1]); // �ر�p1�ܵ���д��
        close(p2[0]); // �ر�p2�ܵ��Ķ���

        // ���ӽ��̷��͡�ping��
        write(p2[1], "ping", 4);

        // ���ӽ��̽��ա�pong��
        read(p1[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);

        // �ر�ʣ��Ĺܵ���
        close(p1[0]);
        close(p2[1]);
    } else if(pid == 0) {
        // �ӽ��̴���
        close(p1[0]); // �ر�p1�ܵ��Ķ���
        close(p2[1]); // �ر�p2�ܵ���д��

        // �Ӹ����̽��ա�ping��
        read(p2[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);

        // �򸸽��̷��͡�pong��
        write(p1[1], "pong", 4);

        // �ر�ʣ��Ĺܵ���
        close(p1[1]);
        close(p2[0]);
    } else {
        // fork ʧ��
        printf("fork error\n");
        exit(1);
    }

    exit(0);
}

