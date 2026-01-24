
#include "main.h"

SYSTICK_Timer systick_timer;

ISR(PORTA_PORT_vect)
{
	// Restart System
	CCP = CCP_IOREG_gc;
	RSTCTRL.SWRR = RSTCTRL_SWRE_bm;

	PORTA.INTFLAGS = PORT_INT_6_bm;
}

// Called every ~ millisecond!
ISR(RTC_CNT_vect)
{
	systick_tick();
	RTC.INTFLAGS = RTC_OVF_bm;
}

void systick_timer_wait_ms(unsigned int ms)
{
	systick_timer_wait(ms);
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
	rtc_init();
	sei();
	
	systick_init();
	uart_init();
	twi_init();
	input_init();
	
	PORTA.DIRSET = PIN7_bm;
	
	while(input_status(INPUT_SW1) == INPUT_Status_OFF)
	{
		PORTA.OUTTGL = PIN7_bm;
		systick_timer_wait_ms(250UL);
	}
	
	systick_timer_wait_ms(500UL);
	PORTA.OUTCLR = PIN7_bm;

	PORTA.PIN6CTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	sei();
	
	printf("\n\rSystem startup:\n\r");
	
	//unsigned char i = 0;
	//while(1)
	//{
	//unsigned char temp = 10;
	//
	//printf("\n\rRead Data (%3u): ", i);
	//twi_start();
	//twi_address(0b01010100, TWI_Write);
	//twi_set(0x00);
	//twi_set((i++));
	//twi_start();
	//twi_address(0b01010100, TWI_Write);
	//twi_get(&temp, TWI_NACK);
	//twi_stop();
	//_delay_ms(500);
	//printf("%3u", temp);
	//}
	//
	at24cm0x_init();
	
	char buffer[100];
	
	printf("\n\rReset Buffer:\n\r");
	
	for (unsigned char i=0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		buffer[i] = 0x00;
	}
	
	#ifdef EEPROM_WRITE_EN
		strcpy(buffer, "Das_ist_ein_wichtiger_Test");
	
		//for (unsigned char i=0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
		//{
		//at24cm0x_write_byte(i, buffer[i]);
		//
		//if(buffer == '\0')
		//{
		//break;
		//}
		//}
	
		at24cm0x_write_page(0x00000000UL, (unsigned char *)buffer, sizeof(buffer));
	#endif
	
	printf("\n\rReset Buffer:\n\r");
	
	for (unsigned char i=0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		buffer[i] = 0x00;
	}
	
	printf("\n\rRead data from buffer (sequential):\n\r");
	
	at24cm0x_read_sequential(0ul, (unsigned char*)buffer, sizeof(buffer));
	
	printf("%s\n\r", buffer);
	
	printf("\n\rRead Data from Buffer (bytewise):\n\r");
	
	for (unsigned char i=0; i < (sizeof(buffer)/sizeof(buffer[0])); i++)
	{
		unsigned char temp = 0x00;
		
		if(i == 0)
		{
			at24cm0x_read_byte((0UL + i), &temp);
		}
		else
		{
			at24cm0x_read_current_byte(&temp);
		}
		
		buffer[i] = temp;
	}
	printf("%s\n\r", buffer);
	
	printf("\n\rReset Buffer:\n\r");
	
	for (unsigned char i=0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
	{
		buffer[i] = 0x00;
	}
	
	printf("Read current bytes:\n\r");
	
	for (unsigned char i=0; i < sizeof(buffer); i++)
	{
		unsigned char temp = 0x00;
		at24cm0x_read_current_byte(&temp);
		printf("0x%02hhx, ", temp);
	}
	
	while (1)
	{
		printf("\n\rReset Buffer:\n\r");
		
		for (unsigned char i=0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
		{
			buffer[i] = 0x00;
		}
		
		printf("\n\rRead data from buffer (sequential):\n\r");
		
		at24cm0x_read_sequential(0ul, (unsigned char*)buffer, sizeof(buffer));
		printf("%s\n\r", buffer);
		
		systick_timer_wait_ms(1000UL);
		
		PORTA.OUTTGL = PIN7_bm;
	}
}

