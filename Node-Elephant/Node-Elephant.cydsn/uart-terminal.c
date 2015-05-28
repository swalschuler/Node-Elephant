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

#include "uart-terminal.h"

static char[][] commandTable;


void terminal_init() {

}

/****************************************************************************
* Function Name: terminal_echo(char serial_in[], uint8* track)
*****************************************************************************
*
* Summary:
*  Echos back to the terminal the current key stroke. If enter key is pressed,
*  this function will call terminal_parse() to get the inputed command
*
* Parameters:
*  serial_in[]: array to store the inputs, array of chars
*  track: pointer to track which keeps track of the number of inputs i.e. chars
*
* Return:
*  None.
*
****************************************************************************/
void terminal_echo(char serial_in[], uint8* track)
{
    uint8 buffer = 0;

    buffer = 0;

    if(USBUART_IsConfigurationChanged() != 0u) /* Host could send double SET_INTERFACE request */
    {
        if(USBUART_GetConfiguration() != 0u)   /* Init IN endpoints when device configured */
        {
            /* Enumeration is done, enable OUT endpoint for receive data from Host */
            USBUART_CDC_Init();
        }
    }
    if(USBUART_GetConfiguration() != 0u)    /* Service USB CDC when device configured */
    {
        if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
        {
            buffer = USBUART_GetChar();           /* Read received data and re-enable OUT endpoint */
            if(buffer != 0)
            {
                serial_in[*track] = (char)buffer;
                while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */
                USBUART_PutChar((char)buffer);       /* Send data back to PC */

                if(buffer == 127)           // checks for backspace = 127
                {
                    if(track != 0)          // subtract only if not at the front of the array
                        (*track)--;
                }
                else
                    (*track)++;

                if((char)buffer == '\r')        // if enter key was pressed
                {
                    serial_in[*track-1] = '\0';
                    terminal_parse(serial_in);
                    *track = 0;
                }
            }
        }
    }
}


/****************************************************************************
* Function Name: terminal_parse(char serial_in[])
*****************************************************************************
*
* Summary:
*  Called when the user press enter. Takes the serial_in[] array and compares
*  it with hard coded commands.
*
* Parameters:
*  serial_in[]: array of inputs
*
* Return:
*  None.
*
****************************************************************************/
void terminal_parse(char serial_in[])
{
    char* helpString = "\r\n1. Help\r\n2. Print ADC\r\n        Press ESC to stop conversions\r\n3. Something3\n\r";         //menu
    double volts;           //used to store adc value
    char adc_val[BUFFER_LEN];       //used to print adc value
    uint8 buffer;

    if(stricmp(serial_in, "help") == 0 || stricmp(serial_in, "1") == 0)
    {
        while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */
        USBUART_PutString(helpString);       /* Send data to PC */
    }
    else if(stricmp(serial_in, "Print ADC") == 0 || stricmp(serial_in, "2") == 0)
    {
        for(;;)     //keep printing conversions
        {
            if(ADC_SAR_IsEndConversion(ADC_SAR_WAIT_FOR_RESULT))        //if ADC conversion is done
            {
                volts = ADC_SAR_CountsTo_Volts(ADC_SAR_GetResult16(0));
                sprintf(adc_val, "\n\r%0.4fv, %d", volts, ADC_SAR_GetResult16(0));            //Makes floating point to acii
                LCD_Position(0,0);
                LCD_PrintString(adc_val);
                while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */
                USBUART_PutString(adc_val);       /* Send data back to PC */
            }

            if(USBUART_DataIsReady() != 0u)               /* Check for input data from PC */
            {
                buffer = USBUART_GetChar();

                if (buffer == 27)               //if escaped pressed then exit printing
                {
                    while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */
                    USBUART_PutString("\n\rStopped\n\r");       /* Send data back to PC */
                    break;
                }
            }
        }
    }
    else
    {
        while(USBUART_CDCIsReady() == 0u);    /* Wait till component is ready to send more data to the PC */
        USBUART_PutString("\n\rNot a recognized command, please use 'help' to see commands.\n\r");       /* Send data to PC */
    }
}

/* [] END OF FILE */
