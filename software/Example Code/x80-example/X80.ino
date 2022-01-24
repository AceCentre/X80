
// Header Files
#include "X80Config.h"
#include "pitches.h"
#include <Adafruit_DotStar.h>
#include <EasyBuzzer.h>

// Variable declarations

// An alternative way of defining the switches

const int switchCount = 5;
uint8_t inputSwitches[switchCount] = { SWITCH_INPUT_ONE, SWITCH_INPUT_TWO, SWITCH_INPUT_THREE, SWITCH_INPUT_FOUR, SWITCH_INPUT_FIVE };

byte switchState[switchCount] = { LOW, LOW, LOW, LOW, LOW };

const int switchInterval = 100;    //number if milliseconds between switch readings. Also allows for debounce


int switch1State = 0;
int switch2State = 0;
int potValue = 0;
float battVoltage = 0;
int switchValue = 0;


//Dotstar LED definitions
// There is only one pixel on the board
#define NUMPIXELS 1 

//Use these pin definitions for the ItsyBitsy M4
#define DATAPIN    8
#define CLOCKPIN   6

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

// Define colours for DotStar LED

uint32_t colour_ace_light_blue = strip.Color(106, 157, 199);
uint32_t colour_ace_dark_blue = strip.Color(0, 83, 127);
uint32_t colour_ace_green = strip.Color(171, 204, 89);
uint32_t colour_ace_orange = strip.Color(225, 136, 0);
uint32_t colour_red = strip.Color(255, 0, 0);
uint32_t colour_green = strip.Color(0, 255, 0);
uint32_t colour_blue = strip.Color(0, 0, 255);
uint32_t colour_orange = strip.Color(255, 165, 0);

// Ace Sound played on startup
int aceSound[] = { NOTE_AS6, NOTE_A6, NOTE_F5, NOTE_CS5, NOTE_D5, NOTE_F5, NOTE_AS4 }; // notes in the melody:
int aceSoundNoteDurations[] = { 8, 4, 4, 16, 4, 4, 4, 4 }; // note durations: 4 = quarter note, 8 = eighth note, etc.:

int buttonState = 0;


unsigned long currentMillis = 0;          // stores the value of millis() in each iteration of loop()
unsigned long previousButtonMillis = 0;   // time when button press last checked.

int sensorValue = 0;
int outputValue = 0;

void setup() {

  
  Serial.begin(9600);

  //playAceSound();

  //Set the Easy Buzzer Pin
  EasyBuzzer.setPin(BUZZER_PIN);

// Dotstar stuff
  strip.begin(); // Initialize pins for output
  strip.setBrightness(80);
  strip.show();  // Turn all LEDs off ASAP

  pinMode(LED_BUILTIN, OUTPUT);  // initialise built in LED
  digitalWrite(LED_BUILTIN, LOW);

  // Temporary setup of input switch
    // initialize the LED pin as an output:
    // initialize the pushbutton pin as an input:
  pinMode(SWITCH_INPUT_ONE, INPUT);     // Only this inout has it's own pull up resistor
  pinMode(SWITCH_INPUT_TWO, INPUT_PULLUP);
  pinMode(SWITCH_INPUT_THREE, INPUT_PULLUP);
  pinMode(SWITCH_INPUT_FOUR, INPUT_PULLUP);
  pinMode(SWITCH_INPUT_FIVE, INPUT_PULLUP);  // Ned to check this one's working with a splitter

  pinMode(SWITCH_OUTPUT_ONE, OUTPUT);  // initialise built in LED
  pinMode(SWITCH_OUTPUT_TWO, OUTPUT);  // initialise built in LED
  pinMode(SWITCH_OUTPUT_THREE, OUTPUT);  // MIGHT BE THIS LINE THAT'S CAUSING THE PROBLEM
  pinMode(SWITCH_OUTPUT_FOUR, OUTPUT);  // initialise built in LED   
        
  pinMode(RELAY_DRIVER, OUTPUT); 
  
  //digitalWrite(RELAY_DRIVER, HIGH);

  SWITCH_OUTPUT_ONE_OFF;
  SWITCH_OUTPUT_TWO_OFF;
  SWITCH_OUTPUT_THREE_OFF;
  SWITCH_OUTPUT_FOUR_OFF;

  SWITCH_ONE_PASSTHROUGH_DISCONNECT;


            //potValue = analogRead(POT_PIN);  // get potentiometer value
          //Serial.print("Pot Setting: ");
          //Serial.println(potValue);
   
}

