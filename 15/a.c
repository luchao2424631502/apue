/*************************************************************************
    > File Name: a.c
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年03月27日 星期五 10时12分04秒
 ************************************************************************/

#include <stdio.h> 
 
int main(int argc,char *argv[]) 
{ 
	FILE *fp;
	fp = popen("cat 1.in","r");
	char buf[100000];

	while (fscanf(fp,"%s",buf) != EOF)
	{
		printf("%s\n",buf);
	}
	pclose(fp);
    return 0;
}
