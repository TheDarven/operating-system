# cette directive sert a rendre l'etiquette publique
.globl traitant_IT_49
# debut du traitant 
traitant_IT_49:
# Passage des paramètres
    pushl %esi
    pushl %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
# Passe en mode kernel
    mov $0x18, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
# appel a la fonction C realisant le traitant
    call sys_call
# Passage en mode user
    mov $0x4b, %bx
    mov %bx, %ds
    mov %bx, %es
    mov %bx, %fs
    mov %bx, %gs
# restauration des registres importants (pour le mode user)
    popl %ebx
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
# fin du traitant et retour de la valeur de retour du syscall
    iret
