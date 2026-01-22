
#include "input.h"

void input_init(void)
{
    INPUT_PORT.DIRCLR = INPUT_PIN_S2 | INPUT_PIN_S1;
    INPUT_PORT.INPUT_PIN_S1_PINCTRL = INPUT_PIN_S1_SETUP;
    INPUT_PORT.INPUT_PIN_S2_PINCTRL = INPUT_PIN_S2_SETUP;
}

INPUT_Status input_status(INPUT_Name name)
{
    if(!(INPUT_PORT.IN & name))
    {
        _delay_ms(INPUT_DEBOUNCE_TIME);
        return INPUT_Status_ON;
    }
    return INPUT_Status_OFF;
}