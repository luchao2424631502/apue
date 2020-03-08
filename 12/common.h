#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <time.h>
#include <sys/time.h>

// 文件域　函数放在头文件中声明
int makethread(void *(*)(void *),void *);

#endif
