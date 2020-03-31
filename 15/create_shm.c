/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月30日 星期一 15时36分57秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <string.h>
 
int main(int argc,char *argv[]) 
{ 
	int id = shmget(0x8888,4096,IPC_CREAT | IPC_EXCL | 0664);
	if (id < 0)
	{
		perror("shmget");
		return -1;
	}

	printf("id = %d\n",id);

	// 将共享内存挂载在此进程的空间中,
	char *buf = shmat(id,NULL,0);
	if (buf == (char *)-1)
	{
		perror("shmat");
		return -1;
	}

	// 向共享空间中复制字符
	strcpy(buf,"share memory!");

	// 进程结束,取消共享内存的挂载
	if (shmdt(buf) < 0)
	{
		perror("shmdt");
		return -1;
	}
    return 0;
}
