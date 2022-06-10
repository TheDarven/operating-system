#include "scheduler.h"
#include "../shared/queue.h"
#include "ctx_sw.h"

Process* runningProcess = NULL;

link processQueue = LIST_HEAD_INIT(processQueue);


// La fonction ordonnance doit être adaptée pour implanter la politique du tourniquet,
// qui active les processus dans l'ordre de leur pid : 0, 1, 2, 3, 0, 1, 2, 3, etc
void ordonnance(void) {
    // Traitement si la liste des processus est pas vide
    if (!queue_empty(&processQueue)) {
        // Réordonne la liste selon les priorités
        Process* nextProcess = queue_out(&processQueue, Process, next);
        addProcessToQueue(nextProcess);

        // Changement de contexte si on change de processus
        if (runningProcess != nextProcess) {
            nextProcess->state = RUNNING;

            if (runningProcess == NULL) {
                runningProcess = nextProcess;
            } else {
                Process* exRunningProcess = runningProcess;
                exRunningProcess->state = READY;

                runningProcess = nextProcess;
                runningProcess->state = RUNNING;

                ctx_sw((exRunningProcess->context), (nextProcess->context));
            }
        }
    } else {
        // TODO : Kill de 1
    }
}

void addProcessToQueue(Process* process) {
    queue_add(process, &processQueue, Process, next, priority);
}

void removeProcessFromQueue(Process* process) {
    queue_del(process, next);
}

Process* getHighestPriorityProcess() {
    return queue_top(&processQueue, Process, next);
}


// Remplir la processQueue
    // Quand on créer un processus
    // Quand un processus repasse à READY

// Enlever un processus de la queue
    // Un processus est terminé
    // Un processus bloqué, interrompu ou Endormis

