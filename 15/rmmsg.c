/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月31日 星期二 15时03分29秒
 ************************************************************************/

#include "local.h"
 
int main(int argc,char *argv[]) 
{ 
	int id = msgget(0x8888,0);
	if (id < 0)
	{
		perror("id error");
		exit(-1);
	}
	int res = msgctl(id,IPC_RMID,NULL);
	if (res < 0)
	{
		perror("msgctl error");
		return -1;
	}
    return 0;
}
