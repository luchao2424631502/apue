/*************************************************************************
    > File Name: select.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月23日 星期一 11时27分11秒
 ************************************************************************/

#include <stdio.h> 
#include <fcntl.h>
#include <errno.h>
#include <sys/select.h>
#include <unistd.h>
#include <stdlib.h>

#define PERR(msg) do { perror(msg); exit(1); } while(0);
 
// 给出指定描述符,从描述符中读取
int process(char *prompt,int fd)
{
	int n;
	char buf[64];
	char line[64];
	n = read(fd,buf,64);
	if (n < 0) { PERR("read"); }
	else if (n == 0)
	{
		sprintf(line,"%s closed\n",prompt);
		printf("%s\n",line);
		return 0;
	}
	else if (n > 0)
	{
		buf[n] = 0;
		sprintf(line,"%s say:%s",prompt,buf);
		printf("%s\n",line);
	}
	return n;
}

// io多路复用,读取指定描述符数据
int main(int argc,char *argv[]) 
{
	int n,res;
	char buf[64];

	fd_set set;
	FD_ZERO(&set);

	int fd1 = open("a.fifo",O_RDONLY);
	printf("open pipe: fd = %d\n",fd1);
	int fd2 = open("b.fifo",O_RDONLY);
	printf("open pipe: fd = %d\n",fd2);

	FD_SET(STDIN_FILENO,&set);
	FD_SET(fd1,&set);
	FD_SET(fd2,&set);

	int maxfd = fd2 + 1;
	
	while(1)
	{
		fd_set tempset = set;
		res = select(maxfd,
				&tempset,NULL,NULL,
				NULL);
		// select 出错 无描述符准备好,且捕捉到信号
		if (res < 0) { PERR("select"); }
		// 超时,且无描述符准备好
		else if (res == 0) continue;
		
		if (FD_ISSET(STDIN_FILENO,&tempset)) 
		{
			// 
			n = process("STDIN_FILENO",STDIN_FILENO);
			// 描述符关闭,从set中移出去
			if (n == 0) FD_CLR(STDIN_FILENO,&set);
		}
		if (FD_ISSET(fd1,&tempset))
		{
			n = process("fd1",fd1);
			if (n == 0) FD_CLR(fd1,&set);
		}
		if (FD_ISSET(fd2,&tempset))
		{
			n = process("fd2",fd2);	
			if (n == 0) FD_CLR(fd2,&set);
		}
	}
    return 0;
}
