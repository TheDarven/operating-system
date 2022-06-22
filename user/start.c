#include "start.h"

int fct_1(void* a) {
    int b = 5;
    b++;
    return (int) a;
}

void user_start(void) {
    int a = start(fct_1, 4000, 128, "fct_1", (void *) 21);
    int* b = mem_alloc(sizeof(int));
    int c = waitpid(a, b);
    printf("finito: %d \n ", *b);
    printf("finito 2: %d \n ", c);
}

/*
    COMMENT ON FAIT :
    - Interruptions systèmes (On a juste la 49 à faire) :
        - Sauvegarder le contexte d'éxecution à chaque changement de processus
            - Il se fait dans la pile correspondant au niveau de privilège du traitant
                - Cette pile se trouve dans la TSS (0x20000)
            - On sauvegarde :
                - Le pointeur de pile (SS,ESP) (dépend du privilège du traitant, Soit pile pour lvl 0 (user) soit pile pour droit lvl 3 (superviseur)) (0x20004)
                - Le registre EFLAGS,
                - Le compteur ordinal (CS,EIP) du programme interrompu.
                
    - Interruptions utilisateurs : 
        - Utilisation l'interruption 49
            - en mode utilisateur : CS = 0x43, autres 0x4b
            - en mode superviseur : CS = 0x10, autres 0x18
        - Retour au mode utilisateur via iret
*/
