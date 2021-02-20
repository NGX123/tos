; @author = ngx123
; @brief = code that takes over if multiboot bootloader is used


; Kernel start called by grub(_start is specified in linker.ld)
global _start

extern kernel_main
extern interpretMultiboot2


%ifndef USE_MULTIBOOT1_PROTOCOL
; If problems occur return to equ instead of %define
%define MAGIC                               0xE85250D6      ; Magic number used by GRUB to detect the executable is multiboot2 compatible
%define ARCH_x86_32                         0               ; Architecture that is used by executbale, 0 - i386
%define MULTIBOOT2_HEADER_TAG_FRAMEBUFFER   0

%define MULTIBOOT2_HEADER_TAG_END           0               ; Indicates the end of tags section in multiboot2 header
%define MULTIBOOT2_HEADER_TAG_OPTIONAL      1


section .multiboot          ; Has data located that will be used by grub to find the executable
multiboot_header_start:
    align 8                                                                         ; The header must be 8 bytes(64 bit) aligned
    dd  MAGIC
    dd  ARCH_x86_32
    dd  multiboot_header_end - multiboot_header_start                               ; Size of the header
    dd  -(MAGIC + ARCH_x86_32 + (multiboot_header_end - multiboot_header_start))    ; Checksum
%ifdef ENABLE_MULTIBOOT2_FRAMEBUFFER
    align 8                                                                         ; Tags must start at 8 byte aligned address
    framebuffer_tag_start:
        dw 5
        dw MULTIBOOT2_HEADER_TAG_OPTIONAL
        dd framebuffer_tag_end - framebuffer_tag_start
        dd 1024
        dd 768
        dd 32
    framebuffer_tag_end:
%endif
    align 8
    dw MULTIBOOT2_HEADER_TAG_END                                                    ; The tag which indicates end of tag section is type - 0
    dw 0                                                                            ; with flags - 0
    dd 8                                                                            ; and size - 8
multiboot_header_end:

%elifdef USE_MULTIBOOT1_PROTOCOL

MULTIBOOT 1 HEADER - may be used if something goes very wrong. Replace multiboot 2 header code with this
MBALIGN         equ 1 << 0              ; Loaded modules page boundaries aligning flag
MEMINFO         equ 1 << 1              ; Memory map flag
FLAGS           equ MBALIGN | MEMINFO   ; GRUB boot flags combination
MAGIC           equ 0x1BADB002          ; Value for grub detection
CHECKSUM        equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot


section .multiboot              ; GRUB section, includes writing special data at start of binary for grub to see the OS
align 4
    dd MAGIC
dd FLAGS
dd CHECKSUM
%endif


section .bss
align 16                        ; Aligning is required for stack
    stack_bottom:
            resb 16384      ; Allocate 16 kilobytes for stack
    stack_top:


section .text
_start:
    mov esp, stack_top      ; Load stack address into stack register(initialize)

    push ebx                ; ebx - holds Multiboot2 information struct address
    push eax                ; eax - holds Multiboot2 Bootloader Checksum
    call interpretMultiboot2

    call kernel_setup       ; Switch to the init code of the platform


hang:
    hlt
    jmp hang               ; Loop forever
