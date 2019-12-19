//
// Created by 张淇 on 2019/12/19.
//

#include "header.h"
#include <cstring>

int main () {
	int shmId = shmget ((key_t) 0x2233, sizeof (buffer), IPC_CREAT | 0666);
	int semId = semget ((key_t) 1, 2, IPC_CREAT | 0666);
	buffer *buf = (buffer *) shmat (shmId, nullptr, SHM_R | SHM_W);
	std::cout << "start read" << std::endl;
	FILE *fp;
	if ((fp = fopen ("hust.jpg", "rb")) == nullptr) {
		std::cout << "reader open error" << std::endl;
		return -1;
	}
	int num = 0, size;
	while (buf->flag) {
		std::cout << "read start" << std::endl;
		P (semId, 0);
		buf->length[num] = size = fread (buf->data[num], sizeof (char), 100, fp);
		if (size < 100) {
			buf->flag = false;
			std::cout << "reader reach to file end" << std::endl;
		}
		std::cout << "read " << num << " block" << std::endl;
		V (semId, 1);
		num = (num + 1) % MEMSIZE;
	}
	fclose (fp);
	std::cout << "reader exit" << std::endl;
	return 0;
}
