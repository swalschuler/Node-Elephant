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

void terminal_init();
void terminal_echo(char serial_in[], uint8* track);
void terminal_parse(char serial_in[]);
void terminal_registerCommand(char command, void (*routine)());

#endif


/* [] END OF FILE */
