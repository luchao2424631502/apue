#include "apue.h"
 
static void sig_alrm(int signo) {
	
}

static void sig_alrm_1(int signo) {
	printf("alarm\n");
}

unsigned int sleep1(unsigned int seconds) {
	int res;
	if ((res = alarm(seconds)) < seconds) {
		//恢复原来调用者的闹钟
		alarm(res);
		pause();
		// 设置自己的闹钟
		alarm(seconds - res);
		if (signal(SIGALRM,sig_alrm) == SIG_ERR)
			return seconds;
		pause();
		return 0;
	} else if (res > seconds) {
		typedef void (*func)(int);
		// 保存原来的signal配置,return时恢复
		func old = signal(SIGALRM,sig_alrm);
		pause();
		// 恢复原来的闹钟和signal
		alarm(res - seconds);
		signal(SIGALRM,old);
		return 0;
	} 
	else {
		if (signal(SIGALRM,sig_alrm) == SIG_ERR) 
			return seconds;
		// 暂停等待信号
		pause();
		return 0;
	}
}


int main(int argc,char *argv[]) {
	alarm(5); 
	signal(SIGALRM,sig_alrm_1);
	sleep1(5);
	printf("slepp 10s\n");
	// 等待第一个闹钟
	pause();
    exit(0);
}
