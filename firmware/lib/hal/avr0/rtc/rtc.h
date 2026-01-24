/**
 * @file rtc.h
 * @brief Header file with declarations and macros for the RTC peripheral.
 *
 * This file provides configuration macros and the function prototype for initializing the real-time clock (RTC) on AVR0 devices. It allows selecting the clock source, prescaler, and overflow value used by the RTC.
 *
 * @author g.raf
 * @date 2026-01-24
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2026 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 *
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 *
 * @see https://github.com/0x007e/hal-avr0-rtc "AVR0 GitHub Repository"
 */

#ifndef RTC_H_
#define RTC_H_

	#ifndef RTC_CLOCK
		/**
		 * @def RTC_CLOCK
		 * @brief Defines the main clock source for the RTC.
		 *
		 * @details
		 * This macro selects the active system clock source for the real-time clock of the avr0-system.
		 *
		 * Possible values:
		 * - `RTC_CLKSEL_INT32K_gc` : Internal 32.768 kHz ultra low power oscillator  
		 * - `RTC_CLKSEL_INT1K_gc`  : Internal 1.024 kHz ultra low power oscillator  
		 * - `RTC_CLKSEL_EXTCLK_gc` : External clock source  
		 *
		 * By default, `RTC_CLOCK` is set to `CLKCTRL_CLKSEL_OSCULP32K_gc` (32.768 kHz internal oscillator).
		 *
		 * @note The RTC interrupt service routine (ISR) for `RTC_CNT_vect` must be implemented and enabled in the application, otherwise the system will stop working.
		 */
		#define RTC_CLOCK RTC_CLKSEL_INT32K_gc
	#endif
	
	#ifndef RTC_PRESCALER
		/**
		 * @def RTC_PRESCALER
		 * @brief Defines the prescaler for the RTC clock.
		 *
		 * @details
		 * This macro selects the clock prescaler for the real-time clock (RTC) peripheral on AVR0 devices. It divides the selected RTC clock source to achieve the desired tick frequency.
		 *
		 * Possible values:
		 * - `RTC_PRESCALER_DIV1_gc`      : No prescaling (division by 1)
		 * - `RTC_PRESCALER_DIV2_gc`      : Division by 2
		 * - `RTC_PRESCALER_DIV4_gc`      : Division by 4
		 * - `RTC_PRESCALER_DIV8_gc`      : Division by 8
		 * - `RTC_PRESCALER_DIV16_gc`     : Division by 16
		 * - `RTC_PRESCALER_DIV32_gc`     : Division by 32
		 * - `RTC_PRESCALER_DIV64_gc`     : Division by 64
		 * - `RTC_PRESCALER_DIV128_gc`    : Division by 128
		 * - `RTC_PRESCALER_DIV256_gc`    : Division by 256
		 * - `RTC_PRESCALER_DIV512_gc`    : Division by 512
		 * - `RTC_PRESCALER_DIV1024_gc`   : Division by 1024
		 * - `RTC_PRESCALER_DIV2048_gc`   : Division by 2048
		 * - `RTC_PRESCALER_DIV4096_gc`   : Division by 4096
		 * - `RTC_PRESCALER_DIV8192_gc`   : Division by 8192
		 * - `RTC_PRESCALER_DIV16384_gc`  : Division by 16384
		 * - `RTC_PRESCALER_DIV32768_gc`  : Division by 32768
		 *
		 * By default, `RTC_PRESCALER` is set to `RTC_PRESCALER_DIV1_gc` (no prescaling).
		 */
		#define RTC_PRESCALER RTC_PRESCALER_DIV1_gc
	#endif
	
	#ifndef RTC_OVERFLOW
		/**
		 * @def RTC_OVERFLOW
		 * @brief Defines the overflow value for the RTC counter.
		 *
		 * @details
		 * This macro sets the counter overflow value for the real-time clock (RTC) peripheral on AVR0 devices. When the RTC counter reaches this value, it overflows and optionally triggers an interrupt, depending on the RTC configuration in the application.
		 *
		 * The value is a 16-bit unsigned integer and must be in the range from 0x0001 to 0xFFFF.
		 *
		 * @note By default, `RTC_OVERFLOW` is set to `0x0021`, which, with an RTC clock of 32.768 kHz and a prescaler of 1, results in an interrupt period of roughly 1 ms that can be used as systick.
		 */
		#define RTC_OVERFLOW 0x0021
	#endif

	#include <avr/io.h>

	void rtc_init(void);

#endif /* RTC_H_ */