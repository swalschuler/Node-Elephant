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

//When you program and run this code, the lcd will display what is supposedly being sent over can
//but I have yet to actually test its correctness. To tun the calibration code, press SW2 on the board
//and the calibration code should run. Continue to use SW2 to confirm voltages and step through the code.
//Channel assignments are shown in the TopDesign file near the ADC SAR Seq component. Pinout of these channels
//are shown in Node-Elephant.cydwr. They can be connected to any pin. The files in the Source and Header folders
//are what I wrote.

#include <project.h>
#include <stdio.h>
#include "calibrate.h"

volatile uint32 throttle1, throttle2, brake1, brake2, steering;            //variables for sending average to can
volatile uint16 count = 0;           //tracks number of conversions 

int main()
{
//    clock_StopBlock();
    LCD_Start();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    EEPROM_Start();
    isr_Start();
    Timer_Start();
    CAN_Init();
    CAN_Start();
    CyGlobalIntEnable;          //enable global interrupts 
    setCal();               //set min and max values
    
    for(;;)
    {
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))  
        {            
            throttle1 += ADC_SAR_GetResult16(0);        //get conversion results in terms of counts for throttle 1 (channel 0)  
            throttle2 += ADC_SAR_GetResult16(1);        //get conversion results in terms of counts for throttle 2 (channel 1)  
            brake1 += ADC_SAR_GetResult16(2);        //get conversion results in terms of counts for brake 1 (channel 2)  
            brake2 += ADC_SAR_GetResult16(3);        //get conversion results in terms of counts for brake 2 (channel 3)  
            steering += ADC_SAR_GetResult16(4);        //get conversion results in terms of counts for steering (channel 4)  
            
            if(count != 0xffff)
                count++;
        }
        
        if(Button_Read() == 0)      //press button to run calibration
        {
            clock_StopBlock();      //stop clock to disable interrupt 
            calAll();
            LCD_ClearDisplay();
            isr_ClearPending();     //clear pending interrupts
            clock_Start();          //restart the clock
        }
    }   
    
    return 0;
}


/* [] END OF FILE */
