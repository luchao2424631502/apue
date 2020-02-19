#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void 
abort(void) 
{
	sigset_t mask;
	struct sigaction action;

	// 首先取得原来的信号处理方式
	sigaction(SIGABRT,NULL,&action);
	if (action.sa_handler == SIG_IGN) {
		// 设置为默认处理
		action.sa_handler = SIG_DFL;
		sigaction(SIGABRT,&action,NULL);
	}
	// 如果处理方式是默认处理
	if (action.sa_handler == SIG_DFL)
		// 进程结束前,冲洗所有的标准IO流
		fflush(NULL);

	// 在向自己发送信号之前,确保sigabrt信号没有被阻塞
	sigfillset(&mask);
	sigdelset(&mask,SIGABRT);
	// 阻塞其他所有的信号,确保sigabrt未被阻塞
	sigprocmask(SIG_SETMASK,&mask,NULL);
	// 向自己发送信号
	kill(getpid(),SIGABRT);

	// 如果用户自己设置了信号捕捉函数,则会来到这里
	// 再次冲洗标准IO流,因为信号捕捉函数里面可能用到标准IO
	fflush(NULL);
	// 信号捕捉函数设置成默认
	action.sa_handler = SIG_DFL;
	sigaction(SIGABRT,&action,NULL);
	// 保证sigabrt信号没有被阻塞
	sigprocmask(SIG_SETMASK,&mask,NULL);
	// 再次向自己发出信号
	kill(getpid(),SIGABRT);
	exit(1);
}

int main(int argc,char *argv[]) {
	abort(); 
    exit(0);
}
