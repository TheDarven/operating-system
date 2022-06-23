#include "process.h"
#include "screen.h"
#include "traitant.h"
#include "../shared/string.h"
#include "../user/start.h"
#include "kbd.h"

/////////////////////////////////////////////////////////////////////////////////////


void idle(void) {
	
 	start((int (*)(void *)) user_start, 4000, 128, "user_start", NULL);

	while (1) {
		sti();
		hlt();
		cli();
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void kernel_start(void) {
	
	efface_ecran();

	set_Quartz();

	init_all_traitants();

	masque_IRQ(0, 0);

	masque_IRQ(1, 0);

	initFileMessage();

	start((int (*)(void *)) idle, 4000, 1, "idle", NULL);

	idle();

	while (1) {	
		hlt();
	}

}
