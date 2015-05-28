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
#include <project.h>
#include "uart-terminal.h"

int main()
{
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    LCD_Start();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    USBUART_Start(0u, USBUART_3V_OPERATION);   
    CyGlobalIntEnable;  
    
    uint8 track = 0;
    char serial_in[BUFFER_LEN];
    
    for(;;)
    {
        /* Place your application code here. */
        terminal_echo(serial_in, &track);
    }
}

/* [] END OF FILE */
