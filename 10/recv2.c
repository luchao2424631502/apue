#include "apue.h" 
 
static void handler(int);
static void handler1(int);

int main(int argc,char *argv[]) {
	printf("pid = %d\n",getpid());
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	// 相比signal函数,使用sigaction能在进入信号捕捉函数前更新屏蔽字
	sigaction(SIGUSR1,&act,NULL);

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGUSR2);
	// 阻塞SIGUSR2信号
	sigprocmask(SIG_BLOCK,&sigset,NULL);

	for (; ;)
		pause();
    exit(0);
}
static void
handler1(int signo)
{
	printf("two proc\n");
}

static void 
handler(int signo)
{
	// 在信号处理程序中将自己设置成非阻塞
	if (signo == SIGUSR1) {
		printf("before\n");
		//在sigusr1信号捕捉函数中 解除对sigusr2信号的阻塞,但只是重置pending位,而不是阻塞位
		sigset_t sigset;
		sigemptyset(&sigset);
		sigaddset(&sigset,SIGUSR2);
		sigprocmask(SIG_UNBLOCK,&sigset,NULL);
		printf("end\n");
	}
}

