/*************************************************************************
    > File Name: shmctl.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月31日 星期二 10时40分40秒
 ************************************************************************/

#include "local.h"
 
#define ASSERT(res) if(res<0){perror(__FUNCTION__);exit(-1);}

// ipc_perm
void printPerm(struct ipc_perm *perm)
{
	printf("euid of owner = %d\n"
			"egid of owner = %d\n"
			"euid of creator = %d\n"
			"egid of creator = %d\n"
			"mode = 0%o\n",
			perm->uid,perm->gid,perm->cuid,perm->cgid,perm->mode);
}

//ipc info
void printShmid(struct shmid_ds *shmid)
{
	printPerm(&shmid->shm_perm);
	printf("segment size = %d\n"
			"last attach time = %s"
			"last detach time = %s"
			"last change time = %s"
			"pid of creator = %d\n"
			"pid of last shmat/shmdt = %d\n"
			"No. of current attached = %ld\n",
			shmid->shm_segsz,ctime(&shmid->shm_atime),ctime(&shmid->shm_dtime),
			ctime(&shmid->shm_ctime),shmid->shm_cpid,shmid->shm_lpid,shmid->shm_nattch
		);
}

// 创建ipc
void create()
{
	// 123字节大小
	int id = shmget(0x8888,123,IPC_CREAT | IPC_EXCL | 0644);
	printf("create %d\n",id);
	ASSERT(id)
}

// 获取ipc内核对象信息
void show()
{
	int id = shmget(0x8888,0,0);
	ASSERT(id)
	struct shmid_ds shmid;
	ASSERT(shmctl(id,IPC_STAT,&shmid))
	printShmid(&shmid);	
}

void set()
{
	int id = shmget(0x8888,123,IPC_CREAT|0664);
	ASSERT(id)
	struct shmid_ds shmid;
	ASSERT(shmctl(id,IPC_STAT,&shmid))
	shmid.shm_perm.mode = 0600;
	ASSERT(shmctl(id,IPC_SET,&shmid))
	printf("set %d\n",id);
}

void rm()
{
	int id = shmget(0x8888,123,IPC_CREAT | 0664);
	ASSERT(id)
	ASSERT(shmctl(id,IPC_RMID,NULL))
	printf("remove id = %d\n",id);
}

void at_dt()
{
	int id = shmget(0x8888,123,IPC_CREAT | 0664);
	ASSERT(id)
	char *buf = shmat(id,NULL,0);
	if (buf == (char *)-1)
		ASSERT(-1)
	printf("shmat %p\n",buf);
	sleep(5);
	ASSERT(shmdt(buf))
	printf("shmget %p\n",buf);
}

int main(int argc,char *argv[]) 
{ 
	if (argc < 2)
	{
		printf("usage: %s <option -c -v -s -d -a>\n",argv[0]);
		return -1;
	}

	printf("I'm %d\n",getpid());


	if (!strcmp(argv[1],"-c"))
		create();
	else if (!strcmp(argv[1],"-v"))
		show();
	else if (!strcmp(argv[1],"-s"))
		set();
	else if (!strcmp(argv[1],"-d"))
		rm();
	else if (!strcmp(argv[1],"-a"))
		at_dt();
    return 0;
}
