/*
 * This code is an adaptation of the code found here:
  http://jmsarduino.blogspot.com/2009/05/click-for-press-and-hold-for-b.html
  

This program has 2 input buttons that always operate in the same output. Button 1 is the only button that changes the output
   
*/

#define buttonPin1 3 // analog input pin to use as a digital input
#define buttonPin2 4 // analog input pin to use as a digital input

//OUTPUT 1 LEDs
#define ledPin1 6 // digital output pin for LED 1 indicator - blue LED
#define ledPin2 7 // digital output pin for LED 2 indicator - green LED
#define ledPin3 9 // digital output pin for LED 3 indicator - red LED 
#define ledPin4 10 // digital output pin for LED 4 indicator - yellow LED

//OUTPUT 2 LEDs
#define ledPin5 5 // digital output pin for LED 5 indicator - blue LED
#define ledPin6 8 // digital output pin for LED 6 indicator - green LED
#define ledPin7 11 // digital output pin for LED 7 indicator - red LED 
#define ledPin8 12 // digital output pin for LED 8 indicator - yellow LED

#define debounce 20 // ms debounce period to prevent flickering when pressing or releasing the button
#define holdTime 2000 // ms hold period: how long to wait for press+hold event

boolean PressModeOn = true; // this will determine if the program will work in PRESS mode
boolean ReleaseModeOn = false; // this will determine if the program will work in RELEASE mode

// Buttons variables
int button1Val = 0; // value read from button 1
int button1Last = 0; // buffered value of the button1's previous state
int button2Val = 0; // value read from button 2
int button2Last = 0; // buffered value of the button2's previous state

long btn1DnTime; // time the button 1 was pressed down
long btn1UpTime; // time the button 1 was released
long btn2DnTime; // time the button 2 was pressed down
long btn2UpTime; // time the button 2 was released

boolean ignore1Up = false; // whether to ignore the button 1 release because the click+hold was triggered
boolean ignore2Up = false; // whether to ignore the button 2 release because the click+hold was triggered

// LED variables
boolean ledVal1 = false; // state of LED 1
boolean ledVal2 = false; // state of LED 2
boolean ledVal3 = false; // state of LED 3
boolean ledVal4 = false; // state of LED 4
boolean ledVal5 = false; // state of LED 5
boolean ledVal6 = false; // state of LED 6
boolean ledVal7 = false; // state of LED 7
boolean ledVal8 = false; // state of LED 8


// OUTPUT variables
boolean output1 = true; // output 1 set as default
boolean output2 = false; // output 2 set disabled
boolean output3 = false; // output 3 set disabled
boolean output4 = false; // output 4 set disabled

//=================================================

// Output events to trigger 

// ######################## Button 1 events ###########################
void event1()
{
digitalWrite(ledPin1, HIGH);
delay(200);
digitalWrite(ledPin1, LOW);
}

void event2()
{
digitalWrite(ledPin2, HIGH);
delay(200);
digitalWrite(ledPin2, LOW);
}

void event3()
{
digitalWrite(ledPin3, HIGH);
delay(200);
digitalWrite(ledPin3, LOW);
}

void event4()
{
digitalWrite(ledPin4, HIGH);
delay(200);
digitalWrite(ledPin4, LOW);
}

// ######################## Button 2 events ###########################
void event5()
{
digitalWrite(ledPin5, HIGH);
delay(200);
digitalWrite(ledPin5, LOW);
}

void event6()
{
digitalWrite(ledPin6, HIGH);
delay(200);
digitalWrite(ledPin6, LOW);
}

void event7()
{
digitalWrite(ledPin7, HIGH);
delay(200);
digitalWrite(ledPin7, LOW);
}

void event8()
{
digitalWrite(ledPin8, HIGH);
delay(200);
digitalWrite(ledPin8, LOW);
}


//############### Mode change event ###############

void eventModeChange()
{
digitalWrite(LED_BUILTIN, HIGH);
delay (100);
digitalWrite(LED_BUILTIN, LOW);
}

//=================================================


void setup()
{

// Set button input 1 and 2 pins
pinMode(buttonPin1, INPUT);
digitalWrite(buttonPin1, HIGH );
pinMode(buttonPin2, INPUT);
digitalWrite(buttonPin2, HIGH );

// Set LED output pins
pinMode(ledPin1, OUTPUT);
digitalWrite(ledPin1, ledVal1);
pinMode(ledPin2, OUTPUT);
digitalWrite(ledPin2, ledVal2);
pinMode(ledPin3, OUTPUT);
digitalWrite(ledPin3, ledVal3);
pinMode(ledPin4, OUTPUT);
digitalWrite(ledPin4, ledVal4);
pinMode(ledPin5, OUTPUT);
digitalWrite(ledPin5, ledVal5);
pinMode(ledPin6, OUTPUT);
digitalWrite(ledPin6, ledVal6);
pinMode(ledPin7, OUTPUT);
digitalWrite(ledPin7, ledVal7);
pinMode(ledPin8, OUTPUT);
digitalWrite(ledPin8, ledVal8);

pinMode(LED_BUILTIN, OUTPUT);
digitalWrite(LED_BUILTIN, LOW);

}


