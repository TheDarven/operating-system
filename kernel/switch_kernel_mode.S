
.globl switch_kernel_mode

switch_kernel_mode:
    # Passage en mode user
    mov $0x18, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
    ret
