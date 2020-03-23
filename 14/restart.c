/*************************************************************************
    > File Name: restart.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月23日 星期一 10时30分31秒
 ************************************************************************/

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void handler(int sig) 
{
	switch(sig) 
	{
		case SIGUSR1:
			printf("SIGUSR1\n");
			break;
		case SIGALRM:
			printf("SIGALRM\n");
			break;
	}
}

int main(int argc,char *argv[]) 
{
	char buf[16] = {0};
	int n = 0;
	printf("i'm %d\n",getpid());

	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// sigaction函数使用该标志允许应用程序请求　重启动　被中断的系统调用
	if (argc >= 2 && strcmp(argv[1],"-r") == 0) 
	{
		// 开启 中断重启标志
		act.sa_flags |= SA_RESTART;
	}

	if (sigaction(SIGUSR1,&act,NULL) < 0)
	{
		perror("signal sigusr1");
	}

	if (sigaction(SIGALRM,&act,NULL) < 0)
	{
		perror("signal sigalrm");
	}

	while (1) 
	{
		// 被信号处理程序中断后　没有自动处理　则通过error找到错误
		if ((n = read(STDIN_FILENO,buf,15)) < 0)
		{
			if (errno == EINTR) 
			{
				printf("Interrupted by signal\n");
			}
		}
		else
		{
			// 成功读取到
			buf[n] = 0;
			printf("%s",buf);
		}
	}
    return 0;
}
