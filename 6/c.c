#include <shadow.h>
#include <stdlib.h>
#include <stdio.h>

struct spwd *
getspname(const char *name) {
	struct spwd *ptr;

	// 开启shadow password文件
	setspent();
	while ((ptr == getspent()) != NULL) {
		if (strcmp(name,ptr->sp_name) == 0)
			break;
	}

	// 关闭shadow password文件
	endspent();

	return ptr;
}


int main(int argc,char *argv[]) {
	struct spwd *ptr;
	if ((ptr = getspnam("luchao")) == NULL)
		exit(1);
	printf("%s\n%s",ptr->sp_namp,ptr->sp_pwdp);
	
    return 0;
}
