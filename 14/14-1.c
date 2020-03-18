/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月18日 星期三 13时53分35秒
 ************************************************************************/

#include "apue.h"
#include <errno.h>
#include <fcntl.h>

char buf[500000];
 
int main(int argc,char *argv[]) {
	int ntowrite,nwrite;
	char *ptr;

	// 读取数据
	ntowrite = read(STDIN_FILENO,buf,sizeof(buf));
	fprintf(stderr,"read %d bytes\n",ntowrite);

	set_fl(STDOUT_FILENO,O_NONBLOCK);

	ptr = buf;
	while(ntowrite > 0) {
		errno = 0;
		nwrite = write(STDOUT_FILENO,ptr,ntowrite);
		if (nwrite > 0) {
			ptr += nwrite;
			ntowrite -= nwrite;
		}
	}

	clr_fl(STDOUT_FILENO,O_NONBLOCK);

    return 0;
}
