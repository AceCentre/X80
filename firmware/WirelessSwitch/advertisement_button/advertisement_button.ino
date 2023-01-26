#include <bluefruit.h>

#define LED 3                 // built-in led pin
#define KEY_BUTTON 20          // keyboard button pin
#define PAIR_BUTTON 21         // pair button pin

uint32_t sleep_timeout = 5000; // sleep timeout in ms

uint32_t inactivity_timeout = 500;
uint32_t pairing_timeout = 100;

uint32_t last_action = 0;

uint16_t sample_pack = 0;
uint8_t package_size = 10;
uint8_t package_i = 0;
uint8_t sample_n = 0;
uint16_t set_bit;
uint8_t button_state_prev = 0;
uint8_t button_state = 0;

uint8_t pair_button_state_prev = 0;
uint8_t pair_button_state = 0;

uint32_t read_interval = 10;

uint8_t pair_package[] = {0x9C,0x7C};
uint8_t adv_package[] = {0,0,0};

bool pairing = false;
bool sampling = false;

void update_advertisement_data(){
  adv_package[0] = package_i;
  adv_package[1] = sample_pack>>8;
  adv_package[2] = sample_pack;
  
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
  digitalWrite(LED, LOW);                     
  pinMode(KEY_BUTTON,  INPUT_PULLUP_SENSE);
  sd_power_system_off();
}

void setup() 
{
  last_action = millis();

  set_bit = 1 << package_size;

  // configure KEY_BUTTON as input with a pullup (pin is active low)
  pinMode(KEY_BUTTON, INPUT_PULLUP);
  pinMode(PAIR_BUTTON, INPUT_PULLUP);
  
  Serial.begin(115200);                      
  //while ( !Serial ) delay(10);   // for nrf52840 with native usb

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
  
  Serial.printf("Board is set up after: %u\n", (millis()-last_action));
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  last_action = millis();
}

void loop() {

  button_state = !digitalRead(KEY_BUTTON);
  if(button_state != button_state_prev) {    
    if(!Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.start();      
    }
    Serial.println("Starting button advertisement");
    sampling = true;
    pairing = false;

    last_action = millis();
  }
  button_state_prev = button_state;

  pair_button_state = ! digitalRead(PAIR_BUTTON);
  if(pair_button_state == 1 && pair_button_state_prev == 0) {    
    if(!Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.start();      
    }    
    Serial.println("Starting pairing advertisement");
    pairing = true;
    set_pair_data();    
    sampling = false;

    last_action = millis();
  }
  pair_button_state_prev = pair_button_state;

  
  if(sampling) {
    if((millis() - last_action) > inactivity_timeout && Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.stop();
      sampling = false;
      Serial.println("Stoping button advertisement");      
    }

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
      
      sample_pack = 0;
      sample_n = 0;
      package_i++;
    }
  }

  if(pairing) {
    if((millis() - last_action) > pairing_timeout && Bluefruit.Advertising.isRunning()) {
      Bluefruit.Advertising.stop();
      pairing = false;
      Serial.println("Stoping pairing advertisement");      
    }    
  }
  
  if((millis() - last_action) > sleep_timeout) {
    enter_deepsleep();
  }

  delay(read_interval);
}
