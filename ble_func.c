/* ========================================
 *
 * Copyright (c) prisma.ai
 * All Rights Reserved
 *
 * @file    ble_func.c
 * @date    1 Sep 2020
 * @version 3.4.0
 * @brief   Contains all functions related to BLE / Advertising
 * @author  David Pescariu
 *
 * ========================================
*/

/*******************************************************************************
* Headers / Libs
*******************************************************************************/
#include "ble_func.h"
#include "button_func.h"

/*******************************************************************************
* Global variables
*******************************************************************************/
uint8 count_broadcasts = 0;

/*******************************************************************************
* @brief This routine initializes all the componnets and firmware state.
* 
* @param None
*
* @returns None
*******************************************************************************/
void InitializeSystem(void)
{
    CYBLE_API_RESULT_T apiResult;

    CyGlobalIntEnable;  /* Enable global interrupts */

    apiResult = CyBle_Start(StackEventHandler); /* Init the BLE stack and register an application callback */

    if(apiResult != CYBLE_ERROR_OK)
    {
        CYASSERT(0);  /* BLE stack initialization failed*/
    }
    
    /* Set XTAL divider to 3MHz mode */
    CySysClkWriteEcoDiv(CY_SYS_CLK_ECO_DIV8); 
    
    /* ILO is no longer required, shut it down */
    CySysClkIloStop();
}


/*******************************************************************************
* @brief This routine enters low power mode when it can.
* 
* @param None
*
* @returns None
*******************************************************************************/
void EnterLowPowerMode(void)
{
    CYBLE_BLESS_STATE_T blessState;
    uint8 intrStatus;
    
    /* Configure BLESS in Deep-Sleep mode */
    CyBle_EnterLPM(CYBLE_BLESS_DEEPSLEEP);
    
    /* Prevent interrupts while entering system low power modes */
    intrStatus = CyEnterCriticalSection();
    
    /* Get the current state of BLESS block */
    blessState = CyBle_GetBleSsState();
    
    /* If BLESS is in Deep-Sleep mode or the XTAL oscillator is turning on, 
     * then PSoC 4 BLE can enter Deep-Sleep mode (1.3uA current consumption) */
    if(blessState == CYBLE_BLESS_STATE_ECO_ON || 
        blessState == CYBLE_BLESS_STATE_DEEPSLEEP)
    {
        CySysPmDeepSleep();
    }
    else if(blessState != CYBLE_BLESS_STATE_EVENT_CLOSE)
    {
        /* If BLESS is active, then configure PSoC 4 BLE system in 
         * Sleep mode (~1.6mA current consumption) */
        CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_ECO);
        CySysClkImoStop();
        CySysPmSleep();
        CySysClkImoStart();
        CySysClkWriteHfclkDirect(CY_SYS_CLK_HFCLK_IMO);
    }
    else
    {
        /* Keep trying to enter either Sleep or Deep-Sleep mode */    
    }
    
    CyExitCriticalSection(intrStatus);
}


/*******************************************************************************
* @brief This routine updates the BLE advertisment packet.
* 
*   We update the Manfc. Data (index MFC_DATA_INDEX) with the current status of 
* the button status / how many times was the button pressed
*   What each no. of presses mean:
*       0   => Not pressed / Reseted
*       1   => Pressed once = Feels Unsafe
*       2   => Pressed twiche = Configurable through the app
*       >=4 => Pressed 4 or more times = High Danger / Send 113 Alert
*
* @param None
*
* @returns None
*******************************************************************************/
void DynamicADVPayloadUpdate(void)
{
    if(CyBle_GetBleSsState() == CYBLE_BLESS_STATE_EVENT_CLOSE)
    {
        /*****
        * advPayloadData:
        *   if right_counter = PAIRING_MODE_PRESS_NO, broadcast 255, in app it's
        *  recognized as "pairing mode"
        *   else broadcast the counter for pressing both buttons
        *****/
        uint8 advPayloadData = 0x00;
        
        uint8 all_counter_status = GetCounterStatus(BOTH_BUTTONS);
        uint8 right_counter_status = GetCounterStatus(RIGHT_BUTTON);
        
        /*  After BROADCAST_S broadcasts (ignoring the broadcasts where the 
         * state is 0) clear the press counter (reset to 0) */
        if(all_counter_status != 0 || right_counter_status != 0) {
            ++count_broadcasts;
            if(count_broadcasts == BROADCAST_S) {
                count_broadcasts = 0;
                ResetCounter(ALL_BUTTONS); // Reset all counters
            }
            // Check for special cases ie, pairing mode
            if (right_counter_status == PAIRING_MODE_PRESS_NO){
                advPayloadData = 0xff;
            } else {
                // If not in pairing mode, send button status
                advPayloadData = all_counter_status;
            }
        }
        
        /* Set the payload with the button status */
        advPayload[MFC_DATA_INDEX] = advPayloadData;

        /* Set the ADV data and SCAN response data */
        CyBle_GapUpdateAdvData(
            cyBle_discoveryModeInfo.advData, 
            cyBle_discoveryModeInfo.scanRspData);
    }
}

/*******************************************************************************
* @brief Event callback to receive events from the BLE Component.
* 
* @param    uint32 event:       Event from the CYBLE component
* @param    void* eventParam:   A structure instance for corresponding event 
*                              type. The list of event structure is described 
*                              in the component datasheet.
*
* @returns None
*******************************************************************************/
void StackEventHandler(uint32 event, void *eventParam)
{
    switch(event)
    {
        /* Mandatory events to be handled by Find Me Target design */
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            break;

        default:
            break;
    }
}

/* [] END OF FILE */
