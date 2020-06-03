; In 32 bit mode we have memory segmentation as in 16 bit mode, but it works in a completely
;   different way

; When the CPU is switched into 32-bit mode, the proccess of translation of the address into
;   physical address changes - now segment becomes an index to a segment descriptor in
;   GDT(Global Descriptor Table).

; Segment descriptor - 8-byte structure that defines:
    ; Base address(32 bits) - defines the begining of the memory segment
    ; Segment Limit(20 bits) - defines the size of the segment
    ; CPU flags - affect how cpu interprets segment, e.g. read-only

; Basic flat model - simplest configuration of segment registers which covers full 4gb, 
;   there is two overlaping segments here - one for code and one for data
    ; Code segment configuration
        ; Base - 0x0
        ; Limit - 0xfffff
        ; Present - 1, seince present in memory - used for virtual memory
        ; Privilage - 0, rong 0 is the highest privilage
        ; Descriptor table - 1 for code or data segment, 0 for traps
        ; Type:
            ; Code - 1 for code, since this is code segment
            ; Conforming - 0, not conforming means code in segment with lower privilage 
                ;may not call code in this segment - memory protection
            ; Readable - 1 readable(read constants defined in code), 0 execute-only
            ; Accesed - 0 used in debugging and virtual memory techniques
        ; Other flags:
            ; ...

    ; Data segment configuration
        ; Identical excpt flags
        ; Type
            ; Code - 0 for data
            ; Expand down - allows segment to expand down
            ; Writable - 1, allows segment to be writable
            ; Accessed - 0, used in debugiing and virtual memory



; Flat model in assembly using db and dw to put aprobriate bytes in segment descriptor
        
; GDT 
gdt_start:

gdt_null: ; null descriptor
    dd 0x0  ; define double word - 4 bytes
    dd 0x0  

gdt_code:  ; the code segment descriptor
    ; base=0x0, limit=0xfffff
    ; 1st flags : ( present )1 ( privilege )00 ( descriptor type )1 -> 1001 b
    ; type flags : ( code )1 ( conforming )0 ( readable )1 ( accessed )0 -> 1010 b
    ; 2nd flags : ( granularity )1 (32 - bit default )1 (64 - bit seg )0 ( AVL )0 -> 1100 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10011010b    ; Type flags - b in the end means it is direct binary
    db 11001111b    ; 2nd flags Limit(bits 16-19)
    db 0x0          ; Base (bits 24-31) 

gdt_data: ; data segment descriptor
    ; Same code except for type flags
    ; type flags : ( code )0 ( expand down )0 ( writable )1 ( accessed )0 -> 0010 b
    dw 0xffff       ; Limit (bits 0-15)
    dw 0x0          ; Base (bits 0-15)
    db 0x0          ; Base (bits 16-23)
    db 10010010b    ; 1st flags , type flags
    db 11001111b    ; 2nd flags Limit(bits 16-19)
    db 0x0          ; Base (bits 24-31) 

gdt_end: ; we have a label at the end so we can calculate size of GDT for GDT descriptor table


; GDT descriptor
gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT, always less one of the true size
    dd gdt_start                ; Start of our GDT table

; Define some handy constants for the GDT segment descriptor offsets , which
; are what segment registers must contain when in protected mode. For example ,
; when we set DS = 0 x10 in PM , the CPU knows that we mean it to use the
; segment described at offset 0 x10 ( i.e. 16 bytes ) in our GDT , which in our
; case is the DATA segment (0 x0 -> NULL ; 0x08 -> CODE ; 0 x10 -> DATA )
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start