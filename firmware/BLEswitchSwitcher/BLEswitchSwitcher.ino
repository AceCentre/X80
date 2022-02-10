// Header Files
#include <bluefruit.h>
#include <BLEConnection.h>
#include <Adafruit_LittleFS.h>
#include <InternalFileSystem.h>
#include <Adafruit_NeoPixel.h>    
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

#if usesNeoPixel
  // Neopixel
  #include <Adafruit_NeoPixel.h>    
#else
  // Dotstar
  #include <Adafruit_DotStar.h>
  #include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
  //#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#endif

// No of Neo-pixel LEDs
#define NUMPIXELS      1       

// Variable Declarations
BLEDis bledis;
BLEHidAdafruit blehid;
File dbFile(InternalFS);

#if usesNeoPixel
  //Neopixel
  Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);    
#else 
  Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_DATA, DOTSTAR_CLOCK, DOTSTAR_BRG);
#endif 

SoftwareTimer softTimer;       

const int BUTTON_ONE = KEY_ONE;
const int BUTTON_TWO = KEY_TWO;
const int BUTTON_THREE = KEY_THREE;
const int BUTTON_FOUR = KEY_FOUR;         
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

const unsigned char maxSwapConn = MAXIMUM_SWAP_CONNECTIONS;     
volatile unsigned char currSwapConnIndex = 0;                   
char swapConnDeviceNames[MAXIMUM_SWAP_CONNECTIONS][32] = {0};   
volatile unsigned char flag_blinkNeopixel = 0;                  
volatile unsigned char flag_blinkOnOff = 0;                     
volatile char lastCentral_name[32] = {0};                       
volatile uint8_t flag_manualDisconnection;                      
volatile unsigned long manualDisconnTicks;                      


// Functions Declarations
void startBleAdvertising(void);
void bleConnectCallback(uint16_t conn_handle);
void readDataFromFS(void);
void writeDataToFS(void);
void changeMacAddress(void);
void hidSpecialKeyPress(uint8_t buff[6]);
void handleSwitchControlKeypress(void);
void handleBleConnectionSwap(void);                             
void setNeopixelColor(uint8_t , uint8_t , uint8_t );            
void setNeopixelIndication(unsigned char );                     

// Functions Definations
void setup()
{
  // Begin Serial
  #if SERIAL_DEBUG_EN
  Serial.begin(115200);
  while ( !Serial ) delay(10);                            // temp
  #endif

  // Configure GPIOs  
  #if KEY_IP_TYPE == IP_NO_PULL   
    pinMode(BUTTON_ONE, INPUT);
    pinMode(BUTTON_TWO, INPUT);
    pinMode(BUTTON_THREE, INPUT);
  #else
    pinMode(BUTTON_ONE, INPUT_PULLUP);
    pinMode(BUTTON_TWO, INPUT_PULLUP);
    pinMode(BUTTON_THREE, INPUT_PULLUP);
  #endif
 
  pinMode(BUZZER,OUTPUT);
  BUZZER_OFF;

  pinMode(BUTTON_FOUR, INPUT_PULLUP);         
  pinMode(HARD_RESET_PIN, OUTPUT);
  digitalWrite(HARD_RESET_PIN, HIGH);

  
  // Initialize Neopixel
  // Initialize Neopixel or dotstar
  #if usesNeoPixel
    pixels.begin();                                
    // Set Neopixel Colour
    pixels.setBrightness(50);
    setNeopixelColor(0, 0, 0);    // Off           
  #else
    // Dotstar
    strip.begin();
    strip.setBrightness(50);
    setNeopixelColor(0, 0, 0);    // Off           
    strip.show(); // Initialize all pixels to 'off'
  #endif 

  // Initialize Timer for 500 ms and start it
  softTimer.begin(500, softTimer_callback);
  softTimer.start();
  
  
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

  if(flag_manualDisconnection)
  {
    if((currentMillis > manualDisconnTicks) && (currentMillis - manualDisconnTicks >= LAST_CONNECTION_CHECK_TIMEOUT))
    {
      memset((char*)lastCentral_name, '\0', sizeof(lastCentral_name));
      flag_manualDisconnection = 0;
      currSwapConnIndex = 0;
      
      // Write updated data into FS
      writeDataToFS();
    }
  }
}

