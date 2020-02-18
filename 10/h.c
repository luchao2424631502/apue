#include "apue.h" 
 
int main(int argc,char *argv[]) {
#ifdef _GNU_SOURCE
	printf("defined gnu_source\n");
#else
	printf("not define gnu_source\n");
#endif
    exit(0);
}
