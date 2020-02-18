#include "apue.h" 
#include <setjmp.h>
#include <time.h>

#include <errno.h>
#include "pr_mask.h"

static void sig_usr1(int);
static void sig_alrm(int);
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;
 
int main(int argc,char *argv[]) {
	if (signal(SIGUSR1,sig_usr1) == SIG_ERR)
		err_sys("signal (SIGUSR1) error");
	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		err_sys("signal (SIGALRM) error");

	pr_mask("start main: ");

	if (sigsetjmp(jmpbuf,1)) {
		
		pr_mask("end main: ");
		
		exit(0);
	}
	
	// 确保信号处理程序在sigsetjmp执行完后执行
	canjump = 1;

	//　无线循环等待信号
	for ( ; ; )
		pause();

}

static void sig_usr1(int signo) {
	time_t starttime;
	if (canjump == 0)
		return ;

	pr_mask("start sigusr1: ");
	
	alarm(3);
	starttime = time(NULL);

	for ( ; ; )
		if (time(NULL) > starttime + 5)
			break;

	pr_mask("finish sigusr1: ");

	// 恢复成0,
	canjump = 0;

	// 
	siglongjmp(jmpbuf,1);
}

static void sig_alrm(int signo) { pr_mask("in sig_alrm: "); }
