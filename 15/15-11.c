/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月29日 星期日 09时06分04秒
 ************************************************************************/

#include "apue.h"
#include <sys/wait.h>

#define PAGER "${PAGER:-more}"
 
int main(int argc,char *argv[]) 
{ 
	char line[MAXLINE];
	FILE *fpin,*fpout;

	if (argc != 2)
		err_quit("usage: a.out <pathname>");

	if ((fpin = fopen(argv[1],"r")) == NULL)
		err_sys("can't open %s",argv[1]);

	if ((fpout = popen(PAGER,"w")) == NULL)
		err_sys("popen error");

	while(fgets(line,MAXLINE,fpin) != NULL)
	{
		if (fputs(line,fpout) == EOF)
			err_sys("fputs error to pipe");
	}

	if (ferror(fpin))
		err_sys("fgets error");

	if (pclose(fpout) == -1)
		err_sys("pclose error");

    return 0;
}
