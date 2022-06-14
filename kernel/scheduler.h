#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "process.h"
#include "../shared/string.h"

#define SCHEDFREQ 50

extern Process* runningProcess;

extern link waitQueueHead;

void ordonnance(void);

void initIdle(void);

void addProcessToReadyQueue(Process* process);

void removeProcessFromReadyQueue(Process* process);

int isReadyQueueEmpty();

Process* getHighestPriorityProcess();

void addProcessToWaitQueue(Process* process);

void removeProcessFromWaitQueue(Process* process);

int isWaitQueueEmpty();

#endif