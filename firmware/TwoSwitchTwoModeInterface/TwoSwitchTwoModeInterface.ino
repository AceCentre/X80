// Header Files
#include <bluefruit.h>
#include <BLEConnection.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include "userPinMap.h"
#include "userConfig.h"

using namespace Adafruit_LittleFS_Namespace;

// Buzzer Pin
#if BUZZER_TYPE == ACTIVE_HIGH
  #define BUZZER_ON       digitalWrite(BUZZER, HIGH);
  #define BUZZER_OFF      digitalWrite(BUZZER, LOW);
#else
  #define BUZZER_ON       digitalWrite(BUZZER, LOW);
  #define BUZZER_OFF      digitalWrite(BUZZER, HIGH);
#endif

// Variable Declarations
BLEDis bledis;
BLEHidAdafruit blehid;
File dbFile(InternalFS);

const int BUTTON_ONE = KEY_ONE;
const int BUTTON_TWO = KEY_TWO;
const int BUTTON_THREE = KEY_THREE;
const int HARD_RESET_PIN = RESET_PIN;
int BUZZER = BUZZER_PIN;

volatile unsigned long currentMillis;
const char deviceBleName[] = DEVICE_BLE_NAME_ONE;
const char deviceBleName2[] = DEVICE_BLE_NAME_TWO;
const char deviceManuf[] = DEVICE_MANUFACTURER;
const char deviceModelName[] = DEVICE_MODEL_NAME;

volatile uint8_t keycheck, keyPressCnt;
volatile unsigned long lastScKeyCheckTicks;
volatile unsigned char currMode = DEFAULT_MODE_OF_DEVICE;
volatile uint8_t keycodeComboBuff[6];
const char dbFileName[] = "/database.txt";

// Functions Declarations
void startBleAdvertising(void);
void bleConnectCallback(uint16_t conn_handle);
void readDataFromFS(void);
void writeDataToFS(void);
void changeMacAddress(void);
void handleSwitchControlKeypress(void);

// Functions Definations
void setup()
{
  // Begin Serial
  #if SERIAL_DEBUG_EN
  Serial.begin(115200);
  //while ( !Serial ) delay(10);                            // temp
  #endif

  // Configure GPIOs  
 
    pinMode(BUTTON_ONE, INPUT);
    pinMode(BUTTON_TWO, INPUT_PULLUP);
    pinMode(BUTTON_THREE, INPUT_PULLUP);
  
  pinMode(BUZZER,OUTPUT);
  BUZZER_OFF;

  pinMode(HARD_RESET_PIN, OUTPUT);
  digitalWrite(HARD_RESET_PIN, HIGH);
  
  // Initialize Internal File System
  InternalFS.begin();

  // Read Data from Internal File
  readDataFromFS();

  // Configure Bluefruit Parameters  
  Bluefruit.begin();  
  Bluefruit.setTxPower(4);
  if(currMode == SW_CTRL_TYPE1)
  {
    Bluefruit.setName(deviceBleName);
  }
  else
  {
    Bluefruit.setName(deviceBleName2);
  }  
  Bluefruit.Periph.setConnectCallback(bleConnectCallback);
  bledis.setManufacturer(deviceManuf);
  bledis.setModel(deviceModelName);

  // Set Device MAC Address
  changeMacAddress();

  // Begin BLE Services
  bledis.begin();
  
  // Begin BLE-HID Service
  blehid.begin(); 

  // Start BLE Advertising
  startBleAdvertising();
}

void loop()
{
  currentMillis = millis();

  if(currentMillis - lastScKeyCheckTicks >= 100)
  {
    lastScKeyCheckTicks = currentMillis;
    handleSwitchControlKeypress();
  }
}

void startBleAdvertising(void)
{  
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();
  Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  Bluefruit.Advertising.addService(blehid);
  Bluefruit.Advertising.addName();           
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);
  Bluefruit.Advertising.setFastTimeout(30);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.start();
}

void bleConnectCallback(uint16_t conn_handle)
{
    
}

void readDataFromFS(void)
{
  dbFile.open(dbFileName, FILE_O_READ);
  if(dbFile)
  {
    //#if SERIAL_DEBUG_EN
    //Serial.println("DB file open");
    //#endif

    uint32_t readlen;
    char buff[182] = { 0 };
    uint8_t str_valid = 0;
    
    readlen = dbFile.read(buff, sizeof(buff));

    buff[readlen] = 0;
    /*#if SERIAL_DEBUG_EN
    Serial.println(buff);
    #endif*/

    if(buff[0] == '@')
    {      
      for(uint8_t i = 0; i < sizeof(buff); i++)
      {
        if(buff[i] == '#')
        {
          str_valid = 1;          
          break;
        }
      }
    }

    if(str_valid)
    {
      //#if SERIAL_DEBUG_EN
      //Serial.println("String is valid");
      //#endif

      uint16_t tmp_char = 0;      
      
      tmp_char = atoi(&buff[1]);
      if(tmp_char == SW_CTRL_TYPE1 || tmp_char == SW_CTRL_TYPE2)
      {
        currMode = tmp_char;
      }
      else
      {
        currMode = DEFAULT_MODE_OF_DEVICE;
      }
    }
    else
    {
      #if SERIAL_DEBUG_EN
      Serial.println("String is not valid");
      #endif
      writeDataToFS();
    }
    dbFile.close();
  }
  else
  {
    #if SERIAL_DEBUG_EN
    Serial.println("DB file not present");
    #endif
    writeDataToFS();    
  }
}

