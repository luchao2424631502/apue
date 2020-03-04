#include "apue.h" 
#include <unistd.h>
#include <limits.h>
 
int main(int argc,char *argv[]) {
	printf("%ld \n %ld \n %ld \n %ld",sysconf(_SC_THREAD_DESTRUCTOR_ITERATIONS),
			sysconf(_SC_THREAD_KEYS_MAX),
			sysconf(_SC_THREAD_STACK_MIN),
			sysconf(_SC_THREAD_THREADS_MAX)); 
    exit(0);
}
