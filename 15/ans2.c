/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月02日 星期四 15时02分36秒
 ************************************************************************/

#include "local.h"
#include <fcntl.h>
#include "apue.h"

#define FIFO "temp.fifo"
 
int main(int argc,char *argv[]) 
{ 
	int fdread,fdwrite;

	unlink(FIFO);
	if (mkfifo(FIFO,FILE_MODE) < 0)
		err_sys("mkfifo error");
	// 先读打开FIFO (非阻塞)
	if ((fdread = open(FIFO,O_RDONLY | O_NONBLOCK)) < 0)
		err_sys("oper error for reading");

	if ((fdwrite = open(FIFO,O_WRONLY)) < 0)
		err_sys("open error for writing");

	clr_fl(fdread,O_NONBLOCK);

    return 0;
}
