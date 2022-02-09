#ifndef USERCONFIG_H_
#define USERCONFIG_H_

// ****************************************************************************************
// ******************************* DO NOT CHANGE THIS *************************************
#define SW_CTRL_TYPE1           0
#define SW_CTRL_TYPE2           1

// Type of Input
#define IP_NO_PULL              1
#define IP_PULLUP               0

// Type of Output
#define ACTIVE_HIGH             1
#define ACTIVE_LOW              0

// ****************************************************************************************

/*  Device Specific Settings
 *  DEVICE_BLE_NAME_ONE : Device will advertise by this name in Switch Control - Type1
 *  DEVICE_BLE_NAME_TWO : Device will advertise by this name in Switch Control - Type2
 *  DEVICE_MANUFACTURER : Manufacturere name in BLE device properties
 *  DEVICE_MODEL_NAME   : Modem name in BLE device properties
 *  
 *  note - Write string between double quotes
 */
#define DEVICE_BLE_NAME_ONE     "Switch_DeviceOne"
#define DEVICE_BLE_NAME_TWO     "Switch_DeviceTwo"
#define DEVICE_MANUFACTURER     "AceCentre"
#define DEVICE_MODEL_NAME       "SW-HID-v1"


// Define board type. If Feather & Uses neopixel - Mark with 1. If dotstar mark with 0

#define USESNEOPIXEL      true


/*  Default mode for the device : Switch Control Type1/Type2
 *  
 *  For Type1 Mode : SW_CTRL_TYPE1
 *  For Type2 Mode : SW_CTRL_TYPE2
 */
#define DEFAULT_MODE_OF_DEVICE                SW_CTRL_TYPE1

/*  TYPE 1: Switch Control Commands for each keys
 *  
 *  Write HID Key name
 */
#define TYPE1_KEY1_CMD            HID_KEY_RETURN
#define TYPE1_KEY2_CMD            HID_KEY_SPACE      

/*  TYPE 2: Switch Control Commands for each keys
 *  
 *  Write HID Key name
 */
#define TYPE2_KEY1_CMD            HID_KEY_BACKSPACE
#define TYPE2_KEY2_CMD            HID_KEY_SHIFT_LEFT

/*  Maximum allowable BLE connections swaps
 *  
 *  note : Must be <= 5
 */ 
#define MAXIMUM_SWAP_CONNECTIONS       3                // v2.0

/*  Define timeout for BLE connection swapping
 *  By pressing a switch to swap BLE connection, if device is not connected to another PC till this timeout,
 *  then device will reconnect with older PC.
 *  
 *  note : Define timeout in milliseconds
 */
#define LAST_CONNECTION_CHECK_TIMEOUT          30000

/*  Serial Debug Enable/Disable
 *  1 - Enable / 0 - Disable
 *  
 *  By enabling device will send debug messages on USB port via serial. (Baudrate - 115200)
 */
#define SERIAL_DEBUG_EN                1

/*  Type of Input for Switch Control Input Keys
 *   
 *  Internal Pull Up: IP_PULLUP
 *  No Pull Up      : IP_NO_PULL
 */
#define KEY_IP_TYPE           IP_PULLUP

/*  Type of Output for Buzzer
 *   
 *  Active Low  : ACTIVE_LOW
 *  Active High : ACTIVE_HIGH
 */
// Buzzer Pin Type
#define BUZZER_TYPE           ACTIVE_LOW


#endif
