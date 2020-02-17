#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h> 
#include "apue.h"

int testDir() {
	DIR *dirp;

	if ((dirp = opendir("/home/luchao/Documents/Apue/8")) == NULL)
		err_sys("opendir error");
	int fd = dirfd(dirp);
	int status;
	if ((status = fcntl(fd,F_GETFD)) == -1)
		err_sys("fcntl error");
	// 子进程执行exec时关闭所有打开的文件描述符,
	if (status & FD_CLOEXEC)
		printf("close-on-exec on");
	else 
		printf("close-on-exec close");

	// 关闭　close on exec 标志,也就是在调用exec时,不关闭所有的文件描述符
//	status &= (~FD_CLOEXEC); 
//	if ((status = fcntl(fd,F_SETFD,status)) < 0)
//		err_sys("fcntl2 error");

	return fd;
}

int testFile() {
	int fd;
	if ((fd = open("/home/luchao/Documents/Apue/8/1.in",O_RDWR)) == -1)
		err_sys("open error");
	int status;
	if ((status = fcntl(fd,F_GETFD,0)) == -1)
		err_sys("fcntl error");
	if (status & FD_CLOEXEC)
		printf("close on exec on");
	else 
		printf("close on exec close");
	
	return fd;	
}

int main(int argc,char *argv[]) {
//	int fd = testDir();
	int fd = testFile();
//	setbuf(stdout,NULL);
	pid_t pid;
	char buf[10];
	sprintf(buf,"%d",fd);

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) {	
		char *path = "/home/luchao/Documents/Apue/8/b";
		if (execl(path,"b",buf,(char *)0) < 0)
			err_sys("execl error");
	}

	if (waitpid(pid,NULL,0) == -1)
		err_sys("error waitpid");
    exit(0);
}
