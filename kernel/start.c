#include "process.h"
#include "screen.h"
#include "traitant.h"
#include "../shared/string.h"


int test_run(int n);
void tests(void) {
	sti();
	hlt();
	cli();
	for (int i = 0; i < 19; i++) {
		// Tests qui ne sont pas encore implémentés
		if (i == 6 || i == 7 || i == 11 || i == 17) {
			continue;
		}

		// Test long à skip
		if (i == 9) {
			continue;
		}

		printf("test %d :\n", i);
		test_run(i);
	}

	while(1);
}


void idle(void) {
	sti();
	for (unsigned long i = 0; i < 429496750; i++) {}
	cli();
	printf("%d : a - %d\n", getpid(), getNbStartProcess());
	start((int (*)(void *)) tests, 4000, 128, "tests", NULL);

}






////////////////////////////////////////////////////////////////////////////////////////////////////


void kernel_start(void) {
	efface_ecran();

	masque_IRQ(0, 0);

	set_Quartz();

	init_all_traitants();

	printf("test kernel \n");
	int pid = start((int (*)(void *)) idle, 4000, 1, "idle", NULL);
	printf("idle pid : %d \n", pid);
	idle();
	while (1) {	
		hlt();
	}

}
