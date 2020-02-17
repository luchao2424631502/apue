#include <bits/stdc++.h>
#include "apue.h"
#include <fcntl.h>
#include "path_alloc.h"
 
#define DEPTH 1000
#define STARTDIR "/tmp"
#define NAME "alonglonglonglonglonglonglonglonglonglongname"
#define MAXSZ (10*8192)

int main(int argc,char *argv[]) {
	if(chdir(STARTDIR) < 0) 
		err_sys("chdir error");
	for (int i=0; i<DEPTH; ++i) {
		if (mkdir(NAME,DIR_MODE) < 0)	
			err_sys("mkdir failed, i = %d",i);
		// /tmp/aln/aln/aln/aln
		if (chdir(NAME) < 0)
			err_sys("create error");
	}
	// 走到这一步说明已经递归创建出了1000深度目录路径
	if (creat("afile",FILE_MODE) < 0)
		err_sys("creat error");
	size_t size;
	char *path;
	path = (char *)path_alloc(&size);
	for (;;) {
		// 大小够了,可以得到路径
		if(getcwd(path,size) != NULL)
			break;
		// 否则一直扩大分配内存
		else {
			err_ret("getcwd failed,size = %ld",(long)size);
			size += 1000;
			if (size > MAXSZ)
				err_quit("giving up");
			if ((path = (char *)realloc(path,size)) == NULL)
				err_sys("realloc error");
		}
	}
	printf("length = %ld\n%s\n",(long)strlen(path),path);
    return 0;
}
