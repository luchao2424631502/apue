/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月25日 星期三 15时16分44秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>

// 每次copy 1gb字节
#define COPYINCR (1024*1024*1024)
 
int main(int argc,char *argv[]) 
{ 
	int fdin,fdout;
	void *src,*dst;
	size_t copysz;
	struct stat sbuf;
	off_t fsz = 0;

	if (argc != 3)
		err_quit("usage: %s <fromfile> <tofile>",argv[0]);

	if ((fdin = open(argv[1],O_RDONLY)) < 0)
		err_sys("can't open %s for reading",argv[1]);

	// trunc目的截断文件(文件长度=0),然后,因为要写
	if ((fdout = open(argv[2],O_RDWR | O_CREAT | O_TRUNC,FILE_MODE)) < 0)
		err_sys("can't creat %s for writing",argv[2]);

	// 获取读文件的长度
	if (fstat(fdin,&sbuf) < 0)
		err_sys("fstat error");

	// 将要写的文件长度修改成读文件的长度
	if (ftruncate(fdout,sbuf.st_size) < 0)
		err_sys("ftruncate error");

	while(fsz < sbuf.st_size)
	{
		// 判断每次copy的量是否大于1G byte
		if ((sbuf.st_size - fsz) > COPYINCR)
			copysz = COPYINCR;
		else
			copysz = sbuf.st_size - fsz;

		// 读文件内存映射
		if ((src = mmap(0,copysz,PROT_READ,MAP_SHARED,
						fdin,fsz)) == MAP_FAILED)
			err_sys("mmap error for input");
		if ((dst = mmap(0,copysz,PROT_READ | PROT_WRITE,
						MAP_SHARED,fdout,fsz)) == MAP_FAILED)
			err_sys("mmap error for output");	
		memcpy(dst,src,copysz);
		// 取消内存映射,参数传入大小
		munmap(src,copysz);
		munmap(dst,copysz);
		fsz += copysz;
	}
    return 0;
}
