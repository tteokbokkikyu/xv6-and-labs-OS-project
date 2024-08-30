#include "kernel/types.h"
#include "user/user.h"
 
void transmit(int lp[2], int rp[2], int p1) {
	int data;
	while (read(lp[0], &data, sizeof(int)) == sizeof(int))
		if (data % p1)
			write(rp[1], &data, sizeof(int));
	close(lp[0]);
	close(rp[1]);
}

void primes(int lp[2]) {
	close(lp[1]);
	int p1;
	if(read(lp[0],&p1,sizeof(int))!=sizeof(int))//如果未能成功读取
		exit(0);
	printf("prime %d\n", p1);
	int rp[2];
	pipe(rp);
	transmit(lp, rp, p1);
	if (fork() == 0)
		primes(rp);
	else {
		close(rp[0]);//关闭读端
		wait(0);
	}
	exit(0);
}

int main(int argc, char const* argv[]) {
	int p[2];
	pipe(p);
	for (int i = 2; i <36; i++)
		write(p[1], &i, sizeof(int));
	if (fork() == 0)//子进程
		primes(p);
	else {//父进程
		close(p[1]);
		close(p[0]);
		wait(0);
	}
	exit(0);
}
