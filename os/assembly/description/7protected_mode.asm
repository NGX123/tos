; Switch to protected mode
switch_to_pm:
    cli ; switch off all interrupts until we have protected mode intrrrupt vector
    lgdt [gdt_descriptor]   ; Load out global descriptor table 

    mov eax, cr0            ; To switch to PM we set first bit of cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

init_pm:    ; initialiseregisters and stack once in PM
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax 
    mov gs, ax

    mov epb, 0x90000    ; update stuck position right at top of free space
    mov esp, ebp

    call BEGIN_PM       ; call well known label