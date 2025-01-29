bits 32
mov byte [0xb8000],'b'
mov byte [0xb8001],0x0f
.halt:
    jmp .halt