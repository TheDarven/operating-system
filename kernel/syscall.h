#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "process.h"
#include "../shared/stdarg.h"
#include "clock.h"
#include "message.h"
#include "screen.h"
#include "../shared/syscalldef.h"
#include "../shared/stdbool.h"

#define START_USER_SPACE 0x1000000
#define END_USER_SPACE 0x3000000

int sys_call(int numTraitant, int arg1, int arg2, int arg3, int arg4, int arg5);


#endif