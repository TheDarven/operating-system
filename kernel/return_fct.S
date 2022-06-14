.globl return_fct
# On passe le registre contenant la valeur de retour en paramètre de exit()
return_fct:
    pushl %eax
    call exit
