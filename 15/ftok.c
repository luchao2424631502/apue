/*************************************************************************
    > File Name: test.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 20时06分49秒
 ************************************************************************/

#include "local.h"

extern char **environ;
 
// 设备号: 09 inode:0efe 0a 0a090efe
int main(int argc,char *argv[]) 
{ 
	if (argc < 3)
	{
		printf("usage: %s <path> <id> \n",argv[0]);
		return -1;
	}

	// 命令行第二个参数id
	int id = atoi(argv[2]);

	// 第一个参数是路径
	key_t key = ftok(argv[1],id);

	if (key == -1)
	{
		perror("ftok");	
		return -1;
	}

	printf("key = 0x%08x\n",key);

	//for (int i=0; environ[i]!=NULL; i++)
	//	printf("%s\n",environ[i]);
    return 0;
}
