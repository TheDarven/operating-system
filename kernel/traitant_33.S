# cette directive sert a rendre l'etiquette publique
.globl traitant_IT_33
# debut du traitant 
traitant_IT_33:
# sauvegarde des registres importants
    pushl %eax
    pushl %ebx
    pushl %edx
    pushl %ecx
# Passe en mode kernel
    mov $0x18, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
# appel a la fonction C realisant le traitant
    call keyboard_int
# Passage en mode user
    mov $0x4b, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
# restauration des registres importants
    popl %ecx
    popl %edx
    popl %ebx
    popl %eax
# fin du traitant et retour de la valeur de retour du syscall
    iret
