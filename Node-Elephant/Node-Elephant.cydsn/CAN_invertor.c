#include <project.h>
#include <stdbool.h>
#include <stdio.h>
#include "CAN_invertor.h"

static int16_t* throttle_ptr;
static int16_t* throttle_min;
static int16_t* throttle_max;
static pedal_state state;
volatile uint8_t can_buffer[8];
char LCDBuffer[50];

CY_ISR(isr_CAN_handler)
{
    can_buffer[0] = 0;
    can_buffer[1] = 0;

    can_buffer[4] = 0;
    can_buffer[5] = 0;
    can_buffer[6] = 0;
    can_buffer[7] = 0;
    if (state == pedal_state_driving)
    {
    	int16_t scaled_value = 0;
    	scaled_value = (*throttle_max - *throttle_ptr) * 0x7FFF / (*throttle_max - *throttle_min);
	    can_buffer[2] = scaled_value & 0xFF;
	    can_buffer[3] = (scaled_value >> 8) & 0xFF;
        LCD_Position(0,0);
        LCD_PrintString("Throttle value:");
        LCD_Position(1,0);
        sprintf(LCDBuffer, "$%04X$, $%04X$", *throttle_ptr, scaled_value);
        LCD_PrintString(LCDBuffer);
    }
    else
    {
	    can_buffer[2] = 0;
	    can_buffer[3] = 0;
    }
    CAN_SendMsginvertor();

}

void CAN_invertor_init(void)
{
    isr_CAN_StartEx(isr_CAN_handler);
    CAN_timer_Start();
    LCDBuffer[0] = '\0';
}

void CAN_invertor_set_throttle_ptr(int16_t* _throttle_ptr, int16_t* _throttle_min, int16_t* _throttle_max)
{
    throttle_ptr = _throttle_ptr;
    throttle_max = _throttle_max;
    throttle_min = _throttle_min;
}

void CAN_invertor_update_pedal_state(pedal_state _state)
{
	state = _state;
}
