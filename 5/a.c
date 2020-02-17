#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define BSZ 48
char buf[BSZ];

int main(int argc,char *argv[]) {
	FILE *fp; 
	
	// 1
	memset(buf,'a',BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	// 打开文件流
	if ((fp = fmemopen(buf,BSZ,"w+")) == NULL) {
		printf("fmemopen error");
		exit(1);
	}
	// stdout打印缓冲区
	printf("initial buffer content: %s\n",buf);
	// 打开水龙头,向文件(内存)中写入,但此时只是停留在标准IO的缓冲区中,
	fprintf(fp,"hello,world");
	printf("before standard buffer flush: %s\n",buf);
	// fflush强制IO缓冲区的数据写入fp指定的文件中,fmemopen保证了文件是内存
	fflush(fp);
	printf("after fflush: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));

	//2
	memset(buf,'b',BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	// 向fp指向的文件写入数据,但是还停留在IO库的缓冲区中
	fprintf(fp,"hello, world");
	// reposition会导致缓冲区被flush 
	fseek(fp,0,SEEK_SET);
	// 此时buf应该都为 b
	printf("after fseek: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));
	
	memset(buf,'c',BSZ - 2);
	buf[BSZ - 2] = '\0';
	buf[BSZ - 1] = 'X';
	// 流的读写位置通过seek定位到了0,fclose之前会flush缓冲区,
	fprintf(fp,"hello,world");
	// 关闭流之前,io库自动刷新所有的IO库的缓冲区
	fclose(fp);
	printf("after fclose: %s\n",buf);
	printf("len of string in buf = %ld\n",(long)strlen(buf));

    return 0;
}

