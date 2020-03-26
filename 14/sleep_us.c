/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 08时56分18秒
 ************************************************************************/

#include "apue.h"
#include <sys/select.h>
#include <poll.h>
 
void
sleep_us(unsigned int nsecond)
{
	struct timeval val;
	val.tv_sec = nsecond / 1000000;
	val.tv_usec = nsecond % 1000000;
	select(0,NULL,NULL,NULL,&val);
}

void 
sleep_us1(unsigned int nsecond)
{
	struct pollfd dummy;
	int timeout;

	if ((timeout = nsecond / 1000) <= 0)
		timeout = 1;
	poll(&dummy,0,timeout);
}

int main(int argc,char *argv[]) 
{ 
    return 0;
}
