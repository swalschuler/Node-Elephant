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

volatile uint32 throttle1, throttle2, brake1, brake2, steering;            //variables for sending average to can
volatile uint16 buffSize = 0;           //tracks number of conversions 

int main()
{
    LCD_Start();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    EEPROM_Start();
    isr_Start();
    Timer_Start();
    CAN_Init();
    CAN_Start();
    CyGlobalIntEnable;          //enable global interrupts 
   
    for(;;)
    {
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))  
        {
//            LCD_Position(0,0);
//            LCD_PrintNumber(ADC_SAR_GetResult16(0));   
            
            throttle1 += ADC_SAR_GetResult16(0);        //get conversion results in terms of counts for throttle 1 (channel 0)  
            throttle2 += ADC_SAR_GetResult16(1);        //get conversion results in terms of counts for throttle 2 (channel 1)  
            brake1 += ADC_SAR_GetResult16(2);        //get conversion results in terms of counts for brake 1 (channel 2)  
            brake2 += ADC_SAR_GetResult16(3);        //get conversion results in terms of counts for brake 2 (channel 3)  
            steering += ADC_SAR_GetResult16(4);        //get conversion results in terms of counts for steering (channel 4)  
            
            if(buffSize != 0xffff)
                buffSize++;
        }       
    }
    
    return 0;
}


/* [] END OF FILE */
