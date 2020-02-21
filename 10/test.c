#include "apue.h" 
 
int main(int argc,char *argv[]) {
	kill(atoi(argv[1]),SIGCONT); 
    exit(0);
}
