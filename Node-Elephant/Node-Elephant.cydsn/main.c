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
    
    uint32 throttle = 0;            //declared 32 bits for calculation, will cast to 16 bit when pushing to can
    uint32 throttle1 = 0;
    uint32 throttle2 = 0;
    uint32 brake1 = 0;
    uint32 brake2 = 0;
    uint32 steering = 0;
    uint32 averager = 0;         //counts number of times adc gets a result and used as divisor for average
    
    for(;;)
    {                  
        if(toggle_flag == 1)            //if interrupt fired
        {
            throttle = (float)(throttle1/averager) + 0.5;           //gets rounded average value over time interval between interrupts
            throttle2 =(float)(throttle2/averager) + 0.5;
            brake1 = (float)(brake1/averager) + 0.5;
            brake2 = (float)(brake2/averager) + 0.5;
            steering = (float)(steering/averager) + 0.5;
            
            LED_Write(~LED_Read());             //visual check is interrupt is running
            
            LCD_ClearDisplay();
            LCD_Position(0,0);              //displaying number of times values were retrieved 
            LCD_PrintNumber(averager);
            LCD_Position(1,0);              //displaying number of times values were retrieved 
            LCD_PrintNumber(throttle1);
            //CyDelay(2000);
            LCD_Position(0,7);              //displaying number of times values were retrieved 
            LCD_PrintNumber(throttle);
            averager = 0;
            toggle_flag = 0;
            throttle = 0;
            throttle1 = 0;
            throttle2 = 0;
            brake1 = 0;
            brake2 = 0;
            steering = 0;
            averager = 0;
        }
        
        if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
        {
            throttle1 = throttle1 + ADC_SAR_GetResult16(0);         //summing values over the time interval between interrupts of each channel
            throttle2 = throttle2 + ADC_SAR_GetResult16(1);
            brake1 = brake1 + ADC_SAR_GetResult16(2);
            brake2 = brake2 + ADC_SAR_GetResult16(3);
            steering = steering + ADC_SAR_GetResult16(4);
//            LCD_Position(1,5);              //displaying number of times values were retrieved 
//            LCD_PrintNumber(averager);
            averager++;
            //CyDelay(2000);
        }
    }
    
    return 0;
}

void calAll(void)           //calibrate all sensors
{
    float volts;        //stores voltage conversion value in volts
    uint16 voltCounts;  //stores voltage conversion value in counts
    uint8 i = 0;        //counter for for loop 
    uint8 j = 0;        //counter for for loop
    char buff[50];
    uint8 channelNum[8] = {0,1,0,1,2,3,4,4};    //array of channel numbers
    uint8 getMod[4];     //temporary array to store mod values
    reg8* regPointer = (reg8*)CYDEV_EE_BASE;           //pointer pointing to base of EEPROM (row 1)
    cystatus writeStatus = CYRET_SUCCESS;       //return status of EEPROM_ByteWrite
    uint8 row = 0;              //row of EEPROM to write to (0 = 1st row)
    uint8 byteCount = 0;        //keeps track of which byte in row to write to (0 = 1st byte)
    uint16 temp = 0;        //holds voltCounts value for printing      
    
    for(i = 0; i < 8; i++)
    {
        LCD_ClearDisplay();
        LCD_Position(0,0);
        LCD_PrintString("Wait...");
        CyDelay(1000);              //Delay because button value resets slower than loop runs
        LCD_ClearDisplay();
        
        for(j = 0; j < 4; j++)
            getMod[j] = 0;     //reset temporary array to store mod values
        
        if(i == 4)
            row = 1;        //changes EEPROM row to write to row 2
        
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
            case 4: LCD_PrintString("Break 1: Low");
                break;
            case 5: LCD_PrintString("Break 2: Low");
                break;
            case 6: LCD_PrintString("Steering: Left");
                break;
            case 7: LCD_PrintString("Steering: Right");
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }
            
        while(i<8)          //while loops prints voltage value to LCD, button press gets conversion in volts
        {
            if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))
            {
                volts = ADC_SAR_CountsTo_Volts(ADC_SAR_GetResult16(channelNum[i]));        //Converts ouput (hex16)from indexed channel to floating point voltage value 
                LCD_Position(1u, 0u);
                sprintf(buff, "%0.4fv, %d", volts, ADC_SAR_GetResult16(channelNum[i]));            //Makes floating point to acii
                LCD_PrintString(buff);             //Print ACII voltage value  
            }
            
            if(Button_Read() == 0)      //if button pressed, set to resistive pull up
            {
                if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))        //if ADC conversion is done
                {
                    voltCounts = ADC_SAR_GetResult16(channelNum[i]);
                    
                    for(j = 3; voltCounts != 0; j--)        //seperates voltCounts into individual digits and store into array
                    {
                        getMod[j] = voltCounts % 10;        //mod by 10 returns right most digit
                        voltCounts = voltCounts / 10;             //division by 10 removes right most digit
                    }
                    
                    for(j = 0; j < 4; j++)          //write array getMod into EEPROM
                    {
                        if(CySetTemp() == CYRET_SUCCESS)        //if EEPROM die temp okay
                        {
                            if(byteCount == 16)         //reset byte count, only 16 bits allowed per row (15 = 16th bit)
                                byteCount = 0;
                            
                            if((regPointer+row*16)[byteCount] != getMod[j])       //checks if byte to write is same as byte stored, does not write if they are equal
                                writeStatus = EEPROM_ByteWrite(getMod[j], row, byteCount);      //write individual digit of sevconVolts (stored in getMod) to EEPROM  
                            
                            if(writeStatus != CYRET_SUCCESS)            //if error occured during write
                            {
                                LCD_ClearDisplay();
                                LCD_Position(0,0);
                                LCD_PrintString("ERROR:");
                                sprintf(buff, "%d %d %d", getMod[j], row, byteCount);
                                LCD_PrintString(buff);
                            }
                            
                            byteCount++;
                        }
                    }
                    
                    break;      //break from while loop when write is done
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
    
    byteCount = 0;
    row = 0;
    
    for(i = 0; i < 8; i++)
    {     
        LCD_ClearDisplay();
        LCD_Position(0,0);
        
        temp = 0;           //reset temp
        
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
            case 4: LCD_PrintString("Break 1: Low");
                break;
            case 5: LCD_PrintString("Break 2: Low");
                break;
            case 6: LCD_PrintString("Steering: Left");
                break;
            case 7: LCD_PrintString("Steering: Right");
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }
        
        if(byteCount == 16)             //if byteCount = 16 then row one is finished reading from
        {
            regPointer = regPointer + byteCount;        //add by 16 to move to next row for reading
            byteCount = 0;          //reset byteCount
        }
        
        for(j = 0; j < 4; j++)
        {   
            temp = temp * 10;
            temp = temp + regPointer[byteCount];
            byteCount++;
        }
        
        LCD_Position(1,0);
        LCD_PrintNumber(temp);
        CyDelay(1000);
    }
}

/* [] END OF FILE */
