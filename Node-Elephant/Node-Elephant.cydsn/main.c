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

//#define DMA_BYTES_PER_BURST     12
//#define DMA_REQUEST_PER_BURST   1
//#define DMA_SRC_ADDRESS        CYDEV_PERIPH_BASE
//#define DMA_DEST_ADDRESS       CYDEV_PERIPH_BASE

#define BUFF_LENGTH     300

void calAll(void);          //calibrate all sensors

extern volatile uint8 toggle_flag;      //external variable declared in isr.c for triggering interrupt code
extern volatile uint8 toggle_flag2;

int main()
{
    LCD_Start();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    EEPROM_Start();
    isr_Start();
    isr_filter_Start();
    Timer_Start();
    Filter_Start();
    CyGlobalIntEnable;          //enable global interrupts 
    
    uint16 throttle;
    uint16 throttle1;            //variables for sending average to can
    uint16 throttle2;
    uint16 brake1;
    uint16 brake2;
    uint16 steering;
    uint16 buffT1[BUFF_LENGTH];
    uint16 buffT2[BUFF_LENGTH];         //variables for buffering ADC results between interrupts
    uint16 buffB1[BUFF_LENGTH];
    uint16 buffB2[BUFF_LENGTH];
    uint16 buffST[BUFF_LENGTH];

    uint8 i;
    uint8 j;
    uint8 buffSize = 0;           //tracks number of conversions 
    uint16 window = 10;           //average width 
    
    LCD_Position(0,0);
    LCD_PrintString("Throttle 1 AVG:");
    
    for(;;)
    {
        if(toggle_flag == 1)
        {
            for(i = window; i < buffSize; i++)           //computes moving average
            {
                throttle = 0;
                throttle1 = 0;            //reset variables for sending average to can
                throttle2 = 0;
                brake1 = 0;
                brake2 = 0;
                steering = 0;
                
                for(j = 0+i; j+i < window+i; j++)         //gets the set to be averaged; size specified by window
                {
                    throttle1 += buffT1[i];
                    throttle2 += buffT2[i];             
                    brake1 += buffB1[i];
                    brake2 += buffB2[i];
                    steering += buffST[i];
                }
                
                throttle1 /= window;            //averages the values of the window set
                throttle2 /= window;             
                brake1 /= window;
                brake2 /= window;
                steering /= window;
            }
            
            LCD_Position(1,0);
            LCD_PrintNumber(throttle1);
        }
        
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))  
        {
            buffT1[buffSize] = ADC_SAR_GetResult16(0);        //get conversion results in terms of counts for throttle 1 (channel 0)  
            buffT2[buffSize] = ADC_SAR_GetResult16(1);        //get conversion results in terms of counts for throttle 2 (channel 1)  
            buffB1[buffSize] = ADC_SAR_GetResult16(2);        //get conversion results in terms of counts for brake 1 (channel 2)  
            buffB2[buffSize] = ADC_SAR_GetResult16(3);        //get conversion results in terms of counts for brake 2 (channel 3)  
            buffST[buffSize] = ADC_SAR_GetResult16(4);        //get conversion results in terms of counts for steering (channel 4)  
            buffSize++;
        }
    }
    
    
//    uint8 tdChanA;
//    uint8 myChannel;
//    
//    LCD_ClearDisplay();
//    
//    myChannel = DMA_DmaInitialize(DMA_BYTES_PER_BURST, DMA_REQUEST_PER_BURST, HI16(ADC_SAR_COUNT_PTR), HI16(DMA_DEST_ADDRESS));
//    tdChanA = CyDmaTdAllocate();
//    CyDmaTdSetConfiguration(tdChanA, DMA_BYTES_PER_BURST, tdChanA, 0);
//    CyDmaTdSetAddress(tdChanA, LO16((uint32)ADC_SAR_COUNT_PTR), LO16((uint32)Filter_STAGEAH_PTR));
////    CyDmaTdSetAddress(tdChanA, LO16(DMA_SRC_ADDRESS), LO16(DMA_DEST_ADDRESS));
//    CyDmaChSetInitialTd(myChannel, tdChanA);
//    CyDmaChEnable(myChannel, 1u);
//    
//    for(;;)
//    {
//        if(toggle_flag == 1)
//        {
//            throttle = Filter_Read16(Filter_CHANNEL_A );
//            
//            LCD_Position(0,0);
//            LCD_PrintNumber(throttle);
//        }
//    }
    
    return 0;
}


/* [] END OF FILE */
