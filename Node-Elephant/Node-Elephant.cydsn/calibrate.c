/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "calibrate.h"

/*******************************************************************************
* Function Name: calAll()
********************************************************************************
*
* Summary:
*  Calibrates all sensors and then writes the values to EEPROM. Values
*  are saved in counts not volts. Do (counts/4096)*vref to get voltage. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Note: 
*  There is code that prints to the LCD. This will evntually be removed 
*  and code for printing to a terminal will be implemeted
*
*******************************************************************************/
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
 
//Code below is used for print to LCD for debugging     
        
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