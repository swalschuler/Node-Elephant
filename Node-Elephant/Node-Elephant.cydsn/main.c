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
volatile uint8_t Rx_0;                  //reception data, not sure if needed or how large.
volatile uint8_t Tx0_Throttle[8];       //transmission data for throttle one and two
volatile uint8_t Tx1_BSE[8];            //transmission data for brake 1, brake 2, steering, and error

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
    
    uint32 throttle1, throttle2, brake1, brake2, steering;            //variables for sending average to can

    uint8 buffSize = 0;           //tracks number of conversions 
    uint16 avgVoltT1, avgVoltT2, avgVoltB1, avgVoltB2, avgVoltST, temp;       //average voltages 
    
    for(;;)
    {
        if(toggle_flag == 1)            //if interrupt triggered 
        {
            avgVoltT1 = ADC_SAR_CountsTo_Volts(throttle1/buffSize)*256 + 0.5;         //gets throttle1 voltage using average counts and converts to sevcon. +0.5 to round
            Tx0_Throttle[0] = avgVoltT1 & 0xff;           //lower 8-bits for throttle1 
            temp = avgVoltT1 >> 8;                      //shift right by 8 bits
            Tx0_Throttle[1] = temp & 0xff;           //upper 8-bits for throttle1
            
            avgVoltT2 = ADC_SAR_CountsTo_Volts(throttle2/buffSize)*256 + 0.5;         //gets throttle2 voltage using average counts and converts to sevcon. +0.5 to round
            Tx0_Throttle[2] = avgVoltT2 & 0xff;           //lower 8-bits for throttle2
            temp = avgVoltT2 >> 8;                      //shift right by 8 bits
            Tx0_Throttle[3] = temp & 0xff;           //upper 8-bits for throttle2
            
            avgVoltB1 = ADC_SAR_CountsTo_Volts(brake1/buffSize)*256 + 0.5;            //gets brake1 voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BSE[0] = avgVoltB1 & 0xff;           //lower 8-bits for brake1
            temp = avgVoltB1 >> 8;                      //shift right by 8 bits
            Tx1_BSE[1] = temp & 0xff;           //upper 8-bits for brake1
            
            avgVoltB2 = ADC_SAR_CountsTo_Volts(brake2/buffSize)*256 + 0.5;            //gets brake2 voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BSE[2] = avgVoltB2 & 0xff;           //lower 8-bits for brake2
            temp = avgVoltB2 >> 8;                      //shift right by 8 bits
            Tx1_BSE[3] = temp & 0xff;           //upper 8-bits for brake2
            
            avgVoltST = ADC_SAR_CountsTo_Volts(steering/buffSize)*256 + 0.5;          //gets steering voltage using average counts and converts to sevcon. +0.5 to round
            Tx1_BSE[4] = avgVoltST & 0xff;           //lower 8-bits for steering
            temp = avgVoltST >> 8;                      //shift right by 8 bits
            Tx1_BSE[5] = temp & 0xff;           //upper 8-bits for steering
            
            outOfRange(avgVoltT1, avgVoltT2, avgVoltB1, avgVoltB2, avgVoltST, &Tx1_BSE[6]);     // byte 6 contains lower 8 bits of error msg
            torqueImp(avgVoltT1, avgVoltT2, &Tx1_BSE[6]);     // torque implausibility check
            brakePlaus(avgVoltB1, avgVoltB2, avgVoltT1, avgVoltT2, &Tx1_BSE[6]);       // brake plausibility check
            
            CAN_SendMsg0();         //send throttle message
            CAN_SendMsg1();         //send brake, steering, and error message  
            
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
    calAll();
    
    
    return 0;
}


/* [] END OF FILE */
