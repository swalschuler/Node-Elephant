/*******************************************************************************
* File Name: Brake_2.h  
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

#if !defined(CY_PINS_Brake_2_H) /* Pins Brake_2_H */
#define CY_PINS_Brake_2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Brake_2_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_5 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Brake_2__PORT == 15 && ((Brake_2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    Brake_2_Write(uint8 value) ;
void    Brake_2_SetDriveMode(uint8 mode) ;
uint8   Brake_2_ReadDataReg(void) ;
uint8   Brake_2_Read(void) ;
uint8   Brake_2_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define Brake_2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define Brake_2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define Brake_2_DM_RES_UP          PIN_DM_RES_UP
#define Brake_2_DM_RES_DWN         PIN_DM_RES_DWN
#define Brake_2_DM_OD_LO           PIN_DM_OD_LO
#define Brake_2_DM_OD_HI           PIN_DM_OD_HI
#define Brake_2_DM_STRONG          PIN_DM_STRONG
#define Brake_2_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define Brake_2_MASK               Brake_2__MASK
#define Brake_2_SHIFT              Brake_2__SHIFT
#define Brake_2_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Brake_2_PS                     (* (reg8 *) Brake_2__PS)
/* Data Register */
#define Brake_2_DR                     (* (reg8 *) Brake_2__DR)
/* Port Number */
#define Brake_2_PRT_NUM                (* (reg8 *) Brake_2__PRT) 
/* Connect to Analog Globals */                                                  
#define Brake_2_AG                     (* (reg8 *) Brake_2__AG)                       
/* Analog MUX bux enable */
#define Brake_2_AMUX                   (* (reg8 *) Brake_2__AMUX) 
/* Bidirectional Enable */                                                        
#define Brake_2_BIE                    (* (reg8 *) Brake_2__BIE)
/* Bit-mask for Aliased Register Access */
#define Brake_2_BIT_MASK               (* (reg8 *) Brake_2__BIT_MASK)
/* Bypass Enable */
#define Brake_2_BYP                    (* (reg8 *) Brake_2__BYP)
/* Port wide control signals */                                                   
#define Brake_2_CTL                    (* (reg8 *) Brake_2__CTL)
/* Drive Modes */
#define Brake_2_DM0                    (* (reg8 *) Brake_2__DM0) 
#define Brake_2_DM1                    (* (reg8 *) Brake_2__DM1)
#define Brake_2_DM2                    (* (reg8 *) Brake_2__DM2) 
/* Input Buffer Disable Override */
#define Brake_2_INP_DIS                (* (reg8 *) Brake_2__INP_DIS)
/* LCD Common or Segment Drive */
#define Brake_2_LCD_COM_SEG            (* (reg8 *) Brake_2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Brake_2_LCD_EN                 (* (reg8 *) Brake_2__LCD_EN)
/* Slew Rate Control */
#define Brake_2_SLW                    (* (reg8 *) Brake_2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Brake_2_PRTDSI__CAPS_SEL       (* (reg8 *) Brake_2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Brake_2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Brake_2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Brake_2_PRTDSI__OE_SEL0        (* (reg8 *) Brake_2__PRTDSI__OE_SEL0) 
#define Brake_2_PRTDSI__OE_SEL1        (* (reg8 *) Brake_2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Brake_2_PRTDSI__OUT_SEL0       (* (reg8 *) Brake_2__PRTDSI__OUT_SEL0) 
#define Brake_2_PRTDSI__OUT_SEL1       (* (reg8 *) Brake_2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Brake_2_PRTDSI__SYNC_OUT       (* (reg8 *) Brake_2__PRTDSI__SYNC_OUT) 


#if defined(Brake_2__INTSTAT)  /* Interrupt Registers */

    #define Brake_2_INTSTAT                (* (reg8 *) Brake_2__INTSTAT)
    #define Brake_2_SNAP                   (* (reg8 *) Brake_2__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Brake_2_H */


/* [] END OF FILE */
