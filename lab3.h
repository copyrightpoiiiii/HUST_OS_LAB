//
// Created by 张淇 on 2019/12/19.
//

#ifndef OS_LAB1_LAB3_H
#define OS_LAB1_LAB3_H


#include <iostream>
#include <sys/shm.h>
//#include <chrono>
#include <sys/types.h>
#include <sys/sem.h>
#include <semaphore.h>
//#include <cstddef>
#include <unistd.h>

#define MEMSIZE 10

struct buffer {
	char data[MEMSIZE][100];
	int length[MEMSIZE];
	int semId[MEMSIZE];
	bool flag;
};

void P (int semid, int index);

void V (int semid, int index);

void lab3 ();

#endif //OS_LAB1_LAB3_H
