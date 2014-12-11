/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * 
 * ========================================
*/

#include <project.h>
#include <stdio.h>
#include "calibrate.h"


void calAll(void);          //calibrate all sensors

extern volatile uint8 toggle_flag;      //external variable declared in isr.c for triggering interrupt code

int main()
{
    LCD_Start();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    EEPROM_Start();
    isr_Start();
    Timer_Start();
    CyGlobalIntEnable;          //enable global interrupts 
    
    uint32 throttle1;            //variables for sending average to can
    uint32 throttle2;
    uint32 brake1;
    uint32 brake2;
    uint32 steering;

    uint8 buffSize = 0;           //tracks number of conversions 

    
    LCD_Position(0,0);
    LCD_PrintString("Throttle 1 AVG:");
    
    for(;;)
    {
        if(toggle_flag == 1)
        {
            throttle1 /= buffSize;
            throttle2 /= buffSize;
            brake1 /= buffSize;
            brake2 /= buffSize;
            steering /= buffSize;
            
            LCD_Position(1,0);
            LCD_PrintNumber(throttle1);
            
            throttle1 = 0;            //reset variables for sending average to can
            throttle2 = 0;
            brake1 = 0;
            brake2 = 0;
            steering = 0;
            buffSize = 0;
        }
        
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))  
        {
            throttle1 += ADC_SAR_GetResult16(0);        //get conversion results in terms of counts for throttle 1 (channel 0)  
            throttle2 += ADC_SAR_GetResult16(1);        //get conversion results in terms of counts for throttle 2 (channel 1)  
            brake1 += ADC_SAR_GetResult16(2);        //get conversion results in terms of counts for brake 1 (channel 2)  
            brake2 += ADC_SAR_GetResult16(3);        //get conversion results in terms of counts for brake 2 (channel 3)  
            steering += ADC_SAR_GetResult16(4);        //get conversion results in terms of counts for steering (channel 4)  
            buffSize++;
        }
    }
    
    return 0;
}


/* [] END OF FILE */
