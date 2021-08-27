; use the INT 0x15, eax = 0xE820 BIOS function to get a memory map
; inputs: es:di -> destination buffer for 24 byte entries
; outputs: bp = entry count, trashes all registers except esi

NUM_ENTRIES          equ 0x7E00 ; the number of entries will be stored at 0x8000
STRUCT_BASE          equ 0x7E04
SMAP                 equ 0x0534D4150
CMD_MMAP_GET         equ 0xe820

read_mmap:
    pusha
    
    mov di, STRUCT_BASE ; set di, default might overwrite BIOS code
	xor ebx, ebx	 ; set ebx to 0
	xor bp, bp		 ; keep an entry count in bp (set to 0)
	mov edx, SMAP    ; place "SMAP" into edx
	mov eax, CMD_MMAP_GET
	mov [es:di + 20], dword 1 ; set 6th uint32_t to ensure valid ACPI 3.X entry
	mov ecx, 24      ; ask for 24 bytes
	int 0x15
	jc short .fail   ; carry set on first call -> "unsupported function"
	mov edx, SMAP    ; Some BIOSes apparently trash this register?
	cmp eax, edx     ; on success, eax must have been reset to "SMAP"
	jne short .fail
	test ebx, ebx    ; ebx = 0 implies list is only 1 entry long (worthless)
	je short .fail
	jmp short .check_ACPI
.loop:
	mov eax, CMD_MMAP_GET ; eax, ecx get trashed on every int 0x15 call
	mov [es:di + 20], dword 1 ; set 6th uint32_t to ensure valid ACPI 3.X entry
	mov ecx, 24	     ; ask for 24 bytes again
	int 0x15
	jc short .done   ; carry set -> "end of list already reached"
	mov edx, SMAP    ; repair potentially trashed register
.check_ACPI:
	jcxz .skip_entry ; skip any 0 length entries
	cmp cl, 20       ; got a 24-byte ACPI 3.X response?
	jbe short .check_length
	test byte [es:di + 20], 1 ; if so: is the "ignore this data" bit clear?
	je short .skip_entry
.check_length:
	mov ecx, [es:di + 8] ; get lower uint32_t of memory region length
	or ecx, [es:di + 12] ; "or" it with upper uint32_t to test for zero
	jz .skip_entry   ; if length uint64_t is 0, skip entry
	inc bp           ; got a good entry: count++
	add di, 24       ; increment entry pointer
.skip_entry:
	test ebx, ebx    ; if ebx is 0, list is complete
	jne short .loop
.done:
	mov [NUM_ENTRIES], bp ; store the entry count
	clc			     ; carry bit set on end of list, clear it
    popa
    ret
.fail:
	stc              ; "function unsupported" error exit
    popa
    ret
