#include "kbd.h"

#include "message.h"
#include "cpu.h"
#include "screen.h"

void keyboard_int(void) {
    outb(0x20, 0x20);
    int str = inb(0x60);
    do_scancode(str);
}

int fid;

void keyboard_data(char *str) {
    while (*str != '\0') {

        // Affichage sur l'écran
        if (isEchoEnable) {
            if ((int) *str == 13) {
                // ENTER
                printf("\n");
            } else if (*str == 127) {
                // SUPPR
                printf("\b \b");
            } else if (*str < 127 || *str > 31) {
                printf("%c", *str);
            }
        }

        // Traitement dans la file
        if (*str == 127) {
            // SUPPR
            deleteLastChar(fid);
        } else {
            // Vérifie si la file st pleine
            int count;
            pcount(fid, &count);
            if (count == FILE_SIZE) {
                return;
            }

            psend(fid, (int) *str);
        }
        str++;
    }
}

void kbd_leds(unsigned char leds) {
    outb(0xed, 0x64);
    outb(leds, 0x64);
}

void initFileMessage() {
    fid = pcreate(FILE_SIZE);
}