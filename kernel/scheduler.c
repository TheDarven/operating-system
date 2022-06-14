#include "scheduler.h"
#include "../shared/queue.h"
#include "ctx_sw.h"

#include "screen.h"

Process* runningProcess = NULL;

link processQueue = LIST_HEAD_INIT(processQueue);

link waitQueueHead = LIST_HEAD_INIT(waitQueueHead);

// La fonction ordonnance doit être adaptée pour implanter la politique du tourniquet,
// qui active les processus dans l'ordre de leur pid : 0, 1, 2, 3, 0, 1, 2, 3, etc
void ordonnance(void) {
    // Traitement si la liste des processus est pas vide
    if (!isReadyQueueEmpty()) {
        // Réordonne la liste selon les priorités
        Process* nextProcess = queue_out(&processQueue, Process, readyQueue);
        switchState(nextProcess, RUNNING);

        // Changement de contexte si on change de processus
        if (runningProcess != nextProcess) {
            if (runningProcess == NULL) {
                runningProcess = nextProcess;
            } else {
                Process* exRunningProcess = runningProcess;
                if (exRunningProcess->state == RUNNING) {
                    switchState(nextProcess, READY);
                }

                runningProcess = nextProcess;

                ctx_sw(exRunningProcess->context, nextProcess->context);
            }
        }
    } else {
        // TODO : Kill de 1
    }
}

void addProcessToReadyQueue(Process* process) {
    queue_add(process, &processQueue, Process, readyQueue, priority);
}

void removeProcessFromReadyQueue(Process* process) {
    if (!isReadyQueueEmpty() && process->readyQueue.prev != NULL) {
        queue_del(process, readyQueue);
    }
}

int isReadyQueueEmpty() {
    return queue_empty(&processQueue) != 0;
}

Process* getHighestPriorityProcess() {
    return queue_top(&processQueue, Process, readyQueue);
}

void addProcessToWaitQueue(Process* process) {
    queue_add(process, &waitQueueHead, Process, waitQueue, waitTimeout);
}

void removeProcessFromWaitQueue(Process* process) {
    if (!isWaitQueueEmpty() && process->waitQueue.prev != NULL) {
        queue_del(process, waitQueue);
    }
}

int isWaitQueueEmpty() {
    return queue_empty(&waitQueueHead) != 0;
}

// Remplir la processQueue
    // Quand on créer un processus
    // Quand un processus repasse à READY

// Enlever un processus de la queue
    // Un processus est terminé
    // Un processus bloqué, interrompu ou Endormis

