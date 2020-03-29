/*************************************************************************
    > File Name: add.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 15时25分14秒
 ************************************************************************/

#include "apue.h"
 
int main(int argc,char *argv[]) 
{ 
	int n,int1,int2;
	char line[MAXLINE];

	while((n = read(STDIN_FILENO,line,MAXLINE)) > 0) 
	{
		line[n] = 0;
		if (sscanf(line,"%d%d",&int1,&int2) == 2)
		{
			sprintf(line,"%d\n",int1 + int2);
			n = strlen(line);
			if (write(STDOUT_FILENO,line,n) != n)
				err_sys("write error");
		}
		else
		{
			if (write(STDOUT_FILENO,"invalid args\n",13) != 13)
				err_sys("write error");
		}
	}
    return 0;
}
