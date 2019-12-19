//
// Created by 张淇 on 2019/12/19.
//

#ifndef OS_LAB1_HEADER_H
#define OS_LAB1_HEADER_H

#include <iostream>
#include <sys/shm.h>
#include <chrono>
#include <sys/types.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>

#define MEMSIZE 10

struct buffer {
	char data[MEMSIZE][100];
	int length[MEMSIZE];
	int semId[MEMSIZE];
	bool flag;
};

void P (int semid, int index) {
	struct sembuf sem{};
	sem.sem_num = index;
	sem.sem_op = -1;
	sem.sem_flg = 0;
	semop (semid, &sem, 1);
}

void V (int semid, int index) {
	struct sembuf sem{};
	sem.sem_num = index;
	sem.sem_op = 1;
	sem.sem_flg = 0;
	semop (semid, &sem, 1);
}

#endif //OS_LAB1_HEADER_H
