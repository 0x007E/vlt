
#include "crc16.h"

static unsigned int crc_register = CRC16_INITIAL_VALUE;

inline void crc16_init(unsigned int value)
{
	crc_register = value;
}

// Reference: https://ww1.microchip.com/downloads/en/Appnotes/Atmel-8936-CryptoAuth-Data-Zone-CRC-Calculation-ApplicationNote.pdf
void crc16_update(unsigned char data)
{
	for (unsigned char shift_register=0x01; shift_register > 0x00; shift_register <<= 1)
	{
		unsigned char data_bit = 0;
		
		if (data & shift_register)
		{
			data_bit = 1;
		}
		
		unsigned char crc_bit = (crc_register>>15) & 1;
		crc_register <<= 1;
		
		if (data_bit != crc_bit)
		{
			crc_register ^= CRC16_POLYNOM;
		}
	}
}

void crc16_calculate(unsigned char initial, unsigned char *data, unsigned char length)
{
	crc16_init(initial);
	
	for (unsigned char i=0; i < length; i++)
	{
		crc16_update(data[i]);
	}
}

inline unsigned int crc16_result(void)
{
	return crc_register;
}

void crc16_result_array(unsigned char *crc)
{
	crc[0] = (unsigned char)(crc16_result() & 0x00FF);
	crc[1] = (unsigned char)(crc16_result()>>8);
}
