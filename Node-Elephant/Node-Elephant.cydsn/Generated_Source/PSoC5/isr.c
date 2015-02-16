/*******************************************************************************
* File Name: isr.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <isr.h>

#if !defined(isr__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START isr_intc` */
#include <project.h>
#include "calibrate.h"
//volatile uint8 toggle_flag = 0;
volatile uint8_t Tx0_Throttle[8];       //transmission data for throttle one and two
volatile uint8_t Tx1_BSE[8];            //transmission data for brake 1, brake 2, steering, and error
extern volatile uint32 throttle1, throttle2, brake1, brake2, steering;            //variables for sending average to can
extern volatile uint16 buffSize;
uint16 avgVoltT1, avgVoltT2, avgVoltB1, avgVoltB2, avgVoltST, temp;       //average voltages 
    
/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: isr_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_Start(void)
{
    /* For all we know the interrupt is active. */
    isr_Disable();

    /* Set the ISR to point to the isr Interrupt. */
    isr_SetVector(&isr_Interrupt);

    /* Set the priority. */
    isr_SetPriority((uint8)isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_Enable();
}


/*******************************************************************************
* Function Name: isr_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void isr_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    isr_Disable();

    /* Set the ISR to point to the isr Interrupt. */
    isr_SetVector(address);

    /* Set the priority. */
    isr_SetPriority((uint8)isr_INTC_PRIOR_NUMBER);

    /* Enable it. */
    isr_Enable();
}


/*******************************************************************************
* Function Name: isr_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void isr_Stop(void)
{
    /* Disable this interrupt. */
    isr_Disable();

    /* Set the ISR to point to the passive one. */
    isr_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: isr_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for isr.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(isr_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START isr_Interrupt` */
    
    Timer_ReadStatusRegister();         //reads from status register to clear interrupt

//    LCD_Position(0,0);
//    LCD_PrintU32Number(throttle1);    
//    LCD_Position(1,6);
//    LCD_PrintNumber(buffSize); 
//    LCD_Position(1,0);
//    LCD_PrintNumber(throttle1/buffSize); 
    
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
    
//    LCD_Position(0,0);
//    LCD_PrintU32Number(avgVoltT1);   
    
    throttle1 = 0;            //reset variables 
    throttle2 = 0;
    brake1 = 0;
    brake2 = 0;
    steering = 0;
    buffSize = 0;
    
    /* `#END` */
}


/*******************************************************************************
* Function Name: isr_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling isr_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use isr_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void isr_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)isr__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: isr_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress isr_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)isr__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: isr_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling isr_Start
*   or isr_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   isr_Start or isr_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void isr_SetPriority(uint8 priority)
{
    *isr_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: isr_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 isr_GetPriority(void)
{
    uint8 priority;


    priority = *isr_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: isr_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_Enable(void)
{
    /* Enable the general interrupt. */
    *isr_INTC_SET_EN = isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 isr_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*isr_INTC_SET_EN & (uint32)isr__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: isr_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_Disable(void)
{
    /* Disable the general interrupt. */
    *isr_INTC_CLR_EN = isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_SetPending(void)
{
    *isr_INTC_SET_PD = isr__INTC_MASK;
}


/*******************************************************************************
* Function Name: isr_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void isr_ClearPending(void)
{
    *isr_INTC_CLR_PD = isr__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
