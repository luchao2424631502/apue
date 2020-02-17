/*************************************************************************
    > File Name: a.c
  > Author: luchao
  > Mail: 18271496687@163.com 
  > Created Time: 2020年01月22日 星期三 10时50分18秒
 ************************************************************************/

#include "apue.h"
#define BUFFSIZE 4096

int main () {
	int n;
	char buf[BUFFSIZE];

	while((n = read(STDIN_FILENO,buf,BUFFSIZE)) > 0)
		if (write(STDOUT_FILENO,buf,n) != n)
			err_sys("write_error");
	if (n < 0)
		err_sys("read error");
    return 0;
}
