/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月05日 星期日 18时04分06秒
 ************************************************************************/

#include "apue.h"
#include <fcntl.h>
#include <sys/uio.h>

void test_a()
{

	struct iovec iov[3];
	char name[7];
	char fuhao[2];
	char age[3];
	
	iov[0].iov_base = name;
	iov[0].iov_len  = 6;
	iov[1].iov_base = fuhao;
	iov[1].iov_len  = 1;
	iov[2].iov_base = age;
	iov[2].iov_len  = 2;

	int fd;
	if ((fd = open("1.in",O_RDONLY)) < 0)
		exit(-1);

	int len;
	if ((len = readv(fd,&iov[0],3)) > 0)
		printf("%d\n",len);
	
	name[6] = 0;
	fuhao[1] = 0;
	age[2] = 0;

	printf("%s\n",name);
	printf("%s\n",fuhao);
	printf("%s\n",age);
}


int main(int argc,char *argv[]) 
{ 
	char test[10];
	sprintf(test,"%*s",7,"12");
	printf("%s\n",test);
	for (int i=0; i<7; i++)
		if (test[i] == '0')
			printf("i = %d\n",i);
}
