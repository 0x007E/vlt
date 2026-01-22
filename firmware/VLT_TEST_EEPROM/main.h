
#ifndef MAIN_H_
#define MAIN_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif
	
	#ifndef EEPROM_WRITE_EN
		//#define EEPROM_WRITE_EN
	#endif
	
	#define IO_PORT PORTA
	#define LED PIN7_bm

	#define AT24CM0X_PORT_WP PORTB
	#define AT24CM0X_PIN_WP PIN2_bm
	
	#include <string.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "../lib/hal/avr0/system/system.h"
	#include "../lib/hal/avr0/rtc/rtc.h"
	#include "../lib/hal/avr0/input/input.h"
	#include "../lib/hal/avr0/uart/uart.h"
	#include "../lib/hal/avr0/twi/twi.h"

	#include "../lib/drivers/prom/at24cm0x/at24cm0x.h"
	#include "../lib/utils/systick/systick.h"
	
#endif /* MAIN_H_ */