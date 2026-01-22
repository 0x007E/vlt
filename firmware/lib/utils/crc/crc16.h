
#ifndef CRC16_H_
#define CRC16_H_

	#ifndef CRC16_POLYNOM
		#define CRC16_POLYNOM 0x8005
	#endif
	
	#ifndef CRC16_INITIAL_VALUE
		#define CRC16_INITIAL_VALUE 0x0000
	#endif
	
	#include <stdio.h>
	#include <stdint.h>

	void crc16_init(unsigned int value);
	void crc16_update(unsigned char data);
	void crc16_calculate(unsigned char initial, unsigned char *data, unsigned char length);
	unsigned int crc16_result(void);
	void crc16_result_array(unsigned char *crc);

#endif /* CRC16_H_ */