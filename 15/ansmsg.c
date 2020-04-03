/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月02日 星期四 16时18分56秒
 ************************************************************************/

#include "apue.h"
#include "local.h"
 
int main(int argc,char *argv[]) 
{ 
	int id;
	for (int i=0; i<5; i++)
	{
		if ((id = msgget(0,IPC_CREAT)) == -1)
		{
			printf("msgget error\n");
			exit(1);
		}	

		printf("id = %d\n",id);

		if (msgctl(id,IPC_RMID,NULL) == -1)
		{
			printf("msgctl IPC_RMID error\n");
			exit(1);
		}
	}
	struct {
		long type;
		char *ch;
	}buf;
	buf.type = 1;
	buf.ch = "hello";

	for (int i=0; i<5; i++)
	{
		if ((id = msgget(IPC_PRIVATE,IPC_CREAT)) == -1)
		{
			printf("msgget error\n");
			exit(1);
		}
		printf("id = %d\n",id);

		if (msgsnd(id,&buf,5,IPC_NOWAIT) < 0)
		{
			printf("msgsnd error: %s\n",strerror(errno));
			exit(1);
		}
	}
    return 0;
}
