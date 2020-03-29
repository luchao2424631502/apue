/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 12时28分49秒
 ************************************************************************/

#include "apue.h"
#include <sys/wait.h>
 
int main(int argc,char *argv[]) 
{ 
	char line[MAXLINE];
	FILE *fpin;

	// 从管道中　读
	if ((fpin = popen("./myuclc","r")) == NULL)
		err_sys("popen error");

	for ( ; ; )
	{
		fputs("prompt> ",stdout);
		fflush(stdout);

		if (fgets(line,MAXLINE,fpin) == NULL)
			break;

		if (fputs(line,stdout) == EOF)
			err_sys("fputs error to pipe");
	}

	// 关闭FILE *
	if (pclose(fpin) == -1)
		err_sys("pclose error");

	putchar('\n');

    return 0;
}
