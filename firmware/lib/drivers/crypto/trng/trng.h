

#ifndef TRNG_H_
#define TRNG_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif
	
	#ifndef TRNG_BUFFER_SIZE
		#define TRNG_BUFFER_SIZE 10UL
	#endif

	#include <avr/io.h>
	#include <util/delay.h>
	
	enum TRNG_Buffer_t
    {
		TRNG_Buffer_Empty=0,
		TRNG_Buffer_Filling,
		TRNG_Buffer_Full,
		
    };
    typedef enum TRNG_Buffer_t TRNG_Buffer;
	
    void trng_init(void);
	void trng_next_bit(unsigned char bit);
	TRNG_Buffer trng_buffer_status(void);
	volatile unsigned char* trng_buffer(void);
	void trng_reset(void);

#endif /* TRNG_H_ */