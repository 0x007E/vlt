
#include "at24cm0x.h"

volatile unsigned char at24cm0x_device_identifier;

void at24cm0x_init(void)
{
	#ifdef AT24CM0X_WP_CONTROL_EN
		at24cm0x_wp(AT24CM0X_WP_Mode_Enabled);
	#endif
	
	#ifdef AT24CM0X_MULTI_DEVICES
		at24cm0x_device_identifier = AT24CM0X_BASE_ADDRESS;
	#else
		at24cm0x_device_identifier = AT24CM0X_ADDRESS;
	#endif
}

#ifdef AT24CM0X_MULTI_DEVICES
	void at24cm0x_device(unsigned char identifier)
	{
		at24cm0x_device_identifier = AT24CM0X_BASE_ADDRESS | (identifier & AT24CM0X_ADDRESS_MASK);
	}
#endif

#ifdef AT24CM0X_WRITE_ACKNOWLEDGE_POLLING
	static void at24cm0x_write_acknowledge_polling(void)
	{
		TWI_Error error = TWI_None;
		
		do
		{
			twi_start();
			error = twi_address(at24cm0x_device_identifier, TWI_Write);
		} while (error == TWI_Ack);
		twi_stop();
	}
#endif

static TWI_Error at24cm0x_send_address(unsigned long address)
{
	unsigned char high_byte = (at24cm0x_device_identifier | (AT24CM0X_ADDRESS_HIGH_MASK & (unsigned char)(address>>16)));
	unsigned char middle_byte = (unsigned char)(address>>8);
	unsigned char low_byte = (unsigned char)(address);
	
	TWI_Error error = TWI_None;
	
	error |= twi_address(high_byte, TWI_Write);
	error |= twi_set(middle_byte);
	error |= twi_set(low_byte);
	
	return error;
}

__attribute__((weak))
AT24CM0X_Status at24cm0x_write_byte(unsigned long address, unsigned char data)
{
	TWI_Error error = TWI_None;
	
	if(address >= AT24CM0X_MEMORY_SIZE)
	{
		return AT24CM0X_Status_Address_Error;
	}
	
	#ifdef AT24CM0X_WP_CONTROL_EN
		at24cm0x_wp(AT24CM0X_WP_Mode_Disabled);
	#endif
	
	twi_start();
	error |= at24cm0x_send_address(address);
	error |= twi_set(data);
	twi_stop();
	
	#ifdef AT24CM0X_WRITE_ACKNOWLEDGE_POLLING
		at24cm0x_write_acknowledge_polling();
	#else
		systick_timer_wait_ms(AT24CM0X_WRITE_CYCLE_MS);
	#endif
	
	#ifdef AT24CM0X_WP_CONTROL_EN
		at24cm0x_wp(AT24CM0X_WP_Mode_Enabled);
	#endif
		
	if(error != TWI_None)
	{
		return AT24CM0X_Status_TWI_Error;
	}
	
	#ifdef AT24CM0X_ENABLE_INTEGRITY_CHECK
		unsigned char temp = 0;
		
		if(at24cm0x_read_byte(address, &temp) != AT24CM0X_Status_Done)
		{
			return AT24CM0X_Status_TWI_Error;
		}
		
		if(data != temp)
		{
			return AT24CM0X_Status_Data_Error;
		}
	#endif
	
	return AT24CM0X_Status_Done;
}

AT24CM0X_Status at24cm0x_write_page(unsigned int page, unsigned char *data, unsigned int size)
{
	TWI_Error error = TWI_None;

	if(page >= AT24CM0X_PAGES)
	{
		return AT24CM0X_Status_Page_Error;
	}
	
	if(size == 0 || size >= AT24CM0X_PAGE_SIZE)
	{
		return AT24CM0X_Status_Size_Error;
	}
	
	unsigned long address = (page * AT24CM0X_PAGE_SIZE);
	
	#ifdef AT24CM0X_WP_CONTROL_EN
		at24cm0x_wp(AT24CM0X_WP_Mode_Disabled);
	#endif
	
	twi_start();
	at24cm0x_send_address(address);
	
	for (unsigned int i=0; i < size; i++)
	{
		error |= twi_set(*(data + i));
	}
	twi_stop();
	
	#ifdef AT24CM0X_WRITE_ACKNOWLEDGE_POLLING
		at24cm0x_write_acknowledge_polling();
	#else
		systick_timer_wait_ms(AT24CM0X_WRITE_CYCLE_MS);
	#endif
	
	#ifdef AT24CM0X_WP_CONTROL_EN
		at24cm0x_wp(AT24CM0X_WP_Mode_Enabled);
	#endif
	
	#ifdef AT24CM0X_ENABLE_INTEGRITY_CHECK
		unsigned char buffer[AT24CM0X_PAGE_SIZE];
		
		if(at24cm0x_read_sequential(address, buffer, size) != AT24CM0X_Status_Done)
		{
			return AT24CM0X_Status_TWI_Error;
		}
		
		for (unsigned int i=0; i < size; i++)
		{
			if(buffer[i] != *(data + i))
			{
				return AT24CM0X_Status_Data_Error;
			}
		}
	#endif
	
	if(error != TWI_None)
	{
		return AT24CM0X_Status_TWI_Error;
	}
	return AT24CM0X_Status_Done;
}

AT24CM0X_Status at24cm0x_read_current_byte(unsigned char *data)
{
	TWI_Error error = TWI_None;
	
	twi_start();
	error |= twi_address(at24cm0x_device_identifier, TWI_Read);
	error |= twi_get(data, TWI_NACK);
	twi_stop();
	
	if(error != TWI_None)
	{
		return AT24CM0X_Status_TWI_Error;
	}
	return AT24CM0X_Status_Done;
}

AT24CM0X_Status at24cm0x_read_byte(unsigned long address, unsigned char *data)
{
	if(address >= AT24CM0X_MEMORY_SIZE)
	{
		return AT24CM0X_Status_Address_Error;
	}
	
	AT24CM0X_Status status = AT24CM0X_Status_Done;
	TWI_Error error = TWI_None;
	
	twi_start();
	error |= at24cm0x_send_address(address);
	twi_stop();
	
	twi_start();
	error |= twi_address(at24cm0x_device_identifier, TWI_Read);
	error |= twi_get(data, TWI_NACK);
	twi_stop();
	
	if(error != TWI_None || status != AT24CM0X_Status_Done)
	{
		return AT24CM0X_Status_TWI_Error;
	}
	return AT24CM0X_Status_Done;
}

AT24CM0X_Status at24cm0x_read_sequential(unsigned long address, unsigned char *data, unsigned int size)
{
	if(address >= AT24CM0X_MEMORY_SIZE)
	{
		return AT24CM0X_Status_Address_Error;
	}
	
	if(size == 0)
	{
		return AT24CM0X_Status_Size_Error;
	}
	
	AT24CM0X_Status status = AT24CM0X_Status_Done;
	TWI_Error error = TWI_None;
	
	twi_start();
	error |= at24cm0x_send_address(address);
	twi_stop();
	
	twi_start();
	error |= twi_address(at24cm0x_device_identifier, TWI_Read);
	
	for (unsigned int i=0; i < size; i++)
	{
		TWI_Operation ack = TWI_Ack;
		
		if(i >= (size - 1))
		{
			ack = TWI_NACK;
		}
		error |= twi_get((data + i), ack);
	}
	twi_stop();
	
	if(error != TWI_None || status != AT24CM0X_Status_Done)
	{
		return AT24CM0X_Status_TWI_Error;
	}
	return AT24CM0X_Status_Done;
}