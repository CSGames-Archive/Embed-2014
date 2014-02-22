/*******************************
 * Projet Cortex  -  Dronolab  *
 * http://dronolab.etsmtl.ca/  *
 *******************************/

#include "timer0.h"

#include <stdint.h>
#include <stdbool.h>

#include "sysclk.h"
#include "board.h"
#include "tc.h"
#include "gpio.h"
#include "intc.h"
#include "print_funcs.h"

// see page 869 UC3A complete documentation
// TC1 = 32Khz Internal Oscillator
// TC2 = fPBA / 2.
// TC3 = fPBA / 8.
// TC4 = fPBA / 32.
// TC5 = fPBA / 128.
#define TIMER_CLOCK_SOURCE TC_CLOCK_SOURCE_TC3

#if TIMER_CLOCK_SOURCE == TC_CLOCK_SOURCE_TC2
#define TIMER_DIVISOR   2
#elif TIMER_CLOCK_SOURCE == TC_CLOCK_SOURCE_TC3
#define TIMER_DIVISOR   8
#elif TIMER_CLOCK_SOURCE == TC_CLOCK_SOURCE_TC4
#define TIMER_DIVISOR  32
#elif TIMER_CLOCK_SOURCE == TC_CLOCK_SOURCE_TC5
#define TIMER_DIVISOR  128
#endif

#define TIMER 			(&AVR32_TC)
#define TIMER_CHANNEL 	(0)

volatile uint32_t timer0_ms = 0;

void timer0_int_handler(void);

void timer0_init(void) {
	// Options for waveform generation.
	static const tc_waveform_opt_t waveform_opt = {
		// Channel selection.
		.channel  = TIMER_CHANNEL,
		// Software trigger effect on TIOB.
		.bswtrg   = TC_EVT_EFFECT_NOOP,
		// External event effect on TIOB.
		.beevt    = TC_EVT_EFFECT_NOOP,
		// RC compare effect on TIOB.
		.bcpc     = TC_EVT_EFFECT_NOOP,
		// RB compare effect on TIOB.
		.bcpb     = TC_EVT_EFFECT_NOOP,
		// Software trigger effect on TIOA.
		.aswtrg   = TC_EVT_EFFECT_NOOP,
		// External event effect on TIOA.
		.aeevt    = TC_EVT_EFFECT_NOOP,
		// RC compare effect on TIOA.
		.acpc     = TC_EVT_EFFECT_NOOP,
		/*
		 * RA compare effect on TIOA.
		 * (other possibilities are none, set and clear).
		 */
		.acpa     = TC_EVT_EFFECT_NOOP,
		/*
		 * Waveform selection: Up mode with automatic trigger(reset)
		 * on RC compare.
		 */
		.wavsel   = TC_WAVEFORM_SEL_UP_MODE_RC_TRIGGER,
		// External event trigger enable.
		.enetrg   = false,
		// External event selection.
		.eevt     = 0,
		// External event edge selection.
		.eevtedg  = TC_SEL_NO_EDGE,
		// Counter disable when RC compare.
		.cpcdis   = false,
		// Counter clock stopped with RC compare.
		.cpcstop  = false,
		// Burst signal selection.
		.burst    = false,
		// Clock inversion.
		.clki     = false,
		// Internal source clock 3, connected to fPBA / 8.
		.tcclks   = TIMER_CLOCK_SOURCE
	};

	// Options for enabling TC interrupts
	static const tc_interrupt_t tc_interrupt = {
		.etrgs = 0,
		.ldrbs = 0,
		.ldras = 0,
		.cpcs  = 1, // Enable interrupt on RC compare alone
		.cpbs  = 0,
		.cpas  = 0,
		.lovrs = 0,
		.covfs = 0
	};

	// Register the RTC interrupt handler to the interrupt controller.
	INTC_register_interrupt(&timer0_int_handler, AVR32_TC_IRQ0, AVR32_INTC_INT0);

	// Initialize the timer/counter.
	tc_init_waveform(TIMER, &waveform_opt);

	/*
	 * Set the compare triggers.
	 * We configure it to count every 1 milliseconds.
	 * We want: (1 / (fPBA / 8)) * RC = 1 ms, hence RC = (fPBA / 8) / 1000
	 * to get an interrupt every 10 ms.
	 */
	tc_write_rc(TIMER, TIMER_CHANNEL, ((FOSC0 / TIMER_DIVISOR) / 1000));
	// configure the timer interrupt
	tc_configure_interrupts(TIMER, TIMER_CHANNEL, &tc_interrupt);
	// Start the timer/counter.
	tc_start(TIMER, TIMER_CHANNEL);

	LED_On(LED0);
}

uint32_t timer0_get_us_time() {
	uint32_t ms = timer0_ms;
	uint32_t us = ms * 1000;
	us += (uint16_t) tc_read_tc(TIMER, TIMER_CHANNEL) * 2;

	if (ms != timer0_ms) { // overflow occured while we were calculating
		ms = timer0_ms;
		us = ms * 1000;
		us += (uint16_t) tc_read_tc(TIMER, TIMER_CHANNEL) * 2;
	}

	return us;
}

uint32_t timer0_get_ms_time() {
	return timer0_ms;
}

__attribute__((__interrupt__))
void timer0_int_handler() {
	tc_read_sr(TIMER, TIMER_CHANNEL);
	timer0_ms++;
}
