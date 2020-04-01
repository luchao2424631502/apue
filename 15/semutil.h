/*************************************************************************
    > File Name: semutil.h
    > Author: llc
    > Mail: 18271496687@163.com 
    > Created Time: 2020年04月01日 星期三 20时01分15秒
 ************************************************************************/
#ifndef _SEMUTIL_H
#define _SEMUTIL_H

#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

#define ASSERT(prompt,res) if((res)<0){perror(#prompt);exit(-1);}

// 创建sem
int C(int);

// 初始化
void S(int,int,int);

// 获取
int G(int,int);

//删除
void D(int);

// -1 信号量
void P(int,int);

// +1
void V(int,int);

#endif
