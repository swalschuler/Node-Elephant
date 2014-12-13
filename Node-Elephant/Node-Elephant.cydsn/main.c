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


extern volatile uint8 toggle_flag;      //external variable declared in isr.c for triggering interrupt code
volatile uint8_t Tx0_Throttle[4];       //transmission data for throttle one and two
volatile uint8_t Tx1_BrakeSteer[6];     //transmission data for brake 1 and 2 and steering
volatile uint8_t Tx2_Error;          //transmission data for error/okay code
volatile uint8_t Rx_0;                  //reception data, not sure if needed or how large.


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
    
    uint32 throttle1;            //variables for sending average to can
    uint32 throttle2;
    uint32 brake1;
    uint32 brake2;
    uint32 steering;

    uint8 buffSize = 0;           //tracks number of conversions 
    uint16 avgVolt;
    
    LCD_Position(0,0);
    LCD_PrintString("Throttle 1 AVG:");
    
    for(;;)
    {
        if(toggle_flag == 1)            //if interrupt triggered 
        {           
            avgVolt = ADC_SAR_CountsTo_Volts(throttle1/buffSize)*256 + 0.5;         //gets throttle1 voltage using average counts and converts to sevcon. +0.5 to round
            Tx0_Throttle[0] = avgVolt & 0xff;           //lower 8-bits for throttle1 
            avgVolt >>= 8;                      //shift right by 8 bits
            Tx0_Throttle[1] = avgVolt & 0xff;           //upper 8-bits for throttle1
            
            avgVolt = ADC_SAR_CountsTo_Volts(throttle2/buffSize)*256 + 0.5;         //gets throttle2 voltage using average counts and converts to sevcon. +0.5 to round
            Tx0_Throttle[2] = avgVolt & 0xff;           //lower 8-bits for throttle2
            avgVolt >>= 8;                      //shift right by 8 bits
            Tx0_Throttle[3] = avgVolt & 0xff;           //upper 8-bits for throttle2
            
            avgVolt = ADC_SAR_CountsTo_Volts(brake1/buffSize)*256 + 0.5;            //gets brake1 voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BrakeSteer[0] = avgVolt & 0xff;           //lower 8-bits for brake1
            avgVolt >>= 8;                      //shift right by 8 bits
            Tx1_BrakeSteer[1] = avgVolt & 0xff;           //upper 8-bits for brake1
            
            avgVolt = ADC_SAR_CountsTo_Volts(brake2/buffSize)*256 + 0.5;            //gets brake2 voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BrakeSteer[2] = avgVolt & 0xff;           //lower 8-bits for brake2
            avgVolt >>= 8;                      //shift right by 8 bits
            Tx1_BrakeSteer[3] = avgVolt & 0xff;           //upper 8-bits for brake2
            
            avgVolt = ADC_SAR_CountsTo_Volts(steering/buffSize)*256 + 0.5;          //gets steering voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BrakeSteer[4] = avgVolt & 0xff;           //lower 8-bits for steering
            avgVolt >>= 8;                      //shift right by 8 bits
            Tx1_BrakeSteer[5] = avgVolt & 0xff;           //upper 8-bits for steering
            
            CAN_SendMsg0();         //send throttle message
            CAN_SendMsg1();         //send brake and steering message
            CAN_SendMsg2();         //send error/okay message
            
            LCD_Position(1,0);
            LCD_PrintNumber(throttle1);
            
            throttle1 = 0;            //reset variables 
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
