#include <stdio.h>
#include "apue.h"
 
void pr_stdio(const char *,FILE *);

int is_unbuffered(FILE *);

int is_linebuffered(FILE *);

int buffer_size(FILE *);

void my_setbuf(FILE *restrict fp,char *restrict buf);


// 用setvbuf 实现　mysetbuf
int main(int argc,char *argv[]) {
	char buf[BUFSIZ];
	FILE *fp = fopen("2.in","r+");
	pr_stdio("2.in",fp);
	
	fputs("o ",fp);
	fseek(fp,0,SEEK_SET);
	fgets(buf,BUFSIZ,fp);
	printf("%s",buf);
	fclose(fp);
    return 0;
}

void my_setbuf(FILE *restrict fp,char *restrict buf) {
	// stdio常量
	int len = BUFSIZ;
	// 无缓冲区　和　　stderr
	if (buf == NULL || fp == stderr) {
		// 出错返回非０
		if (setvbuf(fp,NULL,_IONBF,0) != 0)
			err_sys("setvbuf _IONBF failure");
		return ;
	}

	// 返回文件流所使用的文件描述符
	int fd = fileno(fp);
	struct stat stbuf;
	if (fstat(fd,&stbuf) == -1)
		err_sys("get file status error");
	// 得到文件流关联的文件的类型
	else if (S_ISFIFO(stbuf.st_mode) || S_ISSOCK(stbuf.st_mode) || 
		S_ISCHR(stbuf.st_mode) || S_ISBLK(stbuf.st_mode)) {
		// pipe socket character block use linebuf
		if (setvbuf(fp,buf,_IOLBF,len) != 0)
			err_sys("setvbuf _IOLBF failure");
	}
	else {
		// 剩下的都处理为　fullbuf
		if (setvbuf(fp,buf,_IOFBF,len) != 0)
			err_sys("setvbuf _IOFBF failure");
	}
	return ;
}

void pr_stdio(const char *name,FILE *fp) {
	printf("stream = %s ",name);
	if (is_unbuffered(fp))
		printf("unbuffered");
	else if (is_linebuffered(fp))
		printf("line buffered");
	else
		printf("fully buffered");
	printf(", buffer size = %d\n",buffer_size(fp));
}

int is_unbuffered(FILE *fp) { return (fp->_flags & _IO_UNBUFFERED); }

int is_linebuffered(FILE *fp) { return (fp->_flags & _IO_LINE_BUF); }

int buffer_size(FILE *fp) { return (fp->_IO_buf_end - fp->_IO_buf_base); }

