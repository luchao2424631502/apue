#include "apue.h" 

volatile sig_atomic_t quitflag;

static void
sig_int(int signo)
{
	if (signo == SIGINT)
		printf("\ninterrupt\n");
	// 信号处理程序和主程序都要运行该变量,
	else if (signo == SIGQUIT)
		quitflag = 1; // 设置全局变量
}

int main(int argc,char *argv[]) {
	sigset_t newmask,oldmask,zeromask;

	// 设置信号捕捉程序
	if (signal(SIGINT,sig_int) == SIG_ERR)
		err_sys("signal (SIGINT) error");
	if (signal(SIGQUIT,sig_int) == SIG_ERR)
		err_sys("signal (SIGQUIT) error");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);
	
	// 设置信号屏蔽字,阻塞 SIGQUIT信号
	if (sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		err_sys("SIG_BLOCK error");
	// 轮询quitflag
	while(quitflag == 0)
		sigsuspend(&zeromask);
	quitflag = 0;

	// 从newmask转为oldmask信号屏蔽字
	if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
		err_sys("SIG_SETMASK error");
    exit(0);
}
