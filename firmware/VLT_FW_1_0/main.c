
#include "main.h"

unsigned char EEMEM ee_project[] = "\n\rVLT - Data Vault\n\r";
unsigned char EEMEM ee_creator[] = "Author: Ing. Raffael GAECHTER";
unsigned char EEMEM ee_copyright[] = "(c) 2026, All rights reserved!";
unsigned char EEMEM ee_version = 0x10;

unsigned char EEMEM ee_masterkey[] = "Master Key: ";

SYSTICK_Timer systick_timer;
char buffer[100];

enum BYTE_Nibble_t
{
	BYTE_Nibble_Low=0,
	BYTE_Nibble_High
};
typedef enum BYTE_Nibble_t BYTE_Nibble;

ISR(RTC_CNT_vect)
{
	systick_tick();
	RTC.INTFLAGS = RTC_OVF_bm;
}

ISR(TCA0_OVF_vect)
{
	trng_next_bit(TRNG_PORT.IN & TRNG_PIN);
	TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
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

static void trng_start(void)
{
	TRNG_PORT.DIRCLR = TRNG_PIN;
	TRNG_PORT.TRNG_PIN_PINCTRL = TRNG_PIN_SETUP;
	
	TCA0.SINGLE.PER = 0x0085;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
}

static void trng_stop(void)
{
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLA &=  ~TCA_SINGLE_ENABLE_bm;
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
	
	trng_init();
	trng_start();
	trng_stop();
	
	rng90_init();
	
	at24cm0x_init();
	
	PORTA.DIRSET = PIN7_bm;

	while(!input_status(INPUT_SW1))
	{
		PORTA.OUTTGL = PIN7_bm;
		systick_timer_wait_ms(250UL);
	}

	systick_timer_wait_ms(500UL);
	PORTA.OUTCLR = PIN7_bm;
	
	console_clear();
	
	eeprom_read_block(buffer, ee_project, sizeof(ee_project));
	printf("%sVersion: %1u.%1u", buffer, (eeprom_read_byte(&ee_version)>>4), (0x0F & eeprom_read_byte(&ee_version)));
	console_line(40);
	eeprom_read_block(buffer, ee_creator, sizeof(ee_creator));
	printf("%s", buffer);
	console_newline();
	eeprom_read_block(buffer, ee_copyright, sizeof(ee_copyright));
	printf("%s", buffer);
	console_line(40);
	eeprom_read_block(buffer, ee_masterkey, sizeof(ee_masterkey));
	printf("%s", buffer);
	
	unsigned char i = 0;
	unsigned char run = 1;
	
	BYTE_Nibble nibble = BYTE_Nibble_High;
	
	buffer[i] = '\0';
	
	do 
	{
		if(uart_scanchar(&buffer[i]) == UART_Received)
		{
			if(buffer[i] == '\n' || buffer[i] == '\r')
			{
				break;
			}
			uart_putchar('*');
			i++;
		}
		else if(input_status(INPUT_SW1))
		{
			systick_timer_wait_ms(250UL);
			
			if(nibble == BYTE_Nibble_High)
			{
				buffer[i] = buffer[i] + 0x10;
			}
			else if(nibble == BYTE_Nibble_Low)
			{
				buffer[i] = buffer[i] + 0x01;
			}
			
			if(	(nibble == BYTE_Nibble_Low) && 
				((buffer[i] < ' ') || 
				(buffer[i] > '~')))
			{
				printf("Error -> Restarting\n\r");
				
				// Restart System
				CCP = CCP_IOREG_gc;
				RSTCTRL.SWRR = RSTCTRL_SWRE_bm;

				PORTA.INTFLAGS = PORT_INT_6_bm;
			}
			
			while(input_status(INPUT_SW1))
			{
				PORTA.OUTSET = PIN7_bm;
			}
			
		}
		else if(input_status(INPUT_SW2))
		{
			systick_timer_wait_ms(10UL);
			
			if(nibble == BYTE_Nibble_High)
			{
				nibble = BYTE_Nibble_Low;
			}
			else
			{
				uart_putchar('*');
				buffer[(++i)] = '\0';
				nibble = BYTE_Nibble_High;
			}
			
			systick_timer_set(&systick_timer, 2000UL);
			
			while(input_status(INPUT_SW2))
			{
				PORTA.OUTSET = PIN7_bm;
				
				if(systick_timer_elapsed(&systick_timer) && nibble == BYTE_Nibble_High)
				{
					run = 0;
					break;
				}
			}
		}
		PORTA.OUTCLR = PIN7_bm;
		
		systick_timer_wait_ms(10UL);
		
	} while (run);
	
	PORTA.OUTCLR = PIN7_bm;
	buffer[i] = '\0';
	
	console_newline();
	
	printf("\n\rPhrase: %s\n\r", buffer);
	
	while(1)
	{
		systick_timer_wait_ms(1000UL);
		
		// Restart System
		CCP = CCP_IOREG_gc;
		RSTCTRL.SWRR = RSTCTRL_SWRE_bm;

		PORTA.INTFLAGS = PORT_INT_6_bm;
	}
}

