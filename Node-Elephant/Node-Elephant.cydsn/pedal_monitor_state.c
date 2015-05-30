#include "pedal_monitor_state.h"

static pedal_state currentState = pedal_state_neutral;
static char* lit_neutral = "NEUTRAL\0";
static char* lit_driving = "DRIVING\0";
static char* lit_calibrating = "calibrating\0";
static char* lit_out_of_range = "OUT OF RANGE\0";
static char* lit_discrepency = "DISCREPENCY\0";
static char* lit_implausible = "IMPLAUSIBLE\0";


bool monitor_state_routine() {
	char* ptr = "\0";
	switch (currentState) {
		case pedal_state_neutral:
			ptr = lit_neutral;
			break;
		case pedal_state_driving:
			ptr = lit_driving;
			break;
		case pedal_state_calibrating:
			ptr = lit_calibrating;
			break;
		case pedal_state_out_of_range:
			ptr = lit_out_of_range;
			break;
		case pedal_state_discrepency:
			ptr = lit_discrepency;
			break;
		case pedal_state_implausible:
			ptr = lit_implausible;
			break;
		default:
			break;
	}
    while(USBUART_CDCIsReady() == 0u);
    USBUART_PutString(ptr);
	return true;
}

void monitor_update_vechicle_state(pedal_state state) {
	currentState = state;
}
