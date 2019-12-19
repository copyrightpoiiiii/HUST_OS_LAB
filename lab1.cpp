//
// Created by 张淇 on 2019/12/5.
//

#include "lab1.h"

int pipeFd[2];
int state;
pid_t p1, p2, t1, t2;

void work1 (int sig) {
	if (sig == SIGUSR1) {
		close (pipeFd[1]);
		std::cout << "p1 byebye~" << std::endl;
		exit (0);
	}
}

void work2 (int sig) {
	if (sig == SIGUSR2) {
		close (pipeFd[0]);
		std::cout << "p2 byebye~" << std::endl;
		exit (0);
	}
}

void work (int sig) {
	std::cout << "I get a signal" << std::endl;
	if (sig == SIGINT) {
		std::cout << "main receive ctrl+C" << std::endl;
		kill (p1, SIGUSR1);
		kill (p2, SIGUSR2);
	}
}


void lab1 () {
	pipe (pipeFd);
	signal (SIGINT, work);
	p1 = fork ();
	if (p1 == 0) {
		signal (SIGUSR1, work1);
		signal (SIGINT, SIG_IGN);
		int cnt = 0;
		while (1) {
			write (pipeFd[1], &cnt, sizeof (cnt));
			cnt++;
			sleep (1);
		}
	} else if (p1 > 0) {
		p2 = fork ();
		if (p2 == 0) {
			signal (SIGUSR2, work2);
			signal (SIGINT, SIG_IGN);
			while (1) {
				int x;
				read (pipeFd[0], &x, sizeof (x));
				std::cout << "I get data " << x << " times" << std::endl;
			}
		}
	}

	t1 = waitpid (p1, &state, 0);
	t2 = waitpid (p2, &state, 0);
}