#include "timer.h"

int ticks = 0;

void timer_traitement(void) {

    outb(0x20, 0x20);

    increment_nb_interruption();

    ticks = (ticks + 1) % SCHEDFREQ;
    if (ticks == 0) {
        ordonnance();
    }
}