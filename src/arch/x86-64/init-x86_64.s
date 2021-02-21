; @author = ngx123
; @brief = code for the initial setup of the x86-64 platform
; @notes = the labels starting from . or having . somewhere in their name(e.g. .hello.world k.lel) are just normal labels
        ; Passing functions arguments(https://stackoverflow.com/questions/37975177/how-can-i-pass-parameters-in-assembler-x86-function-call)
                ; Pass - ```push register/address```, the stack is used to store function arguments, so the first argument pushed on the stack will be the last one that is poped of it(so the last argument in function) and the last argument to be pushed would be the first one to be read
                ; Get the value - ```pop eax, [eax+4]``` As the stack grows downwards and the esp points to the last entry - then 4 bytes more then esp(esp+4) is the entry before last one pushed on the stack. The thing before last one on the stack should be read because last one is the return address of the function(pushed on the stack by "call").(The first argument would be esp+4, second - esp+8, third - esp+16...)
                ; Clean the stack - after the function returns, it is the responsibility of the caller to pop the same amount of things as pushed from the stack to leave it clean


;;; 32-Bit Bootstrap code ;;;
[BITS 32]
%include "boot/multiboot2/multiboot2_bootstrap.s"


%define ERROR_NO_64     0xE1 ; No 64 bit mode available
%define ERROR_NO_CPUID  0xE2 ; No CPUID available

%define GDT_CODE_SELECTOR 0x8
%define GDT_DATA_SELECTOR 0x10

%define PAGE_SIZE 4096 ; Same as 0x1000


global gdtLoadAsm


; @brief = calls all the functions to set up the the long mode and then switch to it
kernel_setup:
	call detect.CPUID
	call detect.LongMode

	call setup.paging
	call setup.compatability64_mode
	jmp  setup.load_gdt

	hlt
	jmp $               ; Loop forever


;; Functions ;;
detect:
	; @brief = Check if the CPUID is supported by CPU - it is supported if ID bit in FLAGS register can be flipped
	.CPUID:
		pushfd						; Push FLAGS register contents on to the stack
		pop eax						; Pop value from the stack into EAX to get the FLAGS register contents into EAX(value in EAX will be used to set needed bit)

		mov ecx, eax				; Copy FLAGS register contents to ECX for temporary storage(for later comparasion)

		xor eax, 1 << 21			; Flip ID bit in FLAGS register contents that were poped into EAX

		push eax					; Push the changed FLAGS register contents back on to the stack
		popfd						; Fill the FLAGS register contents with data in value poped of the stack(put the changed contents into the FLAGS)

		pushfd						; Push FLAGS register contents on to the stack
		pop eax						; Pop value from the stack into EAX to get the FLAGS register contents into EAX(get the value to check if the ID bit change stayed - if it did CPUID is supported)

		push ecx					; Push the original FLAGS register contents back on to the stack
		popfd						; Fill the FLAGS register contents with data in value poped of the stack(restore the original value of the register(the one that was stored in ECX))

		xor eax, ecx				; XOR EAX(value of FLAGS after being changed written to FLAGS and back to EAX again) and ECX(original value of FLAGS) together to check if the bit is set
		jz error.noCPUID			; If the zero bit is set by xor jump to error, if zero bit is set by xor it means that the output of XOR operation on two values was zero, which means they are the same, which in turn means bit was not set and CPUID is not available

		ret

	; @brief = Check if the CPU supports 64-bit/long mode using CPUID
	.LongMode:
		mov eax, 0x80000000			; The value is loaded into EAX which would cause CPUID instruction to put value higher then that if the extended functions are available
		cpuid						; Use the CPUID instruction to get the information based on value in EAX
		cmp eax, 0x80000001			; Compare EAX and value indication extended functions
		jb error.noLongMode         ; If value in EAX is less then value needed for extended functions, no long mode is available(becuase it's presence can't be checked without extended functions)

		mov eax, 0x80000001			; Move the Extended functions value into EAX for CPUID to give needed flags(extended function)
		cpuid						; Get the data into EAX
		test edx, 1 << 29      		; Test if the Long Mode bit is set, which is bit 29, is set in the EDX(the CPUID puts the required flags into EDX)
		jz error.noLongMode         ; If the long mode bit is not set(compared values are not equal), there is no long mode so jump to error

		ret

setup:
	; @brief = switched to compatibility mode by switching on 64-Bit mode bit and enabling paging after it
	.compatability64_mode:
		mov ecx, 0xC0000080          ; Set the C-register to 0xC0000080, which is the EFER MSR.
		rdmsr                        ; Read from the model-specific register.
		or eax, 1 << 8               ; Set the LM-bit which is the 9th bit (bit 8).
		wrmsr                        ; Write to the model-specific register.

		mov eax, cr0                 ; Set the A-register to control register 0.
		or eax, 1 << 31              ; Set the PG-bit, which is the 32nd bit (bit 31).
		mov cr0, eax                 ; Set control register 0 to the A-register
		ret

	; @brief = setups all the tables and loads everything needed for paging, but does not flip the PG bit
	.paging:
		; Disable Paging by getting cr0 value into eax, changing it to one with paging off and returning
		mov eax, cr0
		and eax, 01111111111111111111111111111111b ; Paging is bit 31, so this bit should be turned off
		mov cr0, eax

		mov edi, PAGE_SIZE    ; Set the destination index to 0x1000.
		mov cr3, edi       ; Set control register 3 to the destination index.
		xor eax, eax       ; Nullify the A-register.
		mov ecx, PAGE_SIZE      ; Set the C-register to 4096.
		rep stosd          ; Clear the memory.
		mov edi, cr3       ; Set the destination index to control register 3.

		mov DWORD [edi], 0x2003      ; Set the uint32_t at the destination index to 0x2003.
		add edi, PAGE_SIZE              ; Add 0x1000 to the destination index.
		mov DWORD [edi], 0x3003      ; Set the uint32_t at the destination index to 0x3003.
		add edi, PAGE_SIZE              ; Add 0x1000 to the destination index.
		mov DWORD [edi], 0x4003      ; Set the uint32_t at the destination index to 0x4003.
		add edi, PAGE_SIZE              ; Add 0x1000 to the destination index.

		mov ebx, 0x00000003          ; Set the B-register to 0x00000003.
		mov ecx, 512                 ; Set the C-register to 512.

		.SetEntry:
			mov DWORD [edi], ebx         ; Set the uint32_t at the destination index to the B-register.
			add ebx, PAGE_SIZE              ; Add 0x1000 to the B-register.
			add edi, 8                   ; Add eight to the destination index.
			loop .SetEntry               ; Set the next entry.

		mov eax, cr4                 ; Set the A-register to control register 4.
		or eax, 1 << 5               ; Set the PAE-bit, which is the 6th bit (bit 5).
		mov cr4, eax                 ; Set control register 4 to the A-register.
		ret

	; @brief = load the GDT and flush it into CPU with long jump
	.load_gdt:
		lgdt[GDT64descriptor]
		jmp GDT_CODE_SELECTOR:longMode.flush64 ; Long jump(using an offset in GDT) to 64-Bit code after loading GDT

error:
	; Routines to push error code on the stack and call the function to output error code
	.noCPUID:
		push ERROR_NO_CPUID
		call error.output
	.noLongMode:
		push ERROR_NO_64
		call error.output
	.hang:
		cli
		hlt
		jmp error.hang

	; @brief = Outputs the error code to the eax register
	.output:
		mov eax, [esp+4]
		jmp error.hang


;; Data ;;
GDT64:
	.Null:
		dw 0xFFFF		; Limit(low)
		dw 0			; Base(low)
		db 0			; Base(middle)
		db 0			; Access
		db 1			; Granularity
		db 0			; Base(high)
	.Code:
		dw 0			; Limit(low)
		dw 0			; Base(low)
		db 0			; Base(middle)
		db 10011010b	; Access
		db 10101111b	; Granularity
		db 0			; Base(high)
	.Data:
		dw 0			; Limit(low)
		dw 0			; Base(low)
		db 0			; Base(middle)
		db 10010010b	; Access
		db 0			; Granularity
		db 0			; Base(high)
	.End:

GDT64descriptor:
	.Limit: dw GDT64.End - GDT64 - 1
	.Base:	dq GDT64



;;; Bootstrapped 64-Bit Code ;;;
[BITS 64]
longMode:
	; @brief = flush all the registers and turn off the interrupts once in 64-Bit Mode
	.flush64:
		cli
		mov ax, GDT_DATA_SELECTOR

		; Flush all registers with data selector offset(using AX)
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		mov ss, ax
		jmp longMode.start64

	; @brief = start of full long mode 64-Bit code execution
	.start64:
		mov rax, 0x7FFFFFFFFFFFFFFF	  ; Move the largest 64-Bit number into 64-Bit register(RAX) to test if the switch has happened(otherwsie it just would not fit)
		mov rax, $
		hlt
