#include "../shared/stdint.h"
#include "cpu.h"

#define CLOCKFREQ 100
#define QUARTZ 0x1234DD

extern unsigned long  nbInterruptions;

void clock_settings(unsigned long *quartz, unsigned long *ticks);

unsigned long current_clock();

void wait_clock(unsigned long clock);

void increment_nb_interruption();

void set_Quartz();