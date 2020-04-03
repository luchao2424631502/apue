/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月02日 星期四 17时18分24秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>
#include <sys/mman.h>
#include "local.h"

#define NLOOPS 1000
#define SIZE sizeof(long)

static int update(long *ptr)
{
	// 返回原来的值,然后+1
	return ((*ptr)++);
}
 
// 父子进程共享一块内存,然后同步使用
int main(int argc,char *argv[]) 
{ 
	int fd,i,counter;
	pid_t pid;
	void *area;
	
//	if ((fd = open("/dev/zero",O_RDWR)) < 0)
//		err_sys("open error");
//	if ((area = mmap(0,SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,
//					fd,0)) == MAP_FAILED)
//		err_sys("mmap error");
//	close(fd);
	int shmid;
	if ((shmid = shmget(IPC_PRIVATE,SIZE,IPC_CREAT|0600)) == -1)
		err_sys("shmget error");
	area = shmat(shmid,NULL,0);

	TELL_WAIT();

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid > 0)
	{
		for (i=0; i<NLOOPS; i+=2)
		{
			printf("parent: %d\n",i);
			if ((counter = update((long *)area)) != i)
				err_quit("parent: expected %d,got %d",i,counter);

			TELL_CHILD(pid);
			WAIT_CHILD();
		}
	
	}
	else 
	{
		for (i=1; i<NLOOPS+1; i+=2)
		{
			WAIT_PARENT();
			printf("child: %d\n",i);
			if ((counter = update((long *)area)) != i)
				err_quit("child: expected %d,got %d",i,counter);
			TELL_PARENT(getppid());
		}
	}
    return 0;
}
