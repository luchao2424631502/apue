#include <stdio.h>
#include "apue.h"
 
int main(int argc,char *argv[]) {
	int **p = calloc(10,sizeof(int *));

	int a[10] = {1,2,3,4,5,6,7,8,9,10};
	
	for (int i=0; i<10; ++i)
		p[i] = &a[i];
	for (int i=0; i<10; ++i)
		printf("%d 栈上的地址 = %p 堆上的地址 = %p\n",*p[i],p[i],&p[i]);
	free(p);
	exit(0);
}
