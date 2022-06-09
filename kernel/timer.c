#include "timer.h"

int ticks = 0;
int seconds = 0;
int minutes = 0;
int hours = 0;

void timer_traitement(void) {

    outb(0x20, 0x20);

    increment_nb_interruption();

    ticks = (ticks + 1) % SCHEDFREQ;
    if (ticks == 0) {
        Process *exProcess = runningProcess,
                *newProcess;

        if (runningProcess == processTable) {
            newProcess = processTable + 1;
        } else {
            newProcess = processTable;
        }

        runningProcess = newProcess;

        ctx_sw(exProcess->context, newProcess->context);
    }

    /* if (ticks == 0) {
        seconds++;
        if (seconds % 60 == 0) {
            seconds = 0;
            minutes++;
            if (minutes % 60 == 0) {
                minutes = 0;
                hours++;
            }
        }
        printf("%d\n", seconds);
    } */
}