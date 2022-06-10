#include "process.h"
#include "scheduler.h"

Process* processTable[NBPROC] = {0};

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


// La valeur de retour de 'getpid' est le pid du processus appelant cette primitive
int getpid(void) {
    return runningProcess->pid;
}


// TODO : Attention, ne sait pas encore comment récupérer les fils d'un processus !!
/* int waitpid(int pid, int *retvalp) {

    // Si le paramètre pid est négatif, le processus appelant attend qu'un de ses fils, n'importe lequel,
    // soit terminé et récupère (le cas échéant) sa valeur de retour dans *retvalp, à moins que retvalp soit nul.
    // Cette fonction renvoie une valeur strictement négative si aucun fils n'existe ou sinon le pid de celui
    // dont elle aura récupéré la valeur de retour.
    if (pid < 0) {
        int *result = retvalp;
        // Parcours la liste des fils du processus dont on connaît le pid, puis traite le premier qui est terminé
        for(//Parcours des fils) {
            if(//Si le fils a terminé) {
                // On stock le résultat du fils dans 'retvalp'
                // return le pid du fils
            }
        }
    } else {

    }
    
}
*/

// Retourne la priorité d'un processus dont on connaît le pid.
int getprio(int pid) {    
    Process* process = getProcessByPid(pid);
    if (process == NULL) {
        return -1;
    }
    return process->priority;
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
    if (process == NULL) {
        return -1;
    }

    lastPriority = process->priority;

    // Si la priorité ne change pas
    if (newprio == lastPriority) {
        return lastPriority;
    }

    // Changement de la priorité du processus
    process->priority = newprio;

    removeProcessFromQueue(process);

    addProcessToQueue(process);

    // Met à jour l'ordonnancement des processus
    // Si il est en tête de la queue, il doit être lancer car cela signifie soit :
    // - qu'il est le premier processus
    // - qu'il est le processus avec la plus haute priorité
    if (getHighestPriorityProcess() == process) {
        ordonnance();
    }
    
    // Retourne l'ancienne priorité du processus
    return lastPriority;
}


// Permet de mettre fin à un processus donné
int kill(int pid) {
    
    // Récupération du processus dont on connaît le pid
    Process* process;
    int i, find = 0;

    // Retire le processus de la table des processus
    for(i = 0; i < nbStartProcess; i++) {
        if (find == 0) {
            if(processTable[i]->pid == pid) {
                process = processTable[i];
                find = 1;
            }
        } else {
            processTable[i - 1] = processTable[i];
        }
    }
    
    // Si le processus n'a pas été trouvé
    if(find == 0) {
        return -1;
    }

    nbStartProcess--;

    // Retire le processus de la file d'attente
    removeProcessFromQueue(process);

    // Arrête le processus si il est en cours d'exécution
    if (runningProcess == process) {
        ordonnance();
    }

    // [USER] Free le context (user/mem.h)

    mem_free(process, sizeof(Process));
    
    return 0;
}


int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg) {
    ssize++; // [USER] Supprimer cette ligne
    
    // Si l'un des paramètres est incorrect
    if (prio < 1 || prio > MAXPRIO) {
        return -1;
    }

    // [USER] Verifier ssize correct (STACK_SIZE < sizeof(uint32_t*) * ssize) ?

    // Si plus de place dans la table des processus
    if (nbStartProcess == NBPROC) {
        return -1;
    }

    // Initialise le processus
    Process* newProcess = mem_alloc(sizeof(Process));

    if (nbStartProcess == 0) {
        newProcess->pid = 1;
    } else {
        newProcess->pid = processTable[nbStartProcess - 1]->pid + 1;
    }

    newProcess->state = READY;

    strcpy(newProcess->name, name);

    newProcess->priority = prio;

    // [USER] newProcess->executionStack = mem_alloc(sizeof(uint32_t*) * ssize) + @retour + param + autres éléments;
    newProcess->executionStack[STACK_SIZE - 1] = (uint32_t) pt_func;
    newProcess->executionStack[STACK_SIZE - 2] = (uint32_t) arg;

    newProcess->context[ESP] = (uint32_t) & (newProcess->executionStack[STACK_SIZE - 1]);

    processTable[nbStartProcess] = newProcess;
    
    addProcessToQueue(processTable[nbStartProcess]);

    nbStartProcess++;

    // Si il est en tête de la queue, il doit être lancer car cela signifie soit :
    // - qu'il est le premier processus
    // - qu'il est le processus avec la plus haute priorité
    if (runningProcess == NULL || getHighestPriorityProcess()->pid == newProcess->pid) {
        ordonnance();
    }

    return newProcess->pid;
}

/* unsigned int sleep(unsigned int nbSecs) {

} */

int getNbStartProcess() {
    return nbStartProcess;
}

