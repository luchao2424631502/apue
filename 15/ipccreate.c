/*************************************************************************
    > File Name: ipccreate.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月30日 星期一 16时13分42秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h> 
#include <string.h>
 
int main(int argc,char *argv[]) 
{ 
	

	// 字符串转化成长整形
	key_t key = strtoll(argv[2],NULL,16);
	char type = argv[1][0];
	char buf[64];
	int id;

	if (type == '0')
	{
		id = shmget(key,getpagesize(),IPC_CREAT | IPC_EXCL | 0644);
		//　填充缓冲区
		strcpy(buf,"share memory");
	}
	else if (type == '1')
	{
		id = msgget(key,IPC_CREAT | IPC_EXCL | 0644);
		strcpy(buf,"message queue");
	}
	else if (type == '2')
	{
		id = semget(key,5,IPC_CREAT | IPC_EXCL | 0644);
		strcpy(buf,"semaphore");
	}
	else
	{
		printf("type error");
		return -1;
	}

	// 检测内核ipc id是否分配正常
	if (id < 0)
	{
		perror("get error");
		return -1;
	}

	printf("create %s at 0x%x, id = %d\n",buf,key,id);
    return 0;
}
