/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月21日 星期六 21时54分48秒
 ************************************************************************/

#include <stdio.h> 
#include <fcntl.h>
#include <unistd.h> 
#include <sys/select.h>

// 给　该文件描述符指定的文件(字节)加锁
int lockfile(int fd) 
{
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_start = 0;
	lock.l_whence = SEEK_SET;
	lock.l_len = 0;

	// file control 文件控制
	return fcntl(fd,F_SETLK,&lock);
}


int main(int argc,char *argv[]) {
	fd_set readset,writeset;
	FD_ZERO(&readset);
	FD_ZERO(&wirteset);
	FD_SET(0,&readset);
	FD_SET(3,&readset);
	FD_SET(1,&writeset);
	FD_SET(2,&writeset);
	
	// select一直阻塞
	select(4,&readset,&writeset,NULL,NULL);

	printf("%d %d\n",FD_ISSET(3,&set),FD_ISSET(5,&set));
    return 0;
}
