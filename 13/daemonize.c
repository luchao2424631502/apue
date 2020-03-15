#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#include <sys/resource.h>

void 
daemonize(const char *cmd) {
	int i,fd0,fd1,fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	// 1.清除文件掩码,设置为0
	umask(0);

	// 得到文件描述符限制
	if (getrlimit(RLIMIT_NOFILE,&rl) < 0)
		err_quit("%s: can;t get file limit",cmd);

	// 2.成为会话首进程,失去控制终端
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork",cmd);
	else if (pid != 0) 
		// 父进程结束,子进程成为孤儿进程
		exit(0);

	// 3. 此时不是进程组的组长,那么会成为会话首进程且切断与终端的联系
	setsid();

	// 3.1 设置信号处理程序为 忽略
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP,&sa,NULL) < 0)
		err_quit("%s: cna't ignore SIGHUP",cmd);

	// 3.2 session首进程退出时, sighup发送到session中前台进程组的每一个进程,但是此时已经忽略
	if ((pid = fork()) < 0)
		err_quit("%s: can't fork",cmd);
	else if (pid != 0)
		exit(0);

	// 4. 改变当前工作的 绝对路径
	if (chdir("/") < 0)
		err_quit("%s: can't change directory to /",cmd);

	// 5.关闭所有文件描述符
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i < rl.rlim_max; i++)
		close(i);
	
	// 6.将0,1,2文件描述符 附加到 /dev/null上
	fd0 = open("/dev/null",O_RDWR);
	// 获得当前可以获得的数值最小的文件描述符
	fd1 = dup(0);
	fd2 = dup(0);

	// initialize the log file
	openlog(cmd,LOG_CONS,LOG_DAEMON);
	// stdin,stdout,stderr,是否正确关闭
	if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
		syslog(LOG_ERR,"unexpected file descriptors %d %d %d",
				fd0,fd1,fd2);
		exit(1);
	}
}

