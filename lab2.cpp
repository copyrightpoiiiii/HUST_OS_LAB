//
// Created by 张淇 on 2019/12/12.
//
#include "lab2.h"

int a = 0;
int semid;
key_t key = 1;

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

void subp1 () {
	for (int i = 1; i <= 100; i++) {
		P (semid, 0);
		std::cout << "thread1 start adding!" << std::endl;
		a += i;
		V (semid, 1);
	}
}

void subp2 () {
	for (int i = 1; i <= 100; i++) {
		P (semid, 1);
		std::cout << "thread2 start printing!" << std::endl;
		std::cout << "now the answer is: " << a << std::endl;
		V (semid, 0);
	}
}

void lab2 () {
	//p_thread_mutex
	semid = semget (key, 2, IPC_CREAT | 0666);
	union semun arg{};
	arg.val = 1;
	semctl (semid, 0, SETVAL, arg);
	arg.val = 0;
	semctl (semid, 1, SETVAL, arg);
	std::thread p1 (subp1);
	std::thread p2 (subp2);
	p1.join ();
	p2.join ();
}