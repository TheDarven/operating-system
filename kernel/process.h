#include "../shared/stdint.h"

#define MAXPRIO 256
#define MAX_NAME_LENGTH 256
#define STACK_SIZE 256

#define ESP 1

#ifndef PROCESS_H
#define PROCESS_H

extern enum State {
    RUNNING, // Elu
    READY, // Activable
    STOPPED,
    ZOMBIE
} State;

typedef struct {
    int pid;
    enum State state;
    //char name[MAX_NAME_LENGTH];
    char* name;
    uint32_t context[5];
    uint32_t executionStack[STACK_SIZE];
} Process;

#endif