void loop() {

  currentMillis = millis();

  readSwitches();
  actionOutputs();
  
  EasyBuzzer.update();

  outputValue = analogRead(POT_PIN);
  Serial.println(outputValue);

//String switchArray[switchCount] = { "A4", "A3", "A2", "A1", "A0" };

/*
  // read the state of the pushbutton value:
  buttonState = digitalRead(switchArray[1]);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, HIGH);
  }
*/





  
}

void ledTurnOn(uint32_t colour) {

  //strip.show(); // Update strip with new contents
  strip.setPixelColor(0, strip.gamma32(colour));
  strip.show();
 
}

void ledTurnOff() {

  strip.setPixelColor(0, 0, 0, 0);
  strip.show(); // Update strip with new contents  
  
}


void audibleFeedback (int type) {

  switch (type) {
    case AUDIBLE_INFORMATION:
      tone(BUZZER_PIN, 925);
      break;
    case AUDIBLE_WARNING:
      tone(BUZZER_PIN, 500);
      break;
    default:
      tone(BUZZER_PIN, 500);
      break;
  }
}

void playAceSound () {

  // This is the Ace sound
  
  /*
  for (int thisNote = 0; thisNote < 8; thisNote++) {              // iterate over the notes of the melody:

    int noteDuration = 1000 / aceSoundNoteDurations[thisNote];    // calculate the note duration, e.g. quarter note = 1000 / 4
    tone(BUZZER_PIN, aceSound[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.13;                  // to distinguish the notes specify pause
    delay(pauseBetweenNotes);
    noTone(BUZZER_PIN);                                           // stop the tone playing:

  }
  */
  
}

void actionOutputs() {

  for (byte j = 0; j < switchCount; j++){

    if (switchState[j] == LOW)  { // This is not working for switch 5, needs a splitter otherwise this one is short circuited. ??Check

      // Complete the action depending on which switch is pressed
      switch (j) {
        case 0:
          // do this thing while switch 1 is pressed
          Serial.println("Switch 1 pressed");
          SWITCH_OUTPUT_ONE_ON;
          break;
        case 1:
          // do this thing while switch 2 is pressed
          Serial.println("Switch 2 pressed");
          EasyBuzzer.singleBeep(500, 200);
          break;
        case 2:
          // do this thing while switch 3 is pressed
          Serial.println("Switch 3 pressed");
          ledTurnOn(colour_blue);
          //potValue = analogRead(POT_PIN);  // get potentiometer value
          //Serial.print("Pot Setting: ");
          //Serial.println(potValue);
          break;
        case 3:
          // do this thing while switch 4 is pressed
          playAceSound();
          break;          
        case 4:
          // do this thing while switch 5 is pressed
          break;
      }
      
    } else {
    // Do this while the switch is released

      switch (j) {
        case 0:
          SWITCH_OUTPUT_ONE_OFF;
          // do this thing while switch 1 is released
          break;
        case 1:
          // do this thing while switch 2 is released
          break;
        case 2:
          // do this thing while switch 3 is released
          ledTurnOff();
          break;
        case 3:
          // do this thing while switch 4 is released
          break;          
        case 4:
          // do this thing while switch 5 is released
          break;
      }

      
    }
    
  }




  
}




//========================================

void readSwitches() {

  if (millis() - previousButtonMillis >= switchInterval) {
    
    byte n = 0;
    for (byte i = 0; i <= switchCount; i++) {

    
    
      buttonState = digitalRead(inputSwitches[i]);
  
      // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
      if (buttonState == LOW) {
          // turn LED on:
        //digitalWrite(LED_BUILTIN, LOW);
        //t = "Switch " + i + " on";
        //Serial.print("Switch ");
        //Serial.print(i);
        //Serial.println(" on blah blah blah blah blah");
        switchState[i] = LOW; //switch has been pressed
        previousButtonMillis += switchInterval;
      } else {
        // turn LED off:
        //Serial.print("Switch ");
        //Serial.print(i);
        //Serial.println(" off");
        switchState[i] = HIGH;  // switch has been pressed
      }
    }
  }







      // this only reads the button state after the button interval has elapsed
      //  this avoids multiple flashes if the button bounces
      // every time the button is pressed it changes buttonLed_State causing the Led to go on or off
      // Notice that there is no need to synchronize this use of millis() with the flashing Leds

/*
  
  if (millis() - previousButtonMillis >= buttonInterval) {

    if (digitalRead(buttonPin) == LOW) {
      buttonLed_State = ! buttonLed_State; // this changes it to LOW if it was HIGH 
                                           //   and to HIGH if it was LOW
      previousButtonMillis += buttonInterval;
    }
  }
*/
}
