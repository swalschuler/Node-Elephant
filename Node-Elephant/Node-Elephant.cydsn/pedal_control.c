#include <stdint.h>
#include <stdbool.h>
#include "pedal_control.h"
#include "EEPROM_util.h"

#define USED_EEPROM_SECTOR                      (1u)
#define CALIBRATION_DATA_BASE_ADDRESS          ((USED_EEPROM_SECTOR * CYDEV_EEPROM_SECTOR_SIZE) + 0x00)

#define CALIBRATION_COUNT (10)

int16_t MIN_THROTTLE1 = 0;
int16_t MIN_THROTTLE2 = 0;
int16_t MAX_THROTTLE1 = 0;
int16_t MAX_THROTTLE2 = 0;

int16_t MIN_BRAKE1 = 0;
int16_t MIN_BRAKE2 = 0;
int16_t MAX_BRAKE1 = 0;
int16_t MAX_BRAKE2 = 0;

int16_t STEER_LEFT = 0;
int16_t STEER_RIGHT = 0;

int16_t throttle1 = 0;
int16_t throttle2 = 0;
int16_t brake1 = 0;
int16_t brake2 = 0;
int16_t steering = 0;

void pedal_calibrate(void)           //calibrate all sensors
{
    double volts;        //stores voltage conversion value in volts
    // uint16 voltCounts;  //stores voltage conversion value in counts
    uint8 i = 0;        //counter for for loop 
    char buff[50];
    uint8_t channelNum[CALIBRATION_COUNT] = {0, 1, 0, 1, 2, 3, 2, 3, 4, 4};    //array of channel numbers
    int16_t* calibrated_value[CALIBRATION_COUNT] = {
        &MIN_THROTTLE1,
        &MIN_THROTTLE2,
        &MAX_THROTTLE1,
        &MAX_THROTTLE2,

        &MIN_BRAKE1,
        &MIN_BRAKE2,
        &MAX_BRAKE1,
        &MAX_BRAKE2,

        &STEER_LEFT,
        &STEER_RIGHT
    };

    // reg8* regPointer = (reg8*)CYDEV_EE_BASE;           //pointer pointing to base of EEPROM (row 1)
    // cystatus writeStatus = CYRET_SUCCESS;       //return status of EEPROM_ByteWrite

    
    for(i = 0; i < CALIBRATION_COUNT; i++)
    {
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("Wait...");
        CyDelay(1000);              //Delay because button value resets slower than loop runs
        LCD_ClearDisplay();
        switch (i)
        {
            case 0: LCD_PrintString("Throttle 1: Low");
                break;
            case 1: LCD_PrintString("Throttle 2: Low");
                break;
            case 2: LCD_PrintString("Throttle 1: High");
                break;
            case 3: LCD_PrintString("Throttle 2: High");
                break;

            case 4: LCD_PrintString("Brake 1: Low");
                break;
            case 5: LCD_PrintString("Brake 2: Low");
                break;
            case 6: LCD_PrintString("Brake 1: HIGH");
                break;
            case 7: LCD_PrintString("Brake 2: HIGH");
                break;
            case 8: LCD_PrintString("Steering: Left");
                break;
            case 9: LCD_PrintString("Steering: Right");
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }

        for (;;)
        {
            int16_t ADC_value = 0;
            if (ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
            {
                ADC_value = ADC_SAR_GetResult16(channelNum[i]);
                volts = ADC_SAR_CountsTo_Volts(ADC_value);        //Converts ouput (hex16)from indexed channel to floating point voltage value 
                LCD_Position(1u, 0u);
                sprintf(buff, "%0.4fv, %d", volts, ADC_value);            //Makes floating point to acii
                LCD_PrintString(buff);             //Print ASCII voltage value  
            
                //If button is pressed, end calibration for current variable
                if (Button_Read() == 0)
                {
                    *(calibrated_value[i]) = ADC_value;
                    EEPROM_set(ADC_value, CALIBRATION_DATA_BASE_ADDRESS, i);
                    break;
                }
            }

        }
    }
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Calibration");
    LCD_Position(1,0);
    LCD_PrintString("Complete");
    CyDelay(2000);
 
//Code below is used for print to LCD for debugging     
        
    for (i = 0; i < CALIBRATION_COUNT; i++)
    {     
        LCD_ClearDisplay();
        LCD_Position(0,0);
        
        switch (i)
        {
            case 0: LCD_PrintString("Throttle 1: Low");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MIN_THROTTLE1);
                break;
            case 1: LCD_PrintString("Throttle 2: Low");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MIN_THROTTLE2);
                break;
            case 2: LCD_PrintString("Throttle 1: High");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MAX_THROTTLE1);
                break;
            case 3: LCD_PrintString("Throttle 2: High");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MAX_THROTTLE2);
                break;

            case 4: LCD_PrintString("Brake 1: Low");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MIN_BRAKE1);
                break;
            case 5: LCD_PrintString("Brake 2: Low");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MIN_BRAKE2);
                break;
            case 6: LCD_PrintString("Brake 1: HIGH");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MAX_BRAKE1);
                break;
            case 7: LCD_PrintString("Brake 2: HIGH");
                    LCD_Position(1,0);
                    LCD_PrintNumber(MAX_BRAKE2);
                break;

            case 8: LCD_PrintString("Steering: Left");
                    LCD_Position(1,0);
                    LCD_PrintNumber(STEER_LEFT);
                break;
            case 9: LCD_PrintString("Steering: Right");
                    LCD_Position(1,0);
                    LCD_PrintNumber(STEER_RIGHT);
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }
        CyDelay(1000);
    }
    Button_ClearInterrupt();
    return;
}


