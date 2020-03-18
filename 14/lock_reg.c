/*************************************************************************
    > File Name: lock_reg.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月18日 星期三 14时45分03秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>

// cmd 是fcntl设置记录锁的选项
int 
lock_reg(int fd,int cmd,int type,off_t offset,int whence,off_t len) {
	// 线程同步的读写锁用到文件中,记录锁(文件锁)
	struct flock lock;

	// 记录锁的类型
	lock.l_type = type;
	lock.l_whence = whence;
	lock.l_start = offset;
	lock.l_len = len;

	return fcntl(fd,cmd,&lock);
}
