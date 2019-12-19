//
// Created by 张淇 on 2019/12/19.
//

#include "header.h"

int main () {
	int shmId = shmget ((key_t) 0x2233, sizeof (buffer), IPC_CREAT | 0666);
	int semId = semget ((key_t) 1, 2, IPC_CREAT | 0666);
	auto *buf = (buffer *) shmat (shmId, nullptr, SHM_R | SHM_W);
	std::cout << "start write" << std::endl;
	FILE *fp;
	if ((fp = fopen ("output.jpg", "wb")) == nullptr) {
		std::cout << "writer open error" << std::endl;
		return -1;
	}
	int num = 0, size;
	while (buf->flag || buf->length[num] > 0) {
		P (semId, 1);
		fwrite (buf->data[num], sizeof (char), buf->length[num], fp);
		buf->length[num] = 0;
		std::cout << "write " << num << " block" << std::endl;
		V (semId, 0);
		num = (num + 1) % MEMSIZE;
	}
	fclose (fp);
	std::cout << "writer exit" << std::endl;
	return 0;
}

