#include <pwd.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct passwd*
getpwnam(const char *name) 
{
	struct passwd *ptr;
	// 打开所使用的文件
	setpwent();
	// 遍历passwd文件中的每一项
	while ((ptr = getpwent()) != NULL) {
		printf("%s\n%s\n%d\n%d\n%s\n%s\n%s\n\n",
				ptr->pw_name,ptr->pw_passwd,
				ptr->pw_uid,ptr->pw_gid,
				ptr->pw_gecos,
				ptr->pw_dir,ptr->pw_shell);
		if (strcmp(name,ptr->pw_name) == 0)
			break;
	}
	// 关闭passwd文件
	endpwent();
	return ptr;
}
 
int main(int argc,char *argv[]) {
	struct passwd  *ptr;
	if ((ptr = getpwnam("nihao")) == NULL)
		exit(1);
    return 0;
}
