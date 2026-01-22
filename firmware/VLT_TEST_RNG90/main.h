
#ifndef MAIN_H_
#define MAIN_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif
	
	#define IO_PORT PORTA
	#define LED PIN7_bm

	#include <string.h>
	#include <avr/io.h>
	#include <avr/interrupt.h>
	#include <util/delay.h>

	#include "../lib/hal/avr0/system/system.h"
	#include "../lib/hal/avr0/rtc/rtc.h"
	#include "../lib/hal/avr0/input/input.h"
	#include "../lib/hal/avr0/uart/uart.h"

	#include "../lib/drivers/crypto/rng90/rng90.h"
	#include "../lib/utils/systick/systick.h"
	
#endif /* MAIN_H_ */