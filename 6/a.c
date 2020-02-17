#include <stdio.h>
#include "apue.h"
#include <pwd.h>
 
int main(int argc,char *argv[]) {
	struct passwd *buf;
	char name[2][20] = {"root","luchao"};
	int index = 0;
	while (index < 2) {
		if ((buf = getpwnam(name[index++])) == NULL)
			err_sys("getpwuid error");
		printf("%s",buf->pw_name);
		printf("\n%s",buf->pw_passwd);
		printf("\n%d",buf->pw_uid);
		printf("\n%d",buf->pw_gid);
		printf("\n%s",buf->pw_gecos);
		printf("\n%s",buf->pw_dir);
		printf("\n%s\n-------------\n",buf->pw_shell);
	}
    return 0;
}
