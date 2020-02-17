#include <stdio.h>
#include "apue.h"

// 判断是否无缓冲区
int is_unbuffered(FILE *fp) { return (fp->_flags & _IO_UNBUFFERED); }

// 判断是否是行缓冲区
int is_linebuffered(FILE *fp) { return (fp->_flags & _IO_LINE_BUF); }

// 得到缓冲区大小
int buffer_size(FILE *fp) { return (fp->_IO_buf_end - fp->_IO_buf_base); }

void pr_stdio(const char *name,FILE *fp) {
	printf("stream = %s",name);
	if (is_unbuffered(fp))
		printf("unbuffered");
	else if (is_linebuffered(fp))
		printf("line buffered");
	else
		printf("fully buffered");
	printf(" , buffer size = %d\n",buffer_size(fp));
	return ;
}

void my_setbuf(FILE *restrict fp,char *restrict buf) {
	int len = BUFSIZ;
	// 先处理特殊情况
	if (buf == NULL || fp == stderr) {
		if (setvbuf(fp,NULL,_IONBF,0) != 0)
			err_sys("setvbuf _IONBF failure");
		return ;
	} 

	int fd = fileno(fp);
	struct stat stbuf;
	// 首先获得文件底层类型
	if (fstat(fd,&stbuf) == -1)
		err_sys("get file status error");
	// 将特殊的文件类型设置为行缓冲区
	else if (S_ISFIFO(stbuf.st_mode) || S_ISSOCK(stbuf.st_mode) || 
			S_ISCHR(stbuf.st_mode) || S_ISBLK(stbuf.st_mode)) {
		if (setvbuf(fp,buf,_IOLBF,len) != 0)
			err_sys("setvbuf _IOLBF failure");
	} else {
		// 剩下的全部设置为fully buf
		if (setvbuf(fp,buf,_IOFBF,len) != 0)
			err_sys("setvbuf _IOFBF failure");
	}
	return ;
}

int main(int argc,char *argv[]) {
	FILE *fp = fopen("2.in","r+");
	char buf[BUFSIZ];
	pr_stdio("fp",fp);
    return 0;
}
