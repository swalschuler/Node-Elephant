#include <stdint.h>
#include <stdbool.h>
#include "pedal_control.h"
#include "EEPROM_util.h"
#include "pedal_state.h"

#define USED_EEPROM_SECTOR                      (1u)
#define CALIBRATION_DATA_BASE_ADDRESS          ((USED_EEPROM_SECTOR * CYDEV_EEPROM_SECTOR_SIZE) + 0x00)

#define CALIBRATION_COUNT (10)

static int16_t MIN_THROTTLE1 = 0;
static int16_t MIN_THROTTLE2 = 0;
static int16_t MAX_THROTTLE1 = 0;
static int16_t MAX_THROTTLE2 = 0;

static int16_t MIN_BRAKE1 = 0;
static int16_t MIN_BRAKE2 = 0;
static int16_t MAX_BRAKE1 = 0;
static int16_t MAX_BRAKE2 = 0;

static int16_t STEER_LEFT = 0;
static int16_t STEER_RIGHT = 0;

static int32_t MIN_THROTTLE1_MV = 0;
static int32_t MIN_THROTTLE2_MV = 0;
static int32_t MAX_THROTTLE1_MV = 0;
static int32_t MAX_THROTTLE2_MV = 0;

static int32_t MIN_BRAKE1_MV = 0;
static int32_t MIN_BRAKE2_MV = 0;
static int32_t MAX_BRAKE1_MV = 0;
static int32_t MAX_BRAKE2_MV = 0;

static int32_t STEER_LEFT_MV = 0;
static int32_t STEER_RIGHT_MV = 0;

static int16_t throttle1 = 0;
static int16_t throttle2 = 0;
static int16_t brake1 = 0;
static int16_t brake2 = 0;
static int16_t steering = 0;

static int32_t throttle1_mv = 0;
static int32_t throttle2_mv = 0;
static int32_t brake1_mv = 0;
static int32_t brake2_mv = 0;
static int32_t steering_mv = 0;

