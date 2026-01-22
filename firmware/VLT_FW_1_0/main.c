

#define F_CPU 20000000UL
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

ISR(PORTA_PORT_vect)
{
	// Restart System
	CCP = CCP_IOREG_gc;
	RSTCTRL.SWRR = RSTCTRL_SWRE_bm;

	PORTA.INTFLAGS = PORT_INT_6_bm;
}

void at24cm0x_wp(AT24CM0X_WP_Mode mode)
{
	if(mode)
	{
		AT24CM0X_PORT_WP.DIRSET = AT24CM0X_PIN_WP;
		AT24CM0X_PORT_WP.OUTCLR = AT24CM0X_PIN_WP;
		return;
	}
	AT24CM0X_PORT_WP.DIRCLR = AT24CM0X_PIN_WP;
}

int main(void)
{
	system_init();
	uart_init();
	twi_init();
	
	PORTA.DIRSET = PIN7_bm;
	
	PORTA.DIRCLR = PIN5_bm | PIN6_bm;
	PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
	PORTA.PIN6CTRL = PORT_PULLUPEN_bm;

	while(PORTA.IN & PIN5_bm)
	{
		PORTA.OUTTGL = PIN7_bm;
		_delay_ms(250);
	}

	_delay_ms(500);
	PORTA.OUTCLR = PIN7_bm;

	PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	sei();
	
	printf("\n\rSystem startup:\n\r");
	
	unsigned char i = 0;
	
	while(1)
	{
		unsigned char temp = 10;
		
		printf("\n\rRead Data (%3u): ", i);
		twi_start();
		twi_address(0b01010100, TWI_Write);
		twi_set(0x00);
		twi_set((i++));
		twi_start();
		twi_address(0b01010100, TWI_Write);
		twi_get(&temp, TWI_NACK);
		twi_stop();
		_delay_ms(500);
		printf("%3u", temp);
	}
	
	
	
	
	
	//RNG90_Status status;
	//
	//status = rng90_init();
	//at24cm0x_init();
	//
	//
	////char buffer[] = "Das_ist_ein_Test";
	//char buffer[100];
	//
	////for (unsigned char i=0; i < 16; i++)
	////{
	////at24cm0x_write_byte((0UL + i), 0xFF);
	////}
	//
	////at24cm0x_write_page(0UL, (unsigned char *)buffer, sizeof(buffer));
	//
	//printf("\n\rReset Buffer:\n\r");
	//
	//for (unsigned char i=0; i < sizeof(buffer); i++)
	//{
	//buffer[i] = 0x00;
	//}
	//
	//printf("\n\rread data from buffer (sequential):\n\r");
	//
	//at24cm0x_read_sequential(0ul, (unsigned char*)buffer, sizeof(buffer));
	//
	//printf("\n\r");
	//
	//printf("\n\rRead Data from Buffer (bytewise):\n\r");
	//
	//for (unsigned char i=0; i < sizeof(buffer); i++)
	//{
	//unsigned char temp = 0x00;
	//
	//if(i == 0)
	//{
	//at24cm0x_read_byte((0UL + i), &temp);
	//}
	//else
	//{
	//at24cm0x_read_current_byte(&temp);
	//}
	//
	//buffer[i] = temp;
	//printf("0x%02hhx, ", temp);
	//}
	//
	//printf("Read current bytes:\n\r");
	//
	//for (unsigned char i=0; i < sizeof(buffer); i++)
	//{
	//unsigned char temp = 0x00;
	//at24cm0x_read_current_byte(&temp);
	//printf("0x%02hhx, ", temp);
	//}
	//
	//printf("\n\rAT24CS0X Serial: \n\r");
	//printf(" -> %16s", buffer);
	//printf("\n\r");
	//
	//printf(" -> RNG90 Initialization: %02hhx\n\r", status);
	//unsigned char serial[RNG90_OPERATION_READ_SERIAL_SIZE];
	//
	//status = rng90_serial(serial);
	//printf(" -> RNG90 Serial: %02hhx\n\r", status);
	//
	//if(status == RNG90_Status_Success)
	//{
	//printf(" -> RNG90 Serial: 0x");
	//for (unsigned char i=0; i < RNG90_OPERATION_READ_SERIAL_SIZE; i++)
	//{
	//printf("%02hhx", serial[i]);
	//}
	//printf("\n\r");
	//}
	//
	//RNG90_Info info;
	//info.DeviceID = 0x00;
	//info.Revision = 0x00;
	//info.RFU = 0x00;
	//info.SiliconID = 0x00;
	//
	//status = rng90_info(&info);
	//printf(" -> RNG90 Info: %02hhx\n\r", status);
	//
	//if(status == RNG90_Status_Success)
	//{
	//printf(" -> RNG90 Data:\n\r");
	//printf("    +-----------------+\n\r");
	//printf("    |  DeviceId: 0x%02hhx |\n\r", info.DeviceID);
	//printf("    |  Revision: 0x%02hhx |\n\r", info.Revision);
	//printf("    |       RFU: 0x%02hhx |\n\r", info.RFU);
	//printf("    | SiliconId: 0x%02hhx |\n\r", info.SiliconID);
	//printf("    +-----------------+\n\n\r");
	//}
	//
	//while (1)
	//{
	//status = rng90_random(rng_numbers);
	//printf(" -> RNG90 Random: %02hhx\n\r", status);
	//printf(" -> RNG90 Numberset:\n\r");
	//printf("    { ");
	//for (unsigned char i=0; i < RNG90_OPERATION_RANDOM_RNG_SIZE; i++)
	//{
	//if(i == (RNG90_OPERATION_RANDOM_RNG_SIZE - 1))
	//{
	//printf("%02hhx", rng_numbers[i]);
	//break;
	//}
	//printf("%02hhx, ", rng_numbers[i]);
	//}
	//printf(" }\n\r");
	//_delay_ms(1000);
	//
	//at24cm0x_read_sequential(0ul, (unsigned char*)buffer, sizeof(buffer));
	//
	//PORTA.OUTTGL = PIN7_bm;
	//}
}

