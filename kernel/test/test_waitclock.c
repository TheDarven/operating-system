#include "test_waitclock.h"

void P1(void) {
	while(1) {
		wait_clock(current_clock() + 1 * CLOCKFREQ);
		printf(".");
	}
}

void P2(void) {
	while(1) {
		wait_clock(current_clock() + 2 * CLOCKFREQ);
		printf("-");
	}

}

void P3(void) {
	while(1) {
		wait_clock(current_clock() + 5 * CLOCKFREQ);
		printf("+");
	}
}

void P4(void) {
	while(1) {
		wait_clock(current_clock() + 10 * CLOCKFREQ);
		printf("*");
	}
}

void test_waitClock(void) {

	int pid1 = start((int (*)(void *)) P1, 4000, 2, "P1", NULL);
	int pid2 = start((int (*)(void *)) P2, 4000, 2, "P2", NULL);
	int pid3 = start((int (*)(void *)) P3, 4000, 2, "P3", NULL);
	int pid4 = start((int (*)(void *)) P4, 4000, 2, "P4", NULL);

	wait_clock(current_clock() + 30 * CLOCKFREQ);

	kill(pid1);
	kill(pid2);
	kill(pid3);
	kill(pid4);
}