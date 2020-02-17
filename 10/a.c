#include "apue.h"
 
static void sig_usr2(int signo) {
	printf("received signal usr2\n");
}

static void sig_usr1(int signo) {
	printf("received signal usr1\n");
}

int main(int argc,char *argv[]) {
	printf("pid = %d\n",getpid());
	// 声明信号集合
	sigset_t sigset;
	// 初始化
	sigemptyset(&sigset);
	// 告诉内核不允许发生该信号集合中的信号
	// 信号集合即使设置了也会发生该信号
	sigaddset(&sigset,SIGUSR1);
	signal(SIGUSR2,sig_usr2);
	signal(SIGUSR1,sig_usr1);
	pause();
	printf("%d %d\n",sigismember(&sigset,SIGUSR1),
					sigismember(&sigset,SIGUSR2));
    exit(0);
}
