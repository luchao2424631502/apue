#include "apue.h" 
#include <setjmp.h>
#include <time.h>

#include <errno.h>
#include "pr_mask.h"

static void sig_usr1(int);
static void sig_alrm(int);
static sigjmp_buf jmpbuf;
// 不能优化的全局静态原子变量?
static volatile sig_atomic_t canjump;

int main(int argc,char *argv[]) {
	// 注册SIGUSR1信号处理程序
	if (signal(SIGUSR1,sig_usr1) == SIG_ERR)
		err_sys("signal (SIGUSR1) error");
	if (signal(SIGALRM,sig_alrm) == SIG_ERR)
		err_sys("signal (SIGALRM) error");

	pr_mask("starting main: ");

	// 设置jmp断点,保存栈信息jmpbuf,和信号mask
	if (sigsetjmp(jmpbuf,1)) {
		
		pr_mask("ending main: ");

		exit(0);
	}

	//由于信号异步,这里使用(全局sig_atomic_t类型)变量保护jmpbuf在设置完后,信号处理程序才能运行
	canjump = 1;

	// 无限循环调用pause()等待信号 
	for ( ; ; )
		pause();

}

static void sig_usr1(int signo) {
	time_t starttime;
	if (canjump == 0)
		return ;

	// 广播sig_usr1当前接受到的信号
	pr_mask("starting sig_usr1: ");

	// 3秒后发出SIGALRM信号
	alarm(3);
	starttime = time(NULL);

	// 等5秒,但是在3s的时候SIGALRM信号发过来了
	for ( ; ; ) 
		if (time(NULL) > starttime + 5)
			break;

	// 再次广播
	pr_mask("finishing sig_usr1: ");
	
	// 保护跳转缓冲仍被设置
	canjump = 0;
	// jmp回去,返回值是1
	siglongjmp(jmpbuf,1);
}

static void sig_alrm(int signo) { pr_mask("in sig_alrm: "); }

