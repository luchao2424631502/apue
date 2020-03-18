/*************************************************************************
    > File Name: lockfile.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月18日 星期三 18时52分01秒
 ************************************************************************/

#include <unistd.h>
#include <fcntl.h>

int lockfile(int fd) {
	struct flock fl;

	fl.l_type = F_WRLCK;
	fl.l_start = 0;
	fl.l_whence = SEEK_SET;
	fl.l_len = 0;
	
	// 通过file control锁住文件字节
	return fcntl(fd,F_SETLK,&fl);
}

#define lockfile(fd) write_lock((fd),0,SEEK_SET,0)
