#include "apue.h" 
 
static void 
sig_alrm(int signo) {}

unsigned int 
sleep(unsigned int seconds)
{
	struct sigaction newact,oldact;
	sigset_t newmask,oldmask,suspmask;
	unsigned int unslept;

	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	// 所有信号都不阻塞,且设置alrm信号的捕捉程序,可能执行其他信号处理程序
	sigaction(SIGALRM,&newact,&oldact);

	// 阻塞ALRM信号
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&newmask,&oldmask);

	alarm(seconds);
	suspmask = oldmask;
	// newmask -> suspmask -> newmask
	sigdelset(&suspmask,SIGALRM);

	// 取消阻塞SIGALRM信号,可能提前收到SIGALRM信号
	sigsuspend(&suspmask);

	// 从信号捕捉函数返回,取消以前闹钟时间,返回剩余时间
	unslept = alarm(0);

	// 恢复以前的信号处理程序
	sigaction(SIGALRM,&oldact,NULL);
	// 恢复以前的信号屏蔽字 newmask->oldmask
	sigprocmask(SIG_SETMASK,&oldmask,NULL);
	
	return unslept;
}

int main(int argc,char *argv[]) {
 
    exit(0);
}
