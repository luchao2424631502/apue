/*************************************************************************
    > File Name: poll.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月23日 星期一 16时05分28秒
 ************************************************************************/

#include <unistd.h>
#include <stdlib.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h> 
 
#define PERR(msg) do { perror(msg); exit(1); } while(0);

void handler(int sig)
{
	if (sig == SIGINT)
	{
		puts("SIGINT");
	}
}

int process(char *prompt,int fd)
{
	char buf[64];
	char line[64];
	int n = read(fd,buf,64);
	if (n < 0)
	{
		PERR("read");
	}
	else if (n == 0)
	{
		//0表示文件末尾
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
	int n,res;
	char buf[64];

	struct pollfd fds[4];
	if (SIG_ERR == signal(SIGINT,handler)) 
	{
		PERR("signal");
	}

	int fd1 = open("a.fifo",O_RDONLY);
	printf("open pipe: fd = %d\n",fd1);
	int fd2 = open("b.fifo",O_RDONLY);
	printf("open pipe: fd = %d\n",fd2);
	// 空描述符
	int fd3 = 100;

	fds[0].fd = STDIN_FILENO;
	fds[1].fd = fd1;
	fds[2].fd = fd2;
	fds[3].fd = fd3;

	for (int i=0; i<4; i++)
	{
		fds[i].events = POLL_IN;
	}

	while(1)
	{
		// 一直阻塞
		res = poll(fds,4,-1);
		// 出错,
		if (res < 0)
		{
			if (errno == EINTR)
			{
				perror("poll");
				continue;
			}
			PERR("poll fault is not interrupt");
		}
		else if (res == 0)
		{
			// 无描述符准备好
			continue;
		}
		
		//　循环处理
		for (int i=0; i<4; i++)
		{
			if (fds[i].revents & POLLIN)
			{
				sprintf(buf,"fd = %d",i);
				n = process(buf,fds[i].fd);
				if (n == 0) fds[i].fd = -1;
			}
			if (fds[i].revents & POLLERR)
			{
				printf("fd = %d ERROR\n",i);
				fds[i].fd = -1;
			}
			if (fds[i].revents & POLLHUP)
			{
				printf("fd = %d hang up\n",i);
				fds[i].fd = -1;
			}
			if (fds[i].revents & POLLNVAL)
			{
				printf("fd %d Invalid request\n",i);
				fds[i].fd = -1;
			}
		}
	}
    return 0;
}
