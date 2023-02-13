#include <bluefruit.h>

//#define DEBUG

#define LED 10
#define KEY_BUTTON 20
#define PAIR_BUTTON 21

uint32_t inactivity_timeout = 500;
uint32_t sleep_timeout = 5000;
uint32_t pairing_timeout = 500;

uint32_t last_action = 0;

uint16_t sample_pack = 0;
uint8_t package_size = 8;
uint8_t package_i = 0;
uint8_t sample_n = 0;
uint16_t set_bit;
uint8_t button_state_prev = 0;
uint8_t button_state = 0;

uint8_t pair_button_state_prev = 0;
uint8_t pair_button_state = 0;

uint32_t read_interval = 30;

uint8_t pair_package[] = {0xFF,0xFF,0x9C,0x7C,0,0,0};
uint8_t adv_package[] =  {0xFF,0xFF,0,0,0,0,0};

bool pairing = false;
bool sampling = false;

void update_advertisement_data(){  
  adv_package[2] = package_i;

  for(uint8_t i=5;i>2;i--) {
    adv_package[i+1] = adv_package[i]; 
  } 

  adv_package[3] = sample_pack;
  
  Bluefruit.Advertising.clearData();    
  Bluefruit.Advertising.addManufacturerData(adv_package, sizeof(adv_package));
}

void set_pair_data(){
  Bluefruit.Advertising.clearData();    
  Bluefruit.Advertising.addManufacturerData(pair_package, sizeof(pair_package));
}

void setup_advertisment(void)
{   
  // Advertising packet
  update_advertisement_data();
  //Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.setType(BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED);

  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 32);    // in units of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  //Bluefruit.Advertising.start(ADV_TIMEOUT);      // Stop advertising entirely after ADV_TIMEOUT seconds 
}

void enter_deepsleep() {
  Serial.println("Sleep timeout. Entering into deepsleep");
  delay(500);
  digitalWrite(LED, LOW);                     
  pinMode(KEY_BUTTON,  INPUT_PULLUP_SENSE);
  sd_power_system_off();
}

void setup() 
{
  last_action = millis();

  set_bit = 1 << package_size;

  // configure BUTTON_PIN as input with a pullup (pin is active low)
  pinMode(KEY_BUTTON, INPUT_PULLUP);
  pinMode(PAIR_BUTTON, INPUT_PULLUP);
  
  Serial.begin(115200);
  #ifdef DEBUG                      
    while ( !Serial ) delay(10);   // for nrf52840 with native usb
  #endif

  Bluefruit.begin();
  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
  
  // Set up advertisement
  setup_advertisment();

  /*
  ble_gap_addr_t button_address = Bluefruit.getAddr();
  Serial.print("Button MAC address: ");
  Serial.printf("%X");
  for(int i=1;i<6;i++) {
    Serial.printf(":%X", button_address.addr[i]);    
  }
  Serial.println();
  */
  
  Serial.printf("Board is set up after: %ums\n", (millis()-last_action));
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  last_action = millis();
}

void loop() {

  button_state = !digitalRead(KEY_BUTTON);
  if(button_state != button_state_prev) {
    sampling = true;
    pairing = false;

    last_action = millis();
  }
  button_state_prev = button_state;

  pair_button_state = ! digitalRead(PAIR_BUTTON);
  if(pair_button_state == 1 && pair_button_state_prev == 0) {    
    set_pair_data();
    
    if(!Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.start();
      Serial.println("Starting pairing advertisement");
    }

    pairing = true;        
    sampling = false;

    last_action = millis();
  }
  pair_button_state_prev = pair_button_state;

  
  if(sampling) {
    if((millis() - last_action) > inactivity_timeout && button_state == 0 && Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.stop();
      Serial.println("Stopping button advertisement");
      for(int i=2;i<7;i++) {
        adv_package[i] = 0;
      }
      
      sampling = false;
            
    } else {
      if(sample_n < package_size) {
        if(sample_n == 0){
          //Serial.printf("Gathering new package\n");
        }
        //Serial.printf("Button state: %u\n", button_state);
        if(button_state) {
          sample_pack = sample_pack | set_bit;
        }
        sample_pack = sample_pack >> 1;
        sample_n++;  
      } else {
        //Serial.printf("Package gathered; id: %X; sample: %X\n", package_i, sample_pack);
        update_advertisement_data();
        
        // start button avertising after package was gathered 
        if(!Bluefruit.Advertising.isRunning()) {
          Bluefruit.Advertising.start();
          Serial.println("Starting button advertisement");
        }

        sample_pack = 0;
        sample_n = 0;
        package_i++;
      }
    }
  }

  if(pairing) {
    if((millis() - last_action) > pairing_timeout && Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.stop();
      pairing = false;
      Serial.println("Stopping pairing advertisement");      
    }    
  }

  #ifndef DEBUG
  if((millis() - last_action) > sleep_timeout && button_state == 0) {
    enter_deepsleep();
  }
  #endif
  
  delay(read_interval);
}
