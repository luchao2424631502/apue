#include "apue.h" 

#define BUFFSIZE 1024

static void
sig_tstp(int signo)
{
	sigset_t mask;
	
	sigemptyset(&mask);
	sigaddset(&mask,SIGTSTP);
	// 取消阻塞自己(这里有问题,signal并不在捕捉函数执行前　阻塞信号自己)
	sigprocmask(SIG_UNBLOCK,&mask,NULL);
	
	//默认处理方式,即终止进程
	signal(SIGTSTP,SIG_DFL);
	
	//向自己发送此信号
	kill(getpid(),SIGTSTP);

	//　此时进程已经转为后台进程组中的进程,等待SIGCONT才能继续执行
	// 再次注册,相同的行为
	signal(SIGTSTP,sig_tstp);

}
 
int main(int argc,char *argv[]) {
	int n;
	char buf[BUFFSIZE];
	
	//第一次注册:　原来是默认就改成sig_tstp,否则改成sig_ign
	if (signal(SIGTSTP,SIG_IGN) == SIG_DFL)
		signal(SIGTSTP,sig_tstp);	

	while ((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO,buf,n) < n)
			err_sys("write error");

	// 检测最后一次的复制是否出错
	if (n < 0)
		err_sys("read error");
    exit(0);
}
