#include "../shared/stdint.h"
#include "cpu.h"

#define CLOCKFREQ 100
#define QUARTZ 0x1234DD

extern unsigned long ticks;

void clock_settings(unsigned long *quartz, unsigned long *ticks);

unsigned long current_clock();

void set_Quartz();