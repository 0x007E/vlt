
#ifndef MAIN_H_
#define MAIN_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif
	
	#define IO_PORT PORTA
	#define LED PIN7_bm

	#ifndef TRNG_PORT
		#define TRNG_PORT PORTB
	#endif

	#ifndef TRNG_PIN
		#define TRNG_PIN         SET_PIN(3, _bm)
		#define TRNG_PIN_PINCTRL SET_PIN(3, CTRL)
		#define TRNG_PIN_SETUP   PORT_PULLUPEN_bm
	#endif

	#define AT24CM0X_PORT_WP PORTB
	#define AT24CM0X_PIN_WP PIN2_bm

	#include <string.h>
	#include <avr/io.h>
	#include <avr/eeprom.h>
	#include <avr/interrupt.h>

	#include "../lib/hal/avr0/system/system.h"
	#include "../lib/hal/avr0/rtc/rtc.h"
	#include "../lib/hal/avr0/input/input.h"
	#include "../lib/hal/avr0/uart/uart.h"
	#include "../lib/hal/avr0/twi/twi.h"

	#include "../lib/drivers/crypto/trng/trng.h"
	#include "../lib/drivers/crypto/rng90/rng90.h"
	#include "../lib/drivers/prom/at24cm0x/at24cm0x.h"
	
	#include "../lib/utils/systick/systick.h"
	#include "../lib/utils/console/console.h"
	
#endif /* MAIN_H_ */