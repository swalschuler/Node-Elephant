//The current code uses switch 2 on the psoc board to comfirm the voltage.  

#include "calibrate.h"

uint16 MAX_THROTTLE1;
uint16 MIN_THROTTLE1;
uint16 MAX_THROTTLE2;
uint16 MIN_THROTTLE2;
uint16 MIN_BRAKE1;
uint16 MIN_BRAKE2;
uint16 STEER_LEFT;
uint16 STEER_RIGHT;

/*******************************************************************************
* Function Name: calAll(void)
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
    double volts;        //stores voltage conversion value in volts
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
//    uint16 temp = 0;        //holds voltCounts value for printing      
    
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

 // The switch statement is used only for debuggin purposes 
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
            case 6: LCD_PrintString("Steering: Left");
                break;
            case 7: LCD_PrintString("Steering: Right");
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }

//This while loop is used only to display the voltage on the lcd and is used for debugging, no use for the actual eeprom code.
//Most of the way the ADC code is written below comes staight from the ADC examples provided in psoc creator
 
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
            {break;
                if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))        //if ADC conversion is done
                {
                    voltCounts = ADC_SAR_GetResult16(channelNum[i]);
                    
                    for(j = 3; voltCounts != 0; j--)        //seperates voltCounts into individual digits and store into array
                    {
                        getMod[j] = voltCounts % 10;        //mod by 10 returns right most digit
                        voltCounts = voltCounts / 10;             //division by 10 removes right most digit
                    }
//For the eeprom write code below, I am writing to each cell of eeprom not each row. One eeprom cell is 1 byte and one row contains 16 cell totaling in 16 bytes
//What I did was to separate the value obtained from the ADC into individual digits and store each digit in a cell. Although now that I think about it you can just
//easily store the upper and lower bits into different cells and then concantenate them when reading from eeprom. Code below is probably not the most efficient.
                    
                    for(j = 0; j < 4; j++)          //write array getMod into EEPROM
                    {
                        if(CySetTemp() == CYRET_SUCCESS)        //if EEPROM die temp okay
                        {
                            if(byteCount == 16)         //reset byte count, only 16 bits allowed per row (15 = 16th bit)
                                byteCount = 0;
                            
                            if((regPointer+row*16)[byteCount] != getMod[j])       //checks if byte to write is same as byte stored, does not write if they are equal
                            //not too sure if the psoc automatically checks this before writing so I included it anyways
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
        //break;
    }
    
    LCD_ClearDisplay();
    LCD_Position(0,0);
    LCD_PrintString("Calibration");
    LCD_Position(1,0);
    LCD_PrintString("Complete");
    CyDelay(2000);
    
    setCal();
    byteCount = 0;
    row = 0;
 
//Code below is used for print to LCD for debugging     
        
    for(i = 0; i < 8; i++)
    {     
        LCD_ClearDisplay();
        LCD_Position(0,0);
        
//        temp = 0;           //reset temp
        
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
            case 6: LCD_PrintString("Steering: Left");
                    LCD_Position(1,0);
                    LCD_PrintNumber(STEER_LEFT);
                break;
            case 7: LCD_PrintString("Steering: Right");
                    LCD_Position(1,0);
                    LCD_PrintNumber(STEER_RIGHT);
                break;
            default: LCD_PrintString("Error in loop");
                break;
        }
//Code below was used to read values from eeprom directly. No longer in use. 
//The function setCal defined below is used to read values from eeprom and store them in temp variables for use.
                
//        if(byteCount == 16)             //if byteCount = 16 then row one is finished reading from
//        {
//            regPointer = regPointer + byteCount;        //add by 16 to move to next row for reading
//            byteCount = 0;          //reset byteCount
//        }
//        
//        for(j = 0; j < 4; j++)
//        {   
//            temp = temp * 10;
//            temp = temp + regPointer[byteCount];
//            byteCount++;
//        }
//        
//        LCD_Position(1,0);
//        LCD_PrintNumber(temp);
        CyDelay(2000);
    }
    
    return;
}


/************************************************************************************
* Function Name: torqueImp(uint16 sensor1, uint16 sensor2, volatile uint8_t* errMsg)
*************************************************************************************
*
* Summary:
*  Checks for torque implausibility (EV2.3.6). The percentage difference is
*  calculated using the voltage read from the two throttle sensors. A percentage
*  difference of 10% will trigger a error to be sent over CAN. FOR USE WITH
*  THROTTLE ONLY.
*
* Parameters:
*  sensor1: 1st sensor count value 
*  sensor2: 2nd sensor count value 
*  errMsg: Pointer to sensor's error message
*
* Return:
*  percentDiff: percent difference between sensor values
*
*
************************************************************************************/

double torqueImp(uint16 sensor1, uint16 sensor2, volatile uint8_t* errMsg)
{
	double percentDiff;
	percentDiff = fabs((double)sensor1 - (double)sensor2) / (((double)sensor1 + (double)sensor2) / 2) * 100;		// calculates percentage difference by dividing the difference by the average

	if (percentDiff > 10.0)		// if percent difference is greater than 10%
	    *errMsg += 0x0020;			// error msg for torque implausibility

	return percentDiff;
}


