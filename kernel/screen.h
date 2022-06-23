#include "stdint.h"
#include "cpu.h"
#include "string.h"
#include "../shared/stdbool.h"
#include "kbd.h"

#define NB_COL 80
#define NB_ROW 25

extern uint32_t cursorRow;
extern uint32_t cursorCol;
extern uint8_t colorBackground;
extern uint8_t colorText;

extern bool isEchoEnable;

uint16_t *ptr_mem(uint32_t lig, uint32_t col);

void ecrit_car(uint32_t lig, uint32_t col, char c, uint8_t colorBackground, uint8_t colorText);

void place_curseur(uint32_t lig, uint32_t col);

void efface_ecran();

void avancer_curseur();

void defilement(void);

void console_putbytes(const char *chaine, int32_t taille);

void cons_write(const char *str, long size);

int cons_read(char *string, unsigned long length);

void cons_echo(int on);