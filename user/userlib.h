
#ifndef __USERLIB_H__
#define __USERLIB_H__

#include "../shared/stdint.h"
#include "../shared/stddef.h"
#include "../shared/syscalldef.h"
#include "fct_syscall.h"

void console_putbytes(const char* chaine, int32_t taille);

unsigned long current_clock();

int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg);

int kill(int pid);

int chprio(int pid, int newprio);

void exit(int retval);

int getpid(void);

int getprio(int pid);

int kill(int pid);

int pcount(int fid, int *count);

int pcreate(int count);

int pdelete(int fid);

int preceive(int fid,int *message);

int preset(int fid);

int psend(int fid, int message);

void clock_settings(unsigned long *quartz, unsigned long *ticks);

unsigned long current_clock(void);

void wait_clock(unsigned long wakeup);

int start(int (*ptfunc)(void *), unsigned long ssize, int prio, const char *name, void *arg);

int waitpid(int pid, int *retval);

#endif