/***********************************************************************************************************************
* Function Name: brakePlaus(uint16 brake1, uint16 brake2, uint16 throttle1, uint16 throttle2, volatile uint8_t* errMsg)
************************************************************************************************************************
*
* Summary:
*  Checks for brake plausibility (EV2.5). If the brakes are actuated, the
*  throttle must not experience more than 25% pedal travel. An error message is
*  sent over CAN if the pedal travel is greater than 25%.
*
* Parameters:
*  brake1: brake 1 sensor value
*  brake2: brake 2 sensor value
*  throttle1: throttle 1 sensor value
*  throttle2: throttle 2 sensor value
*  errMsg: Pointer to sensor's error message
*
* Return:
*  plauseCheck1: percent pedal travel of sensor 1
*  plauseCheck2: percent pedal travel of sensor 2
*
* Note:
*  Only one of the two plausCheck will be returned. Must pass variables in that
*  order.
*  strPlausMsg should be passed in for errMsg since strPlausMsg contains the
*  torque implausibility message.
*
************************************************************************************************************************/

double brakePlaus(uint16 brake1, uint16 brake2, uint16 throttle1, uint16 throttle2, volatile uint8_t* errMsg)
{
	double plauseCheck1 = 0;
	double plauseCheck2 = 0;

	if(brake1 > MIN_BRAKE1 && brake2 > MIN_BRAKE2)			
	{
		plauseCheck1 = ((double)throttle1 / (double)MAX_THROTTLE1) * 100;			// calculates throttle depression percentage 
		plauseCheck2 = ((double)throttle2 / (double)MAX_THROTTLE2) * 100;
        
        if(plauseCheck1 > 25.0)			// if throttle sensor 1 experiences for than 25% pedal travel 
	    {
            *errMsg += 0x0040;				// brake plausibility error msg
		    return plauseCheck1;
	    }
        else if(plauseCheck2 > 25.0)			// if throttle sensor 2 experiences for than 25% pedal travel 
	    {
            *errMsg += 0x0040;				// brake plausibility error msg
	    	return plauseCheck2;
	    }
	}

	return plauseCheck1 = ((double)throttle1 / (double)MAX_THROTTLE1) * 100;		// return pedal travel even if brakes not depressed
}


/****************************************************************************************************************************************
* Function Name: outOfRange(uint16 throttle1, uint16 throttle2, uint16 brake1, uint16 brake2, uint16 steering, volatile uint8_t* errMsg)
*****************************************************************************************************************************************
*
* Summary:
*  Checks for out of range (EV2.3.10, EV2.4.5). Any value between 0.854V and 
*  0.499V (409-decimal)and above 4.4995V (3686-decimal) will will trigger an error 
*  to be sent over CAN.
*
* Parameters:
*  throttle1: throttle 1 sensor count value	  
*  throttle2: throttle 2 sensor count value	 
*  brake1: brake 1 sensor count value	 
*  brake2: brake 2 sensor count value	 
*  errMsg: Pointer to sensor's error message (should be 6th element of Tx1_BSE)
*
* Return:
*  None.
*
*
*******************************************************************************/

void outOfRange(uint16 throttle1, uint16 throttle2, uint16 brake1, uint16 brake2, uint16 steering, volatile uint8_t* errMsg)
{   
    if (throttle1 < MIN_THROTTLE1 || throttle1 > MAX_THROTTLE1)
        *errMsg += 0x0001;              // throttle 1 out of range err msg
    
    if(throttle2 < MIN_THROTTLE2 || throttle2 > MAX_THROTTLE2)
        *errMsg += 0x0002;              // throttle 2 out of range err msg
    
    if (brake1 < MIN_BRAKE1)
        *errMsg += 0x0004;              // brake 1 out of range err msg
    
    if (brake2 < MIN_COUNT)
        *errMsg += 0x0008;              // brake 2 out of range err msg
    
    if (steering < STEER_LEFT || steering > STEER_RIGHT)
        *errMsg += 0x0010;              // steering out of range err msg
}

/*******************************************************************************
* Function Name: setCal(void)
********************************************************************************
*
* Summary:
*  Pulls calibrated value from EEPROM and stores in variable for use when
*  checking for errors.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/

void setCal(void)
{
    uint8 byteCount = 0;
    reg8* regPointer = (reg8*)CYDEV_EE_BASE;           //pointer pointing to base of EEPROM (row 1)    
    
    MIN_THROTTLE1 = concantenate(regPointer, &byteCount);
    MIN_THROTTLE2 = concantenate(regPointer, &byteCount);
    MAX_THROTTLE1 = concantenate(regPointer, &byteCount);
    MAX_THROTTLE2 = concantenate(regPointer, &byteCount);
    MIN_BRAKE1 = concantenate(regPointer+16, &byteCount);
    MIN_BRAKE2 = concantenate(regPointer+16, &byteCount);
    STEER_LEFT = concantenate(regPointer+16, &byteCount);
    STEER_RIGHT = concantenate(regPointer+16, &byteCount);
    
    byteCount = 0;
}

/*******************************************************************************
* Function Name: concantenate(uint8* byteCount)
********************************************************************************
*
* Summary:
*  Concantenate the calibrated values stored in EEPROM. Each EEPROM cell contains
*  a digit corresponding to a sensor value depending on its position within EEPROM
*  Used in setCal. 
*
* Parameters:
*  regPointer: base address of EEPROM
*  byteCount: pointer to byteCount in setCal; keeps track of position in EEPROM
*
* Return:
*  temp: sensor count value
*
*******************************************************************************/

uint16 concantenate(reg8* regPointer, uint8* byteCount)
{
    uint8 i;
    uint16 temp = 0; 
           
    if(*byteCount == 16)             //if byteCount = 16 then row one is finished reading from
        *byteCount = 0;          //reset byteCount 
    
    for(i = 0; i < 4; i++)
    {   
        temp = temp * 10;
        temp = temp + regPointer[*byteCount];
        *byteCount = *byteCount + 1;
    }
    
    return temp;
}
/* [] END OF FILE */