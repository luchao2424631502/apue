/*************************************************************************
    > File Name: select_sig.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月23日 星期一 15时21分25秒
 ************************************************************************/

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
 
#define PERR(msg) do { perror(msg); exit(1); } while(0);

void handler(int sig)
{
	if (sig == SIGALRM)
		puts("hello SIGALRM");
}

// 返回读取字符个数
int process(char *prompt,int fd)
{
	char buf[64];
	char line[64];
	int n = read(fd,buf,64);
	if (n < 0) { PERR("read"); }
	// 读取成功
	else if (n == 0)
	{
		sprintf(line,"%s closed\n",prompt);
		puts(line);
		return 0;
	}
	else if (n > 0) 
	{
		buf[n] = 0;
		sprintf(line,"%s say: %s",prompt,buf);
		puts(line);
	}
	return n;
}

int main(int argc,char *argv[]) 
{ 
	int n,res,maxfd;
	char buf[64];
	struct sigaction sa;
	fd_set set;

	printf("pid = %d\n",getpid());

	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGALRM,&sa,NULL);

	FD_ZERO(&set);
	FD_SET(STDIN_FILENO,&set);

	maxfd = STDIN_FILENO+1;

	while(1)
	{
		fd_set tempset = set;
		res = select(maxfd,
				&tempset,NULL,NULL,
				NULL);
		if (res < 0)
		{
			// select函数被信号处理程序中断,且不自动重启
			if (errno == EINTR)
			{
				perror("select");
				continue;
			}
			PERR("select");
		}
		// 描述符都没有准备好
		else if (res == 0)
		{
			continue;
		}

		if (FD_ISSET(STDIN_FILENO,&tempset))
		{
			n = process("stdin_fileno",STDIN_FILENO);
			if (n == 0) FD_CLR(STDIN_FILENO,&set);
		}
	}

    return 0;
}
