#include <bluefruit.h>
#include "Adafruit_TinyUSB.h"

#include <InternalFileSystem.h>
using namespace Adafruit_LittleFS_Namespace;
File file(InternalFS);
#define MAC_FILENAME    "/button_mac.txt"

#define PAIR_LED 3
uint8_t n_pair_blinks = 3;
uint32_t blink_interval = 500;

bool indicate_pair = false;

uint8_t active_button_hid = HID_KEY_SPACE; // key that will be sent to USB

uint8_t const desc_hid_report[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD()
};

Adafruit_USBD_HID usb_hid(desc_hid_report, sizeof(desc_hid_report), HID_ITF_PROTOCOL_KEYBOARD, 2, false);


uint8_t keycode[6] = { 0 };

bool keyboard_state = false;

uint8_t pair_package[] = {0x03, 0xFF, 0x9C, 0x7C};
uint8_t button_mac_arr[6] = { 0 };

uint8_t button_state = 0;

uint32_t last_update = 0;
uint32_t last_update_diff = 0;

uint16_t received_package;
uint8_t  package_i;
uint8_t  package_size = 10;

uint16_t replay_package = 0;
uint8_t  replay_i = 0;
uint32_t replay_interval = 10;

void get_mac_from_file() {
  Serial.println("Reading saved mac");

  file.open(MAC_FILENAME, FILE_O_READ);
  if(file) {
    Serial.println("File present");
    uint32_t readlen;
    readlen = file.read(button_mac_arr, 6);
    
    if(readlen == 6){
      Serial.print("Saved mac of button: ");
      Serial.printBuffer(button_mac_arr, 6, ':');
      Serial.println();
    } else {
      Serial.println("Unsuccessful reading of mac");
      memset(button_mac_arr, 0, 6);      
    }

    file.close();    
  } else {
    Serial.println("File doesn't exist");
  }
}

void save_mac_to_file() {
  Serial.println("Saving mac into file");
  
  file.open(MAC_FILENAME, FILE_O_WRITE);
  if(file) {
    file.write(button_mac_arr, 6);
    file.close();

    Serial.println("Mac saved");
  } else {
    Serial.println("Failed to open file");
  }
}

void print_full_advertisement(ble_gap_evt_adv_report_t* report) {
    Serial.println();
    Serial.println("Advertisement package: ");
    Serial.println("Timestamp Addr              Rssi Data");
    Serial.printf("%09d ", millis());    
    // MAC is in little endian 
    Serial.printBuffer(report->peer_addr.addr, 6, ':');
    Serial.print(" ");
    Serial.print(report->rssi);
    Serial.print("  ");
    Serial.printBuffer(report->data.p_data, report->data.len, '-');
    Serial.println();  
}

bool target_device(uint8_t* received_mac) {
  for(int i = 0; i<6; i++) {
    if(received_mac[i] != button_mac_arr[i]) {
      return false;
    }
  }

  return true;
}

bool repairing_package(uint8_t* received_package) {
  for(int i = 0; i<4; i++) {
    if(received_package[i] != pair_package[i]) {
      return false;
    }
  }

  return true;
}

void unpack_package(uint16_t received_package) {
  //Serial.printf("Unpacking package: %X\n", received_package);
  for(uint8_t i=0; i<package_size; i++) {
    //Serial.printf("%u; button: ", i);
    while (!usb_hid.ready()) {}
    if(received_package & 1) {
      usb_hid.keyboardReport(0, 0, keycode);
      //Serial.printf("pressed\n");
    } else {
      usb_hid.keyboardRelease(0);
      //Serial.printf("released\n");      
    }
    received_package = received_package >> 1;

    delay(replay_interval);        
  }
}

void scan_callback(ble_gap_evt_adv_report_t* report)
{
  //print_full_advertisement(report);

  if(repairing_package(report->data.p_data)) {
    Serial.println("Got pairing package");
    
    if(!target_device(report->peer_addr.addr)) {
      Serial.print("Old device: ");
      Serial.printBuffer(button_mac_arr, 6, ':');
      Serial.println();
      for(int i=0; i<6; i++){
        button_mac_arr[i] = report->peer_addr.addr[i];
      }
      Serial.print("New device paired: ");
      Serial.printBuffer(button_mac_arr, 6, ':');
      Serial.println();
      
      save_mac_to_file();

      indicate_pair = true;      
    }else {
      Serial.println("Device already paired");
    }
  } else if(target_device(report->peer_addr.addr)) {    
    if(package_i != report->data.p_data[2]) {
      last_update_diff = millis() - last_update;
      last_update = millis();
      Serial.printf("Since last update: %u; Received package: %X-%X-%X\n", last_update_diff, report->data.p_data[2], report->data.p_data[3], report->data.p_data[4]);
      
      package_i = report->data.p_data[2];
      received_package = report->data.p_data[3]<<8;
      received_package += report->data.p_data[4];
      
      //unpack_package(received_package);
    }
  }
  

  // For Softdevice v6: after received a report, scanner will be paused
  // We need to call Scanner resume() to continue scanning
  Bluefruit.Scanner.resume();
}

void setup() 
{
  pinMode(PAIR_LED, OUTPUT);
  
  keycode[0] = active_button_hid;

  Serial.begin(115200);  
  usb_hid.begin();
  InternalFS.begin();
    
  while( !TinyUSBDevice.mounted() ) delay(1);
  
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb
  Serial.println("USB mounted");

  get_mac_from_file();

  Bluefruit.begin(0, 1);
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  Bluefruit.setName("Bluefruit52");

  // Start Central Scan
  Bluefruit.setConnLedInterval(20);
  Bluefruit.Scanner.setRxCallback(scan_callback);
  Bluefruit.Scanner.start(0);
  
  delay(500);
  last_update = millis();
  
  Serial.println("Scanning for button");
    
}

void loop() 
{
  if(indicate_pair) {
    for(int i=0; i<n_pair_blinks; i++){
      digitalWrite(PAIR_LED, HIGH);
      delay(blink_interval);
      digitalWrite(PAIR_LED, LOW);
      delay(blink_interval);
    }

    indicate_pair = false;
  }
  if(replay_i != package_i) {
    replay_i = package_i;
    replay_package = received_package;
    unpack_package(replay_package);    
  }
}
