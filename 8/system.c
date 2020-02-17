#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>
#include "apue.h"
 
void pr_exit(int status) {
	// WIF EXITED 
	if (WIFEXITED(status))
		printf("normal termination, exit status = %d\n",
				WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		printf("abnormal termination, signal number = %d%s\n",
				WTERMSIG(status),
#ifdef WCOREDUMP
				WCOREDUMP(status) ? " (core file generated)" : "" );
#else 
				"");
#endif
	else if (WIFSTOPPED(status))
		printf("child stopped, signal number = %d\n",
				WSTOPSIG(status));
}

int system(const char *cmdstring) {
	pid_t pid;
	int status;
	if (cmdstring == NULL)
		return (1);
	if ((pid = fork()) < 0)	
		status = -1;
	else if (pid == 0) {
		// "sh"作为第一个参数会被取消掉
		execl("/bin/sh","sh","-c",cmdstring,(char *)0);
		// execl执行失败就会来到下一步
		_exit(127);
		// 1. 防止子进程冲洗IO缓冲区和执行终止处理程序
	} else {
		while(waitpid(pid,&status,0) < 0)
			if (errno != EINTR) {
				status = -1;
				break;
			}
	}
	return status;
} 

int main(int argc,char *argv[]) {
	int status;
	if ((status = system("date")) < 0)
		err_sys("system() errror\n");
	pr_exit(status);
	
	if ((status = system("nosuchcommand")) < 0)
		err_sys("system() error\n");
	pr_exit(status);

	if ((status = system("who; exit 44")) < 0)
		err_sys("system() error\n");
	pr_exit(status);
 
	pr_exit(1);
    exit(0);
}
