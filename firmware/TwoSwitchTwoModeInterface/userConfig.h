#ifndef USERCONFIG_H_
#define USERCONFIG_H_

// ****************************************************************************************
// ******************************* DO NOT CHANGE THIS *************************************
#define SW_CTRL_TYPE1           0
#define SW_CTRL_TYPE2           1

// Type of Input
// If you are using external pull up then IP_NO_PULL should used
// And if you are not connecting any external pull up and want to use internal 
//   pull up then IP_PULLUP should used
#define IP_NO_PULL              1
#define IP_PULLUP               0

// Type of Output
// Buzzer. Active High: By setting 1 on pin buzzer will trigger. 
// Active Low: By setting 0 on pin buzzer will trigger
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
#define DEVICE_BLE_NAME_ONE     "SW_HID_ONE"
#define DEVICE_BLE_NAME_TWO     "SW_HID_TWO"
#define DEVICE_MANUFACTURER     "AceCentre"
#define DEVICE_MODEL_NAME       "SW-HID-v1"

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
#define TYPE1_KEY1_CMD            HID_KEY_ENTER
#define TYPE1_KEY2_CMD            HID_KEY_SPACE
#define TYPE1_KEY3_CMD            HID_KEY_BACKSPACE

/*  TYPE 2: Switch Control Commands for each keys
 *  
 *  Write HID Key name
 */
#define TYPE2_KEY1_CMD            HID_KEY_SPACE
#define TYPE2_KEY2_CMD            HID_KEY_SHIFT_LEFT
#define TYPE2_KEY3_CMD            HID_KEY_END

/*  Key 1 long press time for switching to another mode
 *  
 *  In milliseconds
 */
#define LONG_PRESS_KEY_TIME       4000

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
#define BUZZER_TYPE           ACTIVE_HIGH


#endif
