
#include "trng.h"

volatile unsigned char buffer[TRNG_BUFFER_SIZE];
volatile unsigned char current_buffer_position;
volatile unsigned char current_buffer_bit_position;

inline void trng_init(void)
{
	trng_reset();
}

#define TRNG_SAME_BITS_IN_A_ROW 4

volatile unsigned char last_bit;
volatile unsigned char run_length;

volatile unsigned char  primary_run = 0;
volatile unsigned char  first_bit  = 0;

volatile unsigned char lfsr_b = 0xC3;

void trng_next_bit(unsigned char bit)
{	
	if(current_buffer_position >= TRNG_BUFFER_SIZE)
	{
		return;
	}
	
	// Runtime filter
	//if (bit == last_bit)
	//{
		//run_length++;
		//
		//if (run_length >= TRNG_SAME_BITS_IN_A_ROW)
		//{
			//return;
		//}
	//}
	//else
	//{
		//last_bit = bit;
		//run_length = 0;
	//}
	
	// Von-Neumann-Correction
	unsigned char out_bit = 0;

	if (!primary_run)
	{
		first_bit  = bit;
		primary_run = 1;
		return;
	}
	primary_run = 0;

	if (first_bit != bit)
	{
		out_bit = 0;
		
		if(first_bit != 0 && bit == 0)
		{
			out_bit = 1;
		}
	}
	else
	{
		return;
	}
	
	// Write bit to buffer
	if(out_bit > 0)
	{
		buffer[current_buffer_position]	|= (1<<current_buffer_bit_position);
	}
	current_buffer_bit_position++;
	
	if(current_buffer_bit_position >= 8)
	{
		current_buffer_position++;
		current_buffer_bit_position = 0;
	}
}

TRNG_Buffer trng_buffer_status(void)
{
	if( (current_buffer_position == 0UL) &&
		(current_buffer_bit_position == 0UL))
	{
		return TRNG_Buffer_Empty;
	}
	else if(current_buffer_position >= TRNG_BUFFER_SIZE)
	{
		return TRNG_Buffer_Full;
	}
	return TRNG_Buffer_Filling;
}

volatile unsigned char* trng_buffer(void)
{
	return buffer;
}

void trng_reset(void)
{
	for (unsigned char i=0; i < TRNG_BUFFER_SIZE; i++)
	{
		buffer[i] = 0x00;
	}
	current_buffer_position = 0;
	current_buffer_bit_position = 0;

	last_bit = 0;
	run_length = 0;
	primary_run = 0;
	first_bit = 0;
}


