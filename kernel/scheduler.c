#include "scheduler.h"

Process* runningProcess = 0;

Process processTable[NBPROC] = {0};

// La fonction ordonnance doit être adaptée pour implanter la politique du tourniquet,
// qui active les processus dans l'ordre de leur pid : 0, 1, 2, 3, 0, 1, 2, 3, etc
void ordonnance(void) {

    // Cherche le prochain processus à lancer
    Process nextProcess = processTable[0];
    for(int i = 0; i < NBPROC; i++) {
        if(processTable[i].priority > nextProcess.priority || processTable[i].priority == nextProcess.priority && processTable[i].time < nextProcess.time) {
            nextProcess = processTable[i];
        }
    }

    // Le processus en cours d'exécution donne la main au processus plus prioritaire
    if (runningProcess->context != nextProcess.context) {
        // runningProcess->time 

        ctx_sw(runningProcess->context, nextProcess.context);
        runningProcess = &nextProcess;
    }
}


// Permet de changer la priorité d'un processus donné
int chprio(int pid, int newprio) {

    // Retourne une valeur négative si la valeur de 'newprio' n'est pas appropriée
    if(newprio < 0 || newprio > MAXPRIO) {
        return -1;
    }

    int i, lastPriority = -1;
    Process process;

    // Récupération du processus avec le pid donné
    for(i = 0; i < NBPROC; i++) {
        if(processTable[i].pid == pid) {
            process = processTable[i];
            lastPriority = process.priority;
            break;
        }
    }

    // Changement de la priorité du processus
    process.priority = newprio;

    // Met à jour l'ordonnancement des processus
    ordonnance();

    // Retourne l'ancienne priorité du processus
    return lastPriority;

}
