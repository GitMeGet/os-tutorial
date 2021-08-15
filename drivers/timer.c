#include "../cpu/isr.h"
#include "ports.h"
#include "timer.h"
#include <stdint.h>

static int64_t timer_ms = 0;

static
void timer_irq_handler(registers_t regs) {
    timer_ms++;
}

static
void timer_set_irq_freq(void) {
    port_byte_out(PIT_CMD_REG, PIT_CMD_REG_CHL0_SEL |
                               PIT_CMD_REG_LOHIBYTE_ACCESS |
                               PIT_CMD_REG_MODE2);
    port_byte_out(PIT_CHL0_DATA_REG, PIT_FREQ_DIV_1_KHZ & 0xff);
    port_byte_out(PIT_CHL0_DATA_REG, (PIT_FREQ_DIV_1_KHZ >> 8));

}

void timer_init(void) {
    timer_set_irq_freq();
    register_irq_handler(ISR_NUM_TIMER, timer_irq_handler);
}

int64_t timer_get_elapsed_ms(void) {
    return timer_ms;
}

int64_t timer_get_elapsed_s(void) {
    /* TODO: implement divide */
//    return timer_ms / 1000;
}

void timer_sleep_ms(int64_t sleep_ms) {
    int64_t start_time = timer_ms;
    while (timer_ms - start_time < sleep_ms);
}

