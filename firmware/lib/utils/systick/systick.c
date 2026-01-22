
#include "systick.h"

volatile unsigned int systick_tick_value;

void systick_init(void)
{
	systick_reset();
}

void systick_reset(void)
{
	systick_tick_value = 0;
}

void systick_tick(void)
{
	systick_tick_value++;
}

unsigned int systick_current(void)
{
	  unsigned int temp;
	  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	  {
		  temp = systick_tick_value;
	  }
	  return temp;
}

void systick_timer_set(SYSTICK_Timer *timer, unsigned int delay_ticks)
{
	timer->expire = systick_current() + delay_ticks;
	timer->status = SYSTICK_Timer_Running;
}

SYSTICK_Timer_Status systick_timer_elapsed(SYSTICK_Timer *timer)
{
	if(timer->status != SYSTICK_Timer_Expired)
	{
		if((int)(systick_current() - timer->expire) >= 0)
		{
			timer->status = SYSTICK_Timer_Expired;
		}
	}
	return timer->status;
}

unsigned int systick_timer_remaining(SYSTICK_Timer *timer)
{
	if(timer->status != SYSTICK_Timer_Expired)
	{
		unsigned int now = systick_current();
		
		if((int)(now - timer->expire) >= 0)
		{
			timer->status = SYSTICK_Timer_Expired;
			return 0UL;
		}
		return timer->expire - now;
	}
	return 0UL;
}

void systick_timer_wait(SYSTICK_Timer *timer)
{
	while (systick_timer_elapsed(timer) == SYSTICK_Timer_Running)
	{
		asm volatile("NOP");
	}
}

void systick_timer_cancel(SYSTICK_Timer *timer)
{
	timer->status = SYSTICK_Timer_Expired;
}