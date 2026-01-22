
#include "main.h"

SYSTICK_Timer systick_timer;
static unsigned char rng_numbers[RNG90_OPERATION_RANDOM_RNG_SIZE];

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
	cli();
	systick_tick();
	sei();
	RTC.INTFLAGS = RTC_OVF_bm;
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
		systick_timer_set(&systick_timer, 250UL);
		systick_timer_wait(&systick_timer);
	}
	
	systick_timer_set(&systick_timer, 500UL);
	systick_timer_wait(&systick_timer);
	
	PORTA.OUTCLR = PIN7_bm;

	INPUT_PORT.INPUT_PIN_S2_PINCTRL = PORT_PULLUPEN_bm | PORT_ISC_FALLING_gc;
	
	printf("\n\rSystem startup:\n\r");
	
	RNG90_Status status;
	
	status = rng90_init();
	
	printf(" -> RNG90 Initialization: %02hhx\n\r", status);
	unsigned char serial[RNG90_OPERATION_READ_SERIAL_SIZE];
	
	status = rng90_serial(serial);
	printf(" -> RNG90 Serial: %02hhx\n\r", status);
	
	if(status == RNG90_Status_Success)
	{
	printf(" -> RNG90 Serial: 0x");
	for (unsigned char i=0; i < RNG90_OPERATION_READ_SERIAL_SIZE; i++)
	{
	printf("%02hhx", serial[i]);
	}
	printf("\n\r");
	}
	
	RNG90_Info info;
	info.DeviceID = 0x00;
	info.Revision = 0x00;
	info.RFU = 0x00;
	info.SiliconID = 0x00;
	
	status = rng90_info(&info);
	printf(" -> RNG90 Info: %02hhx\n\r", status);
	
	if(status == RNG90_Status_Success)
	{
	printf(" -> RNG90 Data:\n\r");
	printf("    +-----------------+\n\r");
	printf("    |  DeviceId: 0x%02hhx |\n\r", info.DeviceID);
	printf("    |  Revision: 0x%02hhx |\n\r", info.Revision);
	printf("    |       RFU: 0x%02hhx |\n\r", info.RFU);
	printf("    | SiliconId: 0x%02hhx |\n\r", info.SiliconID);
	printf("    +-----------------+\n\n\r");
	}
	
	while (1)
	{
		status = rng90_random(rng_numbers);
		printf(" -> RNG90 Random: %02hhx\n\r", status);
		printf(" -> RNG90 Numberset:\n\r");
		printf("    { ");
			
		for (unsigned char i=0; i < RNG90_OPERATION_RANDOM_RNG_SIZE; i++)
		{
			if(i == (RNG90_OPERATION_RANDOM_RNG_SIZE - 1))
			{
				printf("%02hhx", rng_numbers[i]);
				break;
			}
			printf("%02hhx, ", rng_numbers[i]);
		}
		printf(" }\n\r");
		
		systick_timer_set(&systick_timer, 1000UL);
		systick_timer_wait(&systick_timer);
	
		PORTA.OUTTGL = PIN7_bm;
	}
}

