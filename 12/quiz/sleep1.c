#include "apue.h" 

// 信号处理程序
static void
sig_alrm(int signo) {
	
}

unsigned int 
sleep(unsigned int seconds) {
	struct sigaction newact,oldact;
	sigset_t newmask,oldmask,suspmask;
	unsigned int unslept;

	// sigaction设置sigalrm处理程序
	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGALRM,&newact,&oldact);

	// 设置信号集 阻塞信号
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);

	alarm(seconds);
	suspmask = oldmask;

	// 确保sigalrm不在信号集中
	sigdelset(&suspmask,SIGALRM);

	// 进程挂起,等待信号到来(,设置新的信号屏蔽字,不阻塞信号)
	sigsuspend(&suspmask);

	unslept = alarm(0);

	// 恢复sigalrm信号处理程序
	sigaction(SIGALRM,NULL,&oldact);

	sigprocmask(SIG_SETMASK,&oldmask,NULL);

	return unslept;
}
 
int main(int argc,char *argv[]) {
 
    exit(0);
}