//=================================================


void loop()
{

// Read the state of the buttons
button1Val = digitalRead(buttonPin1);
button2Val = digitalRead(buttonPin2);

// Test for buttons pressed and store the down time
if (button1Val == LOW && button1Last == HIGH && (millis() - btn1UpTime) > long(debounce))
  {
  btn1DnTime = millis();
  }

if (button2Val == LOW && button2Last == HIGH && (millis() - btn2UpTime) > long(debounce))
  {
  btn2DnTime = millis();
  }


// ****************************************** CODE FOR PRESS MODE **************************************************************

if (PressModeOn == true){

// ################## OUTPUTs 1 - 4 for button 1 #################
  if (button1Val == LOW && button1Last == HIGH && btn1DnTime > long(debounce)){
    if (output1 == true) event1();
    if (output2 == true) event2();
    if (output3 == true) event3();
    if (output4 == true) event4();
    btn1UpTime = millis();
  }

// ################## OUTPUTs 1 - 4 for button 2 #################
  if (button2Val == LOW && button2Last == HIGH && btn2DnTime > long(debounce)){
    if (output1 == true) event5();
    if (output2 == true) event6();
    if (output3 == true) event7();
    if (output4 == true) event8();
    btn2UpTime = millis();
  }

}

// ****************************************** CODE FOR RELEASE MODE *************************************************************

if (ReleaseModeOn == true)
{
  
// ################## OUTPUTs 1 - 4 for button 1 #################
// Test for buttons release and store the up time

    if (button1Val == HIGH && button1Last == LOW && (millis() - btn1DnTime) > long(debounce))
      {
      if (ignore1Up == false && output1 == true) event1();
      else if (ignore1Up == false && output2 == true) event2();
      else if (ignore1Up == false && output3 == true) event3();
      else if (ignore1Up == false && output4 == true) event4();
      else ignore1Up = false;
      btn1UpTime = millis();
      }

// ################## OUTPUTs 1 - 4 for button 2 #################
    if (button2Val == HIGH && button2Last == LOW && (millis() - btn2DnTime) > long(debounce))
      {
      if (ignore2Up == false && output1 == true) event5();
      else if (ignore2Up == false && output2 == true) event6();
      else if (ignore2Up == false && output3 == true) event7();
      else if (ignore2Up == false && output4 == true) event8();
      else ignore2Up = false;
      btn2UpTime = millis();
      }

}

// ############ count btnDnTime ############### repeat the code for button one if want to change output mode with button 2
   if (button2Val == LOW  && (millis() - btn2DnTime) > long(holdTime))
      {
      ignore2Up = true;
      btn2DnTime = millis();
      }
      
// ##################################################### MODE CHANGE EVENTS ################################################


// CHANGE FROM OUTPUT 1 TO OUTPUT 2
        if (button1Val == LOW && (millis() - btn1DnTime) > long(holdTime) && output1 == true)
      {
      eventModeChange();
      ignore1Up = true;
      btn1DnTime = millis();  
      output1 = false; 
      output2 = true;
      output3 = false;
      output4 = false;
      }

 

// CHANGE FROM OUTPUT 2 TO OUTPUT 3
    if (button1Val == LOW && (millis() - btn1DnTime) > long(holdTime) && output2 == true)
      {
      eventModeChange();
      ignore1Up = true;
      btn1DnTime = millis();  
      output1 = false; 
      output2 = false;
      output3 = true;
      output4 = false;
      }

// CHANGE FROM OUTPUT 3 TO OUTPUT 4
    if (button1Val == LOW && (millis() - btn1DnTime) > long(holdTime) && output3 == true)
      {
      eventModeChange();
      ignore1Up = true;
      btn1DnTime = millis();  
      output1 = false; 
      output2 = false;
      output3 = false;
      output4 = true;
      }

// CHANGE FROM OUTPUT 4 TO OUTPUT 1
    if (button1Val == LOW && (millis() - btn1DnTime) > long(holdTime) && output4 == true)
      {
      eventModeChange();
      ignore1Up = true;
      btn1DnTime = millis();  
      output1 = true; 
      output2 = false;
      output3 = false;
      output4 = false;
      }
    
button1Last = button1Val;
button2Last = button2Val;

}
