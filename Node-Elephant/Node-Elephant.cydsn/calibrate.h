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

#include <project.h>
#include <stdio.h>
#include <math.h>

#define MIN_COUNT 409			// 409 = 0.4993V
#define MAX_COUNT 3686	        // 3686 = 4.4995V 

void calibrate(void);           // calibrate all prototype 
double torqueImp(uint16 sensor1, uint16 sensor2, volatile uint8_t* errMsg);     // torque implausibility function prototype
double brakePlaus(uint16 brake1, uint16 brake2, uint16 throttle1, uint16 throttle2, volatile uint8_t* errMsg); // brake plausibility function prototype
void outOfRange(uint16 throttle1, uint16 throttle2, uint16 brake1, uint16 brake2, uint16 steering, volatile uint8_t* errMsg);       // out of range funciton prototype

void restore_calibration_data(void);          // set calibrated values to variable 
// uint16 concantenate(reg8* regPointer, uint8* byteCount);      //gets sensor value from EEPROM

/* [] END OF FILE */