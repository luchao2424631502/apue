/*************************************************************************
    > File Name: lock_test.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月18日 星期三 15时27分41秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>

pid_t 
lock_test(int fd,int type,off_t offset,int whence,off_t len) {
	struct flock lock;

	lock.l_type = type;
	lock.l_start = offset;
	lock.l_whence = whence;
	lock.l_len = len;

	if (fcntl(fd,F_GETLK,&lock) < 0)
		err_sys("fcntl error");

	// 文件区域没有被其他的锁占领,
	if (lock.l_type == F_UNLCK)
		return 0;
	return lock.l_pid; // 文件区域被占领,返回当前锁拥有者的pid
}
