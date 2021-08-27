[bits 16]
[org 0x7c00]

REAL_MODE_STACK equ 0x9000
PROT_MODE_STACK equ 0x90000

KERNEL_OFFSET   equ 0x1000 ; The same one we used when linking the kernel
KERNEL_NUM_SECT equ 32     ; each sector has 512 bytes

stage1_start:
    mov [BOOT_DRIVE], dl   ; BIOS sets boot drive in 'dl' on boot, save it
    mov bp, REAL_MODE_STACK
    mov sp, bp

    mov bx, MSG_REAL_MODE
    call print
    call print_nl

    call read_mmap
    jc read_mmap_err

    call load_kernel       ; read the kernel from disk
    call switch_to_pm
    jmp $                  ; Never executed

switch_to_pm:
    cli                    ; 1. disable interrupts
    lgdt [gdt_descriptor]  ; 2. load the GDT descriptor
    mov eax, cr0
    or eax, 0x1            ; 3. set 32-bit mode bit in cr0
    mov cr0, eax
    jmp CODE_SEG:init_pm   ; 4. far jump by using a different segment

load_kernel:
    mov bx, MSG_LOAD_KERNEL
    call print
    call print_nl

    mov bx, KERNEL_OFFSET  ; Read from disk and store at KERNEL_OFFSET
    ; FIXME: auto-detect end of os-image.bin
    mov dh, KERNEL_NUM_SECT
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

read_mmap_err:
    halt

%include "boot/memory_map.asm"
%include "boot/print.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"

[bits 32]
%include "boot/32bit_print.asm"

init_pm:                   ; we are now using 32-bit instructions
    mov ax, DATA_SEG       ; 5. update the segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, PROT_MODE_STACK ; 6. update the stack right at the top of the free space
    mov esp, ebp

    call begin_pm          ; 7. Call a well-known label with useful code

begin_pm:
    mov ebx, MSG_PROT_MODE
    call print_string_pm
    call KERNEL_OFFSET     ; Give control to the kernel
    jmp $                  ; Stay here when the kernel returns control to us (if ever)

BOOT_DRIVE      db 0
MSG_REAL_MODE   db "16-bit", 0
MSG_PROT_MODE   db "32-bit", 0
MSG_LOAD_KERNEL db "Load Kernel", 0

; padding
times 510 - ($-$$) db 0
dw 0xaa55
