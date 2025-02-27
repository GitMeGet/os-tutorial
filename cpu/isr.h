#ifndef ISR_H
#define ISR_H

#include <stdint.h>

#define ISR_NUM_TIMER       32
#define ISR_NUM_KEYBOARD    33

/* ISRs reserved for CPU exceptions */
extern void exception0();
extern void exception1();
extern void exception2();
extern void exception3();
extern void exception4();
extern void exception5();
extern void exception6();
extern void exception7();
extern void exception8();
extern void exception9();
extern void exception10();
extern void exception11();
extern void exception12();
extern void exception13();
extern void exception14();
extern void exception15();
extern void exception16();
extern void exception17();
extern void exception18();
extern void exception19();
extern void exception20();
extern void exception21();
extern void exception22();
extern void exception23();
extern void exception24();
extern void exception25();
extern void exception26();
extern void exception27();
extern void exception28();
extern void exception29();
extern void exception30();
extern void exception31();

/* ISRs reserved for hardware interrupts */
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

/* Describes function args */
typedef struct {
    /* Data segment selector */
    uint32_t ds;
    /* Pushed by pusha. */
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    /* Interrupt number and error code (if applicable) */
    uint32_t int_no, err_code;
    /* Pushed by the processor automatically */
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

void isr_install(void);
void remap_pic(uint8_t master_offset, uint8_t slave_offset);
void exception_handler(registers_t r);

/* Function ptr isr_t, args: registers_t, return void */
typedef void (*isr_t)(registers_t);
void register_irq_handler(uint8_t n, isr_t handler);
void common_irq_handler(registers_t r);

#endif /* ISR_H */
