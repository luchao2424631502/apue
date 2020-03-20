/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月19日 星期四 09时51分59秒
 ************************************************************************/

#include "apue.h"
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
 
int main(int argc,char *argv[]) {
	int fd;
	pid_t pid;
	char buf[5];
	struct stat statbuf;

	if (argc != 2) {
		fprintf(stderr,"usage: %s filename\n",argv[0]);
		exit(1);
	}
	if ((fd = open(argv[1],O_RDWR | O_CREAT | O_TRUNC,FILE_MODE)) < 0)
		err_sys("open error");
	if (write(fd,"abcdef",6) != 6)
		err_sys("write error");

	if (fstat(fd,&statbuf) < 0)
		err_sys("fstat error");
	// 组执行位关闭,打开设置组ID位
	if (fchmod(fd,(statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
		err_sys("fchmod error");

	TELL_WAIT();

	if ((pid = fork()) < 0) 
		err_sys("fork error");
	else if (pid > 0) {
		// 写锁覆盖所有字节
		if (write_lock(fd,0,SEEK_SET,0) < 0)
			err_sys("write_lock error");

		TELL_CHILD(pid);

		if (waitpid(pid,NULL,0) < 0)
			err_sys("waitpid error");
	} else {
		// 子进程等待父进程
		WAIT_PARENT();

		// 文件描述符改成非阻塞IO
		set_fl(fd,O_NONBLOCK);
	
		// read_lock应该返回-1,因为在写锁上面加读锁只能出错
		if (read_lock(fd,0,SEEK_SET,0) != -1)
			err_sys("child: read_lock succeeded");
		// 读锁加上去出错返回 errno
		printf("read_lock of already-locked region returns %d\n",errno);
		
		// 知道被锁住了,那么自己控制程序等锁释放了在读,(建议性锁)
		if (lseek(fd,0,SEEK_SET) == -1 )
			err_sys("lseek error");
		if (read(fd,buf,2) < 0)
			err_ret("read failed (mandatory locking works)");
		else 
			printf("read OK (no mandatory locking), buf = %2.2s\n",buf);
	}

    return 0;
}
