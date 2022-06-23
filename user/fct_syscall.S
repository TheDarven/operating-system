    
.globl fct_syscall

fct_syscall:
    # Sauvegarde des registres user
    pushl %edi
    pushl %esi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    # Passage des paramètres dans les registres
    movl 28(%esp), %eax
    movl 32(%esp), %ebx
    movl 36(%esp), %ecx
    movl 40(%esp), %edx
    movl 44(%esp), %esi
    movl 48(%esp), %edi
    # Trigger de l'interruption
    int $49
    # Restauration des registres
    popl %ebx
    popl %ebx
    popl %ecx
    popl %edx
    popl %esi
    popl %edi
    ret