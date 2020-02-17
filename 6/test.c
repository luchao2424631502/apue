#include <stdio.h>
#include "apue.h"
#include <setjmp.h>

static void f1(int i,int j,int k,int l); 
static void f2(void);

//　栈上保存的信息
static jmp_buf jmpbuffer;

// 全局静态变量
static int globval;
 
int main(int argc,char *argv[]) {
	int			 autoval;
	register int regival;
	volatile int volaval;
	static int   statval;

	globval = 1;
	autoval = 2;
	regival = 3;
	volaval = 4;
	statval = 5;

	// 转条回来的地址
	if (setjmp(jmpbuffer) != 0) {
		printf("after longjmp: \n");
		// 打印值
		printf("global = %d, autoval = %d, regival = %d,"
			" volaval = %d, statval = %d\n",globval,autoval,
			regival,volaval,statval);
		autoval = -1;
		
		printf("\b autoval = %d\n",autoval);
		exit(0);
	}
	
	globval = 95; autoval = 96; regival = 97; volaval = 98;
	statval = 99;
	
	// 全局变量就不需要测试了
	f1(autoval,regival,volaval,statval);
	

	exit(0);
}

static void f1(int i,int j,int k,int l) {

	printf("in f1() \n");
	printf("globval = %d, autoval = %d, regival = %d,"
			" volaval = %d, statval = %d\n",globval,i,j,k,l);
	f2();
}

static void f2(void) {
	longjmp(jmpbuffer,1);
}
