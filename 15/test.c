/*************************************************************************
    > File Name: test.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月26日 星期四 20时06分49秒
 ************************************************************************/

#include <stdio.h> 

extern char **environ;
 
int main(int argc,char *argv[]) 
{ 
	for (int i=0; environ[i]!=NULL; i++)
		printf("%s\n",environ[i]);
    return 0;
}
