/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月01日 星期三 19时30分03秒
 ************************************************************************/

#include "local.h"
 
#define R0 0 
#define R1 1
#define R2 2

void printSem(int id)
{
	unsigned short vals[3] = {0};
	semctl(id,3,GETALL,vals);
	printf("r0 = %d,r1 = %d,r2 = %d\n",vals[0],vals[1],vals[2]);
}

int main(int argc,char *argv[]) 
{ 
	// 得到信号量　
//	int id = semget(0x8888,3,IPC_CREAT | IPC_EXCL | 0664);

	//删除信号
	int id = semget(0x8888,0,0);
	if (id < 0)
		perror("id error");

	int ans = semctl(id,0,IPC_RMID);
	if (ans < 0)
		perror("semctl error");
	return 0;

	// 打印信号量
	puts("信号量初始值");
	printSem(id);

	puts("设置第２个信号量");
	semctl(id,2,SETVAL,20);
	printSem(id);

	puts("同时设置３个信号量");
	unsigned short vals[3] = {12,5,9};
	semctl(id,0,SETALL,vals);
	printSem(id);

	puts("请求2个r0资源");
	struct sembuf op1 = {0,-2,0};
	semop(id,&op1,1);
	printSem(id);

	puts("请求3个r1和5个r2");
	struct sembuf ops1[2] = {
		{1,-3,0},
		{2,-5,0}
	};
	semop(id,ops1,2);
	printSem(id);

	puts("释放2个r1");
	struct sembuf op2 = {1,2,0};
	semop(id,&op2,1);
	printSem(id);

	puts("释放1r0,r1,3个r2");
	struct sembuf ops2[3] = {
		{0,1,0},
		{1,1,0},
		{2,3,0}
	};
	semop(id,ops2,3);
	printSem(id);
	
	puts("删除ipc内核对象");
	semctl(id,0,IPC_RMID);

    return 0;
}
