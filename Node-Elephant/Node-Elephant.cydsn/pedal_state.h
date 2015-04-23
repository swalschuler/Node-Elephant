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

#endif

/* [] END OF FILE */
