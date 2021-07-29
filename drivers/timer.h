#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define PIT_CHL0_DATA_REG 0x40
#define PIT_CHL1_DATA_REG 0x41
#define PIT_CHL2_DATA_REG 0x42
#define PIT_CMD_REG       0x43

#define PIT_CMD_REG_CHL0_SEL        (0 << 6)
#define PIT_CMD_REG_CHL1_SEL        (1 << 6)
#define PIT_CMD_REG_CHL2_SEL        (2 << 6)
#define PIT_CMD_REG_READBACK_CMD    (3 << 6)
#define PIT_CMD_REG_LATCH_CMD       (0 << 4)
#define PIT_CMD_REG_LOBYTE_ACCESS   (1 << 4)
#define PIT_CMD_REG_HIBYTE_ACCESS   (2 << 4)
#define PIT_CMD_REG_LOHIBYTE_ACCESS (3 << 4)
#define PIT_CMD_REG_MODE0           (0 << 1) /* interrupt on terminal count */
#define PIT_CMD_REG_MODE1           (1 << 1) /* hardware re-triggerable one-shot */
#define PIT_CMD_REG_MODE2           (2 << 1) /* rate generator */
#define PIT_CMD_REG_MODE3           (3 << 1) /* square wave generator */
#define PIT_CMD_REG_MODE4           (4 << 1) /* software triggered strobe */
#define PIT_CMD_REG_MODE5           (5 << 1) /* hardware triggered strobe */
#define PIT_CMD_REG_BINARY_MODE     (0 << 0)
#define PIT_CMD_REG_BCD_MODE        (1 << 0)

#define PIT_FREQ_HZ                 1193182
#define PIT_FREQ_DIV_1_KHZ          1193

void timer_init(void);
int64_t timer_get_elapsed_ms(void);
int64_t timer_get_elapsed_s(void);

#endif /* TIMER_H */
