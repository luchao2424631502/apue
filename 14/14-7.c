/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月18日 星期三 16时14分55秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>

static void 
lockbyte(const char *name,int fd,off_t offset) {
	// 尝试锁住1字节
	if (writew_lock(fd,offset,SEEK_SET,1) < 0)
		err_sys("%s : writew_lock error",name);
	printf("%s: got the lock,byte %lld\n",name,(long long)offset);
}

int main(void) {
	int fd;
	pid_t pid;

	if ((fd = creat("templock",FILE_MODE)) < 0)
		err_sys("creat error");

	if (write(fd,"ab",2) != 2)
		err_sys("write error");

	TELL_WAIT();

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid == 0) {
		// 锁住文件的第一个字节
		lockbyte("child",fd,0);
		TELL_PARENT(getppid());
		WAIT_PARENT();
		// 锁住文件的第2个字节
		lockbyte("child",fd,1);
	} else {
		lockbyte("parent",fd,1);
		TELL_CHILD(pid);
		WAIT_CHILD();
		lockbyte("parent",fd,0);
	}
	exit(0);
}

