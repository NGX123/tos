; To access certain hardware or other parts of the system interrupts are used 

; First we choose the function of the interrupt by puting it's hex into first byte of register
mov ah, 0x0e

; Then we could put everything that is needed for interrupt into register
mov al, 'H'

; Then we start the interrupt, int <hex>
int 0x10