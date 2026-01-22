

#ifndef RTC_H_
#define RTC_H_

	#ifndef RTC_CLOCK

		/**
		 * @def SYSTICK_CLOCK
		 * @brief Defines the systick main clock source for the RTC.
		 *
		 * @details
		 * This macro selects the active system clock source from the available options in the AVR ``CLKCTRL`` module.
		 *
		 * Possible values:
		 * - `RTC_CLKSEL_INT32K_gc` : Internal 32.768 kHz ultra low power oscillator  
		 * - `RTC_CLKSEL_INT1K_gc`  : Internal 1.024 kHz ultra low power oscillator  
		 * - `RTC_CLKSEL_EXTCLK_gc` : External clock source  
		 *
		 * By default, `SYSTICK_CLOCK` is set to `CLKCTRL_CLKSEL_OSCULP32K_gc` (20 MHz internal oscillator).
		 *
		 * @note The systick_update() function has to be called in an ISR at the project main location. Otherwise the systick won´t work.
		 */
		#define RTC_CLOCK RTC_CLKSEL_INT32K_gc
	
	#endif
	
	#ifndef RTC_PRESCALER
		#define RTC_PRESCALER RTC_PRESCALER_DIV1_gc
	#endif

	#include <avr/io.h>

	void rtc_init(void);

#endif /* RTC_H_ */