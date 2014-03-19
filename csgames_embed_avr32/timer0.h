/*! Timer0
 Timer qui sert de base de temps au systeme a l'aide de get_us_time()
 Timer overflows at 1ms + time from counter directly (1 tick = 0.5us)
 */

#ifndef TIMER0_H_
#define TIMER0_H_

#include <stdint.h>

void timer0_init(void);

/*! Get system time in [us] */
uint32_t timer0_get_us_time(void);

/*! Get system time in [ms] */
uint32_t timer0_get_ms_time(void);

#endif /* TIMER0_H_ */

