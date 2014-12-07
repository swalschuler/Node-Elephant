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
    
    uint32 throttle1 = 0;            //declared 32 bits for calculation, will cast to 16 bit when pushing to can
    uint32 throttle2 = 0;
    uint32 brake1 = 0;
    uint32 brake2 = 0;
    uint32 steering = 0;
    uint32 averager = 0;         //counts number of times adc gets a result and used as divisor for average
    
//    for(;;)
//    {                  
//        if(toggle_flag == 1)            //if interrupt fired
//        {
//            throttle = (float)(throttle1/averager) + 0.5;           //gets rounded average value over time interval between interrupts
//            throttle2 =(float)(throttle2/averager) + 0.5;
//            brake1 = (float)(brake1/averager) + 0.5;
//            brake2 = (float)(brake2/averager) + 0.5;
//            steering = (float)(steering/averager) + 0.5;
//            
//            LED_Write(~LED_Read());             //visual check is interrupt is running
//            
//            LCD_ClearDisplay();
//            LCD_Position(0,0);              //displaying number of times values were retrieved 
//            LCD_PrintNumber(averager);
//            LCD_Position(1,0);              //displaying number of times values were retrieved 
//            LCD_PrintNumber(throttle1);
//            //CyDelay(2000);
//            LCD_Position(0,7);              //displaying number of times values were retrieved 
//            LCD_PrintNumber(throttle);
//            averager = 0;
//            toggle_flag = 0;
//            throttle = 0;
//            throttle1 = 0;
//            throttle2 = 0;
//            brake1 = 0;
//            brake2 = 0;
//            steering = 0;
//            averager = 0;
//        }
//        
//        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
//        {
//            throttle1 = throttle1 + ADC_SAR_GetResult16(0);         //summing values over the time interval between interrupts of each channel
//            throttle2 = throttle2 + ADC_SAR_GetResult16(1);
//            brake1 = brake1 + ADC_SAR_GetResult16(2);
//            brake2 = brake2 + ADC_SAR_GetResult16(3);
//            steering = steering + ADC_SAR_GetResult16(4);
////            LCD_Position(1,5);              //displaying number of times values were retrieved 
////            LCD_PrintNumber(averager);
//            averager++;
//            //CyDelay(2000);
//        }
//    }
    
    for(;;)
    {
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
        {
               
        }
    }
    
    
    return 0;
}


/* [] END OF FILE */
