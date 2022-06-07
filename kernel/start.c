#include "debugger.h"
#include "cpu.h"
#include "screen.h"
#include "../shared/printf.c"

int fact(int n)
{
	if (n < 2)
		return 1;

	return n * fact(n-1);
}


void kernel_start(void)
{
	int i;
	// call_debugger(); useless with qemu -s -S

	i = 10;

	efface_ecran();
	printf("Hugo est tres beau");
	printf("\n%d", fact(i));

	while(1)
	  hlt();

	return;
}
