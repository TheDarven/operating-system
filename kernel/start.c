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

/*
void idle(void) {
	for (int i = 0; i < 3; i++) {
        printf("[idle] je tente de passer la main a proc1...\n");
       	ctx_sw(processTable[0].context, processTable[1].context);
        printf("[idle] proc1 m'a redonne la main\n");
    }
    printf("[idle] je bloque le systeme\n");
    hlt();
}


void proc1(void) {
    for (;;) {
        printf("[proc1] idle m'a donne la main\n");
        printf("[proc1] je tente de lui la redonner...\n");
        ctx_sw(processTable[1].context, processTable[0].context);
    }
}

void idle(void) {
	for (int i = 0; i < 3; i++) {
        printf("[idle] je tente de passer la main a proc1...\n");
       	ctx_sw(processTable[0].context, processTable[1].context);
        printf("[idle] proc1 m'a redonne la main\n");
    }
    printf("[idle] je bloque le systeme\n");
    hlt();
}
*/
int tstA(void *arg)
{
	arg++;
	unsigned long i;
	while (1){
		printf("A"); /* l'autre processus doit afficher des 'B' */
		__asm__ __volatile__ ("sti"); /* demasquage des interruptions */
		/* une ou plusieurs it du timer peuvent survenir pendant cette boucle d'attente */
		for (i = 0; i < 5000000; i++) {
			//
		}
		__asm__ __volatile__ ("cli"); /* masquage des interruptions */	
	}
}

int tstB(void *arg)
{
	arg++;
	unsigned long i;
	while (1){
		printf("B"); /* l'autre processus doit afficher des 'B' */
		__asm__ __volatile__ ("sti"); /* demasquage des interruptions */
		/* une ou plusieurs it du timer peuvent survenir pendant cette boucle d'attente */
		for (i = 0; i < 5000000; i++) {
			//
		}
		__asm__ __volatile__ ("cli"); /* masquage des interruptions */	
	}
}



void kernel_start(void) {
	efface_ecran();

	Process idleP, proc1P;

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
	processTable[1] = proc1P;

	masque_IRQ(0, 0);

	set_Quartz();

	init_all_traitants();
	
	// Demasquage des interruptions externes
	sti();

	tstA(0);

	while(1)
	  hlt();

	return;

}
