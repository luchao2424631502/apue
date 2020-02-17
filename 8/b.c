#include "apue.h"
#include <fcntl.h>
#include <stdio.h>

int main(int argc,char *argv[]) {
	int status,fd;
	for (int i=0; i<argc; ++i)
		printf("argv[%d] = %s ",i,argv[i]);
	
	if (argv[1] != NULL)
		sscanf(argv[1],"%d",&fd);
	else 
		fd = -1;
	if ((status = fcntl(fd,F_GETFD,0)) < 0)
		err_sys("fcntl error");
	if (status & FD_CLOEXEC) 
		printf("close on exec on");
	else 
		printf("close on exec close");
    exit(0);
}
