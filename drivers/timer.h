#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_init(void);
int64_t timer_get_elapsed(void);

#endif /* TIMER_H */
