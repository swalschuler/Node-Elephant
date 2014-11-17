/*******************************************************************************
* File Name: tx_en.h  
* Version 2.5
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_tx_en_H) /* Pins tx_en_H */
#define CY_PINS_tx_en_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "tx_en_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_5 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 tx_en__PORT == 15 && ((tx_en__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    tx_en_Write(uint8 value) ;
void    tx_en_SetDriveMode(uint8 mode) ;
uint8   tx_en_ReadDataReg(void) ;
uint8   tx_en_Read(void) ;
uint8   tx_en_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define tx_en_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define tx_en_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define tx_en_DM_RES_UP          PIN_DM_RES_UP
#define tx_en_DM_RES_DWN         PIN_DM_RES_DWN
#define tx_en_DM_OD_LO           PIN_DM_OD_LO
#define tx_en_DM_OD_HI           PIN_DM_OD_HI
#define tx_en_DM_STRONG          PIN_DM_STRONG
#define tx_en_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define tx_en_MASK               tx_en__MASK
#define tx_en_SHIFT              tx_en__SHIFT
#define tx_en_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define tx_en_PS                     (* (reg8 *) tx_en__PS)
/* Data Register */
#define tx_en_DR                     (* (reg8 *) tx_en__DR)
/* Port Number */
#define tx_en_PRT_NUM                (* (reg8 *) tx_en__PRT) 
/* Connect to Analog Globals */                                                  
#define tx_en_AG                     (* (reg8 *) tx_en__AG)                       
/* Analog MUX bux enable */
#define tx_en_AMUX                   (* (reg8 *) tx_en__AMUX) 
/* Bidirectional Enable */                                                        
#define tx_en_BIE                    (* (reg8 *) tx_en__BIE)
/* Bit-mask for Aliased Register Access */
#define tx_en_BIT_MASK               (* (reg8 *) tx_en__BIT_MASK)
/* Bypass Enable */
#define tx_en_BYP                    (* (reg8 *) tx_en__BYP)
/* Port wide control signals */                                                   
#define tx_en_CTL                    (* (reg8 *) tx_en__CTL)
/* Drive Modes */
#define tx_en_DM0                    (* (reg8 *) tx_en__DM0) 
#define tx_en_DM1                    (* (reg8 *) tx_en__DM1)
#define tx_en_DM2                    (* (reg8 *) tx_en__DM2) 
/* Input Buffer Disable Override */
#define tx_en_INP_DIS                (* (reg8 *) tx_en__INP_DIS)
/* LCD Common or Segment Drive */
#define tx_en_LCD_COM_SEG            (* (reg8 *) tx_en__LCD_COM_SEG)
/* Enable Segment LCD */
#define tx_en_LCD_EN                 (* (reg8 *) tx_en__LCD_EN)
/* Slew Rate Control */
#define tx_en_SLW                    (* (reg8 *) tx_en__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define tx_en_PRTDSI__CAPS_SEL       (* (reg8 *) tx_en__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define tx_en_PRTDSI__DBL_SYNC_IN    (* (reg8 *) tx_en__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define tx_en_PRTDSI__OE_SEL0        (* (reg8 *) tx_en__PRTDSI__OE_SEL0) 
#define tx_en_PRTDSI__OE_SEL1        (* (reg8 *) tx_en__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define tx_en_PRTDSI__OUT_SEL0       (* (reg8 *) tx_en__PRTDSI__OUT_SEL0) 
#define tx_en_PRTDSI__OUT_SEL1       (* (reg8 *) tx_en__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define tx_en_PRTDSI__SYNC_OUT       (* (reg8 *) tx_en__PRTDSI__SYNC_OUT) 


#if defined(tx_en__INTSTAT)  /* Interrupt Registers */

    #define tx_en_INTSTAT                (* (reg8 *) tx_en__INTSTAT)
    #define tx_en_SNAP                   (* (reg8 *) tx_en__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_tx_en_H */


/* [] END OF FILE */
