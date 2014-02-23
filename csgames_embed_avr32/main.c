#include "timer0.h"

#include <compiler.h>

#include <board.h>

#include <dip204.h>

#include <power_clocks_lib.h>
#include <gpio.h>
#include <usart.h>
#include <led.h>
#include <delay.h>
#include <adc.h>
#include <spi.h>

#include <print_funcs.h>

#include <avr32/io.h>

void loop(void);
void setup(void);

#  define EXAMPLE_TARGET_PBACLK_FREQ_HZ FOSC0  // PBA clock target frequency, in Hz
void setup() {

	static const gpio_map_t DIP204_SPI_GPIO_MAP = { { DIP204_SPI_SCK_PIN,
			DIP204_SPI_SCK_FUNCTION }, // SPI Clock.
			{ DIP204_SPI_MISO_PIN, DIP204_SPI_MISO_FUNCTION }, // MISO.
			{ DIP204_SPI_MOSI_PIN, DIP204_SPI_MOSI_FUNCTION }, // MOSI.
			{ DIP204_SPI_NPCS_PIN, DIP204_SPI_NPCS_FUNCTION } // Chip Select NPCS.
	};

	///////////////////////////////////
	// Configure Osc0 in crystal mode (i.e. use of an external crystal source, with
	// frequency FOSC0) with an appropriate startup time then switch the main clock
	// source to Osc0.
	pcl_switch_to_osc(PCL_OSC0, FOSC0, OSC0_STARTUP);

	///////////////////////////////////
	// Setup board IO config
	board_init();
	gpio_enable_module_pin(ADC_POTENTIOMETER_PIN, ADC_POTENTIOMETER_FUNCTION);

	///////////////////////////////////
	// Setup micro-controller devices
	INTC_init_interrupts(); // Initialize interrupt vectors.

	// add the spi options driver structure for the LCD DIP204
	spi_options_t spiOptions = { .reg = DIP204_SPI_NPCS, .baudrate = 1000000,
			.bits = 8, .spck_delay = 0, .trans_delay = 0, .stay_act = 1,
			.spi_mode = 0, .modfdis = 1 };

	// Assign I/Os to SPI
	gpio_enable_module(DIP204_SPI_GPIO_MAP,
			sizeof(DIP204_SPI_GPIO_MAP) / sizeof(DIP204_SPI_GPIO_MAP[0]));

	// Initialize as master
	spi_initMaster(DIP204_SPI, &spiOptions);

	// Set selection mode: variable_ps, pcs_decode, delay
	spi_selectionMode(DIP204_SPI, 0, 0, 0);

	// Enable SPI
	spi_enable(DIP204_SPI);

	// setup chip registers
	spi_setupChipReg(DIP204_SPI, &spiOptions, FOSC0);

	init_dbg_rs232_ex(DBG_USART_BAUDRATE, FOSC0);

	///////////////////////////////////
	// Setup clock
	timer0_init(); // timer0 milliseconds clock

	///////////////////////////////////
	// Everything is ready, kick off interrupts
	Enable_global_interrupt();

	// initialize LCD
	dip204_init(backlight_PWM, true);

	// Display default message.
	dip204_set_cursor_position(4, 1);
	dip204_write_string("CS GAMES 2014");
	dip204_set_cursor_position(0, 2);
	dip204_write_string("EMBEDDED PROGRAMMING");
	//dip204_set_cursor_position(6, 3);
	//dip204_write_string("AVR32 UC3");
	dip204_set_cursor_position(0, 4);
	dip204_write_string("TEXT:");
	dip204_hide_cursor();

	print_dbg("Booted up\r\n");
}

static const uint32_t task1_period = 500;
static volatile uint32_t task1_last_run_ms = 0;
static const uint32_t task2_period = 100;
static volatile uint32_t task2_last_run_ms = 0;

void loop() {
	while (1) {
		uint32_t now = timer0_get_ms_time();

		if ((now - task1_last_run_ms) > task1_period) {
			task1_last_run_ms = (now / task1_period) * task1_period;

			LED_Toggle(LED0);
			print_dbg("pop\r\n");
		}

		if ((now - task2_last_run_ms) > task2_period) {
			task2_last_run_ms = (now / task2_period) * task2_period;

			uint32_t pot_value = 0;

			/* enable channel for sensor */
			adc_enable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);
			/* start conversion */
			adc_start(&AVR32_ADC);
			/* get value for sensor */
			pot_value = adc_get_value(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL)
					* 255 / ADC_MAX_VALUE;
			/* Disable channel for sensor */
			adc_disable(&AVR32_ADC, ADC_POTENTIOMETER_CHANNEL);

			print_dbg("pot:");
			print_dbg_ulong(pot_value);

			LED_Set_Intensity(LED4, pot_value);
			LED_Set_Intensity(LED5, 255 - pot_value);

			//////////////////////////////////////

			gpio_enable_pin_glitch_filter(GPIO_PUSH_BUTTON_0);
			gpio_enable_pin_glitch_filter(GPIO_PUSH_BUTTON_1);
			gpio_enable_pin_glitch_filter(GPIO_PUSH_BUTTON_2);
			uint8_t btn0_status = gpio_get_pin_value(GPIO_PUSH_BUTTON_0);
			uint8_t btn1_status = gpio_get_pin_value(GPIO_PUSH_BUTTON_1);
			uint8_t btn2_status = gpio_get_pin_value(GPIO_PUSH_BUTTON_2);
			if (btn0_status == GPIO_PUSH_BUTTON_0_PRESSED)
				LED_On(LED1);
			else
				LED_Off(LED1);
			if (btn1_status == GPIO_PUSH_BUTTON_1_PRESSED)
				LED_On(LED2);
			else
				LED_Off(LED2);
			if (btn2_status == GPIO_PUSH_BUTTON_2_PRESSED)
				LED_On(LED3);
			else
				LED_Off(LED3);

			print_dbg(" btn0:");
			print_dbg_ulong(btn0_status == GPIO_PUSH_BUTTON_0_PRESSED);
			print_dbg(" btn1:");
			print_dbg_ulong(btn1_status == GPIO_PUSH_BUTTON_1_PRESSED);
			print_dbg(" btn2:");
			print_dbg_ulong(btn2_status == GPIO_PUSH_BUTTON_2_PRESSED);
			print_dbg("\r\n");
		}

		// handle received char from console queue
		int rx_char;
		int status = usart_read_char(DBG_USART, &rx_char);
		if (USART_SUCCESS == status) {
			LED_Toggle(LED6);
			print_dbg_char(rx_char); // echo back

			dip204_set_cursor_position(5, 4);
			dip204_write_data(rx_char);
		}

		// flush USART.
		while (!usart_tx_empty(DBG_USART))
			;
	}

	print_dbg("Goodbye.\r\n");
}

int main(void) {
	setup();

	while (1)
		loop();
}
