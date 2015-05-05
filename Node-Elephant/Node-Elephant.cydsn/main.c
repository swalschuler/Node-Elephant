/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 *
 * ========================================
*/

#include <project.h>
#include <stdio.h>
#include <stdbool.h>
#include "pedal_control.h"
#include "pedal_state.h"
#include "CAN_invertor.h"

pedal_state pedal_node_state = pedal_state_driving;
volatile bool should_calibrate = false;

CY_ISR(isr_calibration_handler)
{
    if (pedal_node_state != pedal_state_calibrating)
    {
        should_calibrate = true;
    }
    isr_calibration_Disable();
    isr_start_Disable();
    Button_ClearInterrupt();
}

CY_ISR(isr_start_handler)
{
    if (pedal_node_state == pedal_state_neutral)
    {
        pedal_fetch_data();
        if (pedal_is_brake_pressed())
        {
            pedal_node_state = pedal_state_driving;
        }
        else
        {
            //If brake is not pressed, simply return
            isr_start_Disable();
            Start_button_ClearInterrupt();
            isr_start_Enable();
            return;
        }
    }
    else if (pedal_node_state == pedal_state_driving)
    {
        pedal_node_state = pedal_state_neutral;
    }
    isr_calibration_Disable();
    isr_start_Disable();
    Start_button_ClearInterrupt();
}

int main()
{
     pedal_node_state = pedal_state_neutral;
    LCD_Start();
    CAN_invertor_init();
    ADC_SAR_Start();
    ADC_SAR_StartConvert();
    EEPROM_Start();
    //isr_Start();
    //Timer_Start();
    CAN_Init();
    CAN_Start();
    isr_calibration_StartEx(&isr_calibration_handler);
    isr_start_StartEx(&isr_start_handler);
    
    CyGlobalIntEnable;          //enable global interrupts
    pedal_restore_calibration_data();               //set min and max values
    pedal_set_CAN();
    EEPROM_ERROR_LED_Write(0);
    should_calibrate = false;

    for(;;)
    {
        if (pedal_node_state == pedal_state_neutral)
        {
            if (should_calibrate)
            {
                should_calibrate = false;
                pedal_node_state = pedal_state_calibrating;
            }
        }
        else
        {
            should_calibrate = false;
        }

        uint8_t out_of_range_flag;
        double brake_percent = 0, throttle_percent = 0;
        double brake_percent_diff = 0, throttle_percent_diff = 0;
        uint8_t torque_plausible_flag;
        uint8_t brake_plausible_flag;
        pedal_fetch_data();
        switch (pedal_node_state)
        {
            case pedal_state_neutral:
                LCD_ClearDisplay();
                LCD_Position(0,0);
                LCD_PrintString("NEUTRAL");
                break;
            case pedal_state_driving:
                LCD_ClearDisplay();
                LCD_Position(0,0);

                out_of_range_flag = pedal_get_out_of_range_flag();
                if (out_of_range_flag != 0)
                {
                    pedal_node_state = pedal_state_out_of_range;
                    break;
                }

                torque_plausible_flag = pedal_is_pedal_reading_matched(&brake_percent_diff, &throttle_percent_diff);
                if (torque_plausible_flag != 0)
                {
                    pedal_node_state = pedal_state_discrepency;
                    break;
                }

                brake_plausible_flag = pedal_is_brake_plausible(&brake_percent, &throttle_percent);
                if (brake_plausible_flag != 0)
                {
                    pedal_node_state = pedal_state_implausible;
                    break;
                }

                break;

            case pedal_state_calibrating:
                //clock_StopBlock();      //stop clock to disable interrupt
                pedal_calibrate();
                LCD_ClearDisplay();
                //isr_ClearPending();
                //clock_Start();
                isr_calibration_Enable();
                isr_start_Enable();
                pedal_node_state = pedal_state_neutral;
                break;

            case pedal_state_out_of_range:
                LCD_ClearDisplay();
                LCD_Position(0,0);
                LCD_PrintString("Pedal out of");
                LCD_Position(1,0);
                LCD_PrintString("range");

                out_of_range_flag = pedal_get_out_of_range_flag();
                if (out_of_range_flag == 0)
                {
                    pedal_node_state = pedal_state_driving;
                }
                break;

            case pedal_state_discrepency:
                LCD_ClearDisplay();
                LCD_Position(0,0);
                LCD_PrintString("Pedal discrepency");
                torque_plausible_flag = pedal_is_pedal_reading_matched(&brake_percent_diff, &throttle_percent_diff);
                if (torque_plausible_flag == 0)
                {
                    pedal_node_state = pedal_state_driving;
                }
                break;

            case pedal_state_implausible:
                LCD_ClearDisplay();
                LCD_Position(0,0);
                LCD_PrintString("Pedal implausible");
                brake_plausible_flag = pedal_is_brake_plausible(&brake_percent, &throttle_percent);
                if (throttle_percent < PEDAL_BRAKE_IMPLAUSIBLE_EXIT_THROTTLE_PERCENT)
                {
                    pedal_node_state = pedal_state_driving;
                }
                break;
        }

        CyDelay(500);
    }

    return 0;
}


/* [] END OF FILE */
