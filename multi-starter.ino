/*
  Multi Starter 

  Use this sketch as a starter for developing on the Multi AT PCB
  developed by Ace Centre and others. This board is intended to be
  multi functional with a range of inputs and outputs:

  - DB9 connector  with TRACE Center compatible and I2C connections
  - Two switch inputs
  - Three switch outputs
  - "Pass through" master switch output when power removed
  - On/Off Switch
  - User momentary switch
  - Setting potentiometer
  - Buzzer
  - On-board JST-PH 6 way connector with I2C, serial and power
  
*/

const int SWITCH_INPUT_1 = 6;// the number of the pushbutton pin
const int SWITCH_INPUT_2 = 5; 
const int SWITCH_OUTPUT_1 = 9;
const int SWITCH_OUTPUT_2 = 10;
const int SWITCH_OUTPUT_3 = 11;
const int DB9_PIN_1 = A0;
const int DB9_PIN_2 = A1;
const int DB9_PIN_3 = A2;
const int DB9_PIN_4 = A3;
const int DB9_PIN_6 = A4;
const int BUZZER = 13;
const int POT_PIN = 19;  // potentiometer pin used for user setting
const int VBATPIN = A6; // This pin is different on other Feather boards
const int RELAY_DRIVER = 12;  // drives the relay
const long TIMER_INTERVAL = 5000;  // milliseconds

int switch1State = 0;
int switch2State = 0;
int potValue = 0;
float battVoltage = 0;

unsigned long previousMillis = 0; 

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);  // initialise built in LED
  pinMode(SWITCH_INPUT_1, INPUT);  // Switch input 1
  pinMode(SWITCH_INPUT_2, INPUT);  // Switch input 2
  pinMode(POT_PIN, INPUT);  // Setting potentiometer
}

void loop() {
  
  switch1State = digitalRead(SWITCH_INPUT_1);  // Read switch 1 input
  switch2State = digitalRead(SWITCH_INPUT_2);  // Read switch 2 input

  if (switch1State == LOW) {  // if switch 1 pressed
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
    tone(BUZZER, 1000);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
    noTone(BUZZER);
  }

  if (switch2State == LOW) {  // if switch 2 pressed
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
    tone(BUZZER, 1000);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
    noTone(BUZZER);
  }

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= TIMER_INTERVAL) {

    previousMillis = currentMillis;  // 'reset' counter
    
    potValue = analogRead(POT_PIN);  // get potentiometer value
    Serial.print("Pot Setting: ");
    Serial.println(potValue);
    
    battVoltage = ( analogRead(VBATPIN) * 2 * 3.3 ) / 1024;
    Serial.print("Battery Voltage: ");
    Serial.print(battVoltage);
    Serial.println("V");
  }
}
