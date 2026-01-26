

#ifndef AT24CM0X_H_
#define AT24CM0X_H_

	#ifndef F_CPU
		#define F_CPU 20000000UL
	#endif

	#ifndef AT24CM0X_HAL_PLATFORM
		#define AT24CM0X_HAL_PLATFORM avr0
	#endif

	#ifndef AT24CM0X_MEMORY_SIZE
		#define AT24CM0X_MEMORY_SIZE 262144UL
	#endif

	#ifndef AT24CM0X_PAGES
		#define AT24CM0X_PAGES 1024UL
	#endif
	
	#ifndef AT24CM0X_PAGE_SIZE
		#define AT24CM0X_PAGE_SIZE 256UL
	#endif

	#ifndef AT24CM0X_SERIAL_SIZE
		#define AT24CM0X_SERIAL_SIZE 16UL
	#endif
	
	#ifndef AT24CM0X_ENABLE_INTEGRITY_CHECK
		//#define AT24CM0X_ENABLE_INTEGRITY_CHECK
	#endif
	
	#ifndef AT24CM0X_WRITE_ACKNOWLEDGE_POLLING
		//#define AT24CM0X_WRITE_ACKNOWLEDGE_POLLING
	#endif
	
	#ifndef AT24CM0X_BASE_ADDRESS
		#define AT24CM0X_BASE_ADDRESS 0x50
		
		#ifndef AT24CM0X_ADDRESS_MASK
			#define AT24CM0X_ADDRESS_MASK 0x04
		#endif
		
		#ifndef AT24CM0X_ADDRESS_HIGH_MASK
			#define AT24CM0X_ADDRESS_HIGH_MASK 0x03
		#endif
	#endif
	
	#ifndef AT24CM0X_MULTI_DEVICES
		//#define AT24CM0X_MULTI_DEVICES
	#endif
	
	#ifndef AT24CM0X_MULTI_DEVICES
		#ifndef AT24CM0X_ADDRESS
			#ifndef AT24CM0X_ADDRESS_A2
				#define AT24CM0X_ADDRESS_A2 1
			#endif
			
			#define AT24CM0X_ADDRESS (AT24CM0X_BASE_ADDRESS | (AT24CM0X_ADDRESS_A2<<2))
		#endif
	#endif
	
	#ifndef AT24CM0X_WP_CONTROL_EN
		#define AT24CM0X_WP_CONTROL_EN
	#endif
	
	#ifndef AT24CM0X_WRITE_CYCLE_MS
		#define AT24CM0X_WRITE_CYCLE_MS 10UL
	#endif
	
	#include "../../../utils/macros/stringify.h"

	#include _STR(../../../hal/AT24CM0X_HAL_PLATFORM/twi/twi.h)
	
	#include "../../../utils/systick/systick.h"

	enum AT24CM0X_WP_Mode_t
	{
		AT24CM0X_WP_Mode_Enabled=0,
		AT24CM0X_WP_Mode_Disabled
	};
	typedef enum AT24CM0X_WP_Mode_t AT24CM0X_WP_Mode;
	
	enum AT24CM0X_Status_t
	{
		AT24CM0X_Status_Done=0,
		AT24CM0X_Status_Address_Error,
		AT24CM0X_Status_Page_Error,
		AT24CM0X_Status_Size_Error,
		AT24CM0X_Status_Data_Error,
		AT24CM0X_Status_TWI_Error,
		AT24CM0X_Status_General_Error
	};
	typedef enum AT24CM0X_Status_t AT24CM0X_Status;
	
	void at24cm0x_init(void);
	
	#ifdef AT24CM0X_MULTI_DEVICES
		void at24cm0x_device(unsigned char identifier);
	#endif
	
	#ifdef AT24CM0X_WP_CONTROL_EN
		void at24cm0x_wp(AT24CM0X_WP_Mode mode);
	#endif
	
	AT24CM0X_Status at24cm0x_write_byte(unsigned long address, unsigned char data);
	AT24CM0X_Status at24cm0x_write_page(unsigned int page, unsigned char *data, unsigned int size);
	AT24CM0X_Status at24cm0x_read_current_byte(unsigned char *data);
	AT24CM0X_Status at24cm0x_read_byte(unsigned long address, unsigned char *data);
	AT24CM0X_Status at24cm0x_read_sequential(unsigned long address, unsigned char *data, unsigned int size);

#endif /* AT24CM0X_H_ */