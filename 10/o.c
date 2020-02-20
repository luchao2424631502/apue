#include "apue.h" 
 
static void
sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void
sig_child(int signo)
{
	printf("caught SIGCHILD\n");
}

int main(int argc,char *argv[]) {
	if (signal(SIGINT,sig_int) == SIG_ERR)
		err_sys("signal (SIGINT) error");
	if (signal(SIGCHLD,sig_child) == SIG_ERR)
		err_sys("signal (SIGCHLD) error");
	
	// 阻塞SIGCHLD的原因是 避免在收到SIGCHLD信号执行捕捉函数时system函数还没执行完毕,造成system不能回收自己的子进程
	if (system("/bin/vim") < 0)
		err_sys("system() error");

    exit(0);
}
