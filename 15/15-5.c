/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 19时42分57秒
 ************************************************************************/

#include "apue.h"
 
int main(int argc,char *argv[]) 
{ 
	int n;
	int fd[2];
	pid_t pid;
	char line[MAXLINE];

	// 返回环形
	if (pipe(fd) < 0)
		err_sys("pipe error");
	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid > 0)
	{
		// 父进程向子进程写
		close(fd[0]);

		char buf[] = "这是我写给你的话"; 
	
		if (write(fd[1],buf,strlen(buf)) == -1)
			err_sys("write error");
	}
	else 
	{
		close(fd[1]);
		n = read(fd[0],line,64);
		write(STDOUT_FILENO,line,64);
	}

    return 0;
}
