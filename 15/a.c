/*************************************************************************
    > File Name: ad.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 15时32分47秒
 ************************************************************************/

#include "apue.h"
 
static void sig_pipe(int);

int main(int argc,char *argv[]) 
{ 
	int n,fd1[2],fd2[2];
	pid_t pid;
	char line[MAXLINE];

	if (signal(SIGPIPE,sig_pipe) == SIG_ERR)
		err_sys("signal error");

	// 创建2个管道
	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		err_sys("pipe error");

	if ((pid = fork()) < 0)
	{
		err_sys("fork error");	
	}
	else if (pid > 0)
	{
		// 第一条管道用来写, 第二条管道用来读
		close(fd1[0]);
		close(fd2[1]);

		// 从父进程标准输入读
		while(fgets(line,MAXLINE,stdin) != NULL)
		{
			n = strlen(line);
			// 通过管道向子进程写
			if (write(fd1[1],line,n) != n)
				err_sys("write error to pipe");
			// 从第２条管道读
			if ((n = read(fd2[0],line,MAXLINE)) < 0)
				err_sys("read error from pipe");
			if (n == 0)
			{
				err_msg("child closed pipe");
				break;
			}
			line[n] = 0;
			// 将处理完的内容　打印到标准输出
			if (fputs(line,stdout) == EOF)
				err_sys("fputs error");
		}

		// 判断标准输入 是否 有错误
		if (ferror(stdin))
			err_sys("fgets error on stdin");
		exit(0);
	}
	else 
	{
		// 从第一条管道读
		close(fd1[1]);
		// 第二条管道来写
		close(fd2[0]);
		// 修改协程的的std io 为管道的描述符
		if (fd1[0] != STDIN_FILENO) 
		{
			if (dup2(fd1[0],STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2 error to stdin");
			close(fd1[0]);
		}

		if (fd2[1] != STDOUT_FILENO)
		{
			if (dup2(fd2[1],STDOUT_FILENO) != STDOUT_FILENO)
				err_sys("dup2 error to stdout");
			close(fd2[1]);
		}

		// 协程　处理从管道来的数据
		if (execl("./add2","add",(char *)0) < 0)
			err_sys("execl error");
	}
    return 0;
}

static void sig_pipe(int signo)
{
	printf("SIGPIPE caught\n");
	// 信号处理程序直接终止程序
	exit(1);
}
