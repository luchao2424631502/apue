/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月02日 星期四 09时56分29秒
 ************************************************************************/

#include <stdio.h> 
 
int main(int argc,char *argv[]) 
{ 
	FILE *fp = popen("a","r");
	char buf[1024];
	fscanf(fp,"%s",buf);
	printf("%s\n",buf);
    return 0;
}