#define PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_1 ((MAX_THROTTLE1_MV - MIN_THROTTLE1_MV) * PEDAL_MEASURE_TOLERANCE)
#define PEDAL_MEASURE_TOLERANCE_BRAKE_MV_1 ((MAX_BRAKE1_MV - MIN_BRAKE1_MV) * PEDAL_MEASURE_TOLERANCE)
#define PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_2 ((MAX_THROTTLE2_MV - MIN_THROTTLE2_MV) * PEDAL_MEASURE_TOLERANCE)
#define PEDAL_MEASURE_TOLERANCE_BRAKE_MV_2 ((MAX_BRAKE2_MV - MIN_BRAKE2_MV) * PEDAL_MEASURE_TOLERANCE)

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

    int32_t* converted_calibrated_value[CALIBRATION_COUNT] = {
        &MIN_THROTTLE1_MV,
        &MIN_THROTTLE2_MV,
        &MAX_THROTTLE1_MV,
        &MAX_THROTTLE2_MV,

        &MIN_BRAKE1_MV,
        &MIN_BRAKE2_MV,
        &MAX_BRAKE1_MV,
        &MAX_BRAKE2_MV,

        &STEER_LEFT_MV,
        &STEER_RIGHT_MV
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
                sprintf(buff, "%0.4fv, $%04X$", volts, ADC_value);            //Makes floating point to acii
                LCD_PrintString(buff);             //Print ASCII voltage value  
            
                //If button is pressed, end calibration for current variable
                if (Button_Read() == 0)
                {
                    *(calibrated_value[i]) = ADC_value;
                    *(converted_calibrated_value[i]) = ADC_SAR_CountsTo_mVolts(*(calibrated_value[i]));
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

    int32_t* converted_data_list[5] =
    {
        &throttle1_mv,
        &throttle2_mv,
        &brake1_mv,
        &brake2_mv,
        &steering_mv
    };
    
    uint8_t i = 0;
    for (i = 0; i< 5; i++)
    {
        if (ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
        {
           *(data_list[i]) = ADC_SAR_GetResult16(i); 
           *(converted_data_list[i]) = ADC_SAR_CountsTo_mVolts(*(data_list[i]));
        }
    }
}

uint8_t pedal_is_torque_plausible(double* brake_percentage_diff, double* throttle_percentage_diff)
{

    float brake1_percent = (float)(brake1_mv - MIN_BRAKE1_MV) / (float)(MAX_BRAKE1_MV - MIN_BRAKE1_MV);
    float brake2_percent = (float)(brake2_mv - MIN_BRAKE2_MV) / (float)(MAX_BRAKE2_MV - MIN_BRAKE2_MV);

    double percentDiff = fabs(brake1_percent - brake2_percent);
    bool fault_occurred = false;
    *brake_percentage_diff = percentDiff;

    if (percentDiff > PEDAL_INCREPENCY_PERCENT)
    {
        fault_occurred = true;
    }

    float throttle1_percent = (float)(throttle1_mv - MIN_THROTTLE1_MV) / (float)(MAX_THROTTLE1_MV - MIN_THROTTLE1_MV);
    float throttle2_percent = (float)(throttle2_mv - MIN_THROTTLE2_MV) / (float)(MAX_THROTTLE2_MV - MIN_THROTTLE2_MV);
    percentDiff = fabs(throttle1_percent - throttle2_percent);
    *throttle_percentage_diff = percentDiff;
    
    if (percentDiff > PEDAL_INCREPENCY_PERCENT)
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
    if (brake1_mv >= MIN_BRAKE1_MV && brake1_mv <= MAX_BRAKE1_MV)          
    {
        double brake_percentage = (double)(brake1_mv - MIN_BRAKE1_MV) / (MAX_BRAKE1_MV - MIN_BRAKE1_MV);
        double throttle_percentage = (double)(throttle1_mv - MIN_THROTTLE1_MV) / (MAX_THROTTLE1_MV - MIN_THROTTLE1_MV);
        *brake_percentage_ptr = brake_percentage;
        *throttle_percentage_ptr = throttle_percentage;
        if (
            brake_percentage > (PEDAL_BRAKE_IMPLAUSIBLE_BRAKE_PERCENT - PEDAL_MEASURE_TOLERANCE) && 
            throttle_percentage > (PEDAL_BRAKE_IMPLAUSIBLE_THROTTLE_PERCENT + PEDAL_MEASURE_TOLERANCE)
            )
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

    if (
        throttle1_mv < (MIN_THROTTLE1_MV - PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_1)
         || throttle1_mv > (MAX_THROTTLE1_MV + PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_1)
         )
        error_flag |= pedal_out_of_range_throttle1;
    
    if (
        throttle2_mv < (MIN_THROTTLE2_MV - PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_2) 
        || throttle2_mv > (MAX_THROTTLE2_MV + PEDAL_MEASURE_TOLERANCE_THROTTLE_MV_2)
        )
        error_flag |= pedal_out_of_range_throttle2;
    
    if (
        brake1_mv < (MIN_BRAKE1_MV - PEDAL_MEASURE_TOLERANCE_BRAKE_MV_1) 
        || brake1_mv > (MAX_BRAKE1_MV + PEDAL_MEASURE_TOLERANCE_BRAKE_MV_1)
        )
        error_flag |= pedal_out_of_range_brake1;
    
    if (
        brake2_mv < (MIN_BRAKE2_MV - PEDAL_MEASURE_TOLERANCE_BRAKE_MV_2) 
        || brake2_mv > (MAX_BRAKE2_MV + PEDAL_MEASURE_TOLERANCE_BRAKE_MV_2)
        )
        error_flag |= pedal_out_of_range_brake2;
    
    // if (
    //     steering_mv < (STEER_LEFT_MV) 
    //     || steering_mv > (STEER_RIGHT_MV)
    //     )
    //     error_flag |= pedal_out_of_range_steering;

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

    MIN_THROTTLE1_MV = ADC_SAR_CountsTo_mVolts(MIN_THROTTLE1);
    MIN_THROTTLE2_MV = ADC_SAR_CountsTo_mVolts(MIN_THROTTLE2);
    MAX_THROTTLE1_MV = ADC_SAR_CountsTo_mVolts(MAX_THROTTLE1);
    MAX_THROTTLE2_MV = ADC_SAR_CountsTo_mVolts(MAX_THROTTLE2);
    MIN_BRAKE1_MV = ADC_SAR_CountsTo_mVolts(MIN_BRAKE1);
    MIN_BRAKE2_MV = ADC_SAR_CountsTo_mVolts(MIN_BRAKE2);
    MAX_BRAKE1_MV = ADC_SAR_CountsTo_mVolts(MAX_BRAKE1);
    MAX_BRAKE2_MV = ADC_SAR_CountsTo_mVolts(MAX_BRAKE2);
    STEER_LEFT_MV = ADC_SAR_CountsTo_mVolts(STEER_LEFT);
    STEER_RIGHT_MV = ADC_SAR_CountsTo_mVolts(STEER_RIGHT);
}

/* [] END OF FILE */
