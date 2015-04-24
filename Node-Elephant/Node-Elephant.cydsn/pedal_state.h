/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef PEDAL_STATE_H
#define PEDAL_STATE_H

typedef enum {
	pedal_state_normal,
	pedal_state_calibrating,
	pedal_state_out_of_range,
	pedal_state_discrepency,
	pedal_state_implausible
} pedal_state;

#define PEDAL_BRAKE_IMPLAUSIBLE_BRAKE_PERCENT			(0.1)
#define PEDAL_BRAKE_IMPLAUSIBLE_THROTTLE_PERCENT 		(0.25)
#define PEDAL_INCREPENCY_PERCENT						(0.1)
#define PEDAL_BRAKE_IMPLAUSIBLE_EXIT_THROTTLE_PERCENT	(0.1)

#define PEDAL_MEASURE_TOLERANCE							(0.1)

#endif

/* [] END OF FILE */
