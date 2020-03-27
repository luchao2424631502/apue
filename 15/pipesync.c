/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月27日 星期五 09时47分45秒
 ************************************************************************/

#include "apue.h"

static int pfd1[2],pfd2[2];

void
TELL_WAIT(void)
{
	if (pipe(pfd1) < 0 || pipe(pfd2) < 0)
		err_sys("pipe error");
}

// 子进程告诉父进程,tell_parent == wait_child
void 
TELL_PARENT(pid_t pid)
{
	if (write(pfd2[1],"c",1) != 1)
		err_sys("write error");
}

// 子进程等待父进程
void
WAIT_PARENT()
{
	char c;
	if (read(pfd1[0],&c,1) != 1)
		err_sys("read error");
	if (c != 'p')
		err_quit("WAIT_PARENT: incorrect data");
}

// 父进程等待子进程
void
WAIT_CHILD()
{
	char c;
	if (read(pfd2[0],&c,1) != 1)
		err_sys("read error");

	if (c != 'c')
		err_quit("WAIT_CHILD: incorrect data");
}

void 
TELL_CHILD(pid_t pid)
{
	if (write(pfd1[1],"p",1) != 1)
		err_sys("write error");
}

int main(int argc,char *argv[]) 
{ 
    return 0;
}
