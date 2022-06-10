#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include "process.h"
#include "../shared/string.h"

#define SCHEDFREQ 50

extern Process* runningProcess;

void ordonnance(void);

void initIdle(void);

void addProcessToQueue(Process* process);

void removeProcessFromQueue(Process* process);

Process* getHighestPriorityProcess();

#endif