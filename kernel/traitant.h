#include "segment.h"
#include "../shared/stdint.h"
#include "timer.h"
#include "cpu.h"

#define NB_VECTOR 256
#define ADDRESS_TABLE 0x1000

// Traitants
void traitant_IT_32();

void traitant_IT_49();

// Init traitants
void init_traitant_IT(void (*traitant)(void), unsigned int index, unsigned int dpl);

void init_all_traitants();

void masque_IRQ(uint32_t num_IRQ, int masque);