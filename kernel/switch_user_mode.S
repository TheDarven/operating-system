
.globl switch_user_mode

switch_user_mode:
    # Passage en mode user
    mov $0x4b, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
    iret
