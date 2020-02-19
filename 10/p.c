#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

int 
system(const char *cmdstring)
{
	pid_t pid;
	int status;
	struct sigaction ignore,saveintr,savequit;
	sigset_t chldmask,savemask;

	//　检查命令行
	if (cmdstring == NULL)
		return 1;

	// 设置sigaction的捕捉函数
	ignore.sa_handler = SIG_IGN;
	sigemptyset(&ignore.sa_mask);
	ignore.sa_flags = 0;
	// 父进程忽略SIGINT,SIGQUIT信号
	if (sigaction(SIGINT,&ignore,&saveintr) < 0)
		return -1;
	if (sigaction(SIGQUIT,&ignore,&savequit) < 0)
		return -1;

	sigemptyset(&chldmask);
	sigaddset(&chldmask,SIGCHLD);
	// 父进程阻塞SIGCHLD信号
	if (sigprocmask(SIG_BLOCK,&chldmask,&savemask) < 0)
		return -1;

	// 子进程执行cmdstring内容
	if ((pid = fork()) < 0) 
		status = -1;
	else if (pid == 0) {
		//父子进程信号共享,所以要重新设置子进程的信号处理方式
		sigaction(SIGINT,&saveintr,NULL);
		sigaction(SIGQUIT,&savequit,NULL);
		sigprocmask(SIG_SETMASK,&savemask,NULL);

		execl("/bin/sh","sh","-c",cmdstring,(char *)0);
		
	}
	else {}
}
 
int main(int argc,char *argv[]) {
 
    exit(0);
}
