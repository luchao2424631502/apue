#include "apue.h" 

static volatile sig_atomic_t sigflag;
static sigset_t newmask,oldmask,zeromask;

static void
sig_usr(int signo) 
{
	// 捕捉程序将全局原子变量设置成1
	sigflag = 1;
}

void TELL_WAIT(void)
{
	// 注册捕捉程序
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("signal (SIGUSR1) error");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("signal (SIGUSR2) error");
	// 初始化信号集
	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	
	sigaddset(&newmask,SIGUSR1);
	sigaddset(&newmask,SIGUSR2);

	// 设置进程信号屏蔽字
	if (sigprocmask(SIG_BLOCK,&newmask,&oldmask) < 0)
		err_sys("SIG_BLOCK error");
}

void TELL_PARENT(pid_t pid) { kill(pid,SIGUSR2); }
void TELL_CHILD(pid_t pid) { kill(pid,SIGUSR1); }

void WAIT_PARENT(void)
{
	// sigflag==0就　等待信号
	while (sigflag == 0)
		sigsuspend(&zeromask);
	// 重新设置成0
	sigflag = 0;

	if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
		err_sys("SIG_SETMASK error");
}

void WAIT_CHILD(void)
{
	while (sigflag == 0)
		sigsuspend(&zeromask);
	sigflag = 0;

	if (sigprocmask(SIG_SETMASK,&oldmask,NULL) < 0)
		err_sys("SIG_SETMASK error");
}
 
int main(int argc,char *argv[]) {
 
    exit(0);
}
