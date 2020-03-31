/*************************************************************************
    > File Name: ipcget.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月30日 星期一 17时43分24秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h> 
 
int main(int argc,char *argv[]) 
{ 
	if (argc < 3)
	{
		printf("%s <icpc type> <key>\n",argv[0]);
		return -1;
	}
	key_t key = strtoll(argv[2],NULL,16);
	char type = argv[1][0];
	char buf[64];
	int id;

	if (type == '0')
	{
		// 参数:key,创建的大小,创建的flag
		id = shmget(key,0,0);
		strcpy(buf,"share memory");
	}
	else if (type == '1')
	{
		id = msgget(key,0);
		strcpy(buf,"message queue");
	}
	else if (type == '2')
	{
		id = semget(key,0,0);
		strcpy(buf,"semaphore");
	}
	else 
	{
		printf("type error\n");
		return -1;
	}

	// 判断内核 ipc id
	if (id < 0)
	{
		perror("id error");
		return -1;
	}
	printf("get %s at %x,id = %d\n",buf,key,id);
    return 0;
}
