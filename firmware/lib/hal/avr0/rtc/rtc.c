
#include "rtc.h"

void rtc_init(void)
{	
	RTC.PER = 0x0021;
	RTC.INTCTRL = RTC_OVF_bm;
	RTC.CLKSEL = RTC_CLOCK;
	RTC.CTRLA = RTC_PRESCALER | RTC_RTCEN_bm;
}