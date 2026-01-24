
#include "main.h"

SYSTICK_Timer systick_timer;

ISR(PORTA_PORT_vect)
{
	// Restart System
	CCP = CCP_IOREG_gc;
	RSTCTRL.SWRR = RSTCTRL_SWRE_bm;

	INPUT_PORT.INTFLAGS = PORT_INT_6_bm;
}

// Called every ~ millisecond!
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

int main(void)
{
	system_init();
	rtc_init();
	sei();
	
	systick_init();
	uart_init();
	input_init();
	
	PORTA.DIRSET = PIN7_bm;

	while(input_status(INPUT_SW1) == INPUT_Status_OFF)
	{
		PORTA.OUTTGL = PIN7_bm;
		systick_timer_wait_ms(250UL);
	}

	systick_timer_wait_ms(500UL);
	
	PORTA.OUTCLR = PIN7_bm;

	INPUT_PORT.INPUT_PIN_S2_PINCTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	
	printf("\n\rSystem startup\n\r");
	printf("Initialize TRNG\t");
	
	TRNG_PORT.DIRCLR = TRNG_PIN;
	TRNG_PORT.TRNG_PIN_PINCTRL = TRNG_PIN_SETUP;
	trng_init();
	
	printf("Done\n\r");
	
	printf("Initialize Timer\t");
	
	TCA0.SINGLE.PER = 0x0005;
	TCA0.SINGLE.INTCTRL = TCA_SINGLE_OVF_bm;
	TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc | TCA_SINGLE_ENABLE_bm;
	
	printf("Done\n\r");
	
	unsigned int per = 0UL;
	
	do 
	{
		printf("PER->[1-65535]: ");
		
		if(!(scanf("%u", &per) == 1))
		{
			per = 0UL;
			printf(" Error!\n\r");
			continue;
		}
		printf("\n\r");
	} while (per == 0UL);
	
	TCA0.SINGLE.PER = per;
	
	printf("\n\rTRNG_Numbers (PER-Value=%u): { ", TCA0.SINGLE.PER);
	
	while (1)
	{
		if (systick_timer_elapsed(&systick_timer))
		{
			PORTA.OUTTGL = PIN7_bm;
			systick_timer_set(&systick_timer, 500UL);
		}
		
		if(trng_buffer_status() == TRNG_Buffer_Full)
		{
			volatile unsigned char *trng_numbers = trng_buffer();
			
			for (unsigned char i=0; i < TRNG_BUFFER_SIZE; i++)
			{
				if(i < (TRNG_BUFFER_SIZE - 1))
				{
					printf("%u, ", *(trng_numbers)++);
				}
			}
			trng_reset();
		}
	}
}


