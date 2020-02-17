#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include "apue.h"
 
static jmp_buf env_alrm;

static void 
sig_alrm(int signo) { longjmp(env_alrm,1); }

unsigned int 
sleep2(unsigned int seconds) {
	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		return seconds;
	if (setjmp(env_alrm) == 0) {
		alarm(seconds);
		pause();
	}
	return alarm(0);
}

static void 
sig_int(int signo) {
	int i,j;
	// 禁止编译器优化,每次都之间访问内存
	volatile int k;

	printf("\nsig_int starting\n");
	for (int i=0; i < 300000; i++)
		for (int j=0; j < 4000; j++)
			k += i * j;
	printf("sig_int finished\n");
}

int main(int argc,char *argv[]) {
	unsigned int unslept;
	// 接受SIGINT信号的处理函数
	if (signal(SIGINT,sig_int) == SIG_ERR)
		err_sys("signal (SIGINT) error");
	// 
	unslept = sleep2(5);
	printf("sleep2 returned: %u\n",unslept);
    exit(0);
}

