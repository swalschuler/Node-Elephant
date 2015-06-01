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
    if (state == pedal_state_driving)
    {
        can_buffer[0] = 0;
        can_buffer[1] = 0;

        can_buffer[4] = 0;
        can_buffer[5] = 0;
        can_buffer[6] = 0;
        can_buffer[7] = 0;
    	int16_t scaled_value = 0;
    	scaled_value = (*throttle_max - *throttle_ptr) * 0x7FFF / (*throttle_max - *throttle_min);
	    can_buffer[2] = scaled_value & 0xFF;
	    can_buffer[3] = (scaled_value >> 8) & 0xFF;
        CAN_SendMsginvertor();
    }

}

void CAN_invertor_init(void)
{
    CAN_timer_Start();
    isr_CAN_StartEx(isr_CAN_handler);
    LCDBuffer[0] = '\0';
}


void CAN_invertor_pause() {
    // CAN_timer_Stop();
    can_buffer[0] = 'a';
    CAN_SendMsginv_stop();
}

void CAN_invertor_resume() {
    can_buffer[0] = 'a';
    CAN_SendMsginv_start();
    // CAN_timer_Enable();
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
