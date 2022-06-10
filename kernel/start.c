#include "debugger.h"
#include "cpu.h"
#include "process.h"
#include "scheduler.h"
#include "ctx_sw.h"
#include "screen.h"
#include "../shared/printf.c"
#include "../shared/string.h"
#include "traitant.h"
#include "clock.h"
#include "timer.h"

void fct_victime(void) {
	while (1) {
		printf("Mais.. pourquoi j'ai attendu si longtemps :(\n");
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
	}
}

void fct_prio2(void) {
	int counter = 0;
	while (1) {
		printf("%d : Hello, je suis aussi prioritaire ;)\n", getpid());
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		counter++;
		if (counter == 100) {
			kill(getpid());
		}
		cli();
	}
}

void fct_prio(void) {
	int counter = 0;
	start((int (*)(void *)) fct_victime, 0, 3, "fct_victime", NULL);
	while (1) {
		printf("%d : Je suis prioritaire MOUHAHAHAHAHAHAHAHAHAA\n", getpid());
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		counter++;

		if (counter == 20) {
			start((int (*)(void *)) fct_prio2, 0, 5, "fct_prio2", NULL);
		}

		if (counter == 50) {
			kill(getpid());
		}
		cli();
	}
}

void fct4(void) {
	while (1) {
		printf("d");
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
	}
}


void fct3(void) {
	while (1) {
		printf("c");
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
	}
}

void fct2(void) {
	int j = 0;
	start((int (*)(void *)) fct3, 0, 2, "fct3", NULL);
	start((int (*)(void *)) fct4, 0, 2, "fct4", NULL);
	while (1) {
		printf("b");
		sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		j++;
		if (j == 10) {
			start((int (*)(void *)) fct_prio, 0, 5, "fct_prio", NULL);
		}

		cli();
	}
	
}

void idle(void) {

	// start((int (*)(void *)) fct2, 0, 2, "fct2", NULL);
	while(1) {
		for (unsigned long i = 0; i < 429496750; i++) {
			
		}
		printf("%d : a", getpid());
		sti();
		start((int (*)(void *)) fct2, 0, 2, "fct2", NULL);
		cli();
	}
}



void kernel_start(void) {
	efface_ecran();

	/* Process idleP, proc1P;

	// Initialise le processus idle
	idleP.pid = 0;
	idleP.state = RUNNING;
	strcpy(idleP.name, "idle");

	processTable[0] = idleP;

	// Place le processus idle comme processus élu
	runningProcess = &processTable[0];

	// Initialise le processus proc1
	proc1P.pid = 1;
	proc1P.state = READY;
	strcpy(proc1P.name, "proc1");
	proc1P.executionStack[STACK_SIZE - 1] = (uint32_t) tstB;
	proc1P.context[ESP] = (uint32_t) &(proc1P.executionStack[STACK_SIZE - 1]);
	processTable[1] = proc1P; */

	masque_IRQ(0, 0);

	set_Quartz();

	init_all_traitants();

	start((int (*)(void *)) idle, 0, 1, "idle", NULL);

	idle();


	while(1)
	  hlt();

	return;

}
