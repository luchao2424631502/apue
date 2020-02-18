#include "apue.h" 
#include <errno.h>

void pr_mask(const char *str) {
	sigset_t sigset;
	int      errno_save;

	// 首先保存错误
	errno_save = errno;
	// 得到当前信号mask
	if (sigprocmask(0,NULL,&sigset) < 0)
		err_ret("sigprocmask error");
	else {
		printf("%s",str);
		if (sigismember(&sigset,SIGINT))
			printf(" SIGINT");
		if (sigismember(&sigset,SIGQUIT))
			printf(" SIGQUIT");
		if (sigismember(&sigset,SIGALRM))
			printf(" SIGALRM");
		if (sigismember(&sigset,SIGUSR1))
			printf(" SIGUSR1");
		printf("\n");
	}
	errno = errno_save;
}

int main(int argc,char *argv[]) {
	sigset_t sigset;
	sigaddset(&sigset,SIGUSR1);
	// 修改
	sigprocmask(SIG_SETMASK,&sigset,NULL);
	pr_mask("now ");
    exit(0);
}
