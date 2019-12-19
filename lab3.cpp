//
// Created by 张淇 on 2019/12/19.
//

#include "lab3.h"


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


void lab3 () {
	buffer *sharedMem = nullptr;
	int shmId = shmget ((key_t) 0x2233, sizeof (buffer), IPC_CREAT | 0666);
	if (shmId == -1) {
		std::cout << "shmget error" << std::endl;
		return;
	}
	sharedMem = (buffer *) shmat (shmId, nullptr, SHM_R | SHM_W);
	if (sharedMem == nullptr) {
		std::cout << "shmat error" << std::endl;
		return;
	}
	sharedMem->flag = true;
	union semun arg{};
	int semId = semget ((key_t) 1, 2, IPC_CREAT | 0666);
	arg.val = MEMSIZE;
	semctl (semId, 0, SETVAL, arg);
	arg.val = 0;
	semctl (semId, 1, SETVAL, arg);
	pid_t p1, p2;
	p1 = fork ();
	if (p1 == 0) {
		std::cout << "reader start" << std::endl;
		execv ("../get", nullptr);
	} else if (p1 > 0) {
		p2 = fork ();
		if (p2 == 0) {
			std::cout << "writer start" << std::endl;
			execv ("../put", nullptr);
		} else {
			wait (&p1);
			wait (&p2);
			semctl (shmId, IPC_RMID, 0);
			for (int i : sharedMem->semId)
				semctl (i, 1, IPC_RMID, 0);
			return;
		}
	}
}