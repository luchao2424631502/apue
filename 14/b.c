/*************************************************************************
    > File Name: b.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 09时06分17秒
 ************************************************************************/

#include "apue.h"
#include <poll.h>
#include <sys/select.h>
 
void
sleep_us(unsigned int usecond)
{
	struct timeval val;
	val.tv_sec = usecond / 1000000;
	val.tv_usec = usecond % 1000000; 

	select(0,NULL,NULL,NULL,&val);
}

void
sleep_us1(unsigned int usecond)
{
	struct pollfd p;
	int timeout;
	if ((timeout = usecond / 1000) <= 0)
		timeout = 1;
	poll(&p,0,timeout);
}

int main(int argc,char *argv[]) 
{ 
    return 0;
}
