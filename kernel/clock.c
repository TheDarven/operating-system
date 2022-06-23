#include "clock.h"
#include "process.h"
#include "scheduler.h"

unsigned long ticks = 0;

void clock_settings(unsigned long *quartz, unsigned long *ticks) {
    if (quartz != NULL) {
        *quartz = QUARTZ;
    }
    if (ticks != NULL) {
        *ticks = (QUARTZ / CLOCKFREQ);
    }
}

unsigned long current_clock() {
    return ticks;
}

void set_Quartz() {

    unsigned long value = QUARTZ / CLOCKFREQ;

    // Cette commande indique a l'horloge que l'on va lui envoyer la valeur de reglage de la frequence sous la forme de deux valeurs de 8 bits chacunes qui seront emises sur le port de donnees
    outb(0x34, 0x43);

    // On envoie les 8 bits de poids faibles de la valeur de reglage de la frequence sur le port de donnees 0x40 :
    uint8_t lower = value %256;
    outb(lower, 0x40);

    // On envoie les 8 bits de poids forts de la valeur de reglage de la frequence sur le port de donnees 0x40 :
    uint8_t higher = value >> 8;
    outb(higher, 0x40);
    
}

void wait_clock(unsigned long clock) {
    runningProcess->waitTimeout = clock;

    switchState(runningProcess, SLEEP);

    ordonnance();
}
