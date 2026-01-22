
#ifndef INPUT_H_
#define INPUT_H_

    #ifndef F_CPU
        #define F_CPU 20000000UL
    #endif

    #ifndef INPUT_DEBOUNCE_TIME
        #define INPUT_DEBOUNCE_TIME 10
    #endif

    #ifndef INPUT_PORT
        #define INPUT_PORT PORTA
    #endif

    #ifndef INPUT_PIN_S1
        #define INPUT_PIN_S1         SET_PIN(5, _bm)
        #define INPUT_PIN_S1_PINCTRL SET_PIN(5, CTRL)
        #define INPUT_PIN_S1_SETUP   PORT_PULLUPEN_bm
    #endif
    
    #ifndef INPUT_PIN_S2
        #define INPUT_PIN_S2         SET_PIN(6, _bm)
        #define INPUT_PIN_S2_PINCTRL SET_PIN(6, CTRL)
        #define INPUT_PIN_S2_SETUP   PORT_PULLUPEN_bm
    #endif

    #include <avr/io.h>
    #include <util/delay.h>

	#include "../../common/macros/PORT_macros.h"

    enum INPUT_Name_t
    {
        INPUT_SW1=INPUT_PIN_S1,
        INPUT_SW2=INPUT_PIN_S2
    };
    typedef enum INPUT_Name_t INPUT_Name;

    enum INPUT_Status_t
    {
        INPUT_Status_OFF=0,
        INPUT_Status_ON
    };
    typedef enum INPUT_Status_t INPUT_Status;

    void input_init(void);
    INPUT_Status input_status(INPUT_Name name);

#endif /* INPUT_H_ */