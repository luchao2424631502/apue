#include "apue.h" 

#define BUFFSIZE 1024

static void
sig_tstp(int signo)
{
	sigset_t mask;


	sigemptyset(&mask);
	sigaddset(&mask,SIGTSTP);
	// 取消阻塞SIGTSTP
	sigprocmask(SIG_UNBLOCK,&mask,NULL);

	// 使用默认的信号捕捉函数
	signal(SIGTSTP,SIG_DFL);

	// 给自己发信号
	kill(getpid(),SIGTSTP);

	// 信号处理程序重新设置为捕捉
	signal(SIGTSTP,sig_tstp);

}

int main(int argc,char *argv[]) {
	int n;
	char buf[BUFFSIZE];

	// 判断以前的信号捕捉函数是不是 SIG_DFL,(判断是否为登录shell)
	if (signal(SIGTSTP,SIG_IGN) == SIG_DFL)
		// 以前是DFL,则设置成自己的,否则忽略
		signal(SIGTSTP,sig_tstp);

	while ((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO,buf,n) != n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

    exit(0);
}
