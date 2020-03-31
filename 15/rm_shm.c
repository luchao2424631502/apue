/*************************************************************************
    > File Name: b.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月30日 星期一 15时55分01秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h> 
 
int main(int argc,char *argv[]) 
{ 
	// 返回ipc的内核id
	int id = shmget(0x8888,0,0);
	if (id < 0)
	{
		perror("shmget");
		return -1;
	}
	printf("id = %d\n",id);

	char *buf = shmat(id,NULL,0);

	if(buf == (char *)-1)
	{
		perror("shmat");
		return -1;
	}

	// 打印共享内粗的数据
	printf("%s",buf);

	// 将共享内存从进程空间中卸载
	if (shmdt(buf) < 0)
	{
		perror("shmdt");
		return -1;
	}

	// 删除内核对象
	if (shmctl(id,IPC_RMID,NULL) < 0)
	{
		perror("shmctl");
		return -1;
	}
    return 0;
}
