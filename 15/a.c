/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 21时09分58秒
 ************************************************************************/

#include "apue.h"

static int pfd1[2],pfd2[2];

// 开辟2个父子进程间的管道
void
TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
		err_sys("pipe error");
}

void
TELL_PARENT(pid_t pid)
{
	// 向管道写,就能是read不阻塞
	if (write(pfd2[1],"c",1) != 1)
		err_sys("write error");
}

// 子进程阻塞
void 
WAIT_PARENT()
{
	char c;
	if (read(pfd1[0],&c,1) != 1)
		err_sys("read error");
	if (c != 'p')
		err_quit("WAIT_PARENT: incorrect data");
}

void 
TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1],"p",1) != 1)
		err_sys("write error");
}

void 
WAIT_CHILD(void)
{
	char c;
	if (read(pfd2[0],&c,1) != 1)
		err_sys("read error");

	if (c != 'c')
		err_quit("WAIT_CHILD: incorrect data");
}
 
int main(int argc,char *argv[]) 
{ 
    return 0;
}