void pedal_fetch_data(void)
{
    int16_t* data_list[5] =
    {
        &throttle1,
        &throttle2,
        &brake1,
        &brake2,
        &steering
    };
    uint8_t i = 0;
    for (i = 0; i< 5; i++)
    {
        if (ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
        {
           *(data_list[i]) = ADC_SAR_GetResult16(i); 
        }
    }
}


/************************************************************************************
* Function Name: torqueImp(uint16 sensor1, uint16 sensor2, volatile uint8_t* errMsg)
*************************************************************************************
*
* Summary:
*  Checks for torque implausibility (EV2.3.6). The percentage difference is
*  calculated using the voltage read from the two throttle sensors. A percentage
*  difference of 10% will trigger a error to be sent over CAN. FOR USE WITH
*  THROTTLE ONLY.
*
* Parameters:
*  sensor1: 1st sensor count value 
*  sensor2: 2nd sensor count value 
*  errMsg: Pointer to sensor's error message
*
* Return:
*  percentDiff: percent difference between sensor values
*
*
************************************************************************************/

double torqueImp(uint16 sensor1, uint16 sensor2, volatile uint8_t* errMsg)
{
	double percentDiff;
	percentDiff = fabs((double)sensor1 - (double)sensor2) / (((double)sensor1 + (double)sensor2) / 2) * 100;		// calculates percentage difference by dividing the difference by the average

	if (percentDiff > 10.0)		// if percent difference is greater than 10%
	    *errMsg += 0x0020;			// error msg for torque implausibility

	return percentDiff;
}

uint8_t pedal_is_torque_plausible(double* brake_percentage_diff, double* throttle_percentage_diff)
{

    double percentDiff = fabs((double)(brake1 - brake2)) / brake1;
    bool fault_occurred = false;
    *brake_percentage_diff = percentDiff;

    if (percentDiff > 0.1)
    {
        fault_occurred = true;
    }

    percentDiff = fabs((double)(throttle1 - throttle2)) / throttle1;
    *throttle_percentage_diff = percentDiff;
    
    if (percentDiff > 0.1)
    {
        fault_occurred = true;
    }

    if (fault_occurred)
    {
        return pedal_brake_plausible_torque;
    }
    return pedal_brake_plausible_yes;
}

uint8_t pedal_is_brake_plausible(double* brake_percentage_ptr, double* throttle_percentage_ptr)
{
    if (brake1 > MIN_BRAKE1)          
    {
        double brake_percentage = (double)(brake1) / (MAX_BRAKE1 - MIN_BRAKE2);
        double throttle_percentage = (double)(throttle1) / (MAX_THROTTLE1 - MIN_THROTTLE1);
        *brake_percentage_ptr = brake_percentage;
        *throttle_percentage_ptr = throttle_percentage;
        if (brake_percentage > 0.1 && throttle_percentage > 0.25)
        {
            return pedal_brake_plausible_brake;
        }
        return pedal_brake_plausible_yes;
    }
    else
    {
        return pedal_brake_plausible_brake;
    }
    return pedal_brake_plausible_yes;
}

uint8_t pedal_get_out_of_range_flag(void)
{
    uint8_t error_flag = pedal_out_of_range_none;

    if (throttle1 < MIN_THROTTLE1 || throttle1 > MAX_THROTTLE1)
        error_flag |= pedal_out_of_range_throttle1;
    
    if(throttle2 < MIN_THROTTLE2 || throttle2 > MAX_THROTTLE2)
        error_flag |= pedal_out_of_range_throttle2;
    
    if (brake1 < MIN_BRAKE1)
        error_flag |= pedal_out_of_range_brake1;
    
    if (brake2 < MIN_COUNT)
        error_flag |= pedal_out_of_range_brake2;
    
    if (steering < STEER_LEFT || steering > STEER_RIGHT)
        error_flag |= pedal_out_of_range_steering;

    return error_flag;
}

void pedal_restore_calibration_data(void)
{   
    MIN_THROTTLE1 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 0);
    MIN_THROTTLE2 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 1);
    MAX_THROTTLE1 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 2);
    MAX_THROTTLE2 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 3);
    MIN_BRAKE1 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 4);
    MIN_BRAKE2 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 5);
    MAX_BRAKE1 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 6);
    MAX_BRAKE2 = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 7);
    STEER_LEFT = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 8);
    STEER_RIGHT = EEPROM_get(CALIBRATION_DATA_BASE_ADDRESS, 9);
}

/* [] END OF FILE */
