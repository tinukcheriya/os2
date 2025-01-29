    global loader                  
    extern kmain                    
    
    KERNEL_STACK_SIZE equ 4096
    MODULEALIGN equ 1<<0
    MEMINFO equ 1<<1

    			
    MAGIC_NUMBER equ 0x1BADB002    
    FLAGS        equ MODULEALIGN|MEMINFO   
    ;ALIGN_MODULES equ 0x00000001         
    CHECKSUM     equ -(MAGIC_NUMBER+FLAGS)
                                    
       
    extern kernelend
    section .bss
    align 4                      
    kernel_stack:                   
        resb KERNEL_STACK_SIZE      
    
        
    section .text                 
    align 4                   
        dd MAGIC_NUMBER 
        dd FLAGS          
        ;dd ALIGN_MODULES                   
        dd CHECKSUM   

            


    loader:
        mov esp, kernel_stack + KERNEL_STACK_SIZE 
        push $kernelend                               
        call kmain           	   
    .loop:
        jmp .loop              