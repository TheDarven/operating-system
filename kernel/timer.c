#include "timer.h"
#include "process.h"

void timer_traitement(void) {

    outb(0x20, 0x20);

    ticks++;

    // Processus à supprimer
    Process* process;
    bool shouldCheck = true;
    while (shouldCheck == true && !isDeleteQueueEmpty()) {
        shouldCheck = false;
        queue_for_each_prev(process, &deleteQueueHead, Process, deleteQueue) {
            if (process != runningProcess) {
                removeProcessFromDeleteQueue(process);
                deleteProcess(process);
                shouldCheck = true;
                break;
            }
        }
    }

    // Réveil des processus
    Process* sleepProcess;
    bool shouldOrdonnance = false;
    shouldCheck = true;

    while (shouldCheck == true && !isWaitQueueEmpty()) {
        queue_for_each(sleepProcess, &waitQueueHead, Process, waitQueue) {
            if (sleepProcess->waitTimeout <= ticks) {
                switchState(sleepProcess, READY);
                addProcessToReadyQueue(sleepProcess);
                if (getHighestPriorityProcess() == sleepProcess) {
                    shouldOrdonnance = true;
                }
            } else {
                shouldCheck = false;
            }
            // Si on supprime, le foreach ne fonctionne plus
            // Sinon, on sait que plus aucun processus en attente ne doit être réveillé (grâce à la priorité sur tickTimeout)
            break;
        }
    }

    if (shouldOrdonnance || ticks % SCHEDFREQ == 0) {
        ordonnance();
    }
}