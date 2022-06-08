#include "screen.h"

uint32_t cursorRow = 0;
uint32_t cursorCol = 0;
uint8_t colorBackground = 0;
uint8_t colorText = 15;

// Retourne la case mémoire associée à une position de l'écran
uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    return (uint16_t *) (0xB8000 + 2 * (lig * 80 + col));
}

//Ecrit un caractère sur l'écran à la position indiquée
void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t cb, uint8_t ct) {
    uint16_t* carAdress = ptr_mem(lig, col);
    
    uint8_t ascii = (int)c;
    // Color Background - Noir
    cb = cb & 0x7;
    // Color Text - Blanc
    ct = ct & 0xF;
    // bits 15-8 : format | bits 7-0 : caractère
    *carAdress = (( (cb << 4) | ct ) << 8 ) | ascii;
}

void place_curseur(uint32_t lig, uint32_t col) {
    uint16_t pos = lig * NB_COL + col;
    uint8_t lower = (uint8_t) pos;
    uint8_t higher = (uint8_t) (pos >> 8);

    outb(0x0F, 0x3D4);
    outb(lower, 0x3D5);

    outb(0x0E, 0x3D4);
    outb(higher, 0x3D5);

    cursorRow = lig;
    cursorCol = col;
}

void efface_ecran() {
    for (uint32_t row = 0; row < NB_ROW; row++) {
        for (uint32_t col = 0; col < NB_COL; col++) {
            ecrit_car(row, col, ' ', colorBackground, colorText);
        }
    }
    place_curseur(0, 0);
}

void traiter_car(char c) {
    if (c > 126) {
        return;
    }

    // Caractères normaux
    if (c >= 32) {
        ecrit_car(cursorRow, cursorCol, c, colorBackground, colorText);
        avancer_curseur();
        return;
    }

    // Traitements spéciaux
    uint32_t newCol, newCursorRow;
    switch(c) {
        case '\b':
            if (cursorCol != 0) {
                place_curseur(cursorRow, cursorCol - 1);
            }
            break;
        case '\n':
            newCursorRow = cursorRow + 1;
            if (newCursorRow % NB_ROW == 0) {
                newCursorRow--;
                defilement();
            }
            place_curseur(newCursorRow, 0);
            break;
        case '\f':
            efface_ecran();
            break;
        case '\r':
            place_curseur(cursorRow, 0);
            break;
        case '\t':
            newCol = cursorCol + 8;
            // Aller à la tabulation la plus proche
            newCol -= newCol % 8;
            // Si on arrive à la fin de la ligne
            if (newCol > 79) {
                newCol = 79;
            }
            if (newCol != cursorCol) {
                place_curseur(cursorRow, newCol);
            }
            break;
    }
}

void avancer_curseur() {
    uint32_t newCursorCol = cursorCol + 1;
    uint32_t newCursorRow = cursorRow;
    if (newCursorCol % NB_COL == 0) {
        newCursorCol = 0;
        newCursorRow++;
        if (newCursorRow % NB_ROW == 0) {
            newCursorRow--;
            defilement();
        }
    }
    place_curseur(newCursorRow, newCursorCol);
}

void defilement(void) {
    uint32_t sizeOfBloc = NB_COL * 2 * (NB_ROW - 1);
    // On écrase les 79 première lignes par le contenu des 79 dernières
    memmove(ptr_mem(0, 0), ptr_mem(1, 0), sizeOfBloc);
    for (uint32_t col = 0; col < NB_COL; col++) {
        ecrit_car(NB_ROW - 1, col, ' ', colorBackground, colorText);
    }
}

void console_putbytes(const char *chaine, int32_t taille) {
    for (int i = 0; i < taille; i++) {
        traiter_car(chaine[i]);
    }
}
