#include "apue.h" 

// typedef void Sigfunc (int );
Sigfunc *signal(int signo,Sigfunc *func) {
	struct sigaction act,oact;
	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	// 闹钟的中断处理程序　不能重启动
	if (signo == SIGALRM) {
#ifdef SA_INTERRUPT
	act.sa_flags |= SA_INTERRUPT;
#endif
	}
	//　其他信号都重启动
	else 
		act.sa_flags |= SA_RESTART;

	if (sigaction(signo,&act,&oact) < 0)
		return (SIG_ERR);
	// 成功后改变信号处理动作后,返回原来的处理程序
	return oact.sa_handler;
}

// 思路,1.注册信号处理程序,修改mask,阻塞该信号,sleep 5s,然后sigpending判断是否阻塞,
// 2.恢复mask,

void sig_quit(int signo) { 
	printf("handler\n"); 
	if (signal(SIGQUIT,SIG_DFL) == SIG_ERR)
		err_sys("error handler");
} 

int main(int argc,char *argv[]) {
	if (signal(SIGQUIT,sig_quit) == SIG_ERR)
		err_sys("signal error");
	sigset_t sigset,osigset;
	sigemptyset(&sigset);
	sigaddset(&sigset,SIGQUIT);
	// 修改进程的信号mask
	if (sigprocmask(SIG_BLOCK,&sigset,&osigset) < 0)
		err_sys("sigprocmask error");
	// 沉睡5s 看是否收到信号
	sleep(5);

	if (sigpending(&sigset) < 0)
		err_sys("sigpending error");
	if (sigismember(&sigset,SIGQUIT))
		printf("sig block\n");
	
	// 回复原来的信号处理程序
	if (sigprocmask(SIG_SETMASK,&osigset,NULL) < 0)
		err_sys("sigprocmask error");
	printf("UN block\n");

	sleep(5);
    exit(0);
}
