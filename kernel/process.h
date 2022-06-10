#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../shared/stdint.h"
#include "../shared/queue.h"
#include "../shared/string.h"
#include "mem.h"

#define MAXPRIO 256
#define MAX_NAME_LENGTH 256
#define STACK_SIZE 256
#define NBPROC 10

#define CONTEXT_SIZE 5
#define ESP 1

extern enum State {
    RUNNING, // Elu
    READY, // Activable
    BLOCKED_SEMA, // Bloqué sur un sémaphore
    BLOCKED_IO, // Bloqué sur un I/O
    BLOCKED_CHILD, // Bloqué en attente d'un fils
    SLEEP, // Endormi
    ZOMBIE // Zombie
} State;

typedef struct {
    link next;
    int priority;
    int pid;
    enum State state;
    char name[MAX_NAME_LENGTH];
    uint32_t context[CONTEXT_SIZE];
    uint32_t executionStack[STACK_SIZE]; // [USER] Passer en pointeur pour alloc en user
} Process;

extern Process* processTable[NBPROC];

Process* getProcessByPid(int pid);

int getpid(void);

// int waitpid(int pid, int *retvalp);

int getprio(int pid);

int chprio(int pid, int newprio);

int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);

int kill(int pid);

int getNbStartProcess();

// unsigned int sleep(unsigned int nbSecs);

#endif