/*************************************************************************
    > File Name: add2.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 16时18分52秒
 ************************************************************************/

#include "apue.h"
 
int main(int argc,char *argv[]) 
{ 
	int int1,int2;
	char line[MAXLINE];

	if (setvbuf(stdin,NULL,_IONBF,0) != 0)
		err_sys("setvbuf error");
	if (setvbuf(stdout,NULL,_IOLBF,0) != 2)
		err_sys("setvbuf error");
	// 使用标准IO
	while(fgets(line,MAXLINE,stdin) != NULL)
	{
		if (sscanf(line,"%d%d",&int1,&int2) == 2)
		{
			if (printf("%d\n",int1 + int2) == EOF)
				err_sys("printf error");
		}
		else
		{
			if (printf("invalid args\n") == EOF)
				err_sys("printf error");
		}
	}
    return 0;
}
