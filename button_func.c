/* ========================================
 *
 * Copyright (c) prisma.ai
 * All Rights Reserved
 *
 * @file    button_func.c
 * @date    1 Sep 2020
 * @version 3.4.0
 * @brief   Contains all functions related to presses / interrupts / counters
 * @author  David Pescariu
 *
 * ========================================
*/

/*******************************************************************************
* Headers / Libs
*******************************************************************************/
#include "button_func.h"

/*******************************************************************************
* Variables that stores the number of presses
*******************************************************************************/
volatile uint8 press_counter;
volatile uint8 right_presses;

/*******************************************************************************
* @brief Setup Interrupt Handler for the Alert Button
*
* Handles button presses
*
* @param Alert_Interrupt_Handler:   Interrupt Handler Address    
*
* @returns None    
*******************************************************************************/   
CY_ISR(Alert_Interrupt_Handler) {
    /* Wait PRESS_DELAY to make sure if the user wanted to press both 
    buttons we register it */
    CyDelay(PRESS_DELAY);
    
    /* Clear interrupt and get what button was pressed */
    volatile uint8 mask = Alert_Button_ClearInterrupt();
    
    if (Alert_Button_Read() == BUTTON_PRESSED) {
        // Both buttons were pressed
        HandleAllPressed();
    } else {
        switch(mask) {
            case 0x01:
                // First (left?) button pressed
                HandleLeftPressed();
                break;
            case 0x02:
                // Second (right?) button pressed
                HandleRightPressed();
                break;
            default:
                break;
        }
    }
}

/*******************************************************************************
* @brief This routine handles the press of all the band's buttons.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleAllPressed(void) {
    /* If all (atm 2) buttons are pressed increase the press counter to send*/
    AddButtonPress(BOTH_BUTTONS);
}


/*******************************************************************************
* @brief This routine handles the press of the band's left button.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleLeftPressed(void) {
    /* Modify to add an action for when only the left button is pressed */
}


/*******************************************************************************
* @brief This routine handles the press of the band's right button.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleRightPressed(void) {
    AddButtonPress(RIGHT_BUTTON);
}

/*******************************************************************************
* @brief This routine resets the specified counter to 0 = not pressed.
* 
* @param uint8 whatCounter:        The counter to reset (ie: ALL_BUTTONS or 99)
*
* @returns None
*******************************************************************************/
void ResetCounter(uint8 whatCounter) {
    switch(whatCounter){
        case BOTH_BUTTONS:
            press_counter = 0;
            break;
        case RIGHT_BUTTON:
            right_presses = 0;
            break;
        case LEFT_BUTTON:
            break;
        case ALL_BUTTONS:
            press_counter = 0;
            right_presses = 0;
            break;
        default:
            break;
    }    
}


/*******************************************************************************
* @brief This routine increments the state of the button with one.
* 
* NOTE: It will not increase the counter if the respective counter reached it's
*   threshold/max value.
* 
* @param uint8 whatCounter:        The counter to add to (ie: LEFT_BUTTON or 2)
*
* @returns None
*******************************************************************************/
void AddButtonPress(uint8 whatCounter) {
    switch(whatCounter){
        case BOTH_BUTTONS:
            if(right_presses < MAX_AVAILABLE_PRESSES){
                ++press_counter;
            }
            break;
        case RIGHT_BUTTON:
            if(right_presses < PAIRING_MODE_PRESS_NO){
                ++right_presses;
            }
            break;
        case LEFT_BUTTON:
            break;
        default:
            break;
    }
}


/*******************************************************************************
* @brief This function returns the state of the press counter.
* 
* @param uint8 whatCounter:        The counter (ie: BOTH_BUTTONS or 0)
*
* @returns uint8 u_button_state:    The State of the button (0/1/2/4/etc...)
*******************************************************************************/
uint8 GetCounterStatus(uint8 whatCounter) {
    switch(whatCounter){
        case BOTH_BUTTONS:
            return press_counter;
            break;
        case RIGHT_BUTTON:
            return right_presses;
            break;
        case LEFT_BUTTON:
            return (int) NULL;
            break;
        default:
            return (int) NULL;
            break;
    }
}

/* [] END OF FILE */
