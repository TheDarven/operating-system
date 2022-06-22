    
.globl fct_syscall

fct_syscall:
    # Sauvegarde des registres user
    pushl %ebp
    movl %esp, %ebp
    pushl %esi
    pushl %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
    # Passage des paramètres dans les registres
    movl 8(%ebp), %eax
    movl 12(%ebp), %ebx
    movl 16(%ebp), %ecx
    movl 20(%ebp), %edx
    movl 24(%ebp), %edi
    movl 28(%ebp), %esi
    # Trigger de l'interruption
    int $49
    # Restauration des registres
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    popl %ebp
    ret