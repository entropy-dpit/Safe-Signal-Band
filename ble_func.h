/* ========================================
 *
 * Copyright (c) Team Entropy 2020
 * All Rights Reserved
 *
 * @file    ble_func.c
 * @date    1 Sep 2020
 * @version 3.4.0
 * @brief   Header for ble_func.c
 * @author  David Pescariu
 *
 * ========================================
*/

/* Guard: */
#ifndef BLE_HEADER
#define BLE_HEADER
    
/*******************************************************************************
* Headers / Libs
*******************************************************************************/
#include <project.h>
    
/*******************************************************************************
* Extern and constants required for dynamic ADV payload update
*******************************************************************************/
extern CYBLE_GAPP_DISC_MODE_INFO_T cyBle_discoveryModeInfo;
    
/* ADV payload data structure */    
#define advPayload              (cyBle_discoveryModeInfo.advData->advData) 

/* Indexes of the dynamic data contained in the ADV packet */
#define MFC_DATA_INDEX          (25u)   /* Depends on the packet size! */
    
/* How many times to broadcast the status before reseting */
#define BROADCAST_S             (5u)

    
/*******************************************************************************
* @brief This routine initializes all the componnets and firmware state.
* 
* @param None
*
* @returns None
*******************************************************************************/
void InitializeSystem(void);

/*******************************************************************************
* @brief This routine enters low power mode when it can.
* 
* @param None
*
* @returns None
*******************************************************************************/
void EnterLowPowerMode(void);


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
* @params None
*
* @returns None
*******************************************************************************/
void DynamicADVPayloadUpdate(void);

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
void StackEventHandler(uint32 event, void *eventParam);

#endif

/* [] END OF FILE */
