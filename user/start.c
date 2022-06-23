#include "start.h"

#include "../shared/stdio.h"

int test_run(int n);


void user_start(void) {    
    for (int i = 1; i <= 20; i++) {
        test_run(i);
    }
}
//-exec x/3x 0x2803f10 - 8
