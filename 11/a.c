#include <sys/time.h>
#include <stdlib.h>
 
void
maketimeout(struct timespec *tsp,long minutes)
{
	struct timeval now;
	gettimeofday(&now,NULL);
	tsp->tv_sec = now.tv_sec;
	tsp->tv_nsec = now.tv_usec * 1000;
	tsp->tv_sec += minutes * 60;
}

int main(int argc,char *argv[]) {
 
    exit(0);
}
