#include "./test_chprio.h"

void fct_prio_up() {
    int laps = 0;

     printf("[START] Fct up\n");

    while (laps < 50) {
        sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
        laps++;
        printf("up(%d)\n", laps);
        if (laps == 20) {
            printf("I switch up my priority to 5");
            chprio(getpid(), 5);
        }
    }
}

void fct_prio_down() {
    int laps = 0;

    printf("[START] Fct down\n");

    while (laps < 100) {
        sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
        laps++;
        printf("down(%d)\n", laps);
        if (laps == 20) {
            printf("I switch down my priority to 4");
            chprio(getpid(), 4);
        }
    }
}

void fct_same_prio() {
    int laps = 0;

     printf("[START] Fct same\n");

    while (laps < 100) {
        sti();
		for (unsigned long i = 0; i < 42949675; i++) {
			//
		}
		cli();
        laps++;
        printf("same(%d)\n", laps);
        if (laps == 5) {
            start((int (*)(void *)) fct_prio_up, 0, 4, "fct_prio_up", NULL);
            start((int (*)(void *)) fct_prio_down, 0, 5, "fct_prio_down", NULL);
        }
    }
}

void prio_idle() {
    start((int (*)(void *)) fct_same_prio, 0, 3, "fct_same_prio", NULL);
    while (1) {
        sti();
        hlt();
        cli();
    }
}

void test_chprio() {
    void* arg = 0;
	start((int (*)(void *)) prio_idle, 0, 1, "prio_idle", arg);
}