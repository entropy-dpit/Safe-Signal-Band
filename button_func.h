/* ========================================
 *
 * Copyright (c) Team Entropy 2020
 * All Rights Reserved
 *
 * @file    button_func.c
 * @date    1 Sep 2020
 * @version 3.4.0
 * @brief   Header for button_func.c
 * @author  David Pescariu
 *
 * ========================================
*/

/* Guard: */
#ifndef BTN_HEADER
#define BTN_HEADER
    
   
/*******************************************************************************
* Headers / Libs
*******************************************************************************/
#include <project.h>

/*******************************************************************************
* Constants
*******************************************************************************/    
#define BUTTON_PRESSED              (1u) // Change to 0 if pulled low
  
#define PRESS_DELAY                 (50) // In ms
#define MAX_AVAILABLE_PRESSES       (10) // Do not increase (both)counters beyond this threshold
#define PAIRING_MODE_PRESS_NO       (5)  // How many times you need to press to enter "pairing mode" 

#define BOTH_BUTTONS                (0)  // Used for setting/getting counters
#define RIGHT_BUTTON                (1)  // Used for setting/getting counters
#define LEFT_BUTTON                 (2)  // Used for setting/getting counters
#define ALL_BUTTONS                 (99) // Used for reseting ALL counters   

/*******************************************************************************
* @brief Setup Interrupt Handler for the Alert Button
*
* Handles button presses
*
* @param Alert_Interrupt_Handler:   Interrupt Handler Address    
*
* @returns None    
*******************************************************************************/    
CY_ISR(Alert_Interrupt_Handler);    


/*******************************************************************************
* @brief This routine handles the press of all the band's buttons.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleAllPressed(void);


/*******************************************************************************
* @brief This routine handles the press of the band's left button.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleLeftPressed(void);


/*******************************************************************************
* @brief This routine handles the press of the band's right button.
* 
* @param None
*
* @returns None
*******************************************************************************/
void HandleRightPressed(void);

/*******************************************************************************
* @brief This routine resets the specified counter to 0 = not pressed.
* 
* @param uint8 whatCounter:        The counter to reset (ie: ALL_BUTTONS or 99)
*
* @returns None
*******************************************************************************/
void ResetCounter(uint8 whatCounter);


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
void AddButtonPress(uint8 whatCounter);


/*******************************************************************************
* @brief This function returns the state of the press counter.
* 
* @param uint8 whatCounter:        The counter (ie: BOTH_BUTTONS or 0)
*
* @returns uint8 u_button_state:    The State of the button (0/1/2/4/etc...)
*******************************************************************************/
uint8 GetCounterStatus(uint8 whatCounter);

#endif

/* [] END OF FILE */