void writeDataToFS(void)
{
  char buff[182] = { 0 };
  memset(buff, '\0', sizeof(buff));
  sprintf(buff, "@%u#", currMode);

  if(dbFile.open(dbFileName, FILE_O_WRITE))
  {
    dbFile.seek(0);
    dbFile.write(buff, strlen(buff));
    dbFile.close();
    #if SERIAL_DEBUG_EN
    Serial.println("Data written in DB file");
    #endif
  }
  else
  {
    #if SERIAL_DEBUG_EN
    Serial.println("DB file Write error");
    #endif
  }
}

void changeMacAddress(void)
{
  uint8_t mac[6] = {0};
  ble_gap_addr_t gap_addr;

  Bluefruit.getAddr(mac);
  /*Serial.print("Address Type: ");
  Serial.println(addr_type);
  for(int i = 0; i < 6; i++)
  {
    Serial.print(mac[i], HEX);
    Serial.print(' ');
  }
  Serial.println();*/
  
  gap_addr.addr_type = BLE_GAP_ADDR_TYPE_RANDOM_STATIC;
  gap_addr.addr[0] = mac[0];
  gap_addr.addr[1] = mac[1];
  gap_addr.addr[2] = mac[2];
  gap_addr.addr[3] = mac[3];
  gap_addr.addr[4] = mac[4];
  if(currMode == SW_CTRL_TYPE2)
  {
    gap_addr.addr[5] = 0xDD;
  }  

  if(Bluefruit.setAddr(&gap_addr))
  {
    #if SERIAL_DEBUG_EN
    Serial.println("MAC change Done");
    #endif
  }
  else
  {
    #if SERIAL_DEBUG_EN
    Serial.println("MAC change Error");
    #endif
  }

  memset(mac, '\0', sizeof(mac));
  Bluefruit.getAddr(mac);
  #if SERIAL_DEBUG_EN
  Serial.println("New MAC: ");
  for(int i = 0; i < 6; i++)
  {
    Serial.print(mac[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
  #endif
}

void hidSpecialKeyPress(uint8_t buff[6])
{
  hid_keyboard_report_t report;
  varclr(&report);

  report.modifier = 0;
  report.keycode[0] = buff[0];
  report.keycode[1] = buff[1];
  report.keycode[2] = buff[2];
  report.keycode[3] = buff[3];
  report.keycode[4] = buff[4];
  report.keycode[5] = buff[5];

  blehid.keyboardReport(BLE_CONN_HANDLE_INVALID, &report);
  delay(20);
  blehid.keyRelease();
  
  memset((uint8_t*)keycodeComboBuff, NULL, sizeof(keycodeComboBuff));
}

void handleSwitchControlKeypress(void)
{  
  if(digitalRead(BUTTON_ONE) == LOW)
  {
    keyPressCnt++;
    BUZZER_ON;
    if(keyPressCnt >= (LONG_PRESS_KEY_TIME / 100))
    {
      if(currMode == SW_CTRL_TYPE1)
        {
          currMode = SW_CTRL_TYPE2;
          #if SERIAL_DEBUG_EN
          Serial.println("Switch Control Type-2 mode Enable");
          #endif
        }
        else
        {
          currMode = SW_CTRL_TYPE1;
          #if SERIAL_DEBUG_EN
          Serial.println("Switch Control Type-1 mode Enable");
          #endif
        }

        // Write updated data into FS
        writeDataToFS();

        BUZZER_OFF;
                
        #if SERIAL_DEBUG_EN
        Serial.println("Reseting MCU");
        delay(2000);
        #endif
        digitalWrite(HARD_RESET_PIN, LOW);       
    }
  }
  else if(digitalRead(BUTTON_TWO) == LOW)
  {
    if(keycheck)
    {
      BUZZER_ON;
      keycheck = 0;
      
      if(currMode == SW_CTRL_TYPE2)
      {
        keycodeComboBuff[0] = TYPE2_KEY2_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }
      else
      {
        keycodeComboBuff[0] = TYPE1_KEY2_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }
      BUZZER_OFF;
    }
  }
  else if(digitalRead(BUTTON_THREE) == LOW)
  {
    if(keycheck)
    {
      BUZZER_ON;
      keycheck = 0;
      
      if(currMode == SW_CTRL_TYPE2)
      {
        keycodeComboBuff[0] = TYPE2_KEY3_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }
      else
      {
        keycodeComboBuff[0] = TYPE1_KEY3_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }                       
     BUZZER_OFF;
    }
  }
  else 
  {    
    if(keyPressCnt)
    {   
      if(currMode == SW_CTRL_TYPE2)
      {        
        keycodeComboBuff[0] = TYPE2_KEY1_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }
      else
      {
        keycodeComboBuff[0] = TYPE1_KEY1_CMD;
        hidSpecialKeyPress((uint8_t*)keycodeComboBuff);
      }
      BUZZER_OFF;
    }
    keyPressCnt = 0;
    keycheck = 1;
  }  
}
