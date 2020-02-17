#include <stdio.h>
#include "apue.h"
#include <setjmp.h>
 
#define TOK_ADD 5
jmp_buf jmpbuffer;

void do_line(char *);
void cmd_add(void);
int get_token(void);

void myfunc(int val) {
	printf("%d\n",val);
	if (val > 0)
		longjmp(jmpbuffer,2);
}

int main(int argc,char *argv[]) {
	char line[MAXLINE];
	if (setjmp(jmpbuffer) == 1) {
		printf("jump to 1\n");
		return 0;
	}
	else {
		printf("检测过 jump 1\n");
	}
	
	if (setjmp(jmpbuffer) == 0)	{
		printf("set jump 2\n");
		return 0;
	}
	else {
		printf("检测过 jump 2\n");
		return 0;
	}

	myfunc(5);

	exit(0);
}

