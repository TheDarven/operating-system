#include "process.h"
#include "scheduler.h"
#include "message.h"
#include "clock.h"
#include "user_stack_mem.h"
#include "segment.h"

/* 
    LOCAL VARS
*/

Process* processTable[NBPROC] = {0};

link deleteQueueHead = LIST_HEAD_INIT(deleteQueueHead);

int nbStartProcess = 0;

Process* getProcessByPid(int pid) {
    // Récupération du processus avec le pid donné
    for(int i = 0; i < nbStartProcess; i++) {
        if(processTable[i]->pid == pid) {
            return processTable[i];
        }
    }
    return NULL;
}

/*
    GETTERS / SETTERS
*/

// La valeur de retour de 'getpid' est le pid du processus appelant cette primitive
int getpid(void) {
    return runningProcess->pid;
}


// Retourne la priorité d'un processus dont on connaît le pid.
int getprio(int pid) {    
    Process* process = getProcessByPid(pid);
    if (process == NULL) {
        return -1;
    }
    return process->priority;
}

int getNbStartProcess() {
    return nbStartProcess;
}

// Permet de changer la priorité d'un processus donné
int chprio(int pid, int newprio) {

    // Retourne une valeur négative si la valeur de 'newprio' n'est pas appropriée
    if(newprio < 1 || newprio > MAXPRIO) {
        return -1;
    }

    int lastPriority;
    Process* process = getProcessByPid(pid);
    
    // Si aucun processus n'a ce PID
    if (process == NULL || process->state == ZOMBIE) {
        return -1;
    }

    lastPriority = process->priority;

    // Si la priorité ne change pas
    if (newprio == lastPriority) {
        return lastPriority;
    }

    // Changement de la priorité du processus
    process->priority = newprio;

    if (process->state == READY || process->state == RUNNING) {
        removeProcessFromReadyQueue(process);
        addProcessToReadyQueue(process);
    }

    if (process->state == BLOCKED_MSG && process->messageFile != NULL) {
        queue_del(process, messageQueue);
        if (process->isWaitProducer) {
            queue_add(process, &(process->messageFile->blockedProducerQueueHead), Process, messageQueue, priority); 
        } else {
            queue_add(process, &(process->messageFile->blockedConsumerQueueHead), Process, messageQueue, priority); 
        }
    }



    // Met à jour l'ordonnancement des processus
    // Si il est en tête de la queue, il doit être lancer car cela signifie soit :
    // - qu'il est le premier processus
    // - qu'il est le processus avec la plus haute priorité
    if (getHighestPriorityProcess() == process || runningProcess == process) {
        ordonnance();
    }
    
    // Retourne l'ancienne priorité du processus
    return lastPriority;
}


void switchState(Process* process, enum State newState) {

    switch(newState) {

        case ZOMBIE:
            removeProcessFromReadyQueue(process);
            removeProcessFromWaitQueue(process);
            process->priority = 1;
            addZombieChildren(process->parent, process);
        break;

        case BLOCKED_CHILD:
        case BLOCKED_MSG:
            removeProcessFromReadyQueue(process);
            removeProcessFromWaitQueue(process);
        break;

        case SLEEP:
            removeProcessFromReadyQueue(process);
            addProcessToWaitQueue(process);
        break;

        case RUNNING:
            addProcessToReadyQueue(process);
            removeProcessFromWaitQueue(process);
        break;
        
        case READY:
            addProcessToReadyQueue(process);
            removeProcessFromWaitQueue(process);
        break;

        case BLOCKED_SEMA:
            // TODO
        break;

        case BLOCKED_IO:
            // TODO
        break;
        
    }

    process->state = newState;
}

/*
    PROCESS LIFECYLE
*/


