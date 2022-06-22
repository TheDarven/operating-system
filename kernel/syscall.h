#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "process.h"
#include "../shared/stdarg.h"
#include "clock.h"
#include "message.h"
#include "screen.h"
#include "../shared/syscalldef.h"

int sys_call(int numTraitant, int arg1, int arg2, int arg3, int arg4, int arg5);


#endif