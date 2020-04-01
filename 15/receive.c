/*************************************************************************
    > File Name: receive.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月31日 星期二 14时45分28秒
 ************************************************************************/

#include "local.h"

#define ASSERT(prompt,res) if((res)<0){perror(#prompt);exit(-1);}
 
typedef struct {
	char name[20];
	int age;
}Person;

typedef struct {
	long type;
	Person person;
}Msg;

void printMsg(Msg *msg)
{
	printf("{type=%ld, name=%s, age=%d}\n",
			msg->type,msg->person.name,msg->person.age);
}

int main(int argc,char *argv[]) 
{
	if (argc < 2)
	{
		printf("usage: %s <type>\n",argv[0]);
		return -1;
	}
	// 需要接受消息的类型
	long type = atoi(argv[1]);

	// 根据key获得ipc的内核id
	int id = msgget(0x8888,0);
	ASSERT(msgget,id);

	Msg msg;
	int res;

	while(1)
	{
		// 非阻塞取出队列中的消息
		res = msgrcv(id,&msg,sizeof(Person),type,IPC_NOWAIT);
		if (res < 0)
		{
			if (errno == ENOMSG)
			{
				printf("No message\n");
				break;
			}
			else
			{
				ASSERT(msgrcv,res)
			}
		}
		// 打印消息
		printMsg(&msg);
	}
    return 0;
}
