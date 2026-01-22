
#include "rng90.h"

unsigned char rng90_buffer[87];

RNG90_Status rng90_init(void)
{
    if(rng90_selftest(RNG90_Run_DRBG_SelfTest) != RNG90_SelfTest_Success)
    {
        return RNG90_Status_SelfTest_Error;
    }
	return RNG90_Status_Success;
}

static void rng90_write(RNG90_Packet *packet)
{
    unsigned char *ptr = (unsigned char *)packet;
	packet->count += 7;

	crc16_init(CRC16_INITIAL_VALUE);

    twi_address(RNG90_ADDRESS, TWI_Write);
    twi_set(RNG90_EXECUTE_COMMAND);
	
    for (unsigned char i=0; i < (sizeof(RNG90_Packet) - RNG90_CRC_SIZE); i++)
    {
		crc16_update(*(ptr + i));
        twi_set(*(ptr + i));
    }
}

static void rng90_command(RNG90_Packet *packet)
{
    twi_start();
    rng90_write(packet);
	
	packet->crc = crc16_result();
	
    twi_set((unsigned char)(0x00FF & packet->crc));
    twi_set((unsigned char)(0x00FF & (packet->crc>>8)));
    twi_stop();
}

static RNG90_Frame rng90_frame;

static RNG90_Frame rng90_data(unsigned char *data)
{
	crc16_init(CRC16_INITIAL_VALUE);
	
	unsigned char temp = 0;
	unsigned int crc = 0x0000;
	
	rng90_frame.length = 1 + RNG90_CRC_SIZE;
	rng90_frame.status = RNG90_Data_Status_Invalid;
	
    twi_start();
    twi_address(RNG90_ADDRESS, TWI_Read);
	
    for (unsigned char i=0; i < rng90_frame.length - RNG90_CRC_SIZE; i++)
    {	
		twi_get(&temp, TWI_ACK);
        crc16_update(temp);
		
		if(i == 0)
		{
			rng90_frame.length = temp;
			continue;
		}
		*(data + i - 1) = temp;
    }
	
	twi_get(&temp, TWI_ACK);
	crc = (0x00FF & temp);
	twi_get(&temp, TWI_NACK);
	crc |= (0xFF00 & (temp<<8));

    twi_stop();
	
	rng90_frame.status = RNG90_Data_Status_Valid;

    if (crc != crc16_result())
    {
	    rng90_frame.status = RNG90_Data_Status_Invalid;
    }
    return rng90_frame;
}

RNG90_SelfTest_Status rng90_selftest(RNG90_Run_SelfTest test)
{
    RNG90_Packet packet;
    packet.count = 0;
    packet.opcode = RNG90_OPERATION_SELF_TEST;
    packet.param1 = test;
    packet.param2 = RNG90_OPERATION_SELF_TEST_PARAM2;
	packet.crc = 0x0000;

    rng90_command(&packet);
	_delay_ms(RNG90_SELFTEST_EXECUTION_TIME_MS);
	
	RNG90_Frame frame = rng90_data(rng90_buffer);
	
	if((frame.length == RNG90_STANDARD_FRAME_SIZE) && (frame.status == RNG90_Data_Status_Valid))
	{
		return (RNG90_SelfTest_Status)(rng90_buffer[0]);
	}
    return RNG90_SelfTest_Error;
}

RNG90_Status rng90_info(RNG90_Info *info)
{
	RNG90_Packet packet;
	packet.count = 0;
	packet.opcode = RNG90_OPERATION_INFO;
	packet.param1 = RNG90_OPERATION_INFO_PARAM1;
	packet.param2 = RNG90_OPERATION_INFO_PARAM2;
	packet.crc = 0x0000;

	rng90_command(&packet);
    _delay_ms(RNG90_INFO_EXECUTION_TIME_MS);

	RNG90_Frame frame = rng90_data(rng90_buffer);
	
	if((frame.length == RNG90_STANDARD_FRAME_SIZE) && (frame.status == RNG90_Data_Status_Valid))
	{
		return (RNG90_SelfTest_Status)(rng90_buffer[0]);
	}
	else if (frame.length == RNG90_INFO_FRAME_SIZE && (frame.status == RNG90_Data_Status_Valid))
	{
		info->RFU = rng90_buffer[0];
		info->DeviceID = rng90_buffer[1];
		info->SiliconID = rng90_buffer[2];
		info->Revision = rng90_buffer[3];
		return RNG90_Status_Success;
	}
	return RNG90_Status_Other_Error;
}

RNG90_Status rng90_random(unsigned char *numbers)
{
	RNG90_Packet packet;
	packet.count = RNG90_OPERATION_RANDOM_DATA_SIZE;
	packet.opcode = RNG90_OPERATION_RANDOM;
	packet.param1 = RNG90_OPERATION_RANDOM_PARAM1;
	packet.param2 = RNG90_OPERATION_RANDOM_PARAM2;
	packet.crc = 0x0000;
	
    twi_start();
    rng90_write(&packet);

    for (unsigned char i=0; i < RNG90_OPERATION_RANDOM_DATA_SIZE; i++)
    {
        if(twi_set(RNG90_OPERATION_RANDOM_DATA) != TWI_None)
        {
            twi_stop();
            return RNG90_Status_TWI_Error;
        }
        crc16_update(RNG90_OPERATION_RANDOM_DATA);
    }
	packet.crc = crc16_result();
	
	twi_set((unsigned char)(0x00FF & packet.crc));
	twi_set((unsigned char)(0x00FF & (packet.crc>>8)));
	twi_stop();

    _delay_ms(RNG90_RANDOM_EXECUTION_TIME_MS);
	
	RNG90_Frame frame = rng90_data(rng90_buffer);
	
	if((frame.length == RNG90_STANDARD_FRAME_SIZE) && (frame.status == RNG90_Data_Status_Valid))
	{
		return (RNG90_SelfTest_Status)(rng90_buffer[0]);
	}
	else if (frame.length == RNG90_NUMBER_FRAME_SIZE && (frame.status == RNG90_Data_Status_Valid))
	{
		for (unsigned char i=0; i < RNG90_OPERATION_RANDOM_RNG_SIZE; i++)
		{
			*(numbers + i) = rng90_buffer[i];
		}
		return RNG90_Status_Success;
	}
	return RNG90_Status_Other_Error;
}

RNG90_Status rng90_serial(unsigned char *serial)
{
	RNG90_Packet packet;
	packet.count = 0;
	packet.opcode = RNG90_OPERATION_READ;
	packet.param1 = RNG90_OPERATION_READ_PARAM1;
	packet.param2 = RNG90_OPERATION_READ_PARAM2;
	packet.crc = 0x0000;
	
	rng90_command(&packet);
    _delay_ms(RNG90_READ_EXECUTION_TIME_MS);

    RNG90_Frame frame = rng90_data(rng90_buffer);
    
    if((frame.length == RNG90_STANDARD_FRAME_SIZE) && (frame.status == RNG90_Data_Status_Valid))
    {
	    return (RNG90_SelfTest_Status)(rng90_buffer[0]);
    }
    else if (frame.length == RNG90_SERIAL_FRAME_SIZE && (frame.status == RNG90_Data_Status_Valid))
    {
		for (unsigned char i=0; i < RNG90_OPERATION_READ_SERIAL_SIZE; i++)
		{
			*(serial + i) = rng90_buffer[i];
		}
	    return RNG90_Status_Success;
    }
    return RNG90_Status_Other_Error;
}