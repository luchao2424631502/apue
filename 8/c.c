#include <stdio.h>
#include <sys/wait.h>
#include "apue.h"
 
int main(int argc,char *argv[]) {
	pid_t pid;
	if ((pid = fork()) < 0)
		err_sys("fork error");
	// 子进程
	else if (pid == 0) {
		if ((pid = fork()) < 0)
			err_sys("fork error");
		else if (pid > 0)
			exit(0);
		sleep(2);
		printf("second child, parent pid = %ld\n",(long)getppid());
		exit(0);
	}
	// 子进程先死　父进程必须接管信息,否则父进程先死则子进程由init进程接管
	// 主进程pid得到的是　子进程的pid,但是现在子进程已经关闭
	if (waitpid(pid,NULL,0) != pid)
		err_sys("waitpid error");
	exit(0);
}
