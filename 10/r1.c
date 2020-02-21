// 对书中10.21最后程序的修改(在我linux版本下signal并没有阻塞调用该捕捉函数的信号)
#include "apue.h" 

#define BUFFSIZE 1024

static void
sig_tstp(int signo)
{
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask,SIGTSTP);
	// 首先取消此信号的阻塞
	sigprocmask(SIG_UNBLOCK,&mask,NULL);

	struct sigaction act;
	act.sa_handler = SIG_DFL;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	// 设置信号处理程序为默认(终止进程)
	sigaction(SIGTSTP,&act,NULL);

	// 向自己发出信号
	kill(getpid(),SIGTSTP);
	
	// 当sigcont信号到来时重新设置信号捕捉函数
	act.sa_handler = sig_tstp;
	sigaction(SIGTSTP,&act,NULL);
}
 
int main(int argc,char *argv[]) {
	int n;
	char buf[BUFFSIZE];
	
	struct sigaction act;
	act.sa_handler = sig_tstp;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	// 注册信号处理函数
	if (sigaction(SIGTSTP,&act,NULL) == -1)
		err_sys("sigaction error");
	
	while ((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO,buf,n) < n)
			err_sys("write error");

	if (n < 0)
		err_sys("read error");

    exit(0);
}
