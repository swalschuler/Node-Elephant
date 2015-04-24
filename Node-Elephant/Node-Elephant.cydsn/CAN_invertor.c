#include <project.h>
#include "CAN_invertor.h"

static int16_t* throttle_ptr;
static int16_t* throttle_min;
static int16_t* throttle_max;
static pedal_state state;
volatile uint8_t can_buffer[8];

CY_ISR(isr_CAN_handler)
{
    can_buffer[0] = 0;
    can_buffer[1] = 0;

    can_buffer[4] = 0;
    can_buffer[5] = 0;
    can_buffer[6] = 0;
    can_buffer[7] = 0;
    if (state == pedal_state_normal)
    {
    	int16_t scaled_value = 0;
    	scaled_value = (*throttle_ptr - *throttle_min) * 0x7FFF / (*throttle_max - *throttle_min);
	    can_buffer[2] = scaled_value & 0xFF;
	    can_buffer[3] = (scaled_value >> 8) & 0xFF;
    }
    else
    {
	    can_buffer[2] = 0;
	    can_buffer[3] = 0;
    }
    CAN_SendMsginvertor();

    can_buffer[0] = 0;
    can_buffer[1] = 0;
    can_buffer[2] = 0;
    can_buffer[3] = 0;
    can_buffer[4] = 0;
    can_buffer[5] = 0;
    can_buffer[6] = 0;
    can_buffer[7] = 0;
    CAN_SendMsgdummy();
}

void CAN_invertor_init(void)
{
    isr_CAN_StartEx(isr_CAN_handler);
    CAN_timer_Start();
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
