/*************************************************************************
    > File Name: send.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月31日 星期二 14时26分58秒
 ************************************************************************/

#include "local.h"
 
#define ASSERT(prompt,res) if((res)<0){perror(#prompt);exit(-1);}

// 定义Person类型(结构体)
typedef struct {
	char name[20];
	int age;
}Person;

// 定义消息类型
typedef struct 
{
	long type;
	Person person;
}Msg;

int main(int argc,char *argv[]) 
{ 
	int id = msgget(0x8888,IPC_CREAT | 0664);
	ASSERT(msgget,id);

	Msg msg[10] = {
		{1,{"Luffy",17}}	,
		{1,{"Zoro",19}}		,
		{2,{"Nami",18}}		,
		{2,{"Usopo",17}}	,
		{1,{"Sanji",19}}	,
		{3,{"Chopper",15}}	,
		{4,{"Robin",28}}	,
		{4,{"Franky",34}}	,
		{5,{"Brook",88}}	,
		{6,{"Sunny",2}}
	};
	
	for (int i=0; i<10; i++)
	{
		int res = msgsnd(id,&msg[i],24,0);
		ASSERT(msgsend,res);
	}
	printf("ok\n");

    return 0;
}
