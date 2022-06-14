#include "./test_waitpid.h"


// #################################################################################################
// Lancement du test 1 :
// #################################################################################################


// Processus fils :
int childProcess(void* arg) {
	(void) arg;
    while(1) {
        sti();
        for (unsigned long i = 0; i < 1000000000; i++) {}
        cli();
    }
	return 7;
}


// Lancement du test 1 :
void test_1() {

	// Récupération des pid :
    int fatherpid = getpid();
	int childpid = start(childProcess, 0, 2, "childProcess", NULL);

	// Appel du waitpid sans erreur (normalement) :
	int retvalp, pid = waitpid(childpid, &retvalp);

    // Vérifie que le résultat est correct :
    assert(childpid == pid); // La fonction 'waitpid' retourne le pid du processus qui a fini
    assert(retvalp == 7); // La fonction 'waitpid' retourne la valeur de retour du processus fils

}


// #################################################################################################
// Lancement du test 2 :
// #################################################################################################


// Lancement du test 2 :
void test_2() {

	// Récupération des pid :
    int fatherpid = getpid();
	int childpid = start(childProcess, 0, 2, "childProcess", NULL);

    // Supprime le processus fils :
    deleteProcess(getProcessByPid(childpid));

	// Appel du waitpid avec une erreur car le prcessus fils n'existe plus (normalement) :
	int retvalp, result = waitpid(childpid, &retvalp);

    // Vérifie si le programme renvoie bien une erreur :
    assert(result < 0);

}


// #################################################################################################
// Lancement des tests du waitpid :
// #################################################################################################


void test_waitpid() {

    printf("Test du waitpid : ");

    // Test si le résultat retourné par le fils est bien récupéré :
    test_1();

    // Cette fonction renvoie une valeur strictement négative si aucun fils n'existe :
    test_2();

    printf("OK");

}









