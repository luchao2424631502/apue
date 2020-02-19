#include "apue.h" 
#include "pr_mask.h"
 
static void sig_int(int);

int main(int argc,char *argv[]) {
	sigset_t newmask,oldmask,waitmask;

	pr_mask("program start: ");

	if (signal(SIGINT,sig_int) == SIG_ERR)
		err_sys("signal (SIGINT) error");
	// 
	sigemptyset(&waitmask);
	sigaddset(&waitmask,SIGUSR1);
	sigemptyset(&newmask);
	sigaddset(&newmask,SIGINT);

	// SIGINT　信号被阻塞
	if (sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		err_sys("SIG_BLOCK error");
	// 代码临界区
	pr_mask("in critical region: ");

	// 返回后mask,恢复成newmask,这里是阻塞 sigusr1
	if (sigsuspend(&waitmask) != -1)
		err_sys("sigsuspend error");
	pr_mask("after return from sigsuspend: ");

	// 取消block sigint
	if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
		err_sys("SIG_SETMASK error");

	pr_mask("program exit: ");

    exit(0);
}

static void 
sig_int(int signo) {
	pr_mask("\nin sig_int: ");
}
