/*******************************************************************************
* File Name: tx_en.c  
* Version 2.5
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "tx_en.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 tx_en__PORT == 15 && ((tx_en__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: tx_en_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void tx_en_Write(uint8 value) 
{
    uint8 staticBits = (tx_en_DR & (uint8)(~tx_en_MASK));
    tx_en_DR = staticBits | ((uint8)(value << tx_en_SHIFT) & tx_en_MASK);
}


/*******************************************************************************
* Function Name: tx_en_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  tx_en_DM_STRONG     Strong Drive 
*  tx_en_DM_OD_HI      Open Drain, Drives High 
*  tx_en_DM_OD_LO      Open Drain, Drives Low 
*  tx_en_DM_RES_UP     Resistive Pull Up 
*  tx_en_DM_RES_DWN    Resistive Pull Down 
*  tx_en_DM_RES_UPDWN  Resistive Pull Up/Down 
*  tx_en_DM_DIG_HIZ    High Impedance Digital 
*  tx_en_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void tx_en_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(tx_en_0, mode);
}


/*******************************************************************************
* Function Name: tx_en_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro tx_en_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 tx_en_Read(void) 
{
    return (tx_en_PS & tx_en_MASK) >> tx_en_SHIFT;
}


/*******************************************************************************
* Function Name: tx_en_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 tx_en_ReadDataReg(void) 
{
    return (tx_en_DR & tx_en_MASK) >> tx_en_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(tx_en_INTSTAT) 

    /*******************************************************************************
    * Function Name: tx_en_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 tx_en_ClearInterrupt(void) 
    {
        return (tx_en_INTSTAT & tx_en_MASK) >> tx_en_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
