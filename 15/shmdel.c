/*************************************************************************
    > File Name: shmdel.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月30日 星期一 17时56分30秒
 ************************************************************************/

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h> 
#include <stdlib.h>
 
int main(int argc,char *argv[]) 
{ 
	// 命令行给出key,
	if (argc < 2)
	{
		perror("get <key> ");
		return -1;
	}
	// 字符串转为长整形
	key_t key = strtoll(argv[1],NULL,16);

	int id;
	if((id = shmget(key,0,0)) < 0)
	{
		perror("shmget");
		return -1;
	}

	// 得到ipc 的内核 id后,删除共享内存
	if (shmctl(id,IPC_RMID,NULL) < 0)
	{
		perror("rm shm");
		return -1;
	}
    return 0;
}
