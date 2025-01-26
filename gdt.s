global segments_load_gdt
global segments_load_registers
segments_load_gdt:
	lgdt [esp + 4]
segments_load_registers:
	mov ax, 0x10
	mov ds, ax 
	mov es, ax
	mov fs, ax
	mov gs, ax
    jmp 0x08:flush
flush:
	mov byte [0xb8000],'a'
	mov byte [0xb8001],0x0f
    ret

   