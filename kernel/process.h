#ifndef __PROCESS_H__
#define __PROCESS_H__

#include "../shared/stdint.h"
#include "../shared/queue.h"
#include "../shared/string.h"
#include "mem.h"
#include "../shared/stdbool.h"

#define MAXPRIO 256
#define MAX_NAME_LENGTH 256
#define STACK_SIZE 2048
#define NBPROC 1000
#define FIRST_PID 1

#define NB_ARGS_USER_STACK 128

#define MAX_USER_STACK_SIZE 256000000

#define CONTEXT_SIZE 6
#define ESP 1
#define TSS 5

extern enum State {
    RUNNING, // Elu
    READY, // Activable
    BLOCKED_SEMA, // Bloqué sur un sémaphore
    BLOCKED_IO, // Bloqué sur un I/O
    BLOCKED_CHILD, // Bloqué en attente d'un fils
    SLEEP, // Endormi
    ZOMBIE, // Zombie
    BLOCKED_MSG // Bloqué sur un message
} State;

typedef struct Process {
    struct Process* parent;
    link children; // Tête de queue des fils
    link zombieChildren; // Tête de queue des fils zombies


    link childQueue;  // Queue de tous les fils du parent
    link readyQueue;  // Queue des processus ready
    link deleteQueue; // Queue des processus zombies en attente de suppression
    
    link messageQueue; // Queue des messages (producteurs - consommateurs)
    bool isWaitProducer;
    struct MessageFile* messageFile;
    int message;
    bool waitMessageFile;
    
    link waitQueue;             // Queue des processus sleep
    unsigned long waitTimeout;  // Tick when the process will stop waiting

    link zombieChildQueue; // Queue des processus zombies
    int waitChildPid;

    int priority;
    int pid;
    enum State state;
    char name[MAX_NAME_LENGTH];
    uint32_t context[CONTEXT_SIZE];
    uint32_t executionStack[STACK_SIZE]; 
    uint32_t* userStack;
    int retval;

    unsigned long ssize;
} Process;

// Local vars
extern Process* processTable[NBPROC];
extern link deleteQueueHead;

// Getters / setters
Process* getProcessByPid(int pid);
int getpid(void);
int return_fct();
int getNbStartProcess();
int getprio(int pid);
int chprio(int pid, int newprio);
void switchState(Process* process, enum State newState);


// Cycle de vie des processus
int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);
int kill(int pid);
void exit(int retval);
void stopProcess(Process* process);
void deleteProcess(Process* process);
void addProcessToDeleteQueue(Process* process);
int isDeleteQueueEmpty();
void removeProcessFromDeleteQueue(Process* process);

// Filiation
void addChildren(Process* parent, Process* child);
void removeChild(Process* child);

// Fils Zombies
void addZombieChildren(Process* parent, Process* child);
void removeZombieChild(Process* child);
int isZombieChildrenQueueEmpty(Process* parent);
Process* getFirstZombieChild(Process* parent);

// Attente active
unsigned int sleep(unsigned int nbSecs);
int waitpid(int pid, int *retvalp);

// Mode utilisateur
int switch_user_mode();

void user_exit();

#endif