// Initialisation d'un nouveau processus :
int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg) {
    
    // Si le paramètre 'prio' est incorrect
    if (prio < 1 || prio > MAXPRIO) {
        return -1;
    }

    // Si plus de place dans la table des processus
    if (nbStartProcess == NBPROC) {
        return -1;
    }

    if (ssize > MAX_USER_STACK_SIZE) {
        return -1;
    }

    // Initialise le nouveau processus
    Process* newProcess = mem_alloc(sizeof(Process));
    if (!newProcess) {
        return -1;
    }

    // Définit la valeur du pid
    int pid;
    if (nbStartProcess == 0) {
       pid = FIRST_PID;
    } else {
        pid = processTable[nbStartProcess - 1]->pid + 1;
    }
    newProcess->pid = pid;

    // Initialise l'état du processus
    newProcess->state = READY;

    strcpy(newProcess->name, name);

    // Initialise la priorité du processus
    newProcess->priority = prio;

    // Construit la pile d'exécution user
    if (pid == FIRST_PID) {
        // Si processus principal
        newProcess->executionStack[STACK_SIZE - 4] = (uint32_t) arg;
        newProcess->executionStack[STACK_SIZE - 5] = (uint32_t) return_fct;
        newProcess->executionStack[STACK_SIZE - 6] = (uint32_t) pt_func;

        // Context
        newProcess->context[ESP] = (uint32_t) & (newProcess->executionStack[STACK_SIZE - 6]);
    } else {
        // Sinon processus user
        /* newProcess->ssize = ssize + sizeof(uint32_t) * NB_ARGS_USER_STACK;

        newProcess->userStack = user_stack_alloc(newProcess->ssize);
        if (newProcess->userStack == NULL) {
            mem_free(newProcess, sizeof(Process));
            return -1;
        }

        uint32_t * userExit = (uint32_t *) (newProcess->userStack + newProcess->ssize - 2 * sizeof(uint32_t));

        *userExit = (uint32_t) 0x1000005;
        *(userExit + 1) = (uint32_t) arg; */

        newProcess->ssize = (ssize + NB_ARGS_USER_STACK) * sizeof(uint32_t);

        newProcess->userStack = user_stack_alloc(newProcess->ssize);
        if (newProcess->userStack == NULL) {
            mem_free(newProcess, sizeof(Process));
            return -1;
        }

        newProcess->userStack[ssize + NB_ARGS_USER_STACK - 2] = (uint32_t) 0x1000005;
        newProcess->userStack[ssize + NB_ARGS_USER_STACK - 1] = (uint32_t) arg;

        newProcess->executionStack[STACK_SIZE - 1] = USER_DS;
        // newProcess->executionStack[STACK_SIZE - 2] = (uint32_t) userExit;
        newProcess->executionStack[STACK_SIZE - 2] = (uint32_t) &newProcess->userStack[ssize + NB_ARGS_USER_STACK - 2];
        newProcess->executionStack[STACK_SIZE - 3] = 0x202;
        newProcess->executionStack[STACK_SIZE - 4] = USER_CS;
        newProcess->executionStack[STACK_SIZE - 5] = (uint32_t) pt_func;
        newProcess->executionStack[STACK_SIZE - 6] = (uint32_t) switch_user_mode;

        // USER DS dans ax
        // ss dans bx, cx, dx, ex

        // Context
        newProcess->context[ESP] = (uint32_t) & (newProcess->executionStack[STACK_SIZE - 6]);
        newProcess->context[TSS] = (uint32_t) & (newProcess->executionStack[STACK_SIZE - 5]);
    }


    // Initialisation de la liste des enfants
    newProcess->children = (link) LIST_HEAD_INIT(newProcess->children);
    
    // Initialisation de la liste des enfants zombies
    newProcess->zombieChildren = (link) LIST_HEAD_INIT(newProcess->zombieChildren);

    // Initialise le père du processus
    if (nbStartProcess != 0) {
        Process* daddy = getProcessByPid(getpid());
        addChildren(daddy, newProcess);
    } else {
        newProcess->parent = NULL;
    }

    // Ajoute le processus dans la table des processus
    processTable[nbStartProcess] = newProcess;
    
    addProcessToReadyQueue(newProcess);
    
    nbStartProcess++;

    // Si le processus est en tête de file, il doit être lancé car cela signifie soit :
    // - qu'il est le premier processus
    // - qu'il est le processus avec la plus haute priorité
    if (runningProcess == NULL || getHighestPriorityProcess()->pid == newProcess->pid) {
        ordonnance();
    }

    return pid;
}

// Permet de mettre fin à un processus donné
int kill(int pid) {
    
    // Récupération du processus dont on connaît le pid
    Process* process = getProcessByPid(pid);
    if (process == NULL || process->state == ZOMBIE) {
        return -1;
    }

    process->retval = 0;

    stopProcess(process);
    
    return 0;
}

void exit(int retval) {
    // Récupère la valeur de retour
    runningProcess->retval = retval;
    stopProcess(runningProcess);
    // La fonction exit ne termine jamais
    while(1);
}

