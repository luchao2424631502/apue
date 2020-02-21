#include "apue.h" 
 
static void handler(int);

int main(int argc,char *argv[]) {
	printf("pid = %d\n",getpid());
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	sigset_t sigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGINT);
	sigaddset(&sigset,SIGRTMIN);
	// 设置信号屏蔽字
	sigprocmask(SIG_BLOCK,&sigset,NULL);
    // 设置信号捕捉程序
	if (sigaction(SIGINT,&act,NULL) < 0)
		err_quit("sigaction error");
	if (sigaction(SIGRTMIN,&act,NULL) < 0)
		err_quit("sigaction error");
	if (sigaction(SIGUSR1,&act,NULL) < 0)
		err_quit("sigaction error");
	// 这里循环等待信号
	for (; ;)
		pause();
    exit(0);
}

void 
handler(int signo)
{
	if (signo == SIGINT || signo == SIGRTMIN)
		printf("recv a sig = %d\n",signo);
	else if (signo == SIGUSR1) {
		sigset_t sigset;
		sigemptyset(&sigset);
		sigaddset(&sigset,SIGINT);
		sigaddset(&sigset,SIGRTMIN);
		// 手动清除pendint位,变成delivery状态(执行信号捕捉程序,但是不改变阻塞状态),
		sigprocmask(SIG_UNBLOCK,&sigset,NULL);
	}
}
