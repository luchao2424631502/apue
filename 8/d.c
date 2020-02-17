#include <sys/wait.h>
#include "apue.h"
 
char *env_init[] = {"USER=unknown","PATH=/tmp",NULL};

int main(int argc,char *argv[]) {
	pid_t pid;
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) {
		char *path = "/home/luchao/Documents/Apue/8/echoall";
		if (execle(path,"echoall","myarg1","MY ARG2",(char *)0,env_init) < 0)
			err_sys("execle error");
	}
	// 子进程由于调用exec替换了堆栈和代码段数据段,所有exit()终止了,
	if (waitpid(pid,NULL,0) < 0)
		err_sys("fork error");

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0)
		if (execlp("echoall","echoall","only 1 arg",(char *)0) < 0)
			err_sys("execlp error");
    exit(0);
}