void stopProcess(Process* process) {
    
    // Retire le processus des queues d'attentes
    removeProcessFromWaitQueue(process);
    removeProcessFromReadyQueue(process);
    removeFromMessageFile(process);
    Process* child;

    // Supprimer tous les enfants zombies
    
    while(queue_empty(&(process->children)) == 0) {
        queue_for_each(child, &(process->children), Process, childQueue) {
            removeChild(child);
            if(child->state == ZOMBIE) {
                deleteProcess(child);
            }
            break;
        }
    }
    // Le processus passe dans l'état zombie si son père existe toujours
    if (process->parent != NULL) {
        Process* parent = process->parent;
        switchState(process, ZOMBIE);



        if (parent->state == BLOCKED_CHILD && (parent->waitChildPid == -1 || parent->waitChildPid == process->pid)) {
            switchState(parent, READY);
        }
    }
    // sinon il est immédiatement détruit (le pid est libéré)
    else {
        // Supprime le processus
        if (process == runningProcess) {
            addProcessToDeleteQueue(process);
        } else {
            deleteProcess(runningProcess);
        }


    }
    // Arrête le processus s'il est en cours d'exécution
    if (runningProcess == process) {
        ordonnance();
    }
}

void deleteProcess(Process* process) {
    // Retire le processus de la table des processus
    int i, find = 0;

    for(i = 0; i < nbStartProcess; i++) {
        if (find == 0) {
            if(processTable[i] == process) {
                find = 1;
            }
        } else {
            processTable[i - 1] = processTable[i];
        }
    }
    
    // Si le processus n'a pas été trouvé
    if(find == 0) {
        return;
    }

    nbStartProcess--;

    if (process->pid != FIRST_PID && process->userStack != NULL) {
        // Si processus user
        user_stack_free(process->userStack, process->ssize);
    }

    if (process->parent != NULL) {
        removeZombieChild(process);

        removeChild(process);
    }
    mem_free(process, sizeof(Process));
}

void addProcessToDeleteQueue(Process* process) {
    queue_add(process, &deleteQueueHead, Process, deleteQueue, pid);
}

void removeProcessFromDeleteQueue(Process* process) {
    if (!isDeleteQueueEmpty() && process->deleteQueue.prev != NULL) {
        queue_del(process, deleteQueue);
    }
}

int isDeleteQueueEmpty() {
    return queue_empty(&deleteQueueHead) != 0;
}

/*
    FILIATION
*/
void addChildren(Process* parent, Process* child) {
    queue_add(child, &(parent->children), Process, childQueue, pid);
    child->parent = parent;
}

void removeChild(Process* child) {
    queue_del(child, childQueue);
    child->parent = NULL;
}
/*
    FILIATION 
        ZOMBIE
*/
void addZombieChildren(Process* parent, Process* child) {
    queue_add(child, &(parent->zombieChildren), Process, zombieChildQueue, priority);
}

void removeZombieChild(Process* child) {
    if (child->parent != NULL && !isZombieChildrenQueueEmpty(child->parent) && child->zombieChildQueue.prev != NULL) {
        queue_del(child, zombieChildQueue);
    }
}

Process* getFirstZombieChild(Process* parent) {
    return queue_top(&(parent->zombieChildren), Process, zombieChildQueue);
}

int isZombieChildrenQueueEmpty(Process* parent) {
    return queue_empty(&(parent->zombieChildren)) != 0;
}
/*
    ATTENTE ACTIVE
*/

int waitpid(int pid, int *retvalp) {
    Process* targetProcess;

    runningProcess->waitChildPid = pid;

    if (pid < 0) {

        // Le processus n'a aucun fils
        if (queue_empty(&runningProcess->children)) {
            return -1;
        }

        // Le processus n'a aucun fils en zombie
        if (isZombieChildrenQueueEmpty(runningProcess)) {
            switchState(runningProcess, BLOCKED_CHILD);
            ordonnance();
        }

        // Récupère le premier fils a être passé en état Zombie
        targetProcess = getFirstZombieChild(runningProcess);
    } else {
        // Si aucun processus n'a ce PID ou que le processus n'est pas son enfant.
        targetProcess = getProcessByPid(pid);
        if (targetProcess == NULL || targetProcess->parent != runningProcess) {
            return -1;
        }

        if (targetProcess->state != ZOMBIE) {
            switchState(runningProcess, BLOCKED_CHILD);
            ordonnance();
        }
    }

    int processPid = targetProcess->pid;

    if (retvalp != NULL) {
        *retvalp = targetProcess->retval;
    }

    deleteProcess(targetProcess);

    return processPid;
}

unsigned int sleep(unsigned int nbSecs) {

    // Défini la date à laquelle le processus se réveillera
    runningProcess->waitTimeout = current_clock() + nbSecs * CLOCKFREQ;

    // Le processus passe à l'état endormi
    switchState(runningProcess, SLEEP);

    // Recherche un nouveau processus à passer en mode running
    ordonnance();

    // Retourne le temps restant à dormir
    if (runningProcess->state == SLEEP) {
        return runningProcess->waitTimeout - ticks;
    }

    return 0;
}
