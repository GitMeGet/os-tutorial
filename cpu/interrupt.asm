; Defined in isr.c
[extern exception_handler]
[extern common_irq_handler]

; Common ISR code
exception_common_stub:
    ; 1. Save CPU state
    pusha ; Pushes edi,esi,ebp,esp,ebx,edx,ecx,eax
    mov ax, ds ; Lower 16-bits of eax = ds.
    push eax ; save the data segment descriptor
    mov ax, 0x10  ; kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; 2. Call C handler
    call exception_handler
    ; 3. Restore state
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    add esp, 8 ; Cleans up the pushed error code and pushed ISR number
    sti
    iret ; pops 5 things at once: CS, EIP, EFLAGS, SS, and ESP

irq_common_stub:
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ; 2. Call C handler
    call common_irq_handler
    ; 3. Restore state
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    popa
    add esp, 8
    sti
    iret

%macro EXCEPTION_NOERRCODE 1  ; define a macro, taking one parameter
  [GLOBAL exception%1]        ; %1 accesses the first parameter.
  exception%1:
    cli
    push byte 0
    push byte %1
    jmp exception_common_stub
%endmacro

%macro EXCEPTION_ERRCODE 1
  [GLOBAL exception%1]
  exception%1:
    cli
    push byte %1
    jmp exception_common_stub
%endmacro

%macro IRQ 1
  [GLOBAL irq%1]
  irq%1:
    cli
    push byte %1
    push byte %1 + 32
    jmp irq_common_stub
%endmacro

EXCEPTION_NOERRCODE 0
EXCEPTION_NOERRCODE 1
EXCEPTION_NOERRCODE 2
EXCEPTION_NOERRCODE 3
EXCEPTION_NOERRCODE 4
EXCEPTION_NOERRCODE 5
EXCEPTION_NOERRCODE 6
EXCEPTION_NOERRCODE 7
EXCEPTION_ERRCODE 8
EXCEPTION_NOERRCODE 9
EXCEPTION_ERRCODE 10
EXCEPTION_ERRCODE 11
EXCEPTION_ERRCODE 12
EXCEPTION_ERRCODE 13
EXCEPTION_ERRCODE 14
EXCEPTION_NOERRCODE 15
EXCEPTION_NOERRCODE 16
EXCEPTION_NOERRCODE 17
EXCEPTION_NOERRCODE 18
EXCEPTION_NOERRCODE 19
EXCEPTION_NOERRCODE 20
EXCEPTION_NOERRCODE 21
EXCEPTION_NOERRCODE 22
EXCEPTION_NOERRCODE 23
EXCEPTION_NOERRCODE 24
EXCEPTION_NOERRCODE 25
EXCEPTION_NOERRCODE 26
EXCEPTION_NOERRCODE 27
EXCEPTION_NOERRCODE 28
EXCEPTION_NOERRCODE 29
EXCEPTION_NOERRCODE 30
EXCEPTION_NOERRCODE 31

IRQ                 0
IRQ                 1
IRQ                 2
IRQ                 3
IRQ                 4
IRQ                 5
IRQ                 6
IRQ                 7
IRQ                 8
IRQ                 9
IRQ                 10
IRQ                 11
IRQ                 12
IRQ                 13
IRQ                 14
IRQ                 15
