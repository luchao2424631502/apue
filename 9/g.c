#include "apue.h"

static void sig_alrm(int);
 
int main(int argc,char *argv[]) {
	int n;
	char line[MAXLINE];
	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		err_sys("signal (SIGALRM) error");
	// 注册sigalrm信号处理程序后, 设置闹钟
	alarm(10);
	// 1. 这里内核使进程发生阻塞,read永远执行不了
	// 2. 系统调用read可能是自动重启,在信号处理程序结束后,继续执行,alarm设置毫无意义
	if ((n = read(STDIN_FILENO,line,MAXLINE)) < 0)
		err_sys("read error");
	//read执行完 , 取消闹钟
	alarm(0);
	write(STDOUT_FILENO,line,n);
    exit(0);
}

static void sig_alrm(int signo) {
	
}
