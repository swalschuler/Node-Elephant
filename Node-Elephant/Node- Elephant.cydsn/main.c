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

uint8_t Rx0_Data[8];
uint8_t Tx0_Data[8];
uint8_t Txmitted;
uint8_t Rxeved;

int main()
{
    CAN_1_Init();
    CAN_1_Start();
    CAN_1_GlobalIntEnable();
    CyGlobalIntEnable;
    LCD_Start();
    LCD_Position(0u,0u);
    LCD_PrintString("Hello");
    Pin_1_Write(1);
    CyDelay(500);
    Pin_1_Write(0);
    CyDelay(500);
    LCD_ClearDisplay();
    Tx0_Data[0] = 1;
    Txmitted = 0;
    Rxeved = 0;
    
    /*Code for Sending Board*/
    for(;;)
    {
        CAN_1_SendMsg0();
        Pin_1_Write(Txmitted);
        CyDelay(500);
        Txmitted = 0;
        Pin_1_Write(Txmitted);
        LCD_ClearDisplay();
        LCD_Position(0u,0u);
        LCD_PrintString("Sending.....");
        CyDelay(500);
        LCD_ClearDisplay();
        LCD_Position(0u,0u);
        Tx0_Data[0] += 1;
        LCD_PrintNumber(Tx0_Data[0]);
                
    }
    

    
    /*Code for Receiving Board*/    
    /*for(;;)
    {    
        LCD_PrintString("Byte 0 Value");
        LCD_Position(1u,0u);
        LCD_PrintNumber(Rx0_Data[0]);
        Pin_1_Write(Rxeved);
        CyDelay(500);
        Rxeved = 0;
        Pin_1_Write(Rxeved);
        CyDelay(500);
        LCD_ClearDisplay();
        
    }
    */
}

/* [] END OF FILE */
