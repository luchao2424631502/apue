#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>

static jmp_buf env_alrm;

static void sig_alrm(int signo) {
	longjmp(env_alrm,1);
}

unsigned int sleep2(unsigned int seconds) {
	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		return seconds;

	if (setjmp(env_alrm) == 0) {
		alarm(seconds);	
		// 设置闹钟后,执行信号处理程序,然后sig_alrm执行longjmp到if ,跳过if,避免pause不执行
		pause();
	}
	
	// 清楚原来的时钟设定,并返回清除的值
	return alarm(0);
}
 
int main(int argc,char *argv[]) {
 
    exit(0);
}
