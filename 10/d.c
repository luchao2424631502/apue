#include "apue.h" 
#include <signal.h>
 
int main(int argc,char *argv[]) {
	sigset_t sigset;
	int status;
	if ((status = sigprocmask(0,NULL,&sigset)) == -1)
		err_sys("sigprocmask error");
	// 初始化一下
	sigemptyset(&sigset);
	printf("sigusr1 %d sigusr2 %d\n",sigismember(&sigset,SIGUSR1),
			sigismember(&sigset,SIGUSR2));
	// 设置集合的位
	if ((status = sigaddset(&sigset,SIGUSR1)) == -1)
		err_sys("sigaddset error");
	// 和进程的屏蔽字合并	
	if ((status = sigprocmask(SIG_BLOCK,&sigset,NULL)) == -1)
		err_sys("sigprocmask error");
	sigset_t newsigset;
	// 取得进程当前的屏蔽字
	if ((status = sigprocmask(0,NULL,&newsigset)) == -1)
		err_sys("newsigset error");
	printf("sigusr1 %d sigusr2 %d\n",sigismember(&newsigset,SIGUSR1),
			sigismember(&newsigset,SIGUSR2));

	exit(0);
}
