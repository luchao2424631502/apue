#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include "apue.h"

int 
system(const char *cmdstring)
{
	pid_t pid;
	int status;
	struct sigaction ignore,saveintr,savequit;
	sigset_t chldmask,savemask;

	if (cmdstring == NULL)
		return 1;

	// 进行忽略的捕捉函数
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;

	if (sigaction(SIGINT,&ignore,&saveintr) < 0)
		return -1;
	if (sigaction(SIGQUIT,&ignore,&savequit) < 0)
		return -1;
	
	sigemptyset(&chldmask);
	sigaddset(&chldmask,SIGCHLD);

	if (sigprocmask(SIG_BLOCK,&chldmask,&savemask) < 0)
		return -1;

	if ((pid = fork()) < 0)
		status = -1;
	else if (pid == 0) {
		// 子进程恢复原来的信号捕捉方式
		sigaction(SIGINT,&saveintr,NULL);
		sigaction(SIGQUIT,&savequit,NULL);
		sigprocmask(SIG_SETMASK,&savemask,NULL);

		// system() = fork() + execl()
		execl("/bin/sh","sh","-c",cmdstring,(char *)0);
	
		// 底层退出方法
		_exit(127);
	} else {
		while (waitpid(pid,&status,0) < 0)
			if (errno != EINTR) {
				status = -1;
				break;
			}
	}
	// system回收了子进程后恢复信号捕捉
	if (sigaction(SIGINT,&saveintr,NULL) < 0)
		return -1;
	if (sigaction(SIGQUIT,&savequit,NULL) < 0)
		return -1;
	if (sigprocmask(SIG_SETMASK,&savemask,NULL) < 0)
		return -1;

	// 返回进程的状态
	return status;
}

static void 
sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void
sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
}
 
int main(int argc,char *argv[]) {
	if (signal(SIGINT,sig_int) == SIG_ERR)
		err_sys("signal (SIGINT) error");
	if (signal(SIGCHLD,sig_chld) == SIG_ERR)
		err_sys("signal (SIGCHLD) error");
	// linux在system()回收了子进程之后仍将SIGCHLD信号递送给父进程
	// 即使调用system()的进程捕捉函数想回收子进程,但是会失败返回-1 errno = ECHLD
	if (system("/bin/vim") < 0)
		err_sys("system() error");
	exit(0);
}
