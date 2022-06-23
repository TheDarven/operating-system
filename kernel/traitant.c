#include "traitant.h"

// Init traitants
void init_traitant_IT(void (*traitant)(void), unsigned int index, unsigned int dpl) {
    uint32_t addr = (uint32_t) traitant;
    uint32_t* table;

    table = (uint32_t *) (ADDRESS_TABLE + index * 2 * sizeof(uint32_t));
    *table = (KERNEL_CS << 16) | (addr & 0x0000FFFF);
    *(table + 1) = (addr & 0xFFFF0000) | 0x8E00 | (dpl << 13);
}

void init_all_traitants() {
    init_traitant_IT(traitant_IT_32, 32, 0);
    init_traitant_IT(traitant_IT_33, 33, 0);
    init_traitant_IT(traitant_IT_49, 49, 3);
}

void masque_IRQ(uint32_t num_IRQ, int masque) {
    // On a une valeur entre 0 et 255
    // On a donc 7 IRQs (7 bits)
    uint8_t IRQs = inb(0x21);
    // On veut masquer
    if (masque == 1) {
        IRQs = IRQs | (1 << num_IRQ);
    } else if (masque == 0) {
        IRQs = IRQs & ((1 << num_IRQ) ^ 255);
    }
    outb(IRQs, 0x21);
}