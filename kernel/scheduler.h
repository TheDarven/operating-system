#include "process.h"

#define NBPROC 2
#define SCHEDFREQ 50

extern Process processTable[NBPROC];

extern Process* runningProcess;


void ordonnance(void);

int chprio(int pid, int newprio);
