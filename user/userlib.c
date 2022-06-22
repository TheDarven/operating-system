#include "userlib.h"

// Interruption printf :
void console_putbytes(const char* chaine, int32_t taille) {
    fct_syscall(TRAITANT_CONSOLE_PUTBYTES, (int)chaine, taille, 0, 0, 0);
}

unsigned long current_clock() {
    return fct_syscall(TRAITANT_CURRENTCLOCK, 0, 0, 0, 0, 0);
}

int kill(int pid) {
    return fct_syscall(TRAITANT_KILL, pid, 0, 0, 0, 0);
}

int chprio(int pid, int newprio) {
    return fct_syscall(TRAITANT_CHPRIO, pid, newprio, 0, 0, 0);
}

void exit(int retval) {
    fct_syscall(TRAITANT_EXIT, retval, 0, 0, 0, 0);

    while (1);
}

int getpid(void) {
    return fct_syscall(TRAITANT_GETPID, 0, 0, 0, 0, 0);
}

int getprio(int pid) {
    return fct_syscall(TRAITANT_GETPRIO, pid, 0, 0, 0, 0);
}

void wait_clock(unsigned long wakeup) {
    fct_syscall(TRAITANT_WAIT_CLOCK, (int) wakeup, 0, 0, 0, 0);
}

int pcreate(int count) {
    return fct_syscall(TRAITANT_PCREATE, count, 0, 0, 0, 0);
}

int pdelete(int fid) {
    return fct_syscall(TRAITANT_PDELETE, fid, 0, 0, 0, 0);
}

int preceive(int fid,int *message) {
    return fct_syscall(TRAITANT_PRECEIVE, fid, (int) message, 0, 0, 0);
}

int preset(int fid) {
    return fct_syscall(TRAITANT_PRESET, fid, 0, 0, 0, 0);
}

int start(int (*pt_func)(void*), unsigned long ssize, int prio, const char *name, void *arg) {
    return fct_syscall(TRAITANT_START, (int) pt_func, (int) ssize, (int) prio, (int) name, (int) arg);
}

void clock_settings(unsigned long *quartz, unsigned long *ticks) {
    fct_syscall(TRAITANT_CLOCK_SETTINGS, (int) quartz, (int) ticks, 0, 0, 0);
}

int pcount(int fid, int *count) {
    return fct_syscall(TRAITANT_PCOUNT, fid, (int) count, 0, 0, 0);
}

int psend(int fid, int message) {
    return fct_syscall(TRAITANT_PSEND, fid, message, 0, 0, 0);
}

int waitpid(int pid, int *retval) {
    return fct_syscall(TRAITANT_WAITPID, pid, (int) retval, 0, 0, 0);
}

