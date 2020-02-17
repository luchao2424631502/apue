#include "apue.h"
#include <setjmp.h>

static void sig_alrm(int);
static jmp_buf env_alrm;

int main(int argc,char *argv[]) {
	char line[MAXLINE];
	int n;

	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		err_sys("signal (SIGALRM) error");
	// setjmp != 0 说明执行了信号处理程序跳转了回来
	if (setjmp(env_alrm) != 0) 
		err_quit("read timeout");
	// 设置闹钟
	alarm(10);
	// 如果进程在这里阻塞? 到时间了(时钟信号到了直接退出) 
	if ((n = read(STDIN_FILENO,line,MAXLINE)) < 0)
		err_sys("read error");
	//　清除闹钟
	alarm(0);
	write(STDOUT_FILENO,line,n);
    exit(0);
}

static void sig_alrm(int signo) { longjmp(env_alrm,1); }
