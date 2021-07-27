#include "../cpu/isr.h"
#include "timer.h"
#include <stdint.h>

static int64_t timer_sec = 0;

static
void timer_irq_handler(registers_t regs) {
    timer_sec++;
}

void timer_init(void) {
    register_irq_handler(ISR_NUM_TIMER, timer_irq_handler);
}

int64_t timer_get_elapsed(void) {
    return timer_sec;
}
