#include "syscall.h"

int sys_call(int numTraitant, int arg1, int arg2, int arg3, int arg4, int arg5) {

    // Sauvegarde du contexte d'exécution du programme en cours :

    // Exécute un programme associé à l'interruption qui est appelé le traitant de l'interruption :
    outb(0x20, 0x20);

    switch(numTraitant) {

        case TRAITANT_START:
            return start(
                (int (*)(void *))arg1,
                (unsigned long)arg2,
                arg3,
                (const char *)arg4,
                (void *)arg5
            );

        case TRAITANT_CURRENTCLOCK:
            return current_clock();
        
        case TRAITANT_EXIT:
            exit(arg1);
            break;
        
        case TRAITANT_GETPID:
            return getpid();
        
        case TRAITANT_GETPRIO:
            return getprio(arg1);

        case TRAITANT_CHPRIO:
            return chprio(arg1, arg2);
        
        /* case TRAITANT_CONS_WRITE:
            cons_write((char *)arg1, (long)arg2);
            return 0;

        case TRAITANT_CONS_ECHO:
            return cons_echo(arg1);
        
        case TRAITANT_CONS_READ:
            return cons_read((char *)arg1, (unsigned long)arg2);
        */

        case TRAITANT_KILL:
            return kill(arg1);
        
        case TRAITANT_PCREATE:
            return pcreate(arg1);
        
        case TRAITANT_PSEND:
            return psend(arg1, arg2);

        case TRAITANT_PCOUNT: 
            return pcount(arg1, (int *)arg2);
        
        case TRAITANT_PDELETE:
            return pdelete(arg1);
        
        case TRAITANT_PRECEIVE:
            return preceive(arg1, (int *)arg2);
        
        case TRAITANT_PRESET:
            return preset(arg1);

        case TRAITANT_WAIT_CLOCK:
            wait_clock((unsigned long)arg1); // Attention conversion d'un int en long !
            return 0;

        case TRAITANT_CLOCK_SETTINGS:
            clock_settings((unsigned long *)arg1, (unsigned long *)arg2);
            return 0;

        case TRAITANT_WAITPID:
            return waitpid(arg1, (int* )arg2);

        case TRAITANT_CONSOLE_PUTBYTES:
            console_putbytes((const char*)arg1, (int32_t) arg2);
            return 0;
        
    };

    return -1;
}

