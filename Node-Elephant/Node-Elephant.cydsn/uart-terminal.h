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
#ifndef UART_TERMINAL_H
#define UART_TERMINAL_H

#include <project.h>
#include <stdio.h>

#define BUFFER_LEN 64
#define TERMINAL_NODE_NAME "Pedal-Node"

typedef void (*func_ptr_t)( void );

void terminal_init();
void terminal_run();
void terminal_registerCommand(char command[], func_ptr_t routine);

#endif


/* [] END OF FILE */
