/* ========================================
 *
 * Copyright (c) Team Entropy 2020 - Public Release
 * License: GNU GPL v3
 *
 * @file    main.c
 * @date    1 Sep 2020 / Public Release Date: 7 Nov 2020
 * @version 3.4.0
 * @brief   Entry point for the Safe Signal Band
 * @author  David Pescariu
 * @github  https://github.com/entropy-dpit/Safe-Signal-Band
 *
 * ========================================
 * 
 * Special thanks:
 *      - Bosch Engineering Center Cluj, especially
 *          George Mois
 *          Vlad Stroian
 *         > Who helped us during the development proccess
 *
 *      - Academia DPIT for giving us the chance to participate in this contest
 *
 * ========================================
 *
 *  Tested on CY8C4247LQ1-BL483 | PSoC Creator 4.3 (4.3.0.1445)
 * 
 *  Running on CYBLE-222014 (Button mod required!)
 *
 * ========================================
*/

/*******************************************************************************
* Headers / Libs
*******************************************************************************/
#include "project.h"
#include "ble_func.h"
#include "button_func.h"

/*******************************************************************************
* Main Function
*******************************************************************************/
int main(void)
{
    /* Initialize all the neccesarry components */
    InitializeSystem();
    
    /* Enable global interrupts */
    CyGlobalIntEnable;

    /* Reset Button State */
    ResetCounter(ALL_BUTTONS);
    
    /* Start listening for interrupts */
    Alert_Interrupt_StartEx(Alert_Interrupt_Handler);

    for(;;)
    {
        /* Call service all BLE Stack Events */
        CyBle_ProcessEvents();
        
        /* Update the broadcasted packet */
        DynamicADVPayloadUpdate();
        
        /* Enter lowest possible power mode */
        EnterLowPowerMode();
    }
}

/* [] END OF FILE */
