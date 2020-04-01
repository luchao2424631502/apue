/*************************************************************************
    > File Name: semutil.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月01日 星期三 20时04分38秒
 ************************************************************************/

#include "semutil.h"
 
int C(int count)
{
	int id;
	if(count > 0)
		id = semget(0x8888,count,IPC_CREAT | IPC_EXCL | 0664);
	else
		id = semget(0x8888,0,0);
	ASSERT(semget,id)
	return id;
}

void S(int id,int semnum,int val)
{
	ASSERT(semctl,semctl(id,semnum,SETVAL,val))
}

void D(int id)
{
	ASSERT(semctl,semctl(id,0,IPC_RMID))
}

void P(int id,int semnum)
{
	struct sembuf op = {semnum,-1,0};
	ASSERT(semop,semop(id,&op,1))
}

void V(int id,int semnum)
{
	struct sembuf op = {semnum,1,0};
	ASSERT(semop,semop(id,&op,1))
}

int G(int id,int semnum)
{
	// 得到信号量的资源数
	return semctl(id,semnum,GETVAL);
}