void handleSwitchControlKeypress(void)
{  
  if(digitalRead(BUTTON_ONE) == LOW)
  {
    if(keycheck)
    {
      BUZZER_ON;
      keycheck = 0;
      
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
  else if(digitalRead(BUTTON_FOUR) == LOW)           
  {
    if(keycheck)
    {
      BUZZER_ON;
      keycheck = 0;

      #if SERIAL_DEBUG_EN
      Serial.println("BLE Connection Swapping");
      #endif       
      
      handleBleConnectionSwap();
      BUZZER_OFF;
    }
  }
  else 
  { 
    keycheck = 1;
  }  
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
      uint8_t comma_cnt = 0;
      for(uint8_t i = 0; i < sizeof(buff); i++)
      {
        if(buff[i] == ',')
        {
          comma_cnt++;
        }
        else if(buff[i] == '#')
        {
          if(comma_cnt == 6)
          {
            str_valid = 1;
          }
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
      uint8_t comma_cnt = 0;
      char str[32] = {0};
      uint8_t cnt = 0;
      
      tmp_char = atoi(&buff[1]);
      if(tmp_char == SW_CTRL_TYPE1 || tmp_char == SW_CTRL_TYPE2)
      {
        currMode = tmp_char;
      }
      else
      {
        currMode = DEFAULT_MODE_OF_DEVICE;
      }

      for(uint8_t i = 0; i < sizeof(buff); i++)
      {
        if(buff[i] == ',')
        {
          comma_cnt++;
          if(comma_cnt == 2)
          {
            strcpy(swapConnDeviceNames[0], str);
          }
          else if(comma_cnt == 3)
          {
            strcpy(swapConnDeviceNames[1], str);
          }
          else if(comma_cnt == 4)
          {
            strcpy(swapConnDeviceNames[2], str);
          }
          else if(comma_cnt == 5)
          {
            strcpy(swapConnDeviceNames[3], str);
          } 
          else if(comma_cnt == 6)
          {
            strcpy(swapConnDeviceNames[4], str);
          }          

          memset(str, '\0', sizeof(str));
          cnt = 0;
        }
        else if(buff[i] == '#')
        { 
          tmp_char = atoi(str);
          if(tmp_char < MAXIMUM_SWAP_CONNECTIONS)
          {
             currSwapConnIndex = tmp_char;
          }
          else
          {
            currSwapConnIndex = 0;
          }
          
          #if SERIAL_DEBUG_EN
          Serial.println("-------------------------------------------");
          Serial.println("Dev Mode:" + String(currMode));
          Serial.println("Name1:" + String(swapConnDeviceNames[0]));
          Serial.println("Name2:" + String(swapConnDeviceNames[1]));
          Serial.println("Name3:" + String(swapConnDeviceNames[2]));
          Serial.println("Name4:" + String(swapConnDeviceNames[3]));
          Serial.println("Name5:" + String(swapConnDeviceNames[4]));
          Serial.println("Swap Index:" + String(currSwapConnIndex));
          Serial.println("-------------------------------------------");
          #endif
          break;
        }
        else
        {
          str[cnt++] = buff[i];
        }
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
  sprintf(buff, "@%u,%s,%s,%s,%s,%s,%u#",
  currMode, swapConnDeviceNames[0], swapConnDeviceNames[1], swapConnDeviceNames[2], swapConnDeviceNames[3], swapConnDeviceNames[4], currSwapConnIndex);

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
  int i = 0;
  uint16_t connectionHandle = 0;
  BLEConnection* connection = NULL;
  char central_name[32] = {0};
  
  connection = Bluefruit.Connection(conn_handle);  
  connection->getPeerName(central_name, sizeof(central_name));  

  #if SERIAL_DEBUG_EN
  Serial.print("Connection Req from ");
  /*Serial.println((char*)central_name);
  for(i = 0; i < maxSwapConn; i++)
  {
    Serial.print(i);
    Serial.print(": ");
    Serial.print(String((char*)swapConnDeviceNames[i]));
    Serial.print(", ");
  }
  Serial.println();*/
  #endif

  if(flag_manualDisconnection)
  {    
    for(i = 0; i < maxSwapConn; i++)
    {
      if(!strcmp((char*)central_name, (char*)swapConnDeviceNames[i]))
      {
        /*#if SERIAL_DEBUG_EN        
        Serial.print("Name matched in last list, Disconnecting...");
        #endif*/
        
        setNeopixelColor(0, 0, 0);      // Off
        
        if(connection->disconnect())
        {
          #if SERIAL_DEBUG_EN
          Serial.println("Done");
          Serial.println();
          #endif
        }
        else
        {
          #if SERIAL_DEBUG_EN
          Serial.println("ERROR");
          #endif
        }
        break;
      }
    }    
  }
  
  if(i == maxSwapConn || !flag_manualDisconnection)
  {
    #if SERIAL_DEBUG_EN
    Serial.print("New Connection: ");
    Serial.println((char*)central_name);
    Serial.println();
    #endif
    
    flag_blinkNeopixel = 0;
    setNeopixelIndication(currSwapConnIndex);
    memset((char*)lastCentral_name, NULL, sizeof((char*)lastCentral_name));
    memset((char*)swapConnDeviceNames[currSwapConnIndex], NULL, sizeof((char*)swapConnDeviceNames[currSwapConnIndex]));
    memset((char*)swapConnDeviceNames[currSwapConnIndex+1], NULL, sizeof((char*)swapConnDeviceNames[currSwapConnIndex+1]));
    strcpy(swapConnDeviceNames[currSwapConnIndex], (char*)central_name);      
    strcpy((char*)lastCentral_name, (char*)central_name);    

    // Write updated data into FS
    writeDataToFS();
       
    flag_manualDisconnection = 0;
  }
}

void handleBleConnectionSwap(void)      
{
  uint16_t connectionHandle = 0;
  BLEConnection* connection = NULL;
  
  connectionHandle = Bluefruit.connHandle();        
  connection = Bluefruit.Connection(connectionHandle);

  currSwapConnIndex++;
  if(currSwapConnIndex >= maxSwapConn)
  {
    currSwapConnIndex = 0;
  }
  memset(swapConnDeviceNames[currSwapConnIndex], NULL, sizeof(swapConnDeviceNames[currSwapConnIndex]));
 
  // Write updated data into FS
  writeDataToFS();
   
  delay(2000); 
  connection->disconnect();
  #if SERIAL_DEBUG_EN 
  Serial.println("Disconnected");
  #endif
  
  setNeopixelColor(0, 0, 0);      // Off
  flag_blinkNeopixel = 1;

  flag_manualDisconnection = 1;
  manualDisconnTicks = millis();  
}

void setNeopixelColor(uint8_t r, uint8_t g, uint8_t b)    
{
  #if usesNeoPixel
  //Neopoxel
  pixels.clear();
    pixels.setPixelColor(0, pixels.Color(r, g, b));
    pixels.show();
  #else
  //Dotstar
    strip.clear();
    strip.setPixelColor(0, r, g, b);
    strip.show();
  #endif
}

void setNeopixelIndication(unsigned char index)           
{
  if(index == 0)
  {
    setNeopixelColor(0, 0, 255);      // Blue    
  }
  else if(index == 1)
  {
    setNeopixelColor(0, 255, 255);    // Cyan
  }
  else if(index == 2)
  {
    setNeopixelColor(0, 255, 0);      // Green
  }
  else if(index == 3)
  {
    setNeopixelColor(255, 255, 0);    // Yellow
  }
  else if(index == 4)
  {
    setNeopixelColor(255, 128, 0);    // Orange
  }
}

void softTimer_callback(TimerHandle_t xTimerID)       
{
  (void) xTimerID;
  if(flag_blinkNeopixel)
  {
    if(flag_blinkOnOff)
    {
      flag_blinkOnOff = 0;
      setNeopixelColor(0, 0, 0);      // Off
    }
    else
    {
      flag_blinkOnOff = 1;
      setNeopixelIndication(currSwapConnIndex);
    }   
  }  
}
