

#ifndef SYSTICK_H_
#define SYSTICK_H_

	#include <stdint.h>
	#include <util/atomic.h>
	
	enum SYSTICK_Timer_Status_t
	{
		SYSTICK_Timer_Running=0,
		SYSTICK_Timer_Expired,
	};
	typedef enum SYSTICK_Timer_Status_t SYSTICK_Timer_Status;
	
	struct SYSTICK_Timer_t {
		unsigned int expire;
		SYSTICK_Timer_Status status;
	};
    typedef struct SYSTICK_Timer_t SYSTICK_Timer;
	
	
    void systick_init(void);
    void systick_reset(void);
    void systick_tick(void);
	unsigned int systick_current(void);
	
	void systick_timer_set(SYSTICK_Timer *timer, unsigned int delay_ticks);
	SYSTICK_Timer_Status systick_timer_elapsed(SYSTICK_Timer *timer);
	unsigned int systick_timer_remaining(SYSTICK_Timer *timer);
	void systick_timer_wait(SYSTICK_Timer *timer);
	void systick_timer_cancel(SYSTICK_Timer *timer);

#endif /* SYSTICK_H_ */