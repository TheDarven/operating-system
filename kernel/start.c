#include "debugger.h"
#include "cpu.h"
#include "process.h"
#include "scheduler.h"
#include "ctx_sw.h"
#include "screen.h"
#include "../shared/printf.c"

void idle(void)
{
    printf("[idle] je tente de passer la main a proc1...\n");
    ctx_sw(processTable[0].context, processTable[1].context);
}

void proc1(void)
{
    printf("[proc1] idle m'a donne la main\n");
    printf("[proc1] j'arrete le systeme\n");
    hlt();
}

void kernel_start(void)
{
	Process idleP, proc1P;

	// Initialise le processus idle
	idleP.pid = 0;
	idleP.state = RUNNING;
	idleP.name = "idle";

	processTable[0] = idleP;

	// Place le processus idle comme processus élu
	runningProcess = &processTable[0];

	// Initialise le processus proc1
	proc1P.pid = 1;
	proc1P.state = READY;
	proc1P.name = "proc1";
	proc1P.executionStack[0] = (uint32_t) proc1;
	proc1P.context[1] = (uint32_t) &(proc1P.executionStack[0]);
	processTable[1] = proc1P;


	idle();

	while(1)
	  hlt();

	return;
}
