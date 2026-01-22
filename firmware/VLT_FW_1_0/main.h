
#ifndef MAIN_H_
#define MAIN_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif
	
	#define IO_PORT PORTA
	#define LED PIN7_bm
	#define SWITCH_S1 PIN5_bm
	#define SWITCH_S2 PIN6_bm

	#define TRNG_PORT PORTB
	#define TRNG_PIN PIN3_bm

	#define AT24CM0X_PORT_WP PORTB
	#define AT24CM0X_PIN_WP PIN2_bm

	#include <string.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "../lib//hal/avr0/system/system.h"
	#include "../lib/hal/avr0/uart/uart.h"
	#include "../lib/hal/avr0/twi/twi.h"

	#include "../lib/drivers/crypto/rng90/rng90.h"
	#include "../lib/drivers/prom/at24cm0x/at24cm0x.h"
	
#endif /* MAIN_H_ */