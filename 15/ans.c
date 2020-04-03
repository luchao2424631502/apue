/*************************************************************************
    > File Name: ans.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月02日 星期四 10时03分53秒
 ************************************************************************/

#include "apue.h"
 
// 15-18删除信号处理程序,标准IO替代read write
int main(int argc,char *argv[]) 
{ 
	int n,fd1[2],fd2[2];
	pid_t pid;
	char line[MAXLINE];

	if (pipe(fd1) < 0 || pipe(fd2) < 0)
		err_sys("pipe error");

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid > 0)
	{
		// 父进程从第一个通道写,第２个通道读
		close(fd1[0]);
		close(fd2[1]);

		while(fgets(line,MAXLINE,stdin) != NULL)
		{
			n = strlen(line);
			// 向子进程写
		//	if (write(fd1[1],line,n) != n)
		//		err_sys("write error to pipe");
		//	// 这里的阻塞读的意思就是让协程处理数据,然后返回
		//	// 从子进程读
		//	if ((n = read(fd2[0],line,MAXLINE)) < 0)
		//		err_sys("read error from pipe");
			FILE *out = fdopen(fd1[1],"w");
			FILE *in  = fdopen(fd2[0],"r");

			if (in == NULL || out == NULL)
				err_sys("fdopen error");

			if (setvbuf(in,NULL,_IOLBF,0) < 0)
				err_sys("setvbuf error");
			if (setvbuf(out,NULL,_IOLBF,0) < 0)
				err_sys("setvbuf error");
			
			// 用标准IO向 FILE*里面写
			if (fputs(line,out) == EOF)
				err_sys("fputs error to pipe");
			if (fgets(line,MAXLINE,in) == NULL)
			{
				err_msg("child closed pipe");
				break;
			}

			if (n == 0)
			{
				err_msg("child closed pipe");
				break;
			}
			line[n] = 0;
			if (fputs(line,stdout) == EOF)
				err_sys("fputs error");
		}

		if (ferror(stdin))
			err_sys("fgets error on stdin");
		
		exit(0);
	}
	else 
	{
		// 子进程第一个通道读,第２个通道写
		close(fd1[1]);
		close(fd2[0]);
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

		// 协成exec处理数据
		if (execl("./add2","add2",(char *)0) < 0)
			err_sys("execl error");
	}
	exit(0);
}
