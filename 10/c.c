#include "apue.h"
 
static void sig_quit(int);

int main(int argc,char *argv[]) {
	sigset_t newmask,oldmask,pendmask;

	// 设置SIGQUIT信号处理函数,
	if (signal(SIGQUIT,sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");
	
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGQUIT);
	// oldmask 返回的是设置前的信号集,且阻塞SIGQUIT信号
	if (sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		err_sys("SIG_BLOCK error");
	
	sleep(5);

	// 返回当前处于未决状态的信号,如果没有信号被阻塞,那么即使屏蔽字设置了阻塞也无关 
	if (sigpending(&pendmask) < 0)
		err_sys("sigpending error");
	// 检查SIGQUIT是否不能递送
	if (sigismember(&pendmask,SIGQUIT))
		printf("\n SIGQUIT pending \n");
	
	// 恢复设置之前的信号量集合
	if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
		err_sys("SIG_SETMASK error");	
	printf("SIGQUIT unblocked\n");

	sleep(5);
    exit(0);
}

static void 
sig_quit(int signo) {
	printf("caught SIGQUIT\n");
	// 下次信号到来设置为默认
	if (signal(SIGQUIT,SIG_DFL) == SIG_ERR)
		err_sys("can't reset SIGQUIT");
}
