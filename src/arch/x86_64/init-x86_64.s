; @author = ngx123
; @brief = code for the initial setup of the x86_64 platform
; @notes = the labels starting from . or having . somewhere in their name(e.g. .hello.world k.lel) are just normal labels
        ; Passing functions arguments(https://stackoverflow.com/questions/37975177/how-can-i-pass-parameters-in-assembler-x86-function-call)
                ; Pass - ```push register/address```, the stack is used to store function arguments, so the first argument pushed on the stack will be the last one that is poped of it(so the last argument in function) and the last argument to be pushed would be the first one to be read
                ; Get the value - ```pop eax, [eax+4]``` As the stack grows downwards and the esp points to the last entry - then 4 bytes more then esp(esp+4) is the entry before last one pushed on the stack. The thing before last one on the stack should be read because last one is the return address of the function(pushed on the stack by "call").(The first argument would be esp+4, second - esp+8, third - esp+16...)
                ; Clean the stack - after the function returns, it is the responsibility of the caller to pop the same amount of things as pushed from the stack to leave it clean


extern kernel_main
global kernel_setup


%ifndef ENABLE_64_BIT_BOOTSTRAP
kernel_setup:
	call kernel_main
	hlt
	jmp $
%else
;;; 32-Bit Bootstrap code ;;;
[BITS 32]


%define ERROR_NO_64     0xE1 ; No 64 bit mode available
%define ERROR_NO_CPUID  0xE2 ; No CPUID available

%define GDT_CODE_SELECTOR 0x8
%define GDT_DATA_SELECTOR 0x10

%define PAGE_SIZE 4096 ; Same as 0x1000


extern _kernel_start
extern _kernel_end


section .text
; @brief = calls all the functions to set up the the long mode and then switch to it
kernel_setup:
	call detect.CPUID
	call detect.LongMode

	call setup.paging
	call setup.compatability64_mode
	jmp  setup.load_gdt

	hlt
	jmp $               			; Loop forever


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
		mov ecx, 0xC0000080							; Set the ECX to 0xC0000080 to specify the register to read from(which is the EFER MSR)
		rdmsr                 						; Read model specific register(into EAX and EDX) specified in ECX
		or eax, 1 << 8             					; Set the long-mode bit(bit 8) in MSR(model specific register)
		wrmsr                      					; Write changed contents(with 64 bit mode ON) of MSR specified in ECX back to MSR(from EAX and EDX)

		mov eax, cr0                				; Load the value from cr0 to eax(needed because cr0 can't be changed directly)
		or eax, 1 << 31             				; Turn on paging(set bit 31)
		mov cr0, eax                				; Write changed contents(with paging on) of cr0 back to cr0

		ret

	; @brief = setups all the tables and loads everything needed for paging, but does not flip the PG bit
	.paging:
		mov eax, cr0								; Load the value from cr0 to eax(needed because cr0 can't be changed directly)
		and eax, 01111111111111111111111111111111b 	; Turn off paging(clear bit 31)
		mov cr0, eax								; Write changed contents(with paging off) of cr0 back to cr0

		; Map the first PML4(L4) entry to PDPT(L3) address(thus giving access to first GB)
		mov eax, l3_table							; Store the address of pdpt(for setting the flags and filling in PML4)
		or eax, 0b11								; Set the present and R/W flags on pdpt table(to have R/W access to the first GB of Virtual Memory - this does not map it, just gives access to it)
		mov [l4_table], eax							; Move the PDPT address with flags to the first entry in PML4

		; Map the first PDPT(L3) entry to PDTs(PL2) address(thus giving access to first to MB inside the first GB), done in the same way as with l3 and l4, just replaced with addresses of l2 and l3
		mov eax, l2_table
		or eax, 0b11								; Set R/W and Present flags
		mov [l3_table], eax

		; Get the size of the kernel(subtract the end address from start address) and depending on that jump to the right page mapping routine
		.get_kernel_size:
			mov eax, _kernel_start
			mov edx, _kernel_end
			sub edx, eax								; Subtract kernel end address from kernel start address to get it's size
			cmp edx, 0x100000							; Compare kernel with 1MB(to check if it is smaller or larger)
			jb .map_2mb									; If the kernel is smaller than 1MB
														; Continue if kernel is larger then 1 MB
			cmp edx, 40000000
			jb .map_1GB									; If the kernel is smaller then 1GB
			ja .map_large_kern							; If the kernel is bigger then 1GB

		; Map the first two megabytes by making the first entry in l2 table point to a 2MB page(set with size flag) that starts at 0x0(which means that the first 2MB in the system would be allocated)
		.map_2mb:
			mov eax, 0x0								; Load 0x0 in eax as it is the start and address in the system and to map first two megabytes they should be mapped starting at 0x0
			or eax, 0b10000011							; Set R/W, Present and Size(This flag makes this particular entry in l2 table point not to an l1 table, but to a 2MB page instead) flags
			mov [l2_table], eax							; Load the address into first l2 entry to map the first 2 megabytes
			jmp .map_continue
		; The same thing as previous but maps 1GB page for cases when kernel is larger then 1MB
		.map_1GB:
			mov eax, 0x0								; Load 0x0 in eax as it is the start and address in the system and to map the first GB it should be mapped from the start of memory - 0x0
			or eax, 0b10000011							; Set R/W, Present and Size(This flag makes this particular entry in l2 table point not to an l1 table, but to a 1GB page instead) flags
			mov [l3_table], eax							; Load the address into frist l3 entry to map first 1GB of RAM
			jmp .map_continue
		; Maps multiple 1 GB pages for kernels that are bigger then 1GB
		.map_large_kern:
			jmp error.hang; DOES NOT YET SUPPORT KERNELS THAT ARE MORE THAN 1GB, ADD SUPPORT AS NEED COMES
			jmp .map_continue
		.map_continue:

		; Set cr3 to address of level 4 table through eax
		mov eax, l4_table
		mov cr3, eax

		; Enable PAE(in cr4)
		mov eax, cr4                 				; Load the cr4 value into eax(becuase cr4 can't be changed directly)
		or eax, 1 << 5               				; Enable PAE(set bit 5)
		mov cr4, eax                 				; Write changed contents(with PAE on) of cr4 back to cr4

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
;; GDT to load in long mode ;;
section .rodata
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
section .text
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
		mov rsi, 0x7FFFFFFFFFFFFFFF	  	; Move the largest 64-Bit number into 64-Bit register(RAX) to test if the switch has happened(otherwsie it just would not fit)
		call kernel_main

		hlt


[BITS 32]
section .bss
;; Page Tables ;;
alignb 4096
l4_table:		; PML4 Table
	resb 4096
l3_table:		; PDP Table
	resb 4096
l2_table:		; PD Table
	resb 4096
%endif