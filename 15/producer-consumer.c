/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月01日 星期三 20时15分44秒
 ************************************************************************/

#include "local.h"
#include "semutil.h"

#define MUTEX 0
#define FULL 1
#define EMPTY 2

static void
init()
{
	// 创建３个信号量
	int id = C(3);
	// 初始化资源数量
	S(id,MUTEX,1);
	S(id,FULL, 0);
	S(id,EMPTY,5);
	
	// 创建共享内存
	int shmid = shmget(0x8888,4,IPC_CREAT | IPC_EXCL | 0664);
	ASSERT(shmget,shmid)
	int *cake = shmat(shmid,NULL,0);
	if (cake == (int *)-1)
		ASSERT(shmat,-1)
	// 共享内存的值初始化为0
	*cake = 0;
	ASSERT(shmdt,shmdt(cake))
}
 
static int getsemid()
{
	return C(0);
}

static int getshmid()
{
	int id = shmget(0x8888,0,0);
	ASSERT(shmget,id)
	return id;
}

static void release(int id)
{
	// 删除信号量
	D(id);
	//　取消共享内存挂载
	ASSERT(shmctl,shmctl(getshmid(),IPC_RMID,NULL));
}

// 生产者
static void producer()
{
	int id = getsemid();
	int shmid = getshmid();
	int *cake = shmat(shmid,NULL,0);
	while(1)
	{
		// 减少缓冲区位置
		P(id,EMPTY);
		P(id,MUTEX);
		printf("current cake = %d",*cake);
		(*cake)++;
		printf("produce a cake\ncake = %d\n",*cake);
		V(id,MUTEX);
		// 增加蛋糕数量
		V(id,FULL);
		sleep(1);
	}
	shmdt(cake);
}

// 
static void consumer()
{
	int id = getsemid();
	int shmid = getshmid();
	int *cake = shmat(shmid,NULL,0);
	
	int count = 10;
	while(count--)
	{
		// 消耗蛋糕
		P(id,FULL);	
		// 进入互斥量
		P(id,MUTEX);
		printf("current cake = %d\n",*cake);
		(*cake)--;
		printf("consume a cake\ncake = %d\n",*cake);
		// 退出互斥量
		V(id,MUTEX);
		// 增加空位置
		V(id,EMPTY);
	}
	shmdt(cake);
}

int main(int argc,char *argv[]) 
{ 
	if (argc < 2)
	{
		printf("usage: %s <option -b -d -p -c>\n",argv[0]);
		return -1;
	}

	if (!strcmp("-b",argv[1]))
		init();
	else if (!strcmp("-d",argv[1]))
		release(getsemid());
	else if (!strcmp("-p",argv[1]))
		producer();
	else if (!strcmp("-c",argv[1]))
		consumer();
    return 0;